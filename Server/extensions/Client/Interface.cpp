#include <ExecuteString.h>

#include "Interface.h"

InterfaceManager Interface;

void InterfaceManager::ProcessScreenChange( bool show, int screen, int p0, int p1, int p2 )
{
    if( show )
        Screens.insert( screen );
    else
        Screens.erase( screen );
}

void InterfaceManager::ProcessGetActiveScreens( ScriptArray& screens )
{
    screens.Resize( Screens.size() );

    set<int>::iterator it = Screens.begin();
    for( int s = 0, sLen = Screens.size(); s < sLen; s++, ++it )
    {
        int val = *it;

        *(int*)screens.At( s ) = val;
    }
}

void InterfaceManager::ProcessRenderInterface( uint layer )
{
    if( layer != 2 )
        return;

    for( auto it = Screens.begin(); it != Screens.end(); ++it )
    {
        int val = *it;

        DrawHardcodedScreen( val );
    }
}

void InterfaceManager::DrawHardcodedScreen( int screen )
{
    static const char* codeDrawHardcodedScreen = "DrawHardcodedScreen(%d);";
    char               code[50];

    sprintf( code, codeDrawHardcodedScreen, screen );
    ExecuteString( ASEngine, code );
}
