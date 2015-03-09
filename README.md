@mainpage

[TOC]

# Making The Documentation # {#docs}

Type:

    make docs

to create and view this documentation in the `docs/`
directory. This will create documentation for this file and the API.

Once the documentation has been generated, open the `index.html` file in the `docs/html/` directory. Use the [Classes](./annotated.html) and [Files](./files.html) buttons up top to view the API documentation. The [Class Hierarchy](./hierarchy.html) is also useful.

We use doxygen to create the documentation, and have set documentation
generation to be very verbose. The documentation for every function
includes the actual code within the documentation, along with links to
its definition in the source code, and links to functions it
references and that reference it. You are free to adjust this
verbosity: Edit doxygen.cfg to suit your tastes.

# The WatGUI Toolkit: Overview # {#intro}

The WatGUI toolkit is a very basic X11-based graphical user interface
(GUI) toolkit. The intent of the toolkit is to give you an opportunity
to learn about the internals of a GUI toolkit through a relatively
small code base.

The toolkit also demonstrates how to integrate a scripting engine with
an application. We have tightly integrated TinyScheme, a very small
Scheme interpreter with the toolkit. Briefly, the Scheme interpreter
allows you to directly interact with the running application, creating
new components on-the-fly, or adjusting existing components'
attributes. More documentation about the Scheme integration can be
found below.

# Finishing the WatGUI Toolkit # {#finishing}

The provided toolkit will compile, but is incomplete. You need to
complete the toolkit's missing parts for it to run correctly.

The parts that need to be completed are clearly marked in the source
code. Search for "TODO" comments that include "CS349". These parts are
also marked with TODO logging comments.

The missing parts of the toolkit include comments and hints to help
you complete the missing code. The included unit tests will also help you validate and verify the toolkit is working as expected.

At present, you will need to fill in parts in:

+ `Component.cpp`
+ `EventQueue.cpp`
+ `PaintEvent.cpp`
+ `XApplication.cpp`


# Logging # {#logging}

