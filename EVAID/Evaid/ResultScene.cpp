#include "ResultScene.h"
#include "Framework.h"
#include "ResorceTable.h"

CResultScene::CResultScene()
{
	m_Closing_Counter = 500;
}

CResultScene::~CResultScene()
{
}

void CResultScene::update(long TimerTick)
{
	m_Closing_Counter--;
	if (m_Closing_Counter < 0)
	{
		m_Framework->AddScene(eSCENE::SCENE_LOBBY);
		m_Framework->PopScene();
	}
}

void CResultScene::draw(HDC hDC)
{
	RECT rc = m_Framework->getClientRect();

	// 배경
	ResorceTable::getInstance()->img_lobby_bg.Draw(hDC, 0, 0);

	// 승리 플레이어
	{
		HFONT hFont = CreateFont(100, 0, 0, 0, 0, 0, 0, 0
			, DEFAULT_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN
			, TEXT("ArcadeClassic"));

		HFONT oldFont = (HFONT)SelectObject(hDC, hFont);

		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, RGB(255, 255, 255));

		RECT textRect;
		textRect.left = 100;
		textRect.right = 1500;
		textRect.top = 300;
		textRect.bottom = 400;

		if(m_Framework->m_WinnerNum == 0)
			DrawText(hDC, L"Player1 Win!", lstrlen(L"Player1 Win!"), &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		else
			DrawText(hDC, L"Player2 Win!", lstrlen(L"Player2 Win!"), &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		SelectObject(hDC, oldFont);
		DeleteObject(hFont);
	}
}

bool CResultScene::init(CFramework* pFramework, HWND hWnd)
{
	if (!CScene::init(pFramework, hWnd)) return false;
	cout << "RESULT SCENE" << endl;
	return true;
}

bool CResultScene::Keyboard(UINT msg, WPARAM w, LPARAM l)
{
	switch (msg) {
	case WM_KEYUP:
		switch (w) {
		}
		break;
	case WM_KEYDOWN:
		switch (w) {
		}
		break;
	}
	return true;
}

bool CResultScene::Mouse(UINT msg, WPARAM w, LPARAM l)
{
	switch (msg) {
	case WM_LBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONUP:
		break;
	}
	return true;
}

void CResultScene::ReleaseObjects() { }