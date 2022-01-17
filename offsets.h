#pragma once
namespace Offsets
{
	namespace Raid
	{
		static constexpr uint64_t WINDOW_ADDR = 0xadeef4;
		static constexpr uint64_t WINDOW_OFFSET = 0;
		static constexpr uint64_t WINDOW_LOCKED_OFFSET = 0xb96ce0;
		static constexpr uint64_t WINDOW_GROUP_BUTTON_BASE_OFFSET = 0x254;
		static constexpr uint64_t WINDOW_SELECTED_RAIDER_ADDR = 0xb96814;
						 
		static constexpr uint64_t WINDOW_COLOR_ADDR = 0xadeee8;
		static constexpr uint64_t WINDOW_COLOR_BASE_OFFSET = 0x25c;
		static constexpr uint64_t WINDOW_COLOR_OFFSET = 0x50;
		static constexpr uint64_t WINDOW_OPEN_OFFSET = 0x20c;
						 
		static constexpr uint64_t LIST_ADDR = 0xb937a0;
		static constexpr uint64_t MY_NAME_ADDR = 0xb931a0;
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
		static inline constexpr uint64_t INST_ADDR = 0xdf1fa0;
		static inline constexpr uint64_t CHAR_ADDR = 0xde6670;
	}
}