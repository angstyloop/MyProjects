#include <cstdlib>

class Box {
  public:
    char token = '.';
    size_t top = 0;
    size_t bottom = 0;
    size_t left = 0;
    size_t right = 0;
    Box(char c): token(c) {}
    Box(char c, size_t t, size_t b, size_t l, size_t r): top(t), bottom(b), left(l), right(r) {}
    Box& setTop(size_t);
    Box& setBottom(size_t);
    Box& setLeft(size_t);
    Box& setRight(size_t);
 };
