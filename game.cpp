#include <fmt/core.h>
#include <Windows.h>
#include <map>
#include <string>
#include <detours/detours.h>
#include "game.h"
#include <fstream>


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

void __fastcall Game::hookedCommandFunc(uint64_t eq, uint64_t* p, const char* s)
{
    fmt::print(logFile, "CommandFunc {:x} {:x} {}\n", (uint64_t)eq, (uint64_t)p, std::string(s));
    fflush(logFile);
    if (eq == 0 || p == nullptr)
    {
        uint64_t base = (uint64_t)GetModuleHandle(nullptr);
        uint64_t addr = base + Offsets::EQ::INST_ADDR;
        eq = *(uint64_t*)addr;
        addr = base + Offsets::EQ::CHAR_ADDR;
        p = *(uint64_t**)addr;
    }
    Game::eqInst = eq;
    Game::charInfo = p;
    if (commandFuncCallback)
    {
        std::string r = commandFuncCallback(eq, p, s);
        if (r.length() > 0)
        {
            commandFunc(eq, p, r.c_str());
        }
    }
    else
    {
        commandFunc(eq, p, s);
    }
}

void __fastcall Game::hookedItemLinkFunc(void* item, char* buffer, int size, bool unk2)
{
    if (item == nullptr || buffer == nullptr) return;
    itemLinkFunc(item, buffer, size, unk2);
}

int __fastcall Game::hookedRaidGroupFunc(void* window, uint64_t* a, int b, int* c)
{
    //fmt::print(logFile, "hookedRaid {:x} {:x}\n", (int)window, (int)a);
    //fflush(logFile);
    if (window == nullptr || a == nullptr)
    {
        return 0;
    }
    return raidGroupFunc((uint64_t)window, a, b, c);
}

int __fastcall Game::hookedRaidSelectFunc(void* t, int a)
{
    return fnRaidSelectFunc((uint64_t)t, a);
}

void Game::hook(const std::vector<std::string>& funcs) noexcept
{
    try
    {
        uint64_t base = (uint64_t)GetModuleHandle(nullptr);
        PVOID pRealTarget;
        PVOID pRealDetour;
        PDETOUR_TRAMPOLINE tramp;
        for (const auto& s : funcs)
        {
            if (s == "CommandFunc")
            {
                uint64_t addr = findPattern((char*)base, Patterns::SEARCH_SIZE, Patterns::COMMAND_FUNC_PATTERN);
                if (addr > 0)
                {
                    DetourTransactionBegin();
                    DetourUpdateThread(GetCurrentThread());
                    fnCommandFunc = (CommandFuncT)addr;
                    DetourAttachEx(&(PVOID&)addr, hookedCommandFunc, &tramp, &pRealTarget, &pRealDetour);
                    DetourTransactionCommit();
                    commandFunc = (CommandFuncT)tramp;
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
                {
                    DetourTransactionBegin();
                    DetourUpdateThread(GetCurrentThread());
                    fnItemLinkFunc = (ItemLinkFuncT)addr;
                    DetourAttachEx(&(PVOID&)addr, hookedItemLinkFunc, &tramp, &pRealTarget, &pRealDetour);
                    DetourTransactionCommit();
                    itemLinkFunc = (ItemLinkFuncT)tramp;
                }
                else
                {
                    fmt::print(logFile, "Unable to find ItemLinkFunc\n");
                    fflush(logFile);
                }
            }
            else if (s == "RaidGroupFunc")
            {
                uint64_t addr = findPattern((char*)base, Patterns::SEARCH_SIZE, Patterns::RAIDGROUP_FUNC_PATTERN);
                if (addr > 0)
                {
                    DetourTransactionBegin();
                    DetourUpdateThread(GetCurrentThread());
                    fnRaidGroupFunc = (RaidGroupFuncT)addr;
                    DetourAttachEx(&(PVOID&)addr, hookedRaidGroupFunc, &tramp, &pRealTarget, &pRealDetour);
                    DetourTransactionCommit();
                    raidGroupFunc = (RaidGroupFuncT)tramp;
                }
                else
                {
                    fmt::print(logFile, "Unable to find RaidGroupFunc\n");
                    fflush(logFile);
                }
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
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    /*
    if (fnCommandFunc) DetourRemove((PBYTE)fnCommandFunc, (PBYTE)hookedCommandFunc);
    if (fnItemLinkFunc) DetourRemove((PBYTE)fnItemLinkFunc, (PBYTE)hookedItemLinkFunc);
    if (fnRaidGroupFunc) DetourRemove((PBYTE)fnRaidGroupFunc, (PBYTE)hookedRaidGroupFunc);
    if (fnRaidSelectFunc) DetourRemove((PBYTE)fnRaidSelectFunc, (PBYTE)hookedRaidSelectFunc);
    */
    if (fnCommandFunc) DetourDetach(&(PVOID&)fnCommandFunc, hookedCommandFunc);
    if (fnItemLinkFunc) DetourDetach(&(PVOID&)fnItemLinkFunc, hookedItemLinkFunc);
    if (fnRaidGroupFunc) DetourDetach(&(PVOID&)fnRaidGroupFunc, hookedRaidGroupFunc);
    if (fnRaidSelectFunc) DetourDetach(&(PVOID&)fnRaidSelectFunc, hookedRaidSelectFunc);
    
    DetourTransactionCommit();
}