#ifndef __BUFFER__TEST__
#define __BUFFER__TEST__

#include <limits>
#include <set>

#include <App.h>
#include <Defines.Public.h>
#include <Random.h>
#include <Script.h>
#include <Timer.h>

template<typename B>
bool TestGrow()
{
    B      buff;
    uint32 size;

    // initial size must be 0
    size = buff.GetSize();
    if( size != 0 )
    {
        App.WriteLogF( _FUNC_, "<%s> FAIL : invalid default size : current<%u> expected<0>\n", typeid(B).name(), size );
        return false;
    }

    buff.Grow( 1 );
    size = buff.GetSize();
    if( size != 1 )
    {
        App.WriteLogF( _FUNC_, "<%s> FAIL : invalid size : current<%u> expected<1>\n", typeid(B).name(), size );
        return false;
    }

    buff.Grow( 2 );
    size = buff.GetSize();
    if( size != 3 )
    {
        App.WriteLogF( _FUNC_, "<%s> FAIL : invalid size : current<%u> expected<3>\n", typeid(B).name(), size );
        return false;
    }

    buff.GrowToFit( 4 );
    size = buff.GetSize();
    if( size != 4 )
    {
        App.WriteLogF( _FUNC_, "<%s> FAIL : invalid size : current<%u> expected<4>\n", typeid(B).name(), size );
        return false;
    }

    buff.Grow( 6 );
    size = buff.GetSize();
    if( size != 10 )
    {
        App.WriteLogF( _FUNC_, "<%s> FAIL : invalid size : current<%u> expected<10>\n", typeid(B).name(), size );
        return false;
    }

    uint32 grow = Random( 41, 90 ), oldSize = size;
    buff.Grow( grow );
    size = buff.GetSize();
    if( size != oldSize + grow )
    {
        App.WriteLogF( _FUNC_, "<%s> FAIL : invalid size : current<%u> expected<%u>\n", typeid(B).name(), size, oldSize + grow );
        return false;
    }

    buff.GrowToFit( 50 );
    size = buff.GetSize();
    if( size != oldSize + grow )
    {
        App.WriteLogF( _FUNC_, "<%s> FAIL : invalid size : current<%u> expected<100>\n", typeid(B).name(), size, oldSize + grow );
        return false;
    }

    /*
       buff << u8 << u8 << u16;
       size = buff.GetSize();
       if( size != 4 )
       {
       App.WriteLogF( _FUNC_, "<%s> FAIL : invalid autosize : current<%u> expected<4>\n", typeid(B).name(), size );
       return false;
       }
       buff.Clear();

       buff.GrowToFit( 6 );
       buff << u16 << u16 << u32 << u16;
       size = buff.GetSize();
       if( size != 10 )
       {
       App.WriteLogF( _FUNC_, "<%s> FAIL : invalid autosize : current<%u> expected<10>\n", typeid(B).name(), size );
       return false;
       }
       buff.Clear();
     */

    return true;
}

template<typename B, typename T>
bool TestReadPosition()
{
    B      buff;
    T      tmp = Random( std::numeric_limits<T>::min(), std::numeric_limits<T>::max() );
    uint32 read;

    // initial read position must be 0
    read = buff.GetReadPosition();
    if( read != 0 )
    {
        App.WriteLogF( _FUNC_, "<%s,%s> FAIL : invalid default read position : current<%u> expected<0>\n", typeid(B).name(), typeid(T).name(), read );
        return false;
    }

    buff << tmp >> tmp;

    read = buff.GetReadPosition();
    if( read != sizeof(T) )
    {
        App.WriteLogF( _FUNC_, "<%s,%s> FAIL : invalid read position : current<%u> expected<%u>\n", typeid(B).name(), typeid(T).name(), read, sizeof(T) );
        return false;
    }

    return true;
}

template<typename B, typename T>
bool TestWritePositionT()
{
    B      buff;
    T      tmp = Random( std::numeric_limits<T>::min(), std::numeric_limits<T>::max() );
    uint32 size, write;

    // initial write position must be 0
    write = buff.GetWritePosition();
    if( write != 0 )
    {
        App.WriteLogF( _FUNC_, "<%s,%s> FAIL : invalid default write position : current<%u> expected<0>\n", typeid(B).name(), typeid(T).name(), write );
        return false;
    }

    buff << tmp;

    size = buff.GetSize();
    if( size != sizeof(T) )
    {
        App.WriteLogF( _FUNC_, "<%s,%s> FAIL : invalid size : current<%u> expected<%u>\n", typeid(B).name(), typeid(T).name(), size, sizeof(T) );
        return false;
    }

    write = buff.GetWritePosition();
    if( write != sizeof(T) )
    {
        App.WriteLogF( _FUNC_, "<%s,%s> FAIL : invalid write position : current<%u> expected<%u>\n", typeid(B).name(), typeid(T).name(), write, sizeof(T) );
        return false;
    }

    return true;
}

