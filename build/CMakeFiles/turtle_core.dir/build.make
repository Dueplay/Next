# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /home/book/Downloads/cmake-3.27.4-linux-x86_64/bin/cmake

# The command to remove a file.
RM = /home/book/Downloads/cmake-3.27.4-linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/book/study/Next

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/book/study/Next/build

# Include any dependencies generated for this target.
include CMakeFiles/turtle_core.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/turtle_core.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/turtle_core.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/turtle_core.dir/flags.make

CMakeFiles/turtle_core.dir/src/core/acceptor.cpp.o: CMakeFiles/turtle_core.dir/flags.make
CMakeFiles/turtle_core.dir/src/core/acceptor.cpp.o: /home/book/study/Next/src/core/acceptor.cpp
CMakeFiles/turtle_core.dir/src/core/acceptor.cpp.o: CMakeFiles/turtle_core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/book/study/Next/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/turtle_core.dir/src/core/acceptor.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turtle_core.dir/src/core/acceptor.cpp.o -MF CMakeFiles/turtle_core.dir/src/core/acceptor.cpp.o.d -o CMakeFiles/turtle_core.dir/src/core/acceptor.cpp.o -c /home/book/study/Next/src/core/acceptor.cpp

CMakeFiles/turtle_core.dir/src/core/acceptor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/turtle_core.dir/src/core/acceptor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/book/study/Next/src/core/acceptor.cpp > CMakeFiles/turtle_core.dir/src/core/acceptor.cpp.i

CMakeFiles/turtle_core.dir/src/core/acceptor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/turtle_core.dir/src/core/acceptor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/book/study/Next/src/core/acceptor.cpp -o CMakeFiles/turtle_core.dir/src/core/acceptor.cpp.s

CMakeFiles/turtle_core.dir/src/core/buffer.cpp.o: CMakeFiles/turtle_core.dir/flags.make
CMakeFiles/turtle_core.dir/src/core/buffer.cpp.o: /home/book/study/Next/src/core/buffer.cpp
CMakeFiles/turtle_core.dir/src/core/buffer.cpp.o: CMakeFiles/turtle_core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/book/study/Next/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/turtle_core.dir/src/core/buffer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turtle_core.dir/src/core/buffer.cpp.o -MF CMakeFiles/turtle_core.dir/src/core/buffer.cpp.o.d -o CMakeFiles/turtle_core.dir/src/core/buffer.cpp.o -c /home/book/study/Next/src/core/buffer.cpp

CMakeFiles/turtle_core.dir/src/core/buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/turtle_core.dir/src/core/buffer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/book/study/Next/src/core/buffer.cpp > CMakeFiles/turtle_core.dir/src/core/buffer.cpp.i

CMakeFiles/turtle_core.dir/src/core/buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/turtle_core.dir/src/core/buffer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/book/study/Next/src/core/buffer.cpp -o CMakeFiles/turtle_core.dir/src/core/buffer.cpp.s

CMakeFiles/turtle_core.dir/src/core/cache.cpp.o: CMakeFiles/turtle_core.dir/flags.make
CMakeFiles/turtle_core.dir/src/core/cache.cpp.o: /home/book/study/Next/src/core/cache.cpp
CMakeFiles/turtle_core.dir/src/core/cache.cpp.o: CMakeFiles/turtle_core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/book/study/Next/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/turtle_core.dir/src/core/cache.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turtle_core.dir/src/core/cache.cpp.o -MF CMakeFiles/turtle_core.dir/src/core/cache.cpp.o.d -o CMakeFiles/turtle_core.dir/src/core/cache.cpp.o -c /home/book/study/Next/src/core/cache.cpp

CMakeFiles/turtle_core.dir/src/core/cache.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/turtle_core.dir/src/core/cache.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/book/study/Next/src/core/cache.cpp > CMakeFiles/turtle_core.dir/src/core/cache.cpp.i

