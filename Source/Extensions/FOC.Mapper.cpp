#include <string>

#include <Addons/scriptstring.h>
#include <Extension.h>

#include "FOC.Mapper.h"

using namespace std;

void InitExtensionsMapper() // used by engine
{
    FOC::Init<FOC::Mapper>();
}

void FinishExtensionsMapper() // used by engine
{
    FOC::Finish<FOC::Mapper>();
}

static void start()
{}

static uint loop()
{
    return 100;
}

static bool console_message( ScriptString& message )
{
    return false;
}

static void render_iface( uint layer )
{}

static void render_map()
{}

static bool mouse_down( int click )
{
    return false;
}

static bool mouse_up( int click )
{
    return false;
}

static bool key_down( uint8 key, ScriptString& keyText )
{
    return false;
}

static bool key_up( uint8 key, ScriptString& keyText )
{
    return false;
}

static void input_lost()
{}

FOC::Mapper::Mapper() : Extension()
{}

FOC::Mapper::~Mapper()
{}

size_t FOC::Mapper::GetFunctionAddress( const string& name )
{
    return 0;
}
