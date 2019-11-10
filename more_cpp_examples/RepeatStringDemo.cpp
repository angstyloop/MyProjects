#include <sstream>

using std::string;
using std::ostringstream;
using std::ostream_iterator;

string repeatString(string s, unsigned n) {
    ostringstream oss;
    fill_n(ostream_iterator<string>(oss, " "), n, s);
    return oss.str();
}


