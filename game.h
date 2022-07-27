#pragma once
#include <string>
#include <map>
#include <vector>
#include <functional>

#include "offsets.h"

using CommandFuncT = void(__fastcall*)(uint64_t, uint64_t*, const char*);
using CommandFuncCallbackT = std::string(*)(uint64_t, uint64_t*, const char*);

using ItemLinkFuncT = void(__fastcall*)(void*, char*, uint64_t, bool);

using RaidGroupFuncT = int(__fastcall*)(uint64_t, uint64_t*, uint64_t, int*);
using RaidSelectFuncT = int(__fastcall*)(uint64_t, int);
using LinkedListRemoveNodeFuncT = void(__stdcall*)(void*, void*);
using MouseClickFuncT = void(__stdcall*)(void*, int, int);

using OnMessageFuncT = void(__fastcall*)(char*, char*, uint64_t, char, char, char);
using OnLogMessageFuncT = void(__fastcall*)(void*, char*);

namespace Patterns
{
	static inline uint64_t SEARCH_SIZE = 0x800000;

#ifdef _WIN64
	// #%s %s  static inline const char* COMMAND_FUNC_PATTERN = "\x48\x85\xd2\x0f\x84....\x48\x89\x5c\x24\x08\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8d\xac\x24\xb0\xf6\xff\xff";
	static inline const char* COMMAND_FUNC_PATTERN = "\x48\x85\xd2\x0f\x84....\x48\x89\x5c\x24.\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8d....\xff\xff\x48\x81\xec....\x48\x8b\x05....\x48\x33\xc4\x48\x89\x85....\x49\x8b\xd8\x4c\x8b\xfa";
	static inline const char* ITEMLINK_FUNC_PATTERN = "\x55\x8b\xec\x6a\xff\x68....\x64\xa1....\x50\x83\xec\x48\xa1....\x33\xc5\x89\x45\xf0";
	static inline const char* RAIDGROUP_FUNC_PATTERN = "\x48\x89\x5c\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7c\x24\x20\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8b\xec\x48\x83\xec\x60\x4d\x8b\xf9\x45\x8b\xe0";
	static inline const char* RAID_SELECT_FUNC_PATTERN = "\x56\x57\x8b\x7c\x24\x0c\x8b\xf1\x85\xff\x75\x3d\x8b\x86\x9c\x31";
	static inline const char* LINKED_LIST_REMOVE_NODE_FUNC_PATTERN = "\x48\x85\xd2\x0f\x84....\x53\x48\x83\xec\x20\x48\x8b\xda\x48\x8b\x12\x48\x85\xd2";
	static inline const char* MOUSE_CLICK_FUNC_PATTERN = "\x48\x8b\xc4\x48\x89\x58.\x48\x89\x70.\x48\x89\x78.\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8d\xa8....\x48\x81\xec....\x0f\x29\x70.\x0f\x29\x78.\x44\x0f\x29\x40";
	static inline const char* ON_MESSAGE_FUNC_PATTERN = "\x48\x89\x5c\x24\x08\x48\x89\x74\x24\x10\x48\x89\x7c\x24\x18\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8b\xec\x48\x83\xec\x70\x45\x0f\xb6\xf9\x41\x8b\xf8\x48\x8b\xda\x4c\x8b\xe1\x45\x33\xed\x44\x89\x6d\xb0\x48\xc7\xc6\xff\xff\xff\xff";
	static inline const char* ON_MESSAGE2_FUNC_PATTERN = "\x40\x55\x53\x56\x57\x41\x54\x41\x56\x41\x57\x48\x8d\xac\x24\x20\xf8\xff\xff\x48\x81\xec\xe0\x08\x00\x00\x48\x8b\x05\x77\xd5\x96\x00";
	static inline const char* ON_MESSAGE3_FUNC_PATTERN = "\x40\x55\x57\x41\x54\x41\x55\x41\x57\x48\x8d\xac\x24\xc0\xf8\xff\xff\x48\x81\xec\x40\x08\x00\x00\x48\x8b\x05\xe9\x1c\x82\x00\x48\x33\xc4\x48\x89\x85\x30\x07\x00\x00\x48\x8b\xfa\x45\x33\xff";
	static inline const char* ON_MESSAGE4_FUNC_PATTERN = "\x48\x89\x74\x24\x18\x57\x48\x83\xec\x20\x48\x8b\xf2\x48\x8b\xf9\x48\x85\xd2\x74\x7a\x48\x89\x5c\x24\x30\x48\xc7\xc3\xff\xff\xff\xff";
	static inline const char* ON_LOG_MESSAGE_FUNC_PATTERN = "\x48\x89\x5c\x24\x08\x57\x48\x83\xec\x30\x48\x8b\xda\x48\x8b\x05....\x48\x85\xc0\x0f\x84....\x80\xb8....\x00";

#else
	static inline const char* COMMAND_FUNC_PATTERN = "\x55\x8b\xec\x6a\xff\x68....\x64\xa1....\x50\x64\x89\x25....\x81\xec\x30\x0c..";
	static inline const char* ITEMLINK_FUNC_PATTERN = "\x55\x8b\xec\x6a\xff\x68....\x64\xa1....\x50\x83\xec\x48\xa1....\x33\xc5\x89\x45\xf0";
	static inline const char* RAIDGROUP_FUNC_PATTERN = "\x55\x8b\xec\x64\xa1....\x6a\xff\x68....\x50\x64\x89\x25....\x83\xec\x0c\x53\x8b\xd9\x83\xbb.....\x75\x13";
	static inline const char* RAID_SELECT_FUNC_PATTERN = "\x56\x57\x8b\x7c\x24\x0c\x8b\xf1\x85\xff\x75\x3d\x8b\x86\x9c\x31";
#endif
}

