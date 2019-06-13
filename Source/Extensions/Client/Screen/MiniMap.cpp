#include <App.h>
#include <CritterCl.h>
#include <GameOptions.h>
#include <HexManager.h>
#include <Keyboard.h>

#include "Client/Screen/MiniMap.h"
#include "Shared/Hex2D.h"
#include "Shared/SScriptFunc.h"

#include "PGUI/PGUI.Core.h"
#include "PGUI/PGUI.Draw.h"

FOC::Screen::MiniMap::MapHex::MapHex( bool show, uint8 r, uint8 g, uint8 b, uint8 alpha /* = 255 */ ) :
// public
    Show( show ),
    Color( COLOR_ARGB( alpha, r, g, b ) )
{}

FOC::Screen::MiniMap::Bounds::Bounds() :
// public
    MinPX( uint16( -1 ) ), MaxPX( 0 ),
    MinPY( uint16( -1 ) ), MaxPY( 0 ),
    Width( 0 ), Height( 0 )
{}

FOC::Screen::MiniMap::MiniMap( PGUI::Core* gui ) : PGUI::Screen( gui ),
// public
    MiniMapPid( uint( -1 ) ),
    ScrollBlock( false, 127, 127, 127 ),
    ExitGrid( true, 127, 255, 0 ),
    Wall( true, 255, 127, 0 ),
    Scenery( true, 0, 255, 0 ),
// private
    MiniMapBounds(),
    MiniMapZoom( 1.0f ),
    MiniMapData( nullptr ),
    NeedUpdateMiniMap( true )
{
    Layer = 2;
    IsMouseEnabled = false;
}

FOC::Screen::MiniMap::~MiniMap()
{
    PGUI::Draw::DeleteCache( MiniMapData );
}

bool FOC::Screen::MiniMap::GetHex( uint16 hexX, uint16 hexY, bool& show, uint& color )
{
    if( !GameOpt.ClientMap )
        return false;

    if( !FOClient::SScriptFunc::Global_GetCurrentMapPid() )
        return false;

    uint16 mapWidth = FOClient::SScriptFunc::Global_GetMapWidth();

    Field* mapField = (Field*)(GameOpt.ClientMap);
    Field& field = mapField[hexY * mapWidth + hexX];

    show = false;
    color = 0;

    if( field.ScrollBlock )
    {
        show = ScrollBlock.Show;
        color = ScrollBlock.Color;
    }
    else if( field.IsExitGrid )
    {
        show = ExitGrid.Show;
        color = ExitGrid.Color;
    }
    else if( field.IsWall )
    {
        show = Wall.Show;
        color = Wall.Color;
    }
    else if( field.IsScen ) // should be last-ish, to not consume cases above
    {
        show = Scenery.Show;
        color = Scenery.Color;
    }

    return true;
}

void FOC::Screen::MiniMap::SetShow( MapHex& object, bool show )
{
    if( object.Show == show )
        return;

    object.Show = show;
    NeedUpdateMiniMap = true;
}

void FOC::Screen::MiniMap::SetColor( MapHex& object, uint8 r, uint8 g, uint8 b, uint8 alpha /* = 255 */ )
{
    uint newColor = COLOR_ARGB( alpha, r, g, b );
    if( object.Color == newColor )
        return;

    object.Color = newColor;
    NeedUpdateMiniMap = true;
}

void FOC::Screen::MiniMap::SetObject( MapHex& object, bool show, uint8 r, uint8 g, uint8 b, uint8 alpha )
{
    SetShow( object, show );
    SetColor( object, r, g, b, alpha );
}

void FOC::Screen::MiniMap::Update()
{
    if( !IsUpdateEnabled )
        return;

    PGUI::Screen::Update();

    const uint16 mapPid = FOClient::SScriptFunc::Global_GetCurrentMapPid();

    // worldmap
    if( !mapPid )
        NeedUpdateMiniMap = true;

    // field map regeneration
    if( !GameOpt.ClientMap )
        NeedUpdateMiniMap = true;

    // different map; needs fix when/if dynamic sceneries are born
    if( MiniMapPid != mapPid )
        NeedUpdateMiniMap = true;

    // no size
    if( !MiniMapBounds.Width || !MiniMapBounds.Height )
        NeedUpdateMiniMap = true;

    // no data
    if( !MiniMapData )
        NeedUpdateMiniMap = true;

    if( NeedUpdateMiniMap )
        UpdateMiniMap();
}

