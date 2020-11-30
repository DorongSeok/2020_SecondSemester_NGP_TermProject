#pragma once
#include "Scene.h"
#include "Hero.h"
#include "Global.h"
#include "Table.h"

class CHero;
class CTable;

class CGameScene : public CScene {
public:
	CGameScene();
	~CGameScene() override;


	void update(long TimerTick) override;
	void draw(HDC hDC) override;
	bool init(CFramework* pFramework, HWND hWnd) override;
	bool Keyboard(UINT msg, WPARAM w, LPARAM l) override;
	bool Mouse(UINT msg, WPARAM w, LPARAM l) override;
	void ReleaseObjects() override;

	long m_TimerTick = 0;

private:
	int Hero1Score = 0;
	int Hero2Score = 0;

	CHero Hero1P{ HERO::Hero1, 0 };
	CHero Hero2P{ HERO::Hero2, 1 };

	CTable board_1{ ePlayer::PLAYER_FIRST };
	CTable board_2{ ePlayer::PLAYER_SECOND };
};