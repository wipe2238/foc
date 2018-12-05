#include <extension.h>

bool IsCompiler = false;
bool FirstTime = false;

FOCLASSIC_EXTENSION_ENTRY( compiler )
{
	Log("DllMainEx(compiler = %s)\n", compiler ? "true" : "false" );
	IsCompiler = compiler;
}

#if defined(__CLIENT)
# pragma message("Compiling as Client...")
#elif defined(__MAPPER)
# pragma message("Compiling as Mapper...")
#elif defined(__SERVER)
# pragma message("Compiling as Server...")
#endif
