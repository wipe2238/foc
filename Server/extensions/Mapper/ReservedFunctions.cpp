#include <extension.h>

EXPORT void start()
{}

EXPORT uint loop()
{
	return 100;
}

EXPORT bool console_message( ScriptString& message )
{
	return false;
}

EXPORT void render_iface( uint layer )
{}

EXPORT void render_map()
{}

EXPORT bool mouse_down( int click )
{
	return false;
}

EXPORT bool mouse_up( int click )
{
	return false;
}

EXPORT bool key_down(uint8 key, ScriptString& keyText)
{
	return false;
}

EXPORT bool key_up( uint8 key, ScriptString& keyText )
{
	return false;
}

EXPORT void input_lost()
{}
