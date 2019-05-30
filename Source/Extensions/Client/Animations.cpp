#include <CritterCl.h>
#include <Log.h>

#include "FOC.h"

#include "Animations.h"

using namespace std;


FOC::AnimationsManager::AnimationsManager()
{}

void FOC::AnimationsManager::ProcessAction( bool localCall, CritterCl& cr, int action, int actionExt, ItemCl* item )
{
    if( cr.IsChosen() && !localCall )
    {
        switch( action )
        {
            case CRITTER_ACTION_MOVE_ITEM:
            case CRITTER_ACTION_MOVE_ITEM_SWAP:
            case CRITTER_ACTION_USE_ITEM:
            case CRITTER_ACTION_DROP_ITEM:
            case CRITTER_ACTION_USE_WEAPON:
            case CRITTER_ACTION_RELOAD_WEAPON:
            case CRITTER_ACTION_USE_SKILL:
            case CRITTER_ACTION_PICK_ITEM:
            case CRITTER_ACTION_PICK_CRITTER:
            case CRITTER_ACTION_OPERATE_CONTAINER:
                return;
            default:
                break;
        }
    }

    const ProtoItem* proto = (item ? item->Proto : cr.ItemSlotMain->Proto);

    switch( action )
    {
        case CRITTER_ACTION_MOVE_ITEM:
        {
            if( cr.Cond == CRITTER_CONDITION_LIFE )
            {
                uint8 fromSlot = actionExt;
                uint8 toSlot = item->AccCritter.Slot;

                cr.ClearAnim();

                if( toSlot == SLOT_HAND1 )
                    cr.Animate( 0, ANIM2_SHOW_WEAPON, item );
                else if( fromSlot == SLOT_HAND1 )
                    cr.Animate( 0, ANIM2_HIDE_WEAPON, item );
                else
                    cr.Animate( 0, ANIM2_SWITCH_ITEMS, nullptr );
            }
            break;
        }
        case CRITTER_ACTION_USE_ITEM:
        {
            if( cr.Cond == CRITTER_CONDITION_LIFE && proto )
            {
                cr.ClearAnim();

                // proto->UserData[53] = proto->Weapon_Anim2[0]
                if( proto->Type == ITEM_TYPE_WEAPON && FLAG( proto->Flags, ITEM_FLAG_CAN_USE_ON_SMTH ) && cr.IsAnimAviable( proto->Weapon_Anim1, proto->UserData[53] ) )
                    cr.Animate( proto->Weapon_Anim1, proto->UserData[53], item ); // proto->Weapon_Anim2[0]
                else
                    cr.Animate( 0, ANIM2_USE, item );
            }
            break;
        }
        // 5-7
        case CRITTER_ACTION_USE_SKILL:
        {
            if( cr.Cond == CRITTER_CONDITION_LIFE && actionExt != SK_SNEAK )
            {
                cr.ClearAnim();
                cr.Animate( 0, ANIM2_USE, nullptr );
            }
            break;
        }
        case CRITTER_ACTION_PICK_ITEM:
        {
            if( cr.Cond == CRITTER_CONDITION_LIFE && proto )
            {
                bool isGround = (proto->Type >= ITEM_TYPE_ARMOR && proto->Type <= ITEM_TYPE_KEY && !item->IsCar() );
                if( proto->Type == ITEM_TYPE_CONTAINER )
                    isGround = proto->GroundLevel;
                cr.ClearAnim();
                cr.Animate( 0, isGround ? ANIM2_PICKUP : ANIM2_USE, nullptr );
            }
            break;
        }
        case CRITTER_ACTION_FIDGET:
        {
            cr.Animate( 0, ANIM2_FIDGET, nullptr );
            break;
        }
    }
}

