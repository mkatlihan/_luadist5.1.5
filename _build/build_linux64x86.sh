#!/bin/bash
#source /usr/Intel/Compiler/11.1/060/bin/iccvars.sh intel64
rm -rf linux64x86/*  
mv linux64x86.log linux64x86.log.bak
cd linux64x86 
echo $(date) Begin build process | tee ../linux64x86.log
cmake -DXOPT_OUTPUT_DIR=ON -G "NMake Makefiles" $@ ../../  | tee -a ../linux64x86.log
make | tee -a ../linux64x86.log
echo $(date) End build process | tee -a ../linux64x86.log
cd ..
