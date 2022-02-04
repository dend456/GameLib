#include <Windows.h>
#include <libloaderapi.h>
#include <regex>
#include "guild.h"
#include "offsets.h"


GuildMember* Guild::getHead() noexcept
{
	uint64_t base = (uint64_t)GetModuleHandle(nullptr);
	uint64_t addr = base + Offsets::Guild::GUILD_LIST_ADDR;
	addr = *(int*)addr;

	GuildMember* m = (GuildMember*)addr;
	if (m && m->name[0] != 0) return m;
	return nullptr;
}

std::map<std::string, std::vector<std::string>> Guild::getAlts() noexcept
{
	static std::regex altRe{ R"x((RAID )?ALT - ([A-Za-z]{4,15}))x" };
	std::map<std::string, std::vector<std::string>> alts;
	GuildMember* m = getHead();
	if (!m) return alts;

	std::smatch match;
	std::string publicNote(256, '\x00');
	do
	{
		std::copy_n(m->publicNote, 256, publicNote.data());
		if (std::regex_search(publicNote, match, altRe))
		{
			alts[match.str(2)].emplace_back(m->name);
		}
		m = m->next;
	} while (m);
	return alts;
}