#include <App.h>
#include <Defines.Public.h>
#include <Crypt.h>
#include <GameOptions.h>
#include <Keyboard.h>

#include "Client/Screen/Login.h"

#include "PGUI/PGUI.Core.h"
#include "PGUI/PGUI.Screen.h"

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

    SetSize( 270, 70 );
    SetPositionAt( 0, 0 );

    SetBackgroundVisible( true );
    SetBorderVisible( true );

    for( uint8 id = ID::UserName; id <= ID::UserPass; id++ )
    {
        PGUI::TextBox* box = new PGUI::TextBox( gui );

        box->IsKeyboardEnabled = false;
        box->IsDrawCursorEnabled = false;
        box->SetText( std::string( MAX_NAME, 'w' ) );
        box->AutoSize();
        box->SetText( "" );
        box->SetPosition( 10, 10 );

        if( id == ID::UserPass )
            box->SetTextMask( "*" );

        AddElement( id, box );
    }

    GetElement( ID::UserPass )->SetTop( GetElement( ID::UserPass )->GetBottom( true ) + GUI->Settings.TextBoxMargin * 2 );
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
            GameOpt.Quit = true;

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

    if( GUI->IsKeyPressed( DIK_LMENU ) && GUI->IsKeyPressed( DIK_R ) )
    {
        FOClient::Self->ShowMainScreen( CLIENT_MAIN_SCREEN_REGISTRATION );
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

    GameOpt.HideCursor = true;
}

void FOC::Screen::Login::OnClose()
{
    GetElementAs<PGUI::TextBox>( ID::UserPass )->SetText( "" );

    GameOpt.HideCursor = false;
}

void FOC::Screen::Login::OnTop( bool top )
{
    SetBorderThickness( top ? 2 : 1 );
}
