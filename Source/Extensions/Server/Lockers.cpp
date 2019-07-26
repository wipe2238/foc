#include <Critter.h>
#include <CritterManager.h>
#include <Defines.Public.h>
#include <GameOptions.h>
#include <ItemManager.h>
#include <Log.h>
#include <Map.h>
#include <MapManager.h>
#include <Script.h>

#include "FOC.Server.h"
#include "Server/Lockers.h"

#include "Shared/Buffer.h"
#include "Shared/Buffer.Array.h"
#include "Shared/SScriptFunc.h"
#include "Shared/TimeCalc.h"

using namespace std;

FOC::LockersManager::LockersManager() :
// public
    Debug( false ),
    AutoCloseTime( 0 )
{}

FOC::LockersManager::~LockersManager()
{}

//

static uint LockersOnOpenClose( ScriptArray* values )
{
    return FOC::Self()->Lockers->OnOpenClose( values );
}

void FOC::LockersManager::Init()
{
    FOC::Self()->SetFunctionAddress( "Lockers/OpenClose", (size_t)&LockersOnOpenClose );
}

//

bool FOC::LockersManager::UseLocker( Critter* cr, Item* locker, int skill )
{
    if( Debug )
        WriteLogF( _FUNC_, "(cr=%u, locker=%u, skill=%d)\n", cr ? cr->GetId() : 0, locker ? locker->GetId() : 0, skill );

    if( !cr )
    {
        WriteLogF( _FUNC_, " ERROR : critter nullptr\n" );
        return false;
    }
    else if( !locker )
    {
        WriteLogF( _FUNC_, " ERROR : locker nullptr : cr<%u> map<%u> hx<%u> hy<%u>\n", cr->GetId(), cr->GetMap(), cr->GetHexX(), cr->GetHexY() );
        return false;
    }
    else if( cr->IsNotValid || locker->IsNotValid )
    {
        WriteLogF( _FUNC_, "ERROR : %s not valid\n", cr ? "locker" : "critter" );
        return false;
    }
    else if( skill != SKILL_PICK_ON_GROUND )
        return false;

    bool        open = false;
    int         transferType = -1;

    std::string lockerType;

    if( locker->IsDoor() )
    {
        if( !locker->Proto->Container_Changeble )
        {
            WriteLogF( _FUNC_, " WARNING : item<%u> pid<%u> critter<%u> : doors not changeable\n", locker->GetId(), locker->Proto->ProtoId, cr->GetId() );

            // TODO STR_USE_NOTHING
            return true;
        }

        open = locker->LockerIsClose();
    }
    else if( locker->IsContainer() )
    {
        open = locker->LockerIsClose();
        transferType = locker->Proto->GroundLevel ? TRANSFER_HEX_CONT_DOWN : TRANSFER_HEX_CONT_UP;
    }

    uint delay = REAL_SECOND( 1 ), wait = 0;

    if( cr->ItemSlotMain && cr->ItemSlotMain->GetType() == ITEM_TYPE_WEAPON )
    {
        delay = REAL_MS( GameOpt.Breaktime * 2 );

        cr->SendAA_Animate( 0, 21, cr->ItemSlotMain, true, false );       // ANIM2_HIDE_WEAPON
        cr->SendAA_Animate( 0, 28, cr->ItemSlotMain, false, true );       // ANIM2_USE
        cr->SendAA_Animate( 0, 20, cr->ItemSlotMain, false, true );       // ANIM2_SHOW_WEAPON

        wait = GameOpt.Breaktime * 3;
    }
    else
    {
        delay = REAL_MS( GameOpt.Breaktime );

        cr->SendAA_Animate( 0, 28, cr->ItemSlotMain, true, false );       // ANIM2_USE

        wait = GameOpt.Breaktime;
    }

    if( wait )
        FOServer::SScriptFunc::Crit_Wait( cr, wait );

    if( Debug )
        WriteLogF( _FUNC_, " : item<%u> pid<%u> critter<%u> show<%s> %s %s\n", locker->GetId(), locker->GetProtoId(), cr->GetId(), transferType >= 0 ? "true" : "false", open ? "open" : "close", locker->IsDoor() ? "door" : "container" );

    if( open )
        Open( locker, cr, transferType, delay );
    else
        Close( locker, cr, delay );

    return true;
}

void FOC::LockersManager::Open( Item* locker, Critter* cr /* = nullptr */, int transferType /* = -1 */, uint delay /* = 0 */ )
{
    BufferArray buff( "array<int>" );
    int8        action = locker->LockerIsChangeble() ? 1 : 0;

    buff << locker->GetId();
    buff << action;
    buff << (cr && !cr->IsNotValid ? cr->GetId() : 0);
    buff << transferType;

    FOServer::SScriptFunc::Global_CreateTimeEventValues( GameOpt.FullSecond + delay, ScriptString( FOC::Self()->GetFunctionName( (size_t)&LockersOnOpenClose ) ), *buff.GetArray(), true );
}

