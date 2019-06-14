#ifndef __PGUI__BUTTON__
#define __PGUI__BUTTON__

#include <string>

#include "PGUI.Element.h"
#include "PGUI.Event.h"

namespace PGUI
{
    class Core;
    struct DrawCache;
    class Label;

    class Button : public PGUI::Element
    {
        friend class PGUI::Core;
protected:
        bool       IsHover;
        DrawCache* BackgroundHoverCache;

public:
        Button( PGUI::Core* gui );
        virtual ~Button();

        PGUI::Label* GetLabel();
        void         SetText( const std::string& text );

public:
        virtual void UpdateDecorations() override;
        virtual void DrawBack() override;
        virtual void MouseMove( int16 fromX, int16 fromY, int16 toX, int16 toY ) override;
        virtual void MouseClick( uint8 button ) override;

public:
        Event::UIntButton EventMouseClick;
    };
}

#endif // __PGUI__BUTTON__ //
