#!/bin/bash
#source /usr/Intel/Compiler/11.1/060/bin/iccvars.sh intel64
LOGF=_logs/linux64x86.log
if [ ! -d linux64x86 ]; then
    mkdir linux64x86
fi
rm -rf linux64x86/*  
mv $LOGF $LOGF.bak
cd linux64x86 
echo $(date) Begin build process | tee ../$LOGF
cmake -DXOPT_OUTPUT_DIR=ON -G "Unix Makefiles" ../..  $@ | tee -a ../$LOGF
make | tee -a ../$LOGF
echo $(date) End build process | tee -a ../$LOGF
make install
cd ..