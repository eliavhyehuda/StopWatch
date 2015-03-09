// Author: Michael Terry

#include "Point.h"
#include <sstream>

namespace cs349
{

  Point::Point()
  {
    x = 0;
    y = 0;
  }

  Point::Point(double x, double y)
  {
    this->x = x;
    this->y = y;
  }

  Point::~Point()
  {
  }

  string Point::ToString() const {
    stringstream sstream;
    sstream << "Point(x: " << this->x << ", y: " << this->y << ")";
    return sstream.str();
  }

  ostream & operator << (ostream &os, const Point & p)
  {
    os << p.ToString();
    return os;
  }
}
