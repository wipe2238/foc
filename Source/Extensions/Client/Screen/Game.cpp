#include <App.h>
#include <CritterCl.h>
#include <GameOptions.h>
#include <Item.h>
#include <ResourceManager.h>
#include <SpriteManager.h>

#include "Client/Screen/Game.h"
#include "Shared/SScriptFunc.h"

#include "PGUI/PGUI.Core.h"
#include "PGUI/PGUI.Element.h"
#include "PGUI/PGUI.Label.h"
#include "PGUI/PGUI.Screen.h"

#define IF_LOG( what ) \
    if( what )         \
        App.WriteLogF( _FUNC_, " : %s\n",# what )

FOC::Screen::Game::Game( PGUI::Core* gui ) : PGUI::Screen( gui )
{
    uint16 itemWidth = 100, itemHeight = 50;

    IsStatic = true;
    Layer = 3;
    SetSize( itemWidth * 3, GUI->GetScreenHeight() );

    PGUI::Label* label;

    label = new PGUI::Label( gui, "0" );
    label->AutoSize();
    AddElement( ID::HitPoints, label );
    uint hpBottom = label->GetBottom( true );

    label = new PGUI::Label( gui, "*" );
    label->AutoSize();
    label->SetTop( hpBottom );
    AddElement( ID::ActionPoints, label );
    int            apBottom = label->GetBottom( true );

    PGUI::Element* element = new PGUI::Element( gui, itemWidth, itemHeight );
    AddElement( ID::BodyArmor, element );
    element->SetBorderVisible( true );
    element->SetBackgroundVisible( true );
    element->SetPositionAt( 0, -apBottom - 1 );
    uint armorLeft = element->GetLeft( true ), armorRight = element->GetRight( true ), armorTop = element->GetTop( true );

    SetHeight( element->GetBottom( true ) );
    SetPositionAt( 1, 1 );

    element = new PGUI::Element( gui, itemWidth, itemHeight );
    AddElement( ID::HandLeft, element );
    element->SetBorderVisible( true );
    element->SetBackgroundVisible( true );
    element->SetPosition( armorLeft - element->GetWidth(), armorTop );

    element = new PGUI::Element( gui, itemWidth, itemHeight );
    AddElement( ID::HandRight, element );
    element->SetBorderVisible( true );
    element->SetBackgroundVisible( true );
    element->SetPosition( armorRight, armorTop );
}

FOC::Screen::Game::~Game()
{
    App.WriteLogF( _FUNC_, "\n" );
}

void FOC::Screen::Game::Update()
{
    CritterCl* chosen = FOClient::SScriptFunc::Global_GetChosen();
    IF_LOG( !chosen );
    if( !chosen )
        return;

    bool debug = GUI->Debug;
    GUI->Debug = false;

    PGUI::Label* label = GetElementType<PGUI::Label>( ID::HitPoints );

    label->SetText( std::to_string( (long long)chosen->GetParam( ST_CURRENT_HP ) ) );
    label->AutoSize();
    label->SetLeft( GetWidth() / 2 - label->GetWidth() / 2 );

    label = GetElementType<PGUI::Label>( ID::ActionPoints );

    int ap = chosen->GetRawParam( ST_CURRENT_AP ) / AP_DIVIDER;
    if( ap > 0 )
        label->SetText( std::string( ap, '*' ) );
    else if( ap < 0 )
        label->SetText( std::string( std::abs( ap ), '-' ) );
    else
        label->SetText( "" );

    ap = chosen->GetParam( ST_MOVE_AP );
    if( ap > 0 )
        label->AppendText( std::string( ap, '+' ) );

    label->AutoSize();
    label->SetLeft( GetWidth() / 2 - label->GetWidth() / 2 );

    GUI->Debug = debug;

    PGUI::Screen::Update();
}

void FOC::Screen::Game::Draw()
{
    PGUI::Screen::Draw();

    CritterCl* chosen = FOClient::SScriptFunc::Global_GetChosen();
    IF_LOG( !chosen );
    if( !chosen )
        return;

    DrawItem( ID::HandLeft, chosen->ItemSlotMain );
    DrawItem( ID::BodyArmor, chosen->ItemSlotArmor );
    DrawItem( ID::HandRight, chosen->ItemSlotExt );
}

void FOC::Screen::Game::DrawItem( uint id, Item* item )
{
    PGUI::Element* element = GetElement( id );

    IF_LOG( !element );
    IF_LOG( !item );
    IF_LOG( item->IsNotValid );
    IF_LOG( !item->GetId() );
    if( !element || !item || item->IsNotValid || !item->GetId() )
        return;

    AnyFrames* sprite = ResMngr.GetInvAnim( item->GetPicInv() );
    IF_LOG( !sprite );
    if( !sprite )
        return;

    int size = std::min( element->GetWidth(), element->GetHeight() );
    if( size > 5 )
        size -= 5;

    int         x = element->GetLeft() + (element->GetWidth() / 2 - size / 2);
    int         y = element->GetTop() + (element->GetHeight() / 2 - size / 2);

    static bool scratch = false;
    static bool center = true;

    SprMngr.DrawSpriteSize( sprite, x, y, (float)size, (float)size, scratch, center, GUI->Settings.ColorItem );
}

void FOC::Screen::Game::OnOpen()
{
    GameOpt.DebugInfo = true;

    PGUI::Element::Update();
}

void FOC::Screen::Game::OnClose()
{
    GameOpt.DebugInfo = false;

    GUI->CloseAllScreens();

    PGUI::Element::Update();
}