CMakeFiles/turtle_core.dir/src/core/cache.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/turtle_core.dir/src/core/cache.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/book/study/Next/src/core/cache.cpp -o CMakeFiles/turtle_core.dir/src/core/cache.cpp.s

CMakeFiles/turtle_core.dir/src/core/connection.cpp.o: CMakeFiles/turtle_core.dir/flags.make
CMakeFiles/turtle_core.dir/src/core/connection.cpp.o: /home/book/study/Next/src/core/connection.cpp
CMakeFiles/turtle_core.dir/src/core/connection.cpp.o: CMakeFiles/turtle_core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/book/study/Next/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/turtle_core.dir/src/core/connection.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turtle_core.dir/src/core/connection.cpp.o -MF CMakeFiles/turtle_core.dir/src/core/connection.cpp.o.d -o CMakeFiles/turtle_core.dir/src/core/connection.cpp.o -c /home/book/study/Next/src/core/connection.cpp

CMakeFiles/turtle_core.dir/src/core/connection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/turtle_core.dir/src/core/connection.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/book/study/Next/src/core/connection.cpp > CMakeFiles/turtle_core.dir/src/core/connection.cpp.i

CMakeFiles/turtle_core.dir/src/core/connection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/turtle_core.dir/src/core/connection.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/book/study/Next/src/core/connection.cpp -o CMakeFiles/turtle_core.dir/src/core/connection.cpp.s

CMakeFiles/turtle_core.dir/src/core/looper.cpp.o: CMakeFiles/turtle_core.dir/flags.make
CMakeFiles/turtle_core.dir/src/core/looper.cpp.o: /home/book/study/Next/src/core/looper.cpp
CMakeFiles/turtle_core.dir/src/core/looper.cpp.o: CMakeFiles/turtle_core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/book/study/Next/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/turtle_core.dir/src/core/looper.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turtle_core.dir/src/core/looper.cpp.o -MF CMakeFiles/turtle_core.dir/src/core/looper.cpp.o.d -o CMakeFiles/turtle_core.dir/src/core/looper.cpp.o -c /home/book/study/Next/src/core/looper.cpp

CMakeFiles/turtle_core.dir/src/core/looper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/turtle_core.dir/src/core/looper.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/book/study/Next/src/core/looper.cpp > CMakeFiles/turtle_core.dir/src/core/looper.cpp.i

CMakeFiles/turtle_core.dir/src/core/looper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/turtle_core.dir/src/core/looper.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/book/study/Next/src/core/looper.cpp -o CMakeFiles/turtle_core.dir/src/core/looper.cpp.s

CMakeFiles/turtle_core.dir/src/core/net_addr.cpp.o: CMakeFiles/turtle_core.dir/flags.make
CMakeFiles/turtle_core.dir/src/core/net_addr.cpp.o: /home/book/study/Next/src/core/net_addr.cpp
CMakeFiles/turtle_core.dir/src/core/net_addr.cpp.o: CMakeFiles/turtle_core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/book/study/Next/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/turtle_core.dir/src/core/net_addr.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turtle_core.dir/src/core/net_addr.cpp.o -MF CMakeFiles/turtle_core.dir/src/core/net_addr.cpp.o.d -o CMakeFiles/turtle_core.dir/src/core/net_addr.cpp.o -c /home/book/study/Next/src/core/net_addr.cpp

CMakeFiles/turtle_core.dir/src/core/net_addr.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/turtle_core.dir/src/core/net_addr.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/book/study/Next/src/core/net_addr.cpp > CMakeFiles/turtle_core.dir/src/core/net_addr.cpp.i

CMakeFiles/turtle_core.dir/src/core/net_addr.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/turtle_core.dir/src/core/net_addr.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/book/study/Next/src/core/net_addr.cpp -o CMakeFiles/turtle_core.dir/src/core/net_addr.cpp.s

