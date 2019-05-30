set( EXTENSION_PREFIX "FOC." )
set( EXTENSION_DIR    "${SERVER_DIR}/scripts" )

set( CLIENT_MAPPER_SERVER_SOURCES
	${CMAKE_CURRENT_LIST_FILE}

	FOC.h
	Shared/SScriptFunc.h

	AngelScript/Buffer.cpp
	AngelScript/Buffer.h
)

set( CLIENT_SERVER_SOURCES
	Shared/ApCost.cpp
	Shared/ApCost.h
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

get_property( targets DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" PROPERTY BUILDSYSTEM_TARGETS )
foreach( target IN ITEMS ${EXTENSION_PREFIX}Client ${EXTENSION_PREFIX}Mapper ${EXTENSION_PREFIX}Server )

	target_include_directories( ${target} PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${FOC_CMAKE_DIR} )
#	target_link_libraries( ${target} PRIVATE Utils )
	
	set_property( TARGET ${target} PROPERTY FOLDER "Extensions" )

endforeach()

FOClassicExtension( CLIENT ${EXTENSION_PREFIX}Client )
FOClassicExtension( MAPPER ${EXTENSION_PREFIX}Mapper )
FOClassicExtension( SERVER ${EXTENSION_PREFIX}Server )


source_group( " "         REGULAR_EXPRESSION "\.([Cc]|[Cc][Pp][Pp]|[Hh])$" )
source_group( "CMake"     REGULAR_EXPRESSION "[Cc][Mm][Aa][Kk][Ee]" )
source_group( "Extension" REGULAR_EXPRESSION "^${CMAKE_CURRENT_LIST_DIR}/FOC\..+" )

source_group( "AngelScript" REGULAR_EXPRESSION "^${FOCLASSIC_HEADERS_DIR}/AngelScript" )
source_group( "AngelScript" REGULAR_EXPRESSION "^${CMAKE_CURRENT_LIST_DIR}/AngelScript" )
source_group( "Shared"      REGULAR_EXPRESSION "^${CMAKE_CURRENT_LIST_DIR}/Shared/.+" )
