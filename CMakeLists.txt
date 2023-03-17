# Copyright (C) 2007-2012 LuaDist.
# Created by Peter Drahoš, Peter Kapec
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
# Please note that the package source code is licensed under its own license.

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

project ( lua C )
cmake_minimum_required ( VERSION 2.8 )
include ( cmake/dist.cmake ) 
include ( lua )

if(NOT DEFINED XBITS)
    MESSAGE(STATUS "Bit size not defined")   
    if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        MESSAGE(STATUS "64 bits compiler detected" )   
        SET (XBITS "64")
    else( CMAKE_SIZEOF_VOID_P EQUAL 8 ) 
        MESSAGE(STATUS "32 bits compiler detected" )    
        SET (XBITS "32")
    endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )    
endif()

SET(XPLATFORM_SUFFIX "x86" )
IF(${CMAKE_SYSTEM} MATCHES "v7")
    SET(XPLATFORM_SUFFIX "arm" )
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Linux specific code
    SET(XOSYS "linux")
	add_definitions(-DLUA_USE_LINUX)	
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   # Mac OS X specific code
   SET(XOSYS "osx")
   add_definitions(-DLUA_USE_MACOSX)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(CYGWIN)
   SET(XOSYS "cyg")   
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
   SET(XOSYS "win")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

if (UNIX)
    if (NOT XPLATFORM_SUFFIX STREQUAL "arm")
        set(MXX "-m${XBITS}")  #add -m32 or -m64 for non-arm
    endif()
    if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64")
        set(MXX "")
    endif()
    if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm64")
        set(MXX "")
    endif()   
    set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} ${MXX} -fPIC")
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${MXX} -lm -fPIC")      
    if (XBITS STREQUAL "32")    
		SET(CMAKE_LIBRARY_PATH      "${CMAKE_LIBRARY_PATH} -L/usr/lib")
    else()
        add_definitions(-D__64BIT__)
    endif() 	
elseif(WIN32)
     set(CMAKE_C_FLAGS_RELEASE "/MT")
     set(CMAKE_C_FLAGS_DEBUG   "/MTd")   
     add_definitions(-DWIN32 -D_WINDOWS)
     add_definitions(-DWIN32 -D_CRT_SECURE_NO_WARNINGS)
     if (XBITS STREQUAL "64")
         add_definitions(-D__64BIT__)
     endif()
endif()

MESSAGE("CFLAGS : ${CMAKE_C_FLAGS}") 
MESSAGE("LFLAGS : ${CMAKE_EXE_LINKER_FLAGS}") 

SET(XPLATFORM "${XOSYS}${XBITS}${XPLATFORM_SUFFIX}")
#MESSAGE( "Operating System :" " ${XOSYS}" )
MESSAGE( "System Version  :" " ${CMAKE_SYSTEM}" )
MESSAGE( "System Name     :" " ${CMAKE_SYSTEM_NAME}" )
MESSAGE( "Platform        :" " ${XPLATFORM}" )
MESSAGE( "Build Type      :" " ${CMAKE_BUILD_TYPE}" )

MESSAGE( "Compiler        :" " ${CMAKE_C_COMPILER_ID}")
MESSAGE( "Compiler Version:" " ${CMAKE_C_COMPILER_VERSION}")
set(XOPT_COMPILER "${CMAKE_C_COMPILER_ID}_${CMAKE_C_COMPILER_VERSION}")
MESSAGE( "Compiler tag    :" " ${XOPT_COMPILER}")
#set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/_release/${XPLATFORM} )
set(CMAKE_INSTALL_PREFIX "/usr/local" )

## CONFIGURATION
# Default configuration (we assume POSIX by default)
set ( LUA_PATH "LUA_PATH" CACHE STRING "Environment variable to use as package.path." )
set ( LUA_CPATH "LUA_CPATH" CACHE STRING "Environment variable to use as package.cpath." )
set ( LUA_INIT "LUA_INIT" CACHE STRING "Environment variable for initial script." )

