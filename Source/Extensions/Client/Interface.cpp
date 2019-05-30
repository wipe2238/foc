#include <GameOptions.h>
#include <Random.h>
#include <SpriteManager.h>

#include "Client/Interface.h"
#include "Shared/SScriptFunc.h"

using namespace std;

FOC::InterfaceManager::InterfaceManager()
{
    TextBlock& debug = TextBlocks[TB_DEBUG];
    debug.StartX = debug.StartY = 5;

    SETFLAG( debug.Map[TI_FPS].Flags, FONT_FLAG_CENTERR );
    SETFLAG( debug.Map[TI_SCREENS].Flags, FONT_FLAG_CENTERR );
}

FOC::InterfaceManager::~InterfaceManager()
{
    TextBlocks.clear();
}

void FOC::InterfaceManager::ProcessScreenChange( bool show, int screen, int p0, int p1, int p2 )
{
    if( show )
        Screens.insert( screen );
    else
        Screens.erase( screen );
}

void FOC::InterfaceManager::ProcessGetActiveScreens( ScriptArray& screens )
{
    screens.Resize( Screens.size() );

    set<int>::iterator it = Screens.begin();
    for( int s = 0, sLen = Screens.size(); s < sLen; s++, ++it )
    {
        *(int*)screens.At( s ) = *it;
    }
}

void FOC::InterfaceManager::ProcessRenderInterface( uint layer )
{
    if( layer == 1 )
    {
        bool first = false;
        TextBlocks[TB_DEBUG].Map[TI_FPS].Text = to_string( (long long)GameOpt.FPS );

        TextBlocks[TB_DEBUG].Map[TI_SCREENS].Text = "";        // to_string((long long)Screens.size());
        auto it = Screens.begin();
        for( int s = 0, sLen = Screens.size(); s < sLen; s++, ++it )
        {
            if( s )
                TextBlocks[TB_DEBUG].Map[TI_SCREENS].Text += ", ";

            TextBlocks[TB_DEBUG].Map[TI_SCREENS].Text += (*it < 10 ? "[" : "") + to_string( (long long)*it ) + (*it < 10 ? "]" : "");
        }
    }
    else if( layer == 2 )
    {
        for( auto it = Screens.begin(), end = Screens.end(); it != end; ++it )
        {
            if( *it < 10 )
                FOClient::SScriptFunc::Global_DrawHardcodedScreen( *it );
        }
    }

    if( layer == 2 || layer == 101 )
        DrawTextBlocks();

    if( layer == 3 )
    {
        for( auto it = Screens.begin(), end = Screens.end(); it != end; ++it )
        {
            if( *it >= 10 )
                FOClient::SScriptFunc::Global_DrawHardcodedScreen( *it );
        }
    }
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
