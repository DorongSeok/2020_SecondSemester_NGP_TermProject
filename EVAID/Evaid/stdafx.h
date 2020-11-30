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
constexpr auto CLIENT_WIDTH = 1200;		// 너비;
constexpr auto CLIENT_HEIGHT = 900;		// 높이

//child
constexpr auto STAGE_WIDTH = 600;
constexpr auto STAGE_HEIGHT = 900;

// 플레이어에 따른 x축 그리기 시작 좌표 격차(플레이어 1 = 0, 플레이어 2 = CLIENT_WIDTH / 2)
constexpr auto DrawGapX = CLIENT_WIDTH / 2;

// 표 시작 포인트
constexpr auto  PTStartX = 100;
constexpr auto  PTStartY = 50;

//timer
constexpr auto timer_Main = 1;
constexpr auto tick_Main = 10;		//(ms)
constexpr auto tick_skill = 5;

//Table Size setting
constexpr auto table_WIDTH = 10;
constexpr auto table_HEIGHT = 20;

//Block Size setting (정사각형이며 이미지 파일과 크기를 맞춰야함)
constexpr auto BLOCK_SIZE = 40;
constexpr auto HERO_SIZE = 40;

constexpr auto FIRST_PLAYER = 0;
constexpr auto SECOND_PLAYER = 1;

// shadow
#define shadowDelay (double)2

// 히어로 시작위치
const int HeroStartPosX = 300;
const int HeroStartPosY = 630;

// 히어로 떨어지는 속도
#define Gravity 9.8

// 스킬관련
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
//떨어지는 블록종류
#define Block_Type_Count 5

// 상태
enum class STATE {
	NORMAL
	, LEFT
	, RIGHT
};

// 히어로 종류
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