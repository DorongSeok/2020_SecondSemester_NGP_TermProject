#include "Global.h"


Global::Global() {
	ShadowTick_1 = 0;
	ShadowTick_2 = 0;
	tick_spawn = 250;
	Gameover_1 = false;
	Gameover_2 = false;
	isStart = false;
	Player1HeroKind = HERO::Hero1;
	Player2HeroKind = HERO::Hero1;
	iMyPlayerNum = 1;
}

Global* Global::inst = nullptr;

void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}