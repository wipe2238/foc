add_subdirectory( Utils )

set( EXTENSION_PREFIX "FOC." )

set( CLIENT_MAPPER_SERVER_SOURCES
	${CMAKE_CURRENT_LIST_FILE}

	extension.cpp
	extension.h
)

FOClassicExtension( ${EXTENSION_PREFIX}Client CLIENT "${FOCLASSIC_HEADERS_DIR}" )
FOClassicExtension( ${EXTENSION_PREFIX}Mapper MAPPER "${FOCLASSIC_HEADERS_DIR}" )
FOClassicExtension( ${EXTENSION_PREFIX}Server SERVER "${FOCLASSIC_HEADERS_DIR}" )

target_sources( ${EXTENSION_PREFIX}Client
	PRIVATE
		${CLIENT_MAPPER_SERVER_SOURCES}
		${CLIENT_MAPPER_SOURCES}
		${CLIENT_SERVER_SOURCES}

		Client/ReservedFunctions.cpp
		Client/UserInterface.cpp
		Client/UserInterface.h
)

target_sources( ${EXTENSION_PREFIX}Mapper
	PRIVATE
		${CLIENT_MAPPER_SERVER_SOURCES}
		${CLIENT_MAPPER_SOURCES}
		${MAPPER_SERVER_SOURCES}

		Mapper/ReservedFunctions.cpp
)

target_sources( ${EXTENSION_PREFIX}Server
	PRIVATE
		${CLIENT_MAPPER_SERVER_SOURCES}
		${CLIENT_SERVER_SOURCES}
		${MAPPER_SERVER_SOURCES}

		Server/ReservedFunctions.cpp
)

get_property( targets DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" PROPERTY BUILDSYSTEM_TARGETS )
foreach( target IN LISTS targets )

	get_property( extension TARGET ${target} PROPERTY FOCLASSIC_EXTENSION )
	if( NOT extension )
		continue()
	endif()

	PdbSetup( ${target} )

	get_property( engine_targets TARGET ${target} PROPERTY FOCLASSIC_EXTENSION_DEPENDS )
	add_dependencies( ${target} ${engine_targets} )

	target_include_directories( ${target} PRIVATE ${CMAKE_CURRENT_LIST_DIR} )
	target_compile_definitions( ${target} PRIVATE FOCLASSIC_BLEEDING_EDGE )
	target_link_libraries( ${target} PRIVATE Utils )

	set_property( TARGET ${target} APPEND_STRING PROPERTY COMPILE_OPTIONS " /MP" )
	
	set_property( TARGET ${target} PROPERTY FOLDER "Extensions" )
	set_property( TARGET ${target} PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG "${SCRIPTS_DIR}" )
	set_property( TARGET ${target} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE "${SCRIPTS_DIR}" )
endforeach()

source_group( " "         REGULAR_EXPRESSION "\.([Cc]|[Cc][Pp][Pp]|[Hh])$" )
source_group( "CMake"     REGULAR_EXPRESSION "[Cc][Mm][Aa][Kk][Ee]" )
source_group( "Extension" REGULAR_EXPRESSION "^${CMAKE_CURRENT_LIST_DIR}/extension\.([Cc][Pp][Pp]|[Hh])$" )
source_group( "Utils"     REGULAR_EXPRESSION "^${CMAKE_CURRENT_LIST_DIR}/Utils" )

source_group( "Engine"      REGULAR_EXPRESSION "^${FOCLASSIC_HEADERS_DIR}" )
source_group( "AngelScript" REGULAR_EXPRESSION "^${FOCLASSIC_HEADERS_DIR}/AngelScript" )
