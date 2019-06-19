#ifndef __CLIENT__SCREEN__INVENTORY__
#define __CLIENT__SCREEN__INVENTORY__

#include <Item.h>

#include "PGUI/PGUI.Screen.h"

struct AnyFrames;

namespace FOC
{
    namespace Element
    {
        class ItemSlot : public PGUI::Element
        {
private:
            uint       ItemID;
            AnyFrames* Sprite;

public:
            ItemSlot( PGUI::Core* gui );
            virtual ~ItemSlot();

            void ClearItem();
            bool IsItemSet();
            uint GetItemID();
            void SetItem( Item& item );

            virtual void DrawContent() override;
        };
    };

    namespace Screen
    {
        class Inventory : public PGUI::Screen
        {
private:
            uint SlotSize;
            uint SlotCount;

public:
            Inventory( PGUI::Core* gui );
            virtual ~Inventory();

            void RefreshSlots();
            uint GetSlotSize();
            uint GetSlotColumns();
            uint GetSlotRows();
            void SetSlotSize( uint size );

public:
            void ItemInvIn( Item& item );
            void ItemInvOut( Item& item );
        };
    };
};

#endif // __CLIENT__SCREEN__INVENTORY__ //
