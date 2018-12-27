#include <extension.h>
#include <ExecuteString.h>

#include "Dialogs.h"

DialogsManager Dialogs;

DialogsManager::DialogsManager() :
    Debug( false )
{}

DialogsManager::~DialogsManager()
{}

bool DialogsManager::RunDialog( Critter& cr, uint dialogId, uint16 hexX, uint16 hexY, bool ignoreDistance )
{
    static const char* codeRunDialog = "return RunDialog(GetCritter(%u), %u, %u, %u, %s);";
    char               code[100];
    bool               result = false;

    sprintf( code, codeRunDialog, cr.Id, dialogId, hexX, hexY, ignoreDistance ? "true" : "false" );
    ExecuteString( ASEngine, code, &result, asTYPEID_BOOL, 0, 0 );

    return result;
}

EXPORT bool SceneryDialog( Critter& player, Scenery& scenery, int skill, Item* item, int dialogId )
{
    if( skill == SKILL_PICK_ON_GROUND && !item )
    {
        if( Dialogs.Debug )
            Log( "DialogsManager::SceneryDialog player<%u> dialog<%d>\n", player.Id, dialogId );

        return Dialogs.RunDialog( player, dialogId, scenery.MapX, scenery.MapY, false );
    }

    return false;
}
