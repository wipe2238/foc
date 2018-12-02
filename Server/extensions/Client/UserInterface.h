#ifndef __CLIENT_INTERFACE__
#define __CLIENT_INTERFACE__

#include <extension.h>

class UserInterface
{
private:
	set<int> Screens;

public:
	// reserved functions

	void ShowScreen(int screen, int p0, int p1, int p2); // screen_change( true, ... );
	void HideScreen(int screen, int p0, int p1, int p2); // screen_change( false, ... );
	void GetActiveScreens(ScriptArray& result); // get_active_screens(...);
	void RenderInterface(uint layer); // render_iface(...);
};

extern UserInterface UI;

#endif // __CLIENT_INTERFACE__ //
