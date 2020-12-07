#include "Table.h"
#include "stdafx.h"

CTable::CTable(int Player, CFramework* f) {
	PlayerNum = Player;
	ptstart.x = PTStartX;
	ptstart.y = PTStartY;
	m_framework = f;

	if (PlayerNum == 0)
		iDrawGapX = 0;
	else
		iDrawGapX = DrawGapX;
	ptnextBlock.x += iDrawGapX;
	rcnextBlock.left += iDrawGapX;
	rcnextBlock.right += iDrawGapX;
	
	nextBlock = eBlock::BLOCK_A;

	for (int j = 0; j < table_HEIGHT; ++j) {
		for (int i = 0; i < table_WIDTH; ++i) {
			Val[i][j] = eBlock::BLOCK_NONE;
			rcVal[i][j] = RECT{
				 (i + 0) * BLOCK_SIZE + ptstart.x
				,(j + 0) * BLOCK_SIZE + ptstart.y
				,(i + 1) * BLOCK_SIZE + ptstart.x
				,(j + 1) * BLOCK_SIZE + ptstart.y };
		}
	}
	init();
	spawn(getRandomXPos(), static_cast<eBlock>(getRandom(Block_Type_Count)));
}

CTable::~CTable() {}

void CTable::SetPacketToTable(sc_packet_user& sc_packet_user)
{
	memcpy(Val, sc_packet_user.table[PlayerNum].t, sizeof(Val));

	nextBlock = static_cast<eBlock>(sc_packet_user.nextBlock[PlayerNum]);
}

void CTable::GetTableToPacket(cs_packet_user* cs_pack_user)
{
	memcpy(cs_pack_user->table.t, Val, sizeof(cs_pack_user->table.t));

	cs_pack_user->nextBlock = static_cast<BYTE>(nextBlock);
}

void CTable::drawBg(HDC hDC) {
	ResorceTable::getInstance()->img_ingame_bg.Draw(hDC, 0 + iDrawGapX, 0);
}

void CTable::drawBorder(HDC hDC) {
	ResorceTable::getInstance()->img_ingame_border.Draw(hDC, ptstart.x + iDrawGapX, ptstart.y);
}

void CTable::drawBlock(HDC hDC)
{
	for (int j = 0; j < table_HEIGHT; ++j) {
		for (int i = 0; i < table_WIDTH; ++i) {
			RECT temprect = rcVal[i][j];
			temprect.left += iDrawGapX;
			temprect.right += iDrawGapX;
			switch (Val[i][j]) {
			case eBlock::BLOCK_A:
				ResorceTable::getInstance()->img_block_A.Draw(hDC, temprect);
				break;
			case eBlock::BLOCK_B:
				ResorceTable::getInstance()->img_block_B.Draw(hDC, temprect);
				break;
			case eBlock::BLOCK_C:
				ResorceTable::getInstance()->img_block_C.Draw(hDC, temprect);
				break;
			case eBlock::BLOCK_D:
				ResorceTable::getInstance()->img_block_D.Draw(hDC, temprect);
				break;
			case eBlock::BLOCK_E:
				ResorceTable::getInstance()->img_block_E.Draw(hDC, temprect);
				break;
			case eBlock::BLOCK_STACK:
				ResorceTable::getInstance()->img_block_stack.Draw(hDC, temprect);
				break;
			case eBlock::BLOCK_SHADOW:
				ResorceTable::getInstance()->img_shadow_block.Draw(hDC, temprect);
				break;
			case eBlock::BLOCK_NONE:
				FillRect(hDC, &temprect, static_cast<HBRUSH>(GetStockObject(NULL_BRUSH)));
				break;
			}
		}
	}
}

