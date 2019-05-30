#include <Extension.h>
#include <GameOptions.h>
#include <Log.h>
#include <Script.h>

#include "FOC.Client.h"
#include "Client/Animations.h"
#include "Client/Interface.h"
#include "Shared/ApCost.h"
#include "Shared/MoveItem.h"
#include "Shared/SScriptFunc.h"

using namespace std;

typedef Item ItemCl;

void InitExtensionsClient() // used by engine
{
    FOC::Init<FOC::Client>();
}

void FinishExtensionsClient() // used by engine
{
    FOC::Finish<FOC::Client>();
}

static bool start()
{
    GAME_OPTION( MapDataPrefix ).assign( "art/geometry/fallout/" );
    GAME_OPTION_EXT( WallAlpha ) = 200;

    return true;
}

static void finish()
{}

static uint loop()
{
    return 10;
}

static void get_active_screens( ScriptArray /*int[]*/& screens )
{
    FOC::Self()->Interface->ProcessGetActiveScreens( screens );
}

static void screen_change( bool show, int screen, int p0, int p1, int p2 )
{
    FOC::Self()->Interface->ProcessScreenChange( show, screen, p0, p1, p2 );
}

static void render_iface( uint layer )
{
    FOC::Self()->Interface->ProcessRenderInterface( layer );
}

static void render_map()
{}

static bool mouse_down( int click )
{
    WriteLogF( _FUNC_, "<%u>\n", click );

    if( click == MOUSE_CLICK_WHEEL_UP || click == MOUSE_CLICK_WHEEL_DOWN )
    {
        float zoom = GAME_OPTION( SpritesZoom ) + (click == MOUSE_CLICK_WHEEL_DOWN ? 0.1f : -0.1f);
        FOClient::SScriptFunc::Global_SetZoom( zoom );
    }

    return false;
}

static bool mouse_up( int click )
{
    WriteLogF( _FUNC_, "<%u>\n", click );
    return false;
}

static void mouse_move( int x, int y )
{}

static bool key_down( uint8 key, ScriptString& keyText )
{
    WriteLogF( _FUNC_, "<%u> %s\n", key, keyText.c_str() );
    return false;
}

static bool key_up( uint8 key, ScriptString& keyText )
{
    WriteLogF( _FUNC_, "<%u> %s\n", key, keyText.c_str() );
    return false;
}

static void input_lost()
{}

static void critter_in( CritterCl& cr )
{}

static void critter_out( CritterCl& cr )
{}

static void item_map_in( ItemCl& item )
{}

void item_map_changed( ItemCl& itemNew, ItemCl& itemOld )
{
    WriteLogF( _FUNC_, "\n" );
}

static void item_map_out( ItemCl& item )
{}

static void item_inv_in( ItemCl& item )
{}

static void item_inv_out( ItemCl& item )
{}

static bool map_message( ScriptString& message, uint16& hexX, uint16& hexY, uint& color, uint& delay )
{
    return true;
}

static bool in_message( ScriptString& message, int& sayType, uint& critterId, uint& delay )
{
    return true;
}

static bool out_message( ScriptString& message, int& sayType )
{
    return true;
}

static int to_hit( CritterCl& chosen, CritterCl& crTarget, ProtoItem& weapon, uint8 weaponMode )
{
    return 95;
}

static void hit_aim( uint8& aim )
{}

// void combat_result(uint[] & data)
static void combat_result( ScriptString& data )
{}

/*
   ScriptString& generic_description(int descriptionType, int& offsetX, int& offsetY)
   {
        return ScriptString::Create( "" );
   }

   ScriptString& item_description(ItemCl& item, int lookType)
   {
        return ScriptString::Create("");
   }

   ScriptString& critter_description(CritterCl& cr, int lookType)
   {
        return ScriptString::Create("");
   }
 */

static bool get_elevator( uint type, ScriptArray /*uint[]*/& data )
{
    return true;
}

static uint item_cost( ItemCl& item, CritterCl& chosen, CritterCl& npc, bool sell )
{
    return 1;
}

static bool check_perk( CritterCl& chosen, uint perk )
{
    return true;
}

static void player_data_generate( ScriptArray /*int[]*/& data )
{}

