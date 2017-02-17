# Earth Observation Web Services

**Earth Observation Web Services (EOWS)** is a free and open source platform for the development of web services that must handle massive Earth Observation data.

The following services are under development:
- **Web Time Series Service (WTSS):** a lightweight web service for handling time series data from remote sensing imagery. Read more about it.

- **Web Coverage Service 2.0 (WCS):** a web coverage service for data exchange with SciDB.

- **Spatio-Temporal Web Map Service (WMS):** a web map service for visualizing remote sensing imagery data as dynamic maps (space + time) stored as 3D arrays in SciDB.

Subscribe to the mailing list: https://groups.google.com/d/forum/eows.

## Downloading EOWS

If you want the latest code from the master branch of the project, you can follow these instructions for cloning the project repository:
```bash
$ mkdir ~/eows/codebase
$ git clone http://github.com/eows/eows ~/eows/codebase/
```

## Dependencies

EOWS is built on top of the following libraries:
- [SciDB C++ API](http://www.paradigm4.com/): SciDB  version 15.12 or 16.9 is required.
- [Boost Libraries](http://www.boost.org): we recommend to use the same version used by SciDB (1.54.0). In general, it is installed under a folder such as ```/opt/scidb/15.12/3rdparty/boost```.
- [Proj.4](http://proj4.org): Cartographic projection library. Version 4.8.0 or above is required.
- [RapidJSON](https://github.com/miloyip/rapidjson): a fast JSON parser for C++. Version 1.1.0 is required.
- [Crow](https://github.com/ipkn/crow): a micro web framework in C++. Version. This library is not mandatory if C++ Network Library is used. 
- [C++ Network Library](https://github.com/cpp-netlib/cpp-netlib): branch 0.13-release. This library is not manadatory if Crow is used. 
- [RapidXML](http://rapidxml.sourceforge.net): a fast XML parser. Version 1.13 may be used.

## Building and Installing

To build EOWS you will need to have CMake version 3.1.2 or higher installed appropriately in your system. CMake is available at https://cmake.org.

To check your CMake version, try:
```bash
$ cmake --version
cmake version 3.7.2
```

### Linux Ubuntu 14.04 LTS

**1.** Let's assume that you plan to use SciDB version 15.12 and it can be found in the folder ```/opt/scidb/15.12```.

**2.** We are goin to install the third-party libraries into the folder ```~/eows/3rdparty```

**3.** Download the third-party libraries package we have prepared ```eows-3rdparty-0.1.0-linux-ubuntu-14.04.tar.gz``` to the following folder:
```bash
$ mkdir ~/eows/3rdparty-src

$ cd ~/eows/3rdparty-src

$ wget http://www.dpi.inpe.br/eows/3rdparty/eows-3rdparty-0.1.0-linux-ubuntu-14.04.tar.gz
```

**4.** Install the third-party libraries with the help of a bash script ```install-3rdparty-linux-ubuntu-14.04.sh``` under the ```bash``` folder, in the source code directory:
```bash
$ ~/eows/codebase/bash/install-3rdparty-linux-ubuntu-14.04.sh ~/eows/3rdparty /opt/scidb/15.12
```

The above command will install all the dependencies in the folder ```~/eows/3rdparty```.

**5.** Let's create a build directory out of the source code tree and make it the current directory:
```bash
$ mkdir ~/eows/build-release

$ cd ~/eows/build-release
```

**5.** You can prepare the build with CMake with a command line such as:
```bash
$ cmake -DCMAKE_BUILD_TYPE="Release" -DCMAKE_CXX_FLAGS:STRING="-I/opt/scidb/15.12/3rdparty/extern/" -DCMAKE_PREFIX_PATH:PATH="~/eows/3rdparty;/opt/scidb/15.12/3rdparty/boost" -DCMAKE_INSTALL_PREFIX:PATH="~/eows" -DCMAKE_INSTALL_RPATH:PATH="~/eows/lib" -DCMAKE_SKIP_BUILD_RPATH:BOOL="OFF" -DCMAKE_BUILD_WITH_INSTALL_RPATH:BOOL="OFF" -DCMAKE_INSTALL_RPATH_USE_LINK_PATH:BOOL="ON" ~/eows/codebase/build/cmake
```

**Note:** If you want to use SciDB 16.9, add the following option in the command line above: ```-DEOWS_SCIDB_MAJOR_VERSION=16```.

**6.** Make and install:
```bash
$ make -j 4

$ make install
```

## Configuring the Services

The file ```eows.json``` installed under the folder ```~/eows/share/eows/config``` contains several running options.

If you want to change the log file position, you need to modify the next line:
```json
"log_file": "../../eows/log/eows_%Y-%m-%d_%H-%M-%S.%N.log"
```

## Running the Services

After configuring EOWS, you can launch the application web server:
```bash
$ ~/eows/bin/eows_app_server --base-dir "/home/user/eows"
```

