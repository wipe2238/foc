add_library( Utils STATIC "" )

target_sources( Utils
	PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/ExecuteString.h
	PRIVATE
		${CMAKE_CURRENT_LIST_FILE}
		ExecuteString.cpp
)

target_include_directories( Utils PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${FOCLASSIC_HEADERS_DIR} ${FOCLASSIC_HEADERS_DIR}/AngelScript  )
set_property( TARGET Utils APPEND_STRING PROPERTY COMPILE_OPTIONS "/MT" )

source_group( " "      REGULAR_EXPRESSION "\.([Cc]|[Cc][Pp][Pp]|[Hh])$" )
source_group( "CMake"  REGULAR_EXPRESSION "[Cc][Mm][Aa][Kk][Ee]" )
set_property( TARGET Utils PROPERTY FOLDER "Extensions" )
