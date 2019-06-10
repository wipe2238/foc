#include <climits>

#include <App.h>
#include <Defines.Public.h>
#include <GameOptions.h>
#include <Ini.h>

#include "PGUI.Core.h"
#include "PGUI.Keyboard.h"
#include "PGUI.Screen.h"

PGUI::CoreSettings::CoreSettings() :
// public
    ColorBackground( COLOR_ARGB( 100, 0, 255, 0 ) ),
    ColorBorder( COLOR_ARGB( 255, 0, 255, 0 ) ),
    ColorFont( COLOR_ARGB( 255, 0, 255, 0 ) ),
    ColorItem( COLOR_ARGB( 222, 0, 222, 0 ) ),
    FontType( FONT_TYPE_DEFAULT ),
    FontFlags( FONT_FLAG_BORDERED ),
    TextBoxMargin( 5 )
{}

PGUI::Core::Core( bool visible /* = true */ ) :
// public
    Debug( false ), DebugDraw( false ),
    IsVisible( visible ), IsUpdating( true ), IsKeyboardActive( true ), IsMouseActive( true ),
// private
    AllScreens(), OpenScreens(), ActiveScreen( nullptr ),
    KeyboardPressed(), MousePressed()
{}

PGUI::Core::~Core()
{
    if( Debug )
        App.WriteLogF( _FUNC_, "\n" );

    RemoveAllScreens();
}

int16 PGUI::Core::GetScreenWidth()
{
    return CLAMP( GameOpt.ScreenWidth, SHRT_MIN, SHRT_MAX );
}

int16 PGUI::Core::GetScreenHeight()
{
    return CLAMP( GameOpt.ScreenHeight, SHRT_MIN, SHRT_MAX );
}

void PGUI::Core::GetScreenSize( int16& width, int16& height )
{
    width = GetScreenWidth();
    height = GetScreenHeight();
}

bool PGUI::Core::LoadSettings( Ini* ini, const std::string& section )
{
    if( !ini )
    {
        // if (Debug)
        App.WriteLogF( _FUNC_, " ERROR : ini is null\n", section.c_str() );

        return false;
    }

    if( ini->IsSection( section ) )
    {
        // if (Debug)
        App.WriteLogF( _FUNC_, " ERROR : section<%s> not found\n", section.c_str() );

        return false;
    }

    Settings.ColorBackground = ini->GetInt( section, "ColorBackground", Settings.ColorBackground );
    Settings.ColorBorder = ini->GetInt( section, "ColorBorder", Settings.ColorBorder );
    Settings.ColorFont = ini->GetInt( section, "ColorFont", Settings.ColorFont );

    Settings.FontType = ini->GetInt( section, "FontType", Settings.FontType );
    Settings.FontFlags = ini->GetInt( section, "FontFlags", Settings.FontFlags );

    return true;
}

bool PGUI::Core::IsScreen( uint id )
{
    return AllScreens.find( id ) != AllScreens.end();
}

bool PGUI::Core::IsScreenOpen( uint id )
{
    return std::find( OpenScreens.begin(), OpenScreens.end(), id ) != OpenScreens.end();
}

//

PGUI::Screen* PGUI::Core::NewScreen( uint id, bool add /* = true */ )
{
    PGUI::Screen* screen = new PGUI::Screen( this );

    if( AddScreen( id, screen ) )
        return screen;

    delete screen;
    return nullptr;
}

//

bool PGUI::Core::AddScreen( uint id, PGUI::Screen* screen )
{
    if( Debug )
        App.WriteLogF( _FUNC_, "(%u)\n", id );

    bool is = IsScreen( id );
    bool isOpen = IsScreenOpen( id );

    if( id && screen && !is && !isOpen )
    {
        screen->SetGUI( this );
        AllScreens[id] = screen;

        if( EventScreenAdd )
        {
            if( Debug )
                App.WriteLogF( _FUNC_, "(%u) : EventScreenAdd()\n", id );

            EventScreenAdd( id, screen );
        }
        return true;
    }

    if( Debug )
    {
        if( !id )
            App.WriteLogF( _FUNC_, "(%u) ERROR : invalid id\n", id );
        if( is )
            App.WriteLogF( _FUNC_, "(%u) ERROR : ID already used\n", id );
        if( isOpen )
            App.WriteLogF( _FUNC_, "(%u) ERROR : ID already opened\n", id );
    }

    return false;
}

