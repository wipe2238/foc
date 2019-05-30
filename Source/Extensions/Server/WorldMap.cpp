#include <cassert>

#include <Critter.h>
#include <GameOptions.h>
#include <Item.h>
#include <Log.h>
#include <MapManager.h>

#include "FOC.h"
#include "Server/WorldMap.h"
#include "Shared/SScriptFunc.h"

FOC::WorldMapManager::WorldMapManager() :
    Width( 0 ),
    Height( 0 ),
    ZoneSize( 0 ),
    Debug( false ),
    BaseSpeed( 1.0f )
{}

FOC::WorldMapManager::~WorldMapManager()
{
    Critters.clear();
}

void FOC::WorldMapManager::Init()
{
    Width = GAME_OPTION( GlobalMapWidth ) * GAME_OPTION( GlobalMapZoneLength );
    Height = GAME_OPTION( GlobalMapHeight ) * GAME_OPTION( GlobalMapZoneLength );
    ZoneSize = GAME_OPTION( GlobalMapZoneLength );
}

//

void FOC::WorldMapManager::Process( int processType, Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer )
{
    assert( processType >= 0 && processType <= 7 );

    static const char* processName[] =
    {
        "WORLDMAP_PROCESS_MOVE",
        "WORLDMAP_PROCESS_ENTER",
        "WORLDMAP_PROCESS_START_FAST",
        "WORLDMAP_PROCESS_START",
        "WORLDMAP_PROCESS_SET_MOVE",
        "WORLDMAP_PROCESS_STOPPED",
        "WORLDMAP_PROCESS_NPC_IDLE",
        "WORLDMAP_PROCESS_KICK"
    };

    if( Debug )
        WriteLogF( _FUNC_, "<%s> critter<%u>\n", processName[processType], cr.GetId() );

    switch( processType )
    {
        case WORLDMAP_PROCESS_MOVE:
        {
            ProcessMove( cr, car, curX, curY, toX, toY, speed, encounterDescriptor, waitForAnswer );
            break;
        }
        case WORLDMAP_PROCESS_ENTER:
        {
            ProcessEnter( cr, car, curX, curY, toX, toY, speed, encounterDescriptor, waitForAnswer );
            break;
        }
        case WORLDMAP_PROCESS_START_FAST:
        {
            ProcessStartFast( cr, car, curX, curY, toX, toY, speed, encounterDescriptor, waitForAnswer );
            break;
        }
        case WORLDMAP_PROCESS_START:
        {
            ProcessStart( cr, car, curX, curY, toX, toY, speed, encounterDescriptor, waitForAnswer );
            break;
        }
        case WORLDMAP_PROCESS_SET_MOVE:
        {
            ProcessSetMove( cr, car, curX, curY, toX, toY, speed, encounterDescriptor, waitForAnswer );
            break;
        }
        case WORLDMAP_PROCESS_STOPPED:
        {
            ProcessStopped( cr, car, curX, curY, toX, toY, speed, encounterDescriptor, waitForAnswer );
            break;
        }
        case WORLDMAP_PROCESS_NPC_IDLE:
        {
            ProcessNpcIdle( cr, car, curX, curY, toX, toY, speed, encounterDescriptor, waitForAnswer );
            break;
        }
        case WORLDMAP_PROCESS_KICK:
        {
            ProcessKick( cr, car, curX, curY, toX, toY, speed, encounterDescriptor, waitForAnswer );
            break;
        }
    }
}

void FOC::WorldMapManager::ProcessInvite( Critter& crLeader, Item* car, uint encounterDescriptor, int combatMode, uint& mapId, uint16& hexX, uint16& hexY, uint8& dir )
{
    if( Debug )
        WriteLogF( _FUNC_, " critter<%u>\n", crLeader.GetId() );
}

//

void FOC::WorldMapManager::ProcessMove( Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer )
{
    MoveGroup( cr, car, curX, curY, toX, toY, speed );
}

void FOC::WorldMapManager::ProcessEnter( Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer )
{}

void FOC::WorldMapManager::ProcessStartFast( Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer )
{
    int zoneX = (int)curX / ZoneSize;
    int zoneY = (int)curY / ZoneSize;

    if( Debug )
        WriteLogF( _FUNC_, " critter<%u> zone<%d:%d>\n", cr.GetId(), zoneX, zoneY );

    UpdateFog( cr, zoneX, zoneY ); // << CRASH
}

void FOC::WorldMapManager::ProcessStart( Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer )
{}

void FOC::WorldMapManager::ProcessSetMove( Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer )
{
    speed = BaseSpeed;
    MoveGroup( cr, car, curX, curY, toX, toY, speed );
}

