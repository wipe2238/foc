@echo off

setlocal

set PATH=C:\Program Files\Git\bin;C:\Program Files (x86)\Git\bin;Cache\cmake\bin;%PATH%
cmake -DO=Build.cmake;Upstream/FOClassic/Build.cmake -DOWNLOAD_DIR=Cache -P Upstream/FOClassic/CMake/SelfUpdate/SelfUpdate.cmake
cmake -DNO_BUILD=1 -DNO_ZIP=1 -P Upstream/FOClassic/Build.cmake
