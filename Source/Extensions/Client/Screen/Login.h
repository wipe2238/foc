#ifndef __CLIENT__SCREEN__LOGIN__
#define __CLIENT__SCREEN__LOGIN__

#include <string>

#include "PGUI/PGUI.Screen.h"
#include "PGUI/PGUI.TextBox.h"

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
            };

private:
            uint8 ActiveElement;

public:
            Login( PGUI::Core* gui );

            virtual void OnOpen() override;
            virtual void OnClose() override;
            virtual void OnActive( bool active ) override;
public:
            virtual bool KeyDown( const uint8& key, const std::string& keyString );
        };
    };
};

#endif // __CLIENT__SCREEN__LOGIN__ //
