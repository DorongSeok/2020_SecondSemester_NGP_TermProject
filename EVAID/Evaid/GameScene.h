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


	void update() override;
	void draw(HDC hDC) override;
	bool init(CFramework* pFramework, HWND hWnd) override;
	bool Keyboard(UINT msg, WPARAM w, LPARAM l) override;
	bool Mouse(UINT msg, WPARAM w, LPARAM l) override;
	void ReleaseObjects() override;

	void DrawGameScene(HDC hDC);

private:
	int iMyPlayerNum = 2;

	int Hero1Score = 0;
	int Hero2Score = 0;

	CHero Hero1P{ HERO::Hero1, 1 };
	CHero Hero2P{ HERO::Hero2, 2 };

	CTable board_1{ 1 };
	CTable board_2{ 2 };
};