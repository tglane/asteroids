# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.14.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.14.0/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Tristan/CLionProjects/praktikum4/network/testclient

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Tristan/CLionProjects/praktikum4/network/testclient/build

# Include any dependencies generated for this target.
include CMakeFiles/client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.dir/flags.make

moc_testclient_tcp.cpp: ../testclient_tcp.hpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/Tristan/CLionProjects/praktikum4/network/testclient/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating moc_testclient_tcp.cpp"
	/usr/local/opt/qt/bin/moc @/Users/Tristan/CLionProjects/praktikum4/network/testclient/build/moc_testclient_tcp.cpp_parameters

CMakeFiles/client.dir/client_autogen/mocs_compilation.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/client_autogen/mocs_compilation.cpp.o: client_autogen/mocs_compilation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Tristan/CLionProjects/praktikum4/network/testclient/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/client.dir/client_autogen/mocs_compilation.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/client_autogen/mocs_compilation.cpp.o -c /Users/Tristan/CLionProjects/praktikum4/network/testclient/build/client_autogen/mocs_compilation.cpp

CMakeFiles/client.dir/client_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/client_autogen/mocs_compilation.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Tristan/CLionProjects/praktikum4/network/testclient/build/client_autogen/mocs_compilation.cpp > CMakeFiles/client.dir/client_autogen/mocs_compilation.cpp.i

CMakeFiles/client.dir/client_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/client_autogen/mocs_compilation.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Tristan/CLionProjects/praktikum4/network/testclient/build/client_autogen/mocs_compilation.cpp -o CMakeFiles/client.dir/client_autogen/mocs_compilation.cpp.s

CMakeFiles/client.dir/moc_testclient_tcp.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/moc_testclient_tcp.cpp.o: moc_testclient_tcp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Tristan/CLionProjects/praktikum4/network/testclient/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/client.dir/moc_testclient_tcp.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/moc_testclient_tcp.cpp.o -c /Users/Tristan/CLionProjects/praktikum4/network/testclient/build/moc_testclient_tcp.cpp

CMakeFiles/client.dir/moc_testclient_tcp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/moc_testclient_tcp.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Tristan/CLionProjects/praktikum4/network/testclient/build/moc_testclient_tcp.cpp > CMakeFiles/client.dir/moc_testclient_tcp.cpp.i

CMakeFiles/client.dir/moc_testclient_tcp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/moc_testclient_tcp.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Tristan/CLionProjects/praktikum4/network/testclient/build/moc_testclient_tcp.cpp -o CMakeFiles/client.dir/moc_testclient_tcp.cpp.s

CMakeFiles/client.dir/testclient_tcp.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/testclient_tcp.cpp.o: ../testclient_tcp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Tristan/CLionProjects/praktikum4/network/testclient/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/client.dir/testclient_tcp.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/testclient_tcp.cpp.o -c /Users/Tristan/CLionProjects/praktikum4/network/testclient/testclient_tcp.cpp

CMakeFiles/client.dir/testclient_tcp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/testclient_tcp.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Tristan/CLionProjects/praktikum4/network/testclient/testclient_tcp.cpp > CMakeFiles/client.dir/testclient_tcp.cpp.i

CMakeFiles/client.dir/testclient_tcp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/testclient_tcp.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Tristan/CLionProjects/praktikum4/network/testclient/testclient_tcp.cpp -o CMakeFiles/client.dir/testclient_tcp.cpp.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/client_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/client.dir/moc_testclient_tcp.cpp.o" \
"CMakeFiles/client.dir/testclient_tcp.cpp.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

client: CMakeFiles/client.dir/client_autogen/mocs_compilation.cpp.o
client: CMakeFiles/client.dir/moc_testclient_tcp.cpp.o
client: CMakeFiles/client.dir/testclient_tcp.cpp.o
client: CMakeFiles/client.dir/build.make
client: /usr/local/opt/qt/lib/QtNetwork.framework/QtNetwork
client: /usr/local/opt/qt/lib/QtCore.framework/QtCore
client: CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Tristan/CLionProjects/praktikum4/network/testclient/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client.dir/build: client

.PHONY : CMakeFiles/client.dir/build

CMakeFiles/client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.dir/clean

CMakeFiles/client.dir/depend: moc_testclient_tcp.cpp
	cd /Users/Tristan/CLionProjects/praktikum4/network/testclient/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Tristan/CLionProjects/praktikum4/network/testclient /Users/Tristan/CLionProjects/praktikum4/network/testclient /Users/Tristan/CLionProjects/praktikum4/network/testclient/build /Users/Tristan/CLionProjects/praktikum4/network/testclient/build /Users/Tristan/CLionProjects/praktikum4/network/testclient/build/CMakeFiles/client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client.dir/depend

