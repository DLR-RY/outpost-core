
# Installation

## Install dependencies for library development

### Install build tools

```
sudo apt-get install build-essential clang llvm libboost-dev libsqlite3-dev \
    make scons gcovr lcov cloc wget
```

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
