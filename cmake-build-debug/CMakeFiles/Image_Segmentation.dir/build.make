# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Image_Segmentation.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Image_Segmentation.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Image_Segmentation.dir/flags.make

CMakeFiles/Image_Segmentation.dir/src/main.c.o: CMakeFiles/Image_Segmentation.dir/flags.make
CMakeFiles/Image_Segmentation.dir/src/main.c.o: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Image_Segmentation.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Image_Segmentation.dir/src/main.c.o   -c "/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation/src/main.c"

CMakeFiles/Image_Segmentation.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Image_Segmentation.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation/src/main.c" > CMakeFiles/Image_Segmentation.dir/src/main.c.i

CMakeFiles/Image_Segmentation.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Image_Segmentation.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation/src/main.c" -o CMakeFiles/Image_Segmentation.dir/src/main.c.s

CMakeFiles/Image_Segmentation.dir/src/build_image_matrix.c.o: CMakeFiles/Image_Segmentation.dir/flags.make
CMakeFiles/Image_Segmentation.dir/src/build_image_matrix.c.o: ../src/build_image_matrix.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Image_Segmentation.dir/src/build_image_matrix.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Image_Segmentation.dir/src/build_image_matrix.c.o   -c "/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation/src/build_image_matrix.c"

CMakeFiles/Image_Segmentation.dir/src/build_image_matrix.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Image_Segmentation.dir/src/build_image_matrix.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation/src/build_image_matrix.c" > CMakeFiles/Image_Segmentation.dir/src/build_image_matrix.c.i

CMakeFiles/Image_Segmentation.dir/src/build_image_matrix.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Image_Segmentation.dir/src/build_image_matrix.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation/src/build_image_matrix.c" -o CMakeFiles/Image_Segmentation.dir/src/build_image_matrix.c.s

# Object files for target Image_Segmentation
Image_Segmentation_OBJECTS = \
"CMakeFiles/Image_Segmentation.dir/src/main.c.o" \
"CMakeFiles/Image_Segmentation.dir/src/build_image_matrix.c.o"

# External object files for target Image_Segmentation
Image_Segmentation_EXTERNAL_OBJECTS =

../BUILD/Image_Segmentation: CMakeFiles/Image_Segmentation.dir/src/main.c.o
../BUILD/Image_Segmentation: CMakeFiles/Image_Segmentation.dir/src/build_image_matrix.c.o
../BUILD/Image_Segmentation: CMakeFiles/Image_Segmentation.dir/build.make
../BUILD/Image_Segmentation: CMakeFiles/Image_Segmentation.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable ../BUILD/Image_Segmentation"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Image_Segmentation.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Image_Segmentation.dir/build: ../BUILD/Image_Segmentation

.PHONY : CMakeFiles/Image_Segmentation.dir/build

CMakeFiles/Image_Segmentation.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Image_Segmentation.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Image_Segmentation.dir/clean

CMakeFiles/Image_Segmentation.dir/depend:
	cd "/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation" "/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation" "/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation/cmake-build-debug" "/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation/cmake-build-debug" "/Users/arnauochoa/Documents/Estudis/MSc/Semester 1/C_prog/Image-Segmentation/cmake-build-debug/CMakeFiles/Image_Segmentation.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Image_Segmentation.dir/depend

