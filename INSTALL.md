
# Installation

This repository uses git submodules. To initialize them after cloning
the repository run the following command:

```
git submodule update --init --recursive
```

To update the submodules in case of changes run the same command without
the init parameter:

```
git submodule update --recursive
```

## Install dependencies for library development

The full tool-suite is only required if you want to run all unit tests and
actively develop the library.

To use the library the following tools are sufficient:

- `gcc` >= 4.9 (full support for C++11)
or
- `clang` >= 3.2
- `scons` >= 2.3

### Install build tools

```
sudo apt-get install build-essential clang llvm libboost-dev libsqlite3-dev \
    make scons gcovr lcov cloc wget
```

Requires `lcov` >= 1.10.

### Lua 5.2.3:

```
sudo apt-get install libreadline-dev

wget http://www.lua.org/ftp/lua-5.2.3.tar.gz 
tar zxf lua-5.2.3.tar.gz
cd lua-5.2.3
make linux
sudo make linux install
```

### Busted

Busted is used for running the Lua based unit tests:

```
wget http://luarocks.org/releases/luarocks-2.1.2.tar.gz 
tar zxf luarocks-2.1.2.tar.gz
cd luarocks-2.1.2
./configure
make build
sudo make install

sudo luarocks install busted
```

### clang-format

Install clang-format (if clang-format >= 6 is not already available
in the distribution repositories).

For Debian 9 (stretch) (for other see http://apt.llvm.org/)

```
sudo apt-get install software-properties-common
sudo apt-add-repository "deb http://apt.llvm.org/stretch/ llvm-toolchain-stretch-6.0 main"
sudo apt-get udpate
sudo apt-get install clang-format-6.0
```
