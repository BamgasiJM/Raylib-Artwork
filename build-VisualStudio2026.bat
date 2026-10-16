@echo off
pushd "%~dp0"
tools\premake\premake5.exe --file=tools/premake/premake5.lua vs2026
set "buildResult=%errorlevel%"
popd
exit /b %buildResult%
