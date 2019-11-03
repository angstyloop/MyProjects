#include <iostream>
#include <cstdlib>
#include "Card.h"

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::getline;
using std::stringstream;

Card::Card() {}

Card::Card(int w): width(w) {}

Card::Card(string f, string b): front(f, width), back(b, width) {}

int Card::getWidth() const {
  return width;
}

void Card::setWidth(int w) {
  width = w; 
}

string Card::getFront() const {
  return front.toString();
}

void Card::setFront(string s) {
  front = Text(s, width);
}

string Card::getBack() const {
  return back.toString();
}

void Card::setBack(string s) {
  back = Text(s, width);
}

string Card::getInputString(string prompt) {
  cout << prompt << '\n';
  cout << string(2, ' ') << '\n';
  string s;
  getline(cin, s);
  cout << '\n';
  cout << endl;
  return s;
}

Card& Card::setFrontFromInput() {
  setFront(getInputString("Front"));
  return *this;
}

Card& Card::setBackFromInput() {
  setBack(getInputString("Back"));
  return *this;
}

Card& Card::showFront() {
  cout << getFront() << endl;
  return *this;
}

Card& Card::showBack() {
  cout << getBack() << endl;
  return *this;
}

Card& Card::writeToFile() {
    ofstream os ("temp.txt", std::ios_base::trunc);
    auto cout_buf = cout.rdbuf(os.rdbuf());
    showFront().showBack();
    cout.rdbuf(cout_buf);
    return *this;
}

Card& Card::test() {
 Card c = Card().setFrontFromInput().setBackFromInput().writeToFile();
 //cout << c.getFront() << endl;
 ifstream is ("temp.txt");
 cout << is.rdbuf();
 return *this;
}

//int main() { Card().test(); }
