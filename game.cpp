#include <fmt/core.h>
#include <Windows.h>
#include <map>
#include <string>
#include <MinHook.h>
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
    //fmt::print(logFile, "CommandFunc {:x} {:x} {}\n", (uint64_t)eq, (uint64_t)p, std::string(s));
    //fflush(logFile);
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
            fnCommandFunc(eq, p, r.c_str());
        }
    }
    else
    {
        fnCommandFunc(eq, p, s);
    }
}

void __fastcall Game::hookedItemLinkFunc(void* item, char* buffer, int size, bool unk2)
{
    if (item == nullptr || buffer == nullptr) return;
    fnItemLinkFunc(item, buffer, size, unk2);
}

int __fastcall Game::hookedRaidGroupFunc(void* window, uint64_t* a, uint64_t b, int* c)
{
    //fmt::print(logFile, "hookedRaid {:x} {:x}\n", (uint64_t)window, (uint64_t)a);
    //fflush(logFile);
    if (window == nullptr || a == nullptr)
    {
        return 0;
    }
    return fnRaidGroupFunc((uint64_t)window, a, b, c);
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
        for (const auto& s : funcs)
        {
            if (s == "CommandFunc")
            {
                uint64_t addr = findPattern((char*)base, Patterns::SEARCH_SIZE, Patterns::COMMAND_FUNC_PATTERN);
                if (addr > 0)
                {
                    commandFuncAddr = addr;
                    MH_CreateHook((LPVOID)addr, hookedCommandFunc, (LPVOID*)&fnCommandFunc);
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
                  /*  DetourTransactionBegin();
                    DetourUpdateThread(GetCurrentThread());
                    fnItemLinkFunc = (ItemLinkFuncT)addr;
                    DetourAttachEx(&(PVOID&)addr, hookedItemLinkFunc, &tramp, &pRealTarget, &pRealDetour);
                    DetourTransactionCommit();
                    itemLinkFunc = (ItemLinkFuncT)tramp;*/
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
                    MH_CreateHook((LPVOID)addr, hookedRaidGroupFunc, (LPVOID*)&fnRaidGroupFunc);
                }
                else
                {
                    fmt::print(logFile, "Unable to find RaidGroupFunc\n");
                    fflush(logFile);
                }
            }
        }

        MH_EnableHook(MH_ALL_HOOKS);
    }
    catch (const std::exception&)
    {
    }
}

void Game::unhook() noexcept
{
    MH_DisableHook(MH_ALL_HOOKS);
}