#ifndef __FOC__LOCKERS__
#define __FOC__LOCKERS__

#include <map>

#include <Types.h>

class Critter;
class Item;
class Map;

namespace FOC
{
    class LockersManager
    {
public:
        bool Debug;
        uint AutoCloseTime;

        LockersManager();
        ~LockersManager();

        void Init();

        bool UseLocker( Critter* cr, Item* locker, int skill );
        Map* GetMapHexOf( Item* locker, uint16& hexX, uint16& hexY );

        void Open( Item* door, Critter* cr = nullptr, int transferType = -1, uint delay = 0 );
        void Close( Item* door, Critter* cr = nullptr, uint delay = 0 );
        uint OnOpenClose( ScriptArray* values );         // timevent

private:
        void OnOpen( Item* locker, Critter* cr = nullptr );
        void OnClose( Item* locker, Critter* cr = nullptr );
        void OnShowContainer( Item* locker, Critter* cr, uint8 transferType );
    };

};

#endif // __FOC__LOCKERS__ //
