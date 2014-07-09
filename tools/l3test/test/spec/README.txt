
The lua unittests use *Busted* as test framework.


Install
-------

Busted can be installed on a Linux system via LuaRocks::

    $ sudo apt-get install curl
    $ curl -R -O http://www.lua.org/ftp/lua-5.2.3.tar.gz
    $ tar zxf lua-5.2.3.tar.gz
    $ cd lua-5.2.3
    $ make linux
    $ sudo make linux install

    $ wget http://luarocks.org/releases/luarocks-2.1.2.tar.gz
    $ tar zxf luarocks-2.1.2.tar.gz
    $ cd luarocks-2.1.2
    $ ./configure
    $ make build
    $ sudo make install
    
    $ sudo luarocks install busted
