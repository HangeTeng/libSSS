# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/original/STComm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/original/STComm/build

# Include any dependencies generated for this target.
include CMakeFiles/test_osn.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_osn.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_osn.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_osn.dir/flags.make

CMakeFiles/test_osn.dir/test/test_osn.cpp.o: CMakeFiles/test_osn.dir/flags.make
CMakeFiles/test_osn.dir/test/test_osn.cpp.o: ../test/test_osn.cpp
CMakeFiles/test_osn.dir/test/test_osn.cpp.o: CMakeFiles/test_osn.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/original/STComm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_osn.dir/test/test_osn.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_osn.dir/test/test_osn.cpp.o -MF CMakeFiles/test_osn.dir/test/test_osn.cpp.o.d -o CMakeFiles/test_osn.dir/test/test_osn.cpp.o -c /home/original/STComm/test/test_osn.cpp

CMakeFiles/test_osn.dir/test/test_osn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_osn.dir/test/test_osn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/original/STComm/test/test_osn.cpp > CMakeFiles/test_osn.dir/test/test_osn.cpp.i

CMakeFiles/test_osn.dir/test/test_osn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_osn.dir/test/test_osn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/original/STComm/test/test_osn.cpp -o CMakeFiles/test_osn.dir/test/test_osn.cpp.s

# Object files for target test_osn
test_osn_OBJECTS = \
"CMakeFiles/test_osn.dir/test/test_osn.cpp.o"

# External object files for target test_osn
test_osn_EXTERNAL_OBJECTS =

test_osn: CMakeFiles/test_osn.dir/test/test_osn.cpp.o
test_osn: CMakeFiles/test_osn.dir/build.make
test_osn: libosn.so
test_osn: ../libOTe/cryptoTools/thirdparty/unix/lib/libboost_filesystem.so
test_osn: ../libOTe/cryptoTools/thirdparty/unix/lib/libboost_system.so
test_osn: ../libOTe/cryptoTools/thirdparty/unix/lib/libboost_thread.so
test_osn: ../libOTe/cryptoTools/thirdparty/unix/lib/libboost_atomic.so
test_osn: ../libOTe/cryptoTools/thirdparty/unix/lib/libboost_regex.so
test_osn: CMakeFiles/test_osn.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/original/STComm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_osn"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_osn.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_osn.dir/build: test_osn
.PHONY : CMakeFiles/test_osn.dir/build

CMakeFiles/test_osn.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_osn.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_osn.dir/clean

CMakeFiles/test_osn.dir/depend:
	cd /home/original/STComm/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/original/STComm /home/original/STComm /home/original/STComm/build /home/original/STComm/build /home/original/STComm/build/CMakeFiles/test_osn.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_osn.dir/depend