void FOC::LockersManager::Close( Item* locker, Critter* cr /* = nullptr */, uint delay /* = 0 */ )
{
    BufferArray buff( "array<int>" );
    int8        action = locker->LockerIsChangeble() ? -1 : 0;

    buff << locker->GetId();
    buff << action;
    buff << (cr && !cr->IsNotValid ? cr->GetId() : 0);
    buff << (int)-1;     // dummy transferType

    FOServer::SScriptFunc::Global_CreateTimeEventValues( GameOpt.FullSecond + delay, ScriptString( FOC::Self()->GetFunctionName( (size_t)&LockersOnOpenClose ) ), *buff.GetArray(), true );
}

uint FOC::LockersManager::OnOpenClose( ScriptArray* values )
{
    BufferArray buff( values );

    uint        lockerId = 0, crId = 0;
    int8        action = 0; // -1 close, 1 open
    int         transferType = -1;

    buff >> lockerId >> action >> crId >> transferType;

    Item* locker = ItemMngr.GetItem( lockerId, true );
    if( !locker )
        return 0;

    Critter* cr = nullptr;
    if( crId )
        cr = CrMngr.GetCritter( crId, true );

    if( action > 0 )
        OnOpen( locker, cr );
    else if( action < 0 )
        OnClose( locker, cr );

    if( cr && !cr->IsNotValid && transferType >= 0 )
        OnShowContainer( locker, cr, transferType );

    return 0;
}

// TODO should be global util
// little more strict version of SScriptFunc::Item_GetMapPosition()
Map* FOC::LockersManager::GetMapHexOf( Item* locker, uint16& hexX, uint16& hexY )
{
    if( !locker )
    {
        WriteLogF( _FUNC_, " ERROR : locker nullptr\n" );
        return nullptr;
    }

    switch( locker->Accessory )
    {
        case ITEM_ACCESSORY_CRITTER:
        {
            Critter* cr = CrMngr.GetCritter( locker->AccCritter.Id, true );
            if( cr && !cr->IsNotValid && cr->GetMap() )
            {
                Map* crMap = MapMngr.GetMap( cr->GetMap(), true );
                if( crMap && !crMap->IsNotValid )
                {
                    hexX = cr->GetHexX();
                    hexY = cr->GetHexY();
                    return crMap;
                }
            }

            break;
        }
        case ITEM_ACCESSORY_HEX:
        {
            Map* hexMap = MapMngr.GetMap( locker->AccHex.MapId, true );
            if( hexMap && !hexMap->IsNotValid )
            {
                hexX = locker->AccHex.HexX;
                hexY = locker->AccHex.HexY;
                return hexMap;
            }

            break;
        }
        case ITEM_ACCESSORY_CONTAINER:
        {
            if( locker->AccContainer.ContainerId != locker->GetId() )
            {
                Item* parent = ItemMngr.GetItem( locker->AccContainer.ContainerId, false );
                if( parent && !parent->IsNotValid )
                    return GetMapHexOf( locker, hexX, hexY );
            }

            break;
        }
    }

    return nullptr;
}

void FOC::LockersManager::OnOpen( Item* locker, Critter* cr /* = nullptr */ )
{
    FOServer::SScriptFunc::Item_LockerOpen( locker );

    uint16 hexX = 0, hexY = 0;
    Map*   map = GetMapHexOf( locker, hexX, hexY );
    if( map )
    {
        ScriptString sound = locker->GetType() == ITEM_TYPE_DOOR ? "SODOORSA.ACM" : "IOCNTNRA.ACM";
        sound.rawSet( 7, locker->Proto->SoundId );
        FOServer::SScriptFunc::Map_PlaySoundRadius( map, sound, hexX, hexY, 10 );
    }
}

void FOC::LockersManager::OnClose( Item* locker, Critter* cr /* = nullptr */ )
{
    FOServer::SScriptFunc::Item_LockerClose( locker );

    uint16 hexX = 0, hexY = 0;
    Map*   map = GetMapHexOf( locker, hexX, hexY );
    if( map )
    {
        ScriptString sound = locker->GetType() == ITEM_TYPE_DOOR ? "SCDOORSA.ACM" : "ICCNTNRA.ACM";
        sound.rawSet( 7, locker->Proto->SoundId );
        FOServer::SScriptFunc::Map_PlaySoundRadius( map, sound, hexX, hexY, 10 );
    }
}

void FOC::LockersManager::OnShowContainer( Item* locker, Critter* cr, uint8 transferType )
{
    FOServer::SScriptFunc::Cl_ShowContainer( cr, nullptr, locker, transferType );
}
