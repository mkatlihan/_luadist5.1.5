# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mka/opt/xscript/lua/luadist/5.1.5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mka/opt/xscript/lua/luadist/5.1.5/_build/linux64x86

# Include any dependencies generated for this target.
include CMakeFiles/lua.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lua.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lua.dir/flags.make

CMakeFiles/lua.dir/src/lua.c.o: CMakeFiles/lua.dir/flags.make
CMakeFiles/lua.dir/src/lua.c.o: ../../src/lua.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mka/opt/xscript/lua/luadist/5.1.5/_build/linux64x86/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/lua.dir/src/lua.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/lua.dir/src/lua.c.o   -c /home/mka/opt/xscript/lua/luadist/5.1.5/src/lua.c

CMakeFiles/lua.dir/src/lua.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lua.dir/src/lua.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/mka/opt/xscript/lua/luadist/5.1.5/src/lua.c > CMakeFiles/lua.dir/src/lua.c.i

CMakeFiles/lua.dir/src/lua.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lua.dir/src/lua.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/mka/opt/xscript/lua/luadist/5.1.5/src/lua.c -o CMakeFiles/lua.dir/src/lua.c.s

CMakeFiles/lua.dir/src/lua.c.o.requires:
.PHONY : CMakeFiles/lua.dir/src/lua.c.o.requires

CMakeFiles/lua.dir/src/lua.c.o.provides: CMakeFiles/lua.dir/src/lua.c.o.requires
	$(MAKE) -f CMakeFiles/lua.dir/build.make CMakeFiles/lua.dir/src/lua.c.o.provides.build
.PHONY : CMakeFiles/lua.dir/src/lua.c.o.provides

CMakeFiles/lua.dir/src/lua.c.o.provides.build: CMakeFiles/lua.dir/src/lua.c.o

# Object files for target lua
lua_OBJECTS = \
"CMakeFiles/lua.dir/src/lua.c.o"

# External object files for target lua
lua_EXTERNAL_OBJECTS =

../../bin/linux64x86/lua: CMakeFiles/lua.dir/src/lua.c.o
../../bin/linux64x86/lua: CMakeFiles/lua.dir/build.make
../../bin/linux64x86/lua: ../../bin/linux64x86/liblua.so
../../bin/linux64x86/lua: /usr/lib64/libm.so
../../bin/linux64x86/lua: /usr/lib64/libdl.so
../../bin/linux64x86/lua: CMakeFiles/lua.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable ../../bin/linux64x86/lua"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lua.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lua.dir/build: ../../bin/linux64x86/lua
.PHONY : CMakeFiles/lua.dir/build

CMakeFiles/lua.dir/requires: CMakeFiles/lua.dir/src/lua.c.o.requires
.PHONY : CMakeFiles/lua.dir/requires

CMakeFiles/lua.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lua.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lua.dir/clean

CMakeFiles/lua.dir/depend:
	cd /home/mka/opt/xscript/lua/luadist/5.1.5/_build/linux64x86 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mka/opt/xscript/lua/luadist/5.1.5 /home/mka/opt/xscript/lua/luadist/5.1.5 /home/mka/opt/xscript/lua/luadist/5.1.5/_build/linux64x86 /home/mka/opt/xscript/lua/luadist/5.1.5/_build/linux64x86 /home/mka/opt/xscript/lua/luadist/5.1.5/_build/linux64x86/CMakeFiles/lua.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lua.dir/depend
