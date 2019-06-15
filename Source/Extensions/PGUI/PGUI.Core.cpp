#include <climits>
#include <list>

#include <App.h>
#include <Defines.Public.h>
#include <GameOptions.h>
#include <Ini.h>

#include "PGUI.Core.h"
#include "PGUI.Keyboard.h"
#include "PGUI.Screen.h"

#if 0
static void LogContainer( const char* func, const std::map<uint, PGUI::Screen*>& container, const char* name )
{
    App.WriteLogF( func, " : container<%s> = ", name );

    if( container.empty() )
        App.WriteLogX( "empty\n" );
    else
    {
        bool first = true;
        for( auto it = container.begin(), end = container.end(); it != end; ++it )
        {
            if( first )
                first = false;
            else
                App.WriteLogX( "," );
            App.WriteLogX( "%u", it->first );
        }
        App.WriteLogX( "\n" );
    }
}

static void LogContainer( const char* func, const std::list<uint>& container, const char* name )
{
    App.WriteLogF( func, " : container<%s> = ", name );

    if( container.empty() )
        App.WriteLogX( "empty\n" );
    else
    {
        bool first = true;
        for( auto it = container.begin(), end = container.end(); it != end; ++it )
        {
            if( first )
                first = false;
            else
                App.WriteLogX( "," );
            App.WriteLogX( "%u", *it );
        }
        App.WriteLogX( "\n" );
    }
}
#endif // 0

PGUI::CoreSettings::CoreSettings() :
// public
    ColorBackground( COLOR_ARGB( 100, 0, 255, 0 ) ),
    ColorBackgroundButton( COLOR_ARGB( 150, 0, 200, 0 ) ),
    ColorBackgroundButtonHover( COLOR_ARGB( 200, 0, 200, 0 ) ),
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
    IsDrawEnabled( visible ), IsUpdateEnabled( true ), IsKeyboardEnabled( true ), IsMouseEnabled( true ),
// private
    AllScreens(), OpenScreens(), ModalScreen( nullptr ),
    KeyboardPressed(),
    MousePressed(), MouseLastX( 0 ), MouseLastY( 0 )
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

    bool modal = false;
    screen->OnOpen( modal );

    SetTopScreen( id );

    if( modal )
        SetModalScreen( screen );

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

    if( GetModalScreen() == screen )
        SetModalScreen( nullptr );

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

    std::list<uint> list = GetOpenScreenList();

    for( auto it = list.begin(), end = list.end(); it != end; ++it )
    {
        CloseScreen( *it );
    }
}

