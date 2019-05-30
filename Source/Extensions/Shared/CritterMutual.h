#ifndef __FOC__CRITTER_MUTUAL__
#define __FOC__CRITTER_MUTUAL__

#if defined (FOCLASSIC_CLIENT)
# include <CritterCl.h>
typedef CritterCl CritterMutual;
#elif defined (FOCLASSIC_SERVER)
# include <Critter.h>
typedef Critter   CritterMutual;
#endif

#endif // __FOC__CRITTER_MUTUAL__ //
