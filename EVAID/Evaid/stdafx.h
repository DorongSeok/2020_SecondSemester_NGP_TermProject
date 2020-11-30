#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <commctrl.h>
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

//client
constexpr auto CLIENT_WIDTH = 1200;		// �ʺ�;
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

constexpr auto FIRST_PLAYER = 0;
constexpr auto SECOND_PLAYER = 1;

// shadow
#define shadowDelay (double)2

// ����� ������ġ
const int HeroStartPosX = 300;
const int HeroStartPosY = 630;

// ����� �������� �ӵ�
#define Gravity 9.8

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
		x = NULLVAL;
		y = NULLVAL;
	}
	JPoint(int _x, int _y) : x(_x), y(_y) {};
	~JPoint() {};
};

//-----------------------------------------------
//�������� �������
#define Block_Type_Count 5

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
//---------------------------------------------

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

static int GetPosInValX(int x) { return ((x - PTStartX) / BLOCK_SIZE); }
static int GetPosInValY(int y) { return ((y - PTStartY) / BLOCK_SIZE); }