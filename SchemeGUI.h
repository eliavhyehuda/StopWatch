#ifndef __SCHEME_GUI__
#define __SCHEME_GUI__

#include "scheme/tinyscheme/scheme.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * Installs a set of routines for marshalling data to/from the
   * Scheme interpreter for Graphics objects, Components,
   * SchemeComponents, and TurtleGraphics.
   */
  int scheme_install_gui_procedures(scheme* sc);

#ifdef __cplusplus
}
#endif

#endif /* __SCHEME_GUI__ */
