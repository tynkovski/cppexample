# Guide

Requirements
============

First install dependencies:

Install cmake 

```
sudo dnf install cmake
```

Install curl library and cpr wrapper

``` bash
sudo dnf install libcurl-devel
sudo dnf install cpr-devel
```

Install boost

``` bash
sudo dnf install boost-devel
```

Building
========

Build the shared lib:
``` bash
cd path/to/project/mylib
cmake -S . -B build
cd build
sudo make install
```

build program

``` bash
cd path/to/project/myprogram
cmake -S . -B build
cd build
make
```

Usage
=====

run program
``` bash
cd path/to/project/myprogram/build
./main
```
