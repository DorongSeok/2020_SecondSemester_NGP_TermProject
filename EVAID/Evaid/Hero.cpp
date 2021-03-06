#include "Hero.h"
#include "stdafx.h"

CHero::CHero(HERO KindOfHero, int Player, CFramework* f) {

	HeroType = KindOfHero;
	pPosition = JPoint(HeroStartPosX, HeroStartPosY);
	state = eHeroState::HEROSTATE_NORMAL;
	NowSkillGauge = 0;
	jumpCnt = 0;
	skillCnt = 0;
	IsSkillOn = false;
	Debuff = 0;
	m_framework = f;

	PlayerNum = Player;
	if (PlayerNum == 0)
		iDrawGapX = 0;
	else
		iDrawGapX = DrawGapX;

	SetHeroRect();

	switch (KindOfHero)
	{
	case HERO::Hero1:
		speed = 9;
		jumpHeight = 15;		// jumpHeighy / 5 가 점프하는 칸수
		SkillCostGauge = 4;		// SkillCostGauge / 20 이 스킬 사용하는데 필요한 칸수
		break;
	case HERO::Hero2:
		speed = 9;
		jumpHeight = 15;		// 세칸 점프
		SkillCostGauge = 5;
		break;
	}
}

CHero::~CHero() {}

void CHero::SetPacketToHero(const sc_packet_user& sc_pack_user)
{
	pPosition.x = sc_pack_user.pos[PlayerNum][0];
	pPosition.y = sc_pack_user.pos[PlayerNum][1];
	NowSkillGauge = sc_pack_user.skillGauge[PlayerNum];
	IsSkillOn = sc_pack_user.skillActive[PlayerNum];
	if (sc_pack_user.skillActive[abs(PlayerNum - 1)])
		Debuff = abs(PlayerNum - 1);
	else
		Debuff = 2;
	state = static_cast<eHeroState>(sc_pack_user.state[PlayerNum]);
}
void CHero::GetHeroToPacket(cs_packet_user* cs_pack_user)
{
	cs_pack_user->pos[0] = pPosition.x;
	cs_pack_user->pos[1] = pPosition.y;
	cs_pack_user->skillGauge = NowSkillGauge;
	cs_pack_user->skillActive = IsSkillOn;
	cs_pack_user->state = static_cast<BYTE>(state);
}

void CHero::SetHeroRect() {
	rect.left = pPosition.x - (HERO_SIZE / 2);
	rect.right = pPosition.x + (HERO_SIZE / 2);
	rect.top = pPosition.y - (HERO_SIZE / 2);
	rect.bottom = pPosition.y + (HERO_SIZE / 2);
}

void CHero::DebuffCheck()
{
	if (Debuff == 0)
		speed = 4;
	else if (Debuff == 1)
		jumpHeight = 0;
	else
	{
		switch (HeroType)
		{
		case HERO::Hero1:
			speed = 9;
			jumpHeight = 15;		// jumpHeighy / 5 가 점프하는 칸수
			break;
		case HERO::Hero2:
			speed = 9;
			jumpHeight = 15;		// 세칸 점프
			break;
		}
	}
}

