#include "raid.h"
#include <Windows.h>
#include <cstdio>
#include "game.h"
#include "classes.h"
#include <vector>
#include <algorithm>
#include <fmt/core.h>
#include "guild.h"

bool Group::pickRaider(std::array<Raider, 72>& raiders) noexcept
{
	if (!hasHealer)
	{
		for (Raider& r : raiders)
		{
			if (r.exists && r.group == -1 && Classes::isHealer(r.cls))
			{
				addRaider(&r);
				return true;
			}
		}
	}
	if (!hasBard)
	{
		for (Raider& r : raiders)
		{
			if (r.exists && r.group == -1 && r.cls == Classes::classes::bard)
			{
				addRaider(&r);
				return true;
			}
		}
	}
	if (hasAMDps)
	{
		for (Raider& r : raiders)
		{
			if (r.exists && r.group == -1 && Classes::isMDps(r.cls))
			{
				addRaider(&r);
				return true;
			}
		}
	}
	if (hasMDps)
	{
		for (Raider& r : raiders)
		{
			if (r.exists && r.group == -1 && r.cls != Classes::classes::bard && Classes::isAMDps(r.cls))
			{
				addRaider(&r);
				return true;
			}
		}
	}
	if (hasACDps)
	{
		for (Raider& r : raiders)
		{
			if (r.exists && r.group == -1 && r.cls != Classes::classes::bard && Classes::isCDps(r.cls))
			{
				addRaider(&r);
				return true;
			}
		}
	}
	if (hasCDps)
	{
		for (Raider& r : raiders)
		{
			if (r.exists && r.group == -1 && Classes::isCDps(r.cls))
			{
				addRaider(&r);
				return true;
			}
		}
	}

	for (Raider& r : raiders)
	{
		if (r.exists && r.group == -1 && !Classes::isHealer(r.cls))
		{
			addRaider(&r);
			return true;
		}
	}

	for (Raider& r : raiders)
	{
		if (r.exists && r.group == -1)
		{
			addRaider(&r);
			return true;
		}
	}
	return false;
}

float Group::mergeScore(const Group& o) const noexcept
{
	int size = numRaiders + o.numRaiders;
	if (size > 6) return -1.0f;
	if (size == 6 && !hasHealer && !o.hasHealer) return 0.01f;
	if (hasBard && o.hasBard) return 0.01f;
	return 0.7f;
}

void Group::merge(Group& o) noexcept
{
	for (int i = 0; i < o.numRaiders; ++i)
	{
		addRaider(o.raiders[i]);
		o.raiders[i] = nullptr;
	}
	o.numRaiders = 0;
}

void Group::addRaider(Raider* raider) noexcept
{
	if (full())
	{
		return;
	}

	raiders[numRaiders++] = raider;
	if (raider->group != groupNum)
	{
		raider->group = groupNum;
		raider->changedGroup = true;
	}

	if (Classes::isHealer(raider->cls))
		hasHealer = true;
	if (Classes::isTank(raider->cls))
		hasTank = true;
	if (Classes::isMDps(raider->cls))
		hasMDps = true;
	if (Classes::isCDps(raider->cls))
		hasCDps = true;
	if (Classes::isAMDps(raider->cls))
		hasAMDps = true;
	if (Classes::isACDps(raider->cls))
		hasACDps = true;
	if (raider->cls == Classes::classes::bard)
		hasBard = true;
}

Raid::Raid()
{
	
}

void Raid::init() noexcept
{
	uint64_t base = (uint64_t)GetModuleHandle(NULL);
	eqraiders = (EQRaider*)(base + Offsets::Raid::LIST_ADDR);

	uint64_t winaddr = (base + Offsets::Raid::WINDOW_ADDR);
	window = *((uint64_t*)(winaddr + Offsets::Raid::WINDOW_OFFSET));

	for (int i = 0; i < (int)RaidButton::length; ++i)
	{
		uint64_t buttonOffset = Offsets::Raid::WINDOW_GROUP_BUTTON_BASE_OFFSET + (sizeof(void*) * i);
		uint64_t buttonAddr = *((uint64_t*)(winaddr));
		buttonAddr = *((uint64_t*)(buttonAddr + buttonOffset));
		buttons[i] = buttonAddr;
	}

	selectedRaider = (int*)(base + Offsets::Raid::WINDOW_SELECTED_RAIDER_ADDR);
	//colorArray = (int**)(*(int*)(base + Offsets::Raid::WINDOW_COLOR_ADDR));
	//colorArray = (int**)((int)colorArray + Offsets::Raid::WINDOW_COLOR_BASE_OFFSET);	
	colorArray = (int*)(window + Offsets::Raid::WINDOW_COLOR_OFFSET);

}

