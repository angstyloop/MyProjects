#include <iostream>
#include <cstdlib>
#include "card.h"

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::getline;

/* private methods */

string Card::format(string s) const {
  string t = "";
  return t;
}

/* public */
int Card::getWidth() const {
  return width;
}

void Card::setWidth(int w) {
  width = w; 
}

string Card::getFront() const {
  return format(front);
}

void Card::setFront(string s) {
  front = s;
}

string Card::getBack() const {
  return format(back);
}

void Card::setBack(string s) {
  back = s;
}

int main() {
 return 0; 
}
