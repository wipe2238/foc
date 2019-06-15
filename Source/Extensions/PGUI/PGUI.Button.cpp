#include <App.h>
#include <Defines.Public.h>

#include "PGUI.Button.h"
#include "PGUI.Core.h"
#include "PGUI.Draw.h"
#include "PGUI.Label.h"

PGUI::Button::Button( PGUI::Core* gui ) : PGUI::Element( gui ),
// protected
    IsHover( false ),
    BackgroundHoverCache( nullptr )
{
    IsKeyboardEnabled = false;
    SetBackgroundVisible( true );
    SetBorderVisible( true );

    AddElement( 1, new PGUI::Label( gui ) );
}

PGUI::Button::~Button()
{
    PGUI::Draw::DeleteCache( BackgroundHoverCache );
}

PGUI::Label* PGUI::Button::GetLabel()
{
    return GetElementAs<PGUI::Label>( 1 );
}

void PGUI::Button::SetText( const std::string& text )
{
    PGUI::Label* label = GetElementAs<PGUI::Label>( 1 );

    if( label )
    {
        label->SetText( text );
        label->AutoSize();
        label->SetPositionAt( 0, 0 );
    }
}

void PGUI::Button::UpdateDecorations()
{
    if( !IsUpdateEnabled )
        return;

    if( GUI->Debug )
        App.WriteLogF( _FUNC_, "\n" );

    Draw::DeleteCache( BackgroundCache );
    Draw::DeleteCache( BackgroundHoverCache );
    Draw::DeleteCache( BorderCache );

    DrawData data;

    if( GetBackgroundVisible() )
    {
        data.MakeRectangle( 0, 0, GetWidth(), GetHeight(), GUI->Settings.ColorBackgroundButton );
        BackgroundCache = data.NewCache();

        data.MakeRectangle( 0, 0, GetWidth(), GetHeight(), GUI->Settings.ColorBackgroundButtonHover );
        BackgroundHoverCache = data.NewCache();
    }

    if( GetBorderVisible() )
    {
        data.MakeRectangleFrame( 0, 0, GetWidth(), GetHeight(), GUI->Settings.ColorBorder, GetBorderThickness() );
        BorderCache = data.NewCache();
    }

    NeedUpdateDecorations = false;
}

void PGUI::Button::DrawBack()
{
    if( !IsDrawEnabled )
        return;

    if( GetBackgroundVisible() )
    {
        Draw::RenderData( IsHover ? BackgroundHoverCache : BackgroundCache, GetLeft(), GetTop() );
    }
}

void PGUI::Button::MouseMove( int16 fromX, int16 fromY, int16 toX, int16 toY )
{
    if( !IsMouseEnabled )
        return;

    IsHover = IsInside( toX, toY );

    PGUI::Element::MouseMove( fromX, fromY, toX, toY );
}

void PGUI::Button::MouseClick( uint8 button )
{
    if( !IsMouseEnabled )
        return;

    if( button == MOUSE_CLICK_LEFT || button == MOUSE_CLICK_RIGHT )
    {
        if( EventMouseClick )
            EventMouseClick( GetID(), this );
    }
}