void CHero::move(CTable Target) {
	// 점프 관련
	if (jumpCnt == 0) {
		int bottomY = GetPosInValY(rect.bottom + Gravity);
		int leftX = GetPosInValX(rect.left + 7);
		int rightX = GetPosInValX(rect.right - 7);
		if (((Target.Val[leftX][bottomY] == eBlock::BLOCK_NONE && Target.Val[rightX][bottomY] == eBlock::BLOCK_NONE)
			|| (Target.Val[leftX][bottomY] == eBlock::BLOCK_SHADOW && Target.Val[rightX][bottomY] == eBlock::BLOCK_SHADOW))
			|| (rect.bottom - PTStartY) % BLOCK_SIZE > 0) {
			pPosition.y += Gravity;
		}
		else Isjump = false;
	}
	else {
		pPosition.y -= Gravity;
		jumpCnt--;
	}

	// 떨어지는 블록과 충돌체크
	int PosL = GetPosInValX(rect.left + 1);
	int PosR = GetPosInValX(rect.right - 1);
	int PosT = GetPosInValY(rect.top);
	int PosB = GetPosInValY(rect.bottom);
	if ((Target.Val[PosL][PosT] != eBlock::BLOCK_NONE && Target.Val[PosL][PosT] != eBlock::BLOCK_SHADOW)
		|| (Target.Val[PosR][PosT] != eBlock::BLOCK_NONE && Target.Val[PosR][PosT] != eBlock::BLOCK_SHADOW)) {
		jumpCnt = 0;
		if (!((Target.Val[PosL][PosB + 1] != eBlock::BLOCK_NONE && Target.Val[PosL][PosB + 1] != eBlock::BLOCK_SHADOW)
			|| (Target.Val[PosR][PosB + 1] != eBlock::BLOCK_NONE && Target.Val[PosR][PosB + 1] != eBlock::BLOCK_SHADOW)))
			pPosition.y = PTStartY + (PosT + 1) * BLOCK_SIZE + BLOCK_SIZE / 2;
	}
	// 이동 관련
	if (state == eHeroState::HEROSTATE_LEFT) {
		int PosT = GetPosInValY(rect.top);
		int PosB = GetPosInValY(rect.bottom - 1);
		int PosL = GetPosInValX(rect.left - speed);
		if (((Target.Val[PosL][PosT] == eBlock::BLOCK_NONE && Target.Val[PosL][PosB] == eBlock::BLOCK_NONE)
			|| (Target.Val[PosL][PosT] == eBlock::BLOCK_SHADOW && Target.Val[PosL][PosB] == eBlock::BLOCK_SHADOW))
			&& rect.left - speed >= PTStartX) pPosition.x -= speed;
		else pPosition.x = (rect.left / BLOCK_SIZE) * BLOCK_SIZE + BLOCK_SIZE;
	}
	else if (state == eHeroState::HEROSTATE_RIGHT) {
		int topY = GetPosInValY(rect.top);
		int bottomY = GetPosInValY(rect.bottom - 1);
		int rightX = GetPosInValX(rect.right + speed);
		if (rightX == table_WIDTH)
			rightX -= 1;
		if (((Target.Val[rightX][topY] == eBlock::BLOCK_NONE && Target.Val[rightX][bottomY] == eBlock::BLOCK_NONE)
			|| (Target.Val[rightX][topY] == eBlock::BLOCK_SHADOW && Target.Val[rightX][bottomY] == eBlock::BLOCK_SHADOW))
			&& rect.right + speed <= PTStartX + table_WIDTH * BLOCK_SIZE) pPosition.x += speed;
		else pPosition.x = (rect.right / BLOCK_SIZE) * BLOCK_SIZE;
	}

	if (PlayerNum == 0) m_framework->Player1Center = pPosition;
	else if (PlayerNum == 1) m_framework->Player2Center = pPosition;

	//cout << jumpCnt << "  " << jumpHeight << "  " << Isjump << endl;

	SetHeroRect();
}

