#!/bin/bash

if [ "$1" = "" ]; then
	echo
	echo "   usage   : _copy2dirbnb.sh gen|force"
	echo
	echo "   example :"
	echo "   $ _copy2dirbnb.sh gen      generate the commands for 'force' blocks "
	echo "   $ _copy2dirbnb.sh force    run the copy block"
fi

APIVER=13.0
DIRBNB=~/etc/api/$APIVER/1/dirbnb

## generate commands for 'copy' block
if [ "$1" = "gen" ]; then
	for d in win32x86 win64x86 linux64x86 osx64x86; do	
		echo cp -v lib/$d/*liblua_static* $DIRBNB/xxx/arch/lib
	done
	echo "Note: copy output into other if-block replacing xxx with arch"
fi

## copy block
if [ "$1" = "force" ]; then
	cp -v lib/win32x86/liblua_static.lib    /home/mka/etc/api/13.0/1/dirbnb/arch/win/lib/liblua_static.lib
	cp -v lib/win64x86/liblua_static.lib    /home/mka/etc/api/13.0/1/dirbnb/arch/win64amd/lib/liblua_static.lib
	cp -v lib/linux64x86/libliblua_static.a /home/mka/etc/api/13.0/1/dirbnb/arch/linux64amd/lib/liblua_static.a
	cp -v lib/osx64x86/libliblua_static.a   /home/mka/etc/api/13.0/1/dirbnb/arch/imac64/lib/liblua_static.a
	if [ ! -d ${DIRBNB}/lua ]; then 
		mkdir -v ${DIRBNB}/lua
	fi	
	cp -v src/lua.h      $DIRBNB/lua
	cp -v src/lauxlib.h  $DIRBNB/lua
	cp -v src/luaconf.h  $DIRBNB/lua
	cp -v src/lualib.h   $DIRBNB/lua
fi
