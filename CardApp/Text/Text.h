#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
#include "../Line/Line.cpp"

using std::vector;
using std::ostringstream;
using std::istringstream;
using std::ostream_iterator;
using std::istream_iterator;
using std::string;
using std::list;
using std::cin;
using std::cout;
using std::endl;
using std::copy;
using std::advance;

struct Indent {
    size_t first = 4;
    size_t hanging = 1;
};

class Text {
  Indent indent;
  size_t width;
  vector<Line*> linePtrVec; 
public:
  ~Text();
  Text(); 
  Text(Text&);
  Text(Text&&);
  Text& operator=(Text&);
  Text& operator=(Text&&);
  Text(string text, size_t width); 
  string toString() const;
  string toFormattedString() const;
  Text& test();
};
