#ifndef __FOC__SERVER__
#define __FOC__SERVER__

#include <Extension.h>

#include "FOC.h"

namespace FOC
{
    class DialogsManager;
    class LockersManager;
    class WorldMapManager;

    class Server : public Extension
    {
public:
        Server();
        virtual ~Server();

        DialogsManager*  Dialogs;
        LockersManager*  Lockers;
        WorldMapManager* WorldMap;

        virtual size_t GetFunctionAddress( const std::string& name ) override;
    };
};

#endif // __FOC__SERVER__ //
