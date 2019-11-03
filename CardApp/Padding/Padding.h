#include <iostream>
class Padding {
 private:
  char c;
 public:
  size_t left = 0;
  size_t right = 0;
  size_t top = 0;
  size_t bottom = 0;

  Padding(): c(' ') {}

  Padding(char c): c(c) {};

  Padding(size_t l, size_t r, size_t t, size_t b) : 
      left{l}, 
      right{r}, 
      top{t}, 
      bottom{b} 
  { }

  char getChar() const { return c; }

};
