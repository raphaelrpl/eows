#!/bin/bash
#
#  Description: Install all required software for Earth Observation Web Services on Mac OS Sierra.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#
#
#  Example:
#  $ ./install-3rdparty-linux-ubuntu-14.04.sh "/home/gribeiro/EOWS-Libs" "/opt/scidb/15.12"
#
#  $ /home/gribeiro/MyDevel/eows/codebase/bash/install-3rdparty-linux-ubuntu-14.04.sh /home/gribeiro/EOWS-Libs
#

echo "Installing 3rd-party libraries for Earth Observation Web Services..."
sleep 1s

EOWS_LIBS_DIR=$1
SCIDB_HOME=$2


#
# Valid parameter val or abort script
#
function valid()
{
  if [ $1 -ne 0 ]; then
    echo $2
    echo ""
    exit
  fi
}

#
# Check for tws-3rdparty-macosx-el-capitan.tar.gz
#
if [ ! -f ./eows-3rdparty-0.3.0-linux-ubuntu-14.04.tar.gz ]; then
  echo "Please, make sure to have eows-3rdparty-0.3.0-linux-ubuntu-14.04.tar.gz in the current directory!"
  exit
fi


#
# Extract packages
#
echo "extracting packages..."
sleep 1s

tar xzvf eows-3rdparty-0.3.0-linux-ubuntu-14.04.tar.gz
valid $? "Error: could not extract 3rd party libraries (eows-3rdparty-linux-ubuntu-14.04.tar.gz)"

echo "packages extracted!"
sleep 1s


#
# Go to 3rd party libraries dir
#
cd eows-3rdparty-0.3.0-linux-ubuntu-14.04
valid $? "Error: could not enter 'eows-3rdparty-0.3.0-linux-ubuntu-14.04' dir"

#
# Check for cmake 3.7.2
#
if [ ! -f ./cmake-3.7.2.tar.gz ]; then
  wget -O cmake-3.7.2.tar.gz https://cmake.org/files/v3.7/cmake-3.7.2.tar.gz
  valid $? "Error: Could not download cmake 3.7.2"
fi

#
# Qt5 (Required for CMake GUI)
#
sudo apt-get -y install qt5-default qttools5-dev qttools5-dev-tools libqt5svg5-dev libqt5designer5

#
# Installing CMake
#
cmake_expr_test=`cmake --version | awk 'NR==1{print $3}'` # Retrieving values only first line and third column
if [ "$cmake_expr_test" != "3.7.2" ]; then
  tar zxf cmake-3.7.2.tar.gz
  valid $? "Could not extract CMake 3.7.2 tar.gz"
  cd cmake-3.7.2
  ./configure --qt-gui
  make
  sudo make install

  cd ..
fi

#
# Check installation dir
#
if [ "$EOWS_LIBS_DIR" == "" ]; then
  EOWS_LIBS_DIR="/opt/eows-libs"
fi

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$EOWS_LIBS_DIR/lib"

echo "installing 3rd-party libraries to '$EOWS_LIBS_DIR' ..."
sleep 1s


#
# OpenSSL
#
libssl_dev_test=`dpkg -s libssl-dev | grep Status`

if [ "$libssl_dev_test" != "Status: install ok installed" ]; then
  sudo  apt-get -y install libssl-dev
  valid $? "Error: could not install libssl-dev! Please, install libssl-dev: sudo apt-get -y install libssl-dev"
  echo "libssl-dev installed!"
else
  echo "libssl-dev already installed!"
fi


#
# Proj.4
#
libproj_dev_test=`dpkg -s libproj-dev | grep Status`

if [ "$libproj_dev_test" != "Status: install ok installed" ]; then
  sudo  apt-get -y install libproj-dev
  valid $? "Error: could not install libproj-dev! Please, install libproj-dev: sudo apt-get -y install libproj-dev"
  echo "libproj-dev installed!"
else
  echo "libproj-dev already installed!"
fi

#
# LibGD
#
liblibgd_dev_test=`dpkg -s libgd-dev | grep Status`

if [ "$liblibgd_dev_test" != "Status: install ok installed" ]; then
  sudo  apt-get -y install libgd-dev
  valid $? "Error: could not install libgd-dev! Please, install libgd-dev: sudo apt-get -y install libgd-dev"
  echo "libgd-dev installed!"
else
  echo "libgd-dev already installed!"
fi


#
# RapidJSON
#
if [ ! -f "$EOWS_LIBS_DIR/include/rapidjson/rapidjson.h" ]; then
  echo "installing RapidJSON..."
  sleep 1s
  
  unzip -o rapidjson-1.1.0.zip
  valid $? "Error: RapidJSON!"

  mkdir -p rapidjson-build
  valid $? "Error: RapidJSON!"

  cd rapidjson-build
  valid $? "Error: RapidJSON!"

  cmake -DCMAKE_BUILD_TYPE:STRING="Release" -DCMAKE_PREFIX_PATH:PATH="$EOWS_LIBS_DIR" -DCMAKE_INSTALL_PREFIX:PATH="$EOWS_LIBS_DIR" ../rapidjson-1.1.0
  valid $? "Error: RapidJSON!"

  make -j 4 2> ../rapidjson-build.log
  valid $? "Error: RapidJSON!"
  
  make test 2> ../rapidjson-test.log
  valid $? "Error: RapidJSON!"

  make install 2> ../rapidjson-install.log
  valid $? "Error: RapidJSON!"
  
  cd ..
  valid $? "Error: RapidJSON!"

fi

#
# RapidXML
#
if [ ! -f "$EOWS_LIBS_DIR/include/rapidxml/rapidxml.hpp" ]; then
  echo "installing RapidXML..."
  sleep 1s

  unzip -o rapidxml-1.13.zip
  valid $? "Error: RapidXML!"

  mv rapidxml-1.13 $EOWS_LIBS_DIR/include/rapidxml
  valid $? "Error: RapidXML!"

fi


#
# Crow
#
if [ ! -f "$EOWS_LIBS_DIR/include/crow_all.h" ]; then
  echo "installing Crow..."
  sleep 1s

  unzip -o crow-master.zip
  valid $? "Error: Crow!"

  cp crow-master/amalgamate/crow_all.h $EOWS_LIBS_DIR/include/
  valid $? "Error: Crow!"

fi


#
# Finished!
#
echo " Finished installing Earth Observation Web Services 3rd-party Libraries!"
