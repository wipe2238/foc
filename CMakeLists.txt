# cmake -T v100 ..

cmake_minimum_required(VERSION 3.12.2)

project( FOC
	VERSION 0.0.1
	LANGUAGES CXX
)

message( STATUS "" )
message( STATUS "${PROJECT_NAME} v${PROJECT_VERSION}" )
message( STATUS "" )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_SKIP_INSTALL_RULES TRUE )
set( CMAKE_VERBOSE_MAKEFILE true )

set( FOC_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}" )

# main directories
set( CLIENT_DIR "${FOC_ROOT_DIR}/Client" )
set( MAPPER_DIR "${FOC_ROOT_DIR}/Mapper" )
set( SERVER_DIR "${FOC_ROOT_DIR}/Server" )

# git directories
set( GIT_DIR       "${FOC_ROOT_DIR}/.git" )
set( GIT_HOOKS_DIR "${GIT_DIR}/hooks" )

##
## FOClassic
##

find_package( Git REQUIRED )

set( FOCLASSIC_FOLDER_PREFIX               "Engine" )
list( APPEND FOCLASSIC_COPY_CLIENT_DIR     "${CLIENT_DIR}" )
list( APPEND FOCLASSIC_COPY_MAPPER_DIR     "${MAPPER_DIR}" )
list( APPEND FOCLASSIC_COPY_SERVER_DIR     "${SERVER_DIR}" )
list( APPEND FOCLASSIC_COPY_ASCOMPILER_DIR "${SERVER_DIR}/scripts" )
set( FOCLASSIC_COPY_CLIENT_DX_NAME FOC.DX.exe )
set( FOCLASSIC_COPY_CLIENT_GL_NAME FOC.GL.exe )

execute_process(
	COMMAND ${GIT_EXECUTABLE} config -f .gitmodules --get submodule.FOClassic.path
	WORKING_DIRECTORY ${FOC_ROOT_DIR}
	OUTPUT_VARIABLE FOC_FOCLASSIC_DIR
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

if( NOT FOC_FOCLASSIC_DIR )
	message( FATAL_ERROR "Cannot find git submodule 'FOClassic' path" )
endif()

message( STATUS "Configuring FOClassic... ${FOC_FOCLASSIC_DIR}" )
add_subdirectory( ${FOC_FOCLASSIC_DIR} )
message( STATUS "Configuring FOClassic... OK" )

##
## FOCpp
##

#add_subdirectory( Client )
#add_subdirectory( Mapper )
add_subdirectory( Server )

set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set_property( GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake" )

set_property( TARGET ${FOCLASSIC_ALL_TARGETS} PROPERTY FOLDER Engine )

add_custom_target( Content
	SOURCES
		${CMAKE_CURRENT_LIST_FILE}
		${CONTENT_FILES}
)

source_group( "CMake"  REGULAR_EXPRESSION "[Cc][Mm][Aa][Kk][Ee]" )
source_group( TREE "${SERVER_DIR}" FILES ${CONTENT_FILES} )