void FOC::Screen::MiniMap::UpdateMiniMap()
{
    const uint16 mapPid = FOClient::SScriptFunc::Global_GetCurrentMapPid();

    if( !mapPid || !GameOpt.ClientMap )     // worldmap, ~regenmap, etc.
        return;

    Field*       mapField = (Field*)(GameOpt.ClientMap);

    const uint16 mapWidth = FOClient::SScriptFunc::Global_GetMapWidth();
    const uint16 mapHeight = FOClient::SScriptFunc::Global_GetMapHeight();

    if( GUI->Debug )
        App.WriteLogF( _FUNC_, " : map<pid:%u> size<%ux%u>\n", mapPid, mapWidth, mapHeight );

    // define minimap bounds

    MiniMapBounds = Bounds();

    for( uint16 hexX = 0; hexX < mapWidth; hexX++ )
    {
        for( uint16 hexY = 0; hexY < mapHeight; hexY++ )
        {
            bool show = false;
            uint dummy = 0;

            // check if current hex is going to be part of minimap

            if( GetHex( hexX, hexY, show, dummy ) && show )
            {
                // convert hex position to 2d position

                uint16 px = hexX, py = hexY;
                Hex2D( px, py, mapWidth );

                // update bounds

                MiniMapBounds.MinPX = std::min( MiniMapBounds.MinPX, px );
                MiniMapBounds.MaxPX = std::max( MiniMapBounds.MaxPX, px );
                MiniMapBounds.MinPY = std::min( MiniMapBounds.MinPY, py );
                MiniMapBounds.MaxPY = std::max( MiniMapBounds.MaxPY, py );
            }
        }
    }

    // process map

    PGUI::DrawData automap( DRAW_PRIMITIVE_POINTLIST );

    for( uint16 hexX = 0; hexX < mapWidth; hexX++ )
    {
        for( uint16 hexY = 0; hexY < mapHeight; hexY++ )
        {
            Field& f = mapField[hexY * mapWidth + hexX];

            // convert hex position to 2d position

            uint16 px = hexX, py = hexY;
            Hex2D( px, py, mapWidth );

            // ignore everything outside bounds

            if( px < MiniMapBounds.MinPX || px > MiniMapBounds.MaxPX || py < MiniMapBounds.MinPY || py > MiniMapBounds.MaxPY )
                continue;

            // check if current hex is going to be part of minimap

            bool show = false;
            uint color = 0;
            if( !GetHex( hexX, hexY, show, color ) || !show )
                continue;

            // adjust position

            px -= MiniMapBounds.MinPX;
            py -= MiniMapBounds.MinPY;

            // update minimap

            automap.Points.push_back( PGUI::DrawPoint( px, py, color ) );
        }
    }

    // finalize

    if( GUI->Debug )
        App.WriteLogF( _FUNC_, " : %d point%s\n", automap.Points.size(), automap.Points.size() != 1 ? "s" : "" );

    MiniMapPid = mapPid;
    MiniMapBounds.Width = MiniMapBounds.MaxPX - MiniMapBounds.MinPX;
    MiniMapBounds.Height = MiniMapBounds.MaxPY - MiniMapBounds.MinPY;

    PGUI::Draw::DeleteCache( MiniMapData );
    MiniMapData = automap.NewCache();

    UpdateMiniMapZoom( MiniMapZoom );

    NeedUpdateMiniMap = false;
}

void FOC::Screen::MiniMap::UpdateMiniMapZoom( float zoom )
{
    if( zoom >= 0.1f && zoom <= 3.6f )
    {
        uint16 width = (uint16)(MiniMapBounds.Width / zoom);
        uint16 height = (uint16)(MiniMapBounds.Height / zoom);

        if( width < GUI->GetScreenWidth() && height < GUI->GetScreenWidth() )
        {
            SetSize( width, height );
            SetPositionAt( 5 + 1, -5 - -1 );                 // top-right, 5px margin
            MiniMapZoom = zoom;

            if( GUI->Debug )
                App.WriteLogF( _FUNC_, " = %.1ff\n", MiniMapZoom );
        }
    }
}

void FOC::Screen::MiniMap::DrawContent()
{
    if( !IsDrawEnabled )
        return;

    if( MiniMapData )
    {
        PGUI::Draw::RenderData( MiniMapData, GetLeft(), GetTop(), MiniMapZoom );
    }
}

bool FOC::Screen::MiniMap::KeyDown( uint8 key, std::string& keyText )
{
    if( !IsKeyboardEnabled )
        return false;

    if( key == DIK_ADD || key == DIK_SUBTRACT )
    {
        UpdateMiniMapZoom( MiniMapZoom + (key == DIK_ADD ? -0.1f : 0.1f) );

        return true;
    }

    return false;
}
