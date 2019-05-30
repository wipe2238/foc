#include <Log.h>

#include "Shared/MoveItem.h"

bool FOC::MoveItem::Check( CritterMutual& cr, Item& item, uint8 toSlot, Item* itemSwap )
{
    WriteLogF( _FUNC_, " item<%u> toSlot<%u> itemSwap<%u>n", item.Id, toSlot, itemSwap ? itemSwap->GetId() : 0 );

    return true;
}

#if defined (FOCLASSIC_SERVER)

void FOC::MoveItem::Move( Critter& cr, Item& item, uint8 fromSlot )
{
    WriteLogF( _FUNC_, "critter<%u> item<%u> toSlot<%u>\n", cr.GetId(), item.GetId(), fromSlot );
}

#endif
