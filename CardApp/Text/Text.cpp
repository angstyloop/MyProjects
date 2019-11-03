#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
#include "./Text.h"

using std::string;
using std::list;
using std::cout;
using std::endl;
using std::copy;

// aliases
using OStringStream = std::ostringstream;
using IStringStream = std::istringstream;
using IStringStreamIt = std::istream_iterator<string>;
using OStringStreamIt = std::ostream_iterator<string>;

//dtr
Text::~Text() {
    for (auto linePtr : linePtrVec)
        delete linePtr;
}

//default ctr
Text::Text() {}

//cp ctr
Text::Text(Text& o) : width(o.width) {
    for (auto linePtr : o.linePtrVec)
        linePtrVec.push_back(new Line(*linePtr));
}

//mv ctr
Text::Text(Text&& o): width(std::move(o.width)), linePtrVec(std::move(o.linePtrVec)) {}

//cp assign
Text& Text::operator=(Text& o) {
    for (auto linePtr : linePtrVec) 
        delete linePtr;
    linePtrVec.clear();
    for (auto linePtr: o.linePtrVec)
        linePtrVec.push_back(linePtr);
    width = o.width;
    return *this;
}

//mv assign
Text& Text::operator=(Text&& o) {
    for (auto linePtr : linePtrVec)
        delete linePtr;
    linePtrVec = std::move(o.linePtrVec);
    width = o.width;
    return *this;
}

// ctr from string and size. Parse string of text into lines. If new line created from 
// adding next word would be too long, start a new line with it instead.
Text::Text(string text, size_t w): width{w} {
  istringstream iss(text);
  string tmpStr;
  Line* tmpLinePtr = new Line;
  linePtrVec.push_back(tmpLinePtr);
  tmpLinePtr->setBorderTop(1).setBorderLeft(1).setPaddingLeft(indent.first).setPaddingRight(1).setBorderRight(1);
  while (iss >> tmpStr) {
    if ( tmpLinePtr->width() + tmpStr.size() + 1 + 1 <= width) {
      tmpLinePtr->pushBack(tmpStr);
    } else {
        tmpLinePtr->setPaddingRight(width - tmpLinePtr->width());
        tmpLinePtr = (new Line);
        linePtrVec.push_back(tmpLinePtr);
        tmpLinePtr->setBorderLeft(1).setPaddingLeft(indent.hanging).setPaddingRight(1).setBorderRight(1).pushBack(tmpStr);
    }
  }
  tmpLinePtr->setPaddingRight(width - tmpLinePtr->width()).setBorderBottom(1);
}

string Text::toString() const {
    OStringStream oss;
    for_each(linePtrVec.begin(), prev(linePtrVec.end()), [&](Line* linePtr) { 
        oss << linePtr->toString() << '\n'; 
    });
    oss << linePtrVec.back()->toString(); //don't print trailing newline
    return oss.str();
}

string Text::toFormattedString() const {
  OStringStream oss;
  for_each(linePtrVec.begin(), prev(linePtrVec.end()), [&](Line* linePtr) { 
      oss << linePtr->toFormattedString() << '\n';
  });
  oss << linePtrVec.back()->toFormattedString(); // don't print trailing newline.
  return oss.str();
}

string repeatString(string s, unsigned n) {
  OStringStream oss;
  fill_n(OStringStreamIt(oss, " "), n, s);
  return oss.str();
}

Text& Text::test() {
  //should test invariants here. e.g.  
  for (auto linePtr: linePtrVec)
    assert(linePtr->width() <= width);

  //this is just to see what it looks like
  string s = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
  cout << Text(s, 50).toFormattedString() << endl;
  cout << '\n';
  cout << Text(s, 50).toString() << endl;
  return *this;
}

//int main() { Text().test(); }
