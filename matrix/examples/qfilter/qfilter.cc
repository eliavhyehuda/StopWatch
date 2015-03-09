// $Id: qfilter.cc 166 2008-01-19 19:56:57Z hkuiper $

// This program applies a time domain filter to a time series of
// orientation data.

// This filter method was published by Jehee Lee and Sung Yong Shin in
// an article titled "General Construction of Time-Domain Filters for
// Orientation Data" in IEEE transactions on visualization and
// computer graphics, vol 8, no. 2, april-june 2002.

// Copyright (C) 2004 Harry Kuiper, hkuiper@xs4all.nl

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA

#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include "stat_quatern.h"
#include "stat_coordsys.h"

using namespace std;
using namespace CwMtx;

// Constants.
static const char szVersion[] = "$Id: qfilter.cc 166 2008-01-19 19:56:57Z hkuiper $";
static const char szNotice[] =
"Copyright (C) 2004 Harry Kuiper\n"
"  Qfilter comes with ABSOLUTELY NO WARRANTY.\n"
"  This is free software under the GNU General Public License. You are\n"
"  welcome to redistribute this program under certain conditions. For\n"
"  details read the file named \"LICENSE\" that should be distributed with\n"
"  this program.";

static const double DEG2RAD = M_PI/180.0;

// We need a special version of class vector that implements several
// reflection algorithms. We need that when using datasets that are
// not infinite and not periodic.
template <class T>
class rvector: public vector<T>
{
private:
  bool fMirrorBegin;
  bool fMirrorEnd;

public:
  rvector() { fMirrorBegin = false; fMirrorEnd = false; }

  void mirrorBeginPoint() { fMirrorBegin = true; }
  void mirrorEndPoint() { fMirrorEnd = true; }

  T & operator[](const int n);
};

typedef vector< double > VECTM;
typedef CWTSQuaternion<> QTN;
typedef vector< QTN > VECQTN;
typedef rvector< QTN > RVECQTN;
typedef vector< double > VECCOEFF;

template <class T>
T &
rvector<T>::operator[](const int n)
{
  if (n < 0)
    {
      if (fMirrorBegin)
        {
          // Modified reflection method.
          return vector<T>::operator[](-n - 1);
        }
      else
        {
          // Reflection method suggested by Lee and Yong Shin
          return vector<T>::operator[](-n);
        }
    }
  else if (n >= this->size())
    {
      if (fMirrorEnd)
        {
          // Modified reflection method.
          return vector<T>::operator[](2*(this->size()) - n - 1);
        }
      else
        {
          // Reflection method suggested by Lee and Yong Shin
          return vector<T>::operator[](2*(this->size()) - n - 2);
        }
    }
  else
    {
      return vector<T>::operator[](n);
    }
}

void readFilterMask(const string &strFile, VECCOEFF &vecCoeff)
{
  double coeff;

  // If no mask file is specified use the builtin smoothing mask.
  if (strFile == "")
    {
      vecCoeff.push_back(-1/24.0);
      vecCoeff.push_back(4/24.0);
      vecCoeff.push_back((24 - 6)/24.0);
      vecCoeff.push_back(4/24.0);
      vecCoeff.push_back(-1/24.0);

      return;
    }

  // Open mask file.
  ifstream ifsMask(strFile.c_str());
  if (!ifsMask)
    {
      cerr << "ERROR: Can not open mask file -- " << strFile << endl;
      exit(1);
    }

  // Read mask coefficients from file.
  for (ifsMask >> coeff; ifsMask; ifsMask >> coeff)
    {
      // Store coefficient.
      vecCoeff.push_back(coeff);
    }

  // Close mask file.
  ifsMask.close();

  // Check for an odd number of coefficients.
  if (vecCoeff.size()%2 == 0)
    {
      cerr << "ERROR: Number of coefficients in mask must be odd." << endl;
      exit(2);
    }
}

