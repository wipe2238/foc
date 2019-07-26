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
private:
        std::map<std::string, size_t> FunctionAddressMap;

public:
        Server();
        virtual ~Server();

        DialogsManager*  Dialogs;
        LockersManager*  Lockers;
        WorldMapManager* WorldMap;

        virtual size_t GetFunctionAddress( const std::string& name ) override;
        std::string    GetFunctionName( const size_t address, bool prefix = true );
        void           SetFunctionAddress( const std::string& name, const size_t address );
    };
};

#endif // __FOC__SERVER__ //
