Adventures-in-SDL2
==================

Not much to see here yet.  Mostly just experimenting with framework ideas.


Prerequisites
-------------

Linux: At least GCC 4.6.  Clang 3.2 or later may work but is currently not well-tested.

Windows: Visual Studio 2010 or later, including the free Visual Studio Express editions.

**SDL 2**

SDL 2.0.0 or later is required, along with the standard extension libraries: SDL_image, SDL_ttf, SDL_mixer, and SDL_net.

*Ubuntu 13.05 "Raring" and Linux Mint 15 and earlier:* Snapshot PPA: https://launchpad.net/~zoogie/+archive/sdl2-snapshots

*Ubuntu 13.10 "Saucy" and Debian unstable:* Packages are available from the distro, but untested.

*Windows (Visual Studio / Visual Studio Express):* Prebuilt libraries and headers are included in the repo, so you don't need to do anything  :)

**CMake**

CMake 2.8 or higher is required.  Most Linux distros should have a package for this already.  For Windows, download it from here: http://www.cmake.org/


Building
--------

Note: Once CMake has generated the build files, it doesn't need to be run again -- the build files will detect changes to the `CMakeLists.txt` file and re-run CMake for you.

**Linux**

    mkdir build
    cd build
    cmake ..
    make

**Windows**

Using CMake-GUI:

1. Set "Where is the source code" to the root of the checkout.
2. Set "Where to build the binaries" to be the "build" subdirectory (don't worry if it doesn't exist).
3. Press "Generate".  Select the Visual Studio version you're using from the list ("Visual Studio 10" is for VS2010, "Visual Studio 11" is for VS2012, and "Visual Studio 12" is for VS2013).  I recommend using the 32-bit targets (i.e., not the "Win64" or "IA64" targets).  Leave the compiler selector at "Use default native compilers".
4. Open the `build/adventures-in-sdl2.sln` solution in Visual Studio.
