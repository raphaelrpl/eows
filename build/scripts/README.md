# Continuous Integration

Jenkins - [Link](http://www.dpi.inpe.br/jenkins/view/eows/)

## Structure

- **Dockerfile** - Represents a Docker configuration to generate a ubuntu 14.04 image with SciDB and EOWS 3rdparty dependencies installation;
- **build.sh** - Represents a bash script to compile EOWS on Linux Ubuntu 14.04;
- **eows.conf.cmake** - Represents a EOWS CMake cache options for compilation.