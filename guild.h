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
	int zone;
	char unk1[0x4];
	char unk2[0x2];
	char name[16];
	char unk3[0x32];
	int level;
	int flags;
	int cls;
	int rank;
	char unk4[0x120];
	int lastOn;
	char unk6[0x8];
	char unk7[0x4];
	int tribute;
	int lastDonation;
	char unk8[0x4];
	char publicNote[256];
	char privateNote[256];
	bool tribute;
	bool trophy;
};
#pragma pack(pop)

//int jnr = sizeof(GuildMember);