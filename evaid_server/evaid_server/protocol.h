#pragma once
#define MAXBUFFER 1024

constexpr int CS_1 = 1;
constexpr int CS_2 = 2;
constexpr int CS_3 = 3;
constexpr int CS_4 = 4;

#pragma pack(1)
class cs_packet_1 {
public:
	short size;
	char type;
	char data;
};
class cs_packet_2 {
public:
	short size;
	char type;
	char data;
};
class cs_packet_3 {
public:
	short size;
	char type;
	char data;
};
class cs_packet_4 {
public:
	short size;
	char type;
	char data;
};
#pragma pack(pop)