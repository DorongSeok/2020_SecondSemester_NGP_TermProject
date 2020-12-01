#pragma once
#include "stdafx.h"
#include "Scene.h"

class CFramework {
public:
	CFramework();
	~CFramework() = default;

	void update(long TimerTick);
	void draw(HDC hDC);
	void CreateBackBuffer();
	void ReleaseBackBuffer();
	void DoubleBuffering(HDC hDC);
	void clear();
	void Resize() { ::GetClientRect(m_hWnd, &m_rcClient); }
	void run(long TimerTick);
	bool AddScene(eSCENE scene);
	void PopScene();
	bool init(HWND hWnd);

	void ReleaseObject();
	bool Mouse(UINT msg, WPARAM wParam, LPARAM lParam);
	bool KeyBoard(UINT msg, WPARAM wParam, LPARAM lParam);
	RECT getClientRect() const { return m_rcClient; }
	int recvn(SOCKET s, char* buf, int len, int flags);

public:
	WSADATA wsa;
	SOCKET s;
	sockaddr_in addr;
	int addrlen;

	int m_WinnerNum;
	ePlayer PlayerNum;
	JPoint Player1Center;
	JPoint Player2Center;
	
	bool FirstConnect;

	char ServerIP[30];

	bool GameOver_1;
	bool GameOver_2;
private:
	HWND m_hWnd{ NULL };
	RECT m_rcClient{ NULL };

	CScene* m_pNextScene{ nullptr };
	CScene* m_pCurrentScene{ nullptr };
	HDC m_hDCBackBuffer{ nullptr };
	HBITMAP m_hbmpBackBuffer{ nullptr };
	COLORREF m_hColorBackground{ RGB(255,255,255) };

};