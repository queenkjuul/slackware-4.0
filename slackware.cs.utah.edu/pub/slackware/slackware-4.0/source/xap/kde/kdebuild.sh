#!/bin/sh
# Script to build and install KDE on Slackware
#
CWD=`pwd`
TMP=/tmp
cd $TMP
echo "Building kdesupport-1.1.1.tar.bz2..."
tar xyvf $CWD/kdesupport-1.1.1.tar.bz2
cd kdesupport-1.1.1
./configure --prefix=/opt/kde
make
make install
cd ..
echo
echo "Building kdelibs-1.1.1.tar.bz2..."
tar xyvf $CWD/kdelibs-1.1.1.tar.bz2
cd kdelibs-1.1.1
./configure --prefix=/opt/kde
make
make install
cd ..
echo
echo "Building kdebase-1.1.1.tar.bz2..."
tar xyvf $CWD/kdebase-1.1.1.tar.bz2
cd kdebase-1.1.1
./configure --prefix=/opt/kde --with-shadow
make
make install
cd ..
echo
echo "Building kdeadmin-1.1.1.tar.bz2..."
tar xyvf $CWD/kdeadmin-1.1.1.tar.bz2
cd kdeadmin-1.1.1
./configure --prefix=/opt/kde
make
make install
cd ..
echo
echo "Building kdegames-1.1.1.tar.bz2..."
tar xyvf $CWD/kdegames-1.1.1.tar.bz2
cd kdegames-1.1.1
./configure --prefix=/opt/kde
make
make install
cd ..
echo
echo "Building kdegraphics-1.1.1.tar.bz2..."
tar xyvf $CWD/kdegraphics-1.1.1.tar.bz2
cd kdegraphics-1.1.1
./configure --prefix=/opt/kde
make
make install
cd ..
echo
echo "Building kdemultimedia-1.1.1.tar.bz2..."
tar xyvf $CWD/kdemultimedia-1.1.1.tar.bz2
cd kdemultimedia-1.1.1
./configure --prefix=/opt/kde
make
make install
cd ..
echo
echo "Building kdenetwork-1.1.1.tar.bz2..."
tar xyvf $CWD/kdenetwork-1.1.1.tar.bz2
cd kdenetwork-1.1.1
./configure --prefix=/opt/kde
make
make install
cd ..
echo
echo "Building kdetoys-1.1.1.tar.bz2..."
tar xyvf $CWD/kdetoys-1.1.1.tar.bz2
cd kdetoys-1.1.1
./configure --prefix=/opt/kde
make
make install
cd ..
echo
echo "Building kdeutils-1.1.1.tar.bz2..."
tar xyvf $CWD/kdeutils-1.1.1.tar.bz2
cd kdeutils-1.1.1
./configure --prefix=/opt/kde
make
make install
cd ..
echo
echo "Building korganizer-1.1.1.tar.bz2..."
tar xyvf $CWD/korganizer-1.1.1.tar.bz2
cd korganizer-1.1.1
./configure --prefix=/opt/kde
make
make install
cd ..
