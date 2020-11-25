#include "LobbyScene.h"
#include "Framework.h"
#include "Global.h"
#include "ResorceTable.h"

CLobbyScene::CLobbyScene() 
{
}

CLobbyScene::~CLobbyScene() {}

void CLobbyScene::update() {

}


void CLobbyScene::draw(HDC hDC) 
{
	// backgroud----------------
	ResorceTable::getInstance()->img_lobby_bg.Draw(hDC, 0, 0);
	// -------------------------

	// player border------------
	if(bPlayer1Ready)					ResorceTable::getInstance()->img_lobby_hero1_ready.Draw(hDC, 85, 50);
	else if(bPlayer1Connected)			ResorceTable::getInstance()->img_lobby_hero1_unready.Draw(hDC, 85, 50);
	else								ResorceTable::getInstance()->img_lobby_hero_border.Draw(hDC, 85, 50);

	if (bPlayer2Ready)					ResorceTable::getInstance()->img_lobby_hero2_ready.Draw(hDC, 641, 50);
	else if (bPlayer2Connected)			ResorceTable::getInstance()->img_lobby_hero2_unready.Draw(hDC, 641, 50);
	else								ResorceTable::getInstance()->img_lobby_hero_border.Draw(hDC, 641, 50);
	// -------------------------

	// button-------------------
	if (bImConnected && bImReady)		ResorceTable::getInstance()->img_lobby_ready_disabled_bnt.Draw(hDC, 676, 753);
	else if(bImConnected && !bImReady)	ResorceTable::getInstance()->img_lobby_ready_bnt.Draw(hDC, 676, 753);
	else								ResorceTable::getInstance()->img_lobby_connect_bnt.Draw(hDC, 676, 753);
	// -------------------------
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
			break;
		}
		break;
	case WM_KEYDOWN:
		break;
	}
	return true;
}

bool CLobbyScene::Mouse(UINT msg, WPARAM w, LPARAM l) {
	switch (msg) {
	case WM_LBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONUP:
		POINT M;
		M.x = LOWORD(l);
		M.y = HIWORD(l);
		if (!(M.x > 676 && M.x < 1076)) return false;

		if (M.y > 753 && M.y < 861) 
		{
			if (!bImConnected)
			{
				if (iMyPlayerNum == 1)
					bPlayer1Connected = true;
				else
					bPlayer2Connected = true;

				bImConnected = true;
				cout << "Connected" << endl;

				cout << endl << "IP: " << input << endl;
				break;
			}
			if (!bImReady)
			{
				if (iMyPlayerNum == 1)
					bPlayer1Ready = true;
				else
					bPlayer2Ready = true;

				bImReady = true;
				cout << "Ready" << endl;
				break;
			}
			return true;
		}
		break;
	}
	return true;
}

void CLobbyScene::ReleaseObjects() {

}