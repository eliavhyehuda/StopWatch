#include "SchemeUtils.h"
#include "Logging.h"

typedef vector<vector<double> > DoubleLists;

bool scheme_extract_bool_value(scheme* sc, pointer val_ptr, bool* out);
bool scheme_extract_char_value(scheme* sc, pointer val_ptr, char* out);
bool scheme_extract_long_value(scheme* sc, pointer val_ptr, long* out);
bool scheme_extract_string_value(scheme* sc, pointer val_ptr, string* out);
bool scheme_extract_double_value(scheme* sc, pointer val_ptr, double * out);
bool scheme_extract_double_vector(scheme* sc, pointer args, int position, int vector_length, vector<double>* out);
bool scheme_extract_vector_of_double_vectors(scheme* sc, pointer args, int position, int num_per_vector, DoubleLists* out_lists);


bool scheme_has_arg_num(scheme* sc, pointer args, int arg_num) {
  if (arg_num <= 0) {
    return true;
  }
  if (args == sc->NIL || !is_pair(args)) {
    return false;
  }
  return scheme_has_arg_num(sc, pair_cdr(args), arg_num-1);
}

bool scheme_get_arg_num(scheme* sc, pointer args, int arg_num, pointer* out) {
  if (args == sc->NIL || !is_pair(args)) {
    LOG_ERROR << "Error: Not enough arguments to function";
    return false;
  }
  if (arg_num <= 0) {
    (*out) = pair_car(args);
    return true;
  }
  return scheme_get_arg_num(sc, pair_cdr(args), arg_num-1, out);
}

int  scheme_get_list_length(scheme* sc, pointer list_ptr) {
  if (list_ptr == sc->NIL) {
    return 0;
  }
  if (!is_pair(list_ptr)) {
    return -1;
  }
  return 1 + scheme_get_list_length(sc, pair_cdr(list_ptr));
}

bool scheme_extract_bool_value(scheme* sc, pointer val_ptr, bool* out) {
  if (val_ptr == sc->NIL
      || val_ptr == sc->F
      || (is_integer(val_ptr) && ivalue(val_ptr) == 0))
    {
      (*out) = false;
    }
  else
    {
      (*out) = true;
    }
  return true;
}

bool scheme_extract_char_value(scheme* sc, pointer val_ptr, char* out) {
  if (val_ptr == sc->NIL) {
    LOG_ERROR << "Error: Expected char or string value";
    return false;
  }
  if (is_character(val_ptr)) {
    (*out) = (char)charvalue(val_ptr);
    return true;
  }
  if (is_string(val_ptr)) {
    char* s = string_value(val_ptr);
    if (s) {
      (*out) = *s;
      return true;
    }
  }
  LOG_ERROR << "Error: Expected char or string value";
  return false;
}

bool scheme_extract_long_value(scheme* sc, pointer val_ptr, long* out) {
  if (val_ptr == sc->NIL || !is_integer(val_ptr)) {
    LOG_ERROR << "Error: Expected int value";
    return false;
  }
  (*out) = ivalue(val_ptr);
  return true;
}

bool scheme_extract_double_value(scheme* sc, pointer val_ptr, double * out) {
  if (val_ptr == sc->NIL) {
    LOG_ERROR << "Error: NIL value for extracting real value";
    return false;
  }
  if (is_real(val_ptr)) {
    (*out) = rvalue(val_ptr);
    return true;
  } else if (is_integer(val_ptr)) {
    (*out) = ivalue(val_ptr);
    return true;
  }
  LOG_ERROR << "Error: Expected real value";
  return false;
}

bool scheme_extract_string_value(scheme* sc, pointer val_ptr, string* out) {
  if (val_ptr == sc->NIL || !is_string(val_ptr)) {
    LOG_ERROR << "Error: Expected string value";
    return false;
  }
  (*out) = string_value(val_ptr);
  return true;
}

bool scheme_extract_double_vector(scheme* sc, pointer args, int position, int vector_length, vector<double>* out) {
  pointer list;
  if (!scheme_get_arg_num(sc, args, position, &list)) {
    return false;
  }

  int num_items_read = 0;
  while (1) {
    if (vector_length > 0 && num_items_read == vector_length) {
      return true;
    }
    if (list == sc->NIL) {
      break;
    }
    if (!is_pair(list)) {
      LOG_ERROR << "Error: Expected a list of reals";
      return false;
    }
    double val;
    if (!scheme_extract_double_value(sc, pair_car(list), &val)) {
      return false;
    }
    out->push_back(val);
    num_items_read++;
    list = pair_cdr(list);
  }
  if (vector_length > 0) {
    if (num_items_read < vector_length) {
      LOG_ERROR << "Error: Not enough items in Scheme list. Expected " << vector_length << ", got " << num_items_read;
      return false;
    }
  }
  return true;
}

bool scheme_extract_vector_of_double_vectors(scheme* sc, pointer args, int position, int num_per_vector, DoubleLists* out_lists) {
  pointer cur_pointer;
  if (!scheme_get_arg_num(sc, args, position, &cur_pointer)) {
    return false;
  }
  while (1) {
    if (cur_pointer == sc->NIL) {
      return true;
    }
    if (!is_pair(cur_pointer)) {
      LOG_ERROR << "Error: Expected list of reals";
      return false;
    }
    vector<double> this_vector;
    if (!scheme_extract_double_vector(sc, cur_pointer, 0, num_per_vector, &this_vector)) {
      return false;
    }
    out_lists->push_back(this_vector);
    cur_pointer = pair_cdr(cur_pointer);
  }
  // Shouldn't get here
  LOG_ERROR << "Internal error in scheme_extract_vector_of_double_vectors";
  return true;
}

