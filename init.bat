@echo off

REM Microsoft Visual C/C++ Environment - Checks/Initializes vcvarsall.bat to make sure we can compile!
REM ###################################################################################################################

:init_msvc
set MSVC_PATH_V2017="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"
if exist %MSVC_PATH_V2017% (
	echo Microsoft Visual Studio 2017 detected
	set MSVC_PATH=%MSVC_PATH_V2017% 
	goto init_msvc2
)

set MSVC_PATH_V14="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
if exist %MSVC_PATH_V14% (
	echo Microsoft Visual Studio 14 detected
	set MSVC_PATH=%MSVC_PATH_V14%
	goto init_msvc2
)

:init_msvc2
if exist %MSVC_PATH% goto vcvarsall
echo Failed to initialize the MSVC environment. Please run vcvarsall.bat yourself. **HINT: The vcvarsall.bat file can be found at your Visual Studio installed location within a directory called VC.
goto end

:vcvarsall
echo Running vcvarsall.bat x64
pushd C:\
call %MSVC_PATH% x64
popd

REM ###################################################################################################################

chcp 65001
echo Codepage set to utf8

subl