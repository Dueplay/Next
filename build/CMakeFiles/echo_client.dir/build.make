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
CMAKE_SOURCE_DIR = /home/gxj/Next

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gxj/Next/build

# Include any dependencies generated for this target.
include CMakeFiles/echo_client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/echo_client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/echo_client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/echo_client.dir/flags.make

CMakeFiles/echo_client.dir/demo/echo/echo_client.cpp.o: CMakeFiles/echo_client.dir/flags.make
CMakeFiles/echo_client.dir/demo/echo/echo_client.cpp.o: ../demo/echo/echo_client.cpp
CMakeFiles/echo_client.dir/demo/echo/echo_client.cpp.o: CMakeFiles/echo_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gxj/Next/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/echo_client.dir/demo/echo/echo_client.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/echo_client.dir/demo/echo/echo_client.cpp.o -MF CMakeFiles/echo_client.dir/demo/echo/echo_client.cpp.o.d -o CMakeFiles/echo_client.dir/demo/echo/echo_client.cpp.o -c /home/gxj/Next/demo/echo/echo_client.cpp

CMakeFiles/echo_client.dir/demo/echo/echo_client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/echo_client.dir/demo/echo/echo_client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gxj/Next/demo/echo/echo_client.cpp > CMakeFiles/echo_client.dir/demo/echo/echo_client.cpp.i

CMakeFiles/echo_client.dir/demo/echo/echo_client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/echo_client.dir/demo/echo/echo_client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gxj/Next/demo/echo/echo_client.cpp -o CMakeFiles/echo_client.dir/demo/echo/echo_client.cpp.s

# Object files for target echo_client
echo_client_OBJECTS = \
"CMakeFiles/echo_client.dir/demo/echo/echo_client.cpp.o"

# External object files for target echo_client
echo_client_EXTERNAL_OBJECTS =

../bin/echo_client: CMakeFiles/echo_client.dir/demo/echo/echo_client.cpp.o
../bin/echo_client: CMakeFiles/echo_client.dir/build.make
../bin/echo_client: ../lib/libnext_core.a
../bin/echo_client: ../lib/libnext_log.a
../bin/echo_client: CMakeFiles/echo_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gxj/Next/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/echo_client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/echo_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/echo_client.dir/build: ../bin/echo_client
.PHONY : CMakeFiles/echo_client.dir/build

CMakeFiles/echo_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/echo_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/echo_client.dir/clean

CMakeFiles/echo_client.dir/depend:
	cd /home/gxj/Next/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gxj/Next /home/gxj/Next /home/gxj/Next/build /home/gxj/Next/build /home/gxj/Next/build/CMakeFiles/echo_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/echo_client.dir/depend

