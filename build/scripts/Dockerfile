## 
##  Copyright (C) 2017 National Institute For Space Research (INPE) - Brazil.
##  This file is part of Earth Observation Web Services (EOWS).
##  EOWS is free software: you can redistribute it and/or modify
##  it under the terms of the GNU General Public License version 3 as
##  published by the Free Software Foundation.
##  EOWS is distributed  "AS-IS" in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
##  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
##  GNU Lesser General Public License for more details.
##  You should have received a copy of the GNU Lesser General Public License
##  along with EOWS. See LICENSE. If not, write to
##  e-sensing team at <esensing-team@dpi.inpe.br>.
##
## Description
## ---
## It creates a environment for Continuous Integration Server (Jenkins). It installs SciDB 15.12
## and install EOWS 3rdparty dependencies
##

## ===
## Based in rvernica/scidb-example https://github.com/rvernica/scidb-examples/tree/master/docker-image

## Requirements
## ---
FROM ubuntu:14.04
RUN apt-get update
RUN apt-get install -y wget apt-transport-https software-properties-common

## Installation Notes
## ---
ARG host_ip=127.0.0.1
ARG net_mask=$host_ip/8
ARG scidb_usr=scidb
ARG dev_dir=/home/$scidb_usr/mydevel
ARG EOWS_PATH=$dev_dir/eows
ARG EOWS_DEPENDENCIES_DIR=$EOWS_PATH/3rdparty
ARG EOWS_CODEBASE=$EOWS_PATH/codebase
ARG EOWS_BUILD_DIR=$EOWS_PATH/build
ARG EOWS_INSTALL_DIR=$EOWS_PATH/install

RUN groupadd $scidb_usr
RUN useradd $scidb_usr -s /bin/bash -m -g $scidb_usr
RUN mkdir /home/$scidb_usr/mydevel
RUN chown $scidb_usr:$scidb_usr $dev_dir

## Pre-Installation Tasks
## ===
## https://paradigm4.atlassian.net/wiki/display/ESD/Pre-Installation+Tasks

## Download SciDB Community Edition
## ---
WORKDIR $dev_dir
ARG scidb_url="https://docs.google.com/uc?id=0B7yt0n33Us0raWtCYmNlZWRxWG8&export=download"
RUN wget --no-verbose --output-document scidb-15.12.1.4cadab5.tar.gz \
        --load-cookies cookies.txt \
        "$scidb_url&`wget --no-verbose --output-document - \
            --save-cookies cookies.txt "$scidb_url" | \
            grep --only-matching 'confirm=[^&]*'`"
RUN tar -xzf scidb-15.12.1.4cadab5.tar.gz
RUN mv scidb-15.12.1.4cadab5 scidbtrunk
WORKDIR $dev_dir/scidbtrunk

## Installing Expect, and SSH Packages
## --
RUN apt-get install -y expect openssh-server openssh-client git wget

## Providing Passwordless SSH
## ---
RUN ssh-keygen -f /root/.ssh/id_rsa -N ''
RUN chmod 755 /root
RUN chmod 755 /root/.ssh

RUN mkdir /home/$scidb_usr/.ssh
RUN ssh-keygen -f /home/$scidb_usr/.ssh/id_rsa -N ''
RUN chmod 755 /home/$scidb_usr
RUN chmod 755 /home/$scidb_usr/.ssh

## Avoid setting password and providing it to "deploy.sh access"
RUN cat /root/.ssh/id_rsa.pub >> /root/.ssh/authorized_keys
RUN cat /root/.ssh/id_rsa.pub >> /home/$scidb_usr/.ssh/authorized_keys

## Set correct ownership
RUN chown -R $scidb_usr:$scidb_usr /home/$scidb_usr

RUN service ssh start && \
    ./deployment/deploy.sh access root NA "" $host_ip && \
    ./deployment/deploy.sh access $scidb_usr NA "" $host_ip && \
    ssh $host_ip date

## Installing Build Tools
## ---
RUN service ssh start && \
    ./deployment/deploy.sh prepare_toolchain $host_ip

## Installing Postgres
## ---
RUN service ssh start && \
    ./deployment/deploy.sh prepare_postgresql postgres postgres $net_mask $host_ip

## Providing the postgres user Access to SciDB Code
RUN usermod -G $scidb_usr -a postgres
RUN chmod g+rx $dev_dir
RUN /usr/bin/sudo -u postgres ls $dev_dir


