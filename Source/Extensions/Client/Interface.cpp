#include <App.h>
#include <Defines.Public.h>
#include <GameOptions.h>
#include <HexManager.h>
#include <Keyboard.h>
#include <Random.h>
#include <SpriteManager.h>

#include "Client/Interface.h"
#include "Client/Screen/Game.h"
#include "Client/Screen/Login.h"
#include "Client/Screen/MiniMap.h"
#include "Shared/Hex2D.h"
#include "Shared/SScriptFunc.h"

#include "PGUI/PGUI.Core.h"
#include "PGUI/PGUI.Label.h"
#include "PGUI/PGUI.Screen.h"

using namespace std;

#define LeftCtrl      UI->IsKeyPressed( DIK_LCONTROL )
#define RightCtrl     UI->IsKeyPressed( DIK_RCONTROL )
#define AnyCtrl       (LeftCtrl || RightCtrl)
#define NoCtrl        (!LeftCtrl && !RightCtrl)

#define LeftAlt       UI->IsKeyPressed( DIK_LMENU )
#define RightAlt      UI->IsKeyPressed( DIK_RMENU )
#define AnyAlt        (LeftAlt ||  RightAlt)
#define NoAlt         (!LeftAlt && !RightAlt)

#define LeftShift     UI->IsKeyPressed( DIK_LSHIFT )
#define RightShift    UI->IsKeyPressed( DIK_RSHIFT )
#define AnyShift      (LeftShift ||  RightShift)
#define NoShift       (!LeftShift && !RightShift)

FOC::InterfaceManager::InterfaceManager() :
    UI( nullptr )
{
    TextBlock& debug = TextBlocks[TB_DEBUG];
    debug.StartX = debug.StartY = 5;

    SETFLAG( debug.Map[TI_FPS].Flags, FONT_FLAG_CENTERR );
    SETFLAG( debug.Map[TI_SCREENS].Flags, FONT_FLAG_CENTERR );
    SETFLAG( debug.Map[TI_SCREENZ].Flags, FONT_FLAG_CENTERR );
}

FOC::InterfaceManager::~InterfaceManager()
{
    TextBlocks.clear();

    if( UI )
    {
        delete UI;
        UI = nullptr;
    }
}

void FOC::InterfaceManager::UI_ScreenAdd( uint id, PGUI::Screen* screen )
{}

void FOC::InterfaceManager::UI_ScreenRemove( uint id, PGUI::Screen* screen )
{
    delete screen;
}

bool FOC::InterfaceManager::ProcessStart()
{
    UI = new PGUI::Core();
    UI->Debug = true;

    // UI->EventScreenRemove = std::bind( &InterfaceManager::UI_ScreenAdd, this, placeholders::_1, placeholders::_2 );
    UI->EventScreenRemove = std::bind( &InterfaceManager::UI_ScreenRemove, this, placeholders::_1, placeholders::_2 );

    UI->AddScreen( CLIENT_MAIN_SCREEN_LOGIN, new Screen::Login( UI ) );
    UI->AddScreen( CLIENT_MAIN_SCREEN_GAME, new Screen::Game( UI ) );
    UI->AddScreen( CLIENT_SCREEN_MINIMAP, new Screen::MiniMap( UI ) );

    return true;
}

void FOC::InterfaceManager::ProcessFinish()
{
    if( UI )
    {
        delete UI;
        UI = nullptr;
    }
}

void FOC::InterfaceManager::ProcessLoop()
{
    if( UI )
        UI->Update();
}

void FOC::InterfaceManager::ProcessScreenChange( bool show, int screen, int p0, int p1, int p2 )
{
    if( !UI || !UI->IsScreen( screen ) )
    {
        if( show )
            EngineScreens.insert( screen );
        else
            EngineScreens.erase( screen );
    }
    else
    {
        if( show )
            UI->OpenScreen( screen );
        else
            UI->CloseScreen( screen );

        if( false && screen == CLIENT_MAIN_SCREEN_LOGIN )
        {
            if( show )
                EngineScreens.insert( screen );
            else
                EngineScreens.erase( screen );
        }
    }
}

