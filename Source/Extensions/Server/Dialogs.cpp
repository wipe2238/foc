#include <Critter.h>
#include <Log.h>
#include <ProtoMap.h>

#include "FOC.Server.h"
#include "Server/Dialogs.h"
#include "Shared/SScriptFunc.h"

// wrapper
bool DialogsScenery( Critter& player, MapObject& scenery, int skill, Item* item, int dialogId )
{
    return FOC::Self()->Dialogs->Scenery( player, scenery, skill, item, dialogId );
}

FOC::DialogsManager::DialogsManager() :
    Debug( false )
{}

FOC::DialogsManager::~DialogsManager()
{}

void FOC::DialogsManager::Init()
{
    FOC::Self()->SetFunctionAddress( "Dialogs::Scenery", (size_t)&DialogsScenery );
}

bool FOC::DialogsManager::Scenery( Critter& player, MapObject& scenery, int skill, Item* item, int dialogId )
{
    if( player.IsPlayer() && skill == SKILL_PICK_ON_GROUND && !item )
    {
        if( Debug )
            WriteLogF( _FUNC_, " : player<%u> dialog<%d>\n", player.GetId(), dialogId );

        return ScriptFunc::Global_RunDialogHex( &player, dialogId, player.GetHexX(), player.GetHexY(), false );
    }

    return false;
}
