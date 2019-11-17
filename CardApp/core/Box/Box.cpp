#include <iostream>
#include "Box.h"

using std::cout;
using std::endl;

char Box::getToken() const {
    return token;
}

size_t Box::getTop() const {
    return top;
}

size_t Box::getBottom() const {
    return bottom;
}

size_t Box::getLeft() const {
    return left;
}

size_t Box::getRight() const {
    return right;
}

Box& Box::setToken(char c) {
    token = c;
    return *this;
}

Box& Box::setTop(size_t width) {
    top = width;
    return *this;
}

Box& Box::setBottom(size_t width) {
    bottom = width;
    return *this;
}

Box& Box::setLeft(size_t width) {
    left = width;
    return *this;
}

Box& Box::setRight(size_t width) {
    right = width;
    return *this;
}

Box& Box::test() {
    cout << "this is a box test" << endl;
    return *this;
}

//int main() { Box().test(); }
