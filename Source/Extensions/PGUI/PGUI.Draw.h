#ifndef __PGUI__DRAW__
#define __PGUI__DRAW__

#include <string>

#include <Types.h>

namespace PGUI
{
    struct DrawCache;

    struct DrawPoint
    {
        int16  PointX;
        int16  PointY;
        int16* PointOffsX;
        int16* PointOffsY;
        uint   PointColor;

        DrawPoint();
        DrawPoint( int16 x, int16 y, uint color, int16* ox = NULL, int16* oy = NULL );
    };

    struct DrawData
    {
        uint8                  PointsType;
        std::vector<DrawPoint> Points;

        DrawData( uint8 type = 0 );
        ~DrawData();

        void MakeRectangle( int16 left, int16 top, uint16 width, uint16 height, uint color );
        void MakeRectangleFrame( int16 left, int16 top, uint16 width, uint16 height, uint color, uint8 thickness = 1 );

        void       Clear();
        DrawCache* NewCache();
    };

    struct Draw
    {
        static void GetTextInfo( const std::string& text, uint8 font, uint flags, uint16& width, uint16& height, uint& lines, uint16 maxWidth = 0, uint16 maxHeight = 0 );

        static void RenderData( PGUI::DrawCache* data, int16 offsetX = 0, int16 offsetY = 0, float zoom = 1.0f );
        static void RenderData( PGUI::DrawData& data, int16 offsetX = 0, int16 offsetY = 0, float zoom = 1.0f );
        static void RenderText( const std::string& text, uint color, uint8 font, uint flags, int16 left, int16 top, uint16 width, uint16 height );

        static void DeleteCache( PGUI::DrawCache*& data );
    };
}

#endif // __PGUI__DRAW__ //
