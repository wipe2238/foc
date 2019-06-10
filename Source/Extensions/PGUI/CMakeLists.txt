add_library( PGUI STATIC )
target_sources( PGUI
	PRIVATE
		${CMAKE_CURRENT_LIST_FILE}

		PGUI.Core.cpp
		PGUI.Core.h
		PGUI.Draw.h
		PGUI.Element.cpp
		PGUI.Element.h
		PGUI.Event.h
		PGUI.Keyboard.cpp
		PGUI.Keyboard.h
		PGUI.Label.cpp
		PGUI.Label.h
		PGUI.Screen.cpp
		PGUI.Screen.h
		PGUI.TextBox.cpp
		PGUI.TextBox.h
)
target_link_libraries( PGUI Shared )

foreach( target IN ITEMS Client Mapper Server )
	add_library( PGUI.${target} )
	target_sources( PGUI.${target}
		PRIVATE
			${CMAKE_CURRENT_LIST_FILE}
			PGUI.Draw.cpp
	)
	target_link_libraries( PGUI.${target} PRIVATE PGUI )
	FOClassicExtension( ${target} PGUI.${target} )
endforeach()

get_property( targets DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" PROPERTY BUILDSYSTEM_TARGETS )
foreach( target IN LISTS targets )
	if( MSVC )
		target_compile_definitions( ${target} PRIVATE WIN32_LEAN_AND_MEAN )
		target_compile_options( ${target} PRIVATE "/MT" )
	endif()
	set_property( TARGET ${target} PROPERTY FOLDER "Extensions/PGUI" )
endforeach()

source_group( " "      REGULAR_EXPRESSION "\.([Cc]|[Cc][Pp][Pp]|[Hh])$" )
source_group( "CMake"  REGULAR_EXPRESSION "[Cc][Mm][Aa][Kk][Ee]" )
source_group( "Engine" REGULAR_EXPRESSION "^${FOCLASSIC_ROOT_DIR}/Source/" )
