#include "AffineTransform.h"
#include <math.h>
#include "gtest/gtest.h"

using namespace cs349;

#define ABS_ERR 0.00001

void EqualTransforms(const AffineTransform & l, const AffineTransform & r);
void EqualMatrices(const SquareMatrix & l, const SquareMatrix & r);
void EqualToMatrix(const AffineTransform & t, const SquareMatrix & m);
void EqualPoints(const Point & p1, const Point & p2);
void ValidateIdentity(const AffineTransform & t);
void ValidateRotation(const AffineTransform & t, double theta);
void ValidateScale(const AffineTransform & t, double scale_x, double scale_y);
void ValidateTranslation(const AffineTransform & t, double t_x, double t_y);
void ValidateMultiplication(const AffineTransform & l, const AffineTransform & r, const AffineTransform & expected_m);
void ValidatePointMultiplication(const AffineTransform & t, const Point & in_p, const Point & expected_p);

TEST(AffineTransformTests, IdentityTests) {
  ValidateIdentity(AffineTransform());
  ValidateIdentity(AffineTransform::MakeIdentity());
}

TEST(AffineTransformTests, RotationTests) {
  const int NUM_DIVISIONS = 11;
  for (int i = 0; i <= NUM_DIVISIONS; i++) {
    double theta = 2.0 * PI * i / NUM_DIVISIONS;
    double angle = 360.0 * i / NUM_DIVISIONS;
    ValidateRotation(AffineTransform::MakeRotation(theta), theta);
    ValidateRotation(AffineTransform::MakeRotationInDegrees(angle), theta);
  }
}

TEST(AffineTransformTests, ScaleTests) {
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      ValidateScale(AffineTransform::MakeScale(i, j), i, j);
    }
  }
}

TEST(AffineTransformTests, TranslationTests) {
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      ValidateTranslation(AffineTransform::MakeTranslation(i, j), i, j);
    }
  }
}

TEST(AffineTransformTests, MatrixMultiplicationTests) {
  double          translate_x = 8;
  double          translate_y = 7;
  double          scale_x     = 2;
  double          scale_y     = -3;
  double          theta       = PI / 2;
  AffineTransform identity;
  AffineTransform translation = AffineTransform::MakeTranslation(translate_x, translate_y);
  AffineTransform scale       = AffineTransform::MakeScale(scale_x, scale_y);
  AffineTransform rotation    = AffineTransform::MakeRotation(theta);
  Point           unit_x      = Point(1, 0);
  Point           unit_y      = Point(0, 1);
  Point           unit_neg_x  = Point(-1, 0);
  Point           unit_neg_y  = Point(0, -1);
  Point           origin      = Point(0, 0);
  Point           v11         = Point(1, 1);

  EqualTransforms(identity    , identity * identity);
  EqualTransforms(identity    , identity * identity * identity);
  EqualTransforms(translation , identity * translation);
  EqualTransforms(rotation    , identity * rotation);
  EqualTransforms(scale       , identity * scale);
  EqualTransforms(translation , translation * identity);
  EqualTransforms(rotation    , rotation * identity);
  EqualTransforms(scale       , scale * identity);

  EqualPoints(unit_x, identity * unit_x);

  EqualPoints(Point(translate_x, translate_y), translation * origin);

  EqualPoints(origin, scale * origin);
  EqualPoints(Point(scale_x, scale_y), scale * v11);

  EqualPoints(unit_y     , rotation * unit_x);
  EqualPoints(unit_neg_x , rotation * rotation * unit_x);
  EqualPoints(unit_neg_y , rotation * rotation * rotation * unit_x);
  EqualPoints(unit_x     , rotation * rotation * rotation * rotation * unit_x);

  EqualPoints(Point(translate_x, translate_y), translation * scale * origin);
  EqualPoints(Point(scale_x * translate_x, scale_y * translate_y), scale * translation * origin);
  EqualPoints(Point(scale_x * scale_x, scale_y * scale_y), scale * scale * v11);
}

TEST(AffineTransformTests, CopyConstructorTest) {
  AffineTransform t1 = AffineTransform::MakeRotationInDegrees(45);
  AffineTransform t2 = t1;
  EqualTransforms(t1, t2);
}

