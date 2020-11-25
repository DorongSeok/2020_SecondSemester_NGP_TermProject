#pragma once

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define MAXBUFFER 1024

constexpr BYTE CS_1 = 1;
constexpr BYTE CS_2 = 2;
constexpr BYTE CS_3 = 3;
constexpr BYTE CS_4 = 4;

#pragma pack(1)
class cs_packet_1 {
public:
	BYTE size;	//1
	BYTE type;	//1
	BYTE data;	//1
};
class cs_packet_2 {
public:
	BYTE size;
	BYTE type;
	BYTE data;
};
class cs_packet_3 {
public:
	BYTE size;
	BYTE type;
	BYTE data_1;
	BYTE data_2;
};
class cs_packet_4 {
public:
	BYTE size;
	BYTE type;
	BYTE data;
};
#pragma pack(pop)