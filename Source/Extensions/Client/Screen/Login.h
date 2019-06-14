#ifndef __CLIENT__SCREEN__LOGIN__
#define __CLIENT__SCREEN__LOGIN__

#include <string>

#include "PGUI/PGUI.Button.h"
#include "PGUI/PGUI.Screen.h"

namespace FOC
{
    namespace Screen
    {
        class Login : public PGUI::Screen
        {
public:
            struct ID
            {
                static const uint8 UserName = 1;
                static const uint8 UserPass = 2;
                static const uint8 ButtonRegister = 3;
                static const uint8 ButtonExit = 4;
            };

private:
            uint8 ActiveElement;

public:
            Login( PGUI::Core* gui );

public:
            virtual bool KeyDown( uint8 key, std::string& keyText );

            virtual void OnOpen( bool& modal ) override;
            virtual void OnClose() override;
            virtual void OnTop( bool active ) override;

            void OnButton( uint id, PGUI::Button* button );
        };
    };
};

#endif // __CLIENT__SCREEN__LOGIN__ //
