# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.10.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.10.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/finefenng/WorkSpace/cpp/QPSTest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/finefenng/WorkSpace/cpp/QPSTest/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/QPSTest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/QPSTest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/QPSTest.dir/flags.make

CMakeFiles/QPSTest.dir/main.cpp.o: CMakeFiles/QPSTest.dir/flags.make
CMakeFiles/QPSTest.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/finefenng/WorkSpace/cpp/QPSTest/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/QPSTest.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QPSTest.dir/main.cpp.o -c /Users/finefenng/WorkSpace/cpp/QPSTest/main.cpp

CMakeFiles/QPSTest.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QPSTest.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/finefenng/WorkSpace/cpp/QPSTest/main.cpp > CMakeFiles/QPSTest.dir/main.cpp.i

CMakeFiles/QPSTest.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QPSTest.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/finefenng/WorkSpace/cpp/QPSTest/main.cpp -o CMakeFiles/QPSTest.dir/main.cpp.s

CMakeFiles/QPSTest.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/QPSTest.dir/main.cpp.o.requires

CMakeFiles/QPSTest.dir/main.cpp.o.provides: CMakeFiles/QPSTest.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/QPSTest.dir/build.make CMakeFiles/QPSTest.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/QPSTest.dir/main.cpp.o.provides

CMakeFiles/QPSTest.dir/main.cpp.o.provides.build: CMakeFiles/QPSTest.dir/main.cpp.o


# Object files for target QPSTest
QPSTest_OBJECTS = \
"CMakeFiles/QPSTest.dir/main.cpp.o"

# External object files for target QPSTest
QPSTest_EXTERNAL_OBJECTS =

QPSTest: CMakeFiles/QPSTest.dir/main.cpp.o
QPSTest: CMakeFiles/QPSTest.dir/build.make
QPSTest: CMakeFiles/QPSTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/finefenng/WorkSpace/cpp/QPSTest/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable QPSTest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/QPSTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/QPSTest.dir/build: QPSTest

.PHONY : CMakeFiles/QPSTest.dir/build

CMakeFiles/QPSTest.dir/requires: CMakeFiles/QPSTest.dir/main.cpp.o.requires

.PHONY : CMakeFiles/QPSTest.dir/requires

CMakeFiles/QPSTest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/QPSTest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/QPSTest.dir/clean

CMakeFiles/QPSTest.dir/depend:
	cd /Users/finefenng/WorkSpace/cpp/QPSTest/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/finefenng/WorkSpace/cpp/QPSTest /Users/finefenng/WorkSpace/cpp/QPSTest /Users/finefenng/WorkSpace/cpp/QPSTest/cmake-build-debug /Users/finefenng/WorkSpace/cpp/QPSTest/cmake-build-debug /Users/finefenng/WorkSpace/cpp/QPSTest/cmake-build-debug/CMakeFiles/QPSTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/QPSTest.dir/depend

