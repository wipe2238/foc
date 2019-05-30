#ifndef __FOC__CLIENT__
#define __FOC__CLIENT__

#include <Extension.h>

#include "FOC.h"

namespace FOC
{
    class AnimationsManager;
    class InterfaceManager;

    class Client : public Extension
    {
public:
        Client();
        virtual ~Client();

        virtual size_t GetFunctionAddress( const std::string& name ) override;

        AnimationsManager* Animations;
        InterfaceManager*  Interface;
    };
};

#endif // __FOC__CLIENT__ //
