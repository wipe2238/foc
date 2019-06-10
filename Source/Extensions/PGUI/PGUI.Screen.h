#ifndef __PGUI__SCREEN__
#define __PGUI__SCREEN__

#include <set>
#include <string>

#include <Types.h>

#include "PGUI.Element.h"

class Ini;

namespace PGUI
{
    class Core;

    class Screen : public PGUI::Element
    {
        friend class PGUI::Core;
public:
        bool  IsStatic;
        uint8 Layer;

protected:

        // current layer index, available only during Draw()
        uint8 DrawLayer;
        void* ScreenData;

public:
        Screen( PGUI::Core* gui, uint width = 0, uint height = 0, int left = 0, int top = 0 );
        virtual ~Screen();

        bool LoadSettings( Ini* ini, const std::string& section );

        virtual void AutoSize() override;

protected: // reserved functions handling
        virtual bool MouseDown( uint8 click, int16 x, int16 y ) override;
        virtual void MouseMove( int16 fromX, int16 fromY, int16 toX, int16 toY ) override;
        virtual bool MouseUp( uint8 click, int16 x, int16 y ) override;

public:         // events
        virtual void OnOpen()                {}
        virtual void OnClose()               {}
        virtual void OnActive( bool active ) {}
    };
};

#endif // __PGUI__SCREEN__ //
