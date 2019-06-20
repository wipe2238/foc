#include <cstdlib>

#include <App.h>

#include "PGUI.Core.h"
#include "PGUI.Draw.h"
#include "PGUI.Element.h"

//

PGUI::Element::Element( PGUI::Core* gui, uint16 width /* = 0 */, uint16 height /* = 0 */, int16 left /* = 0 */, int16 top /* = 0 */ ) :
// public
    IsDrawEnabled( true ), IsUpdateEnabled( true ), IsKeyboardEnabled( true ), IsMouseEnabled( true ),
// protected
    Left( left ), Top( top ), Width( width ), Height( height ),
    BackgroundVisible( false ),
    BorderVisible( false ), BorderThickness( 1 ),
    _Padding(),
    GUI( gui ), Parent( nullptr ), Elements(),
    NeedUpdateDecorations( true ),
    BackgroundCache(), BorderCache(),
    MousePressed( false ), MouseButton( -1 )
{
    GUI->NOP();
}

PGUI::Element::~Element()
{
    if( GUI->Debug )
        App.WriteLogF( _FUNC_, "\n" );

    Draw::DeleteData( BackgroundCache );
    Draw::DeleteData( BorderCache );

    Elements.clear();
}

int16 PGUI::Element::GetLeft( bool ignore_parent /* = false */ )
{
    if( !ignore_parent && Parent )
        return Parent->GetLeft() + Left;

    return Left;
}

void PGUI::Element::SetLeft( int16 left )
{
    if( GetLeft( true ) != left )
    {
        if( GUI->Debug )
            App.WriteLogF( _FUNC_, "(%d)\n", left );

        Left = left;
    }
}

int16 PGUI::Element::GetRight( bool ignoreParent /* = false */ )
{
    return GetLeft( ignoreParent ) + GetWidth();
}

int16 PGUI::Element::GetTop( bool ignore_parent /* = false */ )
{
    if( !ignore_parent && Parent )
        return Parent->GetTop() + Top;

    return Top;
}

void PGUI::Element::SetTop( int16 top )
{
    if( GetTop( true ) != top )
    {
        if( GUI->Debug )
            App.WriteLogF( _FUNC_, "(%d)\n", top );

        Top = top;
    }
}

int16 PGUI::Element::GetBottom( bool ignoreParent )
{
    return GetTop( ignoreParent ) + GetHeight();
}

void PGUI::Element::GetPosition( int16& left, int16& top, bool ignoreParent /* = false */ )
{
    left = GetLeft( ignoreParent );
    top = GetTop( ignoreParent );
}

void PGUI::Element::SetPosition( int16 left, int16 top )
{
    if( GUI->Debug )
        App.WriteLogF( _FUNC_, "(%d,%d)\n", left, top );

    SetLeft( left );
    SetTop( top );
}

void PGUI::Element::SetPositionAt( int16 xx, int16 yy, bool ignoreParent /* = false */ )
{
    if( GUI->Debug )
        App.WriteLogF( _FUNC_, "(%d,%d,%s)\n", xx, yy, ignoreParent ? "true" : "false" );

    uint16 width = GetWidth(), height = GetHeight(), parentWidth = GUI->GetScreenWidth(), parentHeight = GUI->GetScreenHeight();

    if( !ignoreParent && Parent )
        Parent->GetSize( parentWidth, parentHeight );

    int16 left = 0, top = 0;

    if( xx < 0 )
        left = 0 + std::abs( ++xx );
    else if( xx == 0 )
        left = parentWidth / 2 - width / 2;
    else if( xx > 0 )
        left = parentWidth - width - --xx;

    if( yy < 0 )
        top = 0 + std::abs( ++yy );
    else if( yy == 0 )
        top = parentHeight / 2 - height / 2;
    else if( yy > 0 )
        top = parentHeight - height - --yy;

    if( GetLeft( true ) != left || GetTop( true ) != top )
        SetPosition( left, top );
}

uint16 PGUI::Element::GetWidth()
{
    return Width;
}

void PGUI::Element::SetWidth( uint16 value )
{
    if( GetWidth() != value )
    {
        if( GUI->Debug )
            App.WriteLogF( _FUNC_, "(%u)\n", value );

        Width = value;
        NeedUpdateDecorations = true;
    }
}

uint16 PGUI::Element::GetHeight()
{
    return Height;
}

void PGUI::Element::SetHeight( uint16 value )
{
    if( GetHeight() != value )
    {
        if( GUI->Debug )
            App.WriteLogF( _FUNC_, "(%u)\n", value );

        Height = value;
        NeedUpdateDecorations = true;
    }
}

void PGUI::Element::GetSize( uint16& width, uint16& height )
{
    width = GetWidth();
    height = GetHeight();
}