bool FOC::AnimationsManager::ProcessFallout( uint crType, uint& anim1, uint& anim2, uint& anim1ex, uint& anim2ex, uint& flags )
{
    WriteLog( "ProcessFallout(crType=%u, anim1=%u, anim2=%u, anim1ex=%u, anim2ex=%u, flags=%u)\n",
              crType, anim1, anim2, anim1ex, anim2ex, flags );

    // Still only weapon
    anim1 &= ANIM1_WEAPON_MASK;

    // Weapon
    switch( anim1 )
    {
        case ANIM1_UNARMED:
            anim1 = ANIM1_FALLOUT_UNARMED;
            break;
        case ANIM1_KNIFE:
            anim1 = ANIM1_FALLOUT_KNIFE;
            break;
        case ANIM1_CLUB:
            anim1 = ANIM1_FALLOUT_CLUB;
            break;
        case ANIM1_HAMMER:
            anim1 = ANIM1_FALLOUT_HAMMER;
            break;
        case ANIM1_SPEAR:
            anim1 = ANIM1_FALLOUT_SPEAR;
            break;
        case ANIM1_PISTOL:
            anim1 = ANIM1_FALLOUT_PISTOL;
            break;
        case ANIM1_SMG:
            anim1 = ANIM1_FALLOUT_UZI;
            break;
        case ANIM1_SHOOTGUN:
            anim1 = ANIM1_FALLOUT_SHOOTGUN;
            break;
        case ANIM1_HEAVY_RIFLE:
            anim1 = ANIM1_FALLOUT_RIFLE;
            break;
        case ANIM1_MINIGUN:
            anim1 = ANIM1_FALLOUT_MINIGUN;
            break;
        case ANIM1_ROCKET_LAUNCHER:
            anim1 = ANIM1_FALLOUT_ROCKET_LAUNCHER;
            break;
        case ANIM1_FLAMER:
            anim1 = ANIM1_FALLOUT_RIFLE;
            break;
        case ANIM1_RIFLE:
            anim1 = ANIM1_FALLOUT_RIFLE;
            break;
        case ANIM1_SWORD:
            anim1 = ANIM1_FALLOUT_KNIFE;
            break;
        case ANIM1_LONG_SWORD:
            anim1 = ANIM1_FALLOUT_KNIFE;
            break;
        case ANIM1_AXE:
            anim1 = ANIM1_FALLOUT_CLUB;
            break;
        case ANIM1_BOW:
            anim1 = ANIM1_FALLOUT_UNARMED;
            break;
        default:
            anim1 = ANIM1_FALLOUT_UNARMED;
            WriteLog( "anim1 ??? %u\n", anim1 );
            break;
    }

    // Action
    switch( anim2 )
    {
        case ANIM2_IDLE:
            anim2 = ANIM2_FALLOUT_STAY;
            flags |= ANIM_FLAG_FIRST_FRAME;
            break;
        case ANIM2_IDLE_STUNNED:
            anim2 = ANIM2_FALLOUT_STAY;
            flags |= ANIM_FLAG_FIRST_FRAME;
            break;
        case ANIM2_WALK:
            anim2 = ANIM2_FALLOUT_WALK;
            break;
        case ANIM2_LIMP:
            anim2 = ANIM2_FALLOUT_WALK;
            break;
        case ANIM2_RUN:
            anim1 = ANIM1_FALLOUT_UNARMED;
            anim2 = ANIM2_FALLOUT_RUN;
            break;
        case ANIM2_PANIC_RUN:
            anim1 = ANIM1_FALLOUT_UNARMED;
            anim2 = ANIM2_FALLOUT_RUN;
            break;
        case ANIM2_SHOW_WEAPON:
            anim2 = ANIM2_FALLOUT_SHOW;
            break;
        case ANIM2_HIDE_WEAPON:
            anim2 = ANIM2_FALLOUT_HIDE;
            break;
        case ANIM2_PREPARE_WEAPON:
            anim2 = ANIM2_FALLOUT_PREPARE_WEAPON;
            break;
        case ANIM2_TURNOFF_WEAPON:
            anim2 = ANIM2_FALLOUT_TURNOFF_WEAPON;
            break;
        case ANIM2_FIDGET:
            anim2 = ANIM2_FALLOUT_STAY;
            break;
        case ANIM2_CLIMBING:
            return true;
        case ANIM2_PICKUP:
            anim1 = ANIM1_FALLOUT_UNARMED;
            anim2 = ANIM2_FALLOUT_PICKUP;
            break;
        case ANIM2_USE:
            anim1 = ANIM1_FALLOUT_UNARMED;
            anim2 = ANIM2_FALLOUT_USE;
            break;
        case ANIM2_SWITCH_ITEMS:
            anim1 = ANIM1_FALLOUT_UNARMED;
            anim2 = ANIM2_FALLOUT_USE;
            break;
        case ANIM2_LOOT:
            anim1 = ANIM1_FALLOUT_UNARMED;
            anim2 = ANIM2_FALLOUT_PICKUP;
            break;
        case ANIM2_STEAL:
            anim1 = ANIM1_FALLOUT_UNARMED;
            anim2 = ANIM2_FALLOUT_USE;
            break;
        case ANIM2_PUSH:
            anim1 = ANIM1_FALLOUT_UNARMED;
            anim2 = ANIM2_FALLOUT_PUNCH;
            break;
        case ANIM2_IDLE_COMBAT:
            anim2 = ANIM2_FALLOUT_STAY;
            flags |= ANIM_FLAG_FIRST_FRAME;
            break;
        case ANIM2_PUNCH_RIGHT:
            anim1 = ANIM1_FALLOUT_UNARMED;
            anim2 = ANIM2_FALLOUT_PUNCH;
            break;
        case ANIM2_KICK_HI:
            anim1 = ANIM1_FALLOUT_UNARMED;
            anim2 = ANIM2_FALLOUT_KICK;
            break;
        case ANIM2_THRUST_1H:
            anim2 = ANIM2_FALLOUT_THRUST;
            break;
        case ANIM2_THRUST_2H:
            anim2 = ANIM2_FALLOUT_THRUST;
            break;
        case ANIM2_SWING_1H:
            anim2 = ANIM2_FALLOUT_SWING;
            break;
        case ANIM2_SWING_2H:
            anim2 = ANIM2_FALLOUT_SWING;
            break;
        case ANIM2_SINGLE:
            anim2 = ANIM2_FALLOUT_SHOOT;
            break;
        case ANIM2_BURST:
            anim2 = ANIM2_FALLOUT_BURST;
            break;
        case ANIM2_SWEEP:
            anim2 = ANIM2_FALLOUT_BURST;
            break;
        case ANIM2_FLAME:
            anim2 = ANIM2_FALLOUT_FLAME;
            break;
        case ANIM2_BUTT:
            anim1 = ANIM1_FALLOUT_UNARMED;
            anim2 = ANIM2_FALLOUT_PUNCH;
            break;
        case ANIM2_THROW:
            anim2 = (anim1 == ANIM1_FALLOUT_UNARMED ? ANIM2_FALLOUT_THROW_EMPTY : ANIM2_FALLOUT_THROW_WEAPON);
            break;
        case ANIM2_DODGE_FRONT:
            anim2 = (anim1 == ANIM1_FALLOUT_UNARMED ? ANIM2_FALLOUT_DODGE_EMPTY : ANIM2_FALLOUT_DODGE_WEAPON);
            break;
        case ANIM2_DODGE_BACK:
            anim2 = (anim1 == ANIM1_FALLOUT_UNARMED ? ANIM2_FALLOUT_DODGE_EMPTY : ANIM2_FALLOUT_DODGE_WEAPON);
            break;
        case ANIM2_DAMAGE_FRONT:
            anim2 = ANIM2_FALLOUT_DAMAGE_FRONT;
            break;
        case ANIM2_DAMAGE_BACK:
            anim2 = ANIM2_FALLOUT_DAMAGE_BACK;
            break;
        case ANIM2_DAMAGE_MUL_FRONT:
            anim2 = ANIM2_FALLOUT_DAMAGE_FRONT;
            break;
        case ANIM2_DAMAGE_MUL_BACK:
            anim2 = ANIM2_FALLOUT_DAMAGE_BACK;
            break;
        case ANIM2_KNOCK_FRONT:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_KNOCK_FRONT;
            break;
        case ANIM2_KNOCK_BACK:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_KNOCK_BACK;
            break;
        case ANIM2_STANDUP_FRONT:
            anim1 = ANIM1_FALLOUT_KNOCKOUT;
            anim2 = ANIM2_FALLOUT_STANDUP_FRONT;
            break;
        case ANIM2_STANDUP_BACK:
            anim1 = ANIM1_FALLOUT_KNOCKOUT;
            anim2 = ANIM2_FALLOUT_STANDUP_BACK;
            break;
        case ANIM2_DEAD_PRONE_FRONT:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_DEAD_FRONT2;
            break;
        case ANIM2_DEAD_PRONE_BACK:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_DEAD_BACK2;
            break;
        case ANIM2_DEAD_BLOODY_SINGLE:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_DEAD_BLOODY_SINGLE;
            break;
        case ANIM2_DEAD_BLOODY_BURST:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_DEAD_BLOODY_BURST;
            break;
        case ANIM2_DEAD_BURST:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_DEAD_BURST;
            break;
        case ANIM2_DEAD_PULSE:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_DEAD_PULSE;
            break;
        case ANIM2_DEAD_PULSE_DUST:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_DEAD_PULSE_DUST;
            break;
        case ANIM2_DEAD_LASER:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_DEAD_LASER;
            break;
        case ANIM2_DEAD_FUSED:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_DEAD_FUSED;
            break;
        case ANIM2_DEAD_EXPLODE:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_DEAD_EXPLODE;
            break;
        case ANIM2_IDLE_PRONE_FRONT:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_KNOCK_FRONT;
            flags |= ANIM_FLAG_LAST_FRAME;
            break;
        case ANIM2_IDLE_PRONE_BACK:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_KNOCK_BACK;
            flags |= ANIM_FLAG_LAST_FRAME;
            break;
        case ANIM2_DEAD_BURN:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_DEAD_BURN;
            break;                                                                                                                          // ANIM2_FALLOUT_DEAD_BURN2
        case ANIM2_DEAD_BURN_RUN:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim1ex = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_DEAD_BURN_RUN;
            anim2ex = ANIM2_FALLOUT_DEAD_BURN;
            break;
        case ANIM2_DEAD_FRONT:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim1ex = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_KNOCK_FRONT;
            anim2ex = ANIM2_FALLOUT_DEAD_FRONT2;
            break;
        case ANIM2_DEAD_BACK:
            anim1 = ANIM1_FALLOUT_DEAD;
            anim1ex = ANIM1_FALLOUT_DEAD;
            anim2 = ANIM2_FALLOUT_KNOCK_BACK;
            anim2ex = ANIM2_FALLOUT_DEAD_BACK2;
            break;
        case ANIM2_NO_RECOIL:
        case ANIM2_RELOAD:
        case ANIM2_REPAIR:
            anim1ex = anim1;
            anim2 = ANIM2_FALLOUT_PREPARE_WEAPON;
            anim2ex = ANIM2_FALLOUT_TURNOFF_WEAPON;
            break;

        // ANIM2_BEGIN_COMBAT
        // ANIM2_END_COMBAT
        // ANIM2_PUNCH_LEFT
        // ANIM2_PUNCH_COMBO
        // ANIM2_KICK_LO
        // ANIM2_KICK_COMBO
        // ANIM2_WALK_DAMAGE_FRONT
        // ANIM2_WALK_DAMAGE_BACK
        // ANIM2_LIMP_DAMAGE_FRONT
        // ANIM2_LIMP_DAMAGE_BACK
        // ANIM2_RUN_DAMAGE_FRONT
        // ANIM2_RUN_DAMAGE_BACK
        // ANIM2_LAYDOWN_FRONT
        // ANIM2_LAYDOWN_BACK
        // ANIM2_DAMAGE_PRONE_FRONT
        // ANIM2_DAMAGE_PRONE_BACK
        // ANIM2_DAMAGE_MUL_PRONE_FRONT
        // ANIM2_DAMAGE_MUL_PRONE_BACK
        // ANIM2_TWITCH_PRONE_FRONT
        // ANIM2_TWITCH_PRONE_BACK
        default:
            WriteLog( "anim2 ??? %u\n", anim2 );
            return false;
    }

    return true;
}
