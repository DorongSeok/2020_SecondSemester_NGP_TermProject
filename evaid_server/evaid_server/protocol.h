#pragma once

#define SERVERIP "127.0.0.1"
//#define SERVERIP "59.14.252.78"
//#define SERVERIP "14.35.11.46" //동석
#define SERVERPORT 9000
#define MAXBUFFER 512
constexpr BYTE CLIENT_LIMITE = 2;
constexpr int NULLVAL = -1;

constexpr BYTE cs_login = 1;
constexpr BYTE sc_login = 2;
constexpr BYTE cs_ready = 3;
constexpr BYTE sc_ready = 4;
constexpr BYTE cs_user = 5;
constexpr BYTE sc_user = 6;
constexpr BYTE cs_start = 7;
constexpr BYTE sc_start = 8;
constexpr BYTE cs_end = 9;
constexpr BYTE sc_end = 10;


#pragma pack(push, 1)
class Protocol_TABLE {
public:
	BYTE t[20][10];
};

class cs_packet_login {
public:
	BYTE size;
	BYTE type;
};

class cs_packet_ready {
public:
	BYTE size;
	BYTE type;
	BYTE id;      //준비완료 클라 ID
};

class cs_packet_user {
public:
	BYTE size;
	BYTE type;
	Protocol_TABLE table;
	int pos[2];		//0: X, 1: Y
	BYTE skillGauge;
	bool skillActive;
	BYTE nextBlock;
};

class cs_packet_start {
public:
	BYTE size;
	BYTE type;
};

class cs_packet_end {
public:
	BYTE size;
	BYTE type;
};

////////

class sc_packet_login {
public:
	BYTE size;
	BYTE type;
	BYTE id;      //접속 클라 ID
	BYTE f_login;
	BYTE s_login;
};

class sc_packet_ready {
public:
	BYTE size;
	BYTE type;
	BYTE id;      //준비완료 한 클라 ID
};

class sc_packet_user {
public:
	//s: client_id/ c: iMyPlayerNum
	BYTE size;
	BYTE type;
	Protocol_TABLE table[2];
	int pos[2][2];		//0: X, 1: Y
	BYTE skillGauge[2];
	bool skillActive[2];
	BYTE nextBlock[2];
};

class sc_packet_start {
public:
	BYTE size;
	BYTE type;
	bool start;		//true일 때 시작
};

class sc_packet_end {
public:
	BYTE size;
	BYTE type;
	bool end;		//true일 때 끝
};
#pragma pack(pop)


///for test dummy packet
constexpr BYTE CS_1 = 11;
constexpr BYTE CS_2 = 12;
constexpr BYTE CS_3 = 13;
constexpr BYTE CS_4 = 14;

#pragma pack(push, 1)
class cs_packet_1 {
public:
	BYTE size;   //1
	BYTE type;   //1
	BYTE data;   //1
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