class Game
{
private:
	static inline CommandFuncT fnCommandFunc = nullptr;
	static inline uint64_t commandFuncAddr = 0;

	static inline ItemLinkFuncT fnItemLinkFunc = nullptr;
	static inline RaidGroupFuncT fnRaidGroupFunc = nullptr;
	static inline uint64_t raidGroupFuncAddr = 0;
	static inline RaidSelectFuncT fnRaidSelectFunc = nullptr;

	static inline CommandFuncT commandFunc = nullptr;
	static inline ItemLinkFuncT itemLinkFunc = nullptr;
	static inline RaidGroupFuncT raidGroupFunc = nullptr;

	static inline OnMessageFuncT onMessageFunc = nullptr;
	static inline OnMessageFuncT fnOnMessageFunc = nullptr;
	static inline OnLogMessageFuncT onLogMessageFunc = nullptr;
	static inline OnLogMessageFuncT fnOnLogMessageFunc = nullptr;
public:
	static inline FILE* logFile = nullptr;
	static inline uint64_t eqInst = 0;
	static inline uint64_t* charInfo = nullptr;

	static inline std::function<std::string(uint64_t eq, uint64_t* p, const char* s)> commandFuncCallback = nullptr;

	static uint64_t findPattern(char* addr, uint64_t size, const char* pattern) noexcept;
	static void hook(const std::vector<std::string>& funcs) noexcept;
	static void unhook() noexcept;

	static void __fastcall hookedCommandFunc(uint64_t eq, uint64_t* p, const char* s);
	static void __fastcall hookedItemLinkFunc(void* item, char* buffer, int size, bool unk2);
	static int __fastcall hookedRaidGroupFunc(void* window, uint64_t* a, uint64_t b, int* c);
	static int __fastcall hookedRaidSelectFunc(void* t, int a);
	static void __fastcall hookedOnMessageFunc(char* c1, char* str, uint64_t color, char c2, char c3, char c4);
	static void __fastcall hookedOnLogMessageFunc(void* unk, char* str);
	static void printGuild();
};

