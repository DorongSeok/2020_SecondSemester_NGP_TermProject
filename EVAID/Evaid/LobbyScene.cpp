#include "LobbyScene.h"
#include "Framework.h"

CLobbyScene::CLobbyScene() {}

CLobbyScene::~CLobbyScene() {}

void CLobbyScene::update() {

}

void CLobbyScene::draw(HDC hDC) {
}

bool CLobbyScene::init(CFramework* pFramework, HWND hWnd) {
	if (!CScene::init(pFramework, hWnd)) return false;
	cout << "LOBBY SCENE" << endl;
	memset(input, NULL, sizeof(input));
	return true;
}

bool CLobbyScene::Keyboard(UINT msg, WPARAM w, LPARAM l) {

	switch (msg) {
	case WM_KEYUP:
		if ('0' <= w && w <= '9')				// 키보드 숫자키
			cout << w - '0';
		else if (96 <= w && w <= 105)			// 오른쪽 숫자키
			cout << w - 96;
		else if (w == 190 || w == 110)			// '.' 키
			cout << '.';

		switch (w) {
		case '0':
		case 96:
			strcat(input, "0");
			break;
		case '1':
		case 97:
			strcat(input, "1");
			break;
		case '2':
		case 98:
			strcat(input, "2");
			break;
		case '3':
		case 99:
			strcat(input, "3");
			break;
		case '4':
		case 100:
			strcat(input, "4");
			break;
		case '5':
		case 101:
			strcat(input, "5");
			break;
		case '6':
		case 102:
			strcat(input, "6");
			break;
		case '7':
		case 103:
			strcat(input, "7");
			break;
		case '8':
		case 104:
			strcat(input, "8");
			break;
		case '9':
		case 105:
			strcat(input, "9");
			break;
		case 110:
		case 190:
			strcat(input, ".");
			break;
		case VK_RETURN:
			cout << endl << "IP: " << input << endl;
			m_Framework->PopScene();
			break;
		}
		break;
	case WM_KEYDOWN:
		break;
	}
	return true;
}

bool CLobbyScene::Mouse(UINT msg, WPARAM w, LPARAM l) {
	return true;
}

void CLobbyScene::ReleaseObjects() {

}