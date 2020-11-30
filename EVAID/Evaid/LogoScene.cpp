#include "Global.h"
#include "ResorceTable.h"
#include "Framework.h"
#include "logoScene.h"

CLogoScene::CLogoScene() {}
CLogoScene::~CLogoScene() {}
void CLogoScene::update(long TimerTick) { }

void CLogoScene::draw(HDC hDC) {
	RECT rc = m_Framework->getClientRect();

	ResorceTable::getInstance()->img_logo_bg.Draw(hDC, 0, 0);
}

bool CLogoScene::init(CFramework* pFramework, HWND hWnd) {
	if (!CScene::init(pFramework, hWnd)) return false;
	cout << "LOGO SCENE" << endl;
	return true;
}

bool CLogoScene::Keyboard(UINT msg, WPARAM w, LPARAM l) {
	return true;
}

bool CLogoScene::Mouse(UINT msg, WPARAM w, LPARAM l) {
	switch (msg) {
	case WM_LBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONUP:
		POINT M;
		M.x = LOWORD(l);
		M.y = HIWORD(l);
		if (!(M.x > 700 && M.x < 1100)) return false;
		//start btn
		if (M.y > 126 && M.y < 234) {
			m_Framework->AddScene(eSCENE::SCENE_LOBBY);
			m_Framework->PopScene();
			return true;
		}
		//rank btn
		if (M.y > 306 && M.y < 414) return true;
		//help btn
		if (M.y > 486 && M.y < 594) return true;
		if (M.y > 666 && M.y < 774) {
			PostQuitMessage(0);
			return true; // 영원히 호출 안됨
		}
		break;
	}
}

void CLogoScene::ReleaseObjects() {}