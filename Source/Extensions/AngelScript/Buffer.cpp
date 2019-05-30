#include <extension.h>

#include <Log.h>

#include "Buffer.h"

#define BUFFER_LOG( ... )                        \
    {                                            \
        if( Debug )                              \
            WriteLog( "[Buffer]" __VA_ARGS__ );  \
    }

#define BUFFER_DELETE_DATA( data ) \
    {                              \
        if( data )                 \
        {                          \
            delete[](data);        \
            data = NULL;           \
        }                          \
    }

// STORE
// operator <<
#define BUFFER_STORE_BEGIN( b_type, b_size, b_val )       \
    {                                                     \
        BUFFER_LOG( "STORE %s = %d\n", # b_type, b_val ); \
                                                          \
        if( BufWrite + b_size >= BufLen )                 \
            Grow( b_size );                               \
    }

#define BUFFER_STORE1( type, val )          \
    {                                       \
        BUFFER_STORE_BEGIN( type, 1, val ); \
        BufData[BufWrite++] = (val) & 0xFF; \
    }

#define BUFFER_STORE2( type, val )               \
    {                                            \
        BUFFER_STORE_BEGIN( type, 2, val );      \
        BufData[BufWrite++] = (val >> 8) & 0xFF; \
        BufData[BufWrite++] = (val) & 0xFF;      \
    }

#define BUFFER_STORE4( type, val )                \
    {                                             \
        BUFFER_STORE_BEGIN( type, 4, val );       \
        BufData[BufWrite++] = (val >> 24) & 0xFF; \
        BufData[BufWrite++] = (val >> 16) & 0xFF; \
        BufData[BufWrite++] = (val >> 8) & 0xFF;  \
        BufData[BufWrite++] = (val) & 0xFF;       \
    }

// FETCH
// operator >>
#define BUFFER_FETCH_BEGIN( b_type, b_size )                                                            \
    {                                                                                                   \
        BUFFER_LOG( "FETCH %s (%db)\n", # b_type, b_size );                                             \
                                                                                                        \
        if( BufRead + b_size > BufWrite )                                                               \
        {                                                                                               \
            BUFFER_LOG( "FETCH out of bounds : Read(%u) + %d > End(%u)\n", BufRead, b_size, BufWrite ); \
            return BUFFER_THIS;                                                                         \
        }                                                                                               \
    }

#define BUFFER_FETCH_END( e_type, e_val )                 \
    {                                                     \
        BUFFER_LOG( "FETCH %s = %d\n", # e_type, e_val ); \
    }

#define BUFFER_FETCH1( type, val )     \
    {                                  \
        BUFFER_FETCH_BEGIN( type, 1 ); \
        val |= BufData[BufRead++];     \
        BUFFER_FETCH_END( type, val ); \
    }

#define BUFFER_FETCH2( type, val )      \
    {                                   \
        BUFFER_FETCH_BEGIN( type, 2 );  \
        val |= BufData[BufRead++] << 8; \
        val |= BufData[BufRead++];      \
        BUFFER_FETCH_END( type, val );  \
    }

#define BUFFER_FETCH4( type, val )       \
    {                                    \
        BUFFER_FETCH_BEGIN( type, 4 );   \
        val |= BufData[BufRead++] << 24; \
        val |= BufData[BufRead++] << 16; \
        val |= BufData[BufRead++] << 8;  \
        val |= BufData[BufRead++];       \
        BUFFER_FETCH_END( type, val );   \
    }

#define BUFFER_THIS       (*this)
#define BUFFER_LENTYPE    uint32

using namespace std;

Buffer::Buffer() : BufData( NULL ), BufLen( 0 ), BufRead( 0 ), BufWrite( 0 ), RefCount( 1 )
{}

Buffer::~Buffer()
{
    BUFFER_DELETE_DATA( BufData );
}

void Buffer::Grow( uint32 len )
{
    BUFFER_LOG( "GROW %u\n", len );

    if( BufWrite + len < BufLen )
    {
        BUFFER_LOG( "GROW complete : not needed\n" );

        return;
    }

    uint32 oldLen = BufLen;
    while( BufWrite + len > BufLen )
        ++BufLen;
    BUFFER_LOG( "GROW %u : %u -> %u\n", len, oldLen, BufLen );

    BUFFER_LOG( "GROW work\n" );
    uint8* newBuf = new uint8[BufLen];
    if( BufData )
        memcpy( newBuf, BufData, BufWrite );
    BUFFER_DELETE_DATA( BufData );
    BufData = newBuf;
    BUFFER_LOG( "GROW complete\n" );
}

Buffer& Buffer::operator<<( bool value )
{
    BUFFER_LOG( "STORE bool = %s\n", value ? "true" : "false" );

    uint8 tmp = (value ? 1 : 0);
    BUFFER_THIS << tmp;

    return BUFFER_THIS;
}

Buffer& Buffer::operator>>( bool& value )
{
    BUFFER_LOG( "FETCH bool" );

    value = false;
    uint8 tmp = 0;
    BUFFER_THIS >> tmp;
    value = (tmp ? true : false);

    BUFFER_LOG( "FETCH bool = %s\n", value ? "true" : "false" );

    return BUFFER_THIS;
}

Buffer& Buffer::operator<<( int8 value )
{
    BUFFER_STORE1( int8, value );

    return BUFFER_THIS;
}

Buffer& Buffer::operator>>( int8& value )
{
    value = 0;
    BUFFER_FETCH1( int8, value );

    return BUFFER_THIS;
}

Buffer& Buffer::operator<<( int16 value )
{
    BUFFER_STORE2( int16, value );

    return BUFFER_THIS;
}

Buffer& Buffer::operator>>( int16& value )
{
    value = 0;

    BUFFER_FETCH2( int16, value );

    return BUFFER_THIS;
}

Buffer& Buffer::operator<<( int32 value )
{
    BUFFER_STORE4( int32, value );

    return BUFFER_THIS;
}

Buffer& Buffer::operator>>( int32& value )
{
    value = 0;
    BUFFER_FETCH4( int32, value );

    return BUFFER_THIS;
}

Buffer& Buffer::operator<<( uint8 value )
{
    BUFFER_STORE1( uint8, value );

    return BUFFER_THIS;
}

Buffer& Buffer::operator>>( uint8& value )
{
    value = 0;
    BUFFER_FETCH1( uint8, value );

    return BUFFER_THIS;
}

Buffer& Buffer::operator<<( uint16 value )
{
    BUFFER_STORE2( uint16, value );

    return BUFFER_THIS;
}

Buffer& Buffer::operator>>( uint16& value )
{
    value = 0;
    BUFFER_FETCH2( uint16, value );

    return BUFFER_THIS;
}

Buffer& Buffer::operator<<( uint32 value )
{
    BUFFER_STORE4( uint32, value );

    return BUFFER_THIS;
}

Buffer& Buffer::operator>>( uint32& value )
{
    value = 0;
    BUFFER_FETCH4( uint32, value );

    return BUFFER_THIS;
}

/*
   Buffer& Buffer::Store( void* ref, int refTypeId )
   {
        BUFFER_LOG("STORE %d\n", refTypeId);
    // Hold on to the object type reference so it isn't destroyed too early
    if( *(void**)ref && (refTypeId & asTYPEID_MASK_OBJECT) )
    {
        asIObjectType* ot = Engine->GetObjectTypeById( refTypeId );
        if( ot )
            ot->AddRef();
    }

    // FreeObject();

    if( refTypeId & asTYPEID_OBJHANDLE )
    {
        // We're receiving a reference to the handle, so we need to dereference it
        // value.valueObj = *(void**)ref;
        // engine->AddRefScriptObject(value.valueObj, engine->GetObjectTypeById(refTypeId));
    }
    else if( refTypeId & asTYPEID_MASK_OBJECT )
    {
        // Create a copy of the object
        // value.valueObj = engine->CreateScriptObjectCopy(ref, engine->GetObjectTypeById(refTypeId));
    }
    else
    {
        int32 refTypeSize = Engine->GetSizeOfPrimitiveType( refTypeId );
        switch( refTypeSize )
        {
            case 1:
            {
                int8 val;
                memcpy( &val, ref, refTypeSize);
                if( refTypeId == asTYPEID_BOOL )
                    BUFFER_THIS << (bool)(val ? true : false);
                else if( refTypeId == asTYPEID_INT8 )
                    BUFFER_THIS << (int8)val;
                else if( refTypeId == asTYPEID_UINT8 )
                    BUFFER_THIS << (uint8)val;

                break;
            }
            case 2:
            {
                int16 val;
                memcpy( &val, ref, refTypeSize);
                if( refTypeId == asTYPEID_INT16 )
                    BUFFER_THIS << (int16)val;
                else if( refTypeId == asTYPEID_UINT16 )
                    BUFFER_THIS << (uint16)val;
                                break;
            }
                        case 4:
                        {
                                int32 val;
                                memcpy(&val, ref, refTypeSize);
                                if (refTypeId == asTYPEID_INT32)
                                        BUFFER_THIS << (int32)val;
                                else if (refTypeId == asTYPEID_UINT32)
                                        BUFFER_THIS << (uint32)val;
                                break;
                        }
                }

        // Primitives can be copied directly
        // value.valueInt = 0;

        // Copy the primitive value
        // We receive a pointer to the value.
        // int size = engine->GetSizeOfPrimitiveType(value.typeId);
        // memcpy(&value.valueInt, ref, size);
    }

    return BUFFER_THIS;
   }

   Buffer& Buffer::Fetch( void* ref, int refTypeId )
   {
    return BUFFER_THIS;
   }
 */

Buffer& Buffer::operator<<( SCRIPTARRAY_STORE arr )
{
    if( !arr )
    {
        BUFFER_LOG( "STORE array is null\n" );

        return BUFFER_THIS;
    }

    BUFFER_LOG( "elementTypeId\n" );
    int              arrTypeId = arr->GetElementTypeId();
    BUFFER_LOG( "arrSize\n" );
    uint32           arrSize = arr->GetSize();

    asIScriptEngine* engine = arr->GetArrayObjectType()->GetEngine();
    BUFFER_LOG( "STORE array<%s>::length = %u\n",
                engine->GetTypeDeclaration( arrTypeId, true ),
                arrSize
                );

    BUFFER_THIS << (BUFFER_LENTYPE)(arrSize);
    if( !arrSize )
        return BUFFER_THIS;

    // ???

    return BUFFER_THIS;
}

Buffer& Buffer::operator>>( SCRIPTARRAY_FETCH arr )
{
    int32            elementTypeId = arr.GetElementTypeId();
    int32            elementTypeSize = arr.GetElementSize();
    asIScriptEngine* engine = arr.GetArrayObjectType()->GetEngine();

    BUFFER_LOG( "FETCH array<%s> sizeof(%s) = %d\n",
                engine->GetTypeDeclaration( elementTypeId, true ),
                engine->GetTypeDeclaration( elementTypeId, true ),
                elementTypeSize
                );

    BUFFER_LENTYPE arrayLen = 0;
    BUFFER_THIS >> arrayLen;

    if( !arrayLen )
        return BUFFER_THIS;

    #define FETCH_PRIMITIVE( type ) \
        {                           \
            type tmp = 0;           \
            BUFFER_THIS >> tmp;     \
            arr.InsertLast( &tmp ); \
        }

    for( BUFFER_LENTYPE a = 0; a < arrayLen; a++ )
    {
        switch( elementTypeId )
        {
            case asTYPEID_BOOL:
                FETCH_PRIMITIVE( bool );
                break;
            case asTYPEID_INT8:
            case asTYPEID_UINT8:
                FETCH_PRIMITIVE( int8 );
                break;
            case asTYPEID_INT16:
            case asTYPEID_UINT16:
                FETCH_PRIMITIVE( int16 );
                break;
            case asTYPEID_INT32:
            case asTYPEID_UINT32:
                FETCH_PRIMITIVE( int32 );
                break;
            default:
                // TODO throw exception
                WriteLog( "[Buffer] ERROR : unknown typeId<%d:%s>\n", elementTypeId, engine->GetTypeDeclaration( elementTypeId, true ) );
                break;
        }
    }

    return BUFFER_THIS;
}

//
// AngelScript
//

void Buffer::AddRef() const
{
    ++RefCount;
}

void Buffer::Release() const
{
    if( --RefCount == 0 )
        delete this;
}

static void BufferFactory( asIScriptGeneric* generic )
{
    *(Buffer**)generic->GetAddressOfReturnLocation() = new Buffer();
}


// ==      opEquals
// !=      opEquals
// <       opCmp
// <=      opCmp
// >       opCmp
// >=      opCmp
// is      opEquals
// !is     opEquals
//
// =       opAssign
// +=      opAddAssign
// -=      opSubAssign
// *=      opMulAssign
// /=      opDivAssign
// %=      opModAssign
// * *=    opPowAssign
// &=      opAndAssign
// |=      opOrAssign
// ^=      opXorAssign
// <<=     opShlAssign
// >>=     opShrAssign
// >>>=    opUShrAssign
//
// +       opAdd   opAdd_r
// -       opSub   opSub_r
// *       opMul   opMul_r
// /       opDiv   opDiv_r
// %       opMod   opMod_r
// **      opPow   opPow_r
// &       opAnd   opAnd_r
// |       opOr    opOr_r
// ^       opXor   opXor_r
// <<      opShl   opShl_r
// >>      opShr   opShr_r
// >>>     opUShr  opUShr_r
//
// []      opIndex
//
// ()      opCall
//
// type(expr)	     constructor, opConv, opImplConv
// cast<type>(expr)  opCast, opImplCast

void RegisterBufferNative( asIScriptEngine* engine )
{
    engine->RegisterObjectType( "Buffer", 0, asOBJ_REF );

    engine->RegisterObjectBehaviour( "Buffer", asBEHAVE_FACTORY, "Buffer@ f()", asFUNCTION( BufferFactory ), asCALL_GENERIC );
    engine->RegisterObjectBehaviour( "Buffer", asBEHAVE_ADDREF,  "void f()", asMETHOD( Buffer, AddRef ), asCALL_THISCALL );
    engine->RegisterObjectBehaviour( "Buffer", asBEHAVE_RELEASE, "void f()", asMETHOD( Buffer, Release ), asCALL_THISCALL );

    engine->RegisterObjectProperty( "Buffer", "bool Debug", asOFFSET( Buffer, Debug ) );

    engine->RegisterObjectMethod( "Buffer", "Buffer& opShl(int8 value)", asMETHODPR( Buffer, operator<<, (int8), Buffer& ), asCALL_THISCALL );
    engine->RegisterObjectMethod( "Buffer", "Buffer& opShr(int8& value)", asMETHODPR( Buffer, operator>>, (int8 &), Buffer& ), asCALL_THISCALL );

    engine->RegisterObjectMethod( "Buffer", "Buffer& opShl(int16 value)", asMETHODPR( Buffer, operator<<, (int16), Buffer& ), asCALL_THISCALL );
    engine->RegisterObjectMethod( "Buffer", "Buffer& opShr(int16& value)", asMETHODPR( Buffer, operator>>, (int16 &), Buffer& ), asCALL_THISCALL );

    engine->RegisterObjectMethod( "Buffer", "Buffer& opShl(int32 value)", asMETHODPR( Buffer, operator<<, (int32), Buffer& ), asCALL_THISCALL );
    engine->RegisterObjectMethod( "Buffer", "Buffer& opShr(int32& value)", asMETHODPR( Buffer, operator>>, (int32 &), Buffer& ), asCALL_THISCALL );

    engine->RegisterObjectMethod( "Buffer", "Buffer& opShl(uint8 value)", asMETHODPR( Buffer, operator<<, (uint8), Buffer& ), asCALL_THISCALL );
    engine->RegisterObjectMethod( "Buffer", "Buffer& opShr(uint8& value)", asMETHODPR( Buffer, operator>>, (uint8 &), Buffer& ), asCALL_THISCALL );

    engine->RegisterObjectMethod( "Buffer", "Buffer& opShl(uint16 value)", asMETHODPR( Buffer, operator<<, (uint16), Buffer& ), asCALL_THISCALL );
    engine->RegisterObjectMethod( "Buffer", "Buffer& opShr(uint16& value)", asMETHODPR( Buffer, operator>>, (uint16 &), Buffer& ), asCALL_THISCALL );

    engine->RegisterObjectMethod( "Buffer", "Buffer& opShl(uint32 value)", asMETHODPR( Buffer, operator<<, (uint32), Buffer& ), asCALL_THISCALL );
    engine->RegisterObjectMethod( "Buffer", "Buffer& opShr(uint32& value)", asMETHODPR( Buffer, operator>>, (uint32 &), Buffer& ), asCALL_THISCALL );

    /*
       engine->RegisterObjectMethod("Buffer", "Buffer& opShl(?&in value)", asMETHODPR(Buffer, Store, (void*, int), Buffer&), asCALL_THISCALL);
       engine->RegisterObjectMethod("Buffer", "Buffer& opShr(?&in value)", asMETHODPR(Buffer, Fetch, (void*, int), Buffer&), asCALL_THISCALL);
     */
}

void RegisterBuffer( asIScriptEngine* engine )
{
    if( !engine->GetObjectTypeByName( "Buffer" ) )
        RegisterBufferNative( engine );
}
