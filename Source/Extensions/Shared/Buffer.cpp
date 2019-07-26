#include <set>

#include <Addons/scriptarray.h>
#include <Addons/scriptstring.h>
#include <App.h>
#include <Defines.Public.h>
#include <Script.h>

#include "Shared/Buffer.h"

///
/// BufferBase
///

BufferBase::BufferBase() :
// protected
    Flags( 0 )
{}

BufferBase::~BufferBase()
{
    if( IsFlag( BUFFER_FLAG_DEBUG ) )
        App.WriteLogF( _FUNC_, "\n" );
}

//

bool BufferBase::IsFlag( uint32 flag )
{
    return FLAG( Flags, flag );
}

void BufferBase::SetDebug( bool enable )
{
    if( enable )
        SETFLAG( Flags, BUFFER_FLAG_DEBUG );
    else
        UNSETFLAG( Flags, BUFFER_FLAG_DEBUG );
}

void BufferBase::SetReadOnly( bool enable )
{
    if( enable )
        SETFLAG( Flags, BUFFER_FLAG_READ_ONLY );
    else
        UNSETFLAG( Flags, BUFFER_FLAG_READ_ONLY );
}

//

bool BufferBase::Grow( uint32 size )
{
    if( !size )
        return true;
    else if( IsFlag( BUFFER_FLAG_READ_ONLY ) )
    {
        App.WriteLogF( _FUNC_, "(%u) not allowed : ReadOnly\n", size );

        return false;
    }

    uint oldSize = GetSize();
    uint newSize = oldSize + size;

    // detect uint overflow abuse
    if( newSize == oldSize )
    {
        App.WriteLogF( _FUNC_, "(%u) ERROR : size not changed : oldSize<%u> newSize<%u>\n", size, oldSize, newSize );

        return true;
    }
    else if( newSize < oldSize )
    {
        App.WriteLogF( _FUNC_, "(%u) ERROR : negative growth : oldSize<%u> newSize<%u>\n", oldSize, newSize );

        return false;
    }

    if( IsFlag( BUFFER_FLAG_DEBUG ) )
        App.WriteLogF( _FUNC_, "(%u) -> DataResize(%u,%u)\n", size, oldSize, newSize );

    DataResize( oldSize, newSize );

    return true;
}

bool BufferBase::GrowToFit( uint32 size )
{
    if( !size )
        return true;

    uint32 write = GetWritePosition(), dataSize = GetSize();

    if( write + size > dataSize )
    {
        if( IsFlag( BUFFER_FLAG_READ_ONLY ) )
        {
            App.WriteLogF( _FUNC_, " not allowed : ReadOnly\n" );

            return false;
        }

        uint grow = write + size - dataSize;

        if( IsFlag( BUFFER_FLAG_DEBUG ) )
            App.WriteLogF( _FUNC_, "(%u) : Write<%u> + size<%u> - Size<%u> -> Grow(%u)\n", size, write, size, dataSize, grow );

        return Grow( grow );
    }

    return true;
}

bool BufferBase::Clear()
{
    if( IsFlag( BUFFER_FLAG_READ_ONLY ) )
    {
        App.WriteLogF( _FUNC_, " not allowed : ReadOnly\n" );

        return false;
    }
    else if( IsFlag( BUFFER_FLAG_DEBUG ) )
        App.WriteLogF( _FUNC_, "\n" );

    DataClear();
    DataReadPosition( 0 );
    DataWritePosition( 0 );

    return true;
}

// int8

BufferBase& BufferBase::operator<<( int8 value )
{
    STATIC_ASSERT( sizeof(int8) == 1 );
    uint32 write = GetWritePosition();

    GrowToFit( 1 );
    DataWrite( write++, (value) & 0xFF );
    DataWritePosition( write );

    return *this;
}

