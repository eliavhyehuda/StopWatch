// $Id: mattest.cpp 166 2008-01-19 19:56:57Z hkuiper $

// CwMtx matrix and vector math library
// Copyright (C) 1999-2000  Harry Kuiper, Will DeVore (template conversion)

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include <stat_cwmtx.h>

// included for a simple test of namespaces
#include <list>

using namespace std;
using namespace CwMtx;

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

void test_static_matrix()
{
  cout << endl << "Start test of static matrices" << endl;

  typedef CWTSMatrix<2, 3> CWSMatrix23;
  typedef CWTSMatrix<3, 2> CWSMatrix32;
  typedef CWTSMatrix<2, 2> CWSMatrix22;

  cout << endl << "CWTSMatrix<r, c, T>" << endl << endl;

  CWSMatrix23 mtx23;
  cout << "mtx23 = CWTSZero<CWSMatrix23>() "
       << (mtx23 = CWTSZero<CWSMatrix23>()) << endl;
  mtx23.fill(1.0);
  cout << "mtx23.fill(1.0) "
       << (mtx23) << endl;

  mtx23[0][0] = 1;
  mtx23[0][1] = 2;
  mtx23[0][2] = 3;
  mtx23[1][0] = 4;
  mtx23[1][1] = 5;
  mtx23[1][2] = 6;
  cout << "mtx23[0][0] = 1; ... mtx23[1][2] = 6; "
       << (mtx23) << endl;
  cout << "mtx23 *= 3.0 "
       << (mtx23 *= 3.0) << endl;

  CWSMatrix32 mtx32;
  cout << "mtx32 = transpose(mtx23) "
       << (mtx32 = transpose(mtx23)) << endl;

  CWSMatrix22 mtx22;
  cout << "mtx22 = CWTSUnity<CWSMatrix22>() "
       << (mtx22 = CWTSUnity<CWSMatrix22>()) << endl;
  cout << "mtx22.storeSum(CWTSUnity<CWSMatrix22>(), CWTSUnity<CWSMatrix22>()) "
       << (mtx22.storeSum(CWTSUnity<CWSMatrix22>(), CWTSUnity<CWSMatrix22>()),
	   mtx22)
       << endl;
  cout << "mtx22 = mtx23*mtx32 "
       << (mtx22 = mtx23*mtx32) << endl;

  mtx23.storeAtPosition(0, 1, mtx22);
  cout << "mtx23.storeAtPosition(0, 1, mtx22) "
       << (mtx23) << endl;

  cout << endl << "CWTSSquareMatrix<r, T>" << endl << endl;

  typedef CWTSSquareMatrix<3> CWSSMatrix3;

  CWSSMatrix3 smtx3;
  cout << "smtx3 = CWTSZero<CWSSMatrix3>() "
       << (smtx3 = CWTSZero<CWSSMatrix3>()) << endl;
  cout << "smtx3 = CWTSUnity<CWSSMatrix3>() "
       << (smtx3 = CWTSUnity<CWSSMatrix3>()) << endl;
  smtx3.fill(1.0);
  cout << "smtx3.fill(1.0) "
       << (smtx3) << endl;
  cout << "smtx3[0][1] = 0.0, transpose(smtx3) "
       << (smtx3[0][1] = 0.0, transpose(smtx3)) << endl;
  smtx3.makeUnity();
  cout << "smtx3.makeUnity() "
       << (smtx3) << endl;
  cout << "smtx3 *= 3.0 "
       << (smtx3 *= 3.0) << endl;
  cout << "inv(smtx3) "
       << (inv(smtx3)) << endl;
  cout << "smtx3*inv(smtx3) "
       << (smtx3*inv(smtx3)) << endl;
  cout << "det(smtx3) "
       << (det(smtx3)) << endl;
  cout << "adj(smtx3) "
       << (adj(smtx3)) << endl;
  cout << "adj(smtx3)/det(smtx3) "
       << (adj(smtx3)/det(smtx3)) << endl;
  cout << "tr(smtx3) "
       << (tr(smtx3)) << endl;

  cout << endl << "CWTSVector<r, T>" << endl << endl;

  typedef CWTSVector<3> CWSVec3;

  CWSVec3 vec3a, vec3b;
  cout << "vec3a = CWTSZero<CWSVec3>() "
       << (vec3a = CWTSZero<CWSVec3>()) << endl;
  vec3a.fill(1.0);
  cout << "vec3a.fill(1.0) "
       << (vec3a) << endl;
  cout << "vec3b = vec3a "
       << (vec3b = vec3a) << endl;
  cout << "vec3a*vec3b "
       << (vec3a*vec3b) << endl;

  typedef CWTSVector<5> CWSVec5;
  CWSVec5 vec5;
  vec5.fill(0);
  cout << "vec5.fill(0) "
       << (vec5) << endl;

  vec5.storeAtRow(2, vec3a);
  cout << "vec5.storeAtRow(2, vec3a) "
       << (vec5) << endl;

  typedef CWTSMatrix<3, 3> CWSMatrix33;

  CWSMatrix33 mtx33;
  mtx33.fill(1.0);
  cout << "mtx33.fill(1.0) "
       << (mtx33) << endl;
  cout << "mtx33*vec3b "
       << (mtx33*vec3b) << endl;
  cout << "norm(vec3a) "
       << norm(vec3a) << endl;

  cout << endl << "CWTSSpaceVector<T>" << endl << endl;

  CWTSSpaceVector<> svec1(1, 0, 0), svec2(0, 1, 0), svec3;
  cout << "svec3 = CWTSZero< CWTSSpaceVector<> >() "
       << (svec3 = CWTSZero< CWTSSpaceVector<> >()) << endl;
  cout << "svec1(1, 0, 0) "
       << (svec1) << endl;
  cout << "svec2(0, 1, 0) "
       << (svec2) << endl;
  cout << "svec1*svec2 "
       << (svec1*svec2) << endl;
  cout << "svec1%svec2 "
       << (svec1%svec2) << endl;
  cout << "mtx33*svec2 "
       << (mtx33*svec2) << endl;
  cout << "(mtx33*svec2)*svec1 "
       << ((mtx33*svec2)*svec1) << endl;
  // This should fail to compile.
//   cout << "(mtx33*svec2)%svec1 "
//        << ((mtx33*svec2)%svec1) << endl;
  cout << "smtx3*svec2 "
       << (smtx3*svec2) << endl;
  cout << "(smtx3*svec2)%svec1 "
       << ((smtx3*svec2)%svec1) << endl << endl;

  cout << "CWTSQuaternion<T>" << endl << endl;

  CWTSQuaternion<> qtn1(1, 2, 3, 4), qtn2(0, 1, 0, 0), qtn3, qtn4(1);
  cout << "qtn3 = CWTSZero< CWTSQuaternion<> >() "
       << (qtn3 = CWTSZero< CWTSQuaternion<> >()) << endl;
  cout << "qtn3 = CWTSUnity< CWTSQuaternion<> >() "
       << (qtn3 = CWTSUnity< CWTSQuaternion<> >()) << endl;
  cout << "qtn1(1, 2, 3, 4) "
       << (qtn1) << endl;
  cout << "qtn4(1) "
       << (qtn4) << endl;
  cout << "qtn2 = qtn1 "
       << (qtn2 = qtn1) << endl;
  cout << "qtn1*qtn2 "
       << (qtn1*qtn2) << endl;
  cout << "conj(qtn1) "
       << (conj(qtn1)) << endl;
  cout << "qtn1*conj(qtn1) "
       << (qtn1*conj(qtn1)) << endl;
  cout << "conj(qtn1)*qtn1 "
       << (conj(qtn1)*qtn1) << endl;
  cout << "conj(qtn1)*svec1*qtn1 "
       << (conj(qtn1)*svec1*qtn1) << endl;
  cout << "inv(qtn1) "
       << (inv(qtn1)) << endl;
  cout << "qtn1*inv(qtn1) "
       << (qtn1*inv(qtn1)) << endl;
  cout << "inv(qtn1)*qtn1 "
       << (inv(qtn1)*qtn1) << endl << endl;

  cout << "SmatFromEuler321Angles(M_PI/4, M_PI/4, M_PI/4) "
       << (smatFromEuler321Angles(M_PI/4, M_PI/4, M_PI/4)) << endl;
  cout << "QtnFromEuler321Angles(M_PI/4, M_PI/4, M_PI/4) "
       << (qtnFromEuler321Angles(M_PI/4, M_PI/4, M_PI/4)) << endl << endl;

  cout << "norm(qtn1) "
       << (norm(qtn1)) << endl;
  cout << "norm(CWTSQuaternion<>(0, 0, 0, 1)) "
       << (norm(CWTSQuaternion<>(0, 0, 0, 1))) << endl;
  cout << "norm(CWTSQuaternion<>(0, 0, 0, 2)) "
       << (norm(CWTSQuaternion<>(0, 0, 0, 2))) << endl;
  cout << "norm(CWTSQuaternion<>(1, 0, 0, 0)) "
       << (norm(CWTSQuaternion<>(1, 0, 0, 0))) << endl;
  cout << "norm(CWTSQuaternion<>(2, 0, 0, 0)) "
       << (norm(CWTSQuaternion<>(2, 0, 0, 0))) << endl;
  cout << "norm(CWTSQuaternion<>(1, 1, 1, 1)) "
       << (norm(CWTSQuaternion<>(1, 1, 1, 1))) << endl;
  cout << "norm(CWTSQuaternion<>(2, 2, 2, 2)) "
       << (norm(CWTSQuaternion<>(2, 2, 2, 2))) << endl;
  cout << "norm(CWTSQuaternion<>(3, 3, 3, 3)) "
       << (norm(CWTSQuaternion<>(3, 3, 3, 3))) << endl;
  cout << "norm(CWTSQuaternion<>(-4, -4, -4, -4)) "
       << (norm(CWTSQuaternion<>(-4, -4, -4, -4))) << endl << endl;

  cout << "sgn(qtn1) "
       << (sgn(qtn1)) << endl;
  cout << "sgn(CWTSQuaternion<>(0, 0, 0, 1)) "
       << (sgn(CWTSQuaternion<>(0, 0, 0, 1))) << endl;
  cout << "sgn(CWTSQuaternion<>(0, 0, 0, 2)) "
       << (sgn(CWTSQuaternion<>(0, 0, 0, 2))) << endl;
  cout << "sgn(CWTSQuaternion<>(1, 0, 0, 0)) "
       << (sgn(CWTSQuaternion<>(1, 0, 0, 0))) << endl;
  cout << "sgn(CWTSQuaternion<>(2, 0, 0, 0)) "
       << (sgn(CWTSQuaternion<>(2, 0, 0, 0))) << endl;
  cout << "sgn(CWTSQuaternion<>(1, 1, 1, 1)) "
       << (sgn(CWTSQuaternion<>(1, 1, 1, 1))) << endl;
  cout << "sgn(CWTSQuaternion<>(2, 2, 2, 2)) "
       << (sgn(CWTSQuaternion<>(2, 2, 2, 2))) << endl;
  cout << "sgn(CWTSQuaternion<>(3, 3, 3, 3)) "
       << (sgn(CWTSQuaternion<>(3, 3, 3, 3))) << endl;
  cout << "sgn(CWTSQuaternion<>(-4, -4, -4, -4)) "
       << (sgn(CWTSQuaternion<>(-4, -4, -4, -4))) << endl << endl;

  cout << "arg(qtn1) "
       << (arg(qtn1)) << endl;
  cout << "arg(CWTSQuaternion<>(0, 0, 0, 1)) "
       << (arg(CWTSQuaternion<>(0, 0, 0, 1))) << endl;
  cout << "arg(CWTSQuaternion<>(0, 0, 0, 2)) "
       << (arg(CWTSQuaternion<>(0, 0, 0, 2))) << endl;
  cout << "arg(CWTSQuaternion<>(1, 0, 0, 0)) "
       << (arg(CWTSQuaternion<>(1, 0, 0, 0))) << endl;
  cout << "arg(CWTSQuaternion<>(2, 0, 0, 0)) "
       << (arg(CWTSQuaternion<>(2, 0, 0, 0))) << endl;
  cout << "arg(CWTSQuaternion<>(1, 1, 1, 1)) "
       << (arg(CWTSQuaternion<>(1, 1, 1, 1))) << endl;
  cout << "arg(CWTSQuaternion<>(2, 2, 2, 2)) "
       << (arg(CWTSQuaternion<>(2, 2, 2, 2))) << endl;
  cout << "arg(CWTSQuaternion<>(3, 3, 3, 3)) "
       << (arg(CWTSQuaternion<>(3, 3, 3, 3))) << endl;
  cout << "arg(CWTSQuaternion<>(-4, -4, -4, -4)) "
       << (arg(CWTSQuaternion<>(-4, -4, -4, -4))) << endl << endl;

  cout << "exp(qtn1) "
       << (exp(qtn1)) << endl;
  cout << "norm(qtn1)*exp(CWTSQuaternion<>(sgn(im(qtn1))*arg(qtn1))) "
       << (norm(qtn1)*exp(CWTSQuaternion<>(sgn(im(qtn1))*arg(qtn1)))) << endl;
  cout << "exp(CWTSQuaternion<>(0, 0, 0, 1)) "
       << (exp(CWTSQuaternion<>(0, 0, 0, 1))) << endl;
  cout << "exp(CWTSQuaternion<>(0, 0, 0, 2)) "
       << (exp(CWTSQuaternion<>(0, 0, 0, 2))) << endl;
  cout << "exp(CWTSQuaternion<>(1, 0, 0, 0)) "
       << (exp(CWTSQuaternion<>(1, 0, 0, 0))) << endl;
  cout << "exp(CWTSQuaternion<>(2, 0, 0, 0)) "
       << (exp(CWTSQuaternion<>(2, 0, 0, 0))) << endl;
  cout << "exp(CWTSQuaternion<>(1, 1, 1, 1)) "
       << (exp(CWTSQuaternion<>(1, 1, 1, 1))) << endl;
  double d = M_PI/sqrt(3.0);
  cout << "exp(CWTSQuaternion<>(" << d << ", " << d << ", " << d << ", "  << d  << ")) "
       << (exp(CWTSQuaternion<>(d, d, d, d))) << endl;
  cout << "exp(CWTSQuaternion<>(2, 2, 2, 2)) "
       << (exp(CWTSQuaternion<>(2, 2, 2, 2))) << endl;
  cout << "exp(CWTSQuaternion<>(3, 3, 3, 3)) "
       << (exp(CWTSQuaternion<>(3, 3, 3, 3))) << endl;
  cout << "exp(CWTSQuaternion<>(-4, -4, -4, -4)) "
       << (exp(CWTSQuaternion<>(-4, -4, -4, -4))) << endl << endl;

  cout << "log(qtn1) "
       << (log(qtn1)) << endl;
  cout << "log(CWTSQuaternion<>(0, 0, 0, 1)) "
       << (log(CWTSQuaternion<>(0, 0, 0, 1))) << endl;
  cout << "log(CWTSQuaternion<>(0, 0, 0, 2)) "
       << (log(CWTSQuaternion<>(0, 0, 0, 2))) << endl;
  cout << "log(CWTSQuaternion<>(1, 0, 0, 0)) "
       << (log(CWTSQuaternion<>(1, 0, 0, 0))) << endl;
  cout << "log(CWTSQuaternion<>(2, 0, 0, 0)) "
       << (log(CWTSQuaternion<>(2, 0, 0, 0))) << endl;
  cout << "log(CWTSQuaternion<>(1, 1, 1, 1)) "
       << (log(CWTSQuaternion<>(1, 1, 1, 1))) << endl;
  cout << "log(CWTSQuaternion<>(" << d << ", " << d << ", " << d << ", "  << d  << ")) "
       << (log(CWTSQuaternion<>(d, d, d, d))) << endl;
  cout << "log(CWTSQuaternion<>(2, 2, 2, 2)) "
       << (log(CWTSQuaternion<>(2, 2, 2, 2))) << endl;
  cout << "log(CWTSQuaternion<>(3, 3, 3, 3)) "
       << (log(CWTSQuaternion<>(3, 3, 3, 3))) << endl;
  cout << "log(CWTSQuaternion<>(-4, -4, -4, -4)) "
       << (log(CWTSQuaternion<>(-4, -4, -4, -4))) << endl << endl;

  cout << "exp(log(qtn1)) "
       << (exp(log(qtn1))) << endl;
  cout << "log(exp(qtn1)) "
       << (log(exp(qtn1))) << endl;
  cout << "exp(log(CWTSQuaternion<>(0, 0, 0, 1))) "
       << (exp(log(CWTSQuaternion<>(0, 0, 0, 1)))) << endl;
  cout << "log(exp(CWTSQuaternion<>(0, 0, 0, 1))) "
       << (log(exp(CWTSQuaternion<>(0, 0, 0, 1)))) << endl;
  cout << "exp(log(CWTSQuaternion<>(0, 0, 0, 2))) "
       << (exp(log(CWTSQuaternion<>(0, 0, 0, 2)))) << endl;
  cout << "log(exp(CWTSQuaternion<>(0, 0, 0, 2))) "
       << (log(exp(CWTSQuaternion<>(0, 0, 0, 2)))) << endl;
  cout << "exp(log(CWTSQuaternion<>(1, 0, 0, 0))) "
       << (exp(log(CWTSQuaternion<>(1, 0, 0, 0)))) << endl;
  cout << "log(exp(CWTSQuaternion<>(1, 0, 0, 0))) "
       << (log(exp(CWTSQuaternion<>(1, 0, 0, 0)))) << endl;
  cout << "exp(log(CWTSQuaternion<>(2, 0, 0, 0))) "
       << (exp(log(CWTSQuaternion<>(2, 0, 0, 0)))) << endl;
  cout << "log(exp(CWTSQuaternion<>(2, 0, 0, 0))) "
       << (log(exp(CWTSQuaternion<>(2, 0, 0, 0)))) << endl;
  cout << "exp(log(CWTSQuaternion<>(0.5, 0.5, 0.5, 0.5))) "
       << (exp(log(CWTSQuaternion<>(0.5, 0.5, 0.5, 0.5)))) << endl;
  cout << "log(exp(CWTSQuaternion<>(0.5, 0.5, 0.5, 0.5))) "
       << (log(exp(CWTSQuaternion<>(0.5, 0.5, 0.5, 0.5)))) << endl;
  cout << "exp(log(CWTSQuaternion<>(1, 1, 1, 1))) "
       << (exp(log(CWTSQuaternion<>(1, 1, 1, 1)))) << endl;
  cout << "log(exp(CWTSQuaternion<>(1, 1, 1, 1))) "
       << (log(exp(CWTSQuaternion<>(1, 1, 1, 1)))) << endl;
  cout << "log(exp(CWTSQuaternion<>(" << d << ", " << d << ", " << d << ", "  << d  << "))) "
       << (log(exp(CWTSQuaternion<>(d, d, d, d)))) << endl;
  cout << "exp(log(CWTSQuaternion<>(" << d << ", " << d << ", " << d << ", "  << d  << "))) "
       << (exp(log(CWTSQuaternion<>(d, d, d, d)))) << endl;
  cout << "exp(log(CWTSQuaternion<>(2, 2, 2, 2))) "
       << (exp(log(CWTSQuaternion<>(2, 2, 2, 2)))) << endl;
  cout << "log(exp(CWTSQuaternion<>(2, 2, 2, 2))) "
       << (log(exp(CWTSQuaternion<>(2, 2, 2, 2)))) << endl;
  cout << "exp(log(CWTSQuaternion<>(3, 3, 3, 3))) "
       << (exp(log(CWTSQuaternion<>(3, 3, 3, 3)))) << endl;
  cout << "log(exp(CWTSQuaternion<>(3, 3, 3, 3))) "
       << (log(exp(CWTSQuaternion<>(3, 3, 3, 3)))) << endl;
  cout << "exp(log(CWTSQuaternion<>(-4, -4, -4, -4))) "
       << (exp(log(CWTSQuaternion<>(-4, -4, -4, -4)))) << endl;
  cout << "log(exp(CWTSQuaternion<>(-4, -4, -4, -4))) "
       << (log(exp(CWTSQuaternion<>(-4, -4, -4, -4)))) << endl << endl;

  cout << "pow(qtn1, qtn2) "
       << (pow(qtn1, qtn2)) << endl;
  cout << "pow(CWTSQuaternion<>(0, 0, 0, 2), CWTSQuaternion<>(0, 0, 0, 1) "
       << (pow(CWTSQuaternion<>(0, 0, 0, 2), CWTSQuaternion<>(0, 0, 0, 1)))
       << endl;
  cout << "pow(CWTSQuaternion<>(0, 0, 0, 2), CWTSQuaternion<>(0, 0, 0, 2) "
       << (pow(CWTSQuaternion<>(0, 0, 0, 2), CWTSQuaternion<>(0, 0, 0, 2)))
       << endl;
  cout << "pow(CWTSQuaternion<>(0, 0, 0, 2), CWTSQuaternion<>(1, 0, 0, 0) "
       << (pow(CWTSQuaternion<>(0, 0, 0, 2), CWTSQuaternion<>(1, 0, 0, 0)))
       << endl;
  cout << "pow(CWTSQuaternion<>(0, 0, 0, 2), CWTSQuaternion<>(2, 0, 0, 0) "
       << (pow(CWTSQuaternion<>(0, 0, 0, 2), CWTSQuaternion<>(2, 0, 0, 0)))
       << endl;
  cout << "pow(CWTSQuaternion<>(2, 0, 0, 0), CWTSQuaternion<>(0, 0, 0, 1) "
       << (pow(CWTSQuaternion<>(2, 0, 0, 0), CWTSQuaternion<>(0, 0, 0, 1)))
       << endl;
  cout << "pow(CWTSQuaternion<>(2, 0, 0, 0), CWTSQuaternion<>(0, 0, 0, 2) "
       << (pow(CWTSQuaternion<>(2, 0, 0, 0), CWTSQuaternion<>(0, 0, 0, 2)))
       << endl;
  cout << "CWTSQuaternion<>(2, 0, 0, 0)*CWTSQuaternion<>(2, 0, 0, 0) "
       << (CWTSQuaternion<>(2, 0, 0, 0)*CWTSQuaternion<>(2, 0, 0, 0))
       << endl;
  cout << "pow(CWTSQuaternion<>(1, 0, 0, 0), CWTSQuaternion<>(1, 0, 0, 0) "
       << (pow(CWTSQuaternion<>(1, 0, 0, 0), CWTSQuaternion<>(1, 0, 0, 0)))
       << endl;
  cout << "pow(CWTSQuaternion<>(1, 0, 0, 0), CWTSQuaternion<>(2, 0, 0, 0) "
       << (pow(CWTSQuaternion<>(1, 0, 0, 0), CWTSQuaternion<>(2, 0, 0, 0)))
       << endl;
  cout << "pow(CWTSQuaternion<>(2, 0, 0, 0), CWTSQuaternion<>(1, 0, 0, 0) "
       << (pow(CWTSQuaternion<>(2, 0, 0, 0), CWTSQuaternion<>(1, 0, 0, 0)))
       << endl;
  cout << "pow(CWTSQuaternion<>(2, 0, 0, 0), CWTSQuaternion<>(2, 0, 0, 0) "
       << (pow(CWTSQuaternion<>(2, 0, 0, 0), CWTSQuaternion<>(2, 0, 0, 0)))
       << endl << endl;

  cout << "End test of static matrices" << endl;
}