void FOC::InterfaceManager::ProcessGetActiveScreens( ScriptArray& screens )
{
    screens.Resize( EngineScreens.size() );

    set<int>::iterator it = EngineScreens.begin();
    for( int s = 0, sLen = EngineScreens.size(); s < sLen; s++, ++it )
    {
        *(int*)screens.At( s ) = *it;
    }
}

void FOC::InterfaceManager::ProcessRenderInterface( uint layer )
{
    if( layer == 1 )
    {
        TextBlocks[TB_DEBUG].Map[TI_FPS].Text = to_string( (long long)GameOpt.FPS );

        TextBlocks[TB_DEBUG].Map[TI_SCREENS].Text = "";                        // to_string((long long)Screens.size());
        if( !EngineScreens.empty() )
        {
            auto it = EngineScreens.begin();
            for( int s = 0, sLen = EngineScreens.size(); s < sLen; s++, ++it )
            {
                if( s )
                    TextBlocks[TB_DEBUG].Map[TI_SCREENS].Text += ", ";

                TextBlocks[TB_DEBUG].Map[TI_SCREENS].Text += (*it < 10 ? "[" : "") + to_string( (long long)*it ) + (*it < 10 ? "]" : "");
            }
        }

        TextBlocks[TB_DEBUG].Map[TI_SCREENZ].Text = "";
        std::list<uint> list = UI->GetOpenScreenList();
        list.sort();
        if( !list.empty() )
        {
            auto it = list.begin();
            for( int s = 0, ssLen = list.size(); s < ssLen; s++, ++it )
            {
                if( s )
                    TextBlocks[TB_DEBUG].Map[TI_SCREENZ].Text += ", ";

                TextBlocks[TB_DEBUG].Map[TI_SCREENZ].Text += (*it < 10 ? "[" : "") + to_string( (long long)*it ) + (*it < 10 ? "]" : "");
            }
        }

        TextBlocks[TB_DEBUG].Map[TI_COORD].Text = Str::FormatBuf( "mouse<%d,%d>", GameOpt.MouseX, GameOpt.MouseY );
        uint16 hexX = 0, hexY = 0;
        if( FOClient::SScriptFunc::Global_GetMonitorHex( GameOpt.MouseX, GameOpt.MouseY, hexX, hexY, true ) )
        {

            TextBlocks[TB_DEBUG].Map[TI_COORD].Text += Str::FormatBuf( " hex<%u,%u>", hexX, hexY );

            uint16 x2d = hexX, y2d = hexY;
            Hex2D( x2d, y2d, FOClient::SScriptFunc::Global_GetMapWidth() );

            TextBlocks[TB_DEBUG].Map[TI_COORD].Text += Str::FormatBuf( " 2d<%d,%d>", x2d, y2d );

            Field* fieldMap = (Field*)(GameOpt.ClientMap);
            Field& field = fieldMap[hexY * FOClient::SScriptFunc::Global_GetMapWidth() + hexX];
            TextBlocks[TB_DEBUG].Map[TI_FIELD].Text = "";

            if( field.ScrollBlock )
                TextBlocks[TB_DEBUG].Map[TI_FIELD].Text += "ScrollBlock ";
            if( field.IsWall )
                TextBlocks[TB_DEBUG].Map[TI_FIELD].Text += "Wall ";
            if( field.IsWallSAI )
                TextBlocks[TB_DEBUG].Map[TI_FIELD].Text += "WallSAI ";
            if( field.IsWallTransp )
                TextBlocks[TB_DEBUG].Map[TI_FIELD].Text += "WallTransp ";
            if( field.IsScen )
                TextBlocks[TB_DEBUG].Map[TI_FIELD].Text += "Scen ";
            if( field.IsExitGrid )
                TextBlocks[TB_DEBUG].Map[TI_FIELD].Text += "ExitGrid ";
            if( field.IsNotPassed )
                TextBlocks[TB_DEBUG].Map[TI_FIELD].Text += "NotPassed ";
            if( field.IsNotRaked )
                TextBlocks[TB_DEBUG].Map[TI_FIELD].Text += "NotRaked ";
            if( field.IsNoLight )
                TextBlocks[TB_DEBUG].Map[TI_FIELD].Text += "NoLight ";
            if( field.IsMultihex )
                TextBlocks[TB_DEBUG].Map[TI_FIELD].Text += "Multihex ";
        }
        else if( FOClient::SScriptFunc::Global_get___WorldmapActive() )
        {}
    }
    else if( layer == 2 )
    {
        for( auto it = EngineScreens.begin(), end = EngineScreens.end(); it != end; ++it )
        {
            if( *it < 10 )
                FOClient::SScriptFunc::Global_DrawHardcodedScreen( *it );
        }
    }

    if( layer == 2 || layer == 101 )
        DrawTextBlocks();

    if( layer == 3 )
    {
        for( auto it = EngineScreens.begin(), end = EngineScreens.end(); it != end; ++it )
        {
            if( *it >= 10 )
                FOClient::SScriptFunc::Global_DrawHardcodedScreen( *it );
        }
    }

    if( UI )
        UI->Draw( layer );

}

