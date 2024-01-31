The Rindow Matrix library for C
===============================
Rindow Math Matrix is the fundamental package for scientific matrix operation

- A powerful N-dimensional array object
- Sophisticated (broadcasting) functions
- A high-speed calculation library written in C.
- Useful linear algebra and random number capabilities
- Parallel calculation with OpenMP

You can call a high-speed calculation library written in C language to speed up matrix calculation processing.
Rindow Matlib includes many matrix operations functions used in machine learning.

Please see the documents about rindow mathematics on [Rindow Mathematics](https://rindow.github.io/mathematics/openblas/mathlibrary.html) web pages.

How to setup pre-build binaries
===============================

### How to setup for Windows
Download the pre-build binary file.

- https://github.com/rindow/rindow-matlib/releases

Unzip the file for Windows and copy rindowmatlib.dll to the directory set in PATH.

```shell
C> PATH %PATH%;C:\path\to\bin
```

### How to setup for Ubuntu
Download the pre-build binary file.

- https://github.com/rindow/rindow-matlib/releases

Please install using the apt command. 

If you use this library under the PHP, you must set it to serial mode.

```shell
$ sudo apt install ./rindow-matlib_X.X.X-X+ubuntuXX.XX_amd64.deb
$ sudo update-alternatives --config librindowmatlib.so
There are 2 choices for the alternative librindowmatlib.so (providing /usr/lib/librindowmatlib.so).

  Selection    Path                                             Priority   Status
------------------------------------------------------------
* 0            /usr/lib/rindowmatlib-openmp/librindowmatlib.so   95        auto mode
  1            /usr/lib/rindowmatlib-openmp/librindowmatlib.so   95        manual mode
  2            /usr/lib/rindowmatlib-serial/librindowmatlib.so   90        manual mode

Press <enter> to keep the current choice[*], or type selection number: 2
```

How to build from source code on Windows
========================================
You can also build and use from source code.

### download source code

Download source code from release and unzip

- https://github.com/rindow/rindow-matlib/releases

### Build and Install

Build with Visual Studio.

```shell
C> cd \path\to\here
C> mkdir build
C> cd build
C> cmake ..
C> cmake --config Release
C> ctest
```


```shell
$ nmake clean
$ nmake
$ nmake test
```
