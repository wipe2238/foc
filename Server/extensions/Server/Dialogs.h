#ifndef __DIALOGS__
#define __DIALOGS__

#include <extension.h>

class DialogsManager
{
public:
    bool Debug;

    DialogsManager();
    ~DialogsManager();

public:
    bool RunDialog( Critter& player, uint dialogId, uint16 hexX, uint16 hexY, bool ignoreDistance );
};

extern DialogsManager Dialogs;

#endif // __DIALOGS__ //
