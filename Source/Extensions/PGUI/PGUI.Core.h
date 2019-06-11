#ifndef __PGUI__
#define __PGUI__

#include <list>
#include <map>
#include <set>
#include <string>

#include <Types.h>

#include "PGUI.Event.h"

class Ini;

namespace PGUI
{
    class Screen;

    struct CoreSettings
    {
        uint  ColorBackground;
        uint  ColorBorder;
        uint  ColorFont;
        uint  ColorItem;

        uint  FontType;
        uint  FontFlags;

        uint8 TextBoxMargin;

        CoreSettings();
    };

    class Core
    {
public:
        bool         Debug;
        bool         DebugDraw;
        bool         IsDrawEnabled;
        bool         IsUpdateEnabled;
        bool         IsKeyboardEnabled;
        bool         IsMouseEnabled;

        CoreSettings Settings;

private:
        std::map<uint, PGUI::Screen*> AllScreens;
        std::list<uint>               OpenScreens;
        PGUI::Screen*                 ActiveScreen;

        std::set<uint8>               KeyboardPressed;
        std::set<int>                 MousePressed;

public:
        Core( bool visible = true );
        virtual ~Core();

        //

        int16 GetScreenWidth();
        int16 GetScreenHeight();
        void  GetScreenSize( int16& width, int16& height );

        bool LoadSettings( Ini* ini, const std::string& section );

        //

        bool IsScreen( uint id );
        bool IsScreenOpen( uint id );

        PGUI::Screen* NewScreen( uint id, bool add = true );

        bool AddScreen( uint id, PGUI::Screen* screen );
        bool RemoveScreen( uint id );
        void RemoveAllScreens();

        bool OpenScreen( uint id );
        void OpenAllScreens();
        bool CloseScreen( uint id );
        void CloseAllScreens();
        void ToggleScreen( uint id );

        PGUI::Screen*   GetScreen( uint id );
        uint            GetScreenID( PGUI::Screen* screen );
        PGUI::Screen*   GetOpenScreen( uint id );
        PGUI::Screen*   GetActiveScreen();
        uint            GetActiveScreenID();
        std::list<uint> GetAllScreenList();
        std::list<uint> GetOpenScreenList();

        // input

        bool IsKeyPressed( uint8 key );
        bool IsMousePressed( int click );

private:
        void SetActiveScreen( PGUI::Screen* screen );

public:
        Event::UIntScreen     EventScreenAdd;
        Event::UIntScreen     EventScreenRemove;
        Event::UIntScreen     EventScreenOpen;
        Event::UIntScreen     EventScreenClose;
        Event::UIntScreenBool EventScreenActive;

public:
        void Update();                                            // reserved_function : loop
        void Draw( uint8 layer );                                 // reserved function : render_iface
        void DrawMap();                                           // reserved function : render_map
        bool KeyDown( uint8 key, std::string& keyString );        // reserved_function : key_down
        bool KeyUp( uint8 key, std::string& keyString );          // reserved_function : key_up
        bool MouseDown( int click, int x, int y );                // reserved_function : mouse_down
        void MouseMove( int fromX, int fromY, int toX, int toY ); // reserved function : mouse_move
        bool MouseUp( int click, int x, int y );                  // reserved_function : mouse_up
        void InputLost();                                         // reserved_function : input_lost

        void NOP() {}
    };
};

#endif // __PGUI__ //
