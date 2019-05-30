#ifndef __BUFFER__
#define __BUFFER__

#include <Addons/scriptarray.h>
#include <Types.h>

class Buffer
{
protected:
    uint8* BufData;
    uint32 BufLen;

    uint32 BufRead;
    uint32 BufWrite;

    // AngelScript
    mutable int32 RefCount;

public:

    bool Debug;

    Buffer();
    ~Buffer();

protected:
    void Grow( uint32 len );

public:
    virtual Buffer& operator<<( bool value );
    virtual Buffer& operator>>( bool& value );

    virtual Buffer& operator<<( int8 value );
    virtual Buffer& operator>>( int8& value );

    virtual Buffer& operator<<( int16 value );
    virtual Buffer& operator>>( int16& value );

    virtual Buffer& operator<<( int32 value );
    virtual Buffer& operator>>( int32& value );

    virtual Buffer& operator<<( uint8 value );
    virtual Buffer& operator>>( uint8& value );

    virtual Buffer& operator<<( uint16 value );
    virtual Buffer& operator>>( uint16& value );

    virtual Buffer& operator<<( uint32 value );
    virtual Buffer& operator>>( uint32& value );

    /*
       Buffer& Store( void* ref, int refTypeId );
       Buffer& Fetch( void* ref, int refTypeId );
     */

    #define SCRIPTARRAY_STORE    ScriptArray*
    #define SCRIPTARRAY_FETCH    ScriptArray&

    virtual Buffer& operator<<( SCRIPTARRAY_STORE arr );
    virtual Buffer& operator>>( SCRIPTARRAY_FETCH arr );

    void AddRef() const;
    void Release() const;
};

extern void RegisterBuffer( asIScriptEngine* engine );

#endif // __BUFFER__ //
