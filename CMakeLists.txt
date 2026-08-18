#/****************************************************************************
# Copyright (c) 2013 cocos2d-x.org
# Copyright (c) 2014 martell malone
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

# build engine library and all tests

cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 20)

project(Cocos2d-x)

# Enable Objective-C/C++ globally on Apple platforms: o2 and cocos both compile
# .m/.mm sources in sibling directories, so the languages must be enabled at the
# top level (enabling them only inside o2/'s project() leaves CMAKE_OBJCXX_*
# variables unset in other directories with non-Xcode generators).
if(APPLE)
    enable_language(OBJC OBJCXX)
endif()

if(EMSCRIPTEN)
    # Emscripten ports replace the prebuilt third party libraries cocos ships for the desktops.
    # The flags are needed at compile time too: the ports provide their headers that way
    # The header providing ports are needed at compile time as well, GLFW only at link
    set(O2_COCOS_WASM_PORTS -sUSE_ZLIB=1 -sUSE_LIBPNG=1 -sUSE_LIBJPEG=1 -sUSE_FREETYPE=1)
    add_compile_options(${O2_COCOS_WASM_PORTS})
    add_link_options(${O2_COCOS_WASM_PORTS} -sUSE_GLFW=3)

    # cocos includes the glfw header unqualified, the port keeps it under GLFW/
    include_directories(SYSTEM "$ENV{EMSDK}/upstream/emscripten/cache/sysroot/include/GLFW")

    # The cocos third party libraries ship as prebuilt desktop binaries; for wasm the ports above
    # replace what is still needed and the rest of the features are off
    foreach(ext BOX2D CHIPMUNK FREETYPE2 BULLET JPEG OPENSSL UV WEBP WEBSOCKETS CURL ZLIB)
        set(BUILD_EXT_${ext} OFF CACHE BOOL "" FORCE)
    endforeach()
endif()

# set default minimum deployment target
if(XCODE)
    if(NOT DEFINED CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET)
        SET (CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 8.0)
    endif()
endif()

# cocos2dx root path
set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

# prevent in-source-build
include(PreventInSourceBuilds)

# works before build libcocos2d
include(CocosBuildSet)

# external libs (needed early so submodules like o2 can link against ext_* targets)
set(BUILD_LUA_LIBS ON)
add_subdirectory(${COCOS2DX_ROOT_PATH}/external ${ENGINE_BINARY_PATH}/external)

# add o2 with options
set(O2_PLATFORM_INITIALIZATION_ENABLED OFF CACHE BOOL "Enables platform initialization" FORCE)
set(O2_ENABLE_LINK_ZLIB OFF CACHE BOOL "Enables linking zlib library" FORCE)
set(O2_ENABLE_LINK_LIBPNG OFF CACHE BOOL "Enables linking libpng library" FORCE)
set(O2_TRACY OFF CACHE BOOL "Enables Tracy profiling" FORCE)
add_subdirectory(${COCOS2DX_ROOT_PATH}/o2)

add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# prevent tests project to build "cocos2d-x/cocos" again
set(BUILD_ENGINE_DONE ON)

# add cpp tests default
add_subdirectory(${COCOS2DX_ROOT_PATH}/tests/cpp-empty-test ${ENGINE_BINARY_PATH}/tests/cpp-empty-test)
add_subdirectory(${COCOS2DX_ROOT_PATH}/tests/cpp-tests ${ENGINE_BINARY_PATH}/tests/cpp-tests)

if(BUILD_LUA_LIBS)
  add_subdirectory(${COCOS2DX_ROOT_PATH}/tests/lua-empty-test/project ${ENGINE_BINARY_PATH}/tests/lua-empty-test)
  add_subdirectory(${COCOS2DX_ROOT_PATH}/tests/lua-tests/project ${ENGINE_BINARY_PATH}/tests/lua-test)
endif(BUILD_LUA_LIBS)

# add cpp-template-default into project(Cocos2d-x) for tmp test
add_subdirectory(${COCOS2DX_ROOT_PATH}/templates/cpp-template-default ${ENGINE_BINARY_PATH}/tests/HelloCpp)
