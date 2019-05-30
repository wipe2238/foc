add_subdirectory( data )
add_subdirectory( dialogs )
add_subdirectory( maps )
add_subdirectory( proto )
add_subdirectory( scripts )
add_subdirectory( text )

list( APPEND CONTENT_FILES ${CMAKE_CURRENT_LIST_DIR}/Server.cfg )
set( CONTENT_FILES "${CONTENT_FILES}" PARENT_SCOPE )
