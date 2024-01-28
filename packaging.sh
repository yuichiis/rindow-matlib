#!/bin/sh

INSTALL_DIR=/usr
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

## runtime
name=rindowmatlib
libname=lib$name
cp build$CONFIG_DIR/${libname}.so ./${libname}-openmp.so
cp build$CONFIG_DIR/${libname}seq.so ./${libname}-serial.so
for type in openmp serial
do
    mkdir -p $PKG_WORK$LIB_DIR/$name-$type
    cp ./$libname-$type.so $PKG_WORK$LIB_DIR/$name-$type/$libname.$RINDOW_MATLIB_VERSION.so
    chmod 744 $PKG_WORK$LIB_DIR/$name-$type/$libname.$RINDOW_MATLIB_VERSION.so
    ( cd $PKG_WORK$LIB_DIR/$name-$type ; ln -s $libname.$RINDOW_MATLIB_VERSION.so $libname.so )
    ( cd $PKG_WORK$LIB_DIR/$name-$type ; ln -s $libname.$RINDOW_MATLIB_VERSION.so $libname.$RINDOW_MATLIB_MAJOR_VERSION.so )
done
rm ./${libname}-openmp.so
rm ./${libname}-serial.so

## dev
cp include/rindow/matlib.h $PKG_WORK$INC_DIR/.
chmod 444 $PKG_WORK$INC_DIR/matlib.h
sed -e s/%RINDOW_MATLIB_VERSION%/$RINDOW_MATLIB_VERSION/ debian/control | \
sed -e s/%OS_VERSION%/$OS_VERSION/ > $PKG_WORK/DEBIAN/control
sed -e s/%RINDOW_MATLIB_VERSION%/$RINDOW_MATLIB_VERSION/ pkgconfig/rindowmatlib.pc | \
sed -e s@%INSTALL_DIR%@$INSTALL_DIR@ > $PKG_WORK$LIB_DIR/pkgconfig/rindowmatlib.pc
for script in postinst prerm
do
    cp debian/$script $PKG_WORK/DEBIAN/.
    chmod 555 $PKG_WORK/DEBIAN/$script
done
#sed -e s@%EXTENSION_DIR%@$EXTENSION_DIR@ debian/rules | \
#sed -e s@%INI_DIR%@$INI_DIR@ debian/rules | \
#	> $PKG_WORK/DEBIAN/rules
#cp debian/changelog $PKG_WORK/DEBIAN/.
#cp debian/copyright $PKG_WORK/DEBIAN/.

rm -f rindow-matlib_*.deb
fakeroot dpkg-deb --build pkgwork .
