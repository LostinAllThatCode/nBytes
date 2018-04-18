@echo off

set defines=
set extlibs="../../src"
set compilerflags=-nologo -Od -MTd -EHa- -Z7 -FC -I%extlibs% -WX
REM set compilerflags=-nologo -O2 -MT -EHa- -FC -I%extlibs% -WX
set linkerflags=-incremental:no -opt:ref 
REM -nodefaultlib 

mkdir build\win 2> NUL
pushd build\win

del *.pdb 2> NUL

cl %defines% %compilerflags% ..\..\simple.c /link %linkerflags%
if %errorlevel% GTR 0 goto error

goto end

:error
echo build returned with exit code: %errorlevel%
goto _exit

:end
echo build succeeded

:_exit
popd