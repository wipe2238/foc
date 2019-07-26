#ifndef __BUFFER__ARRAY__
#define __BUFFER__ARRAY__

#include <string>

#include "Shared/Buffer.h"

class ScriptArray;

// enum BufferArrayFlags : uint32
// {
// // 0x01 - 0x80 reserved
// };

class BufferArray : public BufferBase
{
protected:
    ScriptArray* Array;
    uint32       ArrayDataSize;

    uint32       ArrayRead;
    uint32       ArrayWrite;

public:
    static std::string DefaultType;

    BufferArray();
    BufferArray( std::string type );
    BufferArray( ScriptArray* arr );
    virtual ~BufferArray();

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

public:
    virtual ScriptArray* GetArray();
};

#endif // __BUFFER__ARRAY__ //
