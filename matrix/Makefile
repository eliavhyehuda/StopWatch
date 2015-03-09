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

# Configure these settings

INC_INST_DIR=${HOME}/include/cwmtx

# Uncomment this line for debugging.
#DEBUG=yes

# DO NOT EDIT below this line unless you know what you are doing!

export DEBUG

HDRS=	stat_coordsys.h				\
	stat_cwmtx.h				\
	stat_matrix.h				\
	stat_quatern.h				\
	stat_smatrix.h				\
	stat_svector.h				\
	stat_vector.h

test-app:
	${MAKE} -C test

install:
	if ! [ -e ${INC_INST_DIR} ]; then \
	  mkdir ${INC_INST_DIR}; \
	fi
	cp ${HDRS} ${INC_INST_DIR}

depend:
	${MAKE} -C test depend

clean:
	${MAKE} -C test clean

distclean: clean
	${MAKE} -C test distclean