void PGUI::Element::SetSize( uint16 width, uint16 height )
{
    SetWidth( width );
    SetHeight( height );
}

void PGUI::Element::SetSize( PGUI::Element* other )
{
    uint16 width = 0, height = 0;

    if( other )
        other->GetSize( width, height );

    if( GUI->Debug )
        App.WriteLogF( _FUNC_, "(other) = %ux%u\n", width, height );

    SetSize( width, height );
}

//

bool PGUI::Element::IsInside( int16 x, int16 y )
{
    return x >= GetLeft() && x <= GetRight() && y >= GetTop() && y <= GetBottom();
}

bool PGUI::Element::IsInsideStrict( int16 x, int16 y )
{
    if( IsInside( x, y ) )
        return true;

    for( auto it = Elements.begin(), end = Elements.end(); it != end; ++it )
    {
        PGUI::Element* element = it->second;

        if( !element )
            continue;

        if( element->IsInsideStrict( x, y ) )
            return true;
    }

    return false;
}

bool PGUI::Element::GetBackgroundVisible()
{
    return BackgroundVisible;
}

void PGUI::Element::SetBackgroundVisible( bool value )
{
    if( GetBackgroundVisible() != value )
    {
        if( GUI->Debug )
            App.WriteLogF( _FUNC_, "(%s)\n", value ? "true" : "false" );

        BackgroundVisible = value;
        NeedUpdateDecorations = true;
    }
}

bool PGUI::Element::GetBorderVisible()
{
    return BorderVisible;
}

void PGUI::Element::SetBorderVisible( bool value )
{
    if( GetBorderVisible() != value )
    {
        if( GUI->Debug )
            App.WriteLogF( _FUNC_, "(%s)\n", value ? "true" : "false" );
        BorderVisible = value;
        NeedUpdateDecorations = true;
    }
}

uint8 PGUI::Element::GetBorderThickness()
{
    return BorderThickness;
}

void PGUI::Element::SetBorderThickness( uint8 value )
{
    if( GetBorderThickness() != value )
    {
        if( GUI->Debug )
            App.WriteLogF( _FUNC_, "(%u)\n", value );

        BorderThickness = value;

        if( value && !GetBorderVisible() )
            SetBorderVisible( true );
        else if( !value && GetBorderVisible() )
            SetBorderVisible( false );

        NeedUpdateDecorations = true;
    }
}

//

uint PGUI::Element::GetID()
{
    if( Parent )
        return Parent->GetElementID( this );

    return 0;
}

bool PGUI::Element::IsElement( uint id )
{
    if( !id )
        return false;

    return Elements.find( id ) != Elements.end();
}

bool PGUI::Element::AddElement( uint id, PGUI::Element* element )
{
    if( !id )
    {
        App.WriteLogF( _FUNC_, " ERROR : id is zero\n" );
        return false;
    }

    if( !element )
    {
        App.WriteLogF( _FUNC_, " ERROR : element is null\n" );
        return false;
    }

    if( IsElement( id ) )
    {
        App.WriteLogF( _FUNC_, " ERROR : element with id<%u> already added\n", id );
        return false;
    }

    element->SetGUI( GUI );
    element->Parent = this;

    Elements[id] = element;

    return true;
}

PGUI::Element* PGUI::Element::GetElement( uint id )
{
    if( !id )
        return nullptr;

    auto it = Elements.find( id );
    if( it != Elements.end() )
        return it->second;

    return nullptr;
}

uint PGUI::Element::GetElementID( PGUI::Element* element )
{
    if( element )
    {
        for( auto it = Elements.begin(), end = Elements.end(); it != end; ++it )
        {
            if( it->second == element )
                return it->first;
        }
    }

    return 0;
}

//

void PGUI::Element::SetGUI( PGUI::Core* gui )
{
    GUI = gui;

    for( auto it = Elements.begin(), end = Elements.end(); it != end; ++it )
    {
        it->second->SetGUI( gui );
    }
}

void PGUI::Element::Update()
{
    if( !IsUpdateEnabled )
        return;

    if( (GetBackgroundVisible() || GetBorderVisible() ) && NeedUpdateDecorations )
        UpdateDecorations();

    for( auto it = Elements.begin(), end = Elements.end(); it != end; ++it )
    {
        PGUI::Element* element = it->second;

        if( element->IsUpdateEnabled )
            element->Update();
    }
}

void PGUI::Element::UpdateDecorations()
{
    if( !IsUpdateEnabled )
        return;

    if( GUI->Debug )
        App.WriteLogF( _FUNC_, "\n" );

    Draw::DeleteData( BackgroundCache );
    Draw::DeleteData( BorderCache );

    if( GetBackgroundVisible() )
    {
        BackgroundCache = new PGUI::DrawData();
        BackgroundCache->MakeRectangle( 0, 0, GetWidth(), GetHeight(), GUI->Settings.ColorBackground );
    }

    if( GetBorderVisible() )
    {
        BorderCache = new PGUI::DrawData();
        BorderCache->MakeRectangleFrame( 0, 0, GetWidth(), GetHeight(), GUI->Settings.ColorBorder, GetBorderThickness() );
    }

    NeedUpdateDecorations = false;
}

