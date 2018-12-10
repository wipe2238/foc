#include <cassert>

#include <extension.h>

#include <ExecuteString.h>

#include "Lockers.h"

LockersManager Lockers;

LockersManager::LockersManager() :
    Debug( false )
{}

bool LockersManager::ProcessUseSkill( Critter& cr, int skill, Item* itemTarget )
{
    if( Debug )
        Log( "LockersManager::ProcessUseSkill(cr=%u, skill=%d, itemTarget=%u\n", cr.Id, skill, itemTarget ? itemTarget->Id : 0 );

    assert( itemTarget );
    assert( locker.IsDoor() || locker.IsContainer() );

    if( !cr.MapId )
    {
        // TODO STR_USE_NOTHING
        return true;
    }

    if( skill == SKILL_PICK_ON_GROUND )
    {
        return UseLocker( cr, *itemTarget );
    }

    return true;
}

//

bool LockersManager::UseLocker( Critter& cr, Item& locker )
{
    if( Debug )
        Log( "LockersManager::UseLocker(cr=%u, locker=%u)\n", cr.Id, locker.Id );

    assert( locker.IsDoor() || locker.IsContainer() );

    if( locker.IsDoor() )
    {
        if( !locker.Proto->Container_Changeble )
        {
            if( Debug )
                Log( "LockersManager::UseLocker item<%u> pid<%u> critter<%u> : doors not changeable\n", locker.Id, locker.Proto->ProtoId, cr.Id );

            // TODO STR_USE_NOTHING
            return true;
        }

        if( FLAG( locker.Data.LockerCondition, LOCKER_ISOPEN ) )
        {
            if( Debug )
                Log( "LockersManager::UseLocker item<%u> pid<%u> critter<%u> : close door\n", locker.Id, locker.Proto->ProtoId, cr.Id );
            LockerClose( locker );
        }
        else
        {
            if( Debug )
                Log( "LockersManager::UseLocker item<%u> pid<%u> critter<%u> : open door\n", locker.Id, locker.Proto->ProtoId, cr.Id );
            LockerOpen( locker );
        }
    }
    else if( locker.IsContainer() )
    {
        uint8 transferType = locker.Proto->GroundLevel ? TRANSFER_HEX_CONT_DOWN : TRANSFER_HEX_CONT_UP;

        if( locker.Proto->Container_Changeble )
        {
            if( FLAG( locker.Data.LockerCondition, LOCKER_ISOPEN ) )
            {
                if( Debug )
                    Log( "LockersManager::UseLocker item<%u> pid<%u> critter<%u> : close container\n", locker.Id, locker.Proto->ProtoId, cr.Id );
                LockerClose( locker );
            }
            else
            {
                if( Debug )
                    Log( "LockersManager::UseLocker item<%u> pid<%u> critter<%u> : open container\n", locker.Id, locker.Proto->ProtoId, cr.Id );
                LockerOpen( locker );
                ShowContainer( cr, nullptr, &locker, transferType );
            }
        }
        else
        {
            if( Debug )
                Log( "LockersManager::UseLocker item<%u> pid<%u> critter<%u> : view container\n", locker.Id, locker.Proto->ProtoId, cr.Id );
            ShowContainer( cr, nullptr, &locker, transferType );
        }
    }

    return true;
}

bool LockersManager::LockerOpen( Item& locker )
{
    assert( locker.IsDoor() || locker.IsContainer() );
    assert( locker.Proto->Container_Changeble );

    static const char* codeLockerOpen = "return GetItem(%u).LockerOpen();";
    char               code[50];
    bool               result = false;

    sprintf( code, codeLockerOpen, locker.Id );
    ExecuteString( ASEngine, code, &result, asTYPEID_BOOL, 0, 0 );

    return result;
}

bool LockersManager::LockerClose( Item& locker )
{
    assert( locker.IsDoor() || locker.IsContainer() );
    assert( locker.Proto->Container_Changeble );

    static const char* codeLockerClose = "return GetItem(%u).LockerClose();";
    char               code[50];
    bool               result = false;

    sprintf( code, codeLockerClose, locker.Id );
    ExecuteString( ASEngine, code, &result, asTYPEID_BOOL, 0, 0 );

    return result;
}

void LockersManager::ShowContainer( Critter& cr, Critter* crCont, Item* itemCont, uint8 transferType )
{
    assert( !cr.CritterIsNpc );

    static const char* codeShowContainer =
        "uint id = %u;"
        "GetCritter(%u).ShowContainer(GetCritter(%u),(id > 0 ? GetItem(id) : null),%u)";
    char code[100];

    sprintf( code, codeShowContainer, itemCont ? itemCont->Id : 0, cr.Id, crCont ? crCont->Id : 0,  transferType );
    ExecuteString( ASEngine, code );
}
