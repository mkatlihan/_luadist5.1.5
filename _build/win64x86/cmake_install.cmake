# Install script for directory: C:/opt/xscript/lua/luadist/5.1.5

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "release")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Runtime" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/opt/xscript/lua/luadist/5.1.5/bin/win64x86/wlua.exe")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Runtime" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/opt/xscript/lua/luadist/5.1.5/bin/win64x86/lua.exe")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Runtime" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/opt/xscript/lua/luadist/5.1.5/bin/win64x86/luac.exe")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Library" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/opt/xscript/lua/luadist/5.1.5/lib/win64x86/liblua.lib")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Runtime" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/opt/xscript/lua/luadist/5.1.5/bin/win64x86/liblua.dll")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Data" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/lua" TYPE FILE FILES "C:/opt/xscript/lua/luadist/5.1.5/README.md")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Data" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/lua" TYPE FILE FILES "C:/opt/xscript/lua/luadist/5.1.5/COPYRIGHT")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Data" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/lua" TYPE FILE FILES "C:/opt/xscript/lua/luadist/5.1.5/HISTORY")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Runtime" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/lua" TYPE FILE RENAME "strict.lua" FILES "C:/opt/xscript/lua/luadist/5.1.5/etc/strict.lua")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Header" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/opt/xscript/lua/luadist/5.1.5/src/lua.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Header" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/opt/xscript/lua/luadist/5.1.5/src/lualib.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Header" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/opt/xscript/lua/luadist/5.1.5/src/lauxlib.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Header" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/opt/xscript/lua/luadist/5.1.5/etc/lua.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Header" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/opt/xscript/lua/luadist/5.1.5/_build/win64x86/luaconf.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Documentation" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/lua/doc/" TYPE DIRECTORY FILES "C:/opt/xscript/lua/luadist/5.1.5/doc/")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Other" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/lua/etc/" TYPE DIRECTORY FILES "C:/opt/xscript/lua/luadist/5.1.5/etc/")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Test" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/lua/test/" TYPE DIRECTORY FILES "C:/opt/xscript/lua/luadist/5.1.5/test/")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/opt/xscript/lua/luadist/5.1.5/_build/win64x86/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