void EqualTransforms(const AffineTransform & l, const AffineTransform & r) {
  EqualMatrices(l.GetMatrix(), r.GetMatrix());
}

void EqualMatrices(const SquareMatrix & l, const SquareMatrix & r) {
  EXPECT_EQ(l.getRows(), r.getRows());
  EXPECT_EQ(l.getCols(), r.getCols());
  for (unsigned int row = 0; row < l.getRows(); row++) {
    for (unsigned int col = 0; col < l.getCols(); col++) {
      ASSERT_NEAR(l[row][col], r[row][col], ABS_ERR);
    }
  }
}


void EqualToMatrix(const AffineTransform & t, const SquareMatrix & m) {
  EqualMatrices(t.GetMatrix(), m);
}

void EqualPoints(const Point & p1, const Point & p2) {
  ASSERT_NEAR(p1.x, p2.x, ABS_ERR);
  ASSERT_NEAR(p1.y, p2.y, ABS_ERR);
}



void ValidateIdentity(const AffineTransform & t) {
  SquareMatrix m = t.GetMatrix();
  for (unsigned int row = 0; row < m.getRows(); row++) {
    for (unsigned int col = 0; col < m.getCols(); col++) {
      if (row == col) {
        ASSERT_NEAR(1, m[row][col], ABS_ERR);
      } else {
        ASSERT_NEAR(0, m[row][col], ABS_ERR);
      }
    }
  }
}

void ValidateRotation(const AffineTransform & t, double theta) {
  SquareMatrix m = t.GetMatrix();
  ASSERT_NEAR(cos(theta),  m[0][0], ABS_ERR);
  ASSERT_NEAR(-sin(theta), m[0][1], ABS_ERR);
  ASSERT_NEAR(0,           m[0][2], ABS_ERR);
  ASSERT_NEAR(sin(theta),  m[1][0], ABS_ERR);
  ASSERT_NEAR(cos(theta),  m[1][1], ABS_ERR);
  ASSERT_NEAR(0,           m[1][2], ABS_ERR);
  ASSERT_NEAR(0,           m[2][0], ABS_ERR);
  ASSERT_NEAR(0,           m[2][1], ABS_ERR);
  ASSERT_NEAR(1,           m[2][2], ABS_ERR);
}

void ValidateScale(const AffineTransform & t, double scale_x, double scale_y) {
  SquareMatrix m = t.GetMatrix();
  ASSERT_NEAR(scale_x, m[0][0], ABS_ERR);
  ASSERT_NEAR(0,       m[0][1], ABS_ERR);
  ASSERT_NEAR(0,       m[0][2], ABS_ERR);
  ASSERT_NEAR(0,       m[1][0], ABS_ERR);
  ASSERT_NEAR(scale_y, m[1][1], ABS_ERR);
  ASSERT_NEAR(0,       m[1][2], ABS_ERR);
  ASSERT_NEAR(0,       m[2][0], ABS_ERR);
  ASSERT_NEAR(0,       m[2][1], ABS_ERR);
  ASSERT_NEAR(1,       m[2][2], ABS_ERR);
}

void ValidateTranslation(const AffineTransform & t, double t_x, double t_y) {
  SquareMatrix m = t.GetMatrix();
  ASSERT_NEAR(1,   m[0][0], ABS_ERR);
  ASSERT_NEAR(0,   m[0][1], ABS_ERR);
  ASSERT_NEAR(t_x, m[0][2], ABS_ERR);
  ASSERT_NEAR(0,   m[1][0], ABS_ERR);
  ASSERT_NEAR(1,   m[1][1], ABS_ERR);
  ASSERT_NEAR(t_y, m[1][2], ABS_ERR);
  ASSERT_NEAR(0,   m[2][0], ABS_ERR);
  ASSERT_NEAR(0,   m[2][1], ABS_ERR);
  ASSERT_NEAR(1,   m[2][2], ABS_ERR);
}

void ValidateMultiplication(const AffineTransform & l, const AffineTransform & r, const AffineTransform & expected_m) {
  EqualTransforms(l * r, expected_m);
}

void ValidatePointMultiplication(const AffineTransform & t, const Point & in_p, const Point & expected_p) {
  Point transformed_point = t * in_p;
  EqualPoints(expected_p, t * in_p);
}

