#pragma once
namespace Offsets
{
	namespace Raid
	{
		static constexpr int WINDOW_ADDR = 0xae8fe0;
		static constexpr int WINDOW_OFFSET = 0;
		static constexpr int WINDOW_LOCKED_OFFSET = 0xba0dd0;
		static constexpr int WINDOW_GROUP_BUTTON_BASE_OFFSET = 0x240;
		static constexpr int WINDOW_SELECTED_RAIDER_ADDR = 0xba0904;

		static constexpr int WINDOW_COLOR_ADDR = 0xadeee8;
		static constexpr int WINDOW_COLOR_BASE_OFFSET = 0x2d4;
		static constexpr int WINDOW_COLOR_OFFSET = 0x2d4;
		static constexpr int WINDOW_OPEN_OFFSET = 0x5c;

		static constexpr int LIST_ADDR = 0xb9d890;
		static constexpr int MY_NAME_ADDR = 0xb9d290;
		static constexpr int NAME_OFFSET = 0;
		static constexpr int LEVEL_OFFSET = 0x80;
		static constexpr int CLASS_OFFSET = 0x84;
		static constexpr int GROUP_OFFSET = 0x90;
		static constexpr int DEAD_OFFSET = 0x95;
		static constexpr int RAIDLEAD_OFFSET = 0x88;
		static constexpr int GROUPLEAD_OFFSET = 0x89;
		static constexpr int ASSIST_OFFSET = 0x8a;
		static constexpr int MARKER_OFFSET = 0x8b;
		static constexpr int LOOTER_OFFSET = 0x8c;
	}

	namespace EQ
	{
		static inline constexpr int INST_ADDR = 0xba9ddc;
		static inline constexpr int CHAR_ADDR = 0xb9ae58;
	}

	namespace Baz
	{
		static inline constexpr int SELECTED_TRADER_ADDR = 0xa7b10;
		static inline constexpr int SELECTED_TRADER_OFFSET = 0x384;
		static inline constexpr int SELECTED_SLOT_OFFSET = 0xdc;
	}

	namespace Guild
	{
		static inline constexpr int GUILD_LIST_ADDR = 0xba13e4;
	}
}