void CTable::drawNextBlock(HDC hDC)
{
	ResorceTable::getInstance()->img_ingame_nextblock_border.Draw(hDC, ptnextBlock.x, ptnextBlock.y);

	switch (nextBlock) {
	case eBlock::BLOCK_A:
		ResorceTable::getInstance()->img_block_A.Draw(hDC, rcnextBlock);
		break;
	case eBlock::BLOCK_B:
		ResorceTable::getInstance()->img_block_B.Draw(hDC, rcnextBlock);
		break;
	case eBlock::BLOCK_C:
		ResorceTable::getInstance()->img_block_C.Draw(hDC, rcnextBlock);
		break;
	case eBlock::BLOCK_D:
		ResorceTable::getInstance()->img_block_D.Draw(hDC, rcnextBlock);
		break;
	case eBlock::BLOCK_E:
		ResorceTable::getInstance()->img_block_E.Draw(hDC, rcnextBlock);
		break;
	}
}

void CTable::draw(HDC hDC) {
	//DrawBg
	drawBg(hDC);

	//DrawBorder
	drawBorder(hDC);

	//DrawBlock
	drawBlock(hDC);

	//Draw nextBlock & Border
	drawNextBlock(hDC);
}

void CTable::init() {
	Val[0][table_HEIGHT - 1] = eBlock::BLOCK_STACK;
	Val[1][table_HEIGHT - 1] = eBlock::BLOCK_STACK;
	Val[2][table_HEIGHT - 1] = eBlock::BLOCK_STACK;
	Val[3][table_HEIGHT - 1] = eBlock::BLOCK_STACK;
	Val[4][table_HEIGHT - 1] = eBlock::BLOCK_STACK;
	Val[5][table_HEIGHT - 1] = eBlock::BLOCK_STACK;
	Val[6][table_HEIGHT - 1] = eBlock::BLOCK_STACK;
	Val[7][table_HEIGHT - 1] = eBlock::BLOCK_STACK;
	Val[8][table_HEIGHT - 1] = eBlock::BLOCK_STACK;
	Val[9][table_HEIGHT - 1] = eBlock::BLOCK_STACK;
	Val[0][table_HEIGHT - 2] = eBlock::BLOCK_STACK;
	Val[1][table_HEIGHT - 2] = eBlock::BLOCK_STACK;
	Val[2][table_HEIGHT - 2] = eBlock::BLOCK_STACK;
	Val[3][table_HEIGHT - 2] = eBlock::BLOCK_STACK;
	Val[4][table_HEIGHT - 2] = eBlock::BLOCK_STACK;
	Val[5][table_HEIGHT - 2] = eBlock::BLOCK_STACK;
	Val[6][table_HEIGHT - 2] = eBlock::BLOCK_STACK;
	Val[7][table_HEIGHT - 2] = eBlock::BLOCK_STACK;
	Val[8][table_HEIGHT - 2] = eBlock::BLOCK_STACK;
	Val[9][table_HEIGHT - 2] = eBlock::BLOCK_STACK;
	Val[0][table_HEIGHT - 3] = eBlock::BLOCK_STACK;
	Val[1][table_HEIGHT - 3] = eBlock::BLOCK_STACK;
	Val[2][table_HEIGHT - 3] = eBlock::BLOCK_STACK;
	Val[3][table_HEIGHT - 3] = eBlock::BLOCK_STACK;
	Val[4][table_HEIGHT - 3] = eBlock::BLOCK_STACK;
	Val[5][table_HEIGHT - 3] = eBlock::BLOCK_STACK;
	Val[6][table_HEIGHT - 3] = eBlock::BLOCK_STACK;
	Val[7][table_HEIGHT - 3] = eBlock::BLOCK_STACK;
	Val[8][table_HEIGHT - 3] = eBlock::BLOCK_STACK;
	Val[9][table_HEIGHT - 3] = eBlock::BLOCK_STACK;
}

