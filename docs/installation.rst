Build and Installation
======================

Build
-----

Dotrix uses the Qt5 framework and is configured with CMake. It can be built from the Qt Creator or from the command line.

For building from the command line, generate Makefiles for your build tool of choice::

    cd path/to/source
    mkdir build && cd build
    cmake .. -G "NMake Makefiles"
    nmake

For building against 64 bit Qt remember to configure your environment as such.

For example when building with MSVC 64 bit run the above commands in the Visual Studio x64 developer command prompt.

Installation
------------

Check out the latest `releases <https://github.com/nnarain/dotrix/releases>`_.
