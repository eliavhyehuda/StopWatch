/**
 * Series of routines to install and manage the TinyScheme
 * interpreter. Only one interpreter can be run at a time.
 * Author: Michael Terry
 */

#ifndef __CS349_SCHEME__
#define __CS349_SCHEME__

#include "scheme/tinyscheme/scheme-private.h"
#include "scheme/tinyscheme/scheme.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * Initializes the interpreter.
   * @param scheme_init_fname The init.scm file used to read in common
   *                          Scheme definitions
   * @param cs349_init_fname  The cs349_init.scm file that contains useful
   *                          definitions for working with the GUI
   *                          toolkit
   * @param argc Command-line argument count
   * @param argv Command-line arguments. Used for executing files
   * named on the command line.
   * @return 0 if no error, non-zero on error
   */
  int scheme_interpreter_init(const char* scheme_init_fname, const char* cs349_init_fname, int argc, char** argv);

  /**
   * Must be called regularly to check for keyboard input and to allow
   * the scheme interpreter to run.
   * @return 0 if no error, 1 if the user entered Ctrl-D on the
   *         terminal (indicating the app should quit), and a negative
   *         value on error.
   */
  int scheme_interpreter_service();

  /**
   * Executes the set of commands specified by s.
   * @return 0 if no error, non-zero on error
   */
  int scheme_interpreter_execute_string(const char* s);

  /**
   * Evaluates the string represented by s and returns the result
   * (along with a pointer to the Scheme interpreter) via the
   * arguments sc and results.
   * @return 0 if no error on evaluating string, non-zero on error
   */
  int scheme_interpreter_evaluate_string(const char* s, scheme** outSC, pointer* outResults);

  /**
   * Executes the code found in the given file.
   * @return 0 if no error, non-zero on error
   */
  int scheme_interpreter_execute_file(const char* scheme_file_name);

  /**
   * Call when ready to shut down the app. After calling this, do not
   * call scheme_interpreter_service() anymore.
   * @return The interpreter's return code.
   */
  int scheme_interpreter_shutdown();

  /**
   * This is a special-purpose function for executing a graphics operation.
   */
  int scheme_interpreter_execute_graphics_instruction(long window_id, const char* op_args_list_string);


#ifdef __cplusplus
}
#endif

#endif /* __CS349_SCHEME__ */
