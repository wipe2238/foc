#ifndef __WORLDMAP__
#define __WORLDMAP__

class WM
{
protected:
	uint Width;
	uint Height;
	uint ZoneSize;

	set<uint> Critters;

public:

	float BaseSpeed; // 1.0f = 1px/s
	bool Debug;

public:
	WM();
	~WM();
	void Init();
//
	void Process(int processType, Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer);
	void ProcessInvite(Critter& leader, Item* car, uint encounterDescriptor, int combatMode, uint& mapId, uint16& hexX, uint16& hexY, uint8& dir);
//
	void ProcessMove(Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer);
	void ProcessEnter(Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer);
	void ProcessStartFast(Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer);
	void ProcessStart(Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer);
	void ProcessSetMove(Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer);
	void ProcessStopped(Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer);
	void ProcessNpcIdle(Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer);
	void ProcessKick(Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed, uint& encounterDescriptor, bool& waitForAnswer);
//

protected:
	void MoveGroup(Critter& cr, Item* car, float& curX, float& curY, float& toX, float& toY, float& speed);
	void MoveGroupZone(Critter& cr, const uint16& fromZoneX, const uint16& fromZoneY, const uint16& toZoneX, const uint16& toZoneY);

public:
	inline float GetDistance(const float& fromX, const float& fromY, const float& toX, const float& toY)
	{
		return sqrt(((fromX)-(toX))*((fromX)-(toX)) + ((fromY)-(toY))*((fromY)-(toY)));
	}

	int GetFog(Critter& cr, uint16 zx, uint16 zy);
	void SetFog(Critter& cr, uint16 zx, uint16 zy, uint8 fog);
	void UpdateFog(Critter&cr, const uint16& zoneX, const uint16& zoneY);
	void UpdateFog(const CrVec& crVec, const uint16& zoneX, const uint16& zoneY);
	void UpdateLocations(Critter& cr, const uint16& zoneX, const uint16& zoneY);
	void UpdateLocations(const CrVec& crVec, const uint16& zoneX, const uint16& zoneY);
};

extern WM WorldMap;

#endif // __WORLDMAP__ //