const std::array<Raider, Raid::RAID_SIZE>& Raid::read() noexcept
{
	int totalLevel = 0;
	numRaiders = 0;
	for (int i = 0; i < RAID_SIZE; ++i)
	{
		if (eqraiders[i].name[0] == '\0')
		{
			raiders[i].exists = false;
			continue;
		}
		++numRaiders;
		raiders[i].eqraider = &eqraiders[i];
		raiders[i].name = eqraiders[i].name;
		raiders[i].level = eqraiders[i].level;
		raiders[i].cls = eqraiders[i].cls;
		raiders[i].group = eqraiders[i].group;
		raiders[i].afk = eqraiders[i].afk;
		raiders[i].dead = eqraiders[i].dead;
		raiders[i].raidLead = eqraiders[i].raidLead;
		raiders[i].groupLead = eqraiders[i].groupLead;
		raiders[i].assist = eqraiders[i].assist;
		raiders[i].marker = eqraiders[i].marker;
		raiders[i].masterLooter = eqraiders[i].masterLooter;
		raiders[i].exists = true;
		raiders[i].changedGroup = false;
		raiders[i].inZone = eqraiders[i].inZone;

		totalLevel += raiders[i].level;
	}
	if (numRaiders)
		avgLevel = (int)(((float)totalLevel / numRaiders) + 0.5f);
	else
		avgLevel = 0;
	return raiders;
}

void Raid::moveGroupToGroup(int group1, int group2) const noexcept
{
	for (int i=0; i < RAID_SIZE; ++i)
	{
		if (eqraiders[i].name[0] != 0 && eqraiders[i].group == group1)
		{
			*selectedRaider = i;
			clickButton((RaidButton)((int)RaidButton::group1 + group2));
			Sleep(25);
		}
	}
}

void Raid::clickButton(RaidButton button) const noexcept
{
	Game::hookedRaidGroupFunc((void*)window, (uint64_t*)buttons[(int)button], 36, 0);
	Game::hookedRaidGroupFunc((void*)window, (uint64_t*)buttons[(int)button] , 35, 0);
	Game::hookedRaidGroupFunc((void*)window, (uint64_t*)buttons[(int)button], 47, 0);
	Game::hookedRaidGroupFunc((void*)window, (uint64_t*)buttons[(int)button], 2, 0);
	Game::hookedRaidGroupFunc((void*)window, (uint64_t*)buttons[(int)button], 1, 0);
}

bool Raid::raidWindowOpen() const noexcept
{
	uint64_t addr = *(uint64_t*)((uint64_t)(GetModuleHandle(nullptr)) + Offsets::Raid::WINDOW_ADDR);
	return *(bool*)(addr + Offsets::Raid::WINDOW_OPEN_OFFSET);
}

void Raid::removeFromGroup(const char* name) const noexcept
{
	if (setSelectedRaider(name))
	{
		clickButton(RaidButton::nogroup);
	}
}

bool Raid::moveToGroup(const char* name, int group) const noexcept
{
	if (setSelectedRaider(name))
	{
		clickButton((RaidButton)((int)RaidButton::group1 + group));
		return true;
	}
	return false;
}

const char* Raid::myName() const noexcept
{
	uint64_t addr = (uint64_t)GetModuleHandle(nullptr) + Offsets::Raid::MY_NAME_ADDR;
	const char* name = (const char*)addr;
	return name;
}

