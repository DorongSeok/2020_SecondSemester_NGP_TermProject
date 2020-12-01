#pragma once
#include "Scene.h"
#include "Hero.h"
#include "stdafx.h"
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

	void DrawGameScene(HDC hDC);

	long m_TimerTick = 0;

private:
	int Hero1Score = 0;
	int Hero2Score = 0;

	CHero Hero1P{ HERO::Hero1, 0, m_Framework };
	CHero Hero2P{ HERO::Hero2, 1, m_Framework};

	CTable board_1{ 0, m_Framework };
	CTable board_2{ 1, m_Framework };
};