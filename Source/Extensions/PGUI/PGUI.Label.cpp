#include "PGUI.Label.h"
#include <App.h>
#include <Defines.Public.h>

#include "PGUI.Core.h"
#include "PGUI.Draw.h"
#include "PGUI.Element.h"
#include "PGUI.Label.h"

PGUI::Label::Label( PGUI::Core* gui, const std::string& text /* = std::string() */ ) : PGUI::Element( gui ),
// protected
    Text( text )
{}

PGUI::Label::~Label()
{
    if( GUI->Debug )
        App.WriteLogF( _FUNC_, "\n" );
}

std::string PGUI::Label::GetText()
{
    return Text;
}

void PGUI::Label::SetText( const std::string& value )
{
    if( GetText() != value )
    {
        if( GUI->Debug )
            App.WriteLogF( _FUNC_, "(\"%s\")\n", value.c_str() );

        Text = value;
    }
}

void PGUI::Label::AppendText( const std::string& value )
{
    SetText( GetText() + value );
}

void PGUI::Label::AutoSize()
{
    uint16 width = 0, height = 0;
    uint   lines = 0;
    Draw::GetTextInfo( Text, GUI->Settings.FontType, GUI->Settings.FontFlags, width, height, lines );

    SetSize( width, height );
}

//

void PGUI::Label::UpdateDecorations()
{
    if( GUI->Debug )
        App.WriteLogF( _FUNC_, "\n" );

    AutoSize();

    PGUI::Element::UpdateDecorations();
}

//

void PGUI::Label::DrawContent()
{
    if( !IsVisible )
        return;

    Draw::RenderText( Text, GUI->Settings.ColorFont, GUI->Settings.FontType, GUI->Settings.FontFlags, GetLeft(), GetTop(), Width, Height );
}
