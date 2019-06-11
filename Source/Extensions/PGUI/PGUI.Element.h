#ifndef __PGUI__ELEMENT__
#define __PGUI__ELEMENT__

#include <map>
#include <string>

#include <Types.h>

namespace PGUI
{
    class Core;
    struct DrawCache;

    class Element
    {
        friend class PGUI::Core;
public:
        bool IsDrawEnabled;
        bool IsUpdateEnabled;
        bool IsKeyboardEnabled;
        bool IsMouseEnabled;

protected:

        // config

        int16  Left;
        int16  Top;
        uint16 Width;
        uint16 Height;

        bool   BackgroundVisible;
        bool   BorderVisible;
        uint8  BorderThickness;
        uint8  Padding; // 12b

        // runtime

        PGUI::Core*              GUI;
        PGUI::Element*           Parent;
        std::map<uint, Element*> Elements;

        bool                     NeedUpdateDecorations; // if true, background/border cache will be rebuilt on next Update() call
        DrawCache*               BackgroundCache;
        DrawCache*               BorderCache;

        bool                     MousePressed;
        int8                     MouseButton;


public:
        Element( PGUI::Core* gui, uint16 width = 0, uint16 height = 0, int16 left = 0, int16 top = 0 );
        virtual ~Element();

        int16 GetLeft( bool ignoreParent = false );
        void  SetLeft( int16 value );
        int16 GetRight( bool ignoreParent = false );
        int16 GetTop( bool ignoreParent = false );
        void  SetTop( int16 value );
        int16 GetBottom( bool ignoreParent = false );
        void  GetPosition( int16& left, int16& top, bool ignoreParent = false );
        void  SetPosition( int16 left, int16 top );
        void  SetPositionAt( int16 xx, int16 yy, bool ignoreParent = false );

        uint16       GetWidth();
        void         SetWidth( uint16 value );
        uint16       GetHeight();
        void         SetHeight( uint16 value );
        void         GetSize( uint16& width, uint16& height );
        void         SetSize( uint16 width, uint16 height );
        void         SetSize( PGUI::Element* other );
        virtual void AutoSize() {}

        //

        bool IsInside( int16 x, int16 y );

        //

        bool GetBackgroundVisible();
        void SetBackgroundVisible( bool value );

        bool  GetBorderVisible();
        void  SetBorderVisible( bool value );
        uint8 GetBorderThickness();
        void  SetBorderThickness( uint8 value );

        //

        bool           IsElement( uint id );
        bool           AddElement( uint id, PGUI::Element* element );
        PGUI::Element* GetElement( uint id );

        template<typename T>
        T* GetElementAs( uint id )
        {
            return (T*)GetElement( id );
        }

private:
        void SetGUI( PGUI::Core* gui ); // should be final...

public:
        virtual void Update();
        virtual void UpdateDecorations();

        // Draw

        virtual void Draw();
        virtual void DrawBack();
        virtual void DrawContent() {}
        virtual void DrawFront();
        virtual void DrawMap();

        // Keyboard

        virtual bool KeyDown( const uint8& key, const std::string& keyString );
        virtual bool KeyUp( const uint8& key, const std::string& keyString );

        // Mouse

        virtual bool MouseDown( uint8 click, int16 x, int16 y );
        virtual void MouseMove( int16 fromX, int16 fromY, int16 toX, int16 toY );
        virtual bool MouseUp( uint8 click, int16 x, int16 y );
        virtual void MouseClick() {}

        // Misc

        virtual void InputLost();
    };
};

#endif // __PGUI__ELEMENT__ //