void CHero::draw(HDC hDC) {
	int motionTick = (m_TimerTick / 10) % 2;

	RECT temprect = rect;
	temprect.left += iDrawGapX;
	temprect.right += iDrawGapX;

	// 히어로
	if (IsSkillOn) {
		switch (HeroType) {
		case HERO::Hero1:
			if (motionTick) ResorceTable::getInstance()->img_Hero1_S1.Draw(hDC, temprect);
			else ResorceTable::getInstance()->img_Hero1_S2.Draw(hDC, temprect);
			break;
		case HERO::Hero2:
			if (motionTick) ResorceTable::getInstance()->img_Hero2_S1.Draw(hDC, temprect);
			else ResorceTable::getInstance()->img_Hero2_S2.Draw(hDC, temprect);
			break;
		}
	}
	else {
		switch (HeroType) {
		case HERO::Hero1:
			switch (state) {
			case eHeroState::HEROSTATE_NORMAL:
				ResorceTable::getInstance()->img_Hero1_L1.Draw(hDC, temprect);
				break;
			case eHeroState::HEROSTATE_LEFT:
				if (motionTick) ResorceTable::getInstance()->img_Hero1_L1.Draw(hDC, temprect);
				else ResorceTable::getInstance()->img_Hero1_L2.Draw(hDC, temprect);
				break;
			case eHeroState::HEROSTATE_RIGHT:
				if (motionTick) ResorceTable::getInstance()->img_Hero1_R1.Draw(hDC, temprect);
				else ResorceTable::getInstance()->img_Hero1_R2.Draw(hDC, temprect);
				break;
			}
			break;
		case HERO::Hero2:
			switch (state) {
			case eHeroState::HEROSTATE_NORMAL:
				ResorceTable::getInstance()->img_Hero2_L1.Draw(hDC, temprect);
				break;
			case eHeroState::HEROSTATE_LEFT:
				if (motionTick) ResorceTable::getInstance()->img_Hero2_L1.Draw(hDC, temprect);
				else ResorceTable::getInstance()->img_Hero2_L2.Draw(hDC, temprect);
				break;
			case eHeroState::HEROSTATE_RIGHT:
				if (motionTick) ResorceTable::getInstance()->img_Hero2_R1.Draw(hDC, temprect);
				else ResorceTable::getInstance()->img_Hero2_R2.Draw(hDC, temprect);
				break;
			}
			break;
		}
	}

	// 디버프 상태
	switch (Debuff) {
	case 0:
		ResorceTable::getInstance()->img_Debuff_H1.Draw(hDC, temprect);
		break;
	case 1:
		ResorceTable::getInstance()->img_Debuff_H2.Draw(hDC, temprect);
		break;
	}

	// 스킬 게이지
	RECT SkillBarRect{ 530, 545, 570, 755 };
	SkillBarRect.left += iDrawGapX;
	SkillBarRect.right += iDrawGapX;
	ResorceTable::getInstance()->img_SkillBar.Draw(hDC, SkillBarRect);
	for (int i = 9; i >= 10 - NowSkillGauge; --i) {
		RECT SkillGaugeRect{ 530,550 + (i * 20), 570, 550 + ((i + 1) * 20) };
		SkillGaugeRect.left += iDrawGapX;
		SkillGaugeRect.right += iDrawGapX;
		ResorceTable::getInstance()->img_SkillGauge.Draw(hDC, SkillGaugeRect);
	}
}

void CHero::herojump() {
	if (!Isjump) {
		ResorceTable::getInstance()->m_Sound.PlayEffect(1);
		jumpCnt = jumpHeight;
		Isjump = true;
	}
}

void CHero::skillGaugeUp(CHero& enemyHero) {
	// 지속시간 스킬
	if (IsSkillOn) {
		if (skillCnt > 0) skillCnt--;
		else SkillOff(enemyHero);
	}
	// 스킬 게이지
	if (m_TimerTick % 50 == 0) if (NowSkillGauge + NormalSkillGaugeIncrement <= MaxSkillGauge)	NowSkillGauge += NormalSkillGaugeIncrement;
}

void CHero::SkillOn(CHero& enemyHero) {
	if (NowSkillGauge >= SkillCostGauge && !IsSkillOn) {
		ResorceTable::getInstance()->m_Sound.PlayEffect(4);
		IsSkillOn = true;
		NowSkillGauge -= SkillCostGauge;
		skillCnt = 10;
	}
}

void CHero::SkillOff(CHero& enemyHero)
{
	IsSkillOn = false;
}