# Keela
Optical Mapping Image Recording Program

Based off of the game engine Hazel by The Cherno

## Installation instructions

### Windows
- Install MSYS2 MinGW https://www.msys2.org/
- Install Cmake https://cmake.org/download/
- add mingw64 bin directory to PATH env variable (by default this will be C:\msys64\mingw64\bin)
- open a mingw64 terminal via the windows search bar
- install the following dependencies via pacman
  - mingw-w64-x86_64-pacman
  - mingw-w64-x86_64-toolchain
  - mingw-w64-x86_64-gtkmm3
  - mingw-w64-x86_64-gstreamer
  - mingw-w64-x86_64-gst-plugins-base
  - mingw-w64-x86_64-gst-plugins-good
  - mingw-w64-x86_64-gst-plugins-bad
  - mingw-w64-x86_64-gst-plugins-ugly
  - mingw-w64-x86_64-spdlog
- (Optional) if you like, in a fresh terminal, run the command `gst-inspect-1.0` to confirm gstreamer is installed
- run the following commands in the repository directory
  - mkdir build
  - cd build
  - cmake .. -G "MinGW Makefiles"
  - mingw32-make
- Depending on your IDE, your IDE may be able to perform the build step for you - I am using CLion

### Linux
- Install the equivalent dependencies in your package manager
  - cmake
  - gtkmm3
  - gstreamer
  - gst-plugins-good
  - gst-plugins-bad
  - gst-plugins-ugly
  - build-essential
  - spdlog

### Building
```
mkdir build && cd build
cmake ..
make keela -j$(nproc)
```

Build with custom `keela-videotestsrc` plugin enabled:
```
make gstkeelavideotestsrc3
cmake .. -DENABLE_KEELA_VIDEOTESTSRC=ON
make keela -j$(nproc)
```

To switch back to the default configuration (without the custom plugin):
```
cmake .. -DENABLE_KEELA_VIDEOTESTSRC=OFF
make keela -j$(nproc)
```

or:
```
make clean-config
cmake ..
make keela -j$(nproc)
```