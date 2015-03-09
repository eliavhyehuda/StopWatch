// Author: Michael Terry

#ifndef __RECTANGLE__
#define __RECTANGLE__

#include <string>

using namespace std;

#include "Point.h"

namespace cs349
{

  /**
   * A rudimentary rectangle class.
   */
  class Rectangle
  {
  public:
    double x;
    double y;
    double width;
    double height;
    
    /**
     * Constructs an empty rectangle (one with x, y, width, and height
     * equal to 0).
     */
    Rectangle();
    Rectangle(double x, double y, double width, double height);
    virtual ~Rectangle();
    
    /**
     * Returns the location of the bottom-right corner of the
     * rectangle.
     */
    Point      GetBottomRight() const;

    /**
     * Returns the location of the top-left corner of the
     * rectangle.
     */
    Point      GetTopLeft() const;

    /**
     * Returns the intersection (common area) of this rectangle and
     * the one passed in. If either rectangle is empty, returns an
     * empty Rectangle.
     */
    Rectangle  GetIntersection(const Rectangle & r) const;

    /**
     * Returns true if either the width or height is <= 0.
     */
    bool IsEmpty() const;

    /**
     * Returns whether a point is in the rectangle, or not. If the
     * rectangle is empty, returns false, regardless of the point
     * passed in.
     */
    bool IsPointInRectangle(const Point & p) const;

    /**
     * Returns whether the two rectangles intersect (overlap) each
     * other. Returns false if either rectangle is empty.
     */
    bool IntersectsRectangle(const Rectangle & r) const;

    string ToString() const;
  };

  ostream & operator << (ostream &os, const Rectangle& r);

}

#endif /* __RECTANGLE__ */
