Flexiv RDKDemo
===============

This is a repository contains different applications and rdk programs as a example to show how to use RDK in real project

Setup build environment
-----------------------

1. Install the build tools using:

````
    sudo apt update
    sudo apt install build-essential pkg-config git wget autoconf libtool
````

2. Install some necessary dependencies:

````
    sudo apt install -y libjsoncpp-dev libspdlog-dev 

3. Use flexiv_thirdparty2/scripts build_boost.sh install boost library, you can also download from
   https://www.boost.org/, what we use boost lib version is 1.72.0

```

Configure and build the apps
-------------------------------

1. Go to the main folder and clone submodules with:

````
    git submodule update --init
````

2. Configure the top-level CMake using ``cmake-gui``:

````
    mkdir build && cd build
    cmake-gui .. &
````

3. Click Configure, then choose the default Unix Makefile configuration. An error will pop out, which is expected.

4. Choose the apps to build by checking options starting with ``BUILD_APP_``.

5. Click Configure then click Generate.

6. If no error occurs, close ``cmake-gui`` window, then go back to the terminal and:

````
    make -j4
````

7. All checked apps will be compiled and the executable binaries are under ``build/app/`` directory.
