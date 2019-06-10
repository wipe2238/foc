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
    uint16           Dummy0;
    bool             Dummy1;
    uint*            Dummy2;
    std::string      Password;

    void LogTryConnect();
    void ShowMainScreen( int new_screen );
};

FOC::Screen::Login::Login( PGUI::Core* gui ) : PGUI::Screen( gui ),
// private
    ActiveElement( 0 )
{
    Layer = 3;
    SetSize( 270, 70 );
    SetPositionAt( 0, 0 );

    SetBackgroundVisible( true );
    SetBorderVisible( true );

    for( uint8 id = ID::UserName; id <= ID::UserPass; id++ )
    {
        PGUI::TextBox* box = new PGUI::TextBox( gui );

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

bool FOC::Screen::Login::KeyDown( const uint8& key, const std::string& keyString )
{
    if( !IsKeyboardActive )
        return false;

    if( key == DIK_ESCAPE )
    {
        if( GUI->IsKeyPressed( DIK_LSHIFT ) )
            GameOpt.Quit = true;

        return true;
    }
    else if( key == DIK_TAB )
    {
        if( ActiveElement )
        {
            PGUI::TextBox* element = GetElementType<PGUI::TextBox>( ActiveElement );
            element->CursorDraw = false;
            element->SetBorderThickness( 1 );
        }

        ActiveElement++;

        if( ActiveElement > ID::UserPass )
            ActiveElement = ID::UserName;

        PGUI::TextBox* element = GetElementType<PGUI::TextBox>( ActiveElement );
        element->CursorDraw = true;
        element->SetBorderThickness( 2 );

        return true;
    }
    else if( key == DIK_RETURN || key == DIK_NUMPADENTER )
    {
        GameOpt.Name = GetElementType<PGUI::TextBox>( ID::UserName )->GetText();
        FOClient::Self->Password = GetElementType<PGUI::TextBox>( ID::UserPass )->GetText();

        FOClient::Self->LogTryConnect();

        return true;
    }

    if( GUI->IsKeyPressed( DIK_LMENU ) && GUI->IsKeyPressed( DIK_R ) )
    {
        FOClient::Self->ShowMainScreen( CLIENT_MAIN_SCREEN_REGISTRATION );
        return true;
    }

    if( ActiveElement && IsElement( ActiveElement ) && GetElement( ActiveElement )->KeyDown( key, keyString ) )
        return true;

    return false;
}

//

void FOC::Screen::Login::OnOpen()
{
    GetElementType<PGUI::TextBox>( ID::UserName )->SetText( GameOpt.Name.c_std_str() );
    GetElementType<PGUI::TextBox>( ID::UserPass )->SetText( "" );
}

void FOC::Screen::Login::OnClose()
{
    GetElementType<PGUI::TextBox>( ID::UserPass )->SetText( "" );
}

void FOC::Screen::Login::OnActive( bool active )
{
    SetBorderThickness( active ? 2 : 1 );

    if( !ActiveElement )
        return;

    PGUI::TextBox* element = GetElementType<PGUI::TextBox>( ActiveElement );

    element->CursorDraw = false;
    element->SetBorderThickness( 1 );

    ActiveElement = 0;
}
