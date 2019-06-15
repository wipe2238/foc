#include <App.h>
#include <Timer.h>

#include "PGUI.Core.h"
#include "PGUI.Draw.h"
#include "PGUI.Keyboard.h"
#include "PGUI.TextBox.h"

PGUI::TextBox::TextBox( PGUI::Core* gui ) : PGUI::Element( gui ),
// protected
    Cursor( "" ),
    IsDrawCursorEnabled( false ),
    CursorList(),
    CursorTime( 1000 ),
    CursorChanged( 0 ),
    CursorPosition( 0 ),
    Text( "" ), TextMask( "" ), TextToDraw( "" )
{
    SetBorderVisible( true );

    CursorList.push_back( "!" );
    CursorList.push_back( "." );

    UpdateDrawText();
}

PGUI::TextBox::~TextBox()
{
    if( GUI->Debug )
        App.WriteLogF( _FUNC_, "\n" );
}

void PGUI::TextBox::AutoSize()
{
    uint16 width = 0, height = 0;
    uint   lines = 0;

    Draw::GetTextInfo( Text, GUI->Settings.FontType, GUI->Settings.FontFlags, width, height, lines );

    SetSize( width + GUI->Settings.TextBoxMargin * 2, height + GUI->Settings.TextBoxMargin * 2 );

    if( GUI->Debug )
        App.WriteLogF( _FUNC_, " = %ux%u\n", GetWidth(), GetHeight() );

}

std::string PGUI::TextBox::GetText()
{
    return Text;
}

void PGUI::TextBox::SetText( const std::string& text )
{
    Text = text;

    CursorPosition = Text.length();

    UpdateDrawText();
}

void PGUI::TextBox::AppendText( const std::string& text )
{
    SetText( Text + text );
}

void PGUI::TextBox::SetTextMask( const std::string& mask )
{
    TextMask = mask;

    UpdateDrawText();
}


void PGUI::TextBox::Update()
{
    if( !IsUpdateEnabled )
        return;

    uint tick = Timer::FastTick();

    if( tick - CursorChanged > CursorTime )
    {
        CursorChanged = tick;

        if( IsDrawCursorEnabled )
        {
            Cursor = CursorList.front();
            CursorList.pop_front();
            CursorList.push_back( Cursor );
        }
        else
            Cursor = "";

        UpdateDrawText();
    }

    PGUI::Element::Update();
}

void PGUI::TextBox::UpdateDrawText()
{
    if( !IsUpdateEnabled )
        return;

    std::string text = GetText();

    if( TextMask.length() == 1 )
        text = std::string( text.length(), TextMask.front() );
    else if( TextMask.length() >= 2 )
    {
        TextToDraw = TextMask;
        return;
    }

    if( CursorPosition == text.length() )
        TextToDraw = text + Cursor;
    else
        TextToDraw = text.substr( 0, CursorPosition ) + Cursor + text.substr( CursorPosition, text.length() - CursorPosition );
}

void PGUI::TextBox::DrawContent()
{
    if( !IsDrawEnabled )
        return;

    Draw::RenderText( TextToDraw, GUI->Settings.ColorFont, GUI->Settings.FontType, GUI->Settings.FontFlags, GetLeft() + GUI->Settings.TextBoxMargin, GetTop() + GUI->Settings.TextBoxMargin, Width, Height );
}

bool PGUI::TextBox::KeyDown( uint8 key, std::string& keyText )
{
    if( !IsKeyboardEnabled )
        return false;

    if( GUI->Debug )
        App.WriteLogF( _FUNC_, "(%u,%s)\n", key, keyText.c_str() );

    switch( key )
    {
        case PGUI::Keyboard::KEY_BACKSPACE:
        {
            if( Text.length() && CursorPosition )
            {
                Text = Text.substr( 0, CursorPosition - 1 ) + Text.substr( CursorPosition, Text.length() - CursorPosition );
                CursorPosition--;
            }

            break;
        }
        case PGUI::Keyboard::KEY_DELETE:
        {
            if( Text.length() && CursorPosition < Text.length() )
                Text = Text.substr( 0, CursorPosition ) + Text.substr( CursorPosition + 1, Text.length() - CursorPosition - 1 );

            break;
        }
        case PGUI::Keyboard::KEY_HOME:
        {
            CursorPosition = 0;

            break;
        }
        case PGUI::Keyboard::KEY_END:
        {
            CursorPosition = Text.length();

            break;
        }
        case PGUI::Keyboard::KEY_LEFT:
        {
            if( CursorPosition )
                CursorPosition--;

            break;
        }
        case PGUI::Keyboard::KEY_RIGHT:
        {
            if( CursorPosition < Text.length() )
                CursorPosition++;

            break;
        }
        // case PGUI::Keyboard::KEY_ESCAPE:
        // {
        //    Focus = false;
        //    break;
        // }
        default:
        {
            // if (this.maxLength > 0 && this.maxLength > this.label.text.length())
            //  break;

            // if (this.onlyNumbers && !PGUI::Keyboard::IsNumber(key) )
            //  break;

            if( !keyText.empty() )
            {
                Text += keyText;
                CursorPosition++;
            }
            break;
        }
    }

    UpdateDrawText();

    return true;
}
