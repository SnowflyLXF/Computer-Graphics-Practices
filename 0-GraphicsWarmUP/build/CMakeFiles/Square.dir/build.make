# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/lixuefei/Desktop/starter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/lixuefei/Desktop/starter/build

# Include any dependencies generated for this target.
include CMakeFiles/Square.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Square.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Square.dir/flags.make

CMakeFiles/Square.dir/Square.cpp.o: CMakeFiles/Square.dir/flags.make
CMakeFiles/Square.dir/Square.cpp.o: ../Square.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lixuefei/Desktop/starter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Square.dir/Square.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Square.dir/Square.cpp.o -c /Users/lixuefei/Desktop/starter/Square.cpp

CMakeFiles/Square.dir/Square.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Square.dir/Square.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lixuefei/Desktop/starter/Square.cpp > CMakeFiles/Square.dir/Square.cpp.i

CMakeFiles/Square.dir/Square.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Square.dir/Square.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lixuefei/Desktop/starter/Square.cpp -o CMakeFiles/Square.dir/Square.cpp.s

# Object files for target Square
Square_OBJECTS = \
"CMakeFiles/Square.dir/Square.cpp.o"

# External object files for target Square
Square_EXTERNAL_OBJECTS =

Square: CMakeFiles/Square.dir/Square.cpp.o
Square: CMakeFiles/Square.dir/build.make
Square: glad/libglad.a
Square: CMakeFiles/Square.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/lixuefei/Desktop/starter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Square"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Square.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Square.dir/build: Square

.PHONY : CMakeFiles/Square.dir/build

CMakeFiles/Square.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Square.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Square.dir/clean

CMakeFiles/Square.dir/depend:
	cd /Users/lixuefei/Desktop/starter/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/lixuefei/Desktop/starter /Users/lixuefei/Desktop/starter /Users/lixuefei/Desktop/starter/build /Users/lixuefei/Desktop/starter/build /Users/lixuefei/Desktop/starter/build/CMakeFiles/Square.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Square.dir/depend
