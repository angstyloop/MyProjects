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

string Card::getFormattedFront() const {
  return front.toFormattedString();
}

void Card::setFront(string s) {
  front = Text(s, width);
}

string Card::getBack() const {
    return back.toString();
}

string Card::getFormattedBack() const {
  return back.toFormattedString();
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
  cout << getFormattedFront() << endl;
  return *this;
}

Card& Card::showBack() {
  cout << getFormattedBack() << endl;
  return *this;
}

Card& Card::writeFrontToFile() {}

Card& Card::writeBackToFile() {}

Card& Card::writeFormattedFrontToFile() {
    ofstream ofs (id + "_front.txt", std::ios_base::trunc);
    ofs << getFormattedFront();
    return *this;
}

Card& Card::writeFormattedBackToFile() {
    ofstream ofs(id + "_back.txt", std::ios_base::trunc);
    ofs << getFormattedBack();
    return *this;
}

Card& Card::test() {
 Card c = Card().setFrontFromInput().setBackFromInput().writeFormattedFrontToFile().writeFormattedBackToFile();
 ifstream is_front ("_front.txt"), is_back("_back.txt");
 cout << is_front.rdbuf() << "\n\n" << is_back.rdbuf();
 return *this;
}

//int main() { Card().test(); }
