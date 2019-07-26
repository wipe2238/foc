#include <App.h>
#include <Defines.Public.h>
#include <Script.h>

#include "Shared/Buffer.Array.h"

std::string BufferArray::DefaultType = "array<uint8>";

BufferArray::BufferArray() : BufferBase(),
// protected
    Array( nullptr ), ArrayDataSize( 0 ),
    ArrayRead( 0 ), ArrayWrite( 0 )
{
    if( DefaultType.back() == ']' || DefaultType.back() == '>' )
        Array = Script::CreateArray( DefaultType.c_str() );
}

BufferArray::BufferArray( std::string type ) : BufferBase(),
// protected
    Array( nullptr ), ArrayDataSize( 0 ),
    ArrayRead( 0 ), ArrayWrite( 0 )
{
    Array = Script::CreateArray( type.c_str() );
}

BufferArray::BufferArray( ScriptArray* arr ) : BufferBase(),
// protected
    Array( arr ), ArrayDataSize( arr->GetSize() * arr->GetElementSize() ),
    ArrayRead( 0 ), ArrayWrite( 0 )
{
    Array->AddRef();
}

BufferArray::~BufferArray()
{
    if( IsFlag( BUFFER_FLAG_DEBUG ) )
        App.WriteLogF( _FUNC_, "\n" );

    Array->Release();
}

uint8 BufferArray::DataRead( uint32 index )
{
    if( !Array )
        return 0;

    uint8* at0 = (uint8*)Array->At( 0 );
    return *(at0 + index);
}

void BufferArray::DataReadPosition( uint32 index )
{
    if( !Array )
        return;

    ArrayRead = index;
}

void BufferArray::DataWrite( uint32 index, uint8 byte )
{
    if( !Array )
        return;

    uint8* at0 = (uint8*)Array->At( 0 );
    *(at0 + index) = byte;
}

void BufferArray::DataWritePosition( uint32 index )
{
    if( !Array )
        return;

    ArrayWrite = index;
}

void BufferArray::DataResize( uint32 oldSize, uint32 newSize )
{
    if( !Array )
        return;

    int    elementSize = Array->GetElementSize();
    uint32 arraySize = (uint32)std::ceilf( float(newSize) / float(elementSize) );

    if( IsFlag( BUFFER_FLAG_DEBUG ) )
    {
        const char* elementDecl = Array->GetArrayObjectType()->GetEngine()->GetTypeDeclaration( Array->GetElementTypeId(), true );

        App.WriteLogF( _FUNC_, "(%u,%u) : ScriptArray<%s> elementSize<%u> -> arraySize<%u>\n",
                       oldSize, newSize, elementDecl, elementSize, arraySize );
    }

    Array->Resize( arraySize );

    if( newSize > oldSize )
    {
        uint8* at0 = (uint8*)Array->At( 0 );
        memzero( at0 + oldSize, newSize - oldSize );
    }

    ArrayDataSize = newSize;
}

void BufferArray::DataClear()
{
    if( !Array )
        return;

    Array->Resize( 0 );
    ArrayDataSize = 0;
}

uint32 BufferArray::GetSize()
{
    if( !Array )
        return 0;

    return ArrayDataSize;
}

uint32 BufferArray::GetReadPosition()
{
    if( !Array )
        return 0;

    return ArrayRead;
}

uint32 BufferArray::GetWritePosition()
{
    if( !Array )
        return 0;

    return ArrayWrite;
}

ScriptArray* BufferArray::GetArray()
{
    return Array;
}
