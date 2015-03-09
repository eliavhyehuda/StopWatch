#ifndef __SCHEME_AFFINE_TRANSFORM__
#define __SCHEME_AFFINE_TRANSFORM__

#include "scheme/tinyscheme/scheme.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * Installs functions to marshall AffineTransforms to/from the
   * Scheme interpreter
   */
  int scheme_install_affine_procedures(scheme* sc);

#ifdef __cplusplus
}
#endif

#endif /* __SCHEME_AFFINE_TRANSFORM__ */
