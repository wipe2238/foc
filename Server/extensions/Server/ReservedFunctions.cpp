#include <extension.h>

#include "WorldMap.h"

#define Scenery    MapObject

EXPORT void init()
{
    // WorldMap
    WorldMap.Init();
    WorldMap.Debug = true;
    WorldMap.BaseSpeed = 10.0f;
}

EXPORT bool start()
{
    return true;
}

EXPORT void get_start_time( uint16& multiplier, uint16& year, uint16& month, uint16& day, uint16& hour, uint16& minute )
{
    FirstTime = true;
}

EXPORT void finish()
{}

EXPORT uint loop()
{
    return 10;
}

EXPORT void global_process( int processType, Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer )
{
    WorldMap.Process( processType, cr, car, curX, curY, toX, toY, speed, encounterDescriptor, waitForAnswer );
}

EXPORT void global_invite( Critter& crLeader, Item* car, uint encounterDescriptor, int combatMode, uint& mapId, uint16& hexX, uint16& hexY, uint8& dir )
{
    WorldMap.ProcessInvite( crLeader, car, encounterDescriptor, combatMode, mapId, hexX, hexY, dir );
}

EXPORT void critter_attack( Critter& cr, Critter& crTarget, ProtoItem& weapon, uint8 weaponMode, ProtoItem* ammo )
{}

EXPORT void critter_attacked( Critter& cr, Critter& crAttacker )
{}

EXPORT bool critter_stealing( Critter& cr, Critter& crThief, Item& item, uint count )
{
    return true;
}

EXPORT bool critter_use_item( Critter& cr, Item& item, Critter* crTarget, Item* itemTarget, Scenery* scenTarget, uint param )
{
    Log( "critter_use_item\n" );

    return true;
}

EXPORT bool critter_use_skill( Critter& cr, int skill, Critter* crTarget, Item* itemTarget, Scenery* scenTarget )
{
    Log( "critter_use_skill( cr=%u, skill=%d, targetCr=%u, targetItem=%u, targetScenery=%u )\n",
         cr.Id, skill, crTarget ? crTarget->Id : 0, itemTarget ? itemTarget->Id : 0, scenTarget ? scenTarget->UID : 0 );

    switch( skill )
    {
        case SKILL_PICK_ON_GROUND:
        {
            if( itemTarget )
            {
                // Doors and containers
                if( itemTarget->IsDoor() || itemTarget->IsContainer() )
                {
                    // if (Lockers.ProcessUseSkill(cr, skill, itemTarget))
                    //	return true;
                }
            }
        }
    }
    return false;
}

EXPORT void critter_reload_weapon( Critter& cr, Item& weapon, Item* ammo )
{}

EXPORT void critter_init( Critter& cr, bool firstTime )
{}

EXPORT void critter_finish( Critter& cr, bool toDelete )
{}

EXPORT void critter_idle( Critter& cr )
{}

EXPORT void critter_dead( Critter& cr, Critter* crKiller )
{}

EXPORT void critter_respawn( Critter& cr )
{}

EXPORT bool critter_check_move_item( Critter& cr, Item& item, uint8 toSlot, Item* itemSwap )
{
    return true;
}

EXPORT void critter_move_item( Critter& cr, Item& item, uint8 fromSlot )
{}

EXPORT void map_critter_in( Map& map, Critter& cr )
{}

EXPORT void map_critter_out( Map& map, Critter& cr )
{}

EXPORT bool npc_plane_begin( Critter& npc, NpcPlane& plane, int reason, Critter* crTarget, Item* itemTarget )
{
    return true;
}

EXPORT bool npc_plane_end( Critter& npc, NpcPlane& plane, int reason, Critter* crTarget, Item* itemTarget )
{
    return true;
}

EXPORT bool npc_plane_run( Critter& npc, NpcPlane& plane, int reason, uint& r0, uint& r1, uint& r2 )
{
    return true;
}

EXPORT void karma_voting( Critter& crFrom, Critter& crTo, bool valUp )
{}

EXPORT bool check_look( Map& map, Critter& cr, Critter& crOther )
{
    return true;
}

EXPORT uint item_cost( Item& item, Critter& cr, Critter& npc, bool buy )
{
    return 1;
}

EXPORT bool items_barter( ScriptArray /*Item@[]*/& saleItems, ScriptArray /*uint[]*/& saleItemsCount, ScriptArray /*Item@[]*/& buyItems, ScriptArray /*uint[]*/& buyItemsCount, Critter& cr, Critter& npc )
{
    return true;
}

EXPORT void items_crafted( ScriptArray /*Item@[]*/& items, ScriptArray /*uint[]*/& itemsCount, ScriptArray /*Item@[]*/& resources, Critter& cr )
{}

EXPORT void player_levelup( Critter& player, uint skillIndex, uint skillUp, uint perkIndex )
{}

EXPORT void turn_based_begin( Map& map )
{}

EXPORT void turn_based_end( Map& map )
{}

EXPORT void turn_based_process( Map& map, Critter& cr, bool beginTurn )
{}

EXPORT void turn_based_sequence( Map& map, ScriptArray /*Critter@[]*/& critters, Critter* crFirstTurn )
{}

EXPORT void world_save( uint currentIndex, ScriptArray /*uint[]*/& deleteIndexes )
{}

EXPORT bool player_connect( uint ip, ScriptString& message ) // not implemented
{
    return true;
}

EXPORT bool player_registration( uint ip, ScriptString& name, uint& textMsg, uint& strNum )
{
    return true;
}

EXPORT bool player_login( uint ip, ScriptString& name, uint id, uint& textMsg, uint& strNum )
{
    return true;
}

EXPORT bool player_getaccess( Critter& cr, int access, ScriptString& password )
{
    return true;
}

EXPORT bool player_allowcommand( Critter* cr, ScriptString* admin, uint8 command )
{
    return true;
}

EXPORT bool check_trap_look( Map& map, Critter& cr, Item& trap )
{
    return true;
}
