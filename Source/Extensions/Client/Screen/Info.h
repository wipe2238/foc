#ifndef __CLIENT__SCREEN__INFO__
#define __CLIENT__SCREEN__INFO__

#include "PGUI/PGUI.Screen.h"

namespace FOC
{
    namespace Screen
    {
        class Info : public PGUI::Screen
        {
public:
            struct ID
            {};

            Info( PGUI::Core* gui );
            virtual ~Info();
        };
    };
};

#endif // __CLIENT__SCREEN__INFO__ //