void Raid::swapRaiders(const EQRaider* r1, const EQRaider* r2) const noexcept
{
	int group1 = r1->group;
	int group2 = r2->group;
	if (group1 == group2) return;
	int size1 = groupSize(group1);
	int size2 = groupSize(group2);

	if (size1 == 6 && size2 == 6)
	{
		removeFromGroup(r1->name);
		Sleep(20);
		moveToGroup(r2->name, group1);
		Sleep(20);
		moveToGroup(r1->name, group2);
	}
	else if (size1 == 6)
	{
		moveToGroup(r1->name, group2);
		moveToGroup(r2->name, group1);
	}
	else if (size2 == 6)
	{
		moveToGroup(r2->name, group1);
		moveToGroup(r1->name, group2);
	}
	else if (size1 == 1 && size2 == 1)
	{
		removeFromGroup(r1->name);
		Sleep(20);
		removeFromGroup(r2->name);
		Sleep(20);
		moveToGroup(r1->name, group2);
		Sleep(20);
		moveToGroup(r2->name, group1);
	}
	else if (size1 == 2)
	{
		if (size2 == 2)
		{
			EQRaider* other = nullptr;
			for (int i = 0; i < RAID_SIZE; ++i)
			{
				if (eqraiders[i].name[0] != 0 && eqraiders[i].group == group2 && strcmp(eqraiders[i].name, r2->name) != 0)
				{
					other = &eqraiders[i];
					break;
				}
			}
			if (other)
			{
				removeFromGroup(other->name);
				Sleep(20);
				removeFromGroup(r2->name);
				Sleep(20);
				moveToGroup(r2->name, group1);
				Sleep(20);
				moveToGroup(r1->name, group2);
				Sleep(20);
				moveToGroup(other->name, group2);
			}
		}
		else
		{
			moveToGroup(r2->name, group1);
			moveToGroup(r1->name, group2);
		}
	}
	else if (size2 == 2)
	{
		moveToGroup(r1->name, group2);
		moveToGroup(r2->name, group1);
	}
	else if (size1 == 1)
	{
		removeFromGroup(r1->name);
		moveToGroup(r2->name, group1);
		moveToGroup(r1->name, group2);
	}
	else if (size2 == 1)
	{
		removeFromGroup(r2->name);
		moveToGroup(r1->name, group2);
		moveToGroup(r2->name, group1);
	}
	else
	{
		moveToGroup(r2->name, group1);
		moveToGroup(r1->name, group2);
	}
}

int Raid::groupSize(int group) const noexcept
{
	int count = 0;
	for (int i = 0; i < RAID_SIZE; ++i)
	{
		if (eqraiders[i].name[0] != '\0' && eqraiders[i].group == group)
		{
			++count;
		}
	}
	return count;
}

int Raid::colorForClass(int cls) const noexcept
{
	//int* addr = (int*)((int)colorArray[Classes::classColorIndex[cls]] + Offsets::Raid::WINDOW_COLOR_OFFSET);
	//int color = *addr; 
	int color = colorArray[Classes::classColorIndex[cls]];
	//fmt::print(Game::logFile, "{} {}\n", color, (uint64_t)colorArray);
	int R = color & 0x000000ff;
	int G = (color & 0x0000ff00) >> 8;
	int B = (color & 0x00ff0000) >> 16;
	int A = (color & 0xff000000) >> 24;
	return (A << 24) | (R << 16) | (G << 8) | B;
}

int Raid::getRaiderIndex(const char* name) const noexcept
{
	for (int i = 0; i < RAID_SIZE; ++i)
	{
		if (eqraiders[i].name[0] == '\x00')
		{
			continue;
		}
		if (strcmp(name, eqraiders[i].name) == 0)
			return i;
	}
	return -1;
}

bool Raid::amIRaidLead() const noexcept
{
	if (eqraiders)
	{
		const char* name = myName();
		int index = getRaiderIndex(name);
		if (index >= 0)
		{
			return eqraiders[index].raidLead;
		}
	}
	return false;
}

bool Raid::setSelectedRaider(const char* name) const noexcept
{
	int index = getRaiderIndex(name);
	if (index < 0) return false;
	*selectedRaider = index;
	return true;
}

const Raider* Raid::getUngroupedRaider() const noexcept
{
	for (const Raider& r : raiders)
	{
		if (r.exists && r.group == -1)
			return &r;
	}
	return nullptr;
}

Group* Raid::getUnfilledGroup(std::array<Group, 12>& groups) const noexcept
{
	int firstEmpty = -1;
	for (int i = 0; i < 12; ++i)
	{
		if (!groups[i].full())
		{
			if (groups[i].numRaiders > 0)
				return &groups[i];
			else if(firstEmpty == -1)
				firstEmpty = i;
		}
	}
	return (firstEmpty == -1) ? nullptr : &groups[firstEmpty];
}

