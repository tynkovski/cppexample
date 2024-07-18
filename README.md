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

then build the project:
``` bash
cd path/to/project
cmake -S . -B build
cd build
make
```

you can run the app

``` bash
./main
```

