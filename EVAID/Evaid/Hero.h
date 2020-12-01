#pragma once
#include "Table.h"
#include "ResorceTable.h"
#include "Framework.h"
#include "stdafx.h"

// ��ų����
#define MaxSkillGauge (BYTE)10
#define NormalSkillGaugeIncrement (BYTE)1

class CHero
{
private:
	HERO HeroType;
	JPoint pPosition;
	RECT rect;			// Rect
	int speed;			// �̵��ӵ� �ɷ�ġ

	int jumpHeight;		// ���� �ɷ�ġ
	int jumpCnt;		// ���� ī��Ʈ
	bool Isjump;		// ���� üũ

	int skillCnt;		// ��ų ī��Ʈ
	bool IsSkillOn;		// ��ų üũ
	BYTE SkillCostGauge;	// ��ų�� ����ϱ� ���� ������
	BYTE NowSkillGauge;	// ���� ��ų ������

	int PlayerNum;		// �÷��̾� ����
	int Debuff;			// ���������

	long m_TimerTick;
	CFramework* m_framework;
public:
	STATE state;		// ����

	int iDrawGapX;		// �÷��̾ ���� x�� �׸��� ���� ��ǥ ����(�÷��̾� 1 = 0, �÷��̾� 2 = CLIENT_WIDTH / 2)

	CHero(HERO, int, CFramework*);		// ���� = ����� ����, �÷��̾��
	~CHero();

	void SetPacketToHero(const sc_packet_user& sc_pack_user);
	void GetHeroToPacket(cs_packet_user* cs_pack_user);

	void setFramework(CFramework* f) { m_framework = f; }
	void SetTimer(long TimerTick) { m_TimerTick = TimerTick; }
	void SetHeroRect();
	void move(CTable);
	void draw(HDC);
	void herojump();
	void skillGaugeUp(CHero&);
	void SkillOn(CHero&);
	void SkillOff(CHero&);
};

