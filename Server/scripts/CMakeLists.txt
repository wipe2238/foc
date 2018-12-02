file( GLOB_RECURSE SCRIPT_FILES LIST_DIRECTORIES false CONFIGURE_DEPENDS "*.fos" )
add_custom_target( Scripts SOURCES ${CMAKE_CURRENT_LIST_FILE} ${SCRIPT_FILES} )

source_group( " "      REGULAR_EXPRESSION "\.([Ff][Oo][Ss])$" )
source_group( "CMake"  REGULAR_EXPRESSION "[Cc][Mm][Aa][Kk][Ee]" )
source_group( TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${SCRIPT_FILES} )
