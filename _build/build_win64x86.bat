rem call "D:\usr\Intel\Compiler\11.1\060\bin\iclvars.bat" intel64
if not exist win64x86 mkdir win64x86
rm -rf win64x86/*  
mv x64.log x64.log.bak
cd win64x86 
echo %date% Begin build process | tee ../x64.log
cmake.exe -DXOPT_OUTPUT_DIR=ON -G "NMake Makefiles" %* ..\..  | tee -a ../x64.log
nmake | tee -a ../x64.log
echo %date% End build process | tee -a ../x64.log
nmake install | tee -a ../x64.log
cd .. 