void CTable::drop() 
{
	for (int j = table_HEIGHT - 1; j >= 0; --j) {
		for (int i = 0; i < table_WIDTH; ++i) {
			//A~E블럭과 STACK만 떨어짐
			if (Val[i][j] != eBlock::BLOCK_NONE
				&& Val[i][j] != eBlock::BLOCK_SHADOW)
			{
				//쌓인것 위 이거나 바닥일때
				if (Val[i][j + 1] == eBlock::BLOCK_STACK || j == table_HEIGHT - 1)
				{
					effect(POINT{ i,j }, Val[i][j]);
				}
				//비어있는곳 위, 그림자 위일때
				if (Val[i][j + 1] == eBlock::BLOCK_NONE
					|| Val[i][j + 1] == eBlock::BLOCK_SHADOW)
				{
					// 바닥위가 아니라면
					if (j != table_HEIGHT - 1)
					{
						nowDrop = TRUE;
						Val[i][j + 1] = Val[i][j];
						Val[i][j] = eBlock::BLOCK_NONE;
					}
				}
			}
		}
	}
	if (!nowDrop) {
		if (!check(standard_line, delete_line)) 
		{
			spawn(getRandomXPos(), static_cast<eBlock>(getRandom(Block_Type_Count)));
		}
	}
	nowDrop = FALSE;
}

void CTable::effect(POINT _pt, eBlock type)
{
	if (m_framework->GameOver_1 == true && PlayerNum == 0) ResorceTable::getInstance()->m_Sound.PlayEffect(5);
	else if (m_framework->GameOver_2 == true && PlayerNum == 1) ResorceTable::getInstance()->m_Sound.PlayEffect(5);
	else ResorceTable::getInstance()->m_Sound.PlayEffect(3);
	switch (type) {
	case eBlock::BLOCK_A:
		safeChange(_pt, eBlock::BLOCK_STACK);
		break;
	case eBlock::BLOCK_B:
		safeChange(POINT{ _pt.x - 3,_pt.y }, eBlock::BLOCK_SHADOW);
		safeChange(POINT{ _pt.x - 2,_pt.y }, eBlock::BLOCK_SHADOW);
		safeChange(POINT{ _pt.x - 1,_pt.y }, eBlock::BLOCK_SHADOW);
		safeChange(POINT{ _pt.x + 1,_pt.y }, eBlock::BLOCK_SHADOW);
		safeChange(POINT{ _pt.x + 2,_pt.y }, eBlock::BLOCK_SHADOW);
		safeChange(POINT{ _pt.x + 3,_pt.y }, eBlock::BLOCK_SHADOW);
		safeChange(POINT{ _pt.x + 0,_pt.y }, eBlock::BLOCK_STACK);
		break;
	case eBlock::BLOCK_C:
		safeChange(POINT{ _pt.x , _pt.y - 1 }, eBlock::BLOCK_SHADOW);
		safeChange(POINT{ _pt.x , _pt.y - 0 }, eBlock::BLOCK_STACK);
		break;
	case eBlock::BLOCK_D:
		safeChange(POINT{ _pt.x - 1 , _pt.y - 1 }, eBlock::BLOCK_SHADOW);
		safeChange(POINT{ _pt.x + 0 , _pt.y - 1 }, eBlock::BLOCK_SHADOW);
		safeChange(POINT{ _pt.x + 1 , _pt.y - 1 }, eBlock::BLOCK_SHADOW);
		safeChange(POINT{ _pt.x + 1 , _pt.y + 0 }, eBlock::BLOCK_SHADOW);
		safeChange(POINT{ _pt.x - 1 , _pt.y + 0 }, eBlock::BLOCK_SHADOW);
		safeChange(POINT{ _pt.x + 0 , _pt.y + 0 }, eBlock::BLOCK_STACK);
		break;
	case eBlock::BLOCK_E:
		safeChange(POINT{ _pt.x - 1, _pt.y - 3 }, eBlock::BLOCK_SHADOW);
		safeChange(POINT{ _pt.x + 0, _pt.y - 3 }, eBlock::BLOCK_SHADOW);
		safeChange(POINT{ _pt.x + 1, _pt.y - 3 }, eBlock::BLOCK_SHADOW);
		safeChange(POINT{ _pt.x + 0, _pt.y + 0 }, eBlock::BLOCK_STACK);
		break;
	}
	check(standard_line, delete_line);
	isShadowOn = true;
	GameOverCheck(PlayerNum);
}

void CTable::effect_shadow()
{
	for (int j = 0; j < table_HEIGHT; ++j) {
		for (int i = 0; i < table_WIDTH; ++i) {
			if (Val[i][j] == eBlock::BLOCK_SHADOW) {
				Val[i][j] = eBlock::BLOCK_STACK;
			}
		}
	}
	GameOverCheck(PlayerNum);
}

