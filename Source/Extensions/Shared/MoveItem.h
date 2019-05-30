#ifndef __FOC__MOVE_ITEM__
#define __FOC__MOVE_ITEM__

#include <Item.h>

#include "Shared/CritterMutual.h"

namespace FOC
{
    struct MoveItem
    {
        static bool Check( CritterMutual& cr, Item& item, uint8 toSlot, Item* itemSwap );
        #if defined (FOCLASSIC_SERVER)
        static void Move( Critter& cr, Item& item, uint8 fromSlot );
        #endif
    };
};

#endif // __FOC__MOVE_ITEM__ //
