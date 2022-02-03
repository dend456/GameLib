#include <fmt/core.h>
#include <Windows.h>
#include <map>
#include <string>
#include "detours.h"
#include "game.h"
#include <fstream>
#include "guild.h"


uint64_t Game::findPattern(char* addr, uint64_t size, const char* pattern) noexcept
{
    std::size_t len = std::strlen(pattern);
    if (len <= 0) return 0;

    for (char* end = addr + size; addr < end; ++addr)
    {
        bool found = true;
        for (std::size_t i = 0; i < len; ++i)
        {
            if (!(pattern[i] == '.' || addr[i] == pattern[i]))
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            return (uint64_t)addr;
        }
    }
    return 0;
}

void __fastcall Game::hookedCommandFunc(int eq, void* unk, int* p, const char* s)
{
    //fmt::print(logFile, "CommandFunc {:x} {:x} {}\n", (int)eq, (int)p, std::string(s));
    //fflush(logFile);
    if (eq == 0 || p == nullptr)
    {
        uint64_t base = (uint64_t)GetModuleHandle(nullptr);
        uint64_t addr = base + Offsets::EQ::INST_ADDR;
        eq = *(int*)addr;
        addr = base + Offsets::EQ::CHAR_ADDR;
        p = *(int**)addr;
    }
    Game::eqInst = eq;
    Game::charInfo = p;
    if (commandFuncCallback)
    {
        std::string r = commandFuncCallback(eq, p, s);
        if (r.length() > 0)
            fnCommandFunc(eq, p, r.c_str());
    }
    else
    {
        fnCommandFunc(eq, p, s);
    }
}

void __fastcall Game::hookedItemLinkFunc(void* item, void* unk, char* buffer, int size, bool unk2)
{
    if (item == nullptr || buffer == nullptr) return;
    fnItemLinkFunc(item, buffer, size, unk2);
}

int __fastcall Game::hookedRaidGroupFunc(void* window, void* unk, int* a, int b, int* c)
{
    //fmt::print(logFile, "hookedRaid {:x} {:x}\n", (int)window, (int)a);
    //fflush(logFile);
    if (window == nullptr || a == nullptr)
    {
        return 0;
    }
    return fnRaidGroupFunc((uint64_t)window, a, b, c);
}

int __fastcall Game::hookedRaidSelectFunc(void* t, void* unk, int a)
{
    return fnRaidSelectFunc((uint64_t)t, a);
}

int* __fastcall Game::hookedRaidInviteAllFunc(void* t, void* unk, char* p)
{
    static void* savedt = nullptr;
    static char* savedp = nullptr;

    if (t)
    {
        savedt = t;
        savedp = p;
    }
    if (savedt == nullptr) return nullptr;

    //fmt::print(logFile, "inviteall {:x} {:x}\n", (int)t, (int)p);
    //fflush(logFile);

    return fnRaidInviteAllFunc(savedt, savedp);
}

int __fastcall Game::hookedBazaarFindFunc(int t, void* unk, int a)
{
    static int savedt = 0;
    static int saveda = 0;
    if (t != 0 && a != 0)
    {
        savedt = t;
        saveda = a;
    }
    if (savedt == 0 || saveda == 0) return 0;
    //fmt::print(logFile, "{:x} {:x}\n", savedt, saveda);
    //fflush(logFile);
    return fnBazaarFindFunc(savedt, saveda);
}

