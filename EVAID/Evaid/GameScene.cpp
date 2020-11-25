#include "GameScene.h"
#include "Framework.h"
#include "ResorceTable.h"
#include "Table.h"

CGameScene::CGameScene()
{
	Hero1Score = 0;
	Hero2Score = 0;

	Global::getInstance()->isStart = true;
}

CGameScene::~CGameScene()
{
}

void CGameScene::update()
{
	if (board_1.isShadowOn)
	{
		Global::getInstance()->ShadowTick_1 += 0.1f;
		if (Global::getInstance()->ShadowTick_1 > shadowDelay)
		{
			board_1.effect_shadow();
			Global::getInstance()->ShadowTick_1 = 0;
			board_1.isShadowOn = false;
		}
	}
	if (board_2.isShadowOn)
	{
		Global::getInstance()->ShadowTick_2 += 0.1f;
		if (Global::getInstance()->ShadowTick_2 > shadowDelay)
		{
			board_2.effect_shadow();
			Global::getInstance()->ShadowTick_2 = 0;
			board_2.isShadowOn = false;
		}
	}

	if (Global::getInstance()->Gameover_1)
	{
		bool FullBoard_1 = true;
		for (int i = 0; i < table_WIDTH; ++i)
		{
			if (board_1.Val[i][0] != BLOCK_TYPE::STACK && board_1.Val[i][0] != BLOCK_TYPE::SHADOW)
			{
				FullBoard_1 = false;
				break;
			}
		}
		if (!FullBoard_1)
		{
			board_1.spawn(board_1.getRandomXPos(), static_cast<BLOCK_TYPE>(getRandom(Block_Type_Count)));
		}
	}
	if (Global::getInstance()->Gameover_2)
	{
		bool FullBoard_2 = true;
		for (int i = 0; i < table_WIDTH; ++i)
		{
			if (board_2.Val[i][0] != BLOCK_TYPE::STACK && board_2.Val[i][0] != BLOCK_TYPE::SHADOW)
			{
				FullBoard_2 = false;
				break;
			}
		}
		if (!FullBoard_2)
		{
			board_2.spawn(board_2.getRandomXPos(), static_cast<BLOCK_TYPE>(getRandom(Block_Type_Count)));
		}
	}

	if (Global::getInstance()->TimerTick % tick_block == 0)
	{
		board_1.drop();
		board_2.drop();
	}

	if (Global::getInstance()->TimerTick % tick_skill == 0)
	{
		Hero1P.skillGaugeUp(Hero2P);
		Hero2P.skillGaugeUp(Hero1P);
	}

	Hero1P.move(board_1);
	Hero2P.move(board_2);
}

void CGameScene::draw(HDC hDC)
{
	DrawGameScene(hDC);
}

bool CGameScene::init(CFramework* pFramework, HWND hWnd)
{
	if (!CScene::init(pFramework, hWnd)) return false;
	cout << "GAME SCENE" << endl;
	return true;
}

bool CGameScene::Keyboard(UINT msg, WPARAM w, LPARAM l)
{
	switch (msg) {
	case WM_KEYUP:
		switch (w) {
			// P1ayer1
		case 'A':
			if (Hero1P.state == STATE::LEFT) Hero1P.state = STATE::NORMAL;
			break;
		case 'S':
			if (Hero1P.state == STATE::RIGHT) Hero1P.state = STATE::NORMAL;
			break;

			// P1ayer2
		case 'H':
			if (Hero2P.state == STATE::LEFT) Hero2P.state = STATE::NORMAL;
			break;
		case 'J':
			if (Hero2P.state == STATE::RIGHT) Hero2P.state = STATE::NORMAL;
			break;
		}
	case WM_KEYDOWN:
		switch (w) {
			// Player1
		case 'A':
			Hero1P.state = STATE::LEFT;
			break;
		case 'S':
			Hero1P.state = STATE::RIGHT;
			break;
		case 'D':
			Hero1P.herojump();
			break;
		case 'F':
			Hero1P.SkillOn(Hero2P);
			break;

			// Player2
		case 'H':
			Hero2P.state = STATE::LEFT;
			break;
		case 'J':
			Hero2P.state = STATE::RIGHT;
			break;
		case 'K':
			Hero2P.herojump();
			break;
		case 'L':
			Hero2P.SkillOn(Hero1P);
			break;
		}
	}
	return true;
}

bool CGameScene::Mouse(UINT msg, WPARAM w, LPARAM l)
{
	switch (msg) {
	case WM_LBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONUP:
		break;
	}
	return true;
}

void CGameScene::ReleaseObjects()
{
}

void CGameScene::DrawGameScene(HDC hDC)
{
	RECT temp{ 0,0,CLIENT_WIDTH,CLIENT_HEIGHT };
	FillRect(hDC, &temp, (HBRUSH)GetStockObject(WHITE_BRUSH));

	board_1.draw(hDC);
	board_2.draw(hDC);

	if (!Global::getInstance()->Gameover_1)	Hero1P.draw(hDC);
	if (!Global::getInstance()->Gameover_2)	Hero2P.draw(hDC);

	// 스코어 그리기
	{
		HFONT hFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0
			, DEFAULT_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN
			, TEXT("ArcadeClassic"));

		HFONT oldFont = (HFONT)SelectObject(hDC, hFont);

		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, RGB(255, 255, 255));

		TCHAR ScorePrint[100];
		RECT textRect;
		textRect.left = PTStartX;
		textRect.right = CLIENT_WIDTH / 2 - PTStartX;
		textRect.top = 0;
		textRect.bottom = PTStartY;

		if (!Global::getInstance()->Gameover_1)	Hero1Score = Global::getInstance()->TimerTick;
		if (!Global::getInstance()->Gameover_2)	Hero2Score = Global::getInstance()->TimerTick;

		// 출력할 텍스트 입력
		wsprintf(ScorePrint, L"Score: %d", Hero1Score);
		DrawText(hDC, ScorePrint, lstrlen(ScorePrint), &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		textRect.left += DrawGapX;
		textRect.right += DrawGapX;
		wsprintf(ScorePrint, L"Score: %d", Hero2Score);
		DrawText(hDC, ScorePrint, lstrlen(ScorePrint), &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		SelectObject(hDC, oldFont);
		DeleteObject(hFont);
	}

	// 게임오버 그리기
	{
		HFONT hFont = CreateFont(75, 0, 0, 0, 0, 0, 0, 0
			, DEFAULT_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN
			, TEXT("ArcadeClassic"));

		HFONT oldFont = (HFONT)SelectObject(hDC, hFont);

		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, RGB(255, 255, 255));

		RECT textRect;
		textRect.left = 20;
		textRect.right = CLIENT_WIDTH / 2;
		textRect.top = 0;
		textRect.bottom = CLIENT_HEIGHT;

		if (Global::getInstance()->Gameover_1)	DrawText(hDC, L"Game Over!", lstrlen(L"Game Over!"), &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		textRect.left += DrawGapX;
		textRect.right += DrawGapX;
		if (Global::getInstance()->Gameover_2)	DrawText(hDC, L"Game Over!", lstrlen(L"Game Over!"), &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		SelectObject(hDC, oldFont);
		DeleteObject(hFont);
	}
}