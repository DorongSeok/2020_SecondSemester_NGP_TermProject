#pragma once
#include "MISC.h"
#include "RankMng.h"

class Global {
private:
	static Global* inst;
	Global();
	~Global() {};
public:
	//Global::getInstance()-> 객체이름
	static Global* getInstance() {
		if (inst == nullptr) inst = new Global();
		return inst;
	}

	// tick_Main 간격으로 증가함.
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