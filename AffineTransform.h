// Author: Michael Terry

#ifndef __AFFINETRANSFORM__
#define __AFFINETRANSFORM__

#include "matrix/stat_smatrix.h"
typedef CwMtx::CWTSSquareMatrix<3> SquareMatrix;
typedef CwMtx::CWTSMatrix<3,1> PointVector;

#include "Point.h"
#include "Rectangle.h"

#define PI 3.14159265358979323846

namespace cs349
{

  /**
   * Object for representing affine transforms and applying them to
   * points.
   * AffineTransform objects are immutable.
   */
  class AffineTransform
  {
  private:
    SquareMatrix transform;
	
  public:

    /**
     * Creates an affine transform that is the identity matrix.
     */
    AffineTransform();
    /**
     * Creates an affine transform with the given matrix.
     */
    AffineTransform(const SquareMatrix & m);
    /**
     * Copy constructor for AffineTransform
     */
    AffineTransform(const AffineTransform & t);

    /**
     * Creates an affine transform that is the identity matrix.
     */
    static AffineTransform MakeIdentity();

    /**
     * Creates an affine transform representing a rotation matrix.
     * @param theta The angle, in radians
     */
    static AffineTransform MakeRotation(double theta);

    /**
     * Creates an affine transform representing a rotation matrix.
     * @param degrees The angle, in degrees
     */
    static AffineTransform MakeRotationInDegrees(double degrees);

    /**
     * Creates an affine transform representing a scale matrix.
     * @param scaleX The amount to scale along x axis
     * @param scaleY The amount to scale along y axis
     */
    static AffineTransform MakeScale(double scaleX, double scaleY);

    /**
     * Creates an affine transform representing a translation matrix.
     * @param translateX The amount to translate along the x axis
     * @param translateY The amount to translate along the y axis
     */
    static AffineTransform MakeTranslation(double translateX, double translateY);

    /**
     * Multiplies this matrix by the one passed in and returns the
     * result as a new AffineTransform object. The order of matrix
     * multiplications is precisely as expected by this operator:
     *
     * \code
     * this_matrix * right
     * \endcode
     *
     */
    AffineTransform operator * (const AffineTransform & right) const;

    /**
     * Multiplies this AffineTransform and the point provided. The
     * point is turned into a column vector using homogenous
     * coordinates before multiplication, and is equivalent to:
     *
     * \code
     * this_matrix * point
     * \endcode
     */
    Point operator * (const Point & point) const;
    
    /**
     * Returns the inverse of this matrix. You probably won't need
     * this (and if you think you do, ask a TA or instructor).
     */
    AffineTransform GetInverse() const;

    /**
     * Returns the underlying matrix of this AffineTransform. You
     * probably won't need this (and if you think you do, ask a TA or
     * instructor).
     */
    SquareMatrix GetMatrix() const;
  };

}

#endif /* __AFFINETRANSFORM__ */
