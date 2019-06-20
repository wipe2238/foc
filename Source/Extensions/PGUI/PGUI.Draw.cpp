#include <App.h>
#include <Defines.Public.h>
#include <Types.h>

#if defined (FOCLASSIC_CLIENT) || defined (FOCLASSIC_MAPPER)
# include <SpriteManager.h>
#endif

#include "PGUI.Draw.h"

static void FunctionNotAvailable( const char* func )
{
    App.WriteLogF( func, " ERROR : function not available\n" );
}

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

    if( !width )
    {
        App.WriteLogF( _FUNC_, " ERROR : no width\n" );
        return;
    }

    if( !height )
    {
        App.WriteLogF( _FUNC_, " ERROR : no height\n" );
        return;
    }

    PointsType = DRAW_PRIMITIVE_TRIANGLELIST;

    int16 right = left + width;
    int16 bottom = top + height;

    AddPoint( left, bottom, color );
    AddPoint( left, top, color );
    AddPoint( right, bottom, color );
    AddPoint( left, top, color );
    AddPoint( right, top, color );
    AddPoint( right, bottom, color );
}

void PGUI::DrawData::MakeRectangleFrame( int16 left, int16 top, uint16 width, uint16 height, uint color, uint8 thickness /* = 1 */ )
{
    Clear();

    if( !width )
    {
        App.WriteLogF( _FUNC_, " ERROR : no width\n" );
        return;
    }

    if( !height )
    {
        App.WriteLogF( _FUNC_, " ERROR : no height\n" );
        return;
    }

    if( !thickness )
    {
        App.WriteLogF( _FUNC_, " ERROR : no thickness\n" );
        return;
    }

    PointsType = DRAW_PRIMITIVE_LINESTRIP;

    int16 right = left + width;
    int16 bottom = top + height;

    for( uint t = 0; t < thickness; t++ )
    {
        AddPoint( left, top, color );
        AddPoint( right, top, color );
        AddPoint( right, bottom, color );
        AddPoint( left, bottom, color );
        AddPoint( left, top, color );

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

PGUI::DrawData* PGUI::DrawData::Copy()
{
    DrawData* data = new DrawData( PointsType );

    for( auto it = Points.begin(), end = Points.end(); it != end; ++it )
    {
        auto& point = *it;

        data->Points.push_back( PrepPoint( point.PointX, point.PointY, point.PointColor, point.PointOffsX, point.PointOffsY ) );
    }

    return data;
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

void PGUI::Draw::RenderData( PGUI::DrawData* data, int16 offsetX /* = 0 */, int16 offsetY /* = 0 */, float zoom /* = 1.0f */ )
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

void PGUI::Draw::RenderText( const std::string& text, uint color, uint8 font, uint flags, int16 left, int16 top, uint16 width, uint16 height )
{
    #if defined (FOCLASSIC_CLIENT) || defined (FOCLASSIC_MAPPER)
    Rect rect = Rect( left, top, left + width, top + height );

    SprMngr.DrawStr( rect, text.c_str(), flags, color, font );
    #else
    FunctionNotAvailable( _FUNC_ );
    #endif
}

void PGUI::Draw::DeleteData( PGUI::DrawData*& data )
{
    if( data )
    {
        delete data;
        data = nullptr;
    }
}
