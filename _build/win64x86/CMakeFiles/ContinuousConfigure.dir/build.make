# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = E:\MinGW\opt\bin\cmake.exe

# The command to remove a file.
RM = E:\MinGW\opt\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\opt\xscript\lua\luadist\5.1.5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\opt\xscript\lua\luadist\5.1.5\_build\win64x86

# Utility rule file for ContinuousConfigure.

# Include the progress variables for this target.
include CMakeFiles\ContinuousConfigure.dir\progress.make

CMakeFiles\ContinuousConfigure:
	e:\Mingw\opt\bin\ctest.exe -D ContinuousConfigure

ContinuousConfigure: CMakeFiles\ContinuousConfigure
ContinuousConfigure: CMakeFiles\ContinuousConfigure.dir\build.make

.PHONY : ContinuousConfigure

# Rule to build all files generated by this target.
CMakeFiles\ContinuousConfigure.dir\build: ContinuousConfigure

.PHONY : CMakeFiles\ContinuousConfigure.dir\build

CMakeFiles\ContinuousConfigure.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\ContinuousConfigure.dir\cmake_clean.cmake
.PHONY : CMakeFiles\ContinuousConfigure.dir\clean

CMakeFiles\ContinuousConfigure.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\opt\xscript\lua\luadist\5.1.5 C:\opt\xscript\lua\luadist\5.1.5 C:\opt\xscript\lua\luadist\5.1.5\_build\win64x86 C:\opt\xscript\lua\luadist\5.1.5\_build\win64x86 C:\opt\xscript\lua\luadist\5.1.5\_build\win64x86\CMakeFiles\ContinuousConfigure.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\ContinuousConfigure.dir\depend