## Installing SciDB Community Edition
## ===
## https://paradigm4.atlassian.net/wiki/display/ESD/Installing+SciDB+Community+Edition

## Configuring Environment Variables
## ---
ENV SCIDB_VER=15.12
ENV SCIDB_INSTALL_PATH=$dev_dir/scidbtrunk/stage/install
ENV SCIDB_BUILD_TYPE=Debug
ENV PATH=$SCIDB_INSTALL_PATH/bin:$PATH

RUN echo "\
export SCIDB_VER=$SCIDB_VER\n\
export SCIDB_INSTALL_PATH=$SCIDB_INSTALL_PATH\n\
export SCIDB_BUILD_TYPE=$SCIDB_BUILD_TYPE\n\
export PATH=$PATH\n" | tee /root/.bashrc > /home/$scidb_usr/.bashrc


### Activating and Verifying the New .bashrc File
RUN echo $SCIDB_VER
RUN echo $SCIDB_INSTALL_PATH
RUN echo $PATH

## Building SciDB CE
## ---
RUN ./run.py setup --force
RUN ./run.py make -j4

## Installing SciDB CE
## ---
RUN service ssh start && \
    service postgresql start && \
    echo "\n\ny" | ./run.py install --force


## Starting and Stopping SciDB
## ===
## https://paradigm4.atlassian.net/wiki/display/ESD/Starting+and+Stopping+SciDB

RUN echo "#!/bin/bash\n\
service ssh start\n\
service postgresql start\n\
scidb.py startall mydb\n\
trap \"scidb.py stopall mydb; service postgresql stop\" EXIT HUP INT QUIT TERM\n\
bash" > /docker-entrypoint.sh
RUN chmod +x /docker-entrypoint.sh

# Adding special sudo power to scidb user due EOWS 3rdparty libraries used apt-get install
RUN echo "scidb ALL=(ALL) NOPASSWD: /usr/bin/apt-get" >> /etc/sudoers
RUN echo "scidb ALL=(ALL) NOPASSWD: /usr/bin/make" >> /etc/sudoers

# Remove CMake 2.8X
RUN cmake --version
RUN apt-get remove cmake* -y
RUN apt-get install curl

# Install CMake 3.7.2
ARG CMAKE_SRC_DIR=$dev_dir/cmake
WORKDIR $CMAKE_SRC_DIR

RUN curl -O https://cmake.org/files/v3.7/cmake-3.7.2.tar.gz
RUN tar -zxf cmake-3.7.2.tar.gz
WORKDIR $CMAKE_SRC_DIR/cmake-3.7.2
RUN ./configure
RUN make && make install

#
# Removing Broken header SciDB
#
RUN sed -i '/**MurmurHash3.h/c\//MurmurHash3 has been removed' $SCIDB_INSTALL_PATH/include/util/Hashing.h

# Creating EOWS dev dir
RUN mkdir -p $EOWS_DEPENDENCIES_DIR $EOWS_CODEBASE $EOWS_BUILD_DIR $EOWS_INSTALL_DIR

# Download EOWS
WORKDIR $EOWS_CODEBASE
RUN git clone https://github.com/eows/eows.git .

WORKDIR $EOWS_DEPENDENCIES_DIR  
RUN cp $EOWS_CODEBASE/bash/install-3rdparty-linux-ubuntu-14.04.sh .
RUN curl -O http://www.dpi.inpe.br/foss/eows/eows-3rdparty-0.3.0-linux-ubuntu-14.04.tar.gz
RUN chmod +x install-3rdparty-linux-ubuntu-14.04.sh
# Compile Dependendies
RUN ./install-3rdparty-linux-ubuntu-14.04.sh $EOWS_DEPENDENCIES_DIR

# Testing PreCompilation
WORKDIR $EOWS_BUILD_DIR
RUN cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="Release" -DCMAKE_PREFIX_PATH="$SCIDB_INSTALL_PATH;/opt/scidb/15.12/3rdparty/boost;$EOWS_DEPENDENCIES_DIR" $EOWS_CODEBASE/build/cmake
RUN make -j 2

WORKDIR /home/$scidb_usr
## Starting SciDB and compile EOWS dependencies
## ---
ENTRYPOINT ["/docker-entrypoint.sh"]
