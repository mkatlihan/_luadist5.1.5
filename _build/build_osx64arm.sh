#!/bin/bash
#source /usr/Intel/Compiler/11.1/060/bin/iccvars.sh intel64
rm -rf osx64arm/*  
mv osx64arm.log osx64arm.log.bak
cd osx64arm 
echo $(date) Begin build process | tee ../osx64arm.log
cmake -DXOPT_OUTPUT_DIR=ON -G "NMake Makefiles" $@ ../../  | tee -a ../osx64arm.log
make | tee -a ../osx64arm.log
echo $(date) End build process | tee -a ../osx64arm.log
cd ..
