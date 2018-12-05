#include <ExecuteString.h>

#include "Interface.h"

InterfaceManager Interface;

void InterfaceManager::ShowScreen( int screen, int p0, int p1, int p2 )
{
    Screens.insert( screen );
}

void InterfaceManager::HideScreen( int screen, int p0, int p1, int p2 )
{
    Screens.erase( screen );
}

void InterfaceManager::GetActiveScreens( ScriptArray& screens )
{
    screens.Resize( Screens.size() );

    set<int>::iterator it = Screens.begin();
    for( int s = 0, sLen = Screens.size(); s < sLen; s++, ++it )
    {
        int val = *it;

        *(int*)screens.At( s ) = val;
    }
}

void InterfaceManager::RenderInterface( uint layer )
{
    if( layer != 3 )
        return;

    for( auto it = Screens.begin(); it != Screens.end(); ++it )
    {
        int  val = *it;

        char code[50];
        sprintf( code, "DrawHardcodedScreen(%d);", val );
        // ASEngine->WriteMessage("", 0, 0, asMSGTYPE_INFORMATION, code );
        ExecuteString( ASEngine, code );

    }
}
