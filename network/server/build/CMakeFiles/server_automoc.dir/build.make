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
CMAKE_SOURCE_DIR = /home/steffen/praktikum4/network/server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/steffen/praktikum4/network/server/build

# Utility rule file for server_automoc.

# Include the progress variables for this target.
include CMakeFiles/server_automoc.dir/progress.make

CMakeFiles/server_automoc:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/steffen/praktikum4/network/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic moc for target server"
	/usr/bin/cmake -E cmake_autogen /home/steffen/praktikum4/network/server/build/CMakeFiles/server_automoc.dir/ ""

server_automoc: CMakeFiles/server_automoc
server_automoc: CMakeFiles/server_automoc.dir/build.make

.PHONY : server_automoc

# Rule to build all files generated by this target.
CMakeFiles/server_automoc.dir/build: server_automoc

.PHONY : CMakeFiles/server_automoc.dir/build

CMakeFiles/server_automoc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server_automoc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server_automoc.dir/clean

CMakeFiles/server_automoc.dir/depend:
	cd /home/steffen/praktikum4/network/server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/steffen/praktikum4/network/server /home/steffen/praktikum4/network/server /home/steffen/praktikum4/network/server/build /home/steffen/praktikum4/network/server/build /home/steffen/praktikum4/network/server/build/CMakeFiles/server_automoc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server_automoc.dir/depend

