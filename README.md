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