option ( LUA_ANSI "Use only ansi features." OFF )
option ( LUA_USE_RELATIVE_LOADLIB "Use modified loadlib.c with support for relative paths on posix systems." 
  ON )
set ( LUA_IDSIZE 60 CACHE NUMBER "gives the maximum size for the description of the source." )
set ( LUA_PROMPT "> " CACHE STRING "Is the default prompt used by stand-alone Lua." )
set ( LUA_PROMPT2 ">> " CACHE STRING "Is the default continuation prompt used by stand-alone Lua." )
set ( LUA_MAXINPUT 512 CACHE NUMBER "Is the maximum length for an input line in the stand-alone interpreter." )

#2DO: LUAI_* and LUAL_* settings, for now defaults are used.
set ( LUA_DIRSEP "/" )
set ( LUA_MODULE_SUFFIX ${CMAKE_SHARED_MODULE_SUFFIX} )
set ( LUA_LDIR ${INSTALL_LMOD} )
set ( LUA_CDIR ${INSTALL_CMOD} )

if ( LUA_USE_RELATIVE_LOADLIB )
  # This will set up relative paths to lib
  string ( REGEX REPLACE "[^!/]+" ".." LUA_DIR "!/${INSTALL_BIN}/" )
else ( )
  # Direct path to installation
  set ( LUA_DIR ${CMAKE_INSTALL_PREFIX} CACHE STRING "Destination from which modules will be resolved. See INSTALL_LMOD and INSTALL_CMOD." )
endif ( )

set ( LUA_PATH_DEFAULT "./?.lua;${LUA_DIR}${LUA_LDIR}/?.lua;${LUA_DIR}${LUA_LDIR}/?/init.lua;./?/init.lua" )
set ( LUA_CPATH_DEFAULT "./?${LUA_MODULE_SUFFIX};${LUA_DIR}${LUA_CDIR}/?${LUA_MODULE_SUFFIX};${LUA_DIR}${LUA_CDIR}/loadall${LUA_MODULE_SUFFIX}" )

if ( WIN32 AND NOT CYGWIN )
  # Windows systems
  option ( LUA_WIN "Windows specific build." ON )
  option ( LUA_BUILD_WLUA "Build wLua interpretter without console output." ON )
  option ( LUA_BUILD_AS_DLL "Build Lua library as Dll." ${BUILD_SHARED_LIBS} )

  # Paths (Double escapes needed)
  set ( LUA_DIRSEP "\\\\" ) 
  string ( REPLACE " /" ${LUA_DIRSEP} LUA_DIR "${LUA_DIR}" )
  string ( REPLACE "/" ${LUA_DIRSEP} LUA_LDIR "${LUA_LDIR}" )
  string ( REPLACE "/" ${LUA_DIRSEP} LUA_CDIR "${LUA_CDIR}" )
  string ( REPLACE "/" ${LUA_DIRSEP} LUA_PATH_DEFAULT "${LUA_PATH_DEFAULT}" )
  string ( REPLACE "/" ${LUA_DIRSEP} LUA_CPATH_DEFAULT "${LUA_CPATH_DEFAULT}" )
else ( )
  option ( LUA_USE_POSIX "Use POSIX functionality." ON )
  option ( LUA_USE_DLOPEN "Use dynamic linker to load modules." ON )
  option ( LUA_USE_MKSTEMP "Use mkstep." ON )
  option ( LUA_USE_ISATTY "Use tty." ON )
  option ( LUA_USE_POPEN "Use popen." ON )
  option ( LUA_USE_ULONGJMP "Use ulongjmp" ON )
  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	option ( LUA_USE_MACOSX "Use macosx." ON )
	option ( LUA_USE_READLINE "Use readline." ON )		
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")  
endif ( )

#Additional customizations
option( XOPT_LIBLUA "Use non-standard library name liblua." OFF)
option( XOPT_COPY2TABOX "Copy shared library to TABOX binary folder." OFF)
option( XOPT_OUTPUT_DIR "Change output directory." OFF)

## SETUP
# Optional libraries
find_package ( readline )
if ( READLINE_FOUND )
  option ( LUA_USE_READLINE "Use readline in the Lua CLI." ON )