//

void PGUI::Element::Draw()
{
    if( !IsDrawEnabled )
        return;

    DrawBack();
    DrawElements();
    DrawContent();
    DrawFront();
}

void PGUI::Element::DrawBack()
{
    if( !IsDrawEnabled )
        return;

    if( GetBackgroundVisible() )
        Draw::RenderData( BackgroundCache, GetLeft(), GetTop() );
}

void PGUI::Element::DrawElements()
{
    if( !IsDrawEnabled )
        return;

    for( auto it = Elements.begin(), end = Elements.end(); it != end; ++it )
    {
        PGUI::Element* element = it->second;

        if( element->IsDrawEnabled )
            element->Draw();
    }
}

void PGUI::Element::DrawFront()
{
    if( !IsDrawEnabled )
        return;

    if( GetBorderVisible() )
        Draw::RenderData( BorderCache, GetLeft(), GetTop() );
}

void PGUI::Element::DrawMap()
{
    if( !IsDrawEnabled )
        return;

    for( auto it = Elements.begin(), end = Elements.end(); it != end; ++it )
    {
        PGUI::Element* element = it->second;

        if( element->IsDrawEnabled )
            element->DrawMap();
    }
}

bool PGUI::Element::KeyDown( uint8 key, std::string& keyText )
{
    if( !IsKeyboardEnabled )
        return false;

    if( GUI->Debug )
        App.WriteLogF( _FUNC_, "(%u,\"%s\")\n", key, keyText.c_str() );

    for( auto it = Elements.begin(), end = Elements.end(); it != end; ++it )
    {
        PGUI::Element* element = it->second;

        if( !element || !element->IsKeyboardEnabled )
            continue;

        if( element->KeyDown( key, keyText ) )
            return true;
    }

    return false;
}

bool PGUI::Element::KeyUp( uint8 key, std::string& keyText )
{
    if( !IsKeyboardEnabled )
        return false;

    if( GUI->Debug )
        App.WriteLogF( _FUNC_, "(%u,\"%s\")\n", key, keyText.c_str() );

    for( auto it = Elements.begin(), end = Elements.end(); it != end; ++it )
    {
        PGUI::Element* element = it->second;

        if( !element || !element->IsKeyboardEnabled )
            continue;

        if( element->KeyUp( key, keyText ) )
            return true;
    }

    return false;
}

bool PGUI::Element::MouseDown( uint8 button, int16 mx, int16 my )
{
    if( !IsMouseEnabled )
        return false;

    bool result = false;
    for( auto it = Elements.begin(), end = Elements.end(); it != end; ++it )
    {
        PGUI::Element* element = it->second;

        if( !element || !element->IsMouseEnabled )
            continue;

        result = element->MouseDown( button, mx, my ) ? true : result;
    }

    if( !result )
    {
        MousePressed = IsInside( mx, my );
        MouseButton = MousePressed ? button : uint8( -1 );

        result = MousePressed;
    }

    return result;
}

void PGUI::Element::MouseMove( int16 fromMX, int16 fromMY, int16 toMX, int16 toMY )
{
    if( !IsMouseEnabled )
        return;

    for( auto it = Elements.begin(), end = Elements.end(); it != end; ++it )
    {
        PGUI::Element* element = it->second;

        if( !element || !element->IsMouseEnabled )
            continue;

        element->MouseMove( fromMX, fromMY, toMX, toMY );
    }
}

bool PGUI::Element::MouseUp( uint8 button, int16 mx, int16 my )
{
    if( !IsMouseEnabled )
        return false;

    bool result = false;
    for( auto it = Elements.begin(), end = Elements.end(); it != end; ++it )
    {
        PGUI::Element* element = it->second;

        if( !element || !element->IsMouseEnabled )
            continue;

        result = element->MouseUp( button, mx, my ) ? true : result;
    }

    if( !result )
    {
        if( IsInside( mx, my ) && MousePressed )
        {
            if( GUI->Debug )
                App.WriteLogF( _FUNC_, "(%u,%d,%d) : MouseClick(%u)\n", button, mx, my, button );

            MouseClick( button );
            result = true;
        }
    }

    MousePressed = false;
    MouseButton = uint8( -1 );

    return result;
}

void PGUI::Element::InputLost()
{
    for( auto it = Elements.begin(), end = Elements.end(); it != end; ++it )
    {
        PGUI::Element* element = it->second;

        if( !element )
            continue;

        element->InputLost();
    }
}
