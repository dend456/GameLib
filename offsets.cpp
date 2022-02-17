#include <Windows.h>
#include <WinBase.h>
#include "offsets.h"
#include "game.h"
#include <fstream>
#include <string>
#include <sstream>
#include <fmt/core.h>
#include <map>

namespace Offsets
{
	bool load(const std::filesystem::path& path) noexcept
	{
		if (!std::filesystem::exists(path)) return false;

		try
		{
			char str[255] = { 0 };
			std::string pathStrtmp = path.string();
			const char* pathStr = pathStrtmp.c_str();

			GetPrivateProfileStringA("EQ", "instAddr", TEXT(""), str, sizeof(str), pathStr);
			Offsets::EQ::INST_ADDR = std::stoi(std::string(str), 0, 0);
			GetPrivateProfileStringA("EQ", "charAddr", TEXT(""), str, sizeof(str), pathStr);
			Offsets::EQ::CHAR_ADDR = std::stoi(std::string(str), 0, 0);

			GetPrivateProfileStringA("Raid", "windowAddr", TEXT(""), str, sizeof(str), pathStr);
			Offsets::Raid::WINDOW_ADDR = std::stoi(std::string(str), 0, 0);
			GetPrivateProfileStringA("Raid", "windowOffset", TEXT(""), str, sizeof(str), pathStr);
			Offsets::Raid::WINDOW_OFFSET = std::stoi(std::string(str), 0, 0);
			GetPrivateProfileStringA("Raid", "colorOffset", TEXT(""), str, sizeof(str), pathStr);
			Offsets::Raid::WINDOW_COLOR_OFFSET = std::stoi(std::string(str), 0, 0);
			GetPrivateProfileStringA("Raid", "lockedOffset", TEXT(""), str, sizeof(str), pathStr);
			Offsets::Raid::WINDOW_LOCKED_OFFSET = std::stoi(std::string(str), 0, 0);
			GetPrivateProfileStringA("Raid", "openOffset", TEXT(""), str, sizeof(str), pathStr);
			Offsets::Raid::WINDOW_OPEN_OFFSET = std::stoi(std::string(str), 0, 0);
			GetPrivateProfileStringA("Raid", "buttonsOffset", TEXT(""), str, sizeof(str), pathStr);
			Offsets::Raid::WINDOW_GROUP_BUTTON_BASE_OFFSET = std::stoi(std::string(str), 0, 0);
			GetPrivateProfileStringA("Raid", "selectedRaiderAddr", TEXT(""), str, sizeof(str), pathStr);
			Offsets::Raid::WINDOW_SELECTED_RAIDER_ADDR = std::stoi(std::string(str), 0, 0);
			GetPrivateProfileStringA("Raid", "listAddr", TEXT(""), str, sizeof(str), pathStr);
			Offsets::Raid::LIST_ADDR = std::stoi(std::string(str), 0, 0);
			GetPrivateProfileStringA("Raid", "nameAddr", TEXT(""), str, sizeof(str), pathStr);
			Offsets::Raid::MY_NAME_ADDR = std::stoi(std::string(str), 0, 0);

			GetPrivateProfileStringA("Guild", "listAddr", TEXT(""), str, sizeof(str), pathStr);
			Offsets::Guild::GUILD_LIST_ADDR = std::stoi(std::string(str), 0, 0);
		}
		catch (const std::exception& e)
		{
			fmt::print(Game::logFile, "Error loading ini: {}\n", std::string(e.what()));
			fflush(Game::logFile);
			return false;
		}
		return true;
	}
}
