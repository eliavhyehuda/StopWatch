.PHONY: all docs clean distclean

# See below for how to add your own source files to the build

# Logging and testing libraries
GLOG_SRC= glog
GTEST_SRC = gtest

GLOG_DIR = $(GLOG_SRC)/glog
GTEST_DIR = $(GTEST_SRC)/gtest

GLOG_A = $(GLOG_DIR)/lib/libglog.a
GTEST_A = $(GTEST_DIR)/lib/gtest.a
GTEST_MAIN_A = $(GTEST_DIR)/lib/gtest_main.a

# Scheme directory
SCHEME_DIR = scheme/tinyscheme

# Where it puts documentation for the code base when you
# run make docs
DOCS_DIR = docs

# Compile flags
CC = gcc -pedantic -pthread
LD = gcc
DEBUG = -g -Wall -Wno-char-subscripts -Wno-long-long -Wno-variadic-macros -O
SYS_LIBS = -lm -lreadline -lstdc++ -lX11 -lrt $(GLOG_A)
RM = -rm -f
INCLUDES = -I. -I$(GLOG_DIR)/include -I$(GTEST_DIR)/include 

SCHEME_FEATURES = -DUSE_DL=0 -DUSE_MATH=1 -DUSE_ASCII_NAMES=0 -DSTANDALONE=0 -DUSE_INTERFACE=1


# Targets. Builds the logging and testing libraries first, then the
# watgui app and the unittests app
all: $(GLOG_A) $(GTEST_A) $(GTEST_MAIN_A) watgui_demo a1 a1_unittests


# We build the documentation with doxygen
docs:
	doxygen doxygen.cfg

# Common objects (everything except files with main defined, to allow linking
# with a different main function (for example, for unit tests)).
# *** If you add new files, add them here, following the same pattern ***
OBJS =		AffineTransform.o \
		ComponentEvent.o \
		Component.o \
		Event.o \
		EventQueue.o \
		Graphics.o \
		KeyEvent.o \
		MouseEvent.o \
		PaintEvent.o \
		Point.o \
		Rectangle.o \
		SchemeAffineTransform.o \
		SchemeComponent.o \
		SchemeGUI.o \
		SchemeInterpreter.o \
		SchemeUtils.o \
		Timer.o \
		TimerEvent.o \
		TimerListener.o \
		XApplication.o \
		XWindow.o

A1_OBJS = 	StopWatch.o

# Objects used for testing
# *** If you add new unit tests, add them here ***
A1_UNIT_TEST_OBJS = 	AffineTransformUnitTests.o \
			XApplicationUnitTests.o \
			EventQueueUnitTests.o \
			A1UnitTests.o \
			MarkingUnitTests.o

WATGUI_DEMO_OBJS = demo_main.o

# Object dependencies (a *very* coarse-level dependency graph)
# *** If you add new .h files, add them here ***
$(OBJS):	$(SCHEME_DIR)/scheme.h \
		$(SCHEME_DIR)/scheme-private.h \
		$(SCHEME_DIR)/opdefines.h \
		$(SCHEME_DIR)/scheme.c \
		AffineTransform.h \
		ComponentEvent.h \
		Component.h \
		Event.h \
		EventQueue.h \
		Graphics.h \
		KeyEvent.h \
		Logging.h \
		MouseEvent.h \
		PaintEvent.h \
		Point.h \
		Rectangle.h \
		SchemeAffineTransform.h \
		SchemeComponent.h \
		SchemeGUI.h \
		SchemeInterpreter.h \
		SchemeUtils.h \
		Timer.h \
		TimerEvent.h \
		TimerListener.h \
		XApplication.h \
		XWindow.h

# The Scheme interpreter needs a set of different flags than the other
# files
SchemeInterpreter.o: SchemeInterpreter.c
	$(CC) $(INCLUDES) -c $(DEBUG) $(SCHEME_FEATURES) $<

# Rule to compile the .cpp files
%.o: %.cpp
	$(CC) $(INCLUDES) -c $(DEBUG) $<

# The watgui demo app
watgui_demo: $(OBJS) $(WATGUI_DEMO_OBJS)
	$(CC) -o $@ $(DEBUG) $(OBJS) $(WATGUI_DEMO_OBJS) $(LDFLAGS) $(SYS_LIBS)

a1: $(OBJS) $(A1_OBJS) a1_main.o StopWatch.h
	$(CC) -o $@ $(DEBUG) $(OBJS) $(A1_OBJS) a1_main.o $(LDFLAGS) $(SYS_LIBS)

# The unittests app
a1_unittests: $(OBJS) $(A1_OBJS) $(A1_UNIT_TEST_OBJS)
	$(CC) -o $@ $(DEBUG) $(OBJS) $(A1_OBJS) $(A1_UNIT_TEST_OBJS) $(GTEST_A) $(GTEST_MAIN_A) $(LDFLAGS) $(SYS_LIBS) 

# Below are rules to build the logging and testing libraries
$(GLOG_A):
	cd $(GLOG_SRC) && $(MAKE)

$(GTEST_A):
	cd $(GTEST_SRC) && $(MAKE)

$(GTEST_MAIN_A):
	cd $(GTEST_SRC) && $(MAKE)

# To clean all of the app's outputs
clean:
	$(RM) $(OBJS) $(A1_OBJS) $(A1_UNIT_TEST_OBJS) $(TEST_OBJS) $(WATGUI_DEMO_OBJS) watgui_demo a1 a1_unittests a1_main.o 

# To clean *everything*, including the docs and test and logging libraries
distclean:
	$(MAKE) clean
	cd $(GTEST_SRC) && $(MAKE) clean
	cd $(GLOG_SRC) && $(MAKE) clean
	cd $(DOCS_DIR) && rm -Rf html latex
