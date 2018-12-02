#include <extension.h>

#define Scenery MapObject

EXPORT void init()
{
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
}

EXPORT uint loop()
{
	return 10;
}

EXPORT void global_process(int, Critter&, Item*, float&, float&, float&, float&, float&, uint&, bool&)
{

}

EXPORT void global_invite(Critter&, Item*, uint, int, uint&, uint16&, uint16&, uint8&)
{

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