void streamInTimeSeries(istream &ifs, VECTM &vecTm, VECQTN &vecQtn)
{
  double tm;
  QTN qtn;

  // Read input data from file.
  for (ifs >> tm; ifs; ifs >> tm)
    {
      // Read remainder of line.
      ifs >> qtn[0] >> qtn[1] >> qtn[2] >> qtn[3];

      // To be safe: renormalise.
      qtn /= norm(qtn);

      // Store time point.
      vecTm.push_back(tm);
      vecQtn.push_back(qtn);
    }
}

void readTimeSeries(const string &strIn, VECTM &vecTm, VECQTN &vecQtn)
{
  if (strIn == "-")
    {
      // Use stdin.
      streamInTimeSeries(cin, vecTm, vecQtn);
    }
  else
    {
      // Open input file.
      ifstream ifs(strIn.c_str());
      if (!ifs)
        {
          cerr << "ERROR: Can not open input file -- " << strIn << endl;
          exit(3);
        }

      streamInTimeSeries(ifs, vecTm, vecQtn);

      // Close input file.
      ifs.close();
    }
}

void streamOutSmoothedSeries(ostream &os, VECTM &vecTm, VECQTN &vecQtnOut)
{
  double tm;
  char szLine[80];
  char szFormat[] = "%8.3f %8.3f %8.3f %8.3f %8.3f";

  // Write the smoothed data set.
  for (int i = 0; i < vecQtnOut.size(); i++)
    {
      sprintf(szLine, szFormat,
              vecTm[i], 
              vecQtnOut[i][0], vecQtnOut[i][1], vecQtnOut[i][2],
              vecQtnOut[i][3]);

      os << szLine << endl;
    }
}

void writeSmoothedSeries(const string &strOutFile,
                         VECTM &vecTm, VECQTN &vecQtnOut)
{
  if (strOutFile == "-")
    {
      // Use stdout.
      streamOutSmoothedSeries(cout, vecTm, vecQtnOut);
    }
  else
    {
      // Open output file. NOTE: We assume this always succeeds.
      ofstream ofs(strOutFile.c_str());

      streamOutSmoothedSeries(ofs, vecTm, vecQtnOut);

      // Close output file.
      ofs.close();
    }
}