BufferBase& BufferBase::operator>>( int8& value )
{
    STATIC_ASSERT( sizeof(int8) == 1 );
    uint32 read = GetReadPosition();
    value = 0;

    if( read + 1 > GetSize() )
    {
        SETFLAG( Flags, BUFFER_FLAG_ERROR_READ );
        return *this;
    }

    UNSETFLAG( Flags, BUFFER_FLAG_ERROR_READ );
    value |= DataRead( read++ );
    DataReadPosition( read );

    return *this;
}

// uint8

BufferBase& BufferBase::operator<<( uint8 value )
{
    STATIC_ASSERT( sizeof(uint8) == 1 );
    uint32 write = GetWritePosition();

    GrowToFit( 1 );
    DataWrite( write++, (value) & 0xFF );
    DataWritePosition( write );

    return *this;
}

BufferBase& BufferBase::operator>>( uint8& value )
{
    STATIC_ASSERT( sizeof(uint8) == 1 );
    uint32 read = GetReadPosition();
    value = 0;

    if( read + 1 > GetSize() )
    {
        SETFLAG( Flags, BUFFER_FLAG_ERROR_READ );
        return *this;
    }

    UNSETFLAG( Flags, BUFFER_FLAG_ERROR_READ );
    value |= DataRead( read++ );
    DataReadPosition( read );

    return *this;
}

// int16

BufferBase& BufferBase::operator<<( int16 value )
{
    STATIC_ASSERT( sizeof(int16) == 2 );
    uint32 write = GetWritePosition();

    GrowToFit( 2 );
    DataWrite( write++, (value >> 8) & 0xFF );
    DataWrite( write++, (value) & 0xFF );
    DataWritePosition( write );

    return *this;
}

BufferBase& BufferBase::operator>>( int16& value )
{
    STATIC_ASSERT( sizeof(int16) == 2 );
    uint32 read = GetReadPosition();
    value = 0;

    if( read + 2 > GetSize() )
    {
        SETFLAG( Flags, BUFFER_FLAG_ERROR_READ );
        return *this;
    }

    UNSETFLAG( Flags, BUFFER_FLAG_ERROR_READ );
    value |= DataRead( read++ ) << 8;
    value |= DataRead( read++ );
    DataReadPosition( read );

    return *this;
}

// uint16

BufferBase& BufferBase::operator<<( uint16 value )
{
    STATIC_ASSERT( sizeof(uint16) == 2 );
    uint32 write = GetWritePosition();

    GrowToFit( 2 );
    DataWrite( write++, (value >> 8) & 0xFF );
    DataWrite( write++, (value) & 0xFF );
    DataWritePosition( write );

    return *this;
}

BufferBase& BufferBase::operator>>( uint16& value )
{
    STATIC_ASSERT( sizeof(uint16) == 2 );
    uint32 read = GetReadPosition();
    value = 0;

    if( read + 2 > GetSize() )
    {
        SETFLAG( Flags, BUFFER_FLAG_ERROR_READ );
        return *this;
    }

    UNSETFLAG( Flags, BUFFER_FLAG_ERROR_READ );
    value |= DataRead( read++ ) << 8;
    value |= DataRead( read++ );
    DataReadPosition( read );

    return *this;
}

// int32

BufferBase& BufferBase::operator<<( int32 value )
{
    STATIC_ASSERT( sizeof(int32) == 4 );
    uint32 write = GetWritePosition();

    GrowToFit( 4 );
    DataWrite( write++, (value >> 24) & 0xFF );
    DataWrite( write++, (value >> 16) & 0xFF );
    DataWrite( write++, (value >> 8) & 0xFF );
    DataWrite( write++, (value) & 0xFF );
    DataWritePosition( write );

    return *this;
}

