#ifndef __CLIENT_INTERFACE__
#define __CLIENT_INTERFACE__

#include <extension.h>

class InterfaceManager
{
private:
    set<int> Screens;

public:
    // reserved functions processing
    void ProcessScreenChange( bool show, int screen, int p0, int p1, int p2 );
    void ProcessGetActiveScreens( ScriptArray& screens );
    void ProcessRenderInterface( uint layer );

public:
    // script wrappers
    void DrawHardcodedScreen( int screen );
};

extern InterfaceManager Interface;

#endif // __CLIENT_INTERFACE__ //
