#ifndef __FOC__DIALOGS__
#define __FOC__DIALOGS__

#include <Critter.h>
#include <ProtoMap.h>
#include <Types.h>

namespace FOC
{
    class DialogsManager
    {
public:
        bool Debug;

        DialogsManager();
        ~DialogsManager();

        void Init();

        bool Scenery( Critter& player, MapObject& scenery, int skill, Item* item, int dialogId );

    };
};

#endif // __DIALOGS__ //