bool scheme_extract_bool(scheme* sc, pointer args, int position, bool* out) {
  pointer val;
  return (scheme_get_arg_num(sc, args, position, &val)
          && scheme_extract_bool_value(sc, val, out));
}

bool scheme_extract_char(scheme* sc, pointer args, int position, char* out) {
  pointer val;
  return (scheme_get_arg_num(sc, args, position, &val)
          && scheme_extract_char_value(sc, val, out));
}

bool scheme_extract_long(scheme* sc, pointer args, int position, long* out) {
  pointer val;
  return (scheme_get_arg_num(sc, args, position, &val)
          && scheme_extract_long_value(sc, val, out));
}

bool scheme_extract_double(scheme* sc, pointer args, int position, double* out) {
  pointer val;
  return (scheme_get_arg_num(sc, args, position, &val)
          && scheme_extract_double_value(sc, val, out));
}

bool scheme_extract_point(scheme* sc, pointer args, int position, Point* out) {
  vector<double> values;
  if (!scheme_extract_double_vector(sc, args, position, 2, &values)) {
    return false;
  }
  out->x = values[0];
  out->y = values[1];
  return true;
}

bool scheme_extract_rect(scheme* sc, pointer args, int position, Rectangle* out) {
  vector<double> values;
  if (!scheme_extract_double_vector(sc, args, position, 4, &values)) {
    return false;
  }
  out->x      = values[0];
  out->y      = values[1];
  out->width  = values[2];
  out->height = values[3];
  return true;
}

bool scheme_extract_matrix(scheme* sc, pointer args, int position, SquareMatrix* out) {
  DoubleLists lists;
  if (!scheme_extract_vector_of_double_vectors(sc, args, position, 3, &lists)) {
    return false;
  }
  if (lists.size() < 3) {
    LOG_ERROR << "Error: Not enough rows in matrix";
    return false;
  }
  for (unsigned int row = 0; row < out->getRows(); row++) {
    for (unsigned int col = 0; col < out->getCols(); col++) {
      (*out)[row][col] = lists[row][col];
    }
  }
  return true;
}

bool scheme_extract_point_list(scheme* sc, pointer args, int position, vector<Point>* out) {
  DoubleLists lists;
  if (!scheme_extract_vector_of_double_vectors(sc, args, position, 2, &lists)) {
    return false;
  }
  for (DoubleLists::iterator i = lists.begin(); i != lists.end(); i++) {
    out->push_back(Point((*i)[0], (*i)[1]));
  }
  return true;
}

bool scheme_extract_string(scheme* sc, pointer args, int position, string* out) {
  pointer val;
  return (scheme_get_arg_num(sc, args, position, &val)
          && scheme_extract_string_value(sc, val, out));
}

bool scheme_extract_string_list(scheme* sc, pointer args, int position, vector<string>* out) {
  string s;
  pointer slist;
  if (!scheme_get_arg_num(sc, args, position, &slist)) {
    return false;
  }
  if (slist == sc->NIL) {
    return true;
  }
  if (!is_pair(slist)) {
    LOG_ERROR << "Error: Expected string list";
    return false;
  }
  while (slist != sc->NIL) {
    if (!scheme_extract_string(sc, slist, 0, &s)) {
      LOG_ERROR << "Error: Expected string in string list";
      return false;
    }
    out->push_back(s);
    slist = pair_cdr(slist);
  }
  return true;
}


pointer scheme_long_vector_to_list(scheme* sc, const vector<long> & v) {
  pointer returnList = sc->NIL;
  for (vector<long>::const_reverse_iterator i = v.rbegin(); i != v.rend(); i++) {
    returnList = cons(sc, mk_integer(sc, *i), returnList);
  }
  return returnList;
}

pointer scheme_double_vector_to_list(scheme* sc, const vector<double> & v) {
  pointer returnList = sc->NIL;
  for (vector<double>::const_reverse_iterator i = v.rbegin(); i != v.rend(); i++) {
    returnList = cons(sc, mk_real(sc, *i), returnList);
  }
  return returnList;
}

pointer scheme_rectangle_to_list(scheme* sc, const Rectangle & rect) {
  vector<double> doubleList;
  doubleList.push_back(rect.x);
  doubleList.push_back(rect.y);
  doubleList.push_back(rect.width);
  doubleList.push_back(rect.height);
  return scheme_double_vector_to_list(sc, doubleList);
}

pointer scheme_point_to_list(scheme* sc, const Point & point) {
  vector<double> doubleList;
  doubleList.push_back(point.x);
  doubleList.push_back(point.y);
  return scheme_double_vector_to_list(sc, doubleList);
}

// Produces a list of lists (3 lists of 3 real values)
pointer scheme_matrix_to_lists(scheme* sc, const SquareMatrix & matrix) {
  pointer matrix_list = sc->NIL;
  for (int row = matrix.getRows()-1; row >= 0; row--) {
    pointer row_list = sc->NIL;
    for (int col = matrix.getCols()-1; col >= 0; col--) {
      row_list = cons(sc, mk_real(sc, matrix[row][col]), row_list);
    }
    matrix_list = cons(sc, row_list, matrix_list);
  }
  return matrix_list;
}

pointer scheme_string_vector_to_list(scheme* sc, const vector<string> & v) {
  pointer returnList = sc->NIL;
  for (vector<string>::const_reverse_iterator i = v.rbegin(); i != v.rend(); i++) {
    returnList = cons(sc, mk_string(sc, i->c_str()), returnList);
  }
  return returnList;
}