BufferBase& BufferBase::operator>>( int32& value )
{
    STATIC_ASSERT( sizeof(int32) == 4 );
    uint32 read = GetReadPosition();
    value = 0;

    if( read + 4 > GetSize() )
    {
        SETFLAG( Flags, BUFFER_FLAG_ERROR_READ );
        return *this;
    }

    UNSETFLAG( Flags, BUFFER_FLAG_ERROR_READ );
    value |= DataRead( read++ ) << 24;
    value |= DataRead( read++ ) << 16;
    value |= DataRead( read++ ) << 8;
    value |= DataRead( read++ );
    DataReadPosition( read );

    return *this;
}

// uint32

BufferBase& BufferBase::operator<<( uint32 value )
{
    STATIC_ASSERT( sizeof(uint32) == 4 );
    uint32 write = GetWritePosition();

    GrowToFit( 4 );
    DataWrite( write++, (value >> 24) & 0xFF );
    DataWrite( write++, (value >> 16) & 0xFF );
    DataWrite( write++, (value >> 8) & 0xFF );
    DataWrite( write++, (value) & 0xFF );
    DataWritePosition( write );

    return *this;
}

BufferBase& BufferBase::operator>>( uint32& value )
{
    STATIC_ASSERT( sizeof(uint32) == 4 );
    uint32 read = GetReadPosition();
    value = 0;

    if( read + 4 > GetSize() )
    {
        SETFLAG( Flags, BUFFER_FLAG_ERROR_READ );
        return *this;
    }

    UNSETFLAG( Flags, BUFFER_FLAG_ERROR_READ );
    value |= DataRead( read++ ) << 24;
    value |= DataRead( read++ ) << 16;
    value |= DataRead( read++ ) << 8;
    value |= DataRead( read++ );
    DataReadPosition( read );

    return *this;
}

// bool

BufferBase& BufferBase::operator<<( bool value )
{
    uint8 tmp = (value ? 1 : 0);
    *this << tmp;

    return *this;
}

BufferBase& BufferBase::operator>>( bool& value )
{
    value = false;

    uint8 tmp = 0;
    *this >> tmp;
    value = (tmp ? true : false);

    return *this;
}

// std::string

BufferBase& BufferBase::operator<<( std::string value )
{
    uint32 length = (uint32)value.length();

    *this << length;

    if( !length )
        return *this;

    GrowToFit( length );
    for( uint32 idx = 0; idx < length; idx++ )
    {
        *this << (uint8)value.at( idx );
    }

    return *this;
}

BufferBase& BufferBase::operator>>( std::string& value )
{
    value.clear();

    uint32 length = 0;
    *this >> length;

    if( !length )
        return *this;

    for( uint32 idx = 0; idx < length; idx++ )
    {
        value.push_back( (char)Read<uint8>() );
    }

    return *this;
}

// ScriptArray*

BufferBase& BufferBase::operator<<( ScriptArray* arr )
{
    if( !arr )
    {
        App.WriteLogF( _FUNC_, " ERROR : ScriptArray is null\n" );
        return *this;
    }

    uint32      arrSize = arr->GetSize();
    int         arrETypeId = arr->GetElementTypeId();
    int         arrETypeSize = arr->GetElementSize();
    const char* arrETypeDecl = arr->GetArrayObjectType()->GetEngine()->GetTypeDeclaration( arrETypeId, true );

    if( IsFlag( BUFFER_FLAG_DEBUG ) )
        App.WriteLogF( _FUNC_, " : ScriptArray<%s> ElementTypeId<%u> ElementSize<%d> Size<%u>\n", arrETypeDecl, arrETypeId, arrETypeSize, arrSize );

    // always write array size
    *this << arrSize;
    if( !arrSize )
        return *this;

    GrowToFit( arrETypeSize * arrSize );
    uint8* at0 = (uint8*)arr->At( 0 );
    for( uint idx = 0; idx < arrSize * arrETypeSize; idx++ )
    {
        *this << *(at0 + idx);
    }

    #if 0 // old method
    for( uint idx = 0; idx < arrSize; idx++ )
    {
        switch( arrETypeId )
        {
            case asTYPEID_BOOL:
                *this << *(bool*)arr->At( idx );
                break;
            case asTYPEID_INT8:
                *this << *(int8*)arr->At( idx );
                break;
            case asTYPEID_UINT8:
                *this << *(uint8*)arr->At( idx );
                break;
            case asTYPEID_INT16:
                *this << *(int16*)arr->At( idx );
                break;
            case asTYPEID_UINT16:
                *this << *(uint16*)arr->At( idx );
                break;
            case asTYPEID_INT32:
                *this << *(int32*)arr->At( idx );
                break;
            case asTYPEID_UINT32:
                *this << *(uint32*)arr->At( idx );
                break;
        }
    }
    #endif

    return *this;
}

