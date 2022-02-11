#pragma once
namespace Offsets
{
	namespace Raid
	{
		static constexpr uint64_t WINDOW_ADDR = 0xd30b60;
		static constexpr uint64_t WINDOW_OFFSET = 0;
		static constexpr uint64_t WINDOW_COLOR_OFFSET = 0x430;
		static constexpr uint64_t WINDOW_LOCKED_OFFSET = 0xde8dbc;
		static constexpr uint64_t WINDOW_GROUP_BUTTON_BASE_OFFSET = 0x308;
		static constexpr uint64_t WINDOW_SELECTED_RAIDER_ADDR = 0xde88f0;
						 /*
		static constexpr uint64_t WINDOW_COLOR_ADDR = 0xd31b40;
		static constexpr uint64_t WINDOW_COLOR_OFFSET = 0x38;
		static constexpr uint64_t WINDOW_COLOR_BASE_OFFSET = 0xa34;
						 */
		static constexpr uint64_t WINDOW_OPEN_OFFSET = 0x20c;

		static constexpr uint64_t LIST_ADDR = 0xde587c;
		static constexpr uint64_t MY_NAME_ADDR = 0xde5170;
		static constexpr uint64_t NAME_OFFSET = 0;
		static constexpr uint64_t LEVEL_OFFSET = 0x80;
		static constexpr uint64_t CLASS_OFFSET = 0x84;
		static constexpr uint64_t GROUP_OFFSET = 0x90;
		static constexpr uint64_t DEAD_OFFSET = 0x95;
		static constexpr uint64_t RAIDLEAD_OFFSET = 0x88;
		static constexpr uint64_t GROUPLEAD_OFFSET = 0x89;
		static constexpr uint64_t ASSIST_OFFSET = 0x8a;
		static constexpr uint64_t MARKER_OFFSET = 0x8b;
		static constexpr uint64_t LOOTER_OFFSET = 0x8c;
	}

	namespace EQ
	{
		static inline constexpr uint64_t INST_ADDR = 0xdf0da0;
		static inline constexpr uint64_t CHAR_ADDR = 0xde5670;
	}

	namespace Baz
	{
		static inline constexpr uint64_t SELECTED_TRADER_ADDR = 0xa7b10;
		static inline constexpr uint64_t SELECTED_TRADER_OFFSET = 0x384;
		static inline constexpr uint64_t SELECTED_SLOT_OFFSET = 0xdc;
	}

	namespace Guild
	{
		static inline constexpr uint64_t GUILD_LIST_ADDR = 0xde93f0;
	}
}