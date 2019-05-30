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

execute_process(
	COMMAND ${GIT_EXECUTABLE} config -f .gitmodules --get submodule.Engine.path
	WORKING_DIRECTORY ${FOC_ROOT_DIR}
	OUTPUT_VARIABLE ENGINE_DIR
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

if( NOT ENGINE_DIR )
	message( FATAL_ERROR "Cannot find git submodule 'Engine' path" )
endif()

#include( ${ENGINE_DIR}/CMake/Extensions.cmake )
#FOClassicExtensionInit( "${CMAKE_CURRENT_LIST_DIR}/${FOC_FOCLASSIC_DIR}" )

#set( FOC_CMAKE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${FOC_FOCLASSIC_DIR}/CMake/include )

message( STATUS "Configuring FOClassic... ${ENGINE_DIR}" )
add_subdirectory( ${ENGINE_DIR} )
message( STATUS "Configuring FOClassic... OK" )

##
## FOCpp
##

#add_subdirectory( Client )
#add_subdirectory( Mapper )
add_subdirectory( Server )

set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set_property( GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake" )

#set_property( TARGET ${FOCLASSIC_ALL_TARGETS} PROPERTY FOLDER Engine )

add_custom_target( Content
	SOURCES
		${CMAKE_CURRENT_LIST_FILE}
		${CONTENT_FILES}
)

source_group( "CMake"  REGULAR_EXPRESSION "[Cc][Mm][Aa][Kk][Ee]" )
source_group( TREE "${SERVER_DIR}" FILES ${CONTENT_FILES} )