void FOC::WorldMapManager::ProcessStopped( Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer )
{}

void FOC::WorldMapManager::ProcessNpcIdle( Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer )
{}

void FOC::WorldMapManager::ProcessKick( Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer )
{}

//

void FOC::WorldMapManager::MoveGroup( Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed )
{
    const float oldDist = GetDistance( curX, curY, toX, toY );
    const uint  movementType = (car ? car->Proto->Car_MovementType : WORLDMAP_WALK_GROUND);

    const float speedPix = speed * float(GAME_OPTION( GlobalMapMoveTime ) ) / 1000.0f;
    const float speedAngle = atan2( toY - curY, toX - curX );
    const float speedX = cos( speedAngle ) * speedPix;
    const float speedY = sin( speedAngle ) * speedPix;

    float       speedMod = 1.0f;

    /*
       if( movementType == WORLDMAP_WALK_GROUND )
       {
            // Speed modifiers
            //  F   E   D   C   B   A   9    8   7    6   5    4   3    2   1    0
            // 1.5 1.4 1.3 1.2 1.1 1.0 0.95 0.9 0.85 0.8 0.75 0.7 0.65 0.6 0.55 0.0
            switch( GetGlobalMapRelief( curXi, curYi ))
            {
                    case 0x0: speedMod = 1.00f; break;
                    case 0x1: speedMod = 0.55f; break;
                    case 0x2: speedMod = 0.60f; break;
                    case 0x3: speedMod = 0.65f; break;
                    case 0x4: speedMod = 0.70f; break;
                    case 0x5: speedMod = 0.75f; break;
                    case 0x6: speedMod = 0.80f; break;
                    case 0x7: speedMod = 0.85f; break;
                    case 0x8: speedMod = 0.90f; break;
                    case 0x9: speedMod = 0.95f; break;
                    case 0xA: speedMod = 1.00f; break;
                    case 0xB: speedMod = 1.10f; break;
                    case 0xC: speedMod = 1.20f; break;
                    case 0xD: speedMod = 1.30f; break;
                    case 0xE: speedMod = 1.40f; break;
                    case 0xF: speedMod = 1.50f; break;
                    default: break;
            }

            if( speedMod < 1.0f && cr.Params[PE_EXPLORER] != 0 )
                    speedMod = speedMod / 2.0f + 0.5f;
       }
     */

    //

    int oldXi = (int)curX, oldYi = (int)curY;

    curX += speedX * speedMod;
    curY += speedY * speedMod;

    int curXi = (int)curX, curYi = (int)curY;

    if( Debug )
        WriteLogF( _FUNC_, " critter<%u> move<%d,%d -> %d,%d>\n", cr.GetId(), oldXi, oldYi, curXi, curYi );
    //

    if( oldXi != curXi || oldYi != curYi )
    {
        if( curXi < 0 || curYi < 0 || curXi > (int)Width || curYi > (int)Height )
        {
            if( curXi < 0 )
                curXi = 0;
            if( curXi >= (int)Width )
                curXi = Width - 1;
            if( curYi < 0 )
                curYi = 0;
            if( curYi >= (int)Height )
                curYi = Height - 1;

            curX = (float)curXi;
            curY = (float)curYi;
            speed = 0.0f;

            return;
        }

        int steps = std::max( abs( curXi - oldXi ), abs( curYi - oldYi ) );
        int newXi = oldXi, newYi = oldYi;
        if( steps )
        {
            float oldXf =  (float)oldXi, oldYf =  (float)oldYi;
            float ox = (float)(curXi - oldXi) / (float)steps;
            float oy = (float)(curYi - oldYi) / (float)steps;

            for( int i = 0; i < steps; i++ )
            {
                oldXf += ox;
                oldYf += oy;
                int xi = (int)(oldXf >= 0.0f ? oldXf + 0.5f : oldXf - 0.5f);
                int yi = (int)(oldYf >= 0.0f ? oldYf + 0.5f : oldYf - 0.5f);

                // TODO
                // uint relief_ = GetGlobalMapRelief(xxi, yyi);
                // if (movementType == WORLDMAP_WALK_GROUND && relief != 0 && relief_ == 0) break;
                // if (movementType == WORLDMAP_WALK_WATER && relief_ != 0) break;

                newXi = xi;
                newYi = yi;
            }
        }

        //
        if( newXi != curXi || newYi != curYi )
        {
            curX = (float)newXi;
            curY = (float)newYi;
            speed = 0.0f;
            return;
        }

        int oldZoneX = oldXi / ZoneSize;
        int oldZoneY = oldYi / ZoneSize;
        int curZoneX = curXi / ZoneSize;
        int curZoneY = curYi / ZoneSize;

        assert( oldZoneX >= 0 && oldZoneX <= MAX_UINT16 );
        assert( oldZoneY >= 0 && oldZoneY <= MAX_UINT16 );
        assert( curZoneX >= 0 && curZoneX <= MAX_UINT16 );
        assert( curZoneY >= 0 && curZoneY <= MAX_UINT16 );

        if( oldZoneX != curZoneX || oldZoneY != curZoneY )
            MoveGroupZone( cr, oldZoneX, oldZoneY, curZoneX, curZoneY );

        //
        const float curDist = GetDistance( curX, curY, toX, toY );
        if( curDist <= 0.01f || curDist > oldDist )
        {
            curX = toX;
            curY = toY;
            speed = 0.0f;
            return;
        }
    }
}