This project uses [Google's logging library for logging](http://google-glog.googlecode.com/svn/trunk/doc/glog.html).

To use the library, add

    #include "Logging.h"

to your C++ code.

## Logging in Code ## {#logging_in_code}

We're using the library's verbose logging features to help manage
logging. If you open `Logging.h`, you'll notice the following macros
are defined:

+ `LOG_TODO`
+ `LOG_PAINT`
+ `LOG_DEBUG`
+ `LOG_INFO`
+ `LOG_WARN`
+ `LOG_ERROR`
+ `LOG_FATAL` 

To log something, use normal C++ streams operators. For example:

    LOG_DEBUG << "Value returned: " << value;

Note that you don't need an `endl` at the end of the logging
comment.

## Logging Levels ## {#logging_levels}

Google's logging library has two basic forms of logging macros, the
"normal" logging macro (`LOG(level)`) and the verbose logging macro
(`VLOG(level)`). The verbose logging macro provides greater
flexibility in terms of controlling what log messages are output at
runtime, so we use it for the TODO, paint, debug, info, and warning
levels. More information about how to control what is output is
provided in a section below.

`LOG_TODO` logs sections of the toolkit that need to be
implemented by you. `LOG_PAINT` logs painting operations to debug
drawing to the screen. `LOG_ERROR` and `LOG_FATAL` are used to log
unexpected states of the running application.

**When logging messages for your own benefit (i.e., during development
and during debugging), you should only use macros that use the VLOG
macros** (such as `LOG_PAINT`, `LOG_TODO`, `LOG_DEBUG`, `LOG_INFO`,
and `LOG_WARN`, or ones you define using VLOG). Using these macros
allows you to keep logging comments in your code when submitting it
because we can execute your binary with these logging messages
disabled (as described below).

Error and fatal levels can also be inserted in your code, but should
only be used for true error/fatal conditions. These *will* be output
during marking. *Furthermore, LOG_FATAL messages will forcibly
terminate the application with a core dump, so use with care.*

You are free to define new levels of logging for your own purposes --
use the debug, info, and warn macros as templates. Any new logging
levels you define using the VLOG macro will not be output during marking.

## Enabling Logging at Runtime and Specifying Which Files to Log ## {#logging_config}

To output log messages, you need to set an environment flag
corresponding to the minimum logging level you wish to use. main.cpp
specifies that all logging messages should be output to stderr.

To output all log messages at the TODO level or lower (where "TODO"
marks all the "to-do" points in the code), start the app from the
command line like this:

    GLOG_v=70 ./watgui

`LOG_TODO` is mapped to verbose logging level 70, so by setting the
Google logging flag "v" to 70, it ensures all to-do messages (and all
other log messages with lower logging levels, including paint and debug
messages) are logged.

*For your convenience, we have defined the shell script
 `debug_app.sh`, which makes your project, then executes the app with
 the most verbose logging level.* Use it like this:
 
     ./debug_app.sh a1 [any other command-line arguments to pass to the app]

You can also enable logging on a per-file basis, which is *very* useful
when targeting a specific bug. To do so, use a comma-separated list of
file name globs, along with the logging level.

For example, to show debug messages from all Event* files and
info messages from Component.cpp, set the environment variables like
this:

    GLOG_vmodule='Event*=50,Component=40' ./watgui

Note the use of quotes (') to ensure the glob operator (*) is not
interpreted by the shell. *Also note that the comma-separated list of
files cannot have a space between items.*

## Logging Level for Assignment Marking ## {#logging_marking}

To mark your application, we will execute it using the
`mark_app.sh` shell script (included), which runs your
application as:

    GLOG_v=0 ./app_name

## Setting Logging Levels via Scheme ## {#logging_scheme}

We have also provided a number of functions to enable you to set
logging levels at runtime via the Scheme interpreter. For example, to
set to debug logging levels, you can use:

    (logging-set-to-debug)
    
See similar logging routines in `cs349_init.scm`.

# Unit Tests # {#unit_tests}

We have included a number of unit tests to help you test/verify your
code, once you have made the necessary additions to the provided
source code. We are using [Google's unit testing framework](http://code.google.com/p/googletest/wiki/V1_7_Documentation) for testing.

Unit tests will be automatically built by the Makefile and output as a
separate program called `x_unittests` where "x" will correspond to the
app name (e.g., `a1_unittests`). Executing this will run the unit
tests.

Included in the build is a file MarkingUnitTests.cpp. This file is
empty and should remain so -- it will be replaced by our own unit
tests for marking purposes.

Writing a unit test is pretty straightforward. In a C++ file, create a
new test using a TEST block:

    TEST(test_case_name, test_name) {
     // test code
    }

For example, a very simple test of the EventQueue object:

    TEST(EventQueueTests, EventQueuing) {
      EventQueue eventQueue;
      EXPECT_EQ(0, eventQueue.GetNumEventsInQueue());
      MemoryTrackingEvent* event = new MemoryTrackingEvent();
      eventQueue.AddEventToQueue(event);
      EXPECT_EQ(1, eventQueue.GetNumEventsInQueue());
      delete event;
    }

See the
  [Google test documentation](http://code.google.com/p/googletest/wiki/V1_7_Documentation)
  or the included unit tests for more examples of how to write your
  own unit tests.
  
## Adding Unit Tests to the Build Process ## {#adding_unit_tests}

To add unit tests to the build process, add them in the section of the
Makefile where indicated. They will be automatically linked in to
the app and automatically added to all of the other tests (you
don't need to explicitly register the tests with anything).


# Makefile and Adding Files # {#makefile}

The Makefile is documented, and indicates where you should add files.

# Scheme Interpreter # {#scheme}

The interpreter is a very slightly modified version of TinyScheme,
which implements a subset of R5RS Scheme. A list of the Scheme
commands built-in to the interpreter can be found in
scheme/scheme_cmds.txt.

The goal of having the interpreter is to help you experiment with the
GUI toolkit without needing to always write C/C++ code (thus avoiding
compile and test cycles). Once you have implemented the missing parts
of the toolkit, you should be able to perform many of the same GUI
operations you would do in C++ using the Scheme interpreter.

*You are not required to understand any of the code provided that is
related to embedding the interpreter in the application. Nearly any
file that starts with "Scheme" can be ignored by you.*

That file `cs349_scheme_tests.scm` serves to document many of the
functions available via the interpreter. `cs349_init.scm` provides
more functions of interest and relevance to your work.

To execute any typed input, hit RETURN twice or press Ctrl-G. To quit
the interpreter (and the app), press Ctrl-D.

**Note**: If you application terminates in an unexpected manner and
  doesn't cleanly exit, the terminal can get a little unpredictable
  since the readline library didn't properly shut down. The effects of
  this are visible if you try to ssh into a remote host -- after it
  asks for your password, it won't display anything else on the
  terminal. Note that this is not the only symptom of this problem --
  if things start getting a little weird and regular apps don't print
  out on the console as expected, you may need to start a new
  terminal.

The Scheme interpreter will try to run any files passed to the
executable on the command line. For example, the following will
execute the Scheme demo/test file:

    ./a1 cs349_scheme_tests.scm

To help debug Scheme scripts, you can turn tracing on/off while in the interpreter:

    (tracing 1) ;; Turns tracing on
    (tracing 0) ;; Turns tracing off

# Adding Scheme Scripts # {#scheme_scripts}

You are free to add new Scheme scripts. You can add them to
student-scripts.scm, which will be automatically loaded after all
other scripts have been loaded. (It is loaded at the end of
cs349_init.scm).
