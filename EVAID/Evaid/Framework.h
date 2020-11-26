#pragma once
#include "Global.h"
#include "Scene.h"

class CFramework {
public:
	CFramework();
	~CFramework() = default;

	void update();
	void draw(HDC hDC);
	void CreateBackBuffer();
	void ReleaseBackBuffer();
	void DoubleBuffering(HDC hDC);
	void clear();
	void Resize() { ::GetClientRect(m_hWnd, &m_rcClient); }
	void run();
	bool AddScene(const HWND& hWnd);
	void PopScene();
	bool init(HWND hWnd);

	void ReleaseObject();
	bool Mouse(UINT msg, WPARAM wParam, LPARAM lParam);
	bool KeyBoard(UINT msg, WPARAM wParam, LPARAM lParam);
	RECT getClientRect() const { return m_rcClient; }

public:
	WSADATA wsa;
	SOCKET s;
	sockaddr_in addr;
	int addrlen;

private:
	HWND m_hWnd{ NULL };
	RECT m_rcClient{ NULL };

	CScene* m_Scenes[3];
	int m_nCurrentScene{ 0 };
	CScene* m_pCurrentScene{ nullptr };
	HDC m_hDCBackBuffer{ nullptr };
	HBITMAP m_hbmpBackBuffer{ nullptr };
	COLORREF m_hColorBackground{ RGB(255,255,255) };
};