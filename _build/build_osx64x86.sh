#!/bin/bash
#source /usr/Intel/Compiler/11.1/060/bin/iccvars.sh intel64
LOGF=_logs/osx64x86.log
rm -rf osx64x86/*  
mv $LOGF $LOGF.bak
cd osx64x86 
echo $(date) Begin build process | tee ../$LOGF
cmake -DXOPT_OUTPUT_DIR=ON -G "Unix Makefiles" ../..  -DCMAKE_OSX_ARCHITECTURES=x86_64 $@ | tee -a ../$LOGF
make | tee -a ../$LOGF
echo $(date) End build process | tee -a ../$LOGF
make install
cd ..
