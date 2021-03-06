# CMake 最低版本号要求.
cmake_minimum_required (VERSION 2.8)
# 项目名称
project(up2p)

# 主版本号
set(PRJ_VERSION_MAJOR 0)
# 次版本号
set(PRJ_VERSION_MINOR 1)
# 补丁版本
set(PRJ_VERSION_PATCH 1)

set(up2pc "up2pc.${PRJ_VERSION_MAJOR}.${PRJ_VERSION_MINOR}.${PRJ_VERSION_PATCH}")
set(up2ps "up2ps.${PRJ_VERSION_MAJOR}.${PRJ_VERSION_MINOR}.${PRJ_VERSION_PATCH}")

set(restapi "restapi.${PRJ_VERSION_MAJOR}.${PRJ_VERSION_MINOR}.${PRJ_VERSION_PATCH}")
set(protocol "protocol.${PRJ_VERSION_MAJOR}.${PRJ_VERSION_MINOR}.${PRJ_VERSION_PATCH}")

set(ulink "ulink.${PRJ_VERSION_MAJOR}.${PRJ_VERSION_MINOR}.${PRJ_VERSION_PATCH}")
set(ulink_a "ulink_a.${PRJ_VERSION_MAJOR}.${PRJ_VERSION_MINOR}.${PRJ_VERSION_PATCH}")

set(ulink_ios "ulink_ios.${PRJ_VERSION_MAJOR}.${PRJ_VERSION_MINOR}.${PRJ_VERSION_PATCH}")
set(ulink_a_ios "ulink_a_ios.${PRJ_VERSION_MAJOR}.${PRJ_VERSION_MINOR}.${PRJ_VERSION_PATCH}")

#set(ulink_andr "ulink_andr.${PRJ_VERSION_MAJOR}.${PRJ_VERSION_MINOR}.${PRJ_VERSION_PATCH}")
set(ulink_a_andr"ulink_a_andr.${PRJ_VERSION_MAJOR}.${PRJ_VERSION_MINOR}.${PRJ_VERSION_PATCH}")

message("CMakeLists version ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")
message("currentt sdk version = ${PROJECT_NAME}.${PRJ_VERSION_MAJOR}.${PRJ_VERSION_MINOR}.${PRJ_VERSION_PATCH}")
#目标文件放置到 bin 文件夹
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#设置库输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
SET(SRC_PATH ${PROJECT_SOURCE_DIR}/src)