endif ( )

#find_package ( Curses )
if ( CURSES_FOUND )
  option ( LUA_USE_CURSES "Use curses in the Lua CLI." ON )
endif ( )

# Setup needed variables and libraries
if ( LUA_USE_POSIX )
  # On POSIX Lua links to standard math library "m"
	find_library ( MATH_LIBRARY NAMES m )
	if ( MATH_LIBRARY )
		list ( APPEND LIBS ${MATH_LIBRARY} )
	endif ( )
endif ( )

if ( LUA_USE_DLOPEN )
  # Link to dynamic linker library "dl"
	find_library ( DL_LIBRARY NAMES dl )
	if ( DL_LIBRARY )
		list ( APPEND LIBS ${DL_LIBRARY} )
	endif ( )
endif ( )

if ( LUA_WIN )
  # Add extra rc files to the windows build
  if ( MSVC OR MINGW )
    set ( LUA_DEF src/lua.def )
    set ( LUA_DLL_RC src/lua_dll.rc )
    set ( LUA_RC src/lua.rc )
    set ( LUAC_RC src/luac.rc )
  endif ( )
endif ( )

if ( LUA_USE_READLINE )
  # Add readline
  include_directories ( ${READLINE_INCLUDE_DIR} )
  list ( APPEND LIBS ${READLINE_LIBRARY} )
endif ( )

if ( LUA_USE_CURSES )
  # Add curses
  include_directories ( ${CURSES_INCLUDE_DIR} )
  list ( APPEND LIBS ${CURSES_LIBRARY} )
endif ( )

## SOURCES
# Generate luaconf.h
configure_file ( src/luaconf.h.in ${CMAKE_CURRENT_BINARY_DIR}/luaconf.h )

if (XOPT_OUTPUT_DIR)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${XPLATFORM}/${XOPT_COMPILER})
	if (UNIX)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${XPLATFORM}/${XOPT_COMPILER})
	else()
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${XPLATFORM}/${XOPT_COMPILER})
	endif()	
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${XPLATFORM}/${XOPT_COMPILER})
endif()

# Sources and headers
include_directories ( src ${CMAKE_CURRENT_BINARY_DIR} )
set ( SRC_LIB src/lapi.c src/lcode.c src/ldebug.c src/ldo.c src/ldump.c src/lfunc.c 
  src/lgc.c src/llex.c src/lmem.c src/lobject.c src/lopcodes.c src/lparser.c src/lstate.c 
  src/lstring.c src/ltable.c src/ltm.c src/lundump.c src/lvm.c src/lzio.c src/lauxlib.c 
  src/lbaselib.c src/ldblib.c src/liolib.c src/lmathlib.c src/loslib.c src/ltablib.c 
  src/lstrlib.c src/linit.c )
set ( SRC_LUA src/lua.c )
set ( SRC_LUAC src/luac.c src/print.c )

if ( LUA_USE_RELATIVE_LOADLIB )
  # Use modified loadlib
  list ( APPEND SRC_LIB src/loadlib_rel.c )
else ( )
  list ( APPEND SRC_LIB src/loadlib.c )
endif ( )

## BUILD
# Create dynamic library
add_library ( liblua SHARED ${SRC_LIB} ${LUA_DLL_RC} ${LUA_DEF} )
target_link_libraries ( liblua ${LIBS} )
if (LUA_WIN)
	if (XOPT_LIBLUA)
		set_target_properties ( liblua PROPERTIES OUTPUT_NAME liblua CLEAN_DIRECT_OUTPUT 1 )
	else()
		set_target_properties ( liblua PROPERTIES OUTPUT_NAME lua51 CLEAN_DIRECT_OUTPUT 1 )		
	endif()
else()
	set_target_properties ( liblua PROPERTIES OUTPUT_NAME lua CLEAN_DIRECT_OUTPUT 1 )
endif()
if ( LUA_BUILD_AS_DLL )
  set_target_properties ( liblua PROPERTIES COMPILE_DEFINITIONS LUA_BUILD_AS_DLL )
