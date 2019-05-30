#include <Critter.h>
#include <Log.h>
#include <ProtoMap.h>

#include "FOC.Server.h"
#include "Server/Dialogs.h"
#include "Shared/SScriptFunc.h"

typedef MapObject Scenery;

FOC::DialogsManager::DialogsManager() :
    Debug( false )
{}

FOC::DialogsManager::~DialogsManager()
{}

bool SceneryDialog( Critter& player, Scenery& scenery, int skill, Item* item, int dialogId )
{
    if( skill == SKILL_PICK_ON_GROUND && !item )
    {
        if( FOC::Self()->Dialogs->Debug )
            WriteLogF( _FUNC_, " player<%u> dialog<%d>\n", player.GetId(), dialogId );

        return FOServer::SScriptFunc::Global_RunDialogHex( &player, dialogId, player.GetHexX(), player.GetHexY(), false );
    }

    return false;
}
