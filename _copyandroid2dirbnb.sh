#!/bin/sh

APIVER=$1
BUILD=$2
if [ "$APIVER" = "" ]; then
	APIVER=13.0
fi
if [ "$BUILD" = "" ]; then
	BUILD=1
fi

rsync -av _release/arm/lib/android/local/ ~/etc/api/${APIVER}/${BUILD}/dirbnb/arch/android/lib/ --exclude=objs
