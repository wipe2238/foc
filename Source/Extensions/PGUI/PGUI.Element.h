#ifndef __PGUI__ELEMENT__
#define __PGUI__ELEMENT__

#include <map>
#include <string>

#include <Types.h>

namespace PGUI
{
    class Core;
    class DrawData;

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
        uint8  _Padding; // 12b

        // runtime

        PGUI::Core*              GUI;
        PGUI::Element*           Parent;
        std::map<uint, Element*> Elements;

        bool                     NeedUpdateDecorations; // if true, background/border cache will be rebuilt on next Update() call
        DrawData*                BackgroundCache;
        DrawData*                BorderCache;

        bool                     MousePressed;
        uint8                    MouseButton;


public:
        Element( PGUI::Core* gui, uint16 width = 0, uint16 height = 0, int16 left = 0, int16 top = 0 );
        virtual ~Element();

        virtual int16 GetLeft( bool ignoreParent = false );
        virtual void  SetLeft( int16 value );
        virtual int16 GetRight( bool ignoreParent = false );
        virtual int16 GetTop( bool ignoreParent = false );
        virtual void  SetTop( int16 value );
        virtual int16 GetBottom( bool ignoreParent = false );
        virtual void  GetPosition( int16& left, int16& top, bool ignoreParent = false );
        virtual void  SetPosition( int16 left, int16 top );
        virtual void  SetPositionAt( int16 xx, int16 yy, bool ignoreParent = false );

        virtual uint16 GetWidth();
        virtual void   SetWidth( uint16 value );
        virtual uint16 GetHeight();
        virtual void   SetHeight( uint16 value );
        virtual void   GetSize( uint16& width, uint16& height );
        virtual void   SetSize( uint16 width, uint16 height );
        virtual void   SetSize( PGUI::Element* other );
        virtual void   AutoSize() {}

        //

        virtual bool IsInside( int16 x, int16 y );
        virtual bool IsInsideStrict( int16 x, int16 y );

        //

        virtual bool GetBackgroundVisible();
        virtual void SetBackgroundVisible( bool value );

        virtual bool  GetBorderVisible();
        virtual void  SetBorderVisible( bool value );
        virtual uint8 GetBorderThickness();
        virtual void  SetBorderThickness( uint8 value );

        //

        virtual uint   GetID();
        bool           IsElement( uint id );
        bool           AddElement( uint id, PGUI::Element* element );
        PGUI::Element* GetElement( uint id );
        uint           GetElementID( PGUI::Element* element );

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
        virtual void DrawElements();
        virtual void DrawContent() {}
        virtual void DrawFront();
        virtual void DrawMap();

        // Keyboard

        virtual bool KeyDown( uint8 key, std::string& keyText );
        virtual bool KeyUp( uint8 key, std::string& keyText );

        // Mouse

        virtual bool MouseDown( uint8 button, int16 mx, int16 my );
        virtual void MouseMove( int16 fromMX, int16 fromMY, int16 toMX, int16 toMY );
        virtual bool MouseUp( uint8 button, int16 mx, int16 my );
        virtual void MouseClick( uint8 button ) {}

        // Misc

        virtual void InputLost();
    };
};

#endif // __PGUI__ELEMENT__ //
