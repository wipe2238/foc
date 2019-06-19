#include <App.h>
#include <Item.h>
#include <ResourceManager.h>

#include "Inventory.h"

#include "PGUI/PGUI.Core.h"

FOC::Element::ItemSlot::ItemSlot( PGUI::Core* gui ) : PGUI::Element( gui ),
// private
    ItemID( 0 ),
    Sprite( nullptr )
{}

FOC::Element::ItemSlot::~ItemSlot()
{}

void FOC::Element::ItemSlot::ClearItem()
{
    ItemID = 0;
    Sprite = nullptr;

    SetBackgroundVisible( false );
}

bool FOC::Element::ItemSlot::IsItemSet()
{
    return GetItemID() != 0;
}

uint FOC::Element::ItemSlot::GetItemID()
{
    return ItemID;
}

void FOC::Element::ItemSlot::SetItem( Item& item )
{
    ItemID = item.GetId();
    Sprite = ResMngr.GetInvAnim( item.GetPicInv() );

    SetBackgroundVisible( true );
}

void FOC::Element::ItemSlot::DrawContent()
{
    if( !IsDrawEnabled )
        return;

    if( !GetItemID() || !Sprite )
        return;

    SprMngr.DrawSpriteSize( Sprite, GetLeft(), GetTop(), (float)GetWidth(), (float)GetHeight(), false, true, GUI->Settings.ColorItem );
}

//

FOC::Screen::Inventory::Inventory( PGUI::Core* gui ) : PGUI::Screen( gui ),
// private
    SlotSize( 50 ), SlotCount( 20 )
{
    Layer = 3;
    SetBorderVisible( true );
    SetSize( SlotSize * 5, SlotSize * 5 );
    SetPositionAt( 0, 0 );

    for( uint id = 1; id <= SlotCount; id++ )
    {
        FOC::Element::ItemSlot* slot = new FOC::Element::ItemSlot( GUI );

        slot->IsKeyboardEnabled = false;
        slot->SetBorderVisible( true );

        AddElement( id, slot );
    }

    RefreshSlots();
}

FOC::Screen::Inventory::~Inventory()
{}

void FOC::Screen::Inventory::RefreshSlots()
{
    uint cols = GetSlotColumns(), rows = GetSlotRows();

    for( auto it = Elements.begin(), end = Elements.end(); it != end; ++it )
    {
        uint id = it->first - 1;

        if( id <= 1000000 )
        {
            FOC::Element::ItemSlot* slot = (FOC::Element::ItemSlot*)it->second;

            slot->SetSize( SlotSize, SlotSize );

            slot->SetLeft( (id % cols) * SlotSize );
            slot->SetTop( (id / rows) * SlotSize );
        }
    }
}

uint FOC::Screen::Inventory::GetSlotSize()
{
    return SlotSize;
}

uint FOC::Screen::Inventory::GetSlotColumns()
{
    return GetWidth() / GetSlotSize();
}

uint FOC::Screen::Inventory::GetSlotRows()
{
    return GetHeight() / GetSlotSize();
}

void FOC::Screen::Inventory::SetSlotSize( uint size )
{
    if( SlotSize != size )
    {
        SlotSize = size;
        RefreshSlots();
    }
}

//

void FOC::Screen::Inventory::ItemInvIn( Item& item )
{
    if( GUI->Debug )
        App.WriteLogF( _FUNC_, " : item<%u>\n", item.GetId() );

    for( auto it = Elements.begin(), end = Elements.end(); it != end; ++it )
    {
        if( it->first <= 1000000 )
        {
            FOC::Element::ItemSlot* slot = (FOC::Element::ItemSlot*)it->second;

            if( !slot->IsItemSet() || slot->GetItemID() == item.GetId() )
            {
                slot->SetItem( item );
                break;
            }
        }
    }
}

void FOC::Screen::Inventory::ItemInvOut( Item& item )
{
    if( GUI->Debug )
        App.WriteLogF( _FUNC_, " : item<%u>\n", item.GetId() );

    for( auto it = Elements.begin(), end = Elements.end(); it != end; ++it )
    {
        if( it->first <= 1000000 )
        {
            FOC::Element::ItemSlot* slot = (FOC::Element::ItemSlot*)it->second;

            if( slot->GetItemID() == item.GetId() )
            {
                slot->ClearItem();
                break;
            }
        }
    }
}
