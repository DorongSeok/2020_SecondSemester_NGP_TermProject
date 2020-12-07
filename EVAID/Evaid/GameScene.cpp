#include "GameScene.h"
#include "Framework.h"
#include "ResorceTable.h"
#include "Table.h"

int tick_block = 4;

CGameScene::CGameScene() {
	Hero1Score = 0;
	Hero2Score = 0;

	m_GameEnd = false;
}

CGameScene::~CGameScene() {}

void CGameScene::update(long TimerTick) {
	Hero1P.setFramework(m_Framework);
	Hero2P.setFramework(m_Framework);
	board_1.setFramework(m_Framework);
	board_2.setFramework(m_Framework);
	SOCKET& s = m_Framework->s;
	sockaddr_in& addr = m_Framework->addr;
	int retval;

	cs_packet_user cspu;
	ZeroMemory(&cspu, sizeof(cspu));
	cspu.size = sizeof(cspu);
	cspu.type = cs_user;
	if (m_Framework->PlayerNum == ePlayer::PLAYER_FIRST) {
		Hero1P.GetHeroToPacket(&cspu);
		board_1.GetTableToPacket(&cspu);
	}
	else if (m_Framework->PlayerNum == ePlayer::PLAYER_SECOND) {
		Hero2P.GetHeroToPacket(&cspu);
		board_2.GetTableToPacket(&cspu);
	}
	if (m_Framework->GameOver_1)
	{
		m_GameEnd = true;
		m_Framework->m_WinnerNum = 1;
	}
	else if (m_Framework->GameOver_2)
	{
		m_GameEnd = true;
		m_Framework->m_WinnerNum = 0;
	}
	cspu.gameEnd = m_GameEnd;
	retval = send(s, reinterpret_cast<char*>(&cspu), sizeof(cspu), 0);


	sc_packet_user scpu;
	ZeroMemory(&scpu, sizeof(scpu));
	retval = recv(s, reinterpret_cast<char*>(&scpu), sizeof(scpu), 0);
	if (retval != 0) {
		Hero1P.SetPacketToHero(scpu);
		Hero2P.SetPacketToHero(scpu);

		board_1.SetPacketToTable(scpu);
		board_2.SetPacketToTable(scpu);
	}
	if (m_GameEnd)
	{
		m_Framework->AddScene(eSCENE::SCENE_RESULT);
		m_Framework->PopScene();
	}

	m_TimerTick++;
	Hero1P.SetTimer(m_TimerTick);
	Hero2P.SetTimer(m_TimerTick);


	cout << m_TimerTick << endl;
	if (m_TimerTick % 1500 == 0 && tick_block > 1) tick_block -= 1;

	if (board_1.isShadowOn) {
		board_1.shadowTick += 0.1f;
		if (board_1.shadowTick > shadowDelay) {
			board_1.effect_shadow();
			board_1.shadowTick = 0;
			board_1.isShadowOn = false;
		}
	}
	if (board_2.isShadowOn) {
		board_2.shadowTick += 0.1f;
		if (board_2.shadowTick > shadowDelay) {
			board_2.effect_shadow();
			board_2.shadowTick = 0;
			board_2.isShadowOn = false;
		}
	}

	if (m_Framework->GameOver_1) {
		bool FullBoard_1 = true;
		for (int i = 0; i < table_WIDTH; ++i) {
			if (board_1.Val[i][0] != eBlock::BLOCK_STACK && board_1.Val[i][0] != eBlock::BLOCK_SHADOW) {
				FullBoard_1 = false;
				break;
			}
		}
		if (!FullBoard_1) {
			board_1.spawn(board_1.getRandomXPos(), static_cast<eBlock>(getRandom(Block_Type_Count)));
		}
	}
	if (m_Framework->GameOver_2) {
		bool FullBoard_2 = true;
		for (int i = 0; i < table_WIDTH; ++i) {
			if (board_2.Val[i][0] != eBlock::BLOCK_STACK && board_2.Val[i][0] != eBlock::BLOCK_SHADOW) {
				FullBoard_2 = false;
				break;
			}
		}
		if (!FullBoard_2) {
			board_2.spawn(board_2.getRandomXPos(), static_cast<eBlock>(getRandom(Block_Type_Count)));
		}
	}

	if (m_TimerTick % tick_block == 0) {
		board_1.drop();
		board_2.drop();
	}

	if (m_TimerTick % tick_skill == 0) {
		Hero1P.skillGaugeUp(Hero2P);
		Hero2P.skillGaugeUp(Hero1P);
	}

	Hero1P.move(board_1);
	Hero2P.move(board_2);

	Hero1P.DebuffCheck();
	Hero2P.DebuffCheck();
}

