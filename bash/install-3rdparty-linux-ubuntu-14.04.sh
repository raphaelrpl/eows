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
if [ ! -f ./eows-3rdparty-0.5.0-linux-ubuntu-14.04.tar.gz ]; then
  echo "Please, make sure to have eows-3rdparty-0.5.0-linux-ubuntu-14.04.tar.gz in the current directory!"
  exit
fi


#
# Extract packages
#
echo "extracting packages..."
sleep 1s

tar xzvf eows-3rdparty-0.5.0-linux-ubuntu-14.04.tar.gz
valid $? "Error: could not extract 3rd party libraries (eows-3rdparty-0.5.0-linux-ubuntu-14.04.tar.gz)"

echo "packages extracted!"
sleep 1s


#
# Go to 3rd party libraries dir
#
cd eows-3rdparty-0.5.0-linux-ubuntu-14.04
valid $? "Error: could not enter 'eows-3rdparty-0.5.0-linux-ubuntu-14.04' dir"

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
# Check CMake version. EOWS requires 3.x
#
cmake_ver_test=`cmake --version | awk 'NR==1{print substr($3,0,2)}'`
cmake_curr_ver=`cmake --version | awk 'NR==1{print $3}'`
if [ $cmake_ver_test -le 2 ]; then
  valid 1 "Error: EOWS requires CMake >= 3.x version but $cmake_curr_ver found. Please install CMake 3.x."
else
  echo "CMake installation ok. Version $cmake_curr_ver > 2.X"
fi

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

## GDAL Dependencies ##

#
# zlibdevel version 1.2.8
#
zlibdevel_test=`dpkg -s zlib1g-dev | grep Status`

if [ "$zlibdevel_test" != "Status: install ok installed" ]; then
  sudo apt-get -y install zlib1g-dev
  valid $? "Error: could not install zlib1g-dev! Please, install g++: sudo apt-get -y install zlib1g-dev"
  echo "zlib1g-dev installed!"
else
  echo "zlib1g-dev already installed!"
fi

#
# hdf4 version 4.2
#
hdf4_test=`dpkg -s libgeos++-dev | grep Status`

if [ "$hdf4_test" != "Status: install ok installed" ]; then
  sudo apt-get -y install libhdf4-alt-dev
  valid $? "Error: could not install hdf4! Please, install readline: sudo apt-get -y install libhdf4-alt-dev" 
  echo "hdf4 installed!"
else
  echo "hdf4 already installed!"
fi

#
# libPNG version 1.6.20
#
png_test=`dpkg -s libpng12-dev | grep Status`

if [ "$png_test" != "Status: install ok installed" ]; then
  sudo apt-get -y install libpng12-dev
  valid $? "Error: could not install png! Please, install readline: sudo apt-get -y install libpng12-dev" 
  echo "png installed!"
else
  echo "png already installed!"
fi

#
# GeoTIFF version 1.4.0
# Dependencies: zlib, proj, libtiff, jpeg
#
geotiff_test=`dpkg -s libgeotiff-dev | grep Status`

if [ "$geotiff_test" != "Status: install ok installed" ]; then
  sudo apt-get -y install libgeotiff-dev
  valid $? "Error: could not install geotiff! Please, install readline: sudo apt-get -y install libgeotiff-dev" 
  echo "geotiff installed!"
else
  echo "geotiff already installed!"
fi

#
# Jansson (OAuth - JWTXX Dependency)
# Dependencies: openssl
#
jansson_test=`dpkg -s libjansson-dev | grep Status`

if [ "$jansson_test" != "Status: install ok installed" ]; then
  sudo apt-get -y install libjansson-dev
  valid $? "Error: could not install jansson! Please, install readline: sudo apt-get -y install libjansson-dev" 
  echo "jansson installed!"
else
  echo "jansson already installed!"
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
# GDAL 2.1.3
#
if [ ! -f "$EOWS_LIBS_DIR/lib/libgdal.so" ]; then
  echo "Installing GDAL..."

  tar zxf gdal-2.1.3.tar.gz
  valid $? "Error: Could not extract gdal 2.1.3"

  cd gdal-2.1.3
  valid $? "Error: Could not enter gdal dir"

  CPPFLAGS="-I/usr/include -I/usr/include/libxml2" LDFLAGS="-L/usr/lib -Wl,-rpath,/usr/lib" ./configure --prefix=$EOWS_LIBS_DIR --with-png --with-libtiff --with-geotiff --with-jpeg --with-gif --with-ecw=yes --with-expat=yes --with-threads --without-python --with-hdf4 --without-netcdfd

  PREFIX=$EOWS_LIBS_DIR make -j 4 -s
  valid $? "Error: Could not compile GDAL"

  PREFIX=$EOWS_LIBS_DIR make install -j 4
  valid $? "Error: Could not install GDAL into $EOWS_LIBS_DIR"

  cd ..
fi

#
# jwtxx-1.1.6
#
if [ ! -f "$EOWS_LIBS_DIR/lib/libjwtxx.a" ]; then
  echo "Installing jwtxx..."

  tar zxf jwtxx-1.1.6.tar.gz
  valid $? "Error: Could not extract jwtxx-1.1.6.tar.gz"

  cd jwtxx
  valid $? "Error: Could not enter jwtxx directory"
  
  mkdir -p build-release  
  cd build-release
  
  cmake -DCMAKE_BUILD_TYPE:STRING="Release" -DCMAKE_PREFIX_PATH="$EOWS_LIBS_DIR" -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_INSTALL_PREFIX="$EOWS_LIBS_DIR" ../
  valid $? "Error: Could not configure cmake build for jwtxx"

  make install
  valid $? "Error: Could not compile or install jwtxx"

  cd ..
fi


#
# Finished!
#
echo " Finished installing Earth Observation Web Services 3rd-party Libraries!"
