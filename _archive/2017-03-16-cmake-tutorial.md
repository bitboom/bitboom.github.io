---
layout: post
title: Cmake tutorial
tags: [cmake]
---

## 01. make program
```cmake
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
 
ADD_EXECUTABLE(a.out main.cpp)
```

## 02. make shared library
```cmake
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
 
ADD_LIBRARY(mylib SHARED mylib.cpp)
```

## 03. make program with lib
```cmake
# program
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Sample)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib/include)
 
ADD_EXECUTABLE(a.out main.cpp)
TARGET_LINK_LIBRARIES(a.out mylib)

ADD_SUBDIRECTORY(lib)

## sub ###############################################
# library
INCLUDE_DIRECTORIES(./include)

# static library 
# ADD_LIBRARY(mylib mylib.cpp)

# dynamic library
ADD_LIBRARY(mylib SHARED mylib.cpp)
```

## 04 install targets
```cmake
# program
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Sample)

# https://cmake.org/Wiki/CMake_RPATH_handling
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
#SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib/include)
 
ADD_SUBDIRECTORY(lib)

ADD_EXECUTABLE(a.out main.cpp)

TARGET_LINK_LIBRARIES(a.out mylib)

INSTALL(TARGETS a.out DESTINATION bin)

## sub ###############################################
INCLUDE_DIRECTORIES(./include)

# static library 
# ADD_LIBRARY(mylib mylib.cpp)

# dynamic library
ADD_LIBRARY(mylib SHARED mylib.cpp)

INSTALL(FILES ./include/mylib.h DESTINATION include)
INSTALL(TARGETS mylib DESTINATION lib)
```