BufferBase& BufferBase::operator>>( ScriptArray* arr )
{
    if( !arr )
    {
        App.WriteLogF( _FUNC_, " ERROR : ScriptArray is null\n" );

        return *this;
    }

    int         arrETypeId = arr->GetElementTypeId();
    int         arrETypeSize = arr->GetElementSize();
    const char* arrETypeDecl = arr->GetArrayObjectType()->GetEngine()->GetTypeDeclaration( arrETypeId, true );

    if( IsFlag( BUFFER_FLAG_DEBUG ) )
        App.WriteLogF( _FUNC_, " : ScriptArray<%s> ElementTypeId<%u> ElementSize<%d>\n", arrETypeDecl, arrETypeId, arrETypeSize );

    arr->Resize( 0 );

    // always read array size
    uint32 arrSize = Read<uint32>();
    if( !arrSize )
        return *this;

    arr->Resize( arrSize );

    uint8* at0 = (uint8*)arr->At( 0 );
    memzero( at0, arrSize * arrETypeSize );

    for( uint32 idx = 0; idx < arrSize * arrETypeSize; idx++ )
    {
        *(at0 + idx) = Read<uint8>();
    }

    return *this;
}

BufferBase& BufferBase::Load( ScriptArray* arr, bool clear /* = true */ )
{
    if( !arr )
    {
        App.WriteLogF( _FUNC_, " ERROR : ScriptArray is null\n" );
        return *this;
    }

    if( clear )
        Clear();

    uint32 arrSize = arr->GetSize();
    if( !arrSize )
        return *this; // nothing to copy

    int         arrETypeId = arr->GetElementTypeId();
    int         arrETypeSize = arr->GetElementSize();
    uint        arrDataSize = arrSize * arrETypeSize;
    const char* arrETypeDecl = arr->GetArrayObjectType()->GetEngine()->GetTypeDeclaration( arrETypeId, true );

    if( IsFlag( BUFFER_FLAG_DEBUG ) )
        App.WriteLogF( _FUNC_, " : ScriptArray<%s> ElementTypeId<%u> ElementSize<%d> Size<%u> -> DataSize<%u>\n", arrETypeDecl, arrETypeId, arrETypeSize, arrSize, arrDataSize );

    GrowToFit( arrDataSize );

    uint8* at0 = (uint8*)arr->At( 0 );
    for( uint idx = 0; idx < arrDataSize; idx++ )
    {
        *this << *(at0 + idx);
    }

    return *this;
}