endif ( )

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")	
	set_property(TARGET liblua APPEND_STRING PROPERTY LINK_FLAGS " -Wl,-rpath-link,'$ORIGIN' -Wl,-rpath,'$ORIGIN'")
	#set_property(TARGET ${METAXESS_NAME} APPEND_STRING PROPERTY LINK_FLAGS " -ldl -static-intel")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set_target_properties(liblua PROPERTIES BUILD_WITH_INSTALL_RPATH 1 INSTALL_NAME_DIR "@rpath")
	set(CMAKE_MACOSX_RPATH 1)
	set_property(TARGET liblua APPEND_STRING PROPERTY LINK_FLAGS " -single_module -framework IOkit -framework CoreFoundation -Xlinker -bind_at_load -dynamiclib" )
ENDIF()


if (XOPT_COPY2TABOX)
	if (UNIX)
	    SET(ENV{TABOX_HOME}     "$ENV{HOME}/dev/c/tabox/trunk")	    
	endif()	
	get_property(TARGET_LOCATION TARGET liblua PROPERTY LOCATION)
	ADD_CUSTOM_COMMAND(TARGET liblua POST_BUILD
	    COMMAND cp -puv "${TARGET_LOCATION}" "$ENV{TABOX_HOME}/bin/${XPLATFORM}"
	)
endif()

# Create static library, this is needed to compile luac in the 5.1.x Lua series
add_library ( lua_static STATIC ${SRC_LIB} )
target_link_libraries ( lua_static ${LIBS} )

add_executable ( lua ${SRC_LUA} ${LUA_RC} )
target_link_libraries ( lua liblua )

# On windows a variant of the lua interpreter without console output needs to be built
if ( LUA_BUILD_WLUA )
  add_executable ( wlua WIN32 src/wmain.c ${SRC_LUA} ${LUA_RC} )
  target_link_libraries ( wlua liblua )
  install_executable ( wlua )
endif ( )

add_executable ( luac ${SRC_LUAC} ${LUAC_RC} )
target_link_libraries ( luac lua_static )

install_executable ( lua luac )
install_library ( liblua )
install_data ( README.md COPYRIGHT HISTORY )
install_lua_module ( strict etc/strict.lua )
install_header ( src/lua.h src/lualib.h src/lauxlib.h etc/lua.hpp ${CMAKE_CURRENT_BINARY_DIR}/luaconf.h )
#install_doc ( doc/ )
#install_foo ( etc/ )
#install_test ( test/ )

## TESTS
set ( LUA lua )

add_lua_test ( test/bisect.lua )
add_lua_test ( test/cf.lua )
add_lua_test ( test/echo.lua )
add_lua_test ( test/env.lua )
add_lua_test ( test/factorial.lua )
add_lua_test ( test/fib.lua 20 )
add_lua_test ( test/fibfor.lua )
#add_lua_test ( test/globals.lua ) # Requires input
add_lua_test ( test/hello.lua )
file ( READ test/life.lua _data )
# life.lua test, with reduced run-time.
string ( REPLACE "40,20" "20,15" _data "${_data}" )
string ( REPLACE 2000 20 _data "${_data}" )
file ( WRITE ${CMAKE_CURRENT_BINARY_DIR}/test/life-quick.lua "${_data}" )
add_lua_test ( ${CMAKE_CURRENT_BINARY_DIR}/test/life-quick.lua )
#add_lua_test ( test/luac.lua ) # Requires input
add_lua_test ( test/printf.lua )
#add_lua_test ( test/readonly.lua )
#set_property ( TEST readonly PROPERTY PASS_REGULAR_EXPRESSION "cannot redefine global variable `y'" )
add_lua_test ( test/sieve.lua )
add_lua_test ( test/sort.lua )
#add_lua_test ( test/table.lua ) # Requires input
add_lua_test ( test/trace-calls.lua )
add_lua_test ( test/trace-globals.lua )
#add_lua_test ( test/xd.lua ) # Requires input
