#ifndef  __PGUI__TEXT_BOX__
#define __PGUI__TEXT_BOX__

#include <list>
#include <string>

#include <Types.h>

#include "PGUI.Element.h"

namespace PGUI
{
    class Core;

    class TextBox : public PGUI::Element
    {
        friend class PGUI::Core;
public:
        bool IsDrawCursorEnabled;
        uint CursorTime;

protected:
        std::string            Cursor;
        std::list<std::string> CursorList;
        uint                   CursorChanged;
        uint                   CursorPosition;

        std::string            Text;
        std::string            TextMask;
        std::string            TextToDraw;

public:
        TextBox( PGUI::Core* gui );
        virtual ~TextBox();

        virtual void AutoSize() override;

        virtual std::string GetText();
        virtual void        SetText( const std::string& text );
        virtual void        AppendText( const std::string& text );
        virtual void        SetTextMask( const std::string& mask );

        virtual void UpdateDrawText();

public:
        virtual void Update() override;
        virtual void DrawContent() override;
        virtual bool KeyDown( uint8 key, std::string& keyText ) override;
        virtual bool KeyUp( uint8 key, std::string& keyText ) override;
    };
};

#endif // __PGUI__TEXT_BOX__ //
