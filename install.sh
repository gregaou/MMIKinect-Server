#!/bin/bash

if [ "$(id -u)" != "0" ]; then
	echo "Sorry, you are not root."
	exit 1
fi

PACKAGE="autotools-dev make cmake qt4-qmake unzip git pkg-config gcc g++"

SPRO_URL="http://www.irisa.fr/metiss/guig/spro/spro-4.0.1/spro-4.0.1.tar.gz"
SPRO_ARCHIVE="spro.tar.gz"

ALIZE_URL="http://mistral.univ-avignon.fr/src/ALIZE_2.0.zip"
ALIZE_ARCHIVE="alize.zip"

LIA_RAL_URL="http://mistral.univ-avignon.fr/src/LIA_RAL_2.0.zip"
LIA_RAL_ARCHIVE="LIA_RAL.zip"

OPENCV_URL="http://downloads.sourceforge.net/project/opencvlibrary/opencv-unix/2.4.5/opencv-2.4.5.tar.gz?r=http%3A%2F%2Fopencv.org%2Fdownloads.html&ts=1370339637&use_mirror=netcologne"
OPENCV_ARCHIVE="opencv.tar.gz"

MMIKINECT_SERVER_GIT="https://github.com/gregaou/MMIKinect-Server"
MMIKINECT_SERVER_PRO_FILE="MMIKinect-Server.pro"
MMIKINECT_SERVER_DIRECTORY="."

DOWNLOAD_DIRECTORY="Downloads"


# First step : installing packages
apt-get install -y $PACKAGE

# Second step downloading and unzipping and installing libraries
mkdir $DOWNLOAD_DIRECTORY
cd $DOWNLOAD_DIRECTORY

# SPRO library
wget $SPRO_URL -O $SPRO_ARCHIVE
tar xvzf $SPRO_ARCHIVE
cd spro*
aclocal
autoconf
automake
./configure
make sfbcep
cp sfbcep /usr/local/bin
cd ..

# ALIZE library
wget $ALIZE_URL -O $ALIZE_ARCHIVE
unzip $ALIZE_ARCHIVE
cd ALIZE
sed '10d' configure.in > tmp && mv tmp configure.in
sed '5d' configure.in > tmp && mv tmp configure.in
aclocal
autoconf
automake
./configure
make
make install
cd ..

# LIA_RAL library
wget $LIA_RAL_URL -O $LIA_RAL_ARCHIVE
unzip $LIA_RAL_ARCHIVE
cd LIA_RAL
aclocal
autoconf
automake
./configure
make
make install
cd ..

# OpenCV library
wget $OPENCV_URL -O $OPENCV_ARCHIVE
tar xvzf $OPENCV_ARCHIVE
cd opencv*
cmake .
make
make install
cd ..

cd ..

# Third step : Cloning git  and compiling it
cd $MMIKINECT_SERVER_DIRECTORY
git clone $MMIKINECT_SERVER_GIT
cd MMIKinect-Server/
qmake $MMIKINECT_SERVER_PRO_FILE
make
mkdir module/AudioModule/prm
mkdir module/AudioModule/ndx

exit 0