bool PGUI::Core::RemoveScreen( uint id )
{
    if( Debug )
        App.WriteLogF( _FUNC_, "(%u)\n", id );

    PGUI::Screen* screen = GetScreen( id );
    if( !screen )
    {
        if( Debug )
            App.WriteLogF( _FUNC_, "(%u) ERROR : invalid id\n", id );

        return false;
    }

    if( IsScreenOpen( id ) )
        CloseScreen( id );

    AllScreens.erase( id );

    if( EventScreenRemove )
    {
        if( Debug )
            App.WriteLogF( _FUNC_, "(%u) EventScreenRemove()\n", id );

        EventScreenRemove( id, screen );
    }

    screen->SetGUI( nullptr );

    return true;
}

void PGUI::Core::RemoveAllScreens()
{
    if( Debug )
        App.WriteLogF( _FUNC_, "\n" );

    std::list<uint> screens = GetAllScreenList();

    for( auto it = screens.begin(), end = screens.end(); it != end; ++it )
    {
        RemoveScreen( *it );
    }
}

//

bool PGUI::Core::OpenScreen( uint id )
{
    if( Debug )
        App.WriteLogF( _FUNC_, "(%u)\n", id );

    PGUI::Screen* screen = GetScreen( id );
    if( !screen )
    {
        if( Debug )
            App.WriteLogF( _FUNC_, "(%u) ERROR : invalid screen id\n", id );

        return false;
    }

    if( IsScreenOpen( id ) )
    {
        // if (Debug)
        App.WriteLogF( _FUNC_, "(%u) WARNING : screen already opened\n", id );

        return true;
    }

    // allow open events to use OpenAllScreens()
    OpenScreens.push_back( id );

    if( EventScreenOpen )
    {
        if( Debug )
            App.WriteLogF( _FUNC_, "(%u) EventScreenOpen()\n", id );
        EventScreenOpen( id, screen );
    }

    if( Debug )
        App.WriteLogF( _FUNC_, "(%u) Screen::OnOpen()\n", id );

    screen->OnOpen();

    SetActiveScreen( screen );

    if( !screen->Layer )
        App.WriteLogF( _FUNC_, "(%u) WARNING : Screen::Layer not set\n", id );

    return true;
}

void PGUI::Core::OpenAllScreens()
{
    if( Debug )
        App.WriteLogF( _FUNC_, "\n" );

    std::list<uint> list = GetAllScreenList();

    for( auto it = list.begin(), end = list.end(); it != end; ++it )
    {
        uint id = *it;

        if( !IsScreenOpen( id ) )
            OpenScreen( id );
    }
}

bool PGUI::Core::CloseScreen( uint id )
{
    if( Debug )
        App.WriteLogF( _FUNC_, "(%u)\n", id );

    PGUI::Screen* screen = GetOpenScreen( id );
    if( !screen )
    {
        if( Debug )
            App.WriteLogF( _FUNC_, "(%u) : screen not opened\n", id );

        return false;
    }

    if( ActiveScreen == screen )
        SetActiveScreen( nullptr );

    // allow close events to use CloseAllScreens()
    OpenScreens.remove( id );

    if( EventScreenClose )
    {
        if( Debug )
            App.WriteLogF( _FUNC_, "(%u) EventScreenClose()\n", id );
        EventScreenClose( id, screen );
    }

    if( Debug )
        App.WriteLogF( _FUNC_, "(%u) Screen::OnClose()\n", id );
    screen->OnClose();

    return true;
}

void PGUI::Core::CloseAllScreens()
{
    if( Debug )
        App.WriteLogF( _FUNC_, "\n" );

    for( auto it = OpenScreens.begin(), end = OpenScreens.end(); it != end; ++it )
    {
        CloseScreen( *it );
    }
}

void PGUI::Core::ToggleScreen( uint id )
{
    if( !IsScreen( id ) )
        return;

    if( IsScreenOpen( id ) )
        CloseScreen( id );
    else
        OpenScreen( id );
}
//

PGUI::Screen* PGUI::Core::GetScreen( uint id )
{
    auto it = AllScreens.find( id );
    if( it != AllScreens.end() )
        return it->second;

    return nullptr;
}

uint PGUI::Core::GetScreenID( PGUI::Screen* screen )
{
    if( screen )
    {
        for( auto it = AllScreens.begin(), end = AllScreens.end(); it != end; ++it )
        {
            if( it->second == screen )
                return it->first;
        }
    }

    return 0;
}

