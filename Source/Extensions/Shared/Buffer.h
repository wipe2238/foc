#ifndef __BUFFER__
#define __BUFFER__

#include <string>

#include <Types.h>

class ScriptArray;
class ScriptString;

enum BufferFlags : uint8 // flags starting from 0x100 can be freely used by derived class
{
    // if set, extra info is displayed in log
    // enables sanity checks after redirections to derived class functions
    // enabled/disabled by SetDebug()
    BUFFER_FLAG_DEBUG      = 0x01,

    // if set, data buffer cannot be changed, and warning is displayed in log
    // enabled/disabled by SetReadOnly()
    BUFFER_FLAG_READ_ONLY  = 0x02,

    BUFFER_FLAG_RESERVED3  = 0x04, // reserved for future use
    BUFFER_FLAG_RESERVED4  = 0x08, // reserved for future use
    BUFFER_FLAG_RESERVED5  = 0x10, // reserved for future use
    BUFFER_FLAG_RESERVED6  = 0x20, // reserved for future use
    BUFFER_FLAG_RESERVED7  = 0x40, // reserved for future use

    // set when using operator>> and data cannot be read; unset on next succesful read
    // NOTE flag is not set when attempting to copy data into null script object
    BUFFER_FLAG_ERROR_READ = 0x80,
};

// base class for all Buffer object
// contains logic only, actual reading/writing data redirected to derived class
class BufferBase
{
protected:
    uint32 Flags;

public:
    BufferBase();
    virtual ~BufferBase();

protected:
    virtual uint8 DataRead( uint32 index ) = 0;
    virtual void  DataReadPosition( uint32 index ) = 0;
    virtual void  DataWrite( uint32 index, uint8 byte ) = 0;
    virtual void  DataWritePosition( uint32 index ) = 0;
    virtual void  DataResize( uint32 oldSize, uint32 newSize ) = 0;
    virtual void  DataClear() = 0;

public:
    bool IsFlag( uint32 flag );
    void SetDebug( bool enable );
    void SetReadOnly( bool enable );

    // increases data buffer by [size] bytes
    bool Grow( uint32 size );

    // increases data buffer by [size] bytes
    bool GrowToFit( uint32 size );

    // resets data buffer to initial state
    bool Clear();

    // returns data buffer size in bytes
    virtual uint32 GetSize() = 0;

    virtual uint32 GetReadPosition() = 0;
    virtual uint32 GetWritePosition() = 0;

    // basic data types

    BufferBase& operator<<( int8 value );
    BufferBase& operator>>( int8& value );

    BufferBase& operator<<( uint8 value );
    BufferBase& operator>>( uint8& value );

    BufferBase& operator<<( int16 value );
    BufferBase& operator>>( int16& value );

    BufferBase& operator<<( uint16 value );
    BufferBase& operator>>( uint16& value );

    BufferBase& operator<<( int32 value );
    BufferBase& operator>>( int32& value );

    BufferBase& operator<<( uint32 value );
    BufferBase& operator>>( uint32& value );

    // additional data types

    BufferBase& operator<<( bool value );
    BufferBase& operator>>( bool& value );

    BufferBase& operator<<( std::string value );
    BufferBase& operator>>( std::string& value );

    // TODO std::list<T>

    // TODO std::set<T>

    // TODO std::vector<T>

    BufferBase& operator<<( ScriptArray* arr );
    BufferBase& operator>>( ScriptArray* arr );
    BufferBase& Load( ScriptArray* arr, bool clear = true );
    BufferBase& Save( ScriptArray*& arr, std::string type = std::string(), bool all = true );

    BufferBase& operator<<( ScriptString* str );
    BufferBase& operator>>( ScriptString* str );

    template<typename T>
    T Read()
    {
        T tmp;
        *this >> tmp;

        return tmp;
    }
};

// default implementation of BufferBase
// contains work only, all logic is done by parent class
class Buffer : public BufferBase
{
protected:
    uint8* BufferData;
    uint32 BufferDataSize;

    uint32 BufferDataRead;
    uint32 BufferDataWrite;

public:
    Buffer();
    virtual ~Buffer();

protected:
    virtual uint8 DataRead( uint32 index ) override;
    virtual void  DataReadPosition( uint32 index ) override;
    virtual void  DataWrite( uint32 index, uint8 byte ) override;
    virtual void  DataWritePosition( uint32 index ) override;
    virtual void  DataResize( uint32 oldSize, uint32 newSize ) override;
    virtual void  DataClear() override;

public:
    virtual uint32 GetSize() override;
    virtual uint32 GetReadPosition() override;
    virtual uint32 GetWritePosition() override;
};

#endif // __BUFFER__ //
