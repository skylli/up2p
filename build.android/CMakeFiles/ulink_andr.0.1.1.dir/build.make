# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/derry/prj/up2p

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/derry/prj/up2p/build.android

# Include any dependencies generated for this target.
include CMakeFiles/ulink_andr.0.1.1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ulink_andr.0.1.1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ulink_andr.0.1.1.dir/flags.make

CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.o: CMakeFiles/ulink_andr.0.1.1.dir/flags.make
CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.o: ../src/up2pa/ulink.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/derry/prj/up2p/build.android/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.o"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.o   -c /home/derry/prj/up2p/src/up2pa/ulink.c

CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.i"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/derry/prj/up2p/src/up2pa/ulink.c > CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.i

CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.s"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/derry/prj/up2p/src/up2pa/ulink.c -o CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.s

CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.o.requires:

.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.o.requires

CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.o.provides: CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.o.requires
	$(MAKE) -f CMakeFiles/ulink_andr.0.1.1.dir/build.make CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.o.provides.build
.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.o.provides

CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.o.provides.build: CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.o


CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.o: CMakeFiles/ulink_andr.0.1.1.dir/flags.make
CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.o: ../src/up2pa/ctrl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/derry/prj/up2p/build.android/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.o"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.o   -c /home/derry/prj/up2p/src/up2pa/ctrl.c

CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.i"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/derry/prj/up2p/src/up2pa/ctrl.c > CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.i

CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.s"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/derry/prj/up2p/src/up2pa/ctrl.c -o CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.s

CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.o.requires:

.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.o.requires

CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.o.provides: CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.o.requires
	$(MAKE) -f CMakeFiles/ulink_andr.0.1.1.dir/build.make CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.o.provides.build
.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.o.provides

CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.o.provides.build: CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.o


CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.o: CMakeFiles/ulink_andr.0.1.1.dir/flags.make
CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.o: ../src/up2pa/up2pa.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/derry/prj/up2p/build.android/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.o"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.o   -c /home/derry/prj/up2p/src/up2pa/up2pa.c

CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.i"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/derry/prj/up2p/src/up2pa/up2pa.c > CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.i

CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.s"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/derry/prj/up2p/src/up2pa/up2pa.c -o CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.s

CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.o.requires:

.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.o.requires

CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.o.provides: CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.o.requires
	$(MAKE) -f CMakeFiles/ulink_andr.0.1.1.dir/build.make CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.o.provides.build
.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.o.provides

CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.o.provides.build: CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.o


CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.o: CMakeFiles/ulink_andr.0.1.1.dir/flags.make
CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.o: ../src/util/util.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/derry/prj/up2p/build.android/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.o"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.o   -c /home/derry/prj/up2p/src/util/util.c

CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.i"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/derry/prj/up2p/src/util/util.c > CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.i

CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.s"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/derry/prj/up2p/src/util/util.c -o CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.s

CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.o.requires:

.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.o.requires

CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.o.provides: CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.o.requires
	$(MAKE) -f CMakeFiles/ulink_andr.0.1.1.dir/build.make CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.o.provides.build
.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.o.provides

CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.o.provides.build: CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.o


CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.o: CMakeFiles/ulink_andr.0.1.1.dir/flags.make
CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.o: ../src/util/debug.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/derry/prj/up2p/build.android/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.o"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.o   -c /home/derry/prj/up2p/src/util/debug.c

CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.i"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/derry/prj/up2p/src/util/debug.c > CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.i

CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.s"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/derry/prj/up2p/src/util/debug.c -o CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.s

CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.o.requires:

.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.o.requires

CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.o.provides: CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.o.requires
	$(MAKE) -f CMakeFiles/ulink_andr.0.1.1.dir/build.make CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.o.provides.build
.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.o.provides

CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.o.provides.build: CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.o


CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.o: CMakeFiles/ulink_andr.0.1.1.dir/flags.make
CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.o: ../src/crypt/aes.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/derry/prj/up2p/build.android/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.o"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.o   -c /home/derry/prj/up2p/src/crypt/aes.c

CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.i"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/derry/prj/up2p/src/crypt/aes.c > CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.i

CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.s"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/derry/prj/up2p/src/crypt/aes.c -o CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.s

CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.o.requires:

.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.o.requires

CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.o.provides: CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.o.requires
	$(MAKE) -f CMakeFiles/ulink_andr.0.1.1.dir/build.make CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.o.provides.build
.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.o.provides

CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.o.provides.build: CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.o


CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.o: CMakeFiles/ulink_andr.0.1.1.dir/flags.make
CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.o: ../src/crypt/up2p_crypt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/derry/prj/up2p/build.android/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.o"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.o   -c /home/derry/prj/up2p/src/crypt/up2p_crypt.c

CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.i"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/derry/prj/up2p/src/crypt/up2p_crypt.c > CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.i

CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.s"
	/home/derry/data/install/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/derry/prj/up2p/src/crypt/up2p_crypt.c -o CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.s

CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.o.requires:

.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.o.requires

CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.o.provides: CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.o.requires
	$(MAKE) -f CMakeFiles/ulink_andr.0.1.1.dir/build.make CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.o.provides.build
.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.o.provides

CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.o.provides.build: CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.o


# Object files for target ulink_andr.0.1.1
ulink_andr_0_1_1_OBJECTS = \
"CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.o" \
"CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.o" \
"CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.o" \
"CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.o" \
"CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.o" \
"CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.o" \
"CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.o"

# External object files for target ulink_andr.0.1.1
ulink_andr_0_1_1_EXTERNAL_OBJECTS =

../lib/libulink_andr.0.1.1.so: CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.o
../lib/libulink_andr.0.1.1.so: CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.o
../lib/libulink_andr.0.1.1.so: CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.o
../lib/libulink_andr.0.1.1.so: CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.o
../lib/libulink_andr.0.1.1.so: CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.o
../lib/libulink_andr.0.1.1.so: CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.o
../lib/libulink_andr.0.1.1.so: CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.o
../lib/libulink_andr.0.1.1.so: CMakeFiles/ulink_andr.0.1.1.dir/build.make
../lib/libulink_andr.0.1.1.so: CMakeFiles/ulink_andr.0.1.1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/derry/prj/up2p/build.android/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking C shared library ../lib/libulink_andr.0.1.1.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ulink_andr.0.1.1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ulink_andr.0.1.1.dir/build: ../lib/libulink_andr.0.1.1.so

.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/build

CMakeFiles/ulink_andr.0.1.1.dir/requires: CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ulink.c.o.requires
CMakeFiles/ulink_andr.0.1.1.dir/requires: CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/ctrl.c.o.requires
CMakeFiles/ulink_andr.0.1.1.dir/requires: CMakeFiles/ulink_andr.0.1.1.dir/src/up2pa/up2pa.c.o.requires
CMakeFiles/ulink_andr.0.1.1.dir/requires: CMakeFiles/ulink_andr.0.1.1.dir/src/util/util.c.o.requires
CMakeFiles/ulink_andr.0.1.1.dir/requires: CMakeFiles/ulink_andr.0.1.1.dir/src/util/debug.c.o.requires
CMakeFiles/ulink_andr.0.1.1.dir/requires: CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/aes.c.o.requires
CMakeFiles/ulink_andr.0.1.1.dir/requires: CMakeFiles/ulink_andr.0.1.1.dir/src/crypt/up2p_crypt.c.o.requires

.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/requires

CMakeFiles/ulink_andr.0.1.1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ulink_andr.0.1.1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/clean

CMakeFiles/ulink_andr.0.1.1.dir/depend:
	cd /home/derry/prj/up2p/build.android && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/derry/prj/up2p /home/derry/prj/up2p /home/derry/prj/up2p/build.android /home/derry/prj/up2p/build.android /home/derry/prj/up2p/build.android/CMakeFiles/ulink_andr.0.1.1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ulink_andr.0.1.1.dir/depend

