#ifndef __PGUI__EVENT__
#define __PGUI__EVENT__

#include <functional>

#include <Types.h>

namespace PGUI
{
    class Button;
    class Screen;

    namespace Event
    {
        typedef std::function<void (uint, PGUI::Button*)>       UIntButton;

        typedef std::function<void (uint, PGUI::Screen*)>       UIntScreen;
        typedef std::function<void (uint, PGUI::Screen*, bool)> UIntScreenBool;
    };
};

#endif // __PGUI__EVENT__ //
