#ifndef __FOC__LOCKERS__
#define __FOC__LOCKERS__

#include <Critter.h>
#include <Extension.h>
#include <Item.h>
#include <Types.h>

namespace FOC
{
    class LockersManager
    {
public:
        bool Debug;

        LockersManager();
        ~LockersManager();

        bool ProcessUseSkill( Critter& cr, int skill, Item* itemTarget );
        //
        bool UseLocker( Critter& cr, Item* locker );
        void UseLockerAction( Item* locker, bool open, uint delay = 0 );
    };
};

#endif // __FOC__LOCKERS__ //