int main(int cArg, char *szArgVal[])
{
  // Local variables.
  VECTM vecTm;
  VECQTN vecQtn1, vecQtn2;
  RVECQTN rvecQtnLog;
  VECCOEFF vecCoeff, vecFact;

  // Default values for program options. These values will be
  // overwritten by the corresponding option if present on the
  // commandline.
  int cIter = 1;
  double gain = 1.0, tm1 = 0.0, tm2 = 0.0;
  bool fTm1 = false, fTm2 = false;
  string strMask;
  string strInFile = "-";
  string strOutFile = "-";

  // Process commandline options.
  while (1)
    {
      char ch = getopt(cArg, szArgVal, "m:g:i:f:l:bevh");

      if (ch == -1)
        {
          break;
        }

      switch (ch)
        {
        case 'm':
          strMask = optarg;
          break;

        case 'g':
          gain = atof(optarg);
          break;

        case 'i':
          cIter = atoi(optarg);
          break;

        case 'f':
          tm1 = atof(optarg);
          fTm1 = true;
          break;

        case 'l':
          tm2 = atof(optarg);
          fTm2 = true;
          break;

        case 'b':
          rvecQtnLog.mirrorBeginPoint();
          break;

        case 'e':
          rvecQtnLog.mirrorEndPoint();
          break;

        case 'v':
          cout << "Qfilter version: " << szVersion << endl << endl
               << szNotice << endl;
          exit(0);
          break;

        case '?':
        case 'h':
          cerr << "Usage: qfilter [-m mask] [-g gain] [-i iter] [-f tm1] [-l tm2] \\" << endl
               << "               [-b] [-e] [-v] [-h] [in_file [out_file]]" << endl
               << "  -m mask  File containing filter mask." << endl
               << "  -g gain  Filter gain, default=" << gain << "." << endl
               << "  -i iter  Number of iterations, default=" << cIter << "." << endl
               << "  -f tm1   Process data points with time/index >= tm1" << endl
               << "  -l tm2   Process data points with time/index <= tm2" << endl
               << "  -b       Preserve begin point." << endl
               << "  -e       Preserve end point." << endl
               << "  -v       Print program version." << endl
               << "  -h       Print this help message." << endl
               << "  in_file  Input file, default=stdin." << endl
               << "  out_file Output file, default=stdout." << endl;
          exit(0);
          break;

        default:
          cerr << "WARNING: Option not implemented -- " << ch << endl;
        }
    }

  // Process non-option args. First two remaining args are interpreted
  // as input file and output file respectively. Additional args are
  // ignored.

  if (cArg - optind >= 1)
    {
      strInFile = szArgVal[optind];
    }

  if (cArg - optind >= 2)
    {
      strOutFile = szArgVal[optind + 1];
    }

  // Read filter mask.
  readFilterMask(strMask, vecCoeff);

  // Read time series.
  readTimeSeries(strInFile, vecTm, vecQtn1);

  // Create enough storage space in intermediate and result vectors.
  rvecQtnLog.insert(rvecQtnLog.end(), vecQtn1.size() - 1,
                    CWTSZero< QTN >());
  vecQtn2.insert(vecQtn2.end(), vecQtn1.size(),
                 CWTSZero< QTN >());

  // Compute filter weighting factors from mask coefficients.
  int k = (vecCoeff.size() - 1)/2;
  vecFact.insert(vecFact.end(), 2*k, 0.0);

  for (int i = 0; i < vecFact.size(); i++)
    {
      if (i < k)
        {
          for (int i2 = 0; i2 <= i; i2++)
            {
              vecFact[i] -= vecCoeff[i2];
            }
        }
      else
        {
          for (int i2 = i + 1; i2 <= vecFact.size(); i2++)
            {
              vecFact[i] += vecCoeff[i2];
            }
        }
    }

  // Iterate n times over the rest of the data set, write results from
  // vector1 to vector2 and vice versa with each iteration.
  VECQTN *pVecQtnIn = &vecQtn2;
  VECQTN *pVecQtnOut = &vecQtn1;

  for (int c = 0; c < cIter; c++)
    {
      // Swap input and output vector each pass.
      VECQTN *pVecQtnTmp = pVecQtnIn;
      pVecQtnIn = pVecQtnOut;
      pVecQtnOut = pVecQtnTmp;

      // Create references to the input and output vectors to keep the
      // syntax readable.
      VECQTN &vecQtnIn = *pVecQtnIn;
      VECQTN &vecQtnOut = *pVecQtnOut;

      // Compute logarithmic map.
      for (int i = 0; i < (vecQtnIn.size() - 1); i++)
        {
          rvecQtnLog[i] = log(inv(vecQtnIn[i])*vecQtnIn[i + 1]);
        }

      // Iterate through data set.
      for (int i = 0; i < vecQtnIn.size(); i++)
        {
          // Skip data points outside the interval specified on the
          // commandline.
          if ((fTm1 && vecTm[i] < tm1) || (fTm2 && vecTm[i] > tm2))
            {
              // Just copy current data point to output vector.
              vecQtnOut[i] = vecQtnIn[i];
            }
          else
            {
              // Apply filter.
              QTN qtnTmp = CWTSZero< QTN >();

              for (int i2 = 0; i2 < vecFact.size(); i2++)
                {
                  qtnTmp += vecFact[i2]*rvecQtnLog[i - k + i2];
                }

              vecQtnOut[i] = vecQtnIn[i]*exp(gain*qtnTmp);
            }
        }
    }

  // Write output file.
  writeSmoothedSeries(strOutFile, vecTm, *pVecQtnOut);
}
