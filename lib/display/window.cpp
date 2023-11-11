#include "window.h"
#include <iostream>

std::ostream &operator<<(std::ostream &os, const WindowCreateInfo &wci) {
    os << "\"" << wci.title << "\" window has width " << wci.width
       << " and height " << wci.height;
    return os;
}