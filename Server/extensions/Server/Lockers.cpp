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

    if( !cr.MapId )
    {
        // TODO STR_USE_NOTHING
        return true;
    }

    if( skill == SKILL_PICK_ON_GROUND )
    {
        return UseLocker( cr, *itemTarget, skill );
    }

    return true;
}

//

bool LockersManager::UseLocker( Critter& cr, Item& locker, int skill )
{
    if( Debug )
        Log( "LockersManager::UseLocker(cr=%u, skill=%d, locker=%u)\n", cr.Id, skill, locker.Id );

    if( locker.GetType() == ITEM_TYPE_DOOR )
    {
        if( !locker.Proto->Container_Changeble )
        {
            if( Debug )
                Log( "LockersManager::UseLocker item<%u> pid<%u> critter<%u> : container not changeable\n", locker.Id, locker.Proto->ProtoId, cr.Id );

            // TODO STR_USE_NOTHING
            return true;
        }

        bool result = false;
        if( FLAG( locker.Data.LockerCondition, LOCKER_ISOPEN ) )
        {
            if( Debug )
                Log( "LockersManager::UseLocker item<%u> pid<%u> critter<%u> : close\n", locker.Id, locker.Proto->ProtoId, cr.Id );
            result = LockerClose( locker );
        }
        else
        {
            if( Debug )
                Log( "LockersManager::UseLocker item<%u> pid<%u> critter<%u> : open\n", locker.Id, locker.Proto->ProtoId, cr.Id );
            result = LockerOpen( locker );
        }

        if( Debug )
            Log( "LockersManager::UseLocker item<%u> pid<%u> critter<%u> : open/close result = %s\n", locker.Id, locker.Proto->ProtoId, cr.Id, result ? "true" : "false" );
    }

    return true;
}

bool LockersManager::LockerOpen( Item& locker )
{
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
    assert( locker.Proto->Container_Changeble );

    static const char* codeLockerClose = "return GetItem(%u).LockerClose();";
    char               code[50];
    bool               result = false;

    sprintf( code, codeLockerClose, locker.Id );
    ExecuteString( ASEngine, code, &result, asTYPEID_BOOL, 0, 0 );

    return result;
}
