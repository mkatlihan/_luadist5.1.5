# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = d:\usr\CMake\2.8\bin\cmake.exe

# The command to remove a file.
RM = d:\usr\CMake\2.8\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = d:\usr\CMake\2.8\bin\cmake-gui.exe

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = H:\opt\xscript\lua\luadist\5.1.5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = H:\opt\xscript\lua\luadist\5.1.5\_build\win32x86

# Utility rule file for NightlySubmit.

# Include the progress variables for this target.
include CMakeFiles\NightlySubmit.dir\progress.make

CMakeFiles\NightlySubmit:
	d:\usr\cmake\2.8\bin\ctest.exe -D NightlySubmit

NightlySubmit: CMakeFiles\NightlySubmit
NightlySubmit: CMakeFiles\NightlySubmit.dir\build.make
.PHONY : NightlySubmit

# Rule to build all files generated by this target.
CMakeFiles\NightlySubmit.dir\build: NightlySubmit
.PHONY : CMakeFiles\NightlySubmit.dir\build

CMakeFiles\NightlySubmit.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\NightlySubmit.dir\cmake_clean.cmake
.PHONY : CMakeFiles\NightlySubmit.dir\clean

CMakeFiles\NightlySubmit.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" H:\opt\xscript\lua\luadist\5.1.5 H:\opt\xscript\lua\luadist\5.1.5 H:\opt\xscript\lua\luadist\5.1.5\_build\win32x86 H:\opt\xscript\lua\luadist\5.1.5\_build\win32x86 H:\opt\xscript\lua\luadist\5.1.5\_build\win32x86\CMakeFiles\NightlySubmit.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\NightlySubmit.dir\depend

