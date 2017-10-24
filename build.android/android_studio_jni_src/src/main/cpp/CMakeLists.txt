
#
# Copyright (C) The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.4.1)

# configure import libs
set(distribution_DIR ${CMAKE_SOURCE_DIR}/../../../../distribution)

## demo 
# 使用外部预编译好的库
add_library(lib_ulink_andr SHARED IMPORTED)
# 指定库的路径
set_target_properties(lib_ulink_andr PROPERTIES IMPORTED_LOCATION
    ${distribution_DIR}/ulink/lib/${ANDROID_ABI}/libulink_andr.so)

#debug release setting 
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# jni 库
add_library(ulink_jni SHARED
            ulink_jni.c)
# jni 库依赖头文件
target_include_directories(ulink_jni PRIVATE
                           ${distribution_DIR}/ulink/include)               
# jni 库的依赖
target_link_libraries(ulink_jni
                      android
                      lib_ulink_andr
                      log)
