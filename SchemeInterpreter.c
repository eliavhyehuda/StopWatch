/**
 * This program uses GNU readline, and hence is licensed under the GPL.
 * Portions of this code derived from TinyScheme's scheme.c
 * Author: Michael Terry
*/

/* Uncomment the next line if you want to compile the main() function
   below
*/
/* #define __STANDALONE_INTERPRETER__ */

#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "scheme/tinyscheme/scheme-private.h"
#include "scheme/tinyscheme/scheme.h"
#include "SchemeInterpreter.h"
#include "SchemeAffineTransform.h"
#include "SchemeGUI.h"

#include "scheme/tinyscheme/scheme.c"

scheme sc; /* The interpreter */
int ready_to_submit = 0; /* Whether we are ready to submit the user's
                            typed-in input to the interpreter.
                            Currently, if we get a blank line, or the
                            user types Ctrl-G, then we submit
                         */
char* cur_input = NULL; /* The current typed-in input */
int got_EOF = 0; /* Whether the user has typed an EOF or not */
const char* PROMPT = "ts> ";

int  init_interpreter(scheme* sc, const char* scheme_init_fname);
int  load_scheme_script(scheme* sc, const char* fname);
void run_cmd_line_scripts(scheme* sc, int argc, char** argv);
void init_readline();
int  handle_ctrl_g(int count, int key);
void readline_callback(char* new_line);
void update_cur_input(char** cur_input, const char * new_line);


int init_interpreter(scheme* sc, const char* scheme_init_fname) {
  if(!scheme_init(sc)) {
    fprintf(stderr,"Could not initialize interpreter!\n");
    return -1;
  }
  scheme_set_output_port_file(sc, stdout);
  return load_scheme_script(sc, scheme_init_fname);
}

int load_scheme_script(scheme* sc, const char* fname) {
  FILE* fin = NULL;
  fin = fopen(fname, "r");
  if (!fin) {
    fprintf(stderr, "Could not open file %s\n", fname);
    return -1;
  } else {
    scheme_load_named_file(sc, fin, fname);
    fclose(fin);
    fin = NULL;
    if (sc->retcode != 0) {
      fprintf(stderr,"Errors encountered reading %s\n", fname);
    }
    return sc->retcode;
  }
  return 0;
}

void run_cmd_line_scripts(scheme* sc, int argc, char** argv) {
  /* Execute any files specified on command line */
  if (argc > 1) {
    int i;
    for (i = 1; i < argc; i++) {
      const char* fname = argv[i];
      load_scheme_script(sc, fname);
    }
  }
}

void init_readline() {
  const int CTRL_G = 7; /* ASCII value for Ctrl-G */
  rl_unbind_key(CTRL_G);
  rl_bind_key(CTRL_G, handle_ctrl_g);
  rl_variable_bind("blink-matching-paren","on");
}

int handle_ctrl_g(int count, int key) {
  ready_to_submit = 1;
  rl_done = 1;
  printf("\n");
  return 0;
}

void readline_callback(char* new_line) {
  if (!new_line) {
    free(cur_input);
    cur_input = NULL;
    got_EOF = 1;
    rl_callback_handler_remove();
    return;
  }
  if (strlen(new_line) > 0) {
    update_cur_input(&cur_input, new_line);
    add_history(new_line);
  }
  if (strlen(new_line) == 0) {
    ready_to_submit = 1;
  }
  free(new_line);
  new_line = NULL;
  if (!ready_to_submit) {
    rl_set_prompt("... ");
  } else {
    rl_callback_handler_remove(); /* Remove the handler to re-install
                                     it after the interpreter's output
                                     has been printed
                                  */
  }
}

void update_cur_input(char** cur_input, const char* new_line) {
  /* And this code is why programming in C is no picnic. I mean, all
     I want to do is concatenate and/or assign some new strings safely.
     How hard can that be? Here's your answer... */
  if (*cur_input == 0) {
    *cur_input = malloc(strlen(new_line)+1);
    strcpy(*cur_input, new_line);
  } else {
    size_t new_str_size = strlen(*cur_input)+strlen(new_line)+2; /* 2: 1 for terminating character, 1 for newline */
    char* new_str = malloc(new_str_size);
    int result = 0;
    result = snprintf(new_str, new_str_size, "%s\n%s", *cur_input, new_line);
    free(*cur_input);
    *cur_input = new_str;
    if (result < 0 || result >= new_str_size) {
      fprintf(stderr, "Error concatenating strings: %d/%ld\n", result, new_str_size);
    }
  }
}

