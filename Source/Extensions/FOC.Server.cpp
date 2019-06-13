#include <AI.h>
#include <Log.h>
#include <Ini.h>
#include <Item.h>
#include <Map.h>
#include <ProtoMap.h>
#include <Script.h>

#include "FOC.Server.h"

#include "AngelScript/Buffer.h"

#include "Server/Dialogs.h"
#include "Server/Lockers.h"
#include "Server/WorldMap.h"
#include "Shared/ApCost.h"
#include "Shared/MoveItem.h"

using namespace std;

typedef MapObject   Scenery;
typedef AIDataPlane NpcPlane;

#include "Server/Dialogs.h"
#include "Server/Lockers.h"
#include "Server/WorldMap.h"

void InitExtensionsServer()
{
    FOC::Init<FOC::Server>();
}

void FinishExtensionsServer()
{
    FOC::Finish<FOC::Server>();
}

static void init()
{
    WriteLog( "<<<   Initializing   >>>\n" );

    GAME_OPTION( TimeoutTransfer ) = 0;

    // Dialogs
    WriteLog( "Dialogs...\n" );
    FOC::Self()->Dialogs->Debug = true;

    // Lockers
    WriteLog( "Lockers...\n" );
    FOC::Self()->Lockers->Debug = true;

    // WorldMap
    WriteLog( "Worldmap...\n" );
    FOC::Self()->WorldMap->Init();
    FOC::Self()->WorldMap->Debug = true;
    FOC::Self()->WorldMap->BaseSpeed = 10.0f;
    /*
       asIScriptEngine* script_engine = FOC::Self()->ASEngine;
       if( !script_engine )
       {
       WriteLog( "\nERROR\nERROR ASEngine nullptr\nERROR\n" );
       script_engine = Script::GetEngine();
       }
     */

    WriteLog( "SetItemMask...\n" );
    static char mask0[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0 };
    static char mask1[] = { 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    static char mask2[] = { 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0 };
    static char mask3[] = { -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0 };
    static char mask4[] = { -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0 };

    memzero( Item::ItemData::SendMask, sizeof(Item::ItemData::SendMask) );
    memcpy( Item::ItemData::SendMask[0], mask0, sizeof(Item::ItemData::SendMask[0]) );
    memcpy( Item::ItemData::SendMask[1], mask1, sizeof(Item::ItemData::SendMask[1]) );
    memcpy( Item::ItemData::SendMask[2], mask2, sizeof(Item::ItemData::SendMask[2]) );
    memcpy( Item::ItemData::SendMask[3], mask3, sizeof(Item::ItemData::SendMask[3]) );
    memcpy( Item::ItemData::SendMask[4], mask4, sizeof(Item::ItemData::SendMask[4]) );

    WriteLog( "<<<   Initialized   >>>\n" );
}

static bool start()
{
    WriteLog( "<<<   Starting   >>>\n" );

    return true;
}

static void get_start_time( uint16& multiplier, uint16& year, uint16& month, uint16& day, uint16& hour, uint16& minute )
{
    WriteLog( "<<<   Starting for a first time   >>>\n" );

    // FirstStart = true;
}

static void finish()
{
    WriteLog( "<<<   Finishing   >>>\n" );
}

static uint loop()
{
    return 10;
}

static void global_process( int processType, Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer )
{
    FOC::Self()->WorldMap->Process( processType, cr, car, curX, curY, toX, toY, speed, encounterDescriptor, waitForAnswer );
}

static void global_invite( Critter& crLeader, Item* car, uint encounterDescriptor, int combatMode, uint& mapId, uint16& hexX, uint16& hexY, uint8& dir )
{
    FOC::Self()->WorldMap->ProcessInvite( crLeader, car, encounterDescriptor, combatMode, mapId, hexX, hexY, dir );
}

static void critter_attack( Critter& cr, Critter& crTarget, ProtoItem& weapon, uint8 weaponMode, ProtoItem* ammo )
{}

static uint critter_attack_distance( Critter& cr, Item& weapon, uint8 use )
{
    return 1;
}

static void critter_attacked( Critter& cr, Critter& crAttacker )
{}

static bool critter_stealing( Critter& cr, Critter& crThief, Item& item, uint count )
{
    return true;
}

static bool critter_use_item( Critter& cr, Item& item, Critter* crTarget, Item* itemTarget, Scenery* scenTarget, uint param )
{
    WriteLog( "critter_use_item\n" );

    return true;
}

static bool critter_use_skill( Critter& cr, int skill, Critter* crTarget, Item* itemTarget, Scenery* scenTarget )
{
    WriteLogF( _FUNC_, "( cr=%u, skill=%d, targetCr=%u, targetItem=%u, targetScenery=%u )\n",
               cr.GetId(), skill, crTarget ? crTarget->GetId() : 0, itemTarget ? itemTarget->Id : 0, scenTarget ? scenTarget->UID : 0 );

    switch( skill )
    {
        case SKILL_PICK_ON_GROUND:
        {
            if( itemTarget && (itemTarget->IsDoor() || itemTarget->IsContainer() ) )
            {
                if( FOC::Self()->Lockers->ProcessUseSkill( cr, skill, itemTarget ) )
                    return true;
            }
            else if( itemTarget )
                return false;

            break;
        }
        case SKILL_PUT_CONT:
        case SKILL_TAKE_CONT:
        case SKILL_TAKE_ALL_CONT:
            return false;
        default:
            break;
    }

    return true;
}

static void critter_reload_weapon( Critter& cr, Item& weapon, Item* ammo )
{}

static void critter_init( Critter& cr, bool firstTime )
{
    cr.ChangeParam( ST_MAX_LIFE );
    cr.ChangeParam( ST_ACTION_POINTS );
    cr.ChangeParam( ST_CARRY_WEIGHT );
    cr.Data.Params[ST_MAX_LIFE] = std::max( 5, cr.GetRawParam( ST_MAX_LIFE ) );
    cr.Data.Params[ST_ACTION_POINTS] = std::max( 5, cr.GetRawParam( ST_ACTION_POINTS ) );
    cr.Data.Params[ST_CARRY_WEIGHT] = std::max( 10000, cr.GetRawParam( ST_CARRY_WEIGHT ) );
    cr.ProcessChangedParams();
}

static void critter_finish( Critter& cr, bool toDelete )
{}

static void critter_idle( Critter& cr )
{}

static void critter_dead( Critter& cr, Critter* crKiller )
{}

static void critter_respawn( Critter& cr )
{}

static void map_critter_in( Map& map, Critter& cr )
{
    WriteLogF( _FUNC_, " : map<id:%u,pid:%u> critter<%u>\n", map.GetId(), map.GetPid(), cr.GetId() );
}

static void map_critter_out( Map& map, Critter& cr )
{
    WriteLogF( _FUNC_, " : map<id:%u,pid:%u> critter<%u>\n", map.GetId(), map.GetPid(), cr.GetId() );
}

static bool npc_plane_begin( Critter& npc, NpcPlane& plane, int reason, Critter* crTarget, Item* itemTarget )
{
    return true;
}

static bool npc_plane_end( Critter& npc, NpcPlane& plane, int reason, Critter* crTarget, Item* itemTarget )
{
    return true;
}

static bool npc_plane_run( Critter& npc, NpcPlane& plane, int reason, uint& r0, uint& r1, uint& r2 )
{
    return true;
}

static void karma_voting( Critter& crFrom, Critter& crTo, bool valUp )
{}

static bool check_look( Map& map, Critter& cr, Critter& crOther )
{
    return true;
}

static uint item_cost( Item& item, Critter& cr, Critter& npc, bool buy )
{
    return 1;
}

static bool items_barter( ScriptArray /*Item@[]*/& saleItems, ScriptArray /*uint[]*/& saleItemsCount, ScriptArray /*Item@[]*/& buyItems, ScriptArray /*uint[]*/& buyItemsCount, Critter& cr, Critter& npc )
{
    return true;
}

static void items_crafted( ScriptArray /*Item@[]*/& items, ScriptArray /*uint[]*/& itemsCount, ScriptArray /*Item@[]*/& resources, Critter& cr )
{}

static void player_levelup( Critter& player, uint skillIndex, uint skillUp, uint perkIndex )
{}

static void turn_based_begin( Map& map )
{}

static void turn_based_end( Map& map )
{}

static void turn_based_process( Map& map, Critter& cr, bool beginTurn )
{}

static void turn_based_sequence( Map& map, ScriptArray /*Critter@[]*/& critters, Critter* crFirstTurn )
{}

static void world_save( uint currentIndex, ScriptArray /*uint[]*/& deleteIndexes )
{}

static bool player_connect( uint ip, ScriptString& message )     // not implemented
{
    return true;
}

static bool player_registration( uint ip, ScriptString& name, uint& textMsg, uint& strNum )
{
    return true;
}

static bool player_login( uint ip, ScriptString& name, uint id, uint& textMsg, uint& strNum )
{
    return true;
}

static bool player_getaccess( Critter& cr, uint8 access, ScriptString& password )
{
    return true;
}

static bool player_allowcommand( Critter* cr, ScriptString* admin, uint8 command )
{
    WriteLogF( _FUNC_, " : command<%u>\n", command );

    return true;
}

static bool check_trap_look( Map& map, Critter& cr, Item& trap )
{
    return true;
}

extern bool SceneryDialog( Critter& player, Scenery& scenery, int skill, Item* item, int dialogId );

FOC::Server::Server() : Extension()
{
    Dialogs = new DialogsManager();
    Lockers = new LockersManager();
    WorldMap = new WorldMapManager();
}

FOC::Server::~Server()
{
    delete Lockers;
    delete WorldMap;
}

size_t FOC::Server::GetFunctionAddress( const string& name )
{
    GET_ADDRESS( init );
    GET_ADDRESS( start );
    GET_ADDRESS( get_start_time );
    GET_ADDRESS( finish );

    GET_ADDRESS( loop );

    GET_ADDRESS( global_process );
    GET_ADDRESS( global_invite );

    GET_ADDRESS( critter_attack );
    GET_ADDRESS( critter_attack_distance );
    GET_ADDRESS( critter_attacked );
    GET_ADDRESS( critter_stealing );
    GET_ADDRESS( critter_use_item );
    GET_ADDRESS( critter_use_skill );
    GET_ADDRESS( critter_reload_weapon );
    GET_ADDRESS( critter_init );
    GET_ADDRESS( critter_finish );
    GET_ADDRESS( critter_idle );
    GET_ADDRESS( critter_dead );
    GET_ADDRESS( critter_respawn );

    GET_ADDRESS( map_critter_in );
    GET_ADDRESS( map_critter_out );

    GET_ADDRESS( npc_plane_begin );
    GET_ADDRESS( npc_plane_end );
    GET_ADDRESS( npc_plane_run );

    GET_ADDRESS( karma_voting );

    GET_ADDRESS( check_look );

    GET_ADDRESS( item_cost );
    GET_ADDRESS( items_barter );
    GET_ADDRESS( items_crafted );

    GET_ADDRESS( player_levelup );

    GET_ADDRESS( turn_based_begin );
    GET_ADDRESS( turn_based_end );
    GET_ADDRESS( turn_based_process );
    GET_ADDRESS( turn_based_sequence );

    GET_ADDRESS( world_save );

    GET_ADDRESS( player_connect );
    GET_ADDRESS( player_registration );
    GET_ADDRESS( player_login );
    GET_ADDRESS( player_getaccess );
    GET_ADDRESS( player_getaccess );
    GET_ADDRESS( player_allowcommand );

    GET_ADDRESS( check_trap_look );

    GET_ADDRESS_NSX( critter_use_item_ap_cost, ApCost::UseItem );
    GET_ADDRESS_NSX( critter_check_move_item, MoveItem::Check );
    GET_ADDRESS_NSX( critter_move_item, MoveItem::Move );

    // non-reserved function

    GET_ADDRESS( SceneryDialog );

    return 0;
}
