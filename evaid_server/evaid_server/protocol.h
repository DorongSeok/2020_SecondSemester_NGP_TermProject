#pragma once

#define SERVERIP "127.0.0.1"
//#define SERVERIP "59.14.252.78"
//#define SERVERIP "14.35.11.46" //동석
#define SERVERPORT 9000
#define MAXBUFFER 1024

constexpr BYTE cs_login = 1;
constexpr BYTE sc_login = 2;
constexpr BYTE cs_ready = 3;
constexpr BYTE sc_ready = 4;


#pragma pack(1)
class cs_packet_login {
public:
	BYTE size;
	BYTE type;
};

class cs_packet_ready {
public:
	BYTE size;
	BYTE type;
	BYTE id;		//몇 번째 준비완료 클라인지
};
#pragma pack()

#pragma pack(1)
class sc_packet_login {
public:
	BYTE size;
	BYTE type;
	BYTE id;		//몇 번째 접속 클라인지
};

class sc_packet_ready {
public:
	BYTE size;
	BYTE type;
};
#pragma pack()

///for test dummy packet
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
///for test dummy packet
