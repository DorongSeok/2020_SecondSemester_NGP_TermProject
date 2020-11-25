#include "ResorceTable.h"
#include "Global.h"
#include "Table.h"
#include "Hero.h"
#include "resource.h"
#include "Framework.h"

HINSTANCE g_hInst;						// 현재 인스턴스
LPCTSTR lpszClass = TEXT("EVAID CLASS");
LPCTSTR lpszTitle = TEXT("EVAID TITLE");

static CFramework framework;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

// 폰트함수
vector<BYTE> ReadFontOutputFile(LPCTSTR path)
{
	ifstream fFont(path, ios::in | ios::binary);

	if (!fFont.is_open()) {
		MessageBox(NULL, TEXT("Font Load Fail!"), TEXT("프로그램 구동 실패"), MB_OK);
		return vector<BYTE>();
	}

	// size check;
	fFont.seekg(0, fstream::end);
	size_t szData = fFont.tellg();
	fFont.seekg(0);

	vector<BYTE> vbFont;
	vbFont.resize(szData);

	fFont.read(reinterpret_cast<char*>(&(vbFont[0])), szData);
	fFont.close();

	return vbFont;
}

CImage doublebuffer;
CImage doublebuffer_Stage_1;

HWND Stage_hWnd;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	DWORD dwStyle =
		WS_OVERLAPPED 		// 디폴트 윈도우. 타이틀 바와 크기 조절이 안되는 경계선을 가진다. 아무런 스타일도 주지 않으면 이 스타일이 적용된다.
		| WS_CAPTION 		// 타이틀 바를 가진 윈도우를 만들며 WS_BORDER 스타일을 포함한다.
		| WS_SYSMENU		// 시스템 메뉴를 가진 윈도우를 만든다.
		| WS_MINIMIZEBOX	// 최소화 버튼을 만든다.
		| WS_BORDER			// 단선으로 된 경계선을 만들며 크기 조정은 할 수 없다.
//		| WS_THICKFRAME		// 크기 조정이 가능한 두꺼운 경계선을 가진다. WS_BORDER와 같이 사용할 수 없다.
		;					// 추가로 필요한 윈도우 스타일은 http://www.soen.kr/lecture/win32api/reference/Function/CreateWindow.htm 참고.

	RECT getWinSize;
	GetWindowRect(GetDesktopWindow(), &getWinSize);

	RECT clientSize;
	clientSize.left = clientSize.top = 0;
	clientSize.right = CLIENT_WIDTH;
	clientSize.bottom = CLIENT_HEIGHT;
	AdjustWindowRect(&clientSize, dwStyle, FALSE);

	//클라의 절대좌표 (모니터의 중앙에 위치)
	POINT ptClientWorld;
	ptClientWorld.x = (getWinSize.right - clientSize.right) / 2;
	ptClientWorld.y = (getWinSize.bottom - clientSize.bottom) / 2;

	HWND hWnd = CreateWindow(
		lpszClass							// 윈도우 클래스 명
		, lpszTitle							// 캡션 표시 문자열
		, dwStyle							// 윈도우 스타일
		, ptClientWorld.x					// 부모 윈도우 기반 윈도우 시작좌표 x
		, ptClientWorld.y					// 부모 윈도우 기반 윈도우 시작좌표 y
		, clientSize.right - clientSize.left//	윈도우 사이즈 : width
		, clientSize.bottom - clientSize.top//	윈도우 사이즈 : height
		, NULL								// 부모 윈도우
		, (HMENU)NULL						// 메뉴 핸들
		, hInstance							// 인스턴스 핸들
		, NULL								// 추가 파라메터
	);
	if (!hWnd) return false;

	//윈도우 표시
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	framework.init(hWnd);

	return true;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style =
		CS_HREDRAW		// 클라이언트의 너비가 변경되면, 전체 윈도우를 다시 그림
		| CS_VREDRAW	// 클라이언트의 높이가 변경되면, 전체 윈도우를 다시 그림
//		| CS_DBLCLKS	// 더블클릭을 사용하기 위해서 추가함
;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;

	WndClass.hIcon = LoadIcon(g_hInst, (LPCTSTR)IDI_ICON1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = lpszTitle;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(g_hInst, (LPCTSTR)IDI_ICON1);
	return RegisterClassEx(&WndClass);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	srand(static_cast<unsigned>(time(NULL)));
	g_hInst = hInstance;
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow)) return false;
	HACCEL hAccelTable = LoadAccelerators(hInstance, lpszTitle);
	
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hDC;

	switch (iMessage) {
#pragma region 초기화
	case WM_CREATE:
		//framework.init(hWnd);
		Global::getInstance()->rankMng.load();
		// 폰트 설정
		{
			static auto vFont = ReadFontOutputFile(L"Font/ARCADECLASSIC.TTF");
			static DWORD fi;
			AddFontMemResourceEx(&(vFont[0]), vFont.size(), 0, &fi);
		}
		// 배경음
		ResorceTable::getInstance()->m_Sound.PlayBGM(0);
		
		// 타이머
		SetTimer(hWnd, 1, 16, NULL);
		Global::getInstance()->TimerTick = 0;
		break;
#pragma endregion
	//case WM_COMMAND:
	//{
	//	switch (LOWORD(wParam)) {
	//	case ID_PLAYER01_1:
	//		Global::getInstance()->Player1HeroKind = HERO::Hero1;
	//		break;
	//	case ID_PLAYER01_2:
	//		Global::getInstance()->Player1HeroKind = HERO::Hero2;
	//		break;
	//	case ID_PLAYER02_1:
	//		Global::getInstance()->Player2HeroKind = HERO::Hero1;
	//		break;
	//	case ID_PLAYER02_2:
	//		Global::getInstance()->Player2HeroKind = HERO::Hero2;
	//		break;
	//	}
	//}
#pragma region 그리기
	case WM_PAINT:
	{
		HDC hdc = BeginPaint(hWnd, &ps);
		framework.draw(hdc);
		EndPaint(hWnd, &ps);
	}
	break;
#pragma endregion

#pragma region 그리기
	case WM_SIZE:
	{
		framework.Resize();
		framework.CreateBackBuffer();
	}
	break;
#pragma endregion

#pragma region 키보드 I/0 인자는 wParam
	case WM_KEYDOWN:
	case WM_KEYUP:
		framework.KeyBoard(iMessage, wParam, lParam);
		break;
	case WM_CHAR:		break;
#pragma endregion

#pragma region 마우스
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
		framework.Mouse(iMessage, wParam, lParam);
		break;
#pragma endregion

#pragma region 타이머
	case WM_TIMER:
		switch (wParam) {
		case timer_Main:
			Global::getInstance()->TimerTick += 1;
			framework.run();
			break;
		}
		break;
#pragma endregion

#pragma region 종료 시 호출.		윈도우를 소멸시키고 싶으면 DestoryWindow(hWnd)
	case WM_DESTROY:
		framework.ReleaseObject();
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
#pragma endregion

	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}

	return 0;
}