BufferBase& BufferBase::Save( ScriptArray*& arr, std::string type /* = std::string() */, bool all /* = true */ )
{
    if( !arr )
    {
        if( IsFlag( BUFFER_FLAG_DEBUG ) )
            App.WriteLogF( _FUNC_, " : creating ScriptArray<%s>\n", type.c_str() );

        if( type.back() != ']' && type.back() != '>' )
        {
            App.WriteLogF( _FUNC_, " ERROR : invalid type<%s>\n", type.c_str() );

            return *this;
        }

        arr = Script::CreateArray( type.c_str() );
    }
    else if( IsFlag( BUFFER_FLAG_DEBUG ) )
        App.WriteLogF( _FUNC_, " : using existing ScriptArray\n" );

    arr->Resize( 0 );

    uint dataSize = all ? GetSize() : GetSize() - GetReadPosition();
    if( !dataSize )
        return *this; // nothing to copy

    int         arrETypeId = arr->GetElementTypeId();
    int         arrETypeSize = arr->GetElementSize();
    const char* arrETypeDecl = arr->GetArrayObjectType()->GetEngine()->GetTypeDeclaration( arrETypeId, true );
    uint32      arrSize = (uint32)std::ceilf( float(dataSize) / float(arrETypeSize) );

    if( IsFlag( BUFFER_FLAG_DEBUG ) )
        App.WriteLogF( _FUNC_, " : ScriptArray<%s> ElementTypeId<%u> ElementSize<%d> DataSize<%u> -> Size<%u>\n", arrETypeDecl, arrETypeId, arrETypeSize, dataSize, arrSize );

    arr->Resize( arrSize );

    uint8* at0 = (uint8*)arr->At( 0 );
    memzero( at0, arrSize * arrETypeSize );

    // store and reset read position
    uint32 read = GetReadPosition();
    if( all )
        DataReadPosition( 0 );

    for( uint32 idx = 0; idx < dataSize; idx++ )
    {
        *(at0 + idx) = Read<uint8>();
    }

    // restore reading positition
    if( all )
        DataReadPosition( read );

    return *this;
}

// ScriptString*

BufferBase& BufferBase::operator<<( ScriptString* str )
{
    if( !str )
    {
        App.WriteLogF( _FUNC_, " ERROR : ScriptString is null\n" );

        return *this;
    }

    *this << str->c_std_str();

    return *this;
}

BufferBase& BufferBase::operator>>( ScriptString* str )
{
    if( !str )
    {
        App.WriteLogF( _FUNC_, " ERROR : ScriptString is null\n" );

        return *this;
    }

    str->assign( "" );

    std::string tmp;
    *this >> tmp;

    if( tmp.length() )
        str->assign( tmp.c_str(), tmp.length() );

    return *this;
}

///
/// Buffer
///

Buffer::Buffer() : BufferBase(),
// protected
    BufferData( nullptr ), BufferDataSize( 0 ),
    BufferDataRead( 0 ), BufferDataWrite( 0 )
{}

Buffer::~Buffer()
{
    if( IsFlag( BUFFER_FLAG_DEBUG ) )
        App.WriteLogF( _FUNC_, "\n" );

    DataClear();
}

uint8 Buffer::DataRead( uint32 index )
{
    return BufferData[index];
}

void Buffer::DataReadPosition( uint32 index )
{
    BufferDataRead = index;
}

void Buffer::DataWrite( uint32 index, uint8 byte )
{
    BufferData[index] = byte;
}

void Buffer::DataWritePosition( uint32 index )
{
    BufferDataWrite = index;
}

void Buffer::DataResize( uint32 oldSize, uint32 newSize )
{
    uint8* newData = new uint8[newSize];

    if( BufferData )
    {
        if( newSize > oldSize )
        {
            memcpy( newData, BufferData, oldSize );
            memzero( newData + oldSize, newSize - oldSize );
        }
        else if( newSize < oldSize )
        {
            memcpy( newData, BufferData, newSize );
        }

        delete[] BufferData;
    }
    else
        memzero( newData, newSize );

    BufferData = newData;
    BufferDataSize = newSize;
}

void Buffer::DataClear()
{
    if( BufferData )
    {
        delete[] BufferData;
        BufferData = nullptr;
    }

    BufferDataSize = 0;
}

uint32 Buffer::GetSize()
{
    return BufferDataSize;
}

uint32 Buffer::GetReadPosition()
{
    return BufferDataRead;
}

uint32 Buffer::GetWritePosition()
{
    return BufferDataWrite;
}
