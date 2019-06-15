#include "Client/Screen/Info.h"

FOC::Screen::Info::Info( PGUI::Core* gui ) : PGUI::Screen( gui )
{
    Layer = 3;
    // SetBackgroundVisible( true );
    SetBorderVisible( true );

    SetSize( 200, 300 );
    SetPositionAt( 0, 0 );
}

FOC::Screen::Info::~Info()
{}
