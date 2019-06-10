#include <string>
#include <unordered_map>

#include "PGUI.Keyboard.h"


static std::unordered_map<uint8, std::string> KeyMap;
static bool                                   KeyMapInitialized = false;

static void InitKeyMap()
{
    if( KeyMapInitialized )
        return;

    KeyMap.clear();

    #define MAP_KEY( key )    KeyMap[PGUI::Keyboard::key] = # key
    MAP_KEY( KEY_ESCAPE );
    MAP_KEY( KEY_1 );
    MAP_KEY( KEY_2 );
    MAP_KEY( KEY_3 );
    MAP_KEY( KEY_4 );
    MAP_KEY( KEY_5 );
    MAP_KEY( KEY_6 );
    MAP_KEY( KEY_7 );
    MAP_KEY( KEY_8 );
    MAP_KEY( KEY_9 );
    MAP_KEY( KEY_0 );
    MAP_KEY( KEY_MINUS );
    MAP_KEY( KEY_EQUALS );
    MAP_KEY( KEY_BACKSPACE );
    MAP_KEY( KEY_TAB );
    MAP_KEY( KEY_Q );
    MAP_KEY( KEY_W );
    MAP_KEY( KEY_E );
    MAP_KEY( KEY_R );
    MAP_KEY( KEY_T );
    MAP_KEY( KEY_Y );
    MAP_KEY( KEY_U );
    MAP_KEY( KEY_I );
    MAP_KEY( KEY_O );
    MAP_KEY( KEY_P );
    MAP_KEY( KEY_LBRACKET );
    MAP_KEY( KEY_RBRACKET );
    MAP_KEY( KEY_RETURN );
    MAP_KEY( KEY_LCONTROL );
    MAP_KEY( KEY_A );
    MAP_KEY( KEY_S );
    MAP_KEY( KEY_D );
    MAP_KEY( KEY_F );
    MAP_KEY( KEY_G );
    MAP_KEY( KEY_H );
    MAP_KEY( KEY_J );
    MAP_KEY( KEY_K );
    MAP_KEY( KEY_L );
    MAP_KEY( KEY_SEMICOLON );
    MAP_KEY( KEY_APOSTROPHE );
    MAP_KEY( KEY_GRAVE );
    MAP_KEY( KEY_LSHIFT );
    MAP_KEY( KEY_BACKSLASH );
    MAP_KEY( KEY_Z );
    MAP_KEY( KEY_X );
    MAP_KEY( KEY_C );
    MAP_KEY( KEY_V );
    MAP_KEY( KEY_B );
    MAP_KEY( KEY_N );
    MAP_KEY( KEY_M );
    MAP_KEY( KEY_COMMA );
    MAP_KEY( KEY_PERIOD );
    MAP_KEY( KEY_SLASH );
    MAP_KEY( KEY_RSHIFT );
    MAP_KEY( KEY_MULTIPLY );
    MAP_KEY( KEY_LMENU );
    MAP_KEY( KEY_SPACE );
    MAP_KEY( KEY_CAPITAL );
    MAP_KEY( KEY_F1 );
    MAP_KEY( KEY_F2 );
    MAP_KEY( KEY_F3 );
    MAP_KEY( KEY_F4 );
    MAP_KEY( KEY_F5 );
    MAP_KEY( KEY_F6 );
    MAP_KEY( KEY_F7 );
    MAP_KEY( KEY_F8 );
    MAP_KEY( KEY_F9 );
    MAP_KEY( KEY_F10 );
    MAP_KEY( KEY_NUMLOCK );
    MAP_KEY( KEY_SCROLL );
    MAP_KEY( KEY_NUMPAD7 );
    MAP_KEY( KEY_NUMPAD8 );
    MAP_KEY( KEY_NUMPAD9 );
    MAP_KEY( KEY_SUBTRACT );
    MAP_KEY( KEY_NUMPAD4 );
    MAP_KEY( KEY_NUMPAD5 );
    MAP_KEY( KEY_NUMPAD6 );
    MAP_KEY( KEY_ADD );
    MAP_KEY( KEY_NUMPAD1 );
    MAP_KEY( KEY_NUMPAD2 );
    MAP_KEY( KEY_NUMPAD3 );
    MAP_KEY( KEY_NUMPAD0 );
    MAP_KEY( KEY_DECIMAL );
    MAP_KEY( KEY_F11 );
    MAP_KEY( KEY_F12 );
    MAP_KEY( KEY_NUMPADENTER );
    MAP_KEY( KEY_RCONTROL );
    MAP_KEY( KEY_DIVIDE );
    MAP_KEY( KEY_SYSRQ );
    MAP_KEY( KEY_RMENU );
    MAP_KEY( KEY_PAUSE );
    MAP_KEY( KEY_HOME );
    MAP_KEY( KEY_UP );
    MAP_KEY( KEY_PRIOR );
    MAP_KEY( KEY_LEFT );
    MAP_KEY( KEY_RIGHT );
    MAP_KEY( KEY_END );
    MAP_KEY( KEY_DOWN );
    MAP_KEY( KEY_NEXT );
    MAP_KEY( KEY_INSERT );
    MAP_KEY( KEY_DELETE );
    MAP_KEY( KEY_LWIN );
    MAP_KEY( KEY_RWIN );
    MAP_KEY( KEY_CLIPBOARD_PASTE );
    #undef MAP_KEY

    KeyMapInitialized = true;
}

std::string PGUI::Keyboard::GetKeyName( uint8 key )
{
    InitKeyMap();

    std::string result;
    auto        it = KeyMap.find( key );

    if( it != KeyMap.end() )
        result = it->second;

    return result;
}

bool PGUI::Keyboard::IsNumber( uint8 key )
{
    return key >= PGUI::Keyboard::KEY_1 && key <= PGUI::Keyboard::KEY_0;
}

bool PGUI::Keyboard::IsLetter( uint8 key )
{
    return (key >= PGUI::Keyboard::KEY_Q && key <= PGUI::Keyboard::KEY_P) ||
           (key >= PGUI::Keyboard::KEY_A && key <= PGUI::Keyboard::KEY_L) ||
           (key >= PGUI::Keyboard::KEY_Z && key <= PGUI::Keyboard::KEY_M);
}
