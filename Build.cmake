cmake_minimum_required( VERSION 3.12 FATAL_ERROR )

set( BUILD_DIR "${CMAKE_CURRENT_LIST_DIR}/Build" )

find_package( Git REQUIRED )

if( UNIX )
	set( BUILD_FILE      "Makefile" )
	set( BUILD_GENERATOR "Unix Makefiles" )
	set( BUILD_TOOLCHAIN -DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_LIST_DIR}/Upstream/FOClassic/CMake/Toolchain/Linux32.cmake )
elseif( WIN32 )
	set( BUILD_FILE      "FOC.sln" )
	set( BUILD_GENERATOR "Visual Studio 15 2017" )
	set( BUILD_TOOLSET   -T v100 )
endif()

if( NOT EXISTS "${BUILD_DIR}" )
	message( STATUS "Creating build directory (${BUILD_DIR})" )
	file( MAKE_DIRECTORY "${BUILD_DIR}" )
endif()

if( NOT EXISTS "${BUILD_DIR}/${BUILD_FILE}" )
	message( STATUS "Starting generator (${BUILD_GENERATOR})" )
	execute_process(
		COMMAND ${CMAKE_COMMAND} ${BUILD_TOOLCHAIN} -G "${BUILD_GENERATOR}" ${BUILD_TOOLSET} "${CMAKE_CURRENT_LIST_DIR}"
		WORKING_DIRECTORY Build
	)
endif()

execute_process(
	COMMAND ${CMAKE_COMMAND} --build "${BUILD_DIR}" --config Release
	RESULT_VARIABLE result
)
