
Requirements
------------

To build the unittests the following programs in their respective versions
are needed:

- `gcc` >= 4.9 (full support for C++11)
- `clang` >= 3.2
- `scons` >= 2.3
- `python` >= 2.7
- `lcov` >= 1.10

For Debian-based Linux systems (e.g. Debian 8):

    sudo apt-get install clang llvm scons libboost-dev libsqlite3-dev curl lcov
    
    curl -R -O http://www.lua.org/ftp/lua-5.3.0.tar.gz
    tar zxf lua-5.3.0.tar.gz
    cd lua-5.3.0
    make linux test
    sudo make install
    
    wget http://luarocks.org/releases/luarocks-2.2.1.tar.gz
    tar zxpf luarocks-2.2.1.tar.gz
    cd luarocks-2.2.1
    ./configure
    sudo make bootstrap
    
    sudo luarocks install busted
