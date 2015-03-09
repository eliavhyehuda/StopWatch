#include "SchemeAffineTransform.h"
#include "AffineTransform.h"
#include "SchemeUtils.h"

using namespace cs349;


pointer scheme_affine_make_identity(scheme* sc, pointer args);
pointer scheme_affine_make_rotation(scheme* sc, pointer args);
pointer scheme_affine_make_scale(scheme* sc, pointer args);
pointer scheme_affine_make_translate(scheme* sc, pointer args);
pointer scheme_affine_invert(scheme* sc, pointer args);
pointer scheme_matrix_multiply(scheme* sc, pointer args);


int scheme_install_affine_procedures(scheme* sc) {
  scheme_define(sc, sc->global_env, mk_symbol(sc, "affine-make-identity"),
                                    mk_foreign_func(sc, scheme_affine_make_identity));
  scheme_define(sc, sc->global_env, mk_symbol(sc, "affine-make-rotation"),
                                    mk_foreign_func(sc, scheme_affine_make_rotation));
  scheme_define(sc, sc->global_env, mk_symbol(sc, "affine-make-scale"),
                                    mk_foreign_func(sc, scheme_affine_make_scale));
  scheme_define(sc, sc->global_env, mk_symbol(sc, "affine-make-translation"),
                                    mk_foreign_func(sc, scheme_affine_make_translate));
  scheme_define(sc, sc->global_env, mk_symbol(sc, "affine-invert"),
                                    mk_foreign_func(sc, scheme_affine_invert));
  scheme_define(sc, sc->global_env, mk_symbol(sc, "_matrix-mul"),
                                    mk_foreign_func(sc, scheme_matrix_multiply));
  return 0;
}

pointer scheme_affine_make_identity(scheme* sc, pointer args) {
  return scheme_matrix_to_lists(sc, AffineTransform::MakeIdentity().GetMatrix());
}

pointer scheme_affine_make_rotation(scheme* sc, pointer args) {
  double theta;
  if (!scheme_extract_double(sc, args, 0, &theta)) {
    fprintf(stderr, "Error: Need theta parameter for affine-make-rotation\n");
    return sc->NIL;
  }
  return scheme_matrix_to_lists(sc, AffineTransform::MakeRotation(theta).GetMatrix());
}

pointer scheme_affine_make_scale(scheme* sc, pointer args) {
  double scale_x;
  double scale_y;
  if (!scheme_extract_double(sc, args, 0, &scale_x)
      || !scheme_extract_double(sc, args, 1, &scale_y))
    {
      fprintf(stderr, "Error: Need x and y scale values for affine-make-scale\n");
      return sc->NIL;
    }
  return scheme_matrix_to_lists(sc, AffineTransform::MakeScale(scale_x, scale_y).GetMatrix());
}

pointer scheme_affine_make_translate(scheme* sc, pointer args) {
  double translate_x;
  double translate_y;
  if (!scheme_extract_double(sc, args, 0, &translate_x)
      || !scheme_extract_double(sc, args, 1, &translate_y))
    {
      fprintf(stderr, "Error: Need x and y translation values for affine-make-translate\n");
      return sc->NIL;
    }
  return scheme_matrix_to_lists(sc, AffineTransform::MakeTranslation(translate_x, translate_y).GetMatrix());
}

pointer scheme_affine_invert(scheme* sc, pointer args) {
  SquareMatrix m;
  if (!scheme_extract_matrix(sc, args, 0, &m)) {
    fprintf(stderr, "Error: Need list of lists for affine-invert\n");
    return sc->NIL;
  }
  AffineTransform t(m);
  return scheme_matrix_to_lists(sc, t.GetInverse().GetMatrix());
}

pointer scheme_matrix_multiply(scheme* sc, pointer args) {
  SquareMatrix l;
  if (!scheme_extract_matrix(sc, args, 0, &l)) {
    fprintf(stderr, "Error: Need matrix as first argument to matrix-mul\n");
    return sc->NIL;
  }

  pointer secondArg;
  if (!scheme_get_arg_num(sc, args, 1, &secondArg)) {
    fprintf(stderr, "Error: Need matrix or point as second argument to matrix-mul\n");
    return sc->NIL;
  }

  if (scheme_get_list_length(sc, secondArg) == 2) {
    Point p;
    if (!scheme_extract_point(sc, args, 1, &p)) {
      fprintf(stderr, "Error: Not a point: Need matrix or point as second argument to matrix-mul\n");
      return sc->NIL;
    }
    AffineTransform left(l);
    return scheme_point_to_list(sc, left * p);
  }
  SquareMatrix r;
  if (!scheme_extract_matrix(sc, args, 1, &r)) {
    fprintf(stderr, "Error: Not a matrix: Need matrix or point as second argument to matrix-mul\n");
    return sc->NIL;
  }
  return scheme_matrix_to_lists(sc, l*r);
}

