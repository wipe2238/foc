#include <extension.h>

#include "Animations.h"
#include "UserInterface.h"

#define ItemCl    Item

EXPORT bool start()
{
    return true;
}

EXPORT uint loop()
{
    return 10;
}

EXPORT void get_active_screens( ScriptArray /*int[]*/& screens )
{
    UI.GetActiveScreens( screens );
}

EXPORT void screen_change( bool show, int screen, int p0, int p1, int p2 )
{
    if( show )
        UI.ShowScreen( screen, p0, p1, p2 );
    else
        UI.HideScreen( screen, p0, p1, p2 );
}

EXPORT void render_iface( uint layer )
{
    UI.RenderInterface( layer );
}

EXPORT void render_map()
{}

EXPORT bool mouse_down( int click )
{
    return false;
}

EXPORT bool mouse_up( int click )
{
    return false;
}

EXPORT void mouse_move( int x, int y )
{}

EXPORT bool key_down( uint8 key, ScriptString& keyText )
{
    return false;
}

EXPORT bool key_up( uint8 key, ScriptString& keyText )
{
    return false;
}

EXPORT void input_lost()
{}

EXPORT void critter_in( CritterCl& cr )
{}

EXPORT void critter_out( CritterCl& cr )
{}

EXPORT void item_map_in( ItemCl& item )
{}

EXPORT void item_map_changed( ItemCl& itemNew, ItemCl& itemOld )
{}

EXPORT void item_map_out( ItemCl& item )
{}

EXPORT void item_inv_in( ItemCl& item )
{}

EXPORT void item_inv_out( ItemCl& item )
{}

EXPORT bool map_message( ScriptString& message, uint16& hexX, uint16& hexY, uint& color, uint& delay )
{
    return true;
}

EXPORT bool in_message( ScriptString& message, int& sayType, uint& critterId, uint& delay )
{
    return true;
}

EXPORT bool out_message( ScriptString& message, int& sayType )
{
    return true;
}

EXPORT int to_hit( CritterCl& chosen, CritterCl& crTarget, ProtoItem& weapon, uint8 weaponMode )
{
    return 95;
}

EXPORT void hit_aim( uint8& aim )
{}

// EXPORT void combat_result(uint[] & data)
EXPORT void combat_result( ScriptString& data )
{}

/*
   EXPORT ScriptString& generic_description(int descriptionType, int& offsetX, int& offsetY)
   {
        return ScriptString::Create( "" );
   }

   EXPORT ScriptString& item_description(ItemCl& item, int lookType)
   {
        return ScriptString::Create("");
   }

   EXPORT ScriptString& critter_description(CritterCl& cr, int lookType)
   {
        return ScriptString::Create("");
   }
 */

EXPORT bool get_elevator( uint type, ScriptArray /*uint[]*/& data )
{
    return true;
}

EXPORT uint item_cost( ItemCl& item, CritterCl& chosen, CritterCl& npc, bool sell )
{
    return 1;
}

EXPORT bool check_perk( CritterCl& chosen, uint perk )
{
    return true;
}

EXPORT void player_data_generate( ScriptArray /*int[]*/& data )
{}

EXPORT bool player_data_check( ScriptString& name, ScriptArray /*int[]*/& data )
{
    return true;
}

EXPORT void critter_action( bool localCall, CritterCl& cr, int action, int actionExt, ItemCl* item )
{
    Log( "critter_action(localCall=%s, cr=%u, action=%d, actionExt=%d, item=%u)\n",
         localCall ? "true" : "false", cr.Id, action, actionExt, item ? item->Id : 0 );

    Animations.ProcessAction( localCall, cr, action, actionExt, item );
}

EXPORT void animation2d_process( bool animateStay, CritterCl& cr, uint anim1, uint anim2, ItemCl* item )
{
    Log( "animation2d_process\n" );
}

EXPORT void animation3d_process( bool animateStay, CritterCl& cr, uint anim1, uint anim2, ItemCl* item )
{
    Log( "animation3d_process\n" );
}

EXPORT void items_collection( int collection, ScriptArray /*ItemCl@[]*/& items )
{}

EXPORT ScriptString* critter_animation( int animType, uint crType, uint anim1, uint anim2, uint& pass, uint& flags, int& ox, int& oy )
{
    Log( "critter_animation\n" );

    return nullptr;
}

EXPORT bool critter_animation_substitute( int animType, uint crTypeBase, uint anim1base, uint anim2base, uint& crType, uint& anim1, uint& anim2 )
{
    Log( "critter_animation_substitute\n" );

    return true;
}

EXPORT bool critter_animation_fallout( uint crType, uint& anim1, uint& anim2, uint& anim1ex, uint& anim2ex, uint& flags )
{
    return Animations.ProcessFallout( crType, anim1, anim2, anim1ex, anim2ex, flags );
}

EXPORT void filename_logfile( ScriptString& filename )
{}

EXPORT void filename_screenshot( ScriptString& filename )
{}

EXPORT bool critter_check_move_item( CritterCl& cr, ItemCl& item, uint8 toSlot, ItemCl* itemSwap )
{
    return true;
}