void Raid::makeGroups() noexcept
{
	read();
	int numGroups = (int)ceil(numRaiders / 6.0f);

	std::array<Group, 12> groups = {};
	for (int i = 0; i < 12; ++i)
	{
		groups[i].groupNum = i;
	}

	int inGroups = 0;
	for (Raider& r : raiders)
	{
		if (r.exists && r.group != -1)
		{
			groups[r.group].addRaider(&r);
			++inGroups;
		}
	}

	//mergeGroups(groups, 0.5f);

	while (inGroups < numRaiders)
	{
		Group* g = getUnfilledGroup(groups);
		if (!g) break;

		if (g->pickRaider(raiders))
		{
			inGroups++;
		}
	}

	int i = 0;
	for (const auto& r : raiders)
	{
		if (r.exists && r.changedGroup)
		{
			setSelectedRaider(r.name);
			RaidButton button = (RaidButton)((int)RaidButton::group1 + r.group);
			clickButton(button);
			Sleep(25);
		}
	}
}

void Raid::killGroups() noexcept
{
	read();
	for (const auto& r : raiders)
	{
		if (r.exists)
		{
			setSelectedRaider(r.name);
			clickButton(RaidButton::nogroup);
			Sleep(25);
		}
	}
}

void Raid::mergeGroups(std::array<Group, 12>& groups, float minScore) const noexcept
{
	std::vector<std::tuple<int, int, float>> mergeScores;
	mergeScores.reserve(50);
	for (int i = 0; i < 11; ++i)
	{
		if (groups[i].empty() || groups[i].full()) continue;
		for (int j = i + 1; j < 12; ++j)
		{
			if (groups[j].empty() || groups[j].full()) continue;
			float score = groups[i].mergeScore(groups[j]);
			if (score < minScore) continue;
			mergeScores.emplace_back(i, j, score);
		}
	}
	std::sort(mergeScores.begin(), mergeScores.end(), [](const auto& a, const auto& b) { return std::get<2>(a) < std::get<2>(b); });
	while (!mergeScores.empty())
	{
		const auto& tup = mergeScores[0];
		int g1 = std::get<0>(tup);
		int g2 = std::get<1>(tup);
		groups[g1].merge(groups[g2]);

		mergeScores.erase(std::remove_if(mergeScores.begin(), mergeScores.end(), 
			[g1, g2](const auto& a) { return std::get<0>(a) == g1 || std::get<0>(a) == g2 || std::get<1>(a) == g1 || std::get<1>(a) == g2; }));
	}
}

void Raid::inviteGuild(const std::bitset<17>& classes, int minLevel, bool alts) const noexcept
{
	GuildMember* m = Guild::getHead();
	if (m)
	{
		std::string command;
		do
		{
			if (m->online
				&& classes.test(m->cls)
				&& m->level >= minLevel
				&& (alts || (m->flags & GuildFlags::alt) == GuildFlags::none))
			{
				command = fmt::format("/raidinvite {}", std::string(m->name));
				Game::hookedCommandFunc(0, 0, command.c_str());
			}

			m = m->next;
		} while (m);
	}
}

void Raid::groupAlts() noexcept
{
	auto alts = Guild::getAlts();
	killGroups();
	Sleep(500);

	read();

	std::array<Group, 12> groups = {};
	for (int i = 0; i < 12; ++i)
	{
		groups[i].groupNum = i;
	}

	for (Raider& r : raiders)
	{
		if (!r.exists) continue;
		auto it = alts.find(r.name);
		if(it != alts.end())
		{
			Group* group = nullptr;
			for (int i = 0; i < 12; ++i)
			{
				if (groups[i].empty())
				{
					group = &groups[i];
					break;
				}
			}

			if (!group) break;
			bool movedMain = false;

			for (const auto& alt : it->second)
			{
				if (moveToGroup(alt.c_str(), group->groupNum))
				{
					if (!movedMain)
					{
						moveToGroup(r.name, group->groupNum);
						group->addRaider(&r);
						movedMain = true;
					}
				}
			}
		}
	}
}

void Raid::kickp() noexcept
{
	Game::hookedCommandFunc(0, 0, "/kickp raid");
}