PGUI::Screen* PGUI::Core::GetOpenScreen( uint id )
{
    if( IsScreenOpen( id ) )
        return GetScreen( id );

    return nullptr;
}

PGUI::Screen* PGUI::Core::GetActiveScreen()
{
    return ActiveScreen;
}


uint PGUI::Core::GetActiveScreenID()
{
    if( ActiveScreen )
        return GetScreenID( ActiveScreen );

    return 0;
}

std::list<uint> PGUI::Core::GetAllScreenList()
{
    std::list<uint> result;

    for( auto it = AllScreens.begin(), end = AllScreens.end(); it != end; ++it )
    {
        result.push_back( it->first );
    }

    return result;
}

std::list<uint> PGUI::Core::GetOpenScreenList()
{
    std::list<uint> result = OpenScreens;

    return result;
}

//

bool PGUI::Core::IsKeyPressed( uint8 key )
{
    return KeyboardPressed.find( key ) != KeyboardPressed.end();
}

bool PGUI::Core::IsMousePressed( int click )
{
    return MousePressed.find( click ) != MousePressed.end();
}

//

void PGUI::Core::SetActiveScreen( PGUI::Screen* screen )
{
    if( ActiveScreen )
    {
        if( ActiveScreen == screen )
        {
            if( Debug )
                App.WriteLogF( _FUNC_, " : no change\n" );

            return;
        }

        uint id = GetScreenID( ActiveScreen );

        if( EventScreenActive )
        {
            if( Debug )
                App.WriteLogF( _FUNC_, " : screen<%u> EventScreenActive(false)\n", id );
            EventScreenActive( id, ActiveScreen, false );
        }

        if( Debug )
            App.WriteLogF( _FUNC_, " : screen<%u> OnActive(false)\n", id );
        ActiveScreen->OnActive( false );

        ActiveScreen = nullptr;
    }

    if( screen )
    {
        uint id = GetScreenID( screen );

        ActiveScreen = screen;

        if( EventScreenActive )
        {
            if( Debug )
                App.WriteLogF( _FUNC_, " : screen<%u> EventScreenActive(true)\n", id );
            EventScreenActive( id, ActiveScreen, true );
        }

        if( Debug )
            App.WriteLogF( _FUNC_, " : screen<%u> OnActive(true)\n", id );
        ActiveScreen->OnActive( true );
    }
}

//

void PGUI::Core::Update()
{
    if( !IsUpdating )
        return;

    for( auto it = OpenScreens.begin(), end = OpenScreens.end(); it != end; ++it )
    {
        PGUI::Screen* screen = GetOpenScreen( *it );

        if( !screen || !screen->IsUpdating )
            continue;

        screen->Update();
    }
}

void PGUI::Core::Draw( uint8 layer )
{
    if( !IsVisible )
        return;

    if( DebugDraw )
        App.WriteLogF( _FUNC_, "(%u)\n", layer );

    for( auto it = OpenScreens.begin(), end = OpenScreens.end(); it != end; ++it )
    {
        uint          id = *it;

        PGUI::Screen* screen = GetScreen( id );
        if( !screen || !screen->IsVisible )
            continue;

        if( screen->IsStatic && ActiveScreen && ActiveScreen == screen )
            SetActiveScreen( nullptr );

        if( screen->Layer && screen->Layer == layer )
        {
            if( DebugDraw )
                App.WriteLogF( _FUNC_, "(%u) screen<%u> Draw()\n", layer, id );

            screen->DrawLayer = layer;
            screen->Draw();
            screen->DrawLayer = 0;
        }
    }
}

void PGUI::Core::DrawMap()
{
    if( !IsVisible )
        return;

    if( DebugDraw )
        App.WriteLogF( _FUNC_, "\n" );

    for( auto it = OpenScreens.begin(), end = OpenScreens.end(); it != end; ++it )
    {
        uint          id = *it;

        PGUI::Screen* screen = GetScreen( id );

        if( !screen || !screen->IsVisible )
            continue;

        if( DebugDraw )
            App.WriteLogF( _FUNC_, " : screen<%u>\n", id );
        screen->DrawMap();
    }
}

