#ifndef __FOC__MAPPER__
#define __FOC__MAPPER__

#include <Extension.h>

#include "FOC.h"

namespace FOC
{
    class Mapper : public Extension
    {
public:
        Mapper();
        virtual ~Mapper();

        virtual size_t GetFunctionAddress( const std::string& name ) override;
    };
};

#endif // __FOC__MAPPER__ //
