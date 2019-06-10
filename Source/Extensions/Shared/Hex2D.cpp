#include "Hex2D.h"

void FOC::Hex2D( uint16& hexX, uint16& hexY, uint16 mapWidth )
{
    const uint16 hx = hexX, hy = hexY;

    Hex2D( hx, hy, hexX, hexY, mapWidth );
}

void FOC::Hex2D( uint16 hexX, uint16 hexY, uint16& x2d, uint16& y2d, uint16 mapWidth )
{
    // simplified https://github.com/rotators/FOCommon/blob/master/Maps/FOHexMap.cs#L46-L61

    int x = hexY - hexX * 2;
    int y = hexY + hexX / 2;
    if( hexX > 1 )
        x += hexX / 2;
    x += mapWidth;

    x2d = x;
    y2d = y;
}
