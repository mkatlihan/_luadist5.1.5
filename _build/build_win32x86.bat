rem call "D:\usr\Intel\Compiler\11.1\067\bin\iclvars.bat" ia32
if not exist win32x86 mkdir win32x86
rm -rf win32x86/*  
mv x86.log x86.log.bak
cd win32x86 
echo %date% Begin build process | tee ../x86.log
cmake.exe -DXOPT_OUTPUT_DIR=ON -G "NMake Makefiles" %* ..\..  | tee -a ../x86.log
nmake | tee -a ../x86.log
echo %date% End build process | tee -a ../x86.log
name install | tee -a ../x86.log
cd .. 
