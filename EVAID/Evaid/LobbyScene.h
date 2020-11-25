#pragma once
#include "Scene.h"

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


	int iMyPlayerNum = 1;				// 내가 몇번째 플레이어인지

	bool bImConnected = false;
	bool bImReady = false;

	bool bPlayer1Connected = false;
	bool bPlayer2Connected = false;
	bool bPlayer1Ready = false;
	bool bPlayer2Ready = false;
};