bool PGUI::Core::KeyDown( uint8 key, std::string& keyString )
{
    if( !IsKeyboardActive )
        return false;

    if( Debug )
        App.WriteLogF( _FUNC_, "(%u,\"%s\") // %s\n", key, keyString.c_str(), PGUI::Keyboard::GetKeyName( key ).c_str() );

    KeyboardPressed.insert( key );

    if( ActiveScreen && ActiveScreen->KeyDown( key, keyString ) )
        return true;

    return false;
}

bool PGUI::Core::KeyUp( uint8 key, std::string& keyString )
{
    if( !IsKeyboardActive )
        return false;

    if( Debug )
        App.WriteLogF( _FUNC_, "(%u,\"%s\") // %s\n", key, keyString.c_str(), PGUI::Keyboard::GetKeyName( key ).c_str() );

    if( KeyboardPressed.find( key ) == KeyboardPressed.end() )
    {
        App.WriteLogF( _FUNC_, "(%u,\"%s\") WARNING : ghost key ignored\n", key, keyString.c_str() );
        return true;
    }

    KeyboardPressed.erase( key );

    if( ActiveScreen && ActiveScreen->KeyUp( key, keyString ) )
        return true;


    return false;
}

bool PGUI::Core::MouseDown( int click, int x, int y )
{
    if( !IsMouseActive )
        return false;

    if( Debug )
        App.WriteLogF( _FUNC_, "(%u,%d,%d)\n", click, x, y );

    MousePressed.insert( click );

    if( !OpenScreens.empty() )
    {
        for( auto end = OpenScreens.end(), it = OpenScreens.begin(); end != it; --end )
        {
            uint          id = *it;
            PGUI::Screen* screen = GetScreen( id );

            if( !screen || !screen->IsVisible || !screen->IsMouseActive )
                continue;

            if( screen->IsStatic && ActiveScreen == screen )
                SetActiveScreen( nullptr );

            if( screen->IsInside( x, y ) )
            {
                if( !screen->IsStatic )
                {
                    // draw order
                    if( OpenScreens.back() != id )
                    {
                        OpenScreens.remove( id );
                        OpenScreens.push_back( id );
                    }

                    SetActiveScreen( screen );
                }

                if( Debug )
                    App.WriteLogF( _FUNC_, "(%d,%d,%d) : screen<%u> MouseDown()%s\n", click, x, y, id, screen->IsStatic ? " static" : "" );
                if( screen->MouseDown( click, x, y ) )
                    return true;

                if( screen->IsStatic )
                    continue;
                else
                    return false;
            }
        }

        SetActiveScreen( nullptr );
    }

    return false;
}

void PGUI::Core::MouseMove( int fromX, int fromY, int toX, int toY )
{
    if( !IsMouseActive )
        return;

    // if( Debug )
    //    App.WriteLogF( _FUNC_, " from<%d,%d> to<%d,%d>\n", fromX, fromY, toX, toY );

    if( !OpenScreens.empty() )
    {
        PGUI::Screen* last = GetScreen( OpenScreens.back() );

        if( !last )
            return;

        last->MouseMove( fromX, fromY, toX, toY );
    }
}

bool PGUI::Core::MouseUp( int click, int x, int y )
{
    if( !IsMouseActive )
        return false;

    if( Debug )
        App.WriteLogF( _FUNC_, "(%d,%d,%d)\n", click, x, y );

    if( MousePressed.find( click ) == MousePressed.end() )
    {
        App.WriteLogF( _FUNC_, "(%d,%d,%d) WARNING : ghost click ignored\n", click, x, y );
        return true;
    }

    MousePressed.erase( click );

    for( auto end = OpenScreens.end(), it = OpenScreens.begin(); end != it; --end )
    {
        uint          id = *it;
        PGUI::Screen* screen = GetOpenScreen( id );

        if( !screen || !screen->IsVisible || !screen->IsMouseActive  )
            continue;

        if( Debug )
            App.WriteLogF( _FUNC_, "(%d,%d,%d) : screen<%u> MouseUp()%s\n", click, x, y, id, screen->IsStatic ? " static" : "" );

        if( screen->MouseUp( click, x, y ) )
            return true;
    }

    return false;
}

void PGUI::Core::InputLost()
{
    KeyboardPressed.clear();
    MousePressed.clear();

    for( auto end = OpenScreens.end(), it = OpenScreens.begin(); end != it; --end )
    {
        uint          id = *it;
        PGUI::Screen* screen = GetOpenScreen( id );

        if( !screen || !screen->IsVisible )
            continue;

        screen->InputLost();
    }
}
