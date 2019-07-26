#ifndef __FOC__
#define __FOC__

#include <map>
#include <string>

#include <Extension.h>

#if defined (FOCLASSIC_CLIENT)
# define FOC_SELF_CLASS                   Client
#elif defined (FOCLASSIC_MAPPER)
# define FOC_SELF_CLASS                   Mapper
#elif defined (FOCLASSIC_SERVER)
# define FOC_SELF_CLASS                   Server
#endif

#define GET_ADDRESS_X( func, freal ) \
    if( name == # func )             \
        return (size_t)&freal
#define GET_ADDRESS( func )               GET_ADDRESS_X( func, func )
#define GET_ADDRESS_NSX( func, freal )    GET_ADDRESS_X( func, FOC::freal )
#define GET_ADDRESS_NS( func )            GET_ADDRESS_NSX( func, func )

namespace FOC
{
    template<typename T>
    void Init( bool event_debug = true )
    {
        Extension::Map["FOC"] = new T();
        Extension::RunEventDebug = event_debug;
    }

    template<typename T>
    void Finish()
    {
        delete (T*)Extension::Map["FOC"];
    }

    class FOC_SELF_CLASS;

    inline FOC::FOC_SELF_CLASS* Self()
    {
        return (FOC::FOC_SELF_CLASS*)Extension::Map["FOC"];
    }

    inline bool SelfCheck()
    {
        return Extension::Map.find( "FOC" ) != Extension::Map.end();
    }
};

#endif // __FOC__ //
