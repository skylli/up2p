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
CMAKE_SOURCE_DIR = /home/derry/github/up2p

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/derry/github/up2p/build

# Include any dependencies generated for this target.
include CMakeFiles/up2pc.0.1.1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/up2pc.0.1.1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/up2pc.0.1.1.dir/flags.make

CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.o: CMakeFiles/up2pc.0.1.1.dir/flags.make
CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.o: ../src/crypt/aes.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/derry/github/up2p/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.o   -c /home/derry/github/up2p/src/crypt/aes.c

CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/derry/github/up2p/src/crypt/aes.c > CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.i

CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/derry/github/up2p/src/crypt/aes.c -o CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.s

CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.o.requires:

.PHONY : CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.o.requires

CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.o.provides: CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.o.requires
	$(MAKE) -f CMakeFiles/up2pc.0.1.1.dir/build.make CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.o.provides.build
.PHONY : CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.o.provides

CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.o.provides.build: CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.o


CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.o: CMakeFiles/up2pc.0.1.1.dir/flags.make
CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.o: ../src/crypt/up2p_crypt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/derry/github/up2p/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.o   -c /home/derry/github/up2p/src/crypt/up2p_crypt.c

CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/derry/github/up2p/src/crypt/up2p_crypt.c > CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.i

CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/derry/github/up2p/src/crypt/up2p_crypt.c -o CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.s

CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.o.requires:

.PHONY : CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.o.requires

CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.o.provides: CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.o.requires
	$(MAKE) -f CMakeFiles/up2pc.0.1.1.dir/build.make CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.o.provides.build
.PHONY : CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.o.provides

CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.o.provides.build: CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.o


CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.o: CMakeFiles/up2pc.0.1.1.dir/flags.make
CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.o: ../src/up2pc/up2p_posix.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/derry/github/up2p/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.o   -c /home/derry/github/up2p/src/up2pc/up2p_posix.c

CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/derry/github/up2p/src/up2pc/up2p_posix.c > CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.i

CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/derry/github/up2p/src/up2pc/up2p_posix.c -o CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.s

CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.o.requires:

.PHONY : CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.o.requires

CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.o.provides: CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.o.requires
	$(MAKE) -f CMakeFiles/up2pc.0.1.1.dir/build.make CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.o.provides.build
.PHONY : CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.o.provides

CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.o.provides.build: CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.o


CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.o: CMakeFiles/up2pc.0.1.1.dir/flags.make
CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.o: ../src/up2pc/up2pc.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/derry/github/up2p/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.o   -c /home/derry/github/up2p/src/up2pc/up2pc.c

CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/derry/github/up2p/src/up2pc/up2pc.c > CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.i

CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/derry/github/up2p/src/up2pc/up2pc.c -o CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.s

CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.o.requires:

.PHONY : CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.o.requires

CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.o.provides: CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.o.requires
	$(MAKE) -f CMakeFiles/up2pc.0.1.1.dir/build.make CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.o.provides.build
.PHONY : CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.o.provides

CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.o.provides.build: CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.o


CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.o: CMakeFiles/up2pc.0.1.1.dir/flags.make
CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.o: ../src/util/util.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/derry/github/up2p/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.o   -c /home/derry/github/up2p/src/util/util.c

CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/derry/github/up2p/src/util/util.c > CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.i

CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/derry/github/up2p/src/util/util.c -o CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.s

CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.o.requires:

.PHONY : CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.o.requires

CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.o.provides: CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.o.requires
	$(MAKE) -f CMakeFiles/up2pc.0.1.1.dir/build.make CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.o.provides.build
.PHONY : CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.o.provides

CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.o.provides.build: CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.o


CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.o: CMakeFiles/up2pc.0.1.1.dir/flags.make
CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.o: ../src/util/observer_queue.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/derry/github/up2p/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.o   -c /home/derry/github/up2p/src/util/observer_queue.c

CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/derry/github/up2p/src/util/observer_queue.c > CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.i

CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/derry/github/up2p/src/util/observer_queue.c -o CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.s

CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.o.requires:

.PHONY : CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.o.requires

CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.o.provides: CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.o.requires
	$(MAKE) -f CMakeFiles/up2pc.0.1.1.dir/build.make CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.o.provides.build
.PHONY : CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.o.provides

CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.o.provides.build: CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.o


CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.o: CMakeFiles/up2pc.0.1.1.dir/flags.make
CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.o: ../src/util/debug.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/derry/github/up2p/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.o   -c /home/derry/github/up2p/src/util/debug.c

CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/derry/github/up2p/src/util/debug.c > CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.i

CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/derry/github/up2p/src/util/debug.c -o CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.s

CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.o.requires:

.PHONY : CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.o.requires

CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.o.provides: CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.o.requires
	$(MAKE) -f CMakeFiles/up2pc.0.1.1.dir/build.make CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.o.provides.build
.PHONY : CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.o.provides

CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.o.provides.build: CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.o


# Object files for target up2pc.0.1.1
up2pc_0_1_1_OBJECTS = \
"CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.o" \
"CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.o" \
"CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.o" \
"CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.o" \
"CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.o" \
"CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.o" \
"CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.o"

# External object files for target up2pc.0.1.1
up2pc_0_1_1_EXTERNAL_OBJECTS =

../lib/libup2pc.0.1.1.a: CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.o
../lib/libup2pc.0.1.1.a: CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.o
../lib/libup2pc.0.1.1.a: CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.o
../lib/libup2pc.0.1.1.a: CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.o
../lib/libup2pc.0.1.1.a: CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.o
../lib/libup2pc.0.1.1.a: CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.o
../lib/libup2pc.0.1.1.a: CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.o
../lib/libup2pc.0.1.1.a: CMakeFiles/up2pc.0.1.1.dir/build.make
../lib/libup2pc.0.1.1.a: CMakeFiles/up2pc.0.1.1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/derry/github/up2p/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking C static library ../lib/libup2pc.0.1.1.a"
	$(CMAKE_COMMAND) -P CMakeFiles/up2pc.0.1.1.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/up2pc.0.1.1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/up2pc.0.1.1.dir/build: ../lib/libup2pc.0.1.1.a

.PHONY : CMakeFiles/up2pc.0.1.1.dir/build

CMakeFiles/up2pc.0.1.1.dir/requires: CMakeFiles/up2pc.0.1.1.dir/src/crypt/aes.c.o.requires
CMakeFiles/up2pc.0.1.1.dir/requires: CMakeFiles/up2pc.0.1.1.dir/src/crypt/up2p_crypt.c.o.requires
CMakeFiles/up2pc.0.1.1.dir/requires: CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2p_posix.c.o.requires
CMakeFiles/up2pc.0.1.1.dir/requires: CMakeFiles/up2pc.0.1.1.dir/src/up2pc/up2pc.c.o.requires
CMakeFiles/up2pc.0.1.1.dir/requires: CMakeFiles/up2pc.0.1.1.dir/src/util/util.c.o.requires
CMakeFiles/up2pc.0.1.1.dir/requires: CMakeFiles/up2pc.0.1.1.dir/src/util/observer_queue.c.o.requires
CMakeFiles/up2pc.0.1.1.dir/requires: CMakeFiles/up2pc.0.1.1.dir/src/util/debug.c.o.requires

.PHONY : CMakeFiles/up2pc.0.1.1.dir/requires

CMakeFiles/up2pc.0.1.1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/up2pc.0.1.1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/up2pc.0.1.1.dir/clean

CMakeFiles/up2pc.0.1.1.dir/depend:
	cd /home/derry/github/up2p/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/derry/github/up2p /home/derry/github/up2p /home/derry/github/up2p/build /home/derry/github/up2p/build /home/derry/github/up2p/build/CMakeFiles/up2pc.0.1.1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/up2pc.0.1.1.dir/depend

