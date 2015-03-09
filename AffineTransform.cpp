#include "AffineTransform.h"
#include <math.h>
#include <iostream>
using namespace std;
namespace cs349
{

  AffineTransform::AffineTransform()
  {
    this->transform.makeUnity();
  }

  AffineTransform::AffineTransform(const SquareMatrix & m)
  {
    this->transform = m;
  }

  AffineTransform::AffineTransform(const AffineTransform & t)
  {
    this->transform = t.transform;
  }


  AffineTransform AffineTransform::MakeIdentity()
  {
    return AffineTransform();
  }

  SquareMatrix AffineTransform::GetMatrix() const
  {
    return this->transform;
  }

  AffineTransform AffineTransform::MakeRotation(double theta)
  {
    // We assume multiplication of a point represented as a column vector.
    AffineTransform t;
    double cosTheta = cos(theta);
    double sinTheta = sin(theta);
    t.transform[0][0] = cosTheta;
    t.transform[0][1] = -1 * sinTheta;
    t.transform[1][0] = sinTheta;
    t.transform[1][1] = cosTheta;
    return t;
  }

  AffineTransform AffineTransform::MakeRotationInDegrees(double degrees) {
    return MakeRotation(degrees / 180.0 * PI);
  }

  AffineTransform AffineTransform::MakeScale(double scaleX, double scaleY)
  {
    AffineTransform t;
    t.transform[0][0] = scaleX;
    t.transform[1][1] = scaleY;
    return t;
  }

  AffineTransform AffineTransform::MakeTranslation(double translateX, double translateY)
  {
    AffineTransform t;
    t.transform[0][2] = translateX;
    t.transform[1][2] = translateY;
    return t;
  }

  AffineTransform AffineTransform::operator * (const AffineTransform & r) const {
    return AffineTransform(this->transform * r.transform);
  }

  Point AffineTransform::operator * (const Point & p) const
  {
    PointVector pointVector;
    pointVector[0][0] = p.x;
    pointVector[1][0] = p.y;
    pointVector[2][0] = 1;
    PointVector transformedPoint = this->transform * pointVector;
	
    return Point(transformedPoint[0][0], transformedPoint[1][0]);
  }

  AffineTransform AffineTransform::GetInverse() const
  {
    SquareMatrix t = this->transform;
    t.makeInverse();
    return AffineTransform(t);
  }
}