void FOC::InterfaceManager::ProcessRenderMap()
{
    if( UI )
        UI->DrawMap();
}

bool FOC::InterfaceManager::ProcessKeyDown( uint8 key, std::string& keyText )
{
    if( !UI )
        return false;

    if( UI->KeyDown( key, keyText ) )
        return true;

    if( UI->IsScreenOpen( CLIENT_MAIN_SCREEN_GAME ) )
    {
        if( key == DIK_TAB )
        {
            UI->ToggleScreen( CLIENT_SCREEN_MINIMAP );
            return true;
        }
    }

    return false;
}

bool FOC::InterfaceManager::ProcessKeyUp( uint8 key, std::string& keyText )
{
    if( UI && UI->KeyUp( key, keyText ) )
        return true;

    return false;
}

bool FOC::InterfaceManager::ProcessMouseDown( int click )
{

    if( UI && UI->MouseDown( click ) )
        return true;

    if( click == MOUSE_CLICK_WHEEL_UP || click == MOUSE_CLICK_WHEEL_DOWN )
    {
        float zoom = GameOpt.SpritesZoom + (click == MOUSE_CLICK_WHEEL_DOWN ? 0.1f : -0.1f);
        FOClient::SScriptFunc::Global_SetZoom( zoom );

        return true;
    }

    return false;
}

void FOC::InterfaceManager::ProcessMouseMove( int x, int y )
{
    if( UI )
        UI->MouseMove( x, y );

    Mouse.LastX = GameOpt.MouseX;
    Mouse.LastY = GameOpt.MouseY;
}

bool FOC::InterfaceManager::ProcessMouseUp( int click )
{
    if( UI && UI->MouseUp( click ) )
        return true;

    return false;
}

void FOC::InterfaceManager::ProcessInputLost()
{
    if( UI )
        UI->InputLost();
}

void FOC::InterfaceManager::DrawTextBlocks()
{
    ScriptString text;
    uint         yOffset = 0;

    for( auto blockIt = TextBlocks.begin(), blockEnd = TextBlocks.end(); blockIt != blockEnd; ++blockIt )
    {
        TextBlock& block = blockIt->second;

        yOffset = block.StartY;

        for( auto it = block.Map.begin(), end = block.Map.end(); it != end; ++it )
        {
            if( it->second.Text.empty() )
                continue;

            TextInfo& info = it->second;

            if( info.ShowName )
                text = it->first.substr( it->first.find( ":" ) + 1 ) + string( ": " ) + info.Text;
            else
                text = info.Text;

            int tw, th, lines;
            SprMngr.GetTextInfo( GameOpt.ScreenWidth - block.StartX, 0, text.c_str(), info.Font, info.Flags, tw, th, lines );
            FOClient::SScriptFunc::Global_DrawText( text, block.StartX, yOffset, tw, th, info.Color, info.Font, info.Flags );
            yOffset += th + block.SpacingY;
        }
    }
}
