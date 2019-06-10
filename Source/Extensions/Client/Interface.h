#ifndef __CLIENT_INTERFACE__
#define __CLIENT_INTERFACE__

#include <Addons/scriptarray.h>
#include <Defines.Public.h>
#include <Types.h>

#include "PGUI/PGUI.Core.h"

#define TB_DEBUG      "Debug"
#define TI_FPS        "00:FPS"
#define TI_SCREENS    "01:Engine"
#define TI_SCREENZ    "02:PGUI"
#define TI_COORD      "03:Coords"
#define TI_FIELD      "04:Field"

namespace FOC
{
    namespace Screen
    {
        class Login;
        class Game;
        class MiniMap;
    };

    class InterfaceManager
    {
public:
        struct TextInfo
        {
            std::string Text;
            uint8       Font;
            uint        Color;
            uint        Flags;

            bool        ShowName;

            TextInfo() :
                Text( "" ), Font( FONT_TYPE_DEFAULT ), Color( 0 ), Flags( FONT_FLAG_BORDERED ),
                ShowName( true )
            {}
        };

        struct TextBlock
        {
            uint                            StartX;
            uint                            StartY;
            uint                            SpacingY;

            std::map<std::string, TextInfo> Map;

            TextBlock() : StartX( 0 ), StartY( 0 ), SpacingY( 2 ) {}
        };

private:
        std::set<int> CustomScreens;
        std::set<int> EngineScreens;


        struct MouseData
        {
            int LastX;
            int LastY;
        };

        MouseData                        Mouse;
public:
        PGUI::Core*                      UI;

        std::map<std::string, TextBlock> TextBlocks;

public:
        InterfaceManager();
        ~InterfaceManager();

        void UI_ScreenAdd( uint id, PGUI::Screen* screen );
        void UI_ScreenRemove( uint id, PGUI::Screen* screen );

        // reserved functions processing
        bool ProcessStart();
        void ProcessFinish();
        void ProcessLoop();
        void ProcessScreenChange( bool show, int screen, int p0, int p1, int p2 );
        void ProcessGetActiveScreens( ScriptArray& screens );
        void ProcessRenderInterface( uint layer );
        void ProcessRenderMap();
        bool ProcessKeyDown( uint8 key, std::string& keyString );
        bool ProcessKeyUp( uint8 key, std::string& keyString );
        bool ProcessMouseDown( int click );
        void ProcessMouseMove( int x, int y );
        bool ProcessMouseUp( int click );
        void ProcessInputLost();

        void DrawTextBlocks();
    };
};

#endif // __CLIENT_INTERFACE__ //
