#ifndef __SCHEME_UTILS__
#define __SCHEME_UTILS__

#include <vector>
#include <string>
#include "scheme/tinyscheme/scheme.h"
#include "scheme/tinyscheme/scheme-private.h"
#include "Point.h"
#include "Rectangle.h"
#include "AffineTransform.h"

using namespace std;
using namespace cs349;

/*
 * A series of utilities to assist in marshalling data to/from the
 * Scheme interpreter.
 */

bool scheme_has_arg_num(scheme* sc, pointer args, int arg_num);
bool scheme_get_arg_num(scheme* sc, pointer args, int arg_num, pointer* out);
int  scheme_get_list_length(scheme* sc, pointer list_ptr);

bool scheme_extract_bool(scheme* sc, pointer args, int position, bool* out);
bool scheme_extract_char(scheme* sc, pointer args, int position, char* out);
bool scheme_extract_long(scheme* sc, pointer args, int position, long* out);
bool scheme_extract_double(scheme* sc, pointer args, int position, double* out);
bool scheme_extract_point(scheme* sc, pointer args, int position, Point* out);
bool scheme_extract_rect(scheme* sc, pointer args, int position, Rectangle* out);
bool scheme_extract_matrix(scheme* sc, pointer args, int position, SquareMatrix* out);
bool scheme_extract_point_list(scheme* sc, pointer args, int position, vector<Point>* out);
bool scheme_extract_string(scheme* sc, pointer args, int position, string* out);
bool scheme_extract_string_list(scheme* sc, pointer args, int position, vector<string>* out);

pointer scheme_long_vector_to_list(scheme* sc, const vector<long> & v);
pointer scheme_double_vector_to_list(scheme* sc, const vector<double> & v);
pointer scheme_string_vector_to_list(scheme* sc, const vector<string> & v);

pointer scheme_rectangle_to_list(scheme* sc, const Rectangle & rect);
pointer scheme_point_to_list(scheme* sc, const Point & point);
pointer scheme_matrix_to_lists(scheme* sc, const SquareMatrix & matrix);

#endif // __SCHEME_UTILS__
