#/bin/bash
LIB_DIR=../lib
IOS_LIB_NAME=libulink_ios.0.1.1.a
IOS_SIMULATOR64_LIB_NAME=ioslib
IOS_LIB_DIR=$LIB_DIR/$IOS_SIMULATOR64_LIB_NAME

echo "Cmake path is $1"
rm -rf $IOS_LIB_DIR/*
rm -rf CM*  *.xcodeproj

# 编译虚拟机使用的库
echo "building lib for ios simulator lib"
$1  .. -DCMAKE_TOOLCHAIN_FILE=../ios.toolchain.cmake  -GXcode -DIOS_PLATFORM=SIMULATOR64
xcodebuild clean build  -configuration Debug
mkdir -p  $IOS_LIB_DIR
cp $LIB_DIR/Debug/libulink_a_ios*   $IOS_LIB_DIR

rm -rf CM*  *.xcodeproj
# 编译 ipthon 真机使用的库
$1  .. -DCMAKE_TOOLCHAIN_FILE=../ios.toolchain.cmake  -GXcode -DIOS_PLATFORM=OS
xcodebuild clean build  -configuration Debug

# 合并两个库
lipo -create  $IOS_LIB_DIR/libulink_a_ios*.a  $LIB_DIR/Debug/libulink_a_ios*.a    -output    $IOS_LIB_DIR/$IOS_LIB_NAME
rm $IOS_LIB_DIR/libulink_a*
echo "ios lib have been creat  $IOS_LIB_DIR/$IOS_LIB_NAME"
lipo -info $IOS_LIB_DIR/$IOS_LIB_NAME