// Test matrices with structured elements.
void test_struct_matrix()
{
  cout << endl << "Start test of matrices with structured elements" << endl;

  typedef CWTSSquareMatrix<2> CWSMat2;
  typedef CWTSMatrix<2, 3, CWSMat2> CWMat23SMat2;
  typedef CWTSMatrix<3, 2, CWSMat2> CWMat32SMat2;
  typedef CWTSMatrix<2, 2, CWSMat2> CWMat22SMat2;

  cout << endl << "CWTSMatrix<T>" << endl << endl;

  CWSMat2 smtx;
  cout << "smtx = CWTSUnity<CWSMat2>() "
       << (smtx = CWTSUnity<CWSMat2>()) << endl;
  cout << "smtx *= 3.0 "
       << (smtx *= 3.0) << endl;

  CWMat23SMat2 mtx23Smtx;
  mtx23Smtx.fill(smtx);
  cout << "mtx23Smtx.fill(smtx) "
       << mtx23Smtx << endl;

  CWMat32SMat2 mtx32Smtx;
  mtx32Smtx.fill(smtx);
  cout << "mtx32Smtx.fill(smtx) "
       << mtx32Smtx << endl;

  CWMat22SMat2 mtx22Smtx;
  cout << "mtx22Smtx = mtx23Smtx*mtx32Smtx "
       << (mtx22Smtx = mtx23Smtx*mtx32Smtx) << endl;
  cout << "mtx22Smtx = mtx22Smtx + mtx22Smtx "
       << (mtx22Smtx = mtx22Smtx + mtx22Smtx) << endl;
  cout << "mtx22Smtx += mtx22Smtx "
       << (mtx22Smtx += mtx22Smtx) << endl;
  cout << "mtx22Smtx = mtx22Smtx - mtx22Smtx*smtx "
       << (mtx22Smtx = mtx22Smtx - mtx22Smtx*smtx) << endl;
  cout << "mtx22Smtx -= mtx22Smtx*smtx "
       << (mtx22Smtx -= mtx22Smtx*smtx) << endl;
  cout << "mtx22Smtx = -mtx22Smtx "
       << (mtx22Smtx = -mtx22Smtx) << endl;
  cout << "mtx22Smtx = mtx22Smtx*smtx "
       << (mtx22Smtx = mtx22Smtx*smtx) << endl;
  cout << "mtx22Smtx = smtx*mtx22Smtx "
       << (mtx22Smtx = smtx*mtx22Smtx) << endl;
  cout << "mtx22Smtx *= smtx "
       << (mtx22Smtx *= smtx) << endl;
  cout << "mtx22Smtx = mtx22Smtx/smtx "
       << (mtx22Smtx = mtx22Smtx/smtx) << endl;
  cout << "mtx22Smtx /= smtx "
       << (mtx22Smtx /= smtx) << endl;

  typedef CWTSSquareMatrix<2, CWSMat2> CWSMat2SMat2;

  cout << endl << "CWTSSquareMatrix<T>" << endl << endl;

  CWSMat2SMat2 smtxSmtx1, smtxSmtx2, smtxSmtx3;
  cout << "smtxSmtx1 = CWTSUnity<CWSMat2SMat2>() "
       << (smtxSmtx1 = CWTSUnity<CWSMat2SMat2>()) << endl;
  cout << "smtxSmtx2 = CWTSUnity<CWSMat2SMat2>() "
       << (smtxSmtx2 = CWTSUnity<CWSMat2SMat2>()) << endl;
  cout << "smtxSmtx1 = smtxSmtx1*smtx "
       << (smtxSmtx1 = smtxSmtx1*smtx) << endl;
  cout << "smtxSmtx1 = smtx*smtxSmtx1 "
       << (smtxSmtx1 = smtx*smtxSmtx1) << endl;
  cout << "smtxSmtx1 *= smtx "
       << (smtxSmtx1 *= smtx) << endl;
  cout << "smtxSmtx1 = smtxSmtx1/smtx "
       << (smtxSmtx1 = smtxSmtx1/smtx) << endl;
  cout << "smtxSmtx1 /= smtx "
       << (smtxSmtx1 /= smtx) << endl;

  cout << "smtxSmtx1 *= smtx "
       << (smtxSmtx1 *= smtx) << endl;

  cout << "smtxSmtx3 = smtxSmtx1 + smtxSmtx2 "
       << (smtxSmtx3 = smtxSmtx1 + smtxSmtx2) << endl;
  cout << "smtxSmtx3 += smtxSmtx1 "
       << (smtxSmtx3 += smtxSmtx1) << endl;
  cout << "smtxSmtx3 = smtxSmtx1 - smtxSmtx2 "
       << (smtxSmtx3 = smtxSmtx1 - smtxSmtx2) << endl;
  cout << "smtxSmtx3 -= smtxSmtx1 "
       << (smtxSmtx3 -= smtxSmtx1) << endl;
  cout << "smtxSmtx3 = -smtxSmtx1 "
       << (smtxSmtx3 = -smtxSmtx1) << endl;
  cout << "smtxSmtx3 = smtxSmtx1*smtxSmtx2 "
       << (smtxSmtx3 = smtxSmtx1*smtxSmtx2) << endl;
  cout << "smtxSmtx3 *= smtxSmtx1 "
       << (smtxSmtx3 *= smtxSmtx1) << endl;
  cout << "smtxSmtx3 = smtxSmtx2/smtxSmtx1 "
       << (smtxSmtx3 = smtxSmtx2/smtxSmtx1) << endl;
  cout << "smtxSmtx3 /= smtxSmtx1 "
       << (smtxSmtx3 /= smtxSmtx1) << endl;

  typedef CWTSVector<3, CWSMat2> CWVec3SMat2;

  cout << endl << "CWTSVector<T>" << endl << endl;

  CWSMat2 smtx2 = smtx;
  CWVec3SMat2 vecSmtx1, vecSmtx2, vecSmtx3;
  vecSmtx1.fill(smtx2);
  cout << "vecSmtx1.fill(smtx2) "
       << vecSmtx1 << endl;
  cout << "vecSmtx2 = vecSmtx1 "
       << (vecSmtx2 = vecSmtx1) << endl;
  cout << "vecSmtx3 = CWTSZero<CWVec3SMat2>() "
       << (vecSmtx3 = CWTSZero<CWVec3SMat2>()) << endl;

  cout << "vecSmtx3 = vecSmtx1 + vecSmtx2 "
       << (vecSmtx3 = vecSmtx1 + vecSmtx2) << endl;
  cout << "vecSmtx3 += vecSmtx1 "
       << (vecSmtx3 += vecSmtx1) << endl;
  cout << "vecSmtx3 = vecSmtx1 - vecSmtx2 "
       << (vecSmtx3 = vecSmtx1 - vecSmtx2) << endl;
  cout << "vecSmtx3 -= vecSmtx1 "
       << (vecSmtx3 -= vecSmtx1) << endl;
  cout << "vecSmtx3 = -vecSmtx1 "
       << (vecSmtx3 = -vecSmtx1) << endl;
  cout << "vecSmtx3 = vecSmtx3*smtx2 "
       << (vecSmtx3 = vecSmtx3*smtx2) << endl;
  cout << "vecSmtx3 = smtx2*vecSmtx3 "
       << (vecSmtx3 = smtx2*vecSmtx3) << endl;
  cout << "vecSmtx3 *= smtx2 "
       << (vecSmtx3 *= smtx2) << endl;
  cout << "smtx2 = vecSmtx1*vecSmtx2 "
       << (smtx2 = vecSmtx1*vecSmtx2) << endl;
  cout << "vecSmtx3 = vecSmtx3/smtx2 "
       << (vecSmtx3 = vecSmtx3/smtx2) << endl;
  cout << "vecSmtx3 /= smtx2 "
       << (vecSmtx3 /= smtx2) << endl;

  cout << "vecSmtx1 != vecSmtx2 "
       << (vecSmtx1 != vecSmtx2) << endl;

  typedef CWTSSpaceVector<CWSMat2> CWSvecSMat2;

  cout << endl << "CWTSSpaceVector<T>" << endl << endl;

  CWSvecSMat2 svecSmtx1, svecSmtx2, svecSmtx3;
  cout << "svecSmtx1 = CWTSZero<CWSvecSMat2>() "
       << (svecSmtx1 = CWTSZero<CWSvecSMat2>()) << endl;
  svecSmtx2.fill(smtx);
  cout << "svecSmtx2.fill(smtx) "
       << svecSmtx2 << endl;

  cout << "svecSmtx3 = svecSmtx1 + svecSmtx2 "
       << (svecSmtx3 = svecSmtx1 + svecSmtx2) << endl;
  cout << "svecSmtx3 += svecSmtx2 "
       << (svecSmtx3 += svecSmtx2) << endl;
  cout << "svecSmtx3 = svecSmtx1 - svecSmtx2 "
       << (svecSmtx3 = svecSmtx1 - svecSmtx2) << endl;
  cout << "svecSmtx3 -= svecSmtx2 "
       << (svecSmtx3 -= svecSmtx2) << endl;
  cout << "svecSmtx3 = -svecSmtx2 "
       << (svecSmtx3 = -svecSmtx2) << endl;
  cout << "svecSmtx3 = svecSmtx2*smtx "
       << (svecSmtx3 = svecSmtx2*smtx) << endl;
  cout << "svecSmtx3 = smtx*svecSmtx2 "
       << (svecSmtx3 = smtx*svecSmtx2) << endl;
  cout << "svecSmtx3 *= smtx "
       << (svecSmtx3 *= smtx) << endl;
  cout << "smtx2 = svecSmtx2*svecSmtx2 "
       << (smtx2 = svecSmtx2*svecSmtx2) << endl;

  cout << "svecSmtx1[0] = smtx "
       << (svecSmtx1[0] = smtx) << endl;
  cout << "svecSmtx3 = svecSmtx1%svecSmtx2 "
       << (svecSmtx3 = svecSmtx1%svecSmtx2) << endl;
  cout << "svecSmtx3 %= svecSmtx2 "
       << (svecSmtx3 %= svecSmtx2) << endl;

  cout << "svecSmtx3 = svecSmtx2/smtx "
       << (svecSmtx3 = svecSmtx2/smtx) << endl;
  cout << "svecSmtx3 /= smtx "
       << (svecSmtx3 /= smtx) << endl;

  typedef CWTSQuaternion<CWSMat2> CWQuatSMat2;

  cout << endl << "CWTSQuaternion<T>" << endl << endl;

  CWQuatSMat2 qtnSmtx1, qtnSmtx2, qtnSmtx3;
  cout << "qtnSmtx1 = CWTSZero<CWQuatSMat2>() "
       << (qtnSmtx1 = CWTSZero<CWQuatSMat2>()) << endl;
  qtnSmtx1.fill(CWTSUnity<CWSMat2>());
  cout << "qtnSmtx1.fill(CWTSUnity<CWSMat2>()) "
       << qtnSmtx1  << endl;
  cout << "qtnSmtx2 = qtnSmtx1*smtx2 "
       << (qtnSmtx2 = qtnSmtx1*smtx2) << endl;
  cout << "qtnSmtx3 = qtnSmtx1 "
       << (qtnSmtx3 = qtnSmtx1) << endl;

  cout << "qtnSmtx2 = qtnSmtx1 + qtnSmtx1 "
       << (qtnSmtx2 = qtnSmtx1 + qtnSmtx1) << endl;
  cout << "qtnSmtx3 += qtnSmtx1 "
       << (qtnSmtx3 += qtnSmtx1) << endl;
  cout << "qtnSmtx2 = qtnSmtx1 - qtnSmtx1 "
       << (qtnSmtx2 = qtnSmtx1 - qtnSmtx1) << endl;
  cout << "qtnSmtx3 -= qtnSmtx1 "
       << (qtnSmtx3 -= qtnSmtx1) << endl;
  cout << "qtnSmtx3 = -qtnSmtx1 "
       << (qtnSmtx3 = -qtnSmtx1) << endl;
  cout << "qtnSmtx2 = smtx2*qtnSmtx1 "
       << (qtnSmtx2 = smtx2*qtnSmtx1) << endl;
  cout << "qtnSmtx3 *= qtnSmtx1 "
       << (qtnSmtx3 *= qtnSmtx1) << endl;
  cout << "qtnSmtx3 = qtnSmtx1*conj(qtnSmtx1) "
       << (qtnSmtx3 = qtnSmtx1*conj(qtnSmtx1)) << endl;
  cout << "qtnSmtx3 = inv(qtnSmtx1) "
       << (qtnSmtx3 = inv(qtnSmtx1)) << endl;
  cout << "qtnSmtx3 = qtnSmtx1/smtx2 "
       << (qtnSmtx3 = qtnSmtx1/smtx2) << endl;
  cout << "qtnSmtx3 /= smtx2 "
       << (qtnSmtx3 /= smtx2) << endl;
  cout << "qtnSmtx3 = qtnSmtx1/qtnSmtx2 "
       << (qtnSmtx3 = qtnSmtx1/qtnSmtx2) << endl;
  cout << "qtnSmtx3 /= qtnSmtx1 "
       << (qtnSmtx3 /= qtnSmtx1) << endl;

  cout << endl << "End test of matrices with structured elements" << endl;
}

int main(int argc, char *argv[])
{
  test_static_matrix();
  test_struct_matrix();

  cout << endl << "Start test row and column matrices." << endl << endl;

  // Test row and column matrices.
  CWTSMatrix<1, 3> mtx1x3;
  mtx1x3[0][0] = 1;
  mtx1x3[0][1] = 2;
  mtx1x3[0][2] = 3;

  CWTSMatrix<3, 1> mtx3x1;
  mtx3x1[0][0] = 1;
  mtx3x1[1][0] = 2;
  mtx3x1[2][0] = 3;

  cout << "mtx3x1*mtx1x3 "
       << mtx3x1*mtx1x3 << endl;
  cout << "mtx1x3*mtx3x1 "
       << mtx1x3*mtx3x1 << endl;

  cout << endl << "End test row and column matrices." << endl;

  return 0;
}
