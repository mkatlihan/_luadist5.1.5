#!/bin/bash
#source /usr/Intel/Compiler/11.1/060/bin/iccvars.sh intel64
LOGF=_logs/osx64arm.log
rm -rf osx64arm/*  
mv $LOGF $LOGF.bak
cd osx64arm 
echo $(date) Begin build process | tee ../$LOGF
cmake -DXOPT_OUTPUT_DIR=ON -G "Unix Makefiles" ../..   -DCMAKE_OSX_ARCHITECTURES=arm64 $@ | tee -a ../$LOGF
make | tee -a ../$LOGF
echo $(date) End build process | tee -a ../$LOGF
make install
cd ..
