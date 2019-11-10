#include <fstream>
#include <iostream>
#include <cstdlib>
#include "../Text/Text.cpp"
#include "../Deck/Deck.h"

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::string;
using std::shared_ptr;

class Card {
private:
  string name = "";
  string wdir = "/Users/sean/projects/CardApp/cards/example/";
  static int count;
  int width = 50;
  string id = "";
  Text front;
  Text back;
public:
  Card();
  Card(int w);
  Card(string front, string back);
  Card(shared_ptr<Deck>);

  string getId() const;
  Card& setId(string);
  string getName() const;
  Card& setName(string name);
  int getWidth() const;
  void setWidth(int);
  string getFront() const;
  string getFormattedFront() const;
  void setFront(string);
  string getBack() const;
  string getFormattedBack() const;
  void setBack(string);
  string getInputString(string prompt = "");
  Card& setFrontFromInput();
  Card& setBackFromInput();

  Card& showFront();
  Card& showBack();

  Card& writeFrontToFile();
  Card& writeBackToFile();
  Card& writeFormattedFrontToFile();
  Card& writeFormattedBackToFile();

  Card& readFileIntoFront(string filename);
  Card& readFileIntoBack(string filename);

  Card& test();
};
