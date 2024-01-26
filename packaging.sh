#!/bin/sh

INSTALL_DIR=/usr/local
CONFIG_DIR=
LIB_DIR=$INSTALL_DIR/lib
INC_DIR=$INSTALL_DIR/include/rindow

PKG_WORK=pkgwork
RINDOW_MATLIB_VERSION=`fgrep "#define RINDOW_MATLIB_VERSION" include/rindow/matlib.h | cut -d " " -f 3 | cut -d "\"" -f 2`
RINDOW_MATLIB_MAJOR_VERSION=`echo $RINDOW_MATLIB_VERSION | cut -d "." -f 1`
. /etc/os-release
OS_VERSION=$ID$VERSION_ID
echo RINDOW_MATLIB_VERSION=$RINDOW_MATLIB_VERSION
echo OS_VERSION=$OS_VERSION
rm -rf $PKG_WORK
mkdir -p $PKG_WORK$LIB_DIR
mkdir -p $PKG_WORK$LIB_DIR/pkgconfig
mkdir -p $PKG_WORK$INC_DIR
mkdir -p $PKG_WORK/DEBIAN
for name in librindowmatlib librindowmatlibseq
do
    cp build$CONFIG_DIR/$name.so $PKG_WORK$LIB_DIR/$name.$RINDOW_MATLIB_VERSION.so
    chmod 744 $PKG_WORK$LIB_DIR/$name.$RINDOW_MATLIB_VERSION.so
    ( cd $PKG_WORK$LIB_DIR ; ln -s $name.$RINDOW_MATLIB_VERSION.so $name.so )
    ( cd $PKG_WORK$LIB_DIR ; ln -s $name.$RINDOW_MATLIB_VERSION.so $name.$RINDOW_MATLIB_MAJOR_VERSION.so )
done
cp include/rindow/matlib.h $PKG_WORK$INC_DIR/.
chmod 444 $PKG_WORK$INC_DIR/matlib.h
sed -e s/%RINDOW_MATLIB_VERSION%/$RINDOW_MATLIB_VERSION/ debian/control | \
sed -e s/%OS_VERSION%/$OS_VERSION/ > $PKG_WORK/DEBIAN/control
sed -e s/%RINDOW_MATLIB_VERSION%/$RINDOW_MATLIB_VERSION/ pkgconfig/rindowmatlib.pc | \
sed -e s@%INSTALL_DIR%@$INSTALL_DIR@ > $PKG_WORK$LIB_DIR/pkgconfig/rindowmatlib.pc
#sed -e s@%EXTENSION_DIR%@$EXTENSION_DIR@ debian/rules | \
#sed -e s@%INI_DIR%@$INI_DIR@ debian/rules | \
#	> $PKG_WORK/DEBIAN/rules
#cp debian/changelog $PKG_WORK/DEBIAN/.
#cp debian/copyright $PKG_WORK/DEBIAN/.

rm -f rindow-opencl*.deb
fakeroot dpkg-deb --build pkgwork .
