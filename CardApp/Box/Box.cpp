#include "Box.h"

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
