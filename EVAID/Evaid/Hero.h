#pragma once
#include "Table.h"
#include "ResorceTable.h"
#include "Global.h"

// ����� �������� �ӵ�
#define Gravity 8

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
	bool Invincible = false;	// ����

	int PlayerNum;		// �÷��̾� ����
	int Debuff;			// ���������

	long m_TimerTick;
public:
	STATE state;		// ����

	int iDrawGapX;		// �÷��̾ ���� x�� �׸��� ���� ��ǥ ����(�÷��̾� 1 = 0, �÷��̾� 2 = CLIENT_WIDTH / 2)

	CHero(HERO, int);		// ���� = ����� ����, �÷��̾��
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

