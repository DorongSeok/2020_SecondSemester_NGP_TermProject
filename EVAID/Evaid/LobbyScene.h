#pragma once
#include "Scene.h"

class CLobbyScene : public CScene {
public:
	CLobbyScene();
	~CLobbyScene() override;

	void update(long TimerTick) override;
	void draw(HDC hDC) override;
	bool init(CFramework* pFramework, HWND hWnd) override;
	bool Keyboard(UINT msg, WPARAM w, LPARAM l) override;
	bool Mouse(UINT msg, WPARAM w, LPARAM l) override;
	void ReleaseObjects() override;

private:
	char input[30];

	bool bConnected = false;
	bool bReady = false;

	bool bPlayerConnected[(int)ePlayer::PLAYER_MAX];
	bool bPlayerReady[(int)ePlayer::PLAYER_MAX];
};