CMakeFiles/turtle_core.dir/src/core/poller.cpp.o: CMakeFiles/turtle_core.dir/flags.make
CMakeFiles/turtle_core.dir/src/core/poller.cpp.o: /home/book/study/Next/src/core/poller.cpp
CMakeFiles/turtle_core.dir/src/core/poller.cpp.o: CMakeFiles/turtle_core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/book/study/Next/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/turtle_core.dir/src/core/poller.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turtle_core.dir/src/core/poller.cpp.o -MF CMakeFiles/turtle_core.dir/src/core/poller.cpp.o.d -o CMakeFiles/turtle_core.dir/src/core/poller.cpp.o -c /home/book/study/Next/src/core/poller.cpp

CMakeFiles/turtle_core.dir/src/core/poller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/turtle_core.dir/src/core/poller.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/book/study/Next/src/core/poller.cpp > CMakeFiles/turtle_core.dir/src/core/poller.cpp.i

CMakeFiles/turtle_core.dir/src/core/poller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/turtle_core.dir/src/core/poller.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/book/study/Next/src/core/poller.cpp -o CMakeFiles/turtle_core.dir/src/core/poller.cpp.s

CMakeFiles/turtle_core.dir/src/core/socket.cpp.o: CMakeFiles/turtle_core.dir/flags.make
CMakeFiles/turtle_core.dir/src/core/socket.cpp.o: /home/book/study/Next/src/core/socket.cpp
CMakeFiles/turtle_core.dir/src/core/socket.cpp.o: CMakeFiles/turtle_core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/book/study/Next/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/turtle_core.dir/src/core/socket.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turtle_core.dir/src/core/socket.cpp.o -MF CMakeFiles/turtle_core.dir/src/core/socket.cpp.o.d -o CMakeFiles/turtle_core.dir/src/core/socket.cpp.o -c /home/book/study/Next/src/core/socket.cpp

CMakeFiles/turtle_core.dir/src/core/socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/turtle_core.dir/src/core/socket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/book/study/Next/src/core/socket.cpp > CMakeFiles/turtle_core.dir/src/core/socket.cpp.i

CMakeFiles/turtle_core.dir/src/core/socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/turtle_core.dir/src/core/socket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/book/study/Next/src/core/socket.cpp -o CMakeFiles/turtle_core.dir/src/core/socket.cpp.s

CMakeFiles/turtle_core.dir/src/core/thread_pool.cpp.o: CMakeFiles/turtle_core.dir/flags.make
CMakeFiles/turtle_core.dir/src/core/thread_pool.cpp.o: /home/book/study/Next/src/core/thread_pool.cpp
CMakeFiles/turtle_core.dir/src/core/thread_pool.cpp.o: CMakeFiles/turtle_core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/book/study/Next/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/turtle_core.dir/src/core/thread_pool.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turtle_core.dir/src/core/thread_pool.cpp.o -MF CMakeFiles/turtle_core.dir/src/core/thread_pool.cpp.o.d -o CMakeFiles/turtle_core.dir/src/core/thread_pool.cpp.o -c /home/book/study/Next/src/core/thread_pool.cpp

CMakeFiles/turtle_core.dir/src/core/thread_pool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/turtle_core.dir/src/core/thread_pool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/book/study/Next/src/core/thread_pool.cpp > CMakeFiles/turtle_core.dir/src/core/thread_pool.cpp.i

CMakeFiles/turtle_core.dir/src/core/thread_pool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/turtle_core.dir/src/core/thread_pool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/book/study/Next/src/core/thread_pool.cpp -o CMakeFiles/turtle_core.dir/src/core/thread_pool.cpp.s

CMakeFiles/turtle_core.dir/src/core/timer.cpp.o: CMakeFiles/turtle_core.dir/flags.make
CMakeFiles/turtle_core.dir/src/core/timer.cpp.o: /home/book/study/Next/src/core/timer.cpp
CMakeFiles/turtle_core.dir/src/core/timer.cpp.o: CMakeFiles/turtle_core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/book/study/Next/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/turtle_core.dir/src/core/timer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/turtle_core.dir/src/core/timer.cpp.o -MF CMakeFiles/turtle_core.dir/src/core/timer.cpp.o.d -o CMakeFiles/turtle_core.dir/src/core/timer.cpp.o -c /home/book/study/Next/src/core/timer.cpp

