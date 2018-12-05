#ifndef __LOCKERS__
#define __LOCKERS__

#include <extension.h>

class LockersManager
{
public:
    bool Debug;

    LockersManager();

    bool ProcessUseSkill( Critter& cr, int skill,  Item* itemTarget );
    //
    bool UseLocker( Critter& cr, Item& locker, int skill );
    //
    bool LockerOpen( Item& locker );
    bool LockerClose( Item& locker );
};

extern LockersManager Lockers;

#endif // __LOCKERS__ //
