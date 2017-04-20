#!/bin/bash

#
# Copyright (C) 2017 National Institute For Space Research (INPE) - Brazil.
#
# This file is part of Earth Observation Web Services (EOWS).
#
# EOWS is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 3 as
# published by the Free Software Foundation.
#
# EOWS is distributed  "AS-IS" in the hope that it will be useful,
# but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with EOWS. See LICENSE. If not, write to
# e-sensing team at <esensing-team@dpi.inpe.br>.

#
# Description: This script compile EOWS in Linux Ubuntu 14.04 and 16.04 systems.
#
# Author: Raphael Willian da Costa
#
# Usage: cd build/scripts && ./build.sh
#

#
# Defining constants to help during compile process
#
# EOWS_CODEBASE - Points to Codebase root. You may export it before if need expose it outside codebase path
if [ -z "$EOWS_CODEBASE" ]; then
  export EOWS_CODEBASE=`pwd`/../..
fi

# EOWS_3RDPARTY - Points to EOWS 3rdparties.
if [ -z "$EOWS_3RDPARTY" ]; then
  export EOWS_3RDPARTY="$HOME/mydevel/eows/3rdparty"
fi

# EOWS_BOOST_DIR - Points to EOWS 3rdparties.
if [ -z "$EOWS_BOOST_DIR" ]; then
  export EOWS_BOOST_DIR="/opt/scidb/15.12/3rdparty/boost"
fi

# EOWS_SCIDB_DIR - Path to SciDB installation
if [ -z "$EOWS_SCIDB_DIR" ]; then
  export EOWS_SCIDB_DIR="/home/scidb/mydevel/scidbtrunk/stage/install"
fi

# EOWS_BUILD_DIR - Path to EOWS build.
if [ -z "$EOWS_BUILD_DIR" ]; then
  export EOWS_BUILD_DIR="$EOWS_CODEBASE/../build-cmake"
fi

# Creating target directory
mkdir -p $EOWS_BUILD_DIR
# Copying EOWS cache to target directory
cp $EOWS_CODEBASE/build/scripts/eows.conf.cmake $EOWS_BUILD_DIR

cd $EOWS_BUILD_DIR
# Generate Build makefiles
cmake -G "Unix Makefiles" -C eows.conf.cmake \
                             -DCMAKE_BUILD_TYPE="Release" \
                             $EOWS_CODEBASE/build/cmake

# Compile
make -j 4

exit $?