#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "Card.h"

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::getline;
using std::stringstream;
using std::istreambuf_iterator;
using std::shared_ptr;
using std::to_string;

Card::Card() {}

Card::Card(int w): width(w) {}

Card::Card(string f, string b): front(f, width), back(b, width) {}

Card::Card(shared_ptr<Deck> deck){
    id = "c_" + to_string(deck->getNextId());
    deck->pushPath(wdir + "/" + getName());
}

string Card::getId() const {return id;}

Card& Card::setId(string id) {
    this->id = id;
    return *this;
}

string Card::getName() const { return name; }

Card& Card::setName(string name) {
    this->name = name;   
    return *this;
}

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

Card& Card::writeFrontToFile() { 
    ofstream ofs (wdir + id + "_front.txt");
    ofs << getFront();
    return *this; 
}

Card& Card::writeBackToFile() { 
    ofstream ofs (wdir + id + "_back.txt", std::ios_base::trunc);
    ofs << getFormattedBack();
    return *this; 
}

Card& Card::writeFormattedFrontToFile() {
    ofstream ofs (wdir + id + "_formatted_front.txt", std::ios_base::trunc);
    ofs << getFormattedFront();
    return *this;
}

Card& Card::writeFormattedBackToFile() {
    ofstream ofs(wdir + id + "_formatted_back.txt", std::ios_base::trunc);
    ofs << getFormattedBack();
    return *this;
}

Card& Card::readFileIntoFront(string filename) {
    ifstream ifs (filename);
    setFront(string (istreambuf_iterator<char>(ifs), istreambuf_iterator<char>()));
    return *this;
}

Card& Card::readFileIntoBack(string filename) {
    ifstream ifs (filename);
    setBack(string (istreambuf_iterator<char>(ifs), istreambuf_iterator<char>()));
    return *this;
}

Card& Card::test() {
 Card c = Card().setFrontFromInput().setBackFromInput().writeFormattedFrontToFile().writeFormattedBackToFile();
 ifstream ifs_formatted_front (wdir + "_formatted_front.txt");
 ifstream ifs_formatted_back (wdir + "_formatted_back.txt");
 cout << ifs_formatted_front.rdbuf() << "\n\n" << ifs_formatted_back.rdbuf();
 c.readFileIntoFront("foofile.txt");
 c.readFileIntoBack("barfile.txt");
 c.showFront().showBack();
 return *this;
}

//int main() { Card().test(); }