static bool player_data_check( ScriptString& name, ScriptArray /*int[]*/& data )
{
    return true;
}

static void critter_action( bool localCall, CritterCl& cr, int action, int actionExt, ItemCl* item )
{
    WriteLogF( _FUNC_, "(localCall=%s, cr=%u, action=%d, actionExt=%d, item=%u)\n",
               localCall ? "true" : "false", cr.Id, action, actionExt, item ? item->GetId() : 0 );

    FOC::Self()->Animations->ProcessAction( localCall, cr, action, actionExt, item );
}

static uint critter_attack_distance( Critter& cr, Item& weapon, uint8 use )
{
    return 1;
}

static void animation2d_process( bool animateStay, CritterCl& cr, uint anim1, uint anim2, ItemCl* item )
{
    WriteLogF( _FUNC_, "\n" );
}

static void animation3d_process( bool animateStay, CritterCl& cr, uint anim1, uint anim2, ItemCl* item )
{
    WriteLogF( _FUNC_, "\n" );
}

static void items_collection( int collection, ScriptArray /*ItemCl@[]*/& items )
{}

ScriptString* critter_animation( int animType, uint crType, uint anim1, uint anim2, uint& pass, uint& flags, int& ox, int& oy )
{
    WriteLogF( _FUNC_, "\n" );

    return nullptr;
}

static bool critter_animation_substitute( int animType, uint crTypeBase, uint anim1base, uint anim2base, uint& crType, uint& anim1, uint& anim2 )
{
    WriteLogF( _FUNC_, "\n" );

    return true;
}

static bool critter_animation_fallout( uint crType, uint& anim1, uint& anim2, uint& anim1ex, uint& anim2ex, uint& flags )
{
    return FOC::Self()->Animations->ProcessFallout( crType, anim1, anim2, anim1ex, anim2ex, flags );
}

static void filename_logfile( ScriptString& filename )
{}

static void filename_screenshot( ScriptString& filename )
{}

FOC::Client::Client() : Extension()
{
    Animations = new AnimationsManager();
    Interface = new InterfaceManager();
}

FOC::Client::~Client()
{
    delete Animations;
    delete Interface;
}

size_t FOC::Client::GetFunctionAddress( const string& name )
{
    GET_ADDRESS( start );
    GET_ADDRESS( finish );
    GET_ADDRESS( loop );

    GET_ADDRESS( get_active_screens );
    GET_ADDRESS( screen_change );
    GET_ADDRESS( render_iface );
    GET_ADDRESS( render_map );

    GET_ADDRESS( mouse_down );
    GET_ADDRESS( mouse_up );
    GET_ADDRESS( mouse_move );
    GET_ADDRESS( key_down );
    GET_ADDRESS( key_up );
    GET_ADDRESS( input_lost );

    GET_ADDRESS( critter_in );
    GET_ADDRESS( critter_out );
    GET_ADDRESS( item_map_in );
    GET_ADDRESS( item_map_changed );
    GET_ADDRESS( item_map_out );
    GET_ADDRESS( item_inv_in );
    GET_ADDRESS( item_inv_out );

    GET_ADDRESS( map_message );
    GET_ADDRESS( in_message );
    GET_ADDRESS( out_message );

    GET_ADDRESS( to_hit );
    GET_ADDRESS( hit_aim );
    GET_ADDRESS( combat_result );

    GET_ADDRESS( get_elevator );
    GET_ADDRESS( item_cost );

    GET_ADDRESS( check_perk );
    GET_ADDRESS( player_data_generate );
    GET_ADDRESS( player_data_check );

    GET_ADDRESS( critter_action );
    GET_ADDRESS( critter_attack_distance );
    GET_ADDRESS( animation2d_process );
    GET_ADDRESS( animation3d_process );
    GET_ADDRESS( items_collection );
    GET_ADDRESS( critter_animation );
    GET_ADDRESS( critter_animation_substitute );
    GET_ADDRESS( critter_animation_fallout );

    GET_ADDRESS( filename_logfile );
    GET_ADDRESS( filename_screenshot );

    GET_ADDRESS_NSX( critter_use_item_ap_cost, ApCost::UseItem );
    GET_ADDRESS_NSX( critter_check_move_item, MoveItem::Check );

    return 0;
}
