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
  w += border.left + padding.left + padding.right + border.right;
  return w;
}

size_t Line::height() const {
  return border.top + padding.top + padding.bottom + border.bottom + 1;
}

Line& Line::setBorderTop(size_t width) { 
    border.top = width; 
    return *this;
}

Line& Line::setBorderBottom(size_t width) {
    border.bottom = width;
    return *this;
}

Line& Line::setBorderLeft(size_t width) {
    border.left = width;
    return *this;
}

Line& Line::setBorderRight(size_t width) {
    border.right = width;
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

string Line::drawBorderTop() const { return drawLine(border.token, width(), border.top); }

string Line::drawBorderBottom() const { return drawLine(border.token, width(), border.bottom); } 

string Line::drawBorderLeft() const { return string(border.left, border.token); }

string Line::drawBorderRight() const { return string(border.right, border.token); }

Line& Line::setPaddingTop(size_t width) {
    padding.top =  width;
    return *this;
}

Line& Line::setPaddingBottom(size_t width) {
    padding.bottom =  width;
    return *this;
}

Line& Line::setPaddingLeft(size_t width) {
    padding.left =  width;
    return *this;
}

Line& Line::setPaddingRight(size_t width) {
    padding.right =  width;
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
  oss << string(padding.top, '\n');
  oss << drawBorderLeft();
  oss << string(padding.left, ' ');

  oss << toString();

  oss << string(padding.right, ' ');
  oss << drawBorderRight();
  oss << string(padding.bottom, '\n');
  if (border.bottom > 0) {
    oss << '\n' << drawBorderBottom();
  }

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
