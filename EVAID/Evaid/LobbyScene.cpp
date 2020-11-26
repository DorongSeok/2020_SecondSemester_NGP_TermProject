#include "LobbyScene.h"
#include "Framework.h"
#include "Global.h"
#include "ResorceTable.h"

CLobbyScene::CLobbyScene() 
{
	for (int i = 0; i < ePlayer::pMAX; ++i) {
		bPlayerConnected[i] = false;
		bPlayerReady[i] = false;
	}
}

CLobbyScene::~CLobbyScene() {}

void CLobbyScene::update() {
	SOCKET& s = m_Framework->s;
	sockaddr_in& addr = m_Framework->addr;
	int retval;
	char buffer[MAXBUFFER];

	if (bConnected) {
		ZeroMemory(buffer, sizeof(buffer));
		retval = recv(s, buffer, sizeof(buffer), 0);
		switch (buffer[1]) {
		case sc_login:
			cout << "get login packet" << endl;
			break;
		case sc_ready:
			cout << "get ready packet" << endl;
			break;
		}
		if (buffer[1] == sc_login && 
			bPlayerConnected[ePlayer::pFIRST] == false || bPlayerConnected[ePlayer::pSECOND] == false) {
			sc_packet_login scpl;
			memcpy(&scpl, buffer, sizeof(scpl));
			if (retval != 0 && retval != -1) {
				bPlayerConnected[ePlayer::pFIRST] = scpl.f_login;
				bPlayerConnected[ePlayer::pSECOND] = scpl.s_login;
			}
		}
		if (buffer[1] == sc_ready) {
			sc_packet_ready scpr;
			memcpy(&scpr, buffer, sizeof(scpr));
			if (retval != 0 && retval != -1) {
				bPlayerReady[scpr.id] = true;
				if (scpr.id == ePlayer::pMAX) {
					bPlayerReady[0] = true;
					bPlayerReady[1] = true;
					cout << "!!" << endl;
				}
			}
		}
	}
}


void CLobbyScene::draw(HDC hDC)
{
	// backgroud----------------
	ResorceTable::getInstance()->img_lobby_bg.Draw(hDC, 0, 0);
	// -------------------------

	// player border------------
	if (bPlayerReady[ePlayer::pFIRST])					ResorceTable::getInstance()->img_lobby_hero1_ready.Draw(hDC, 85, 50);
	else if(bPlayerConnected[ePlayer::pFIRST])			ResorceTable::getInstance()->img_lobby_hero1_unready.Draw(hDC, 85, 50);
	else												ResorceTable::getInstance()->img_lobby_hero_border.Draw(hDC, 85, 50);

	if (bPlayerReady[ePlayer::pSECOND])					ResorceTable::getInstance()->img_lobby_hero2_ready.Draw(hDC, 641, 50);
	else if (bPlayerConnected[ePlayer::pSECOND])			ResorceTable::getInstance()->img_lobby_hero2_unready.Draw(hDC, 641, 50);
	else												ResorceTable::getInstance()->img_lobby_hero_border.Draw(hDC, 641, 50);
	// -------------------------

	// button-------------------
	if (bConnected && bReady)		ResorceTable::getInstance()->img_lobby_ready_disabled_bnt.Draw(hDC, 676, 753);
	else if(bConnected && !bReady)	ResorceTable::getInstance()->img_lobby_ready_bnt.Draw(hDC, 676, 753);
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
		case VK_F1:
			strcat(input, "127.0.0.1");
			break;
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
			SOCKET& s = m_Framework->s;
			sockaddr_in& addr = m_Framework->addr;
			int retval;
			if (!bConnected)
			{
				ZeroMemory(&addr, sizeof(addr));
				addr.sin_family = AF_INET;
				addr.sin_addr.S_un.S_addr = inet_addr(input);
				addr.sin_port = htons(SERVERPORT);
				retval = connect(s, (sockaddr*)&addr, sizeof(addr));
				if (retval == SOCKET_ERROR) cout << "connect error" << endl;

				cs_packet_login cspl;
				ZeroMemory(&cspl, sizeof(cspl));
				cspl.size = sizeof(cspl);
				cspl.type = cs_login;
				retval = send(s, reinterpret_cast<char*>(&cspl), cspl.size, 0);

				sc_packet_login scpl;
				ZeroMemory(&scpl, sizeof(scpl));
				retval = recv(s, reinterpret_cast<char*>(&scpl), sizeof(scpl), 0);

				iPlayerNum = static_cast<int>(scpl.id);
				bPlayerConnected[ePlayer::pFIRST] = scpl.f_login;
				bPlayerConnected[ePlayer::pSECOND] = scpl.s_login;
				bConnected = true;
				cout << "id: " << iPlayerNum << endl;
				cout << "Connected" << endl;
				cout << endl << "IP: " << input << endl;
				this->draw(GetDC(m_hWnd));
				break;
			}
			if (!bReady)
			{
				cs_packet_ready cspr;
				ZeroMemory(&cspr, sizeof(cspr));
				cspr.size = sizeof(cspr);
				cspr.type = cs_ready;
				cspr.id = iPlayerNum;
				retval = send(s, reinterpret_cast<char*>(&cspr), cspr.size, 0);
				bPlayerReady[iPlayerNum] = true;
				bReady = true;
				cout << "Ready" << endl;
				break;
			}
			return true;
		}
		break;
	}
	return true;
}

void CLobbyScene::ReleaseObjects() {}