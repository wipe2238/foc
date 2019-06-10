#include <cassert>

#include <Addons/scriptarray.h>
#include <Log.h>
#include <Map.h>
#include <MapManager.h>

#include "FOC.Server.h"
#include "Server/Lockers.h"
#include "Shared/SScriptFunc.h"

using namespace std;

FOC::LockersManager::LockersManager() :
    Debug( false )
{}

FOC::LockersManager::~LockersManager()
{}

bool FOC::LockersManager::ProcessUseSkill( Critter& cr, int skill, Item* itemTarget )
{
    if( Debug )
        WriteLog( "LockersManager::ProcessUseSkill(cr=%u, skill=%d, itemTarget=%u\n", cr.GetId(), skill, itemTarget ? itemTarget->Id : 0 );

    if( !itemTarget || !(itemTarget->IsDoor() || itemTarget->IsContainer() ) )
        return false;

    if( !cr.GetMap() )
    {
        // TODO STR_USE_NOTHING
        return true;
    }

    if( skill == SKILL_PICK_ON_GROUND )
    {
        return UseLocker( cr, itemTarget );
    }

    return true;
}

//

bool FOC::LockersManager::UseLocker( Critter& cr, Item* locker )
{
    if( !locker )
    {
        WriteLog( "ERROR[%s] locker nullptr : cr<%u> map<%u> hx<%u> hy<%u>\n", _FUNC_, cr.GetId(), cr.GetMap(), cr.GetHexX(), cr.GetHexY() );
        return true;
    }
    else if( locker->IsNotValid )
        return false;

    if( Debug )
        WriteLog( "LockersManager::UseLocker(cr=%u, locker=%u)\n", cr.GetId(), locker->Id );

    if( locker->IsDoor() )
    {
        if( !locker->Proto->Container_Changeble )
        {
            if( Debug )
                WriteLog( "LockersManager::UseLocker item<%u> pid<%u> critter<%u> : doors not changeable\n", locker->Id, locker->Proto->ProtoId, cr.GetId() );

            // TODO STR_USE_NOTHING
            return true;
        }

        if( FLAG( locker->Data.LockerCondition, LOCKER_ISOPEN ) )
        {
            if( Debug )
                WriteLog( "LockersManager::UseLocker item<%u> pid<%u> critter<%u> : close door\n", locker->Id, locker->Proto->ProtoId, cr.GetId() );
            UseLockerAction( locker, false );
        }
        else
        {
            if( Debug )
                WriteLog( "LockersManager::UseLocker item<%u> pid<%u> critter<%u> : open door\n", locker->Id, locker->Proto->ProtoId, cr.GetId() );
            UseLockerAction( locker, true );
        }
    }
    else if( locker->IsContainer() )
    {
        uint8 transferType = locker->Proto->GroundLevel ? TRANSFER_HEX_CONT_DOWN : TRANSFER_HEX_CONT_UP;

        if( locker->Proto->Container_Changeble )
        {
            if( FLAG( locker->Data.LockerCondition, LOCKER_ISOPEN ) )
            {
                if( Debug )
                    WriteLog( "LockersManager::UseLocker item<%u> pid<%u> critter<%u> : close container\n", locker->Id, locker->Proto->ProtoId, cr.GetId() );
                UseLockerAction( locker, false );
            }
            else
            {
                if( Debug )
                    WriteLog( "LockersManager::UseLocker item<%u> pid<%u> critter<%u> : open container\n", locker->Id, locker->Proto->ProtoId, cr.GetId() );
                UseLockerAction( locker, true );
                FOServer::SScriptFunc::Cl_ShowContainer( &cr, nullptr, locker, transferType );
            }
        }
        else
        {
            if( Debug )
                WriteLog( "LockersManager::UseLocker item<%u> pid<%u> critter<%u> : view container\n", locker->Id, locker->Proto->ProtoId, cr.GetId() );
            FOServer::SScriptFunc::Cl_ShowContainer( &cr, nullptr, locker, transferType );
        }
    }

    return true;
}

void FOC::LockersManager::UseLockerAction( Item* locker, bool open, uint delay /* = 0 */ )
{
    // if( !delay )
    {
        if( open )
            FOServer::SScriptFunc::Item_LockerOpen( locker );
        else
            FOServer::SScriptFunc::Item_LockerClose( locker );

        std::string text = open ? "open" : "close";
        Map*        map = MapMngr.GetMap( locker->AccHex.MapId );
        map->SetText( locker->AccHex.HexX, locker->AccHex.HexY, COLOR_XRGB( 0, 255, 0 ), text.c_str(), text.size(), 0, true );
    }
}
