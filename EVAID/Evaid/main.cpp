#include "MISC.h"
#include "ResorceTable.h"
#include "Global.h"
#include "Table.h"
#include "Hero.h"
#include "resource.h"
#include "Framework.h"

HINSTANCE g_hInst;						// ���� �ν��Ͻ�
LPCTSTR lpszClass = TEXT("EVAID CLASS");
LPCTSTR lpszTitle = TEXT("EVAID TITLE");

CFramework framework;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void CALLBACK TimerProc(HWND hWnd, UINT iMessage, UINT_PTR idEvent, DWORD dwTime);

// ��Ʈ�Լ�
vector<BYTE> ReadFontOutputFile(LPCTSTR path)
{
	ifstream fFont(path, ios::in | ios::binary);

	if (!fFont.is_open()) {
		MessageBox(NULL, TEXT("Font Load Fail!"), TEXT("���α׷� ���� ����"), MB_OK);
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

CTable board_1(1);

CHero Hero1P(Global::getInstance()->Player1HeroKind, 1);

int Hero1Score = 0;

CImage doublebuffer;
CImage doublebuffer_Stage_1;

bool ShakeWindow = true;

HWND Stage_1_hWnd;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	DWORD dwStyle =
		WS_OVERLAPPED 		// ����Ʈ ������. Ÿ��Ʋ �ٿ� ũ�� ������ �ȵǴ� ��輱�� ������. �ƹ��� ��Ÿ�ϵ� ���� ������ �� ��Ÿ���� ����ȴ�.
		| WS_CAPTION 		// Ÿ��Ʋ �ٸ� ���� �����츦 ����� WS_BORDER ��Ÿ���� �����Ѵ�.
		| WS_SYSMENU		// �ý��� �޴��� ���� �����츦 �����.
		| WS_MINIMIZEBOX	// �ּ�ȭ ��ư�� �����.
		| WS_BORDER			// �ܼ����� �� ��輱�� ����� ũ�� ������ �� �� ����.
//		| WS_THICKFRAME		// ũ�� ������ ������ �β��� ��輱�� ������. WS_BORDER�� ���� ����� �� ����.
		;					// �߰��� �ʿ��� ������ ��Ÿ���� http://www.soen.kr/lecture/win32api/reference/Function/CreateWindow.htm ����.

	RECT getWinSize;
	GetWindowRect(GetDesktopWindow(), &getWinSize);

	RECT clientSize;
	clientSize.left = clientSize.top = 0;
	clientSize.right = CLIENT_WIDTH;
	clientSize.bottom = CLIENT_HEIGHT;
	AdjustWindowRect(&clientSize, dwStyle, FALSE);

	//Ŭ���� ������ǥ (������� �߾ӿ� ��ġ)
	POINT ptClientWorld;
	ptClientWorld.x = (getWinSize.right - clientSize.right) / 2;
	ptClientWorld.y = (getWinSize.bottom - clientSize.bottom) / 2;

	HWND hWnd = CreateWindow(
		lpszClass							// ������ Ŭ���� ��
		, lpszTitle							// ĸ�� ǥ�� ���ڿ�
		, dwStyle							// ������ ��Ÿ��
		, ptClientWorld.x					// �θ� ������ ��� ������ ������ǥ x
		, ptClientWorld.y					// �θ� ������ ��� ������ ������ǥ y
		, clientSize.right - clientSize.left//	������ ������ : width
		, clientSize.bottom - clientSize.top//	������ ������ : height
		, NULL								// �θ� ������
		, (HMENU)NULL						// �޴� �ڵ�
		, hInstance							// �ν��Ͻ� �ڵ�
		, NULL								// �߰� �Ķ����
	);
	if (!hWnd) return false;

	//������ ǥ��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	framework.init(hWnd);

	return true;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style =
		CS_HREDRAW		// Ŭ���̾�Ʈ�� �ʺ� ����Ǹ�, ��ü �����츦 �ٽ� �׸�
		| CS_VREDRAW	// Ŭ���̾�Ʈ�� ���̰� ����Ǹ�, ��ü �����츦 �ٽ� �׸�
//		| CS_DBLCLKS	// ����Ŭ���� ����ϱ� ���ؼ� �߰���
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
#pragma region �ʱ�ȭ
	case WM_CREATE:
		//framework.init(hWnd);
		Global::getInstance()->rankMng.load();
		ShakeWindow = true;
		// ��Ʈ ����
		{
			static auto vFont = ReadFontOutputFile(L"Font/ARCADECLASSIC.TTF");
			static DWORD fi;
			AddFontMemResourceEx(&(vFont[0]), vFont.size(), 0, &fi);
		}
		// �����
		ResorceTable::getInstance()->m_Sound.PlayBGM(0);
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
#pragma region �׸���
	case WM_PAINT:
	{
		HDC hdc = BeginPaint(hWnd, &ps);
		framework.draw(hdc);
		EndPaint(hWnd, &ps);
	}
	break;
#pragma endregion

#pragma region �׸���
	case WM_SIZE:
	{
		framework.Resize();
		framework.CreateBackBuffer();
	}
	break;
#pragma endregion

#pragma region Ű���� I/0 ���ڴ� wParam
	case WM_KEYDOWN:
	case WM_KEYUP:
		framework.KeyBoard(iMessage, wParam, lParam);
		break;
	case WM_CHAR:		break;
#pragma endregion

#pragma region ���콺
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
		framework.Mouse(iMessage, wParam, lParam);
		break;
#pragma endregion

#pragma region Ÿ�̸�
	case WM_TIMER:
		switch (wParam) {
		case timer_Main:
			framework.run();
			break;
		}
		break;
#pragma endregion

#pragma region ���� �� ȣ��.		�����츦 �Ҹ��Ű�� ������ DestoryWindow(hWnd)
	case WM_DESTROY:
		framework.ReleaseObject();
		Global::getInstance()->rankMng.save(Hero1Score);
		PostQuitMessage(0);
		return 0;
#pragma endregion

	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}

	return 0;
}

void CALLBACK TimerProc(HWND hWnd, UINT iMessage, UINT_PTR idEvent, DWORD dwTime) {

	Global::getInstance()->TimerTick += 1;

	if (board_1.ShakeDegree < 480) board_1.ShakeDegree += 60;

	if (board_1.isShadowOn) {
		Global::getInstance()->ShadowTick_1 += 0.1f;
		if (Global::getInstance()->ShadowTick_1 > shadowDelay) {
			board_1.effect_shadow();
			Global::getInstance()->ShadowTick_1 = 0;
			board_1.isShadowOn = false;
		}
	}

	if (Global::getInstance()->TimerTick % Global::getInstance()->tick_spawn == 0
		|| Global::getInstance()->Gameover_1) {
		bool FullBoard_1 = true;
		for (int i = 0; i < table_WIDTH; ++i) {
			if (board_1.Val[i][0] != BLOCK_TYPE::STACK && board_1.Val[i][0] != BLOCK_TYPE::SHADOW) {
				FullBoard_1 = false;
				break;
			}
		}
		if (!FullBoard_1) {
			board_1.spawn(board_1.getRandomXPos(), static_cast<BLOCK_TYPE>(getRandom(Block_Type_Count)));
		}
	}

	if (Global::getInstance()->TimerTick % tick_block == 0) {
		board_1.drop();
	}

	Hero1P.move(board_1);

	InvalidateRect(Stage_1_hWnd, NULL, FALSE);
}