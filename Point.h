// Author: Michael Terry

#ifndef __POINT__
#define __POINT__

#include <iostream>
#include <string>

using namespace std;

namespace cs349
{

  /**
   * A rudimentary point class
   */
  class Point
  {
  public:
    double x;
    double y;
    Point();
    Point(double x, double y);
    virtual ~Point();
    string ToString() const;
  };

  ostream & operator << (ostream &os, const Point & p);
}

#endif /* __POINT__ */
