#ifndef __CLIENT_INTERFACE__
#define __CLIENT_INTERFACE__

#include <Addons/scriptarray.h>
#include <Defines.Public.h>
#include <Types.h>

#define TB_DEBUG      "Debug"
#define TI_FPS        "00:FPS"
#define TI_SCREENS    "01:Screens"

namespace FOC
{
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
        std::set<int> Screens;

public:
        std::map<std::string, TextBlock> TextBlocks;

public:
        InterfaceManager();
        ~InterfaceManager();

        // reserved functions processing
        void ProcessScreenChange( bool show, int screen, int p0, int p1, int p2 );
        void ProcessGetActiveScreens( ScriptArray& screens );
        void ProcessRenderInterface( uint layer );

        void DrawTextBlocks();
    };
};

#endif // __CLIENT_INTERFACE__ //