CMakeFiles/turtle_core.dir/src/core/timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/turtle_core.dir/src/core/timer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/book/study/Next/src/core/timer.cpp > CMakeFiles/turtle_core.dir/src/core/timer.cpp.i

CMakeFiles/turtle_core.dir/src/core/timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/turtle_core.dir/src/core/timer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/book/study/Next/src/core/timer.cpp -o CMakeFiles/turtle_core.dir/src/core/timer.cpp.s

# Object files for target turtle_core
turtle_core_OBJECTS = \
"CMakeFiles/turtle_core.dir/src/core/acceptor.cpp.o" \
"CMakeFiles/turtle_core.dir/src/core/buffer.cpp.o" \
"CMakeFiles/turtle_core.dir/src/core/cache.cpp.o" \
"CMakeFiles/turtle_core.dir/src/core/connection.cpp.o" \
"CMakeFiles/turtle_core.dir/src/core/looper.cpp.o" \
"CMakeFiles/turtle_core.dir/src/core/net_addr.cpp.o" \
"CMakeFiles/turtle_core.dir/src/core/poller.cpp.o" \
"CMakeFiles/turtle_core.dir/src/core/socket.cpp.o" \
"CMakeFiles/turtle_core.dir/src/core/thread_pool.cpp.o" \
"CMakeFiles/turtle_core.dir/src/core/timer.cpp.o"

# External object files for target turtle_core
turtle_core_EXTERNAL_OBJECTS =

/home/book/study/Next/lib/libturtle_core.a: CMakeFiles/turtle_core.dir/src/core/acceptor.cpp.o
/home/book/study/Next/lib/libturtle_core.a: CMakeFiles/turtle_core.dir/src/core/buffer.cpp.o
/home/book/study/Next/lib/libturtle_core.a: CMakeFiles/turtle_core.dir/src/core/cache.cpp.o
/home/book/study/Next/lib/libturtle_core.a: CMakeFiles/turtle_core.dir/src/core/connection.cpp.o
/home/book/study/Next/lib/libturtle_core.a: CMakeFiles/turtle_core.dir/src/core/looper.cpp.o
/home/book/study/Next/lib/libturtle_core.a: CMakeFiles/turtle_core.dir/src/core/net_addr.cpp.o
/home/book/study/Next/lib/libturtle_core.a: CMakeFiles/turtle_core.dir/src/core/poller.cpp.o
/home/book/study/Next/lib/libturtle_core.a: CMakeFiles/turtle_core.dir/src/core/socket.cpp.o
/home/book/study/Next/lib/libturtle_core.a: CMakeFiles/turtle_core.dir/src/core/thread_pool.cpp.o
/home/book/study/Next/lib/libturtle_core.a: CMakeFiles/turtle_core.dir/src/core/timer.cpp.o
/home/book/study/Next/lib/libturtle_core.a: CMakeFiles/turtle_core.dir/build.make
/home/book/study/Next/lib/libturtle_core.a: CMakeFiles/turtle_core.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/book/study/Next/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX static library /home/book/study/Next/lib/libturtle_core.a"
	$(CMAKE_COMMAND) -P CMakeFiles/turtle_core.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/turtle_core.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/turtle_core.dir/build: /home/book/study/Next/lib/libturtle_core.a
.PHONY : CMakeFiles/turtle_core.dir/build

CMakeFiles/turtle_core.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/turtle_core.dir/cmake_clean.cmake
.PHONY : CMakeFiles/turtle_core.dir/clean

CMakeFiles/turtle_core.dir/depend:
	cd /home/book/study/Next/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/book/study/Next /home/book/study/Next /home/book/study/Next/build /home/book/study/Next/build /home/book/study/Next/build/CMakeFiles/turtle_core.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/turtle_core.dir/depend

