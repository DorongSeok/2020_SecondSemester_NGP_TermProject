#include "LobbyScene.h"

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
		switch (w) {
		case '0':
			strcat(input, "0");
			break;
		case '1':
			strcat(input, "1");
			break;
		case '2':
			strcat(input, "2");
			break;
		case '3':
			strcat(input, "3");
			break;
		case '4':
			strcat(input, "4");
			break;
		case '5':
			strcat(input, "5");
			break;
		case '6':
			strcat(input, "6");
			break;
		case '7':
			strcat(input, "7");
			break;
		case '8':
			strcat(input, "8");
			break;
		case '9':
			strcat(input, "9");
			break;
		case '.':
			strcat(input, ".");
			break;
		case VK_RETURN:
			cout << input << endl;
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