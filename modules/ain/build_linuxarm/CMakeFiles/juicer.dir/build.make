# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/sheinb/src/dserv/modules/juicer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sheinb/src/dserv/modules/juicer/build_linuxarm

# Include any dependencies generated for this target.
include CMakeFiles/juicer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/juicer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/juicer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/juicer.dir/flags.make

CMakeFiles/juicer.dir/src/juicer.cpp.o: CMakeFiles/juicer.dir/flags.make
CMakeFiles/juicer.dir/src/juicer.cpp.o: /home/sheinb/src/dserv/modules/juicer/src/juicer.cpp
CMakeFiles/juicer.dir/src/juicer.cpp.o: CMakeFiles/juicer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sheinb/src/dserv/modules/juicer/build_linuxarm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/juicer.dir/src/juicer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/juicer.dir/src/juicer.cpp.o -MF CMakeFiles/juicer.dir/src/juicer.cpp.o.d -o CMakeFiles/juicer.dir/src/juicer.cpp.o -c /home/sheinb/src/dserv/modules/juicer/src/juicer.cpp

CMakeFiles/juicer.dir/src/juicer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/juicer.dir/src/juicer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sheinb/src/dserv/modules/juicer/src/juicer.cpp > CMakeFiles/juicer.dir/src/juicer.cpp.i

CMakeFiles/juicer.dir/src/juicer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/juicer.dir/src/juicer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sheinb/src/dserv/modules/juicer/src/juicer.cpp -o CMakeFiles/juicer.dir/src/juicer.cpp.s

# Object files for target juicer
juicer_OBJECTS = \
"CMakeFiles/juicer.dir/src/juicer.cpp.o"

# External object files for target juicer
juicer_EXTERNAL_OBJECTS =

dserv_juicer.so: CMakeFiles/juicer.dir/src/juicer.cpp.o
dserv_juicer.so: CMakeFiles/juicer.dir/build.make
dserv_juicer.so: CMakeFiles/juicer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sheinb/src/dserv/modules/juicer/build_linuxarm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library dserv_juicer.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/juicer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/juicer.dir/build: dserv_juicer.so
.PHONY : CMakeFiles/juicer.dir/build

CMakeFiles/juicer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/juicer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/juicer.dir/clean

CMakeFiles/juicer.dir/depend:
	cd /home/sheinb/src/dserv/modules/juicer/build_linuxarm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sheinb/src/dserv/modules/juicer /home/sheinb/src/dserv/modules/juicer /home/sheinb/src/dserv/modules/juicer/build_linuxarm /home/sheinb/src/dserv/modules/juicer/build_linuxarm /home/sheinb/src/dserv/modules/juicer/build_linuxarm/CMakeFiles/juicer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/juicer.dir/depend

