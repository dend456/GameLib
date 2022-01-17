#pragma once
#include <string>
#include <bitset>
#include <array>

namespace Classes
{
	enum classes : int
	{
		unk0,
		warrior,
		cleric,
		paladin,
		ranger,
		shadowknight,
		druid, 
		monk,
		bard,
		rogue,
		shaman,
		necromancer,
		wizard,
		magician,
		enchanter,
		beastlord,
		berserker
	};

	const inline std::string classStrings[] =
	{
		"Unknown0",
		"Warrior",
		"Cleric",
		"Paladin",
		"Ranger",
		"Shadow Knight",
		"Druid",
		"Monk",
		"Bard",
		"Rogue",
		"Shaman",
		"Necromancer",
		"Wizard",
		"Magician",
		"Enchanter",
		"Beastlord",
		"Berserker",
		"Unknown17",
		"Unknown18",
		"Unknown19",
		"Warrior GM",
		"Cleric GM",
		"Paladin GM",
		"Ranger GM",
		"ShadowKnight GM",
		"Druid GM",
		"Monk GM",
		"Bard GM",
		"Rogue GM",
		"Shaman GM",
		"Necromancer GM",
		"Wizard GM",
		"Magician GM",
		"Enchanter GM",
		"Beastlord GM",
		"Berserker GM",
		"Unknown36",
		"Unknown37",
		"Unknown38",
		"Unknown39",
		"Banker",
		"Shopkeeper",
		"Unknown42",
		"Unknown43",
		"Unknown44",
		"Unknown45",
		"Unknown46",
		"Unknown47",
		"Unknown48",
		"Unknown49",
		"Unknown50",
		"Unknown51",
		"Unknown52",
		"Unknown53",
		"Unknown54",
		"Unknown55",
		"Unknown56",
		"Unknown57",
		"Unknown58",
		"Unknown59",
		"LDoN Recruiter",
		"LDoN Merchant",
		"LDoN Object",
		"Tribute Master",
		"Guild Tribute Master",
		"Unknown65",
		"Guild Banker",
		"DoN Merchant(Norrath's Keepers)",
		"DoN Merchant(Dark Reign)",
		"Unknown69",
		"Unknown70",
		"Mercenary Liaison",
		"Unknown71",
		"Loyalty Liaison",
	};

	const inline std::string classShortStrings[] =
	{
		"Unknown0",
		"War",
		"Clr",
		"Pal",
		"Rng",
		"SK",
		"Dru",
		"Monk",
		"Bard",
		"Rog",
		"Sham",
		"Nec",
		"Wiz",
		"Mag",
		"Ench",
		"Bst",
		"Ber"
	};
	
	const inline int classColorIndex[] =
	{
		-1,
		13,
		2,
		8,
		9,
		11,
		3,
		6,
		0,
		10,
		12,
		7,
		14,
		5,
		4,
		1,
		15
	};

	const std::array<int, 3> healers{ 2, 6, 10 };
	const std::array<int, 3> tanks{ 1, 3, 5 };
	const std::array<int, 8> dps{ 4, 7, 9, 11, 12, 13, 14, 16 };
	const std::array<int, 4> mdps{ 4, 7, 9, 16};
	const std::array<int, 3> cdps{ 11, 12, 13 };
	const std::array<int, 4> amdps{ 8, 10, 15, 16 };
	const std::array<int, 2> acdps{ 8, 14 };

	const std::bitset<17> healerBits{ "00000010001000100" };
	const std::bitset<17>   tankBits{ "00000000000101010" };
	const std::bitset<17>    dpsBits{ "10111101010010000" };
	const std::bitset<17>   mdpsBits{ "10000001010010000" };
	const std::bitset<17>   cdpsBits{ "00011100000000000" };
	const std::bitset<17>  amdpsBits{ "11000010100000000" };
	const std::bitset<17>  acdpsBits{ "00100000100000000" };

	bool inline isHealer(int cls) noexcept { return healerBits[cls]; }
	bool inline isTank(int cls) noexcept { return tankBits[cls]; }
	bool inline isDps(int cls) noexcept { return dpsBits[cls]; }
	bool inline isMDps(int cls) noexcept { return mdpsBits[cls]; }
	bool inline isCDps(int cls) noexcept { return cdpsBits[cls]; }
	bool inline isAMDps(int cls) noexcept { return amdpsBits[cls]; }
	bool inline isACDps(int cls) noexcept { return acdpsBits[cls]; }
}