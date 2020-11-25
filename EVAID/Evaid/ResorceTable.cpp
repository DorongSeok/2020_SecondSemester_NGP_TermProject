#include "ResorceTable.h"

ResorceTable::ResorceTable() {
	loadImg();
	if (!loadCheck()) cout << "file load resource" << endl;

	//m_Sound.AddSound(0, "Sound/MainBGM.mp3"/*, true*/);			// 배경음일때는 뒤에 true
	//m_Sound.AddSound(1, "Sound/JumpSound.mp3");	
	//m_Sound.AddSound(2, "Sound/ClickSound.mp3");
	//m_Sound.AddSound(3, "Sound/DropSound.mp3");	
	//m_Sound.AddSound(4, "Sound/SkillSound.mp3");
	//m_Sound.AddSound(5, "Sound/DeadBlockSound.mp3");
}

ResorceTable::~ResorceTable() {
	// instancename.Destory();

	//7
	img_block_stack.Destroy();
	img_block_A.Destroy();
	img_block_B.Destroy();
	img_block_C.Destroy();
	img_block_D.Destroy();
	img_block_E.Destroy();
	img_shadow_block.Destroy();

	//7
	img_Hero1_L1.Destroy();
	img_Hero1_L2.Destroy();
	img_Hero1_R1.Destroy();
	img_Hero1_R2.Destroy();
	img_Hero1_S1.Destroy();
	img_Hero1_S2.Destroy();
	img_Debuff_H1.Destroy();

	//7
	img_Hero2_L1.Destroy();
	img_Hero2_L2.Destroy();
	img_Hero2_R1.Destroy();
	img_Hero2_R2.Destroy();
	img_Hero2_S1.Destroy();
	img_Hero2_S2.Destroy();
	img_Debuff_H2.Destroy();

	//2
	img_SkillBar.Destroy();
	img_SkillGauge.Destroy();

	//2
	img_ingame_bg.Destroy();
	img_logo_bg.Destroy();
	
	//2
	img_ingame_border.Destroy();
	img_ingame_nextblock_border.Destroy();

	//3
	img_main_btn_start.Destroy();
	img_main_btn_rank.Destroy();
	img_main_btn_exit.Destroy();

	//6
	img_lobby_bg.Destroy();
	img_lobby_hero_border.Destroy();
	img_lobby_hero1_ready.Destroy();
	img_lobby_hero1_unready.Destroy();
	img_lobby_hero2_ready.Destroy();
	img_lobby_hero2_unready.Destroy();

	//3
	img_lobby_connect_bnt.Destroy();
	img_lobby_ready_bnt.Destroy();
	img_lobby_ready_disabled_bnt.Destroy();
}

bool ResorceTable::loadCheck()
{
	//7
	if (img_block_stack.IsNull()) return false;
	if (img_block_A.IsNull()) return false;
	if (img_block_B.IsNull()) return false;
	if (img_block_C.IsNull()) return false;
	if (img_block_D.IsNull()) return false;
	if (img_block_E.IsNull()) return false;
	if (img_shadow_block.IsNull()) return false;

	//7
	if (img_Hero1_L1.IsNull()) return false;
	if (img_Hero1_L2.IsNull()) return false;
	if (img_Hero1_R1.IsNull()) return false;
	if (img_Hero1_R2.IsNull()) return false;
	if (img_Hero1_S1.IsNull()) return false;
	if (img_Hero1_S2.IsNull()) return false;
	if (img_Debuff_H1.IsNull()) return false;

	//7
	if (img_Hero2_L1.IsNull()) return false;
	if (img_Hero2_L2.IsNull()) return false;
	if (img_Hero2_R1.IsNull()) return false;
	if (img_Hero2_R2.IsNull()) return false;
	if (img_Hero2_S1.IsNull()) return false;
	if (img_Hero2_S2.IsNull()) return false;
	if (img_Debuff_H2.IsNull()) return false;
	
	//2
	if (img_SkillBar.IsNull()) return false;
	if (img_SkillGauge.IsNull()) return false;

	//2
	if (img_ingame_bg.IsNull()) return false;
	if (img_logo_bg.IsNull()) return false;

	//2
	if (img_ingame_border.IsNull()) return false;
	if (img_ingame_nextblock_border.IsNull()) return false;
	
	//3
	if (img_main_btn_start.IsNull()) return false;
	if (img_main_btn_rank.IsNull()) return false;
	if (img_main_btn_exit.IsNull()) return false;

	//6
	if (img_lobby_bg.IsNull()) return false;
	if (img_lobby_hero_border.IsNull()) return false;
	if (img_lobby_hero1_ready.IsNull()) return false;
	if (img_lobby_hero1_unready.IsNull()) return false;
	if (img_lobby_hero2_ready.IsNull()) return false;
	if (img_lobby_hero2_unready.IsNull()) return false;

	//3
	if (img_lobby_connect_bnt.IsNull()) return false;
	if (img_lobby_ready_bnt.IsNull()) return false;
	if (img_lobby_ready_disabled_bnt.IsNull()) return false;

	return true;
}