void Game::hook(const std::vector<std::string>& funcs) noexcept
{
    try
    {
        uint64_t base = (uint64_t)GetModuleHandle(nullptr);
        for (const auto& s : funcs)
        {
            if (s == "CommandFunc")
            {
                uint64_t addr = findPattern((char*)base, Patterns::SEARCH_SIZE, Patterns::COMMAND_FUNC_PATTERN);
                if (addr > 0)
                {
                    fnCommandFunc = (CommandFuncT)DetourFunction((PBYTE)addr, (PBYTE)hookedCommandFunc);
                    //fmt::print(logFile, "Hooked CommandFunc at 0x{:x}\n", addr);
                    //fflush(logFile);
                }
                else
                {
                    fmt::print(logFile, "Unable to find CommandFunc\n");
                    fflush(logFile);
                }
            }
            else if (s == "ItemLinkFunc")
            {
                uint64_t addr = findPattern((char*)base, Patterns::SEARCH_SIZE, Patterns::ITEMLINK_FUNC_PATTERN);
                if (addr > 0)
                    fnItemLinkFunc = (ItemLinkFuncT)DetourFunction((PBYTE)addr, (PBYTE)hookedItemLinkFunc);
                else
                {
                    fmt::print(logFile, "Unable to find ItemLinkFunc\n");
                    fflush(logFile);
                }
            }
            else if (s == "BazaarFindFunc")
            {
                uint64_t addr = findPattern((char*)base, Patterns::SEARCH_SIZE, Patterns::BAZAAR_FIND_FUNC_PATTERN);
                if (addr > 0)
                    fnBazaarFindFunc = (BazaarFindFuncT)DetourFunction((PBYTE)addr, (PBYTE)hookedBazaarFindFunc);
                else
                {
                    fmt::print(logFile, "Unable to find BazaarFindFunc\n");
                    fflush(logFile);
                }
            }
            else if (s == "RaidGroupFunc")
            {
                uint64_t addr = findPattern((char*)base, Patterns::SEARCH_SIZE, Patterns::RAIDGROUP_FUNC_PATTERN);
                if (addr > 0)
                {
                    fnRaidGroupFunc = (RaidGroupFuncT)DetourFunction((PBYTE)addr, (PBYTE)hookedRaidGroupFunc);
                }
                else
                {
                    fmt::print(logFile, "Unable to find RaidGroupFunc\n");
                    fflush(logFile);
                }
                /*
                addr = findPattern((char*)base, Patterns::SEARCH_SIZE, Patterns::INVITE_ALL_TO_RAID_FUNC_PATTERN);
                if (addr > 0)
                {
                    fnRaidInviteAllFunc = (RaidInviteAllFuncT)DetourFunction((PBYTE)addr, (PBYTE)hookedRaidInviteAllFunc);
                }
                else
                {
                    fmt::print(logFile, "Unable to find InviteAllToRaidFunc\n");
                    fflush(logFile);
                }
                */
                /*addr = findPattern((char*)base + Patterns::RAID_SELECT_SEARCH_ADDR, Patterns::SEARCH_SIZE, Patterns::RAID_SELECT_FUNC_PATTERN, Patterns::RAID_SELECT_FUNC_MASK);
                if (addr > 0)
                {
                    fnRaidSelectFunc = (RaidSelectFuncT)DetourFunction((PBYTE)addr, (PBYTE)hookedRaidSelectFunc);
                }
                else
                {
#ifdef PRINT_OUTPUT
                    printf("Unable to find RaidSelectFunc base=0x%08x\n", base);
#endif
                }*/
            }

        }
    }
    catch (const std::exception&)
    {
    }
}

void Game::unhook() noexcept
{
    if (fnCommandFunc) DetourRemove((PBYTE)fnCommandFunc, (PBYTE)hookedCommandFunc);
    if (fnItemLinkFunc) DetourRemove((PBYTE)fnItemLinkFunc, (PBYTE)hookedItemLinkFunc);
    if (fnRaidGroupFunc) DetourRemove((PBYTE)fnRaidGroupFunc, (PBYTE)hookedRaidGroupFunc);
    if (fnRaidSelectFunc) DetourRemove((PBYTE)fnRaidSelectFunc, (PBYTE)hookedRaidSelectFunc);
    if (fnBazaarFindFunc) DetourRemove((PBYTE)fnBazaarFindFunc, (PBYTE)hookedBazaarFindFunc);
    if (fnRaidInviteAllFunc) DetourRemove((PBYTE)fnRaidInviteAllFunc, (PBYTE)hookedRaidInviteAllFunc);
}