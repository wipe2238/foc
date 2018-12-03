#include <extension.h>

#include "WorldMap.h"

#define Scenery MapObject

EXPORT void init()
{
	// WorldMap
	WorldMap = new WM( GAME_OPTION(GlobalMapWidth), GAME_OPTION(GlobalMapHeight), GAME_OPTION(GlobalMapZoneLength) );

	WorldMap->Debug = true;
	WorldMap->BaseSpeed = 10.0f;
}

EXPORT bool start()
{
	return true;
}

EXPORT void get_start_time( uint16&, uint16&, uint16&, uint16&, uint16&, uint16& )
{
}

EXPORT void finish()
{
	delete WorldMap;
}

EXPORT uint loop()
{
	return 10;
}

EXPORT void global_process( int processType, Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer )
{
	WorldMap->Process(processType, cr, car, curX, curY, toX, toY, speed, encounterDescriptor, waitForAnswer);
}

EXPORT void global_invite( Critter& leader, Item* car, uint encounterDescriptor, int combatMode, uint& mapId, uint16& hexX, uint16& hexY, uint8& dir )
{
	WorldMap->ProcessInvite(leader, car, encounterDescriptor, combatMode, mapId, hexX, hexY, dir);
}

EXPORT void critter_attack(Critter&, Critter&, ProtoItem&, uint8, ProtoItem*)
{

}

EXPORT void critter_attacked(Critter& attacker, Critter& victim)
{
}

EXPORT bool critter_stealing(Critter&, Critter&, Item&, uint)
{
	return true;
}

EXPORT bool critter_use_item(Critter&, Item&, Critter*,Item*,Scenery*,uint)
{
	return true;
}

EXPORT bool critter_use_skill( Critter&,int,Critter*,Item*,Scenery* ) 
{
	return true;
}

EXPORT void critter_reload_weapon( Critter&,Item&,Item* )
{
}

EXPORT void critter_init( Critter&,bool ) 
{
}

EXPORT void critter_finish( Critter&,bool )
{
}

EXPORT void critter_idle( Critter& )
{
}

EXPORT void critter_dead( Critter&,Critter* )
{
}

EXPORT void critter_respawn( Critter& ) 
{
}

EXPORT bool critter_check_move_item( Critter&,Item&,uint8,Item* ) 
{
	return true;
}

EXPORT void critter_move_item( Critter&,Item&,uint8 ) 
{
}

EXPORT void map_critter_in( Map&,Critter& ) 
{
}

EXPORT void map_critter_out( Map&,Critter& )
{
}

EXPORT bool npc_plane_begin( Critter&,NpcPlane&,int,Critter*,Item* ) 
{
	return true;
}

EXPORT bool npc_plane_end( Critter&,NpcPlane&,int,Critter*,Item* ) 
{
	return true;
}

EXPORT bool npc_plane_run( Critter&,NpcPlane&,int,uint&,uint&,uint& ) 
{
	return true;
}

EXPORT void karma_voting( Critter&,Critter&,bool ) 
{
}

EXPORT bool check_look( Map&,Critter&,Critter& ) 
{
	return true;
}

EXPORT uint item_cost( Item&,Critter&,Critter&,bool ) 
{
	return 1;
}

//EXPORT bool items_barter(Item@[] & , uint[] & , Item@[] & , uint[] & , Critter&, Critter&)
EXPORT bool items_barter( ScriptArray&, ScriptArray&, ScriptArray&, ScriptArray&, Critter&, Critter& )
{
	return true;
}

//EXPORT void items_crafted(Item@[] & , uint[] & , Item@[] & , Critter&)
EXPORT void items_crafted( ScriptArray&, ScriptArray&, ScriptArray&, Critter& )
{
}

EXPORT void player_levelup( Critter&,uint,uint,uint ) 
{
}

EXPORT void turn_based_begin( Map& map ) 
{
}

EXPORT void turn_based_end( Map& map )
{
}

EXPORT void turn_based_process( Map& map, Critter& cr,bool ) 
{
}

//EXPORT void turn_based_sequence(Map&, Critter@[] & , Critter*)
EXPORT void turn_based_sequence( Map&, ScriptArray&, Critter* )
{
}

//EXPORT void world_save(uint, uint[] & )
EXPORT void world_save( uint, ScriptArray& )
{
}

EXPORT bool player_registration( uint,ScriptString&,uint&,uint& ) 
{
	return true;
}

EXPORT bool player_login( uint, ScriptString&, uint,uint&,uint& ) 
{
	return true;
}

EXPORT bool player_getaccess( Critter& cr, int access, ScriptString& password ) 
{
	return true;
}

EXPORT bool player_allowcommand( Critter* cr, ScriptString*, uint8 command ) 
{
	return true;
}

EXPORT bool check_trap_look( Map&,Critter&,Item& ) 
{
	return true;
}
