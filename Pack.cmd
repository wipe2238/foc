@echo off

setlocal
set PATH=Cache\cmake\bin;%PATH%
cmake -DO=Pack.cmake -DOWNLOAD_DIR=Cache -DISABLE_SUBMODULES=1 -P Upstream/FOClassic/CMake/SelfUpdate/SelfUpdate.cmake
cmake -P Pack.cmake