void ResorceTable::loadImg()
{
	//7
	img_block_stack.Load(TEXT("Image/block_stack.png"));
	img_block_A.Load(TEXT("Image/block_A.png"));
	img_block_B.Load(TEXT("Image/block_B.png"));
	img_block_C.Load(TEXT("Image/block_C.png"));
	img_block_D.Load(TEXT("Image/block_D.png"));
	img_block_E.Load(TEXT("Image/block_E.png"));
	img_shadow_block.Load(TEXT("Image/shadow_block.png"));

	//7
	img_Hero1_L1.Load(TEXT("Image/Hero1_L1.png"));
	img_Hero1_L2.Load(TEXT("Image/Hero1_L2.png"));
	img_Hero1_R1.Load(TEXT("Image/Hero1_R1.png"));
	img_Hero1_R2.Load(TEXT("Image/Hero1_R2.png"));
	img_Hero1_S1.Load(TEXT("Image/Hero1_S1.png"));
	img_Hero1_S2.Load(TEXT("Image/Hero1_S2.png"));
	img_Debuff_H1.Load(TEXT("Image/Debuff_H1.png"));

	//7
	img_Hero2_L1.Load(TEXT("Image/Hero2_L1.png"));
	img_Hero2_L2.Load(TEXT("Image/Hero2_L2.png"));
	img_Hero2_R1.Load(TEXT("Image/Hero2_R1.png"));
	img_Hero2_R2.Load(TEXT("Image/Hero2_R2.png"));
	img_Hero2_S1.Load(TEXT("Image/Hero2_S1.png"));
	img_Hero2_S2.Load(TEXT("Image/Hero2_S2.png"));
	img_Debuff_H2.Load(TEXT("Image/Debuff_H2.png"));

	//2
	img_SkillBar.Load(TEXT("Image/SkillBar.png"));
	img_SkillGauge.Load(TEXT("Image/SkillGauge.png"));

	//2
	img_ingame_bg.Load(TEXT("Image/ingame_bg.png"));
	img_logo_bg.Load(TEXT("Image/logo_bg.png"));
	
	//2
	img_ingame_border.Load(TEXT("Image/ingame_border.png"));
	img_ingame_nextblock_border.Load(TEXT("Image/ingame_nextblock_border.png"));
	
	//3
	img_main_btn_start.Load(TEXT("Image/main_start_bnt.png"));
	img_main_btn_rank.Load(TEXT("Image/main_rank_bnt.png"));
	img_main_btn_exit.Load(TEXT("Image/main_exit_bnt.png"));

	//6
	img_lobby_bg.Load(TEXT("Image/lobby_bg.png"));
	img_lobby_hero_border.Load(TEXT("Image/lobby_hero_border.png"));
	img_lobby_hero1_ready.Load(TEXT("Image/lobby_hero1_ready.png"));
	img_lobby_hero1_unready.Load(TEXT("Image/lobby_hero1_unready.png"));
	img_lobby_hero2_ready.Load(TEXT("Image/lobby_hero2_ready.png"));
	img_lobby_hero2_unready.Load(TEXT("Image/lobby_hero2_unready.png"));

	//3
	img_lobby_connect_bnt.Load(TEXT("Image/lobby_connect_bnt.png"));
	img_lobby_ready_bnt.Load(TEXT("Image/lobby_ready_bnt.png"));
	img_lobby_ready_disabled_bnt.Load(TEXT("Image/lobby_ready_disabled_bnt.png"));
}

ResorceTable* ResorceTable::inst = nullptr;