#ifndef __PGUI__LABEL__
#define __PGUI__LABEL__

#include <string>

#include "PGUI.Element.h"

namespace PGUI
{
    class Core;

    class Label : public PGUI::Element
    {
        friend class PGUI::Core;
protected:
        std::string Text;

public:
        Label( PGUI::Core* gui, const std::string& text = std::string() );
        virtual ~Label();

        std::string GetText();
        void        SetText( const std::string& value );
        void        AppendText( const std::string& value );

        virtual void AutoSize() override;

protected:
        virtual void UpdateDecorations() override;

public:
        virtual void DrawContent() override;
    };
};

#endif // __PGUI__LABEL__ //
