#include <App.h>
#include <Defines.Public.h>
#include <Crypt.h>
#include <GameOptions.h>
#include <Keyboard.h>

#include "Client/Screen/Login.h"

#include "PGUI/PGUI.Button.h"
#include "PGUI/PGUI.Core.h"
#include "PGUI/PGUI.Screen.h"
#include "PGUI/PGUI.TextBox.h"

// Client.h cannot be easily included at the moment
class FOClient
{
public:
    static FOClient* Self;
    uint8            Dummy[8];
    std::string      Password;

    void LogTryConnect();
    void ShowMainScreen( int new_screen );
};

static void StaticAsserts()
{
    STATIC_ASSERT( offsetof( FOClient, Password ) == 8 );
}

FOC::Screen::Login::Login( PGUI::Core* gui ) : PGUI::Screen( gui ),
// private
    ActiveElement( 0 )
{
    Layer = 3;
    IsMovable = false;

    SetBackgroundVisible( true );
    SetBorderVisible( true );

    const uint8 offset = 10;

    for( uint8 id = ID::UserName; id <= ID::ButtonExit; id++ )
    {
        if( id <= ID::UserPass )
        {
            PGUI::TextBox* box = new PGUI::TextBox( gui );

            box->IsKeyboardEnabled = false;
            box->IsDrawCursorEnabled = false;
            box->SetText( std::string( MAX_NAME, 'w' ) );
            box->AutoSize();
            box->SetText( "" );
            box->SetPosition( offset, offset );

            if( id == ID::UserPass )
                box->SetTextMask( "*" );

            AddElement( id, box );
        }
        else if( id <= ID::ButtonExit )
        {
            PGUI::Button* button = new PGUI::Button( gui );

            button->SetSize( GetElement( ID::UserName ) );
            button->SetLeft( offset );

            AddElement( id, button );
        }
    }

    GetElement( ID::UserPass )->SetTop( GetElement( ID::UserName )->GetBottom( true ) + GUI->Settings.TextBoxMargin * 2 );
    GetElement( ID::ButtonRegister )->SetTop( GetElement( ID::UserPass )->GetBottom( true ) + GUI->Settings.TextBoxMargin * 2 );
    GetElement( ID::ButtonExit )->SetTop( GetElement( ID::ButtonRegister )->GetBottom( true ) + GUI->Settings.TextBoxMargin * 2 );

    SetSize( GetElement( ID::UserName )->GetWidth() + offset * 2, GetElement( ID::ButtonExit )->GetBottom( true ) + offset );
    SetPositionAt( 0, 0 );

    GetElementAs<PGUI::Button>( ID::ButtonRegister )->SetText( "Registration" );
    GetElementAs<PGUI::Button>( ID::ButtonExit )->SetText( "Exit" );

    for( uint id = ID::ButtonRegister; id <= ID::ButtonExit; id++ )
    {
        GetElementAs<PGUI::Button>( id )->EventMouseClick = std::bind( &FOC::Screen::Login::OnButton, this, std::placeholders::_1, std::placeholders::_2 );
    }
}

//

bool FOC::Screen::Login::KeyDown( uint8 key, std::string& keyText )
{
    if( !IsKeyboardEnabled )
        return false;

    if( GUI->Debug )
        App.WriteLogF( _FUNC_, "(%u,\"%s\")\n", key, keyText.c_str() );

    if( key == DIK_ESCAPE )
    {
        if( GUI->IsKeyPressed( DIK_LSHIFT ) )
            OnButton( ID::ButtonExit, nullptr );

        return true;
    }
    else if( key == DIK_TAB )
    {
        PGUI::TextBox* element;

        if( ActiveElement )
        {
            element = GetElementAs<PGUI::TextBox>( ActiveElement );
            // cursor still will be drawn on unselected element for a short time; not a bug
            element->IsDrawCursorEnabled = false;
            element->IsKeyboardEnabled = false;
            element->SetBorderThickness( 1 );
        }

        ActiveElement++;

        if( ActiveElement > ID::UserPass )
            ActiveElement = ID::UserName;

        element = GetElementAs<PGUI::TextBox>( ActiveElement );
        element->IsDrawCursorEnabled = true;
        element->IsKeyboardEnabled = true;
        element->SetBorderThickness( 2 );

        return true;
    }
    else if( key == DIK_RETURN || key == DIK_NUMPADENTER )
    {
        // copy login data where engine expects it
        GameOpt.Name = GetElementAs<PGUI::TextBox>( ID::UserName )->GetText();
        FOClient::Self->Password = GetElementAs<PGUI::TextBox>( ID::UserPass )->GetText();

        FOClient::Self->LogTryConnect();

        return true;
    }

    if( PGUI::Screen::KeyDown( key, keyText ) )
        return true;

    return false;
}

//

void FOC::Screen::Login::OnOpen( bool& modal )
{
    PGUI::TextBox* username = GetElementAs<PGUI::TextBox>( ID::UserName );
    PGUI::TextBox* userpass = GetElementAs<PGUI::TextBox>( ID::UserPass );

    username->SetText( GameOpt.Name.c_std_str() );
    userpass->SetText( "" );

    // autofocus
    PGUI::TextBox* focus = username->GetText().length() ? userpass : username;
    focus->IsKeyboardEnabled = true;
    focus->IsDrawCursorEnabled = true;
    focus->SetBorderThickness( 2 );

    ActiveElement = focus == username ? ID::UserName : ID::UserPass;
}

void FOC::Screen::Login::OnClose()
{
    GetElementAs<PGUI::TextBox>( ID::UserPass )->SetText( "" );
}

void FOC::Screen::Login::OnTop( bool top )
{
    SetBorderThickness( top ? 2 : 1 );
}

void FOC::Screen::Login::OnButton( uint id, PGUI::Button* button )
{
    if( !id )
    {}
    else if( id == ID::ButtonRegister )
        FOClient::Self->ShowMainScreen( CLIENT_MAIN_SCREEN_REGISTRATION );
    else if( id == ID::ButtonExit )
        GameOpt.Quit = true;
}
