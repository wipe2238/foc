set( EXTENSION_PREFIX "FOC." )

set( CLIENT_MAPPER_SERVER_SOURCES
	${CMAKE_CURRENT_LIST_FILE}

	FOC.h
	Shared/Hex2D.cpp
	Shared/Hex2D.h
	Shared/SScriptFunc.h

	AngelScript/Buffer.cpp
	AngelScript/Buffer.h
)

set( CLIENT_SERVER_SOURCES
	Shared/ApCost.cpp
	Shared/ApCost.h
	Shared/CritterMutual.h
	Shared/MoveItem.cpp
	Shared/MoveItem.h
)

add_library( ${EXTENSION_PREFIX}Client STATIC )
add_library( ${EXTENSION_PREFIX}Mapper STATIC )
add_library( ${EXTENSION_PREFIX}Server STATIC )

target_sources( ${EXTENSION_PREFIX}Client
	PRIVATE
		${CLIENT_MAPPER_SERVER_SOURCES}
		${CLIENT_MAPPER_SOURCES}
		${CLIENT_SERVER_SOURCES}

		FOC.Client.cpp
		FOC.Client.h

		Client/Animations.cpp
		Client/Animations.h
		Client/Interface.cpp
		Client/Interface.h

		Client/Screen/Game.cpp
		Client/Screen/Game.h
		Client/Screen/Info.cpp
		Client/Screen/Info.h
		Client/Screen/Login.cpp
		Client/Screen/Login.h
		Client/Screen/MiniMap.cpp
		Client/Screen/MiniMap.h
)

target_sources( ${EXTENSION_PREFIX}Mapper
	PRIVATE
		${CLIENT_MAPPER_SERVER_SOURCES}
		${CLIENT_MAPPER_SOURCES}
		${MAPPER_SERVER_SOURCES}

		FOC.Mapper.cpp
		FOC.Mapper.h
)

target_sources( ${EXTENSION_PREFIX}Server
	PRIVATE
		${CLIENT_MAPPER_SERVER_SOURCES}
		${CLIENT_SERVER_SOURCES}
		${MAPPER_SERVER_SOURCES}

		FOC.Server.cpp
		FOC.Server.h

		Server/Dialogs.cpp
		Server/Dialogs.h
		Server/Lockers.cpp
		Server/Lockers.h
		Server/WorldMap.cpp
		Server/WorldMap.h
)
if( UNIX )
	target_compile_definitions( ${EXTENSION_PREFIX}Server PUBLIC USE_LIBEVENT LIBEVENT_TIMEOUTS_WORKAROUND )
endif()

get_property( targets DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" PROPERTY BUILDSYSTEM_TARGETS )
foreach( target IN ITEMS ${EXTENSION_PREFIX}Client ${EXTENSION_PREFIX}Mapper ${EXTENSION_PREFIX}Server )

	target_include_directories( ${target} PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${FOC_CMAKE_DIR} )
#	target_link_libraries( ${target} PRIVATE Utils )
	
	set_property( TARGET ${target} PROPERTY FOLDER "Extensions" )

endforeach()

FOClassicExtension( CLIENT ${EXTENSION_PREFIX}Client )
FOClassicExtension( MAPPER ${EXTENSION_PREFIX}Mapper )
FOClassicExtension( SERVER ${EXTENSION_PREFIX}Server )

PdbSetup( ${EXTENSION_PREFIX}Client )
PdbSetup( ${EXTENSION_PREFIX}Mapper )
PdbSetup( ${EXTENSION_PREFIX}Server )

add_subdirectory( PGUI )

source_group( " "         REGULAR_EXPRESSION "\.([Cc]|[Cc][Pp][Pp]|[Hh])$" )
source_group( "CMake"     REGULAR_EXPRESSION "[Cc][Mm][Aa][Kk][Ee]" )
source_group( "Extension" REGULAR_EXPRESSION "^${CMAKE_CURRENT_LIST_DIR}/FOC\." )

source_group( "AngelScript" REGULAR_EXPRESSION "^${FOCLASSIC_HEADERS_DIR}/AngelScript" )
source_group( "AngelScript" REGULAR_EXPRESSION "^${CMAKE_CURRENT_LIST_DIR}/AngelScript" )
source_group( "Shared"      REGULAR_EXPRESSION "^${CMAKE_CURRENT_LIST_DIR}/Shared/" )

source_group( "Screen" REGULAR_EXPRESSION "^${CMAKE_CURRENT_LIST_DIR}/Client/Screen/" )