template<typename B>
bool TestReadWritePosition()
{
    if( !TestWritePositionT<B, int8>() )
        return false;
    if( !TestWritePositionT<B, uint8>() )
        return false;
    if( !TestWritePositionT<B, int16>() )
        return false;
    if( !TestWritePositionT<B, uint16>() )
        return false;
    if( !TestWritePositionT<B, int32>() )
        return false;
    if( !TestWritePositionT<B, uint32>() )
        return false;

    if( !TestReadPosition<B, int8>() )
        return false;
    if( !TestReadPosition<B, uint8>() )
        return false;
    if( !TestReadPosition<B, int16>() )
        return false;
    if( !TestReadPosition<B, uint16>() )
        return false;
    if( !TestReadPosition<B, int32>() )
        return false;
    if( !TestReadPosition<B, uint32>() )
        return false;

    return true;
}

template<typename B>
bool TestFlagDefaults( uint32 flag, const char* name )
{
    B buff;

    // initial flags must be unset
    if( buff.IsFlag( flag ) )
    {
        App.WriteLogF( _FUNC_, "<%s> FAIL : %s set by default\n", typeid(B).name(), name );

        return false;
    }

    return true;
}

template<typename B>
bool TestFlagDebug_SetUnset()
{
    B buff;

    buff.SetDebug( true );
    if( !buff.IsFlag( BUFFER_FLAG_DEBUG ) )
    {
        App.WriteLogF( _FUNC_, "<%s> FAIL : not set correctly\n", typeid(B).name() );
        return false;
    }

    buff.SetDebug( false );
    if( buff.IsFlag( BUFFER_FLAG_DEBUG ) )
    {
        App.WriteLogF( _FUNC_, "<%s> FAIL : not cleared correctly\n", typeid(B).name() );
        return false;
    }

    return true;
}

template<typename B>
bool TestFlagReadOnly_SetUnset()
{
    B buff;

    buff.SetReadOnly( true );
    if( !buff.IsFlag( BUFFER_FLAG_READ_ONLY ) )
    {
        App.WriteLogF( _FUNC_, "<%s> FAIL : not set correctly\n", typeid(B).name() );
        return false;
    }

    buff.SetReadOnly( false );
    if( buff.IsFlag( BUFFER_FLAG_READ_ONLY ) )
    {
        App.WriteLogF( _FUNC_, "<%s> FAIL : not cleared correctly\n", typeid(B).name() );
        return false;
    }

    return true;
}

template<typename B, typename T>
bool TestFlagReadError_SetUnset()
{
    B buff;
    T tmp;

    buff >> tmp;
    if( !buff.IsFlag( BUFFER_FLAG_ERROR_READ ) )
    {
        App.WriteLogF( _FUNC_, "<%s,%s> FAIL : not set correctly\n", typeid(B).name(), typeid(T).name() );

        return false;
    }

    buff.Grow( sizeof(T) );
    buff >> tmp;
    if( buff.IsFlag( BUFFER_FLAG_ERROR_READ ) )
    {
        App.WriteLogF( _FUNC_, "<%s,%s> FAIL : not cleared correctly\n", typeid(B).name(), typeid(T).name() );

        return false;
    }

    return true;
}

template<typename B>
bool TestFlags()
{
    if( !TestFlagDefaults<B>( BUFFER_FLAG_DEBUG, "BUFFER_FLAG_DEBUG" ) )
        return false;
    if( !TestFlagDefaults<B>( BUFFER_FLAG_READ_ONLY, "BUFFER_FLAG_READ_ONLY" ) )
        return false;
    if( !TestFlagDefaults<B>( BUFFER_FLAG_ERROR_READ, "BUFFER_FLAG_ERROR_READ" ) )
        return false;

    if( !TestFlagDebug_SetUnset<B>() )
        return false;
    if( !TestFlagReadOnly_SetUnset<B>() )
        return false;
    if( !TestFlagReadError_SetUnset<B, int8>() )
        return false;
    if( !TestFlagReadError_SetUnset<B, uint8>() )
        return false;
    if( !TestFlagReadError_SetUnset<B, int16>() )
        return false;
    if( !TestFlagReadError_SetUnset<B, uint16>() )
        return false;
    if( !TestFlagReadError_SetUnset<B, int32>() )
        return false;
    if( !TestFlagReadError_SetUnset<B, uint32>() )
        return false;

    return true;
}

template<typename B>
bool TestDirtyData()
{
    B buff;

    // when growing Buffer, new data must be set to zero
    buff.Grow( 50 );
    for( uint32 idx = 0, max = 50; idx < max; idx++ )
    {
        uint8 tmp = idx + 1;

        buff >> tmp;

        if( buff.IsFlag( BUFFER_FLAG_ERROR_READ ) )
        {
            App.WriteLogF( _FUNC_, "<%s> FAIL : read error : index<%u>\n", typeid(B).name(), idx );
            return false;
        }

        if( tmp != 0 )
        {
            App.WriteLogF( _FUNC_, "<%s> FAIL : dirty data : current<%u> expected<0> index<%u>\n", typeid(B).name(), tmp, idx );
            return false;
        }
    }

    return true;
}

