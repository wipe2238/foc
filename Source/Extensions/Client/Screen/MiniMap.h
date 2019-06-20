#ifndef __CLIENT__SCREEN__MINI_MAP__
#define __CLIENT__SCREEN__MINI_MAP__

#include "PGUI/PGUI.Screen.h"

namespace FOC
{
    namespace Screen
    {
        class MiniMap : public PGUI::Screen
        {
public:
            struct MapHex
            {
                bool Show;
                uint Color;

                MapHex( bool show, uint8 r, uint8 g, uint8 b, uint8 alpha = 255 );
            };

            uint   MiniMapPid;

            MapHex ScrollBlock;
            MapHex ExitGrid;
            MapHex Wall;
            MapHex Scenery;

private:
            struct Bounds
            {
                uint16 MinPX;
                uint16 MinPY;
                uint16 MaxPX;
                uint16 MaxPY;

                uint16 Width;
                uint16 Height;

                Bounds();
            };

            Bounds          MiniMapBounds;
            float           MiniMapZoom;
            PGUI::DrawData* MiniMapData;
            bool            NeedUpdateMiniMap;

public:
            MiniMap( PGUI::Core* gui );
            virtual ~MiniMap();

            bool GetHex( uint16 hexX, uint16 hexY, bool& show, uint& color );
            void SetShow( MapHex& object, bool show );
            void SetColor( MapHex& object, uint8 r, uint8 g, uint8 b, uint8 alpha = 255 );
            void SetObject( MapHex& object, bool show, uint8 r, uint8 g, uint8 b, uint8 alpha = 255 );


public:
            virtual void Update() override;
            void         UpdateMiniMap();
            void         UpdateMiniMapZoom( float zoom );

            virtual void DrawContent() override;

            virtual bool KeyDown( uint8 key, std::string& keyText ) override;
        };
    };
};

#endif // __CLIENT__SCREEN__MINI_MAP__ //
