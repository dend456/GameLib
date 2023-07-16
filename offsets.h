#pragma once
#include <filesystem>

namespace Offsets
{
	namespace Raid
	{
		inline uint64_t WINDOW_ADDR = 0xd31b60;
		inline uint64_t WINDOW_OFFSET = 0;
		inline uint64_t WINDOW_COLOR_OFFSET = 0x440;
		inline uint64_t WINDOW_LOCKED_OFFSET = 0xde9dbc;
		inline uint64_t WINDOW_GROUP_BUTTON_BASE_OFFSET = 0x318;
		inline uint64_t WINDOW_SELECTED_RAIDER_ADDR = 0xde98f0;
		inline uint64_t WINDOW_OPEN_OFFSET = 0xf0;
		
		inline uint64_t LIST_ADDR = 0xde687c;
		inline uint64_t MY_NAME_ADDR = 0xde6170;
		inline uint64_t NAME_OFFSET = 0;
		inline uint64_t LEVEL_OFFSET = 0x80;
		inline uint64_t CLASS_OFFSET = 0x84;
		inline uint64_t GROUP_OFFSET = 0x90;
		inline uint64_t DEAD_OFFSET = 0x95;
		inline uint64_t RAIDLEAD_OFFSET = 0x88;
		inline uint64_t GROUPLEAD_OFFSET = 0x89;
		inline uint64_t ASSIST_OFFSET = 0x8a;
		inline uint64_t MARKER_OFFSET = 0x8b;
		inline uint64_t LOOTER_OFFSET = 0x8c;
	}

	namespace EQ
	{
		inline uint64_t INST_ADDR = 0xdf1da0;
		inline uint64_t CHAR_ADDR = 0xde6670;

		inline uint64_t ZONE_OFFSET = 0;
	}

	namespace Baz
	{
		inline uint64_t SELECTED_TRADER_ADDR = 0xa7b10;
		inline uint64_t SELECTED_TRADER_OFFSET = 0x384;
		inline uint64_t SELECTED_SLOT_OFFSET = 0xdc;
	}

	namespace Guild
	{
		inline uint64_t GUILD_LIST_ADDR = 0xdea3f0;
	}

	bool load(const std::filesystem::path& path) noexcept;
}