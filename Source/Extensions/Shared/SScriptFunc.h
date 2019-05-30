#ifndef __SSCRIPT_FUNC__
#define __SSCRIPT_FUNC__

#if defined (FOCLASSIC_CLIENT)
# include <Addons/scriptstring.h>
#elif defined (FOCLASSIC_SERVER)
# include <Critter.h>
# include <Item.h>
#endif

// TODO band-aid to avoid including main class header
//      should be extracted to [TARGET]Script.h in engine
#if !defined (FOCLASSIC_MAPPER)
# if defined (FOCLASSIC_CLIENT)
class FOClient
# elif defined (FOCLASSIC_SERVER)
class FOServer
# endif
{
public:
    struct SScriptFunc
    {
        # if defined (FOCLASSIC_CLIENT)
        static void Global_DrawHardcodedScreen( int screen );
        static void Global_DrawText( ScriptString& text, int x, int y, int w, int h, uint color, int font, int flags );
        static void Global_SetZoom( float zoom );
        # elif defined (FOCLASSIC_SERVER)
        static bool Global_RunDialogHex( Critter* player, uint dlg_pack, uint16 hx, uint16 hy, bool ignore_distance );

        static int  Cl_GetFog( Critter* cl, uint16 zone_x, uint16 zone_y );
        static void Cl_SetFog( Critter* cl, uint16 zone_x, uint16 zone_y, int fog );
        static bool Cl_IsKnownLoc( Critter* cl, bool by_id, uint loc_num );
        static bool Cl_SetKnownLoc( Critter* cl, bool by_id, uint loc_num );
        static void Cl_ShowContainer( Critter* cl, Critter* cr_cont, Item* item_cont, uint8 transfer_type );

        static bool Item_LockerOpen( Item* item );
        static bool Item_LockerClose( Item* item );
        # endif
    };
};
#endif // !FOCLASSIC_MAPPER

#endif // __SCRIPT_FUNC__ //
