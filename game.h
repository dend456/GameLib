#pragma once
#include <string>
#include <map>
#include <vector>
#include <functional>

#include "offsets.h"

using CommandFuncT = void(__thiscall*)(int, int*, const char*);
using CommandFuncCallbackT = std::string(*)(int, int*, const char*);

using ItemLinkFuncT = void(__thiscall*)(void*, char*, int, bool);

using RaidGroupFuncT = int(__thiscall*)(int, int*, int, int*);
using RaidSelectFuncT = int(__thiscall*)(int, int);

namespace Patterns
{
	static inline int SEARCH_SIZE = 0x800000;

#ifdef _WIN64
	static inline const char* COMMAND_FUNC_PATTERN = "\x48\x85\xd2\x0f\x84....\x48\x89\x5c\x24\x08\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8d\xac\x24\xb0\xf6\xff\xff";
	static inline const char* ITEMLINK_FUNC_PATTERN = "\x55\x8b\xec\x6a\xff\x68....\x64\xa1....\x50\x83\xec\x48\xa1....\x33\xc5\x89\x45\xf0";
	static inline const char* RAIDGROUP_FUNC_PATTERN = "\x55\x8b\xec\x64\xa1....\x6a\xff\x68....\x50\x64\x89\x25....\x83\xec\x0c\x53\x8b\xd9\x83\xbb\xf8\x01...\x75\x13";
	static inline const char* RAID_SELECT_FUNC_PATTERN = "\x56\x57\x8b\x7c\x24\x0c\x8b\xf1\x85\xff\x75\x3d\x8b\x86\x9c\x31";
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
	static inline ItemLinkFuncT fnItemLinkFunc = nullptr;
	static inline RaidGroupFuncT fnRaidGroupFunc = nullptr;
	static inline RaidSelectFuncT fnRaidSelectFunc = nullptr;

public:
	static inline FILE* logFile = nullptr;
	static inline int eqInst = 0;
	static inline int* charInfo = nullptr;

	static inline std::function<std::string(int eq, int* p, const char* s)> commandFuncCallback = nullptr;

	static uint64_t findPattern(char* addr, uint64_t size, const char* pattern) noexcept;
	static void hook(const std::vector<std::string>& funcs) noexcept;
	static void unhook() noexcept;

	static void __fastcall hookedCommandFunc(int eq, void* unk, int* p, const char* s);
	static void __fastcall hookedItemLinkFunc(void* item, void* unk, char* buffer, int size, bool unk2);
	static int __fastcall hookedRaidGroupFunc(void* window, void* unk, int* a, int b, int* c);
	static int __fastcall hookedRaidSelectFunc(void* t, void* unk, int a);
};

