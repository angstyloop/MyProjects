#include <string>
#include <algorithm>
#include <list>
#include "../Box/Box.h"

using std::list;
using std::string;

struct Padding {
    size_t top = 0;
    size_t bottom = 0;
    size_t left = 0;
    size_t right = 0;
    Padding() {}
    Padding(size_t t, size_t b, size_t l, size_t r): top(t), bottom(b), left(l), right(r) {}
};

struct Border {
    char token = '*';
    size_t top = 0;
    size_t bottom = 0;
    size_t left = 0;
    size_t right = 0;
    Border() {}
    Border(char c): token(c) {}
    Border(size_t t, size_t b, size_t l, size_t r): top(t), bottom(b), left(l), right(r) {}
};

class Line {
  list<string> words;
  Box padding {' '};
  Box border {'*'};
 public:
  size_t width() const;
  size_t height() const;
  Line& setBorderTop(size_t);
  Line& setBorderBottom(size_t);
  Line& setBorderLeft(size_t);
  Line& setBorderRight(size_t);
  Line& setPaddingTop(size_t);
  Line& setPaddingBottom(size_t);
  Line& setPaddingLeft(size_t);
  Line& setPaddingRight(size_t);
  string drawBorderTop() const;
  string drawBorderBottom() const;
  string drawBorderLeft() const;
  string drawBorderRight() const;
  string toString();
  string toFormattedString();
  Line();
  Line(string);
  Line& pushBack(string);
  string popBack();
  Line& pushFront(string);
  string popFront();
  void clear();
  Line& test();
};
