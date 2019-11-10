#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cassert>
#include "./Line.h"

using std::string;
using std::list;
using std::cout;
using std::endl;
using std::copy;
using std::prev;

// aliases
using OStringStream = std::ostringstream;
using IStringStream = std::istringstream;
using IStringStreamIt = std::istream_iterator<string>;
using OStringStreamIt = std::ostream_iterator<string>;

size_t Line::width() const {
  size_t w = 0; 
  for (auto s: words)
    w += s.size();
  w += words.size() - 1; // spaces
  w += border.getLeft() + padding.getLeft() + padding.getRight() + border.getRight();
  return w;
}

size_t Line::height() const {
  return border.getTop() + padding.getTop() + padding.getBottom() + border.getBottom() + 1;
}

Line& Line::setBorderTop(size_t width) { 
    border.setTop(width); 
    return *this;
}

Line& Line::setBorderBottom(size_t width) {
    border.setBottom(width);
    return *this;
}

Line& Line::setBorderLeft(size_t width) {
    border.setLeft(width);
    return *this;
}

Line& Line::setBorderRight(size_t width) {
    border.setRight(width);
    return *this;
}

// no trailing newline
string drawLine(char token, size_t length, size_t thickness) {
    OStringStream oss;
    while (thickness--) {
        oss << string(length, token) << '\n';
    }
    return oss.str();
}

string Line::drawBorderTop() const { return drawLine(border.getToken(), width(), border.getTop()); }

string Line::drawBorderBottom() const { return drawLine(border.getToken(), width(), border.getBottom()); } 

string Line::drawBorderLeft() const { return string(border.getLeft(), border.getToken()); }

string Line::drawBorderRight() const { return string(border.getRight(), border.getToken()); }

Line& Line::setPaddingTop(size_t width) {
    padding.setTop(width);
    return *this;
}

Line& Line::setPaddingBottom(size_t width) {
    padding.setBottom(width);
    return *this;
}

Line& Line::setPaddingLeft(size_t width) {
    padding.setLeft(width);
    return *this;
}

Line& Line::setPaddingRight(size_t width) {
    padding.setRight(width);
    return *this;
}

string Line::toString() {
    OStringStream oss;
    OStringStreamIt osi (oss, " ");
    copy(words.begin(), prev(words.end()), osi); // copy leaves a trailing space
    oss << words.back(); // ... so append the last element separately
    return oss.str();
}

string Line::toFormattedString() {
  OStringStream oss;

  oss << drawBorderTop();
  oss << string(padding.getTop(), '\n');
  oss << drawBorderLeft();
  oss << string(padding.getLeft(), ' ');

  oss << toString();

  oss << string(padding.getRight(), ' ');
  oss << drawBorderRight();
  oss << string(padding.getBottom(), '\n');
  if (border.getBottom())
    oss << '\n' << drawBorderBottom();

  return oss.str();
}

Line::Line() {} 

Line::Line(string s) {
  IStringStream iss(s);
  words = list<string>(IStringStreamIt(iss), IStringStreamIt());
}

Line& Line::pushBack(string s) {
  words.push_back(s);
  return *this;
}

string Line::popBack() {
  string s = words.back();
  words.pop_back();
  string prop {s};
  return prop;
}

Line& Line::pushFront(string s) {
  words.push_front(s);
  return *this;
}

string Line::popFront() {
  string s = words.front();
  words.pop_front();
  return s;
}

void Line::clear() { words.clear(); }

Line& Line::test() {
  string s {"It's a wonderful life."};
  Line l {s};
  cout << "|" + l.toFormattedString() + "|" << endl;
  return *this;
}

//int main() { Line().test(); }