template<typename B>
bool TestScriptArray()
{
    B      buff;
    uint32 tmp = Random( std::numeric_limits<uint32>::min(), std::numeric_limits<uint32>::max() );
    uint32 tmp2 = tmp - 1;

    // simulate ScriptArray[2] = { tmp, tmp2 };
    buff << (uint32)2;
    buff << tmp;
    buff << tmp2;

    ScriptArray* arr = Script::CreateArray( "uint32[]" );
    buff >> arr;
    buff.Clear();
    buff << arr;
    arr->Release();

    buff >> tmp2;
    if( tmp2 != 2 )
    {
        WriteLogF( _FUNC_, "<%s> FAIL : invalid array length info : current<%u> expected<2>\n", typeid(B).name(), tmp );
        return false;
    }

    buff >> tmp2;
    if( tmp2 != tmp )
    {
        WriteLogF( _FUNC_, "<%s> FAIL : invalid data : current<%u> expected<%u>\n", typeid(B).name(), tmp2, tmp );
        return false;
    }

    buff >> tmp2;
    if( tmp2 != tmp - 1 )
    {
        WriteLogF( _FUNC_, "<%s> FAIL : invalid data : current<%u> expected<%u>\n", typeid(B).name(), tmp2, tmp - 1 );
        return false;
    }

    buff.Clear();

    return true;
}

template<typename B>
bool TestScriptString()
{
    B             buff;
    ScriptString* str0 = new ScriptString( ":ASD" );
    ScriptString* str1 = new ScriptString();

    buff << str0 >> str1;

    if( str0->c_std_str() != str1->c_std_str() )
    {
        WriteLogF( _FUNC_, "<%s> FAIL : current<%s> expected<%s>\n", typeid(B).name(), str1->c_str(), str0->c_str() );
        str0->Release();
        str1->Release();
        return false;
    }

    str0->Release();
    str1->Release();

    return true;
}

template<typename B>
bool TestAll()
{
    if( !TestGrow<B>() )
        return false;
    // if( !TestClear<B>() )
    //    return false;
    if( !TestReadWritePosition<B>() )
        return false;
    if( !TestFlags<B>() )
        return false;
    if( !TestDirtyData<B>() )
        return false;
    if( !TestScriptArray<B>() )
        return false;
    if( !TestScriptString<B>() )
        return false;

    return true;
}

template<typename B>
bool BufferTest()
{
    if( !TestAll<B>() )
        return false;

    return true;
}

///

template<typename B>
void BufferTestSpeed()
{
    B      buff;
    uint32 elementsCount[5] = { 10, 100, 1000, 10000, 100000 };

    for( uint32 idx = 0, len = sizeof(elementsCount) / sizeof(uint32); idx < len; idx++ )
    {
        uint32 count = elementsCount[idx];
        double readTime = 0, writeRawTime = 0, writeGrowTime = 0, growTime = 0;

        App.WriteLogF( _FUNC_, "<%s> %u ints : raw write\n", typeid(B).name(), count );
        writeRawTime = Timer::AccurateTick();
        for( uint32 c = 0; c < count; c++ )
        {
            buff << (int32)1207;
        }
        writeRawTime = Timer::AccurateTick() - writeRawTime;
        buff.Clear();

        uint32 growCount = count * sizeof(int32);
        App.WriteLogF( _FUNC_, "<%s> %u ints : grow\n", typeid(B).name(), count );
        growTime = Timer::AccurateTick();
        buff.Grow( growCount );
        growTime = Timer::AccurateTick() - growTime;

        App.WriteLogF( _FUNC_, "<%s> %u ints : grow write\n", typeid(B).name(), count );
        writeGrowTime = Timer::AccurateTick();
        for( uint32 c = 0; c < count; c++ )
        {
            buff << (int32)1207;
        }
        writeGrowTime = Timer::AccurateTick() - writeGrowTime;

        App.WriteLogF( _FUNC_, "<%s> %u ints : read\n", typeid(B).name(), count );
        readTime = Timer::AccurateTick();
        for( uint32 c = 0; c < count; c++ )
        {
            int32 tmp;
            buff >> tmp;
        }
        readTime = Timer::AccurateTick() - readTime;
        buff.Clear();

        App.WriteLogF( _FUNC_, "<%s> %u ints : read %gms, raw write %gms, grow write %gms, grow %gms\n", typeid(B).name(), count, readTime, writeRawTime, writeGrowTime, growTime );
        App.WriteLogF( _FUNC_, "\n" );
    }
}

#endif // __BUFFER__TEST__ //
