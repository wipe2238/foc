#include <App.h>
#include <Defines.Public.h>
#include <GameOptions.h>
#include <Ini.h>

#include "PGUI.Core.h"
#include "PGUI.Screen.h"

PGUI::Screen::Screen( PGUI::Core* gui, uint width /* = 0 */, uint height /* = 0 */, int left /* = 0 */, int top /* = 0 */ ) : PGUI::Element( gui, width, height, left, top ),
// public
    IsMovable( true ),
    Layer( 0 ),
    ScreenData( nullptr ),
// protected
    DrawLayer( 0 )
{}

PGUI::Screen::~Screen()
{
    if( GUI->Debug )
        App.WriteLogF( _FUNC_, "\n" );
}

uint PGUI::Screen::GetID()
{
    return GUI->GetScreenID( this );
}

bool PGUI::Screen::LoadSettings( Ini* ini, const std::string& section )
{
    if( !ini )
    {
        // if (Debug)
        App.WriteLogF( _FUNC_, " WARNING : ini is null\n", section.c_str() );

        return false;
    }

    if( ini->IsSection( section ) )
    {
        // if (Debug)
        App.WriteLogF( _FUNC_, " WARNING : section<%s> not found\n", section.c_str() );

        return false;
    }

    return true;
}

void PGUI::Screen::AutoSize()
{
    uint16 width = 0, height = 0;

    for( auto it = Elements.begin(), end = Elements.end(); it != end; ++it )
    {
        PGUI::Element* element = it->second;

        if( !element )
            continue;

        uint16 w = element->GetWidth() + element->GetLeft( true );
        uint16 h = element->GetHeight() + element->GetTop( true );

        width = std::max( width, w );
        height = std::max( height, h );
    }

    if( GUI->Debug )
        App.WriteLogF( _FUNC_, " = %ux%u\n", width, height );

    SetSize( width, height );
}

void PGUI::Screen::MouseMove( int16 fromX, int16 fromY, int16 toX, int16 toY )
{
    if( !IsMouseEnabled )
        return;

    if( IsMovable && MousePressed && MouseButton == MOUSE_CLICK_LEFT )
    {
        int16 oldLeft = 0, oldTop = 0;
        GetPosition( oldLeft, oldTop, true );

        int  newLeft = oldLeft + (toX - fromX);
        int  newTop = oldTop + (toY - fromY);

        bool reset = false;

        if( newLeft < 0 )
        {
            newLeft = 0;
            reset = true;
        }
        if( newTop < 0 )
        {
            newTop = 0;
            reset = true;
        }

        uint16 width = 0, height = 0;
        GetSize( width, height );

        if( newLeft + width > GUI->GetScreenWidth() )
        {
            newLeft = GUI->GetScreenWidth() - width;
            reset = true;
        }
        if( newTop + height > GUI->GetScreenHeight() )
        {
            newTop = GUI->GetScreenHeight() - height;
            reset = true;
        }

        if( reset )
        {
            // TODO cursor goes crazy here

            // GameOpt.MouseX = fromX;
            // GameOpt.MouseY = fromY;
        }

        if( oldLeft != newLeft || oldTop != newTop )
            SetPosition( newLeft, newTop );
    }

    PGUI::Element::MouseMove( fromX, fromY, toX, toY );
}
