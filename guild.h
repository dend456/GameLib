#pragma once

enum GuildFlags
{
	none = 0,
	alt = 1 << 1
};

#pragma pack(push, 1)
struct GuildMember
{
	GuildMember* next;
	int online;
	char unk0[0x4];
	char unk1[0x4];
	char unk2[0x2];
	char name[16];
	char unk3[0x32];
	int level;
	int flags;
	int cls;
	int rank;
	char unk4[0x120];
	char unk5[0x4];
	char unk6[0x8];
	char unk7[0x4];
	char unk8[0xc];
	char publicNote[256];
	char privateNote[256];
};
#pragma pack(pop)

//int jnr = sizeof(GuildMember);