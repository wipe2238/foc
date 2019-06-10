#ifndef __FOC__HEX_2D__
#define __FOC__HEX_2D__

#include <Types.h>

namespace FOC
{
    void Hex2D( uint16& hexX, uint16& hexY, uint16 mapWidth );
    void Hex2D( uint16 hexX, uint16 hexY, uint16& x, uint16& y, uint16 mapWidth );
}

#endif // __FOC__HEX_2D__ //