void FOC::WorldMapManager::MoveGroupZone( Critter& cr, const uint16& fromZoneX, const uint16& fromZoneY, const uint16& toZoneX, const uint16& toZoneY )
{
    if( Debug )
        WriteLog( _FUNC_, " critter<%u> zone<%u:%u> -> zone<%u:%u>\n", cr.GetId(), fromZoneX, fromZoneY, toZoneX, toZoneY );

    UpdateFog( cr.GroupMove->CritMove, toZoneX, toZoneY );
    UpdateLocations( cr.GroupMove->CritMove, toZoneX, toZoneY );
}

//

void FOC::WorldMapManager::UpdateFog( Critter& cr, const uint16& zoneX, const uint16& zoneY )
{
    if( cr.IsNotValid || cr.IsNpc() )
        return;

    static const char* fogName[] =
    {
        "WORLDMAP_FOG_FULL",
        "WORLDMAP_FOG_HALF",
        "WORLDMAP_FOG_HALF_EX",
        "WORLDMAP_FOG_NONE"
    };

    const uint8 look = 1;

    for( int x = -look; x <= look; x++ )
    {
        for( int y = -look; y <= look; y++ )
        {
            int lookZoneX = zoneX + x, lookZoneY = zoneY + y;

            if( lookZoneX >= 0 && (uint)lookZoneX < Width && lookZoneY >= 0 && (uint)lookZoneY < Height )
            {
                uint8 oldFog = FOServer::SScriptFunc::Cl_GetFog( &cr, lookZoneX, lookZoneY );
                uint8 newFog = ( (zoneX == lookZoneX && zoneY == lookZoneY) ? WORLDMAP_FOG_NONE : WORLDMAP_FOG_HALF );
                if( oldFog < 0 || oldFog > 3 )
                    WriteLogF( _FUNC_, " critter<%u> fog<ERROR>\n", cr.GetId() );
                else if( oldFog < newFog )
                {
                    if( Debug )
                        WriteLogF( _FUNC_, " critter<%u> fog<%s -> %s>\n", cr.GetId(), fogName[oldFog], fogName[newFog] );

                    FOServer::SScriptFunc::Cl_SetFog( &cr, lookZoneX, lookZoneY, newFog );
                }
            }
        }
    }
}

void FOC::WorldMapManager::UpdateFog( const CrVec& crVec, const uint16& zoneX, const uint16& zoneY )
{
    for( auto it = crVec.begin(); it != crVec.end(); ++it )
    {
        Critter* cr = *it;

        if( !cr || cr->IsNotValid || cr->IsNpc() )
            continue;

        UpdateFog( *cr, zoneX, zoneY );
    }
}

void FOC::WorldMapManager::UpdateLocations( Critter& cr, const uint16& zoneX, const uint16& zoneY )
{
    if( cr.IsNotValid || cr.IsNpc() )
        return;

    // SScriptFunc not used to avoid pointless array conversion
    UIntVec loc_ids;
    MapMngr.GetZoneLocations( zoneX, zoneY, 1, loc_ids );

    for( auto it = loc_ids.begin(), end = loc_ids.end(); it != end; ++it )
    {
        if( !FOServer::SScriptFunc::Cl_IsKnownLoc( &cr, true, *it ) )
            FOServer::SScriptFunc::Cl_SetKnownLoc( &cr, true, *it );
    }
}

void FOC::WorldMapManager::UpdateLocations( const CrVec& crVec, const uint16& zoneX, const uint16& zoneY )
{
    for( auto it = crVec.begin(), end = crVec.end(); it != end; ++it )
    {
        Critter* cr = *it;

        if( !cr || cr->IsNotValid || cr->IsNpc() )
            continue;

        UpdateLocations( *cr, zoneX, zoneY );
    }
}