bool CTable::safeChange(POINT _pt, eBlock type)
{
	if (_pt.x < 0 || _pt.x >= table_WIDTH) return false;
	if (_pt.y < 0 || _pt.y >= table_HEIGHT) return false;
	if (Val[_pt.x][_pt.y] == eBlock::BLOCK_STACK && type == eBlock::BLOCK_SHADOW) return false;
	Val[_pt.x][_pt.y] = type;
	return true;
}

void CTable::spawn(int _spawnXpos, eBlock _spawnType)
{
	safeChange(POINT{ _spawnXpos, 0 }, nextBlock);

	nextBlock = _spawnType;
}

bool CTable::check(int _standard, int _delete)
{
	for (int i = 0; i < table_WIDTH; ++i) {
		for (int j = 0; j < standard_line; ++j) {
			if (Val[i][(table_HEIGHT - 1) - j] != eBlock::BLOCK_STACK) return FALSE;
		}
	}
	if ((m_framework->GameOver_1 == false && PlayerNum == 0) ||
		(m_framework->GameOver_2 == false && PlayerNum == 1))
	{
		for (int i = 0; i < table_WIDTH; ++i) {
			for (int j = 0; j < delete_line; ++j) {
				Val[i][(table_HEIGHT - 1) - j] = eBlock::BLOCK_NONE;
			}
		}
	}
	return TRUE;
}

int CTable::getRandomXPos()
{
	bool realRandom = true;
	for (int i = 0; i < table_WIDTH; ++i)
	{
		if (Val[i][table_HEIGHT - standard_line - 2] == eBlock::BLOCK_STACK)
		{
			realRandom = false;
			break;
		}
	}
	if (realRandom)
		return rand() % table_WIDTH;

	int targetPos[table_WIDTH];
	int targetPosCnt = 0;
	for (int i = 0; i < table_WIDTH; ++i)
	{
		if (Val[i][table_HEIGHT - standard_line] == eBlock::BLOCK_NONE || Val[i][table_HEIGHT - standard_line] == eBlock::BLOCK_SHADOW)
			targetPos[targetPosCnt++] = i;
	}
	if (targetPosCnt > 0)
		return targetPos[rand() % targetPosCnt];
	return rand() % table_WIDTH;
}

void CTable::GameOverCheck(int Player)
{
	if (Player == 0)
	{
		if ((Val[GetPosInValX(m_framework->Player1Center.x - 17)][GetPosInValY(m_framework->Player1Center.y)] != eBlock::BLOCK_NONE &&
			Val[GetPosInValX(m_framework->Player1Center.x - 17)][GetPosInValY(m_framework->Player1Center.y)] != eBlock::BLOCK_SHADOW)
			|| (Val[GetPosInValX(m_framework->Player1Center.x + 17)][GetPosInValY(m_framework->Player1Center.y)] != eBlock::BLOCK_NONE &&
				Val[GetPosInValX(m_framework->Player1Center.x + 17)][GetPosInValY(m_framework->Player1Center.y)] != eBlock::BLOCK_SHADOW))
		{
			m_framework->GameOver_1 = true;
		}
	}
	else if (Player == 1)
	{
		if ((Val[GetPosInValX(m_framework->Player2Center.x - 17)][GetPosInValY(m_framework->Player2Center.y)] != eBlock::BLOCK_NONE &&
			Val[GetPosInValX(m_framework->Player2Center.x - 17)][GetPosInValY(m_framework->Player2Center.y)] != eBlock::BLOCK_SHADOW)
			|| (Val[GetPosInValX(m_framework->Player2Center.x + 17)][GetPosInValY(m_framework->Player2Center.y)] != eBlock::BLOCK_NONE &&
				Val[GetPosInValX(m_framework->Player2Center.x + 17)][GetPosInValY(m_framework->Player2Center.y)] != eBlock::BLOCK_SHADOW))
		{
			m_framework->GameOver_2 = true;
		}
	}
}