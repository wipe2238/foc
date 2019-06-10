#include <App.h>
#include <GameOptions.h>
#include <HexManager.h>


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

FOC::Screen::MiniMap::MiniMap( PGUI::Core* gui ) : PGUI::Screen( gui ),
// public
    MiniMapPid( uint( -1 ) ),
    MiniMapData(),
    MiniMapZoom( 1.0f ),
    ScrollBlock( false, 127, 127, 127 ),
    ExitGrid( true, 127, 255, 0 ),
    Wall( true, 255, 127, 0 ),
    Scenery( true, 0, 255, 0 ),
// private
    NeedUpdateMiniMap( true )
{
    Layer = 2;

    SetBorderVisible( false );
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
    else if( field.IsScen )
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
    if( !IsUpdating )
        return;

    PGUI::Screen::Update();

    const uint16 mapPid = FOClient::SScriptFunc::Global_GetCurrentMapPid();

    // worldmap
    if( !mapPid )
        NeedUpdateMiniMap = true;

    // field map regeneration
    if( !GameOpt.ClientMap )
        NeedUpdateMiniMap = true;

    // different map
    if( MiniMapPid != mapPid )
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

    uint16 minPX = uint16( -1 ), maxPX = 0, minPY = uint16( -1 ), maxPY = 0;
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

                minPX = std::min( minPX, px );
                maxPX = std::max( maxPX, px );
                minPY = std::min( minPY, py );
                maxPY = std::max( maxPY, py );
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

            if( px < minPX || px > maxPX || py < minPY || py > maxPY )
                continue;

            // check if current hex is going to be part of minimap

            bool show = false;
            uint color = 0;
            if( !GetHex( hexX, hexY, show, color ) || !show )
                continue;

            // adjust position

            px -= minPX;
            py -= minPY;

            // update minimap

            automap.Points.push_back( PGUI::DrawPoint( px, py, color ) );
        }
    }

    // finalize

    if( GUI->Debug )
        App.WriteLogF( _FUNC_, " : %d point%s\n", automap.Points.size(), automap.Points.size() != 1 ? "s" : "" );

    PGUI::Draw::DeleteCache( MiniMapData );
    MiniMapData = automap.NewCache();
    MiniMapPid = mapPid;

    SetSize( maxPX - minPX, maxPY - minPY );
    SetPositionAt( 5 + 1, -5 - -1 ); // top-right, 5px margin

    NeedUpdateMiniMap = false;
}

void FOC::Screen::MiniMap::DrawContent()
{
    if( !IsVisible )
        return;

    PGUI::Draw::RenderData( MiniMapData, GetLeft(), GetTop(), MiniMapZoom );
}