int scheme_interpreter_service() {
  fd_set fds;
  int retval = 0;
  struct timeval timeout;

  FD_ZERO(&fds);
  FD_SET(fileno(stdin), &fds);
  timeout.tv_sec  = 0;
  timeout.tv_usec = 0;
  retval = select(fileno(stdin)+1, &fds, NULL, NULL, &timeout);

  if (retval < 0) {
    fprintf(stderr, "Error selecting on stdin\n");
    return retval;
  } else if (retval == 0) {
    ; /* Timeout. Ignore. */
  } else if (FD_ISSET(fileno(stdin), &fds)) {
    rl_callback_read_char();
  }
  if (ready_to_submit) {
    ready_to_submit = 0;
    if (cur_input != NULL) {
      scheme_load_string(&sc, cur_input);
      printf("\n");
      free(cur_input);
      cur_input = NULL;
    }
    rl_callback_handler_install(PROMPT, readline_callback);
  }
  if (got_EOF) {
    return 1;
  }
  return 0;
}

int scheme_interpreter_execute_string(const char* s) {
  scheme_load_string(&sc, s);
  return 0;
}

int scheme_interpreter_evaluate_string(const char* s, scheme** outSC, pointer* outResults)
{
  scheme_load_string_silent(&sc, s);
  if (outSC != NULL) {
    (*outSC) = &sc;
  }
  if (outResults != NULL) {
    (*outResults) = sc.value;
  }
  return 0;
}


int scheme_interpreter_execute_file(const char* scheme_file_name) {
  return load_scheme_script(&sc, scheme_file_name);
}

int scheme_interpreter_execute_graphics_instruction(long window_id, const char* op_args_list_string) {
  scheme_eval(&sc, cons(&sc, mk_symbol(&sc, "scheme-component-execute-instruction"),
                       cons(&sc, mk_integer(&sc, window_id),
                            cons(&sc, mk_string(&sc, op_args_list_string), sc.NIL))));
  return 0;
}

int scheme_interpreter_init(const char* scheme_init_fname, const char* cs349_init_fname, int argc, char** argv) {
  int retcode = 0;

  retcode = init_interpreter(&sc, scheme_init_fname);
  if (retcode != 0) {
    return retcode;
  }
  scheme_install_affine_procedures(&sc);
  scheme_install_gui_procedures(&sc);
  load_scheme_script(&sc, cs349_init_fname);
  run_cmd_line_scripts(&sc, argc, argv);
  init_readline();
  printf("\n");
  printf("TinyScheme 1.41 Interpreter\n\n");
  printf("Press RETURN twice or CTRL-G to submit input to the interpreter.\n");
  printf("Press CTRL-D to exit the interpreter.\n\n");
  rl_callback_handler_install(PROMPT, readline_callback);
  return retcode;
}

int scheme_interpreter_shutdown() {
  int retcode = 0;
  rl_callback_handler_remove();
  retcode = sc.retcode;
  scheme_deinit(&sc);
  return retcode;
}

#ifdef __STANDALONE_INTERPRETER__
/* Used for testing */
int main(int argc, char **argv) {
  const char* scheme_init_fname       = "init.scm";
  const char* scheme_cs349_init_fname = "cs349_init.scm";
  int         retcode                 = 0;

  retcode = scheme_interpreter_init(scheme_init_fname, scheme_cs349_init_fname, argc, argv);
  if (retcode) {
    fprintf(stderr, "Error init'ing interpreter: %d\n", retcode);
    return retcode;
  }

  do {
    retcode = scheme_interpreter_service();
    usleep(10000);
  } while (retcode == 0);

  return scheme_interpreter_shutdown();
}
#endif /* __STANDALONE_INTERPRETER__ */
