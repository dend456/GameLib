#pragma once
#include <Windows.h>
#include <string>
#include <array>
#include "offsets.h"

class Raid;

#pragma pack(push, 1)
struct EQRaider
{
	const char name[16] = { 0 };
	char unk0[0x70];
	int level = 0;
	int cls = 0;
	char raidLead = 0;
	char groupLead = 0;
	char assist = 0;
	char marker = 0;
	char masterLooter = 0;
	char unk1[0x3];
	int group = -1;
	char afk = 0;
	char dead = 0;
	char inZone = 0;
	char unk4[0x5];
};
#pragma pack(pop)

struct Raider
{
	EQRaider* eqraider = nullptr;
	const char* name = nullptr;
	int level = 0;
	int cls = 0;
	bool raidLead = false;
	bool groupLead = false;
	int group = -1;
	bool afk = false;
	bool assist = false;
	bool marker = false;
	bool masterLooter = false;
	bool exists = false;
	bool changedGroup = false;
	bool dead = false;
	bool inZone = false;
};

struct Group
{
	Raider* raiders[6] = { nullptr };
	int groupNum = 0;
	int numRaiders = 0;
	bool hasHealer = false;
	bool hasTank = false;
	bool hasMDps = false;
	bool hasCDps = false;
	bool hasAMDps = false;
	bool hasACDps = false;
	bool hasBard = false;

	bool full() const noexcept { return numRaiders == 6; }
	bool empty() const noexcept { return numRaiders == 0; }
	void addRaider(Raider* raider) noexcept;
	bool pickRaider(std::array<Raider, 72>& raiders) noexcept;
	float mergeScore(const Group& o) const noexcept;
	void merge(Group& o) noexcept;
};

enum class RaidButton : int
{
	invite,
	disband,
	accept,
	decline,
	makeleader,
	options,
	addlooter,
	removelooter,
	dump,
	masterlooter,
	assist,
	mark,
	findpc,
	lock,
	unlock,
	remleader,
	nogroup,
	group1,
	group2,
	group3,
	group4,
	group5,
	group6,
	group7,
	group8,
	group9,
	group10,
	group11,
	group12,
	invitetask,
	inviteexped,
	length
};

class Raid
{
public:
	static constexpr int RAID_SIZE = 72;

private:
	static constexpr int RAIDER_SIZE = 0x9c;

	EQRaider* eqraiders = nullptr;
	int* selectedRaider = nullptr;
	int* colorArray = nullptr;
	std::array<Raider, RAID_SIZE> raiders;
	int numRaiders = 0;
	int avgLevel = 0;
	uint64_t window = 0;
	std::array<uint64_t, (size_t)RaidButton::length> buttons = {};
	int getRaiderIndex(const char* name) const noexcept;
	const Raider* getUngroupedRaider() const noexcept;
	Group* getUnfilledGroup(std::array<Group, 12>& groups) const noexcept;

public:
	Raid();

	void init() noexcept;
	const std::array<Raider, RAID_SIZE>& read() noexcept;
	bool locked() const noexcept { return *(bool*)((uint64_t)GetModuleHandle(NULL) + Offsets::Raid::WINDOW_LOCKED_OFFSET); }
	void clickButton(RaidButton button) const noexcept;
	bool setSelectedRaider(const char* name) const noexcept;
	void makeGroups() noexcept;
	void killGroups() noexcept;
	void mergeGroups(std::array<Group, 12>& groups, float minScore) const noexcept;
	void moveGroupToGroup(int group1, int group2) const noexcept;
	int colorForClass(int cls) const noexcept;
	void removeFromGroup(const char* name) const noexcept;
	void moveToGroup(const char* name, int group) const noexcept;
	void swapRaiders(const EQRaider* r1, const EQRaider* r2) const noexcept;
	int groupSize(int group) const noexcept;
	int raidSize() const noexcept { return numRaiders; }
	int averageLevel() const noexcept { return avgLevel; }
	bool amIRaidLead() const noexcept;
	const char* myName() const noexcept;
	bool raidWindowOpen() const noexcept;
};