#ifndef __FOC__AP_COST__
#define __FOC__AP_COST__

#include "Shared/CritterMutual.h"

namespace FOC
{
    struct ApCost
    {
        static uint UseItem( CritterMutual& cr, Item& item, uint8 use );
    };
};

#endif // __FOC__AP_COST__ //
