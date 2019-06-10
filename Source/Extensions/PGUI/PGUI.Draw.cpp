#include <App.h>
#include <Defines.Public.h>
#include <Types.h>

#if defined (FOCLASSIC_CLIENT) || defined (FOCLASSIC_MAPPER)
# include <SpriteManager.h>
#elif defined (FOCLASSIC_SERVER)
struct PrepPoint
{
    short  PointX;
    short  PointY;
    short* PointOffsX;
    short* PointOffsY;
    uint   PointColor;

    PrepPoint() : PointX( 0 ), PointY( 0 ), PointColor( 0 ), PointOffsX( NULL ), PointOffsY( NULL ) {}
    PrepPoint( short x, short y, uint color, short* ox = NULL, short* oy = NULL ) : PointX( x ), PointY( y ), PointColor( color ), PointOffsX( ox ), PointOffsY( oy ) {}
};
typedef std::vector<PrepPoint> PointVec;
#endif

#include "PGUI.Draw.h"

template<typename F, typename T, typename P>
void CopyDrawData( F* from, T* to )
{
    to->PointsType = from->PointsType;

    for( auto it = from->Points.begin(), end = from->Points.end(); it != end; ++it )
    {
        auto& point = *it;

        to->Points.push_back( P( point.PointX, point.PointY, point.PointColor, point.PointOffsX, point.PointOffsY ) );
    }
}

static void FunctionNotAvailable( const char* func )
{
    App.WriteLogF( func, " ERROR : function not available\n" );
}

namespace PGUI
{
    struct DrawCache
    {
        uint8    PointsType;
        PointVec Points;

        ~DrawCache()
        {
            Points.clear();
        }
    };
};

PGUI::DrawPoint::DrawPoint() :
// public
    PointX( 0 ), PointY( 0 ), PointColor( 0 ),
    PointOffsX( NULL ), PointOffsY( NULL ) {}

PGUI::DrawPoint::DrawPoint( int16 x, int16 y, uint color, int16* ox /* = NULL */, int16* oy /* = NULL */ ) :
// public
    PointX( x ), PointY( y ), PointColor( color ),
    PointOffsX( ox ), PointOffsY( oy ) {}

PGUI::DrawData::DrawData( uint8 type ) :
// public
    PointsType( type ), Points()
{}

PGUI::DrawData::~DrawData()
{
    Clear();
}

void PGUI::DrawData::MakeRectangle( int16 left, int16 top, uint16 width, uint16 height, uint color )
{
    Clear();

    if( !width || !height )
        return;

    PointsType = DRAW_PRIMITIVE_TRIANGLELIST;

    int16 right = left + width;
    int16 bottom = top + height;

    Points.push_back( PGUI::DrawPoint( left, bottom, color ) );
    Points.push_back( PGUI::DrawPoint( left, top, color ) );
    Points.push_back( PGUI::DrawPoint( right, bottom, color ) );
    Points.push_back( PGUI::DrawPoint( left, top, color ) );
    Points.push_back( PGUI::DrawPoint( right, top, color ) );
    Points.push_back( PGUI::DrawPoint( right, bottom, color ) );
}

void PGUI::DrawData::MakeRectangleFrame( int16 left, int16 top, uint16 width, uint16 height, uint color, uint8 thickness /* = 1 */ )
{
    Clear();

    if( !width || !height || !thickness )
        return;

    PointsType = DRAW_PRIMITIVE_LINESTRIP;

    int16 right = left + width;
    int16 bottom = top + height;

    for( uint t = 0; t < thickness; t++ )
    {
        Points.push_back( PGUI::DrawPoint( left, top, color ) );
        Points.push_back( PGUI::DrawPoint( right, top, color ) );
        Points.push_back( PGUI::DrawPoint( right, bottom, color ) );
        Points.push_back( PGUI::DrawPoint( left, bottom, color ) );
        Points.push_back( PGUI::DrawPoint( left, top, color ) );

        left--;
        top--;
        right++;
        bottom++;
    }
}

void PGUI::DrawData::Clear()
{
    PointsType = 0;
    Points.clear();
}

PGUI::DrawCache* PGUI::DrawData::NewCache()
{
    DrawCache* cache = new DrawCache();
    cache->PointsType = PointsType;

    for( auto it = Points.begin(), end = Points.end(); it != end; ++it )
    {
        auto& point = *it;

        cache->Points.push_back( PrepPoint( point.PointX, point.PointY, point.PointColor, point.PointOffsX, point.PointOffsY ) );
    }

    return cache;
}

//

void PGUI::Draw::GetTextInfo( const std::string& text, uint8 font, uint flags, uint16& width, uint16& height, uint& lines, uint16 maxWidth /* = 0 */, uint16 maxHeight /* = 0 */ )
{
    #if defined (FOCLASSIC_CLIENT) || defined (FOCLASSIC_MAPPER)
    int w = 0, h = 0, l = 0;
    SprMngr.GetTextInfo( maxWidth, maxHeight, text.c_str(), font, flags, w, h, l );

    width = CLAMP( w, 0, uint16( -1 ) );
    height = CLAMP( h, 0, uint16( -1 ) );
    lines = CLAMP( l, 0, uint( -1 ) );

    // App.WriteLogF( _FUNC_, "(\"%s\",%u,%u,%u,%u) = %d,%d,%d\n", text.c_str(), font, flags, maxWidth, maxHeight, width, height, lines );
    #else
    FunctionNotAvailable( _FUNC_ );
    #endif
}

void PGUI::Draw::RenderData( PGUI::DrawCache* data, int16 offsetX /* = 0 */, int16 offsetY /* = 0 */, float zoom /* = 1.0f */ )
{
    #if defined (FOCLASSIC_CLIENT) || defined (FOCLASSIC_MAPPER)
    if( !data )
    {
        App.WriteLogF( _FUNC_, " ERROR : data is null\n" );
        return;
    }

    if( data->PointsType < DRAW_PRIMITIVE_POINTLIST || data->PointsType > DRAW_PRIMITIVE_TRIANGLEFAN )
    {
        App.WriteLogF( _FUNC_, " ERROR : invalid data type<%u>\n", data->PointsType );
        return;
    }

    static PointF offset;

    offset.X = static_cast<float>(offsetX);
    offset.Y = static_cast<float>(offsetY);

    SprMngr.DrawPoints( data->Points, data->PointsType, &zoom, nullptr, &offset );
    #else
    FunctionNotAvailable( _FUNC_ );
    #endif
}

void PGUI::Draw::RenderData( DrawData& data, int16 offsetX /* = 0 */, int16 offsetY /* = 0 */, float zoom /* = 1.0f */ )
{
    #if defined (FOCLASSIC_CLIENT) || defined (FOCLASSIC_MAPPER)
    PGUI::DrawCache* cache = data.NewCache();

    RenderData( cache, offsetX, offsetY, zoom );
    #else
    FunctionNotAvailable( _FUNC_ );
    #endif
}

void PGUI::Draw::RenderText( const std::string& text, uint color, uint8 font, uint flags, int16 left, int16 top, uint16 width, uint16 height )
{
    #if defined (FOCLASSIC_CLIENT) || defined (FOCLASSIC_MAPPER)
    Rect rect = Rect( left, top, left + width, top + height );

    SprMngr.DrawStr( rect, text.c_str(), flags, color, font );
    #else
    FunctionNotAvailable( _FUNC_ );
    #endif
}

void PGUI::Draw::DeleteCache( PGUI::DrawCache* data )
{
    if( data )
    {
        delete data;
        data = nullptr;
    }
}