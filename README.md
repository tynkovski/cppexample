# Installation

First install dependencies:

install curl library

``` bash
sudo dnf install libcurl-devel
```

and install and cpr wrapper

``` bash
sudo dnf install cpr-devel
```

install boost
``` bash
sudo yum install boost-devel
```

then build the shared lib:
``` bash
cd path/to/project/mylib
cmake -S . -B build
cd build
sudo make install
```

build program and run

``` bash
cd path/to/project/myprogram
cmake -S . -B build
cd build
make
./main
```

