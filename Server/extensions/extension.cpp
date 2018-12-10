#include <extension.h>

bool IsCompiler = false;
bool FirstStart = false;

FOCLASSIC_EXTENSION_ENTRY( compiler )
{
    IsCompiler = compiler;
}

#if !defined (FOCLASSIC_BLEEDING_EDGE)
# if defined (__CLIENT)
#  pragma message("Compiling as Client...")
# elif defined (__MAPPER)
#  pragma message("Compiling as Mapper...")
# elif defined (__SERVER)
#  pragma message("Compiling as Server...")
# endif
#else
# if defined (__CLIENT)
#  pragma message("Compiling as Client... (Bleeding Edge mode)")
# elif defined (__MAPPER)
#  pragma message("Compiling as Mapper... (Bleeding Edge mode)")
# elif defined (__SERVER)
#  pragma message("Compiling as Server... (Bleeding Edge mode)")
# endif
#endif
