cmake_minimum_required( VERSION 3.12 FATAL_ERROR )

set( dir "${CMAKE_CURRENT_LIST_DIR}" )
string( TIMESTAMP ts "%Y-%m-%d %H:%M:%S" )

file( GLOB Base LIST_DIRECTORIES false RELATIVE ${dir} */*.exe */*.pdb )
list( APPEND files ${Base} )

file( GLOB_RECURSE ClientData LIST_DIRECTORIES false RELATIVE ${dir} Client/data/* ) 
list( APPEND files ${ClientData} Client/DataFiles.cfg Client/FOnline.cfg )

file( GLOB_RECURSE MapperData LIST_DIRECTORIES false RELATIVE ${dir} Mapper/data/* ) 
list( APPEND files ${MapperData} Mapper/Mapper.cfg Mapper/tabs.cfg )

file( GLOB_RECURSE ServerData       LIST_DIRECTORIES false RELATIVE ${dir} Server/data/*.cfg ) 
file( GLOB_RECURSE ServerDialogs    LIST_DIRECTORIES false RELATIVE ${dir} Server/dialogs/*.fodlg ) 
file( GLOB_RECURSE ServerMaps       LIST_DIRECTORIES false RELATIVE ${dir} Server/maps/*.fomap ) 
file( GLOB_RECURSE ServerProto      LIST_DIRECTORIES false RELATIVE ${dir} Server/proto/*/*.fopro ) 
file( GLOB_RECURSE ServerScripts    LIST_DIRECTORIES false RELATIVE ${dir} Server/scripts/*.exe Server/scripts/*.pdb fos Server/scripts/*.fos Server/scripts/*.fosb )
file( GLOB_RECURSE ServerText       LIST_DIRECTORIES false RELATIVE ${dir} Server/text/*/*.MSG )
list( APPEND files
	${ServerData}
	${ServerDialogs} Server/dialogs/dialogs.cfg
	${ServerMaps}    Server/maps/GenerateWorld.cfg Server/maps/Locations.cfg
	${ServerProto}   Server/proto/critters/critters.lst Server/proto/items/items.lst
	${ServerScripts} Server/scripts/scripts.cfg
	${ServerText}
	Server/Server.cfg
)

foreach( file IN LISTS ClientData )
	if( "${file}" MATCHES "^Client/data/.+\.dat$" )
		list( REMOVE_ITEM files "${file}" )
	endif()
	if( "${file}" MATCHES "^Client/data/.+\.zip$" )
		list( REMOVE_ITEM files "${file}" )
	endif()
	if( "${file}" MATCHES "^Client/data/cache/" )
		list( REMOVE_ITEM files "${file}" )
	endif()
endforeach()

foreach( file IN LISTS files )
	message( STATUS "${file}" )
endforeach()

execute_process(
	COMMAND ${CMAKE_COMMAND} -E tar cfv FOC.zip --format=zip --mtime=${ts} ${files}
	WORKING_DIRECTORY ${dir}
	OUTPUT_QUIET
)