void CGameScene::draw(HDC hDC) {
	DrawGameScene(hDC);
}

bool CGameScene::init(CFramework* pFramework, HWND hWnd) {
	if (!CScene::init(pFramework, hWnd)) return false;
	cout << "GAME SCENE" << endl;
	return true;
}

bool CGameScene::Keyboard(UINT msg, WPARAM w, LPARAM l) {
	switch (msg) {
	case WM_KEYUP:
		switch (w) {
		case VK_LEFT: {
			if (m_Framework->PlayerNum == ePlayer::PLAYER_FIRST && Hero1P.state == eHeroState::HEROSTATE_LEFT)		Hero1P.state = eHeroState::HEROSTATE_NORMAL;
			else if (m_Framework->PlayerNum == ePlayer::PLAYER_SECOND && Hero2P.state == eHeroState::HEROSTATE_LEFT)	Hero2P.state = eHeroState::HEROSTATE_NORMAL;
			break;
		}
		case VK_RIGHT: {
			if (m_Framework->PlayerNum == ePlayer::PLAYER_FIRST && Hero1P.state == eHeroState::HEROSTATE_RIGHT)		Hero1P.state = eHeroState::HEROSTATE_NORMAL;
			else if (m_Framework->PlayerNum == ePlayer::PLAYER_SECOND && Hero2P.state == eHeroState::HEROSTATE_RIGHT)	Hero2P.state = eHeroState::HEROSTATE_NORMAL;
			break;
		}
		}
		break;
	case WM_KEYDOWN:
		switch (w) {
		case VK_LEFT: {
			if (m_Framework->PlayerNum == ePlayer::PLAYER_FIRST)			Hero1P.state = eHeroState::HEROSTATE_LEFT;
			else if (m_Framework->PlayerNum == ePlayer::PLAYER_SECOND)		Hero2P.state = eHeroState::HEROSTATE_LEFT;
			break;
		}
		case VK_RIGHT: {
			if (m_Framework->PlayerNum == ePlayer::PLAYER_FIRST)			Hero1P.state = eHeroState::HEROSTATE_RIGHT;
			else if (m_Framework->PlayerNum == ePlayer::PLAYER_SECOND)		Hero2P.state = eHeroState::HEROSTATE_RIGHT;
			break;
		}
		case 'Z': {
			if (m_Framework->PlayerNum == ePlayer::PLAYER_FIRST)			Hero1P.herojump();
			else if (m_Framework->PlayerNum == ePlayer::PLAYER_SECOND)		Hero2P.herojump();
			break;
		}
		case 'X': {
			if (m_Framework->PlayerNum == ePlayer::PLAYER_FIRST)			Hero1P.SkillOn(Hero2P);
			else if (m_Framework->PlayerNum == ePlayer::PLAYER_SECOND)		Hero2P.SkillOn(Hero1P);
			break;
		}
		}
		break;
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

void CGameScene::ReleaseObjects() {}

void CGameScene::DrawGameScene(HDC hDC) {
	RECT temp{ 0,0,CLIENT_WIDTH,CLIENT_HEIGHT };
	FillRect(hDC, &temp, (HBRUSH)GetStockObject(WHITE_BRUSH));

	board_1.draw(hDC);
	board_2.draw(hDC);

	if (!m_Framework->GameOver_1)	Hero1P.draw(hDC);
	if (!m_Framework->GameOver_2)	Hero2P.draw(hDC);

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

		if (!m_Framework->GameOver_1)	Hero1Score = m_TimerTick;
		if (!m_Framework->GameOver_2)	Hero2Score = m_TimerTick;

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

		if (m_Framework->GameOver_1)	DrawText(hDC, L"Game Over!", lstrlen(L"Game Over!"), &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		textRect.left += DrawGapX;
		textRect.right += DrawGapX;
		if (m_Framework->GameOver_2)	DrawText(hDC, L"Game Over!", lstrlen(L"Game Over!"), &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		SelectObject(hDC, oldFont);
		DeleteObject(hFont);
	}
}