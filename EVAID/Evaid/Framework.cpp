#include "Global.h"
#include "Framework.h"
#include "logoScene.h"
#include "LobbyScene.h"
#include "GameScene.h"

CFramework::CFramework() {
	if (WSAStartup(MAKEWORD(2, 2), &wsa)) cout << "wsa error" << endl;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) cout << "sock error" << endl;
}

void CFramework::update() {
	if (m_pCurrentScene) m_pCurrentScene->update();
}

void CFramework::draw(HDC hDC) {
	clear();
	ReleaseBackBuffer();
	CreateBackBuffer();
	if (m_pCurrentScene) {
		m_pCurrentScene->draw(m_hDCBackBuffer);
	}
	DoubleBuffering(hDC);
}

void CFramework::run() {
	update();
	InvalidateRect(m_hWnd, &m_rcClient, false);
}

bool CFramework::init(HWND hWnd) {
	m_hWnd = hWnd;
	::GetClientRect(hWnd, &m_rcClient);
	SetTimer(hWnd, timer_Main, FPS, NULL);

	return AddScene(hWnd);
}

bool CFramework::AddScene(const HWND& hWnd) {
	//////////////////////////////////////////////
	auto pGame = new CGameScene{};
	if (!pGame->init(this, hWnd)) {
		delete pGame;
		return false;
	}
	m_Scenes[m_nCurrentScene++] = pGame;
	m_pCurrentScene = pGame;
	//////////////////////////////////////////////
	auto pLobby = new CLobbyScene{};
	if (!pLobby->init(this, hWnd)) {
		delete pLobby;
		return false;
	}
	m_Scenes[m_nCurrentScene++] = pLobby;
	m_pCurrentScene = pLobby;
	////////////////////////////////////////////////
	//auto pLogo = new CLogoScene{};
	//if (!pLogo->init(this, hWnd)) {
	//	delete pLogo;
	//	return false;
	//}
	//m_Scenes[m_nCurrentScene++] = pLogo;
	//m_pCurrentScene = pLogo;
	m_nCurrentScene--;
	return true;
}

void CFramework::PopScene() {
	auto pScene = m_Scenes[m_nCurrentScene];
	m_Scenes[m_nCurrentScene] = nullptr;
	m_pCurrentScene = m_Scenes[--m_nCurrentScene];
	delete pScene;
}

void CFramework::ReleaseObject() {
	KillTimer(m_hWnd, timer_Main);
	for (auto& p : m_Scenes) if (p != nullptr) p->ReleaseObjects();
	ReleaseBackBuffer();
}

bool CFramework::Mouse(UINT msg, WPARAM w, LPARAM l) {
	if (m_pCurrentScene && m_pCurrentScene->Mouse(msg, w, l)) return true;
}

bool CFramework::KeyBoard(UINT msg, WPARAM w, LPARAM l) {
	if (m_pCurrentScene && m_pCurrentScene->Keyboard(msg, w, l))return true;

	switch (msg) {
	case WM_KEYDOWN:
		switch (w) {
		case VK_ESCAPE:
			DestroyWindow(m_hWnd);
			break;
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
		break;
	}
	return TRUE;
}

void CFramework::CreateBackBuffer() {
	ReleaseBackBuffer();

	m_hDCBackBuffer = CreateCompatibleDC(nullptr);
	HDC hDC = GetDC(m_hWnd);
	m_hbmpBackBuffer = CreateCompatibleBitmap(hDC, m_rcClient.right, m_rcClient.bottom);
	SelectObject(m_hDCBackBuffer, m_hbmpBackBuffer);

	ReleaseDC(m_hWnd, hDC);
}

void CFramework::ReleaseBackBuffer() {
	SafeRelease(m_hbmpBackBuffer);
	SafeRelease(m_hDCBackBuffer);
}

void CFramework::DoubleBuffering(HDC hDC) {
	::BitBlt(hDC,
		m_rcClient.left, m_rcClient.top,
		m_rcClient.right - m_rcClient.left,
		m_rcClient.bottom - m_rcClient.top,
		m_hDCBackBuffer, m_rcClient.left, m_rcClient.top,
		SRCCOPY);
}

void CFramework::clear() {
	SetDCBrushColor(m_hDCBackBuffer, m_hColorBackground);
	FillRect(m_hDCBackBuffer, &m_rcClient, static_cast<HBRUSH>(GetStockObject(DC_BRUSH)));
}
//
//int CFramework::recvn(SOCKET s, char* buf, int len, int flags)
//{
//	int received;
//	char* ptr = buf;
//	int left = len;
//
//	while (left > 0) {
//		received = recv(s, ptr, left, flags);
//		if (received == SOCKET_ERROR)
//			return SOCKET_ERROR;
//		else if (received == 0)
//			break;
//		left -= received;
//		ptr += received;
//	}
//
//	return (len - left);
//
//}
//
//void CFramework::err_display(const char* msg)
//{
//	LPVOID lpMsgBuf;
//	FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//		NULL, WSAGetLastError(),
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPTSTR)&lpMsgBuf, 0, NULL);
//	printf("[%s] %s", msg, (char*)lpMsgBuf);
//	LocalFree(lpMsgBuf);
//}
//