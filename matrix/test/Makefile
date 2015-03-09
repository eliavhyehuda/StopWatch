# $Id: Makefile 166 2008-01-19 19:56:57Z hkuiper $

# CwMtx matrix and vector math library
# Copyright (C) 1999  Harry Kuiper

# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.

# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

APP=mattest

SRCS=mattest.cpp

OBJS=${SRCS:.cpp=.o}

INCS=-I..

LIBS=

CXX=g++
ifdef DEBUG
  CXXFLAGS=-g
else
  CXXFLAGS=-O2
endif

CPP=cpp
CPPFLAGS=-M

${APP}: ${OBJS}
	${CXX} ${OBJS} ${LIBS} -o $@

${OBJS}: %.o: %.cpp
	${CXX} -c ${CXXFLAGS} ${INCS} $< -o $@

depend:
	rm -f .depend
	for f in ${SRCS}; do ${CPP} ${CPPFLAGS} ${INCS} $$f >> .depend; done

clean:
	rm -f ${OBJS} ${APP}

distclean: clean
	rm -f .depend

-include .depend
