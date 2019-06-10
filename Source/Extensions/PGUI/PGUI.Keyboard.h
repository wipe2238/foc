#ifndef __PGUI__KEYBOARD__
#define __PGUI__KEYBOARD__

#include <string>

#include <Types.h>

namespace PGUI
{
    struct Keyboard
    {
        static const uint8 KEY_ESCAPE = 0x01;
        static const uint8 KEY_1 = 0x02;
        static const uint8 KEY_2 = 0x03;
        static const uint8 KEY_3 = 0x04;
        static const uint8 KEY_4 = 0x05;
        static const uint8 KEY_5 = 0x06;
        static const uint8 KEY_6 = 0x07;
        static const uint8 KEY_7 = 0x08;
        static const uint8 KEY_8 = 0x09;
        static const uint8 KEY_9 = 0x0A;
        static const uint8 KEY_0 = 0x0B;
        static const uint8 KEY_MINUS = 0x0C;                               // - on main keyboard
        static const uint8 KEY_EQUALS = 0x0D;
        static const uint8 KEY_BACKSPACE = 0x0E;                           // backspace
        static const uint8 KEY_TAB = 0x0F;
        static const uint8 KEY_Q = 0x10;
        static const uint8 KEY_W = 0x11;
        static const uint8 KEY_E = 0x12;
        static const uint8 KEY_R = 0x13;
        static const uint8 KEY_T = 0x14;
        static const uint8 KEY_Y = 0x15;
        static const uint8 KEY_U = 0x16;
        static const uint8 KEY_I = 0x17;
        static const uint8 KEY_O = 0x18;
        static const uint8 KEY_P = 0x19;
        static const uint8 KEY_LBRACKET = 0x1A;
        static const uint8 KEY_RBRACKET = 0x1B;
        static const uint8 KEY_RETURN = 0x1C;                              // enter on main keyboard
        static const uint8 KEY_LCONTROL = 0x1D;
        static const uint8 KEY_A = 0x1E;
        static const uint8 KEY_S = 0x1F;
        static const uint8 KEY_D = 0x20;
        static const uint8 KEY_F = 0x21;
        static const uint8 KEY_G = 0x22;
        static const uint8 KEY_H = 0x23;
        static const uint8 KEY_J = 0x24;
        static const uint8 KEY_K = 0x25;
        static const uint8 KEY_L = 0x26;
        static const uint8 KEY_SEMICOLON = 0x27;
        static const uint8 KEY_APOSTROPHE = 0x28;
        static const uint8 KEY_GRAVE = 0x29;                               // accent grave
        static const uint8 KEY_LSHIFT = 0x2A;
        static const uint8 KEY_BACKSLASH = 0x2B;
        static const uint8 KEY_Z = 0x2C;
        static const uint8 KEY_X = 0x2D;
        static const uint8 KEY_C = 0x2E;
        static const uint8 KEY_V = 0x2F;
        static const uint8 KEY_B = 0x30;
        static const uint8 KEY_N = 0x31;
        static const uint8 KEY_M = 0x32;
        static const uint8 KEY_COMMA = 0x33;                               // ,
        static const uint8 KEY_PERIOD = 0x34;                              // . on main keyboard
        static const uint8 KEY_SLASH = 0x35;                               // / on main keyboard
        static const uint8 KEY_RSHIFT = 0x36;
        static const uint8 KEY_MULTIPLY = 0x37;                            // * on numeric keypad
        static const uint8 KEY_LMENU = 0x38;                               // left alt
        static const uint8 KEY_SPACE = 0x39;
        static const uint8 KEY_CAPITAL = 0x3A;                             // caps lock
        static const uint8 KEY_F1 = 0x3B;
        static const uint8 KEY_F2 = 0x3C;
        static const uint8 KEY_F3 = 0x3D;
        static const uint8 KEY_F4 = 0x3E;
        static const uint8 KEY_F5 = 0x3F;
        static const uint8 KEY_F6 = 0x40;
        static const uint8 KEY_F7 = 0x41;
        static const uint8 KEY_F8 = 0x42;
        static const uint8 KEY_F9 = 0x43;
        static const uint8 KEY_F10 = 0x44;
        static const uint8 KEY_NUMLOCK = 0x45;
        static const uint8 KEY_SCROLL = 0x46;                              // scroll lock
        static const uint8 KEY_NUMPAD7 = 0x47;
        static const uint8 KEY_NUMPAD8 = 0x48;
        static const uint8 KEY_NUMPAD9 = 0x49;
        static const uint8 KEY_SUBTRACT = 0x4A;                            // - on numeric keypad
        static const uint8 KEY_NUMPAD4 = 0x4B;
        static const uint8 KEY_NUMPAD5 = 0x4C;
        static const uint8 KEY_NUMPAD6 = 0x4D;
        static const uint8 KEY_ADD = 0x4E;                                 // + on numeric keypad
        static const uint8 KEY_NUMPAD1 = 0x4F;
        static const uint8 KEY_NUMPAD2 = 0x50;
        static const uint8 KEY_NUMPAD3 = 0x51;
        static const uint8 KEY_NUMPAD0 = 0x52;
        static const uint8 KEY_DECIMAL = 0x53;                             // . on numeric keypad
        static const uint8 KEY_F11 = 0x57;
        static const uint8 KEY_F12 = 0x58;
        static const uint8 KEY_NUMPADENTER = 0x9C;                         // enter on numeric keypad
        static const uint8 KEY_RCONTROL = 0x9D;
        static const uint8 KEY_DIVIDE = 0xB5;                              // / on numeric keypad
        static const uint8 KEY_SYSRQ = 0xB7;
        static const uint8 KEY_RMENU = 0xB8;                               // right alt
        static const uint8 KEY_PAUSE = 0xC5;                               // pause
        static const uint8 KEY_HOME = 0xC7;                                // home on arrow keypad
        static const uint8 KEY_UP = 0xC8;                                  // up arrow on arrow keypad
        static const uint8 KEY_PRIOR = 0xC9;                               // page up on arrow keypad
        static const uint8 KEY_LEFT = 0xCB;                                // left arrow on arrow keypad
        static const uint8 KEY_RIGHT = 0xCD;                               // right arrow on arrow keypad
        static const uint8 KEY_END = 0xCF;                                 // end on arrow keypad
        static const uint8 KEY_DOWN = 0xD0;                                // down arrow on arrow keypad
        static const uint8 KEY_NEXT = 0xD1;                                // page down on arrow keypad
        static const uint8 KEY_INSERT = 0xD2;                              // insert on arrow keypad
        static const uint8 KEY_DELETE = 0xD3;                              // delete on arrow keypad
        static const uint8 KEY_LWIN = 0xDB;                                // left windows key
        static const uint8 KEY_RWIN = 0xDC;                                // right windows key
        static const uint8 KEY_CLIPBOARD_PASTE = 0xFF;

        //

        static std::string GetKeyName( uint8 key );
        static bool        IsNumber( uint8 key );
        static bool        IsLetter( uint8 key );
    };
}
#endif // __PGUI__KEYBOARD__ //
