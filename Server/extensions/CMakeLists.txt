add_subdirectory( Utils )

set( EXTENSION_PREFIX "FOC." )
set( EXTENSION_DIR    "${SERVER_DIR}/scripts" )

set( CLIENT_MAPPER_SERVER_SOURCES
	${CMAKE_CURRENT_LIST_FILE}

	${FOCLASSIC_HEADERS_DIR}/AngelScript/angelscript.h

	extension.cpp
	extension.h

	AngelScript/Buffer.cpp
	AngelScript/Buffer.h
)

FOClassicExtension( ${EXTENSION_PREFIX}Client CLIENT "${FOCLASSIC_HEADERS_DIR}" )
FOClassicExtension( ${EXTENSION_PREFIX}Mapper MAPPER "${FOCLASSIC_HEADERS_DIR}" )
FOClassicExtension( ${EXTENSION_PREFIX}Server SERVER "${FOCLASSIC_HEADERS_DIR}" )

target_sources( ${EXTENSION_PREFIX}Client
	PRIVATE
		${CLIENT_MAPPER_SERVER_SOURCES}
		${CLIENT_MAPPER_SOURCES}
		${CLIENT_SERVER_SOURCES}

		Client/Animations.cpp
		Client/Animations.h
		Client/Interface.cpp
		Client/Interface.h
		Client/ReservedFunctions.cpp
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

		Server/Dialogs.cpp
		Server/Dialogs.h
		Server/Lockers.cpp
		Server/Lockers.h
		Server/ReservedFunctions.cpp
		Server/WorldMap.cpp
		Server/WorldMap.h
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
	foreach( property IN ITEMS LIBRARY_OUTPUT_DIRECTORY LIBRARY_OUTPUT_DIRECTORY_RELEASE RUNTIME_OUTPUT_DIRECTORY RUNTIME_OUTPUT_DIRECTORY_RELEASE )
		set_property( TARGET ${target} PROPERTY ${property} "${EXTENSION_DIR}" )
	endforeach()

	if( MSVC )
		set_property( TARGET ${target} PROPERTY PDB_OUTPUT_DIRECTORY         "${EXTENSION_DIR}" )
		set_property( TARGET ${target} PROPERTY PDB_OUTPUT_DIRECTORY_RELEASE "${EXTENSION_DIR}" )
	endif()

endforeach()

source_group( " "         REGULAR_EXPRESSION "\.([Cc]|[Cc][Pp][Pp]|[Hh])$" )
source_group( "CMake"     REGULAR_EXPRESSION "[Cc][Mm][Aa][Kk][Ee]" )
source_group( "Extension" REGULAR_EXPRESSION "^${CMAKE_CURRENT_LIST_DIR}/extension\.([Cc][Pp][Pp]|[Hh])$" )
source_group( "Utils"     REGULAR_EXPRESSION "^${CMAKE_CURRENT_LIST_DIR}/Utils" )

source_group( "Engine"      REGULAR_EXPRESSION "^${FOCLASSIC_HEADERS_DIR}" )
source_group( "AngelScript" REGULAR_EXPRESSION "^${FOCLASSIC_HEADERS_DIR}/AngelScript" )
source_group( "AngelScript" REGULAR_EXPRESSION "^${CMAKE_CURRENT_LIST_DIR}/AngelScript" )
