#ifndef __PGUI__DRAW__
#define __PGUI__DRAW__

#include <string>

#include <Types.h>

// include this file *after* all engine headers

#if !defined (PGUI_SKIP_PREP_POINT_VEC) && !defined (__SPRITE_MANAGER__)
struct PrepPoint
{
    int16  PointX;
    int16  PointY;
    int16* PointOffsX;
    int16* PointOffsY;
    uint   PointColor;

    PrepPoint() : PointX( 0 ), PointY( 0 ), PointColor( 0 ), PointOffsX( NULL ), PointOffsY( NULL ) {}
    PrepPoint( int16 x, int16 y, uint color, int16* ox = NULL, int16* oy = NULL ) : PointX( x ), PointY( y ), PointColor( color ), PointOffsX( ox ), PointOffsY( oy ) {}
};
#endif

namespace PGUI
{
    class DrawData
    {
public:
        uint8                  PointsType;
        std::vector<PrepPoint> Points;

        DrawData( uint8 type = 0 );
        ~DrawData();

        inline void PGUI::DrawData::AddPoint( int16 x, int16 y, uint color, int16* offsetX = NULL, int16* offsetY = NULL )
        {
            Points.push_back( PrepPoint( x, y, color, offsetX, offsetY ) );
        }

        void MakeRectangle( int16 left, int16 top, uint16 width, uint16 height, uint color );
        void MakeRectangleFrame( int16 left, int16 top, uint16 width, uint16 height, uint color, uint8 thickness = 1 );

        void      Clear();
        DrawData* Copy();
    };

    struct Draw
    {
        static void GetTextInfo( const std::string& text, uint8 font, uint flags, uint16& width, uint16& height, uint& lines, uint16 maxWidth = 0, uint16 maxHeight = 0 );

        static void RenderData( PGUI::DrawData* data, int16 offsetX = 0, int16 offsetY = 0, float zoom = 1.0f );
        static void RenderText( const std::string& text, uint color, uint8 font, uint flags, int16 left, int16 top, uint16 width, uint16 height );

        static void DeleteData( PGUI::DrawData*& data );
    };
}

#endif // __PGUI__DRAW__ //