# 搜索当前目录下所有的头文件
file(GLOB_RECURSE Foo_HEADERS "*.h")
set (Foo_INCLUDE_DIRS "")
foreach (_headerFile ${Foo_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND Foo_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES Foo_INCLUDE_DIRS)

# 包含工程所有的头文件
include_directories(${Foo_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}/include")

FILE(GLOB LIB_CRYPT_SOURCES ${SRC_PATH}/crypt/*.c)
FILE(GLOB LIB_UP2PC_SOURCES ${SRC_PATH}/up2pc/*.c)
FILE(GLOB LIB_UP2PS_SOURCES ${SRC_PATH}/up2ps/*.c)
FILE(GLOB LIB_RESTAPI_SOURCES ${SRC_PATH}/rest_api/*.c)
FILE(GLOB LIB_UTIL_SOURCES ${SRC_PATH}/util/*.c)
FILE(GLOB LIB_UP2PA_SOURCES ${SRC_PATH}/up2pa/*.c)
FILE(GLOB LIB_PROTOCOL_SOURCES ${SRC_PATH}/protocol/*.c)

#debug
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

IF(ANDROID)
ADD_LIBRARY(ulink_andr SHARED ${LIB_UP2PA_SOURCES} ${LIB_UTIL_SOURCES} ${LIB_CRYPT_SOURCES} )
# ADD_LIBRARY(${ulink_a_andr} STATIC ${LIB_UP2PA_SOURCES} ${LIB_UTIL_SOURCES} ${LIB_CRYPT_SOURCES} )
# 库打包路径
set(distribution_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../distribution)
# jni 库的依赖
target_link_libraries(ulink_andr
                      android
                      log)

set_target_properties(ulink_andr 
                      PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY
                      "${distribution_DIR}/ulink/lib/${ANDROID_ABI}")
# todo must add by head
 add_custom_command(TARGET ulink_andr POST_BUILD
                   COMMAND "${CMAKE_COMMAND}" -E
                   copy "${CMAKE_CURRENT_SOURCE_DIR}/include/ulink.h"
                   "${distribution_DIR}/ulink/include/ulink.h"
                   COMMENT "Copying ulink to output directory")
 add_custom_command(TARGET ulink_andr POST_BUILD
                   COMMAND "${CMAKE_COMMAND}" -E
                   copy "${CMAKE_CURRENT_SOURCE_DIR}/include/up2p.h"
                   "${distribution_DIR}/ulink/include/up2p.h"
                   COMMENT "Copying up2p to output directory")
 add_custom_command(TARGET ulink_andr POST_BUILD
                   COMMAND "${CMAKE_COMMAND}" -E
                   copy "${CMAKE_CURRENT_SOURCE_DIR}/include/util.h"
                   "${distribution_DIR}/ulink/include/util.h"
                   COMMENT "Copying util to output directory")
 add_custom_command(TARGET ulink_andr POST_BUILD
                   COMMAND "${CMAKE_COMMAND}" -E
                   copy "${CMAKE_CURRENT_SOURCE_DIR}/src/up2pa/up2pa.h"
                   "${distribution_DIR}/ulink/include/up2pa.h"
                   COMMENT "Copying up2pa.h to output directory")
add_custom_command(TARGET ulink_andr POST_BUILD
                   COMMAND "${CMAKE_COMMAND}" -E
                   copy "${CMAKE_CURRENT_SOURCE_DIR}/include/config.h"
                   "${distribution_DIR}/ulink/include/config.h"
                   COMMENT "Copying util to output directory")

ELSEIF(APPLE)
ADD_LIBRARY(${ulink_ios} SHARED ${LIB_UP2PA_SOURCES} ${LIB_UTIL_SOURCES} ${LIB_CRYPT_SOURCES} )
ADD_LIBRARY(${ulink_a_ios} STATIC ${LIB_UP2PA_SOURCES} ${LIB_UTIL_SOURCES} ${LIB_CRYPT_SOURCES} )

#制作 up2pc 库
ADD_LIBRARY(${up2pc} STATIC ${LIB_CRYPT_SOURCES} ${LIB_UP2PC_SOURCES} ${LIB_UTIL_SOURCES})
#制作up2pa 库
ADD_LIBRARY(${ulink} SHARED ${LIB_UP2PA_SOURCES} ${LIB_UTIL_SOURCES} ${LIB_CRYPT_SOURCES} )
ADD_LIBRARY(${ulink_a} STATIC ${LIB_UP2PA_SOURCES} ${LIB_UTIL_SOURCES} ${LIB_CRYPT_SOURCES} )

add_executable(vmup2pc vm_up2pc.c)
target_link_libraries(vmup2pc ${up2pc} pthread)
add_executable(vmup2pa vm_up2pa.c)
target_link_libraries(vmup2pa ${ulink} pthread)
ELSE()
#FILE(GLOB LIB_UTIL_SOURCES ${SRC_PATH}/util/*.c)
#制作 up2pc 库
ADD_LIBRARY(${up2pc} STATIC ${LIB_CRYPT_SOURCES} ${LIB_UP2PC_SOURCES} ${LIB_UTIL_SOURCES})
#制作up2ps 库
ADD_LIBRARY(${up2ps} STATIC ${LIB_UP2PS_SOURCES} ${LIB_UTIL_SOURCES})
#制作up2pa 库
ADD_LIBRARY(${ulink} SHARED ${LIB_UP2PA_SOURCES} ${LIB_UTIL_SOURCES} ${LIB_CRYPT_SOURCES} )
ADD_LIBRARY(${ulink_a} STATIC ${LIB_UP2PA_SOURCES} ${LIB_UTIL_SOURCES} ${LIB_CRYPT_SOURCES} )
# 制作 protocol 库
ADD_LIBRARY(${protocol} STATIC ${LIB_PROTOCOL_SOURCES} ${LIB_UTIL_SOURCES} ${LIB_CRYPT_SOURCES})

# 制作 restapi 的库
ADD_LIBRARY(${restapi} STATIC ${LIB_RESTAPI_SOURCES} ${LIB_UTIL_SOURCES} ${LIB_CRYPT_SOURCES})
#LINK_DIRECTORIES("${PROJECT_SOURCE_DIR}")

# 指定生成目标 
add_executable(vmup2pc vm_up2pc.c)
target_link_libraries(vmup2pc ${up2pc} pthread)
add_executable(vmup2ps vm_up2ps.c)
target_link_libraries(vmup2ps ${up2ps} ${ulink} pthread ulfius jansson orcania jwt m)
add_executable(vmup2pa vm_up2pa.c)
target_link_libraries(vmup2pa ${ulink} pthread)

add_executable(vmup2pr vm_up2pr.c)
target_link_libraries(vmup2pr ${restapi} ${protocol} pthread ulfius jansson orcania jwt m)

#ENDIF(APPLE)
ENDIF()
