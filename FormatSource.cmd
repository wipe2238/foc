@echo off

setlocal

set PATH=C:\Program Files\Git\bin;C:\Program Files (x86)\Git\bin;Cache\cmake\bin;%PATH%
cmake -DO=Build.cmake;Source/Engine/Build.cmake -DOWNLOAD_DIR=Cache -P Source/Engine/CMake/SelfUpdate/SelfUpdate.cmake
cmake -DNO_BUILD=1 -DNO_ZIP=1 -P Source/Engine/Build.cmake
