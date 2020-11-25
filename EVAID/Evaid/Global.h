#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <commctrl.h>
#include "RankMng.h"
#include <Windows.h>
#include <atlimage.h>
#include <time.h>
#include <iostream>
#include <assert.h>
#include <vector>
#include <fstream>
#include "SoundManager.h"
#include "..\..\evaid_server\evaid_server\protocol.h"

using namespace std;

#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")

using std::cout;
using std::endl;

//client
constexpr auto CLIENT_WIDTH = 1200;	// �ʺ�;
constexpr auto CLIENT_HEIGHT = 900;		// ����

//child
constexpr auto STAGE_WIDTH = 600;
constexpr auto STAGE_HEIGHT = 900;

// �÷��̾ ���� x�� �׸��� ���� ��ǥ ����(�÷��̾� 1 = 0, �÷��̾� 2 = CLIENT_WIDTH / 2)
constexpr auto DrawGapX = CLIENT_WIDTH / 2;

// ǥ ���� ����Ʈ
constexpr auto  PTStartX = 100;
constexpr auto  PTStartY = 50;

//timer
constexpr auto timer_Main = 1;
constexpr auto tick_Main = 10;		//(ms)
constexpr auto tick_skill = 5;

//Table Size setting
constexpr auto table_WIDTH = 10;
constexpr auto table_HEIGHT = 20;

//Block Size setting (���簢���̸� �̹��� ���ϰ� ũ�⸦ �������)
constexpr auto BLOCK_SIZE = 40;
constexpr auto HERO_SIZE = 40;

// ���� ��
#define Radian (double)0.017453292519943295769236907684886

// shadow
#define shadowDelay (double)2

// ����� ������ġ
const int HeroStartPosX = 300;
const int HeroStartPosY = 630;

// ����� �������� �ӵ�
#define Gravity 9.8

const auto UNSETVAL = -1234;

// ��ų����
#define MaxSkillGauge 10
#define NormalSkillGaugeIncrement 1

#define FPS			int(1000/50)

class JSize {
public:
	int width;
	int height;
	void set(int w, int h) {
		width = w;
		height = h;
	}
	void set(int input) {
		width = input;
		height = input;
	}
};

class JPoint {
public:
	int x;
	int y;
	JPoint() {
		x = UNSETVAL;
		y = UNSETVAL;
	}
	JPoint(int _x, int _y) : x(_x), y(_y) {};
	~JPoint() {};
};

//�������� �������
#define Block_Type_Count 5
enum class BLOCK_TYPE {
	A
	, B
	, C
	, D
	, E
	, NONE
	, STACK		//�׿��� ���
	, SHADOW		//�׸���
};

// ����
enum class STATE {
	NORMAL
	, LEFT
	, RIGHT
};

// ����� ����
enum class HERO {
	Hero1,
	Hero2,
};

///0~MAX-1
static int getRandom(int MAX) {
	return rand() % MAX;
}

template<typename gdiobj>
inline bool SafeRelease(gdiobj& target) {
	if (!target) return false;
	DeleteObject(target);
	target = NULL;
	return true;
}

class Global {
private:
	static Global* inst;
	Global();
	~Global() {};
public:
	//Global::getInstance()-> ��ü�̸�
	static Global* getInstance() {
		if (inst == nullptr) inst = new Global();
		return inst;
	}

	// tick_Main �������� ������.
	long TimerTick = 0;

	float ShadowTick_1;
	float ShadowTick_2;

	int tick_spawn;

	bool Gameover_1 = false;
	bool Gameover_2 = false;

	JPoint Player1Center;
	JPoint Player2Center;

	bool isStart;
	
	CRankMng rankMng;

	HERO	Player1HeroKind;
	HERO	Player2HeroKind;
	
	char* ServerIP;
};

static int GetPosInValX(int x) { return ((x - PTStartX) / BLOCK_SIZE); }
static int GetPosInValY(int y) { return ((y - PTStartY) / BLOCK_SIZE); }

void err_display(const char* msg);