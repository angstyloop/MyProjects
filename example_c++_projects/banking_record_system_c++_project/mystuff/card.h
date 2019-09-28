#include <fstream>
#include <iostream>
#include <cstdlib>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::string;

class Card {
private:
  int width = 50;
  string id = "";
  string front = "";
  string back = "";
  Card() {}
  Card(int w): width(w) {}
  string format(string) const;
public:
  int getWidth() const;
  void setWidth(int);
  string getFront() const;
  void setFront(string);
  string getBack() const;
  void setBack(string);
};
