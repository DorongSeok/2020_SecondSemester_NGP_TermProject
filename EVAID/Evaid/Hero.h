#pragma once
#include "Table.h"
#include "ResorceTable.h"
#include "Global.h"

// 히어로 떨어지는 속도
#define Gravity 8

// 스킬관련
#define MaxSkillGauge (BYTE)10
#define NormalSkillGaugeIncrement (BYTE)1

class CHero
{
private:
	HERO HeroType;
	JPoint pPosition;
	RECT rect;			// Rect
	int speed;			// 이동속도 능력치

	int jumpHeight;		// 점프 능력치
	int jumpCnt;		// 점프 카운트
	bool Isjump;		// 점프 체크

	int skillCnt;		// 스킬 카운트
	bool IsSkillOn;		// 스킬 체크
	BYTE SkillCostGauge;	// 스킬을 사용하기 위한 게이지
	BYTE NowSkillGauge;	// 현재 스킬 게이지
	bool Invincible = false;	// 무적

	int PlayerNum;		// 플레이어 구분
	int Debuff;			// 디버프상태

	long m_TimerTick;
public:
	STATE state;		// 상태

	int iDrawGapX;		// 플레이어에 따른 x축 그리기 시작 좌표 격차(플레이어 1 = 0, 플레이어 2 = CLIENT_WIDTH / 2)

	CHero(HERO, int);		// 인자 = 히어로 종류, 플레이어구분
	~CHero();

	void SetPacketToHero(JPoint pos, BYTE skillgauge, bool skillactive);
	void GetHeroToPacket(cs_packet_user* cs_pack_user);

	void SetTimer(long TimerTick) { m_TimerTick = TimerTick; }
	void SetHeroRect();
	void move(CTable);
	void draw(HDC);
	void herojump();
	void skillGaugeUp(CHero&);
	void SkillOn(CHero&);
	void SkillOff(CHero&);
};

