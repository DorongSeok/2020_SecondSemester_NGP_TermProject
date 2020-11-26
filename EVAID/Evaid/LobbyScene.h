#pragma once
#include "Scene.h"

enum ePlayer { pFIRST = 0, pSECOND = 1, pMAX = 2 };
class CLobbyScene : public CScene {
public:
	CLobbyScene();
	~CLobbyScene() override;

	void update() override;
	void draw(HDC hDC) override;
	bool init(CFramework* pFramework, HWND hWnd) override;
	bool Keyboard(UINT msg, WPARAM w, LPARAM l) override;
	bool Mouse(UINT msg, WPARAM w, LPARAM l) override;
	void ReleaseObjects() override;

private:
	char input[30];


	int iPlayerNum = 1;				// 내가 몇번째 플레이어인지

	bool bConnected = false;
	bool bReady = false;

	bool bPlayerConnected[ePlayer::pMAX];
	bool bPlayerReady[ePlayer::pMAX];
};