void PGUI::Core::ToggleScreen( uint id )
{
    if( !IsScreen( id ) )
        return;

    if( Debug )
        App.WriteLogF( _FUNC_, "\n" );

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

PGUI::Screen* PGUI::Core::GetModalScreen()
{
    return ModalScreen;
}


uint PGUI::Core::GetModalScreenID()
{
    if( ModalScreen )
        return GetScreenID( ModalScreen );

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
    return OpenScreens;
}

//

bool PGUI::Core::IsKeyPressed( uint8 key )
{
    return KeyboardPressed.find( key ) != KeyboardPressed.end();
}

bool PGUI::Core::IsMousePressed( uint8 click )
{
    return MousePressed.find( click ) != MousePressed.end();
}

bool PGUI::Core::IsMouseInsideScreen( int x, int y )
{
    return x >= 0 && x <= GetScreenWidth() && y >= 0 && y <= GetScreenHeight();
}

//

void PGUI::Core::SetModalScreen( PGUI::Screen* screen )
{
    if( ModalScreen )
    {
        if( ModalScreen == screen )
        {
            if( Debug )
                App.WriteLogF( _FUNC_, " : no change\n" );

            return;
        }

        uint id = GetScreenID( ModalScreen );

        if( EventScreenModal )
        {
            if( Debug )
                App.WriteLogF( _FUNC_, " : screen<%u> EventScreenModal(false)\n", id );

            EventScreenModal( id, ModalScreen, false );
        }

        ModalScreen = nullptr;
    }

    if( screen )
    {
        uint id = GetScreenID( screen );

        ModalScreen = screen;

        if( EventScreenModal )
        {
            if( Debug )
                App.WriteLogF( _FUNC_, " : screen<%u> EventScreenModal(true)\n", id );

            EventScreenModal( id, ModalScreen, true );
        }
    }
}

void PGUI::Core::SetTopScreen( uint id )
{
    if( !IsScreenOpen( id ) )
    {
        App.WriteLogF( _FUNC_, " ERROR : screen<%u> not opened\n", id );
        return;
    }

    if( !OpenScreens.empty() && OpenScreens.back() != id )
    {
        // handle old screen

        uint          oldID = OpenScreens.back();

        PGUI::Screen* screen = GetOpenScreen( oldID );

        if( Debug )
            App.WriteLogF( _FUNC_, "(%u) : screen<%u> OnTop(false)\n", id, oldID );

        screen->OnTop( false );

        OpenScreens.remove( id );

        // handle new screen

        OpenScreens.push_back( id );

        screen = GetScreen( id );
        if( Debug )
            App.WriteLogF( _FUNC_, "(%u) : screen<%u> OnTop(true)\n", id, id );

        screen->OnTop( true );
    }
}

//

void PGUI::Core::Update()
{
    if( !IsUpdateEnabled )
        return;

    for( auto it = OpenScreens.begin(), end = OpenScreens.end(); it != end; ++it )
    {
        PGUI::Screen* screen = GetOpenScreen( *it );

        if( !screen || !screen->IsUpdateEnabled )
            continue;

        screen->Update();
    }
}

void PGUI::Core::Draw( uint8 layer )
{
    if( !IsDrawEnabled )
        return;

    if( DebugDraw )
        App.WriteLogF( _FUNC_, "(%u)\n", layer );

    for( auto it = OpenScreens.begin(), end = OpenScreens.end(); it != end; ++it )
    {
        uint          id = *it;

        PGUI::Screen* screen = GetScreen( id );

        if( !screen || !screen->IsDrawEnabled || !screen->Layer )
            continue;

        if( screen->Layer == layer )
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
    if( !IsDrawEnabled )
        return;

    if( DebugDraw )
        App.WriteLogF( _FUNC_, "\n" );

    for( auto it = OpenScreens.begin(), end = OpenScreens.end(); it != end; ++it )
    {
        uint          id = *it;

        PGUI::Screen* screen = GetScreen( id );

        if( !screen || !screen->IsDrawEnabled )
            continue;

        if( DebugDraw )
            App.WriteLogF( _FUNC_, " : screen<%u>\n", id );
        screen->DrawMap();
    }
}

bool PGUI::Core::KeyDown( uint8 key, std::string& keyText )
{
    if( !IsKeyboardEnabled )
        return false;

    if( Debug )
        App.WriteLogF( _FUNC_, "(%u,\"%s\") // %s\n", key, keyText.c_str(), PGUI::Keyboard::GetKeyName( key ).c_str() );

    KeyboardPressed.insert( key );

    PGUI::Screen* screen = GetModalScreen();

    if( screen )
    {
        if( Debug )
            App.WriteLogF( _FUNC_, " : modal check\n" );

        if( screen->IsKeyboardEnabled && screen->KeyDown( key, keyText ) )
            return true;
    }
    else
    {
        std::list<uint> screens = GetOpenScreenList();

        for( auto it = screens.rbegin(), end = screens.rend(); it != end; ++it ) // reversed order
        {
            uint id = *it;
            screen = GetOpenScreen( id );

            if( !screen )
            {
                App.WriteLogF( _FUNC_, " WARNING : screen<%u> is null\n", id );
                continue;
            }

            if( !screen->IsKeyboardEnabled )
                continue;

            if( screen->KeyDown( key, keyText ) )
                return true;
        }
    }

    return false;
}

bool PGUI::Core::KeyUp( uint8 key, std::string& keyText )
{
    if( !IsKeyboardEnabled )
        return false;

    if( Debug )
        App.WriteLogF( _FUNC_, "(%u,\"%s\") // %s\n", key, keyText.c_str(), PGUI::Keyboard::GetKeyName( key ).c_str() );

    if( !IsKeyPressed( key ) )
    {
        App.WriteLogF( _FUNC_, "(%u,\"%s\") WARNING : ghost key ignored\n", key, keyText.c_str() );
        return true;
    }

    KeyboardPressed.erase( key );

    PGUI::Screen* screen = GetModalScreen();

    if( screen )
    {
        if( Debug )
            App.WriteLogF( _FUNC_, " : modal check\n" );

        if( screen->IsKeyboardEnabled && screen->KeyUp( key, keyText ) )
            return true;
    }
    else
    {
        std::list<uint> screens = GetOpenScreenList();

        for( auto it = screens.rbegin(), end = screens.rend(); it != end; ++it ) // reversed order
        {
            uint id = *it;
            screen = GetOpenScreen( id );

            if( !screen )
            {
                App.WriteLogF( _FUNC_, " WARNING : screen<%u> is null\n", id );
                continue;
            }

            if( !screen->IsKeyboardEnabled )
                continue;

            if( screen->KeyUp( key, keyText ) )
                return true;
        }
    }

    return false;
}

bool PGUI::Core::MouseDown( int click )
{
    if( !IsMouseEnabled )
        return false;

    if( Debug )
        App.WriteLogF( _FUNC_, "(%d)\n", click );

    return MouseDown( click, GameOpt.MouseX, GameOpt.MouseY );
}

bool PGUI::Core::MouseDown( int click, int x, int y )
{
    if( !IsMouseEnabled )
        return false;

    if( Debug )
        App.WriteLogF( _FUNC_, "(%d,%d,%d)\n", click, x, y );

    if( click < 0  || click > uint8( -1 ) )
    {
        App.WriteLogF( _FUNC_, " WARNING : ignored click<%d>\n", click );
        return false;
    }

    uint8 button = CLAMP( click, 0, uint8( -1 ) );

    MousePressed.insert( button );

    int16 mx = CLAMP( x, SHRT_MIN, SHRT_MAX );
    int16 my = CLAMP( y, SHRT_MIN, SHRT_MAX );

    // block events outside of screen bounds
    if( !IsMouseInsideScreen( mx, my ) )
        return false;

    PGUI::Screen* screen = GetModalScreen();

    if( screen )
    {
        if( screen->IsMouseEnabled && screen->IsInside( mx, my ) && screen->MouseDown( button, mx, my ) )
            return true;
    }
    else
    {
        std::list<uint> screens = GetOpenScreenList();

        for( auto it = screens.rbegin(), end = screens.rend(); it != end; ++it )        // reversed order
        {
            uint id = *it;
            screen = GetOpenScreen( id );

            if( !screen )
            {
                App.WriteLogF( _FUNC_, " WARNING : screen<%u> is null\n", id );
                continue;
            }

            if( !screen->IsMouseEnabled )
                continue;

            if( screen->IsInside( x, y ) )
            {
                // draw order
                SetTopScreen( id );

                if( Debug )
                    App.WriteLogF( _FUNC_, "(%d,%d,%d) : screen<%u> MouseDown(%u,%u,%u)\n", click, x, y, id,  button, mx, my );

                if( screen->MouseDown( button, mx, my ) )
                    return true;
            }
        }
    }

    return false;
}

void PGUI::Core::MouseMove( int x, int y )
{
    if( !IsMouseEnabled )
        return;

    // ignore passed arguments, use GameOptions values instead
    int16 mx = CLAMP( GameOpt.MouseX, SHRT_MIN, SHRT_MAX );
    int16 my = CLAMP( GameOpt.MouseY, SHRT_MIN, SHRT_MAX );

    // block events outside of screen bounds
    if( !IsMouseInsideScreen( mx, my ) )
    {
        MouseLastX = mx;
        MouseLastY = my;

        return;
    }

    PGUI::Screen* screen = GetModalScreen();

    if( screen )
    {
        if( screen->IsMouseEnabled && screen->IsInside( mx, my ) )
            screen->MouseMove( MouseLastX, MouseLastY, mx, my );
    }
    else
    {
        std::list<uint> screens = GetOpenScreenList();

        for( auto it = screens.rbegin(), end = screens.rend(); it != end; ++it ) // reversed order
        {
            uint id = *it;
            screen = GetOpenScreen( id );

            if( !screen )
            {
                App.WriteLogF( _FUNC_, " WARNING : screen<%u> is null\n", id );
                continue;
            }

            if( !screen->IsMouseEnabled )
                continue;

            screen->MouseMove( MouseLastX, MouseLastY, mx, my );
        }
    }

    MouseLastX = mx;
    MouseLastY = my;
}

bool PGUI::Core::MouseUp( int click )
{
    if( !IsMouseEnabled )
        return false;

    if( Debug )
        App.WriteLogF( _FUNC_, "(%d)\n", click );

    if( click < 0 || click > uint8( -1 ) )
    {
        App.WriteLogF( _FUNC_, " WARNING : ignored click<%d>\n", click );
        return false;
    }

    uint8 button = CLAMP( click, 0, uint8( -1 ) );

    if( !IsMousePressed( button ) )
    {
        App.WriteLogF( _FUNC_, "(%u) WARNING : ghost button ignored\n", button );
        return true;
    }

    MousePressed.erase( button );

    int16 mx = CLAMP( GameOpt.MouseX, SHRT_MIN, SHRT_MAX );
    int16 my = CLAMP( GameOpt.MouseY, SHRT_MIN, SHRT_MAX );

    // block events outside of screen bounds
    if( !IsMouseInsideScreen( mx, my ) )
        return false;

    PGUI::Screen* screen = GetModalScreen();

    if( screen )
    {
        if( screen->IsMouseEnabled && screen->IsInside( mx, my ) && screen->MouseUp( button, mx, my ) )
            return true;
    }
    else
    {
        std::list<uint> screens = GetOpenScreenList();

        for( auto it = screens.rbegin(), end = screens.rend(); it != end; ++it )        // reversed order
        {
            uint          id = *it;
            PGUI::Screen* screen = GetOpenScreen( id );

            if( !screen )
            {
                App.WriteLogF( _FUNC_, " WARNING : screen<%u> is null\n", id );
                continue;
            }

            if( !screen->IsMouseEnabled )
                continue;

            if( Debug )
                App.WriteLogF( _FUNC_, "(%d) : screen<%u> MouseUp(%u,%u,%u)\n", click, id, button, mx, my );

            if( screen->MouseUp( button, mx, my ) )
                return true;
        }
    }

    return false;
}

void PGUI::Core::InputLost()
{
    // TODO reduce number of events sent to screens
    // Current behavior: Client calls input_lost() N times per loop as long main window is not focused
    // Wanted behavior: send event once when window is not focused, and once when it regains focus

    KeyboardPressed.clear();
    MousePressed.clear();

    /*
       std::list<uint> screens = GetOpenScreenList();

       for( auto it = screens.rbegin(), end = screens.rend(); it != end; ++it ) // reversed order
       {
       uint          id = *it;
       PGUI::Screen* screen = GetOpenScreen( id );

       if( !screen )
           {
           App.WriteLogF(_FUNC_, " WARNING : screen<%u> is null\n",id);
        continue;
                }

       if( Debug )
        App.WriteLogF( _FUNC_, " : screen<%u>\n", id );

       screen->InputLost();
       }
     */
}
