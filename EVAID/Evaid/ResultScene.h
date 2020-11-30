#pragma once
#include "Scene.h"
#include "Global.h"

class CResultScene : public CScene {
public:
	CResultScene(int loser);
	~CResultScene() override;

	void update(long TimerTick) override;
	void draw(HDC hDC) override;
	bool init(CFramework* pFramework, HWND hWnd) override;
	bool Keyboard(UINT msg, WPARAM w, LPARAM l) override;
	bool Mouse(UINT msg, WPARAM w, LPARAM l) override;
	void ReleaseObjects() override;

private:
	int m_LooserNum;

	int m_Closing_Counter;
};