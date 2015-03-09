# $Id: Makefile 166 2008-01-19 19:56:57Z hkuiper $

# Makefile for qfilter utility.
# Copyright (C) 2004 Harry Kuiper, hkuiper@xs4all.nl

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

# Uncomment this line for debugging.
# DEBUG=yes

APP=qfilter

SRCS=${APP}.cc

OBJS=${SRCS:.cc=.o}

INCS=-I../../

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

${OBJS}: %.o: %.cc
	${CXX} -c ${CXXFLAGS} ${INCS} $< -o $@

depend:
	rm -f .depend
	for f in ${SRCS}; do ${CPP} ${CPPFLAGS} ${INCS} $$f >> .depend; done

clean:
	rm -f ${OBJS} ${APP}

distclean: clean
	rm -f .depend

-include .depend
