#!/bin/bash
#source /usr/Intel/Compiler/11.1/060/bin/iccvars.sh intel64
rm -rf osx64x86/*  
mv osx64x86.log osx64x86.log.bak
cd osx64x86 
echo $(date) Begin build process | tee ../osx64x86.log
cmake -DXOPT_OUTPUT_DIR=ON -G "NMake Makefiles" $@ ../../  | tee -a ../osx64x86.log
make | tee -a ../osx64x86.log
echo $(date) End build process | tee -a ../osx64x86.log
cd ..
