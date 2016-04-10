/*!
 * @file mutation.c
 * @brief �����Ѱۥ롼��μ��� / Mutation effects (and racial powers)
 * @date 2014/01/11
 * @author
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke\n
 *\n
 * This software may be copied and distributed for educational, research,\n
 * and not for profit purposes provided that this copyright and statement\n
 * are included in all such copies.  Other copyrights may also apply.\n
 * 2014 Deskull rearranged comment for Doxygen.\n
 */

#include "angband.h"

/*!
 * @brief �ץ쥤�䡼�������Ѱۤ�Ϳ����
 * @param choose_mut Ϳ�����������Ѱۤ�ID��0�ʤ�Х����������
 * @return �ʤ�
 */
bool gain_random_mutation(int choose_mut)
{
	int     attempts_left = 20;
	cptr    muta_desc = "";
	bool    muta_chosen = FALSE;
	u32b    muta_which = 0;
	u32b    *muta_class = NULL;

	if (choose_mut) attempts_left = 1;

	while (attempts_left--)
	{
		switch (choose_mut ? choose_mut : (p_ptr->pclass == CLASS_BERSERKER ? 74+randint1(119) : randint1(193)))
		{
		case 1: case 2: case 3: case 4:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_SPIT_ACID;
			muta_desc = _("�����Ǥ�ǽ�Ϥ�������", "You gain the ability to spit acid.");
			break;
			
		case 5: case 6: case 7:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_BR_FIRE;
			muta_desc = _("�Ф��Ǥ�ǽ�Ϥ�������", "You gain the ability to breathe fire.");
			break;
			
		case 8: case 9:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_HYPN_GAZE;
			muta_desc = _("��̲���ǽ�Ϥ�������", "Your eyes look mesmerizing...");
			break;
			
		case 10: case 11:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_TELEKINES;
			muta_desc = _("ʪ�Τ�ǰư�Ϥ�ư����ǽ�Ϥ�������", "You gain the ability to move objects telekinetically.");
			break;
			
		case 12: case 13: case 14:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_VTELEPORT;
			muta_desc = _("��ʬ�ΰջפǥƥ�ݡ��Ȥ���ǽ�Ϥ�������", "You gain the power of teleportation at will.");
			break;
			
		case 15: case 16:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_MIND_BLST;
			muta_desc = _("���������ǽ�Ϥ�������", "You gain the power of Mind Blast.");
			break;
			
		case 17: case 18:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_RADIATION;
			muta_desc = _("���ʤ��϶�����������ȯ�����Ϥ᤿��", "You start emitting hard radiation.");
			break;
			
		case 19: case 20:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_VAMPIRISM;
			muta_desc = _("��̿�Ϥ�ۼ��Ǥ���褦�ˤʤä���", "You become vampiric.");
			break;
			
		case 21: case 22: case 23:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_SMELL_MET;
			muta_desc = _("��°���������̤�ʬ������褦�ˤʤä���", "You smell a metallic odor.");
			break;
			
		case 24: case 25: case 26: case 27:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_SMELL_MON;
			muta_desc = _("��󥹥����ν������̤�ʬ������褦�ˤʤä���", "You smell filthy monsters.");
			break;
			
		case 28: case 29: case 30:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_BLINK;
			muta_desc = _("���Υ�ƥ�ݡ��Ȥ�ǽ�Ϥ�������", "You gain the power of minor teleportation.");
			break;
			
		case 31: case 32:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_EAT_ROCK;
			muta_desc = _("�ɤ���̣�������˸����롣", "The walls look delicious.");
			break;
			
		case 33: case 34:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_SWAP_POS;
			muta_desc = _("¾�ͤη��ǰ�ޥ����⤯�褦�ʵ�ʬ�����롣", "You feel like walking a mile in someone else's shoes.");
			break;
			
		case 35: case 36: case 37:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_SHRIEK;
			muta_desc = _("���ʤ����������������ʤä���", "Your vocal cords get much tougher.");
			break;
			
		case 38: case 39: case 40:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_ILLUMINE;
			muta_desc = _("���ʤ��ϸ��굱�������������뤯����褦�ˤʤä���", "You can light up rooms with your presence.");
			break;
			
		case 41: case 42:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_DET_CURSE;
			muta_desc = _("�ٰ�����ˡ���ΤǤ���褦�ˤʤä���", "You can feel evil magics.");
			break;
			
		case 43: case 44: case 45:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_BERSERK;
			muta_desc = _("����Ǥ�����򴶤��롣", "You feel a controlled rage.");
			break;
			
		case 46:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_POLYMORPH;
			muta_desc = _("�Τ��Ѱۤ��䤹���ʤä���", "Your body seems mutable.");
			break;
			
		case 47: case 48:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_MIDAS_TCH;
			muta_desc = _("�֥ߥ������μ�פ�ǽ�Ϥ�������", "You gain the Midas touch.");/*�ȥ�����쥤�������ˤ���ޤ����͡� */
			break;
			
		case 49:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_GROW_MOLD;
			muta_desc = _("�������Ӥ˿Ƥ��ߤ�Ф�����", "You feel a sudden affinity for mold.");
			break;
			
		case 50: case 51: case 52:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_RESIST;
			muta_desc = _("���ʤ��ϼ�ʬ���Ȥ���뵤�����롣", "You feel like you can protect yourself.");
			break;
			
		case 53: case 54: case 55:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_EARTHQUAKE;
			muta_desc = _("���󥸥����˲�����ǽ�Ϥ�������", "You gain the ability to wreck the dungeon.");
			break;
			
		case 56:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_EAT_MAGIC;
			muta_desc = _("��ˡ�Υ����ƥब��̣�����˸����롣", "Your magic items look delicious.");
			break;
			
		case 57: case 58:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_WEIGH_MAG;
			muta_desc = _("���ʤ��ϼ��Ϥˤ�����ˡ�����ɤ�����Ǥ��뵤�����롣", "You feel you can better understand the magic around you.");
			break;
			
		case 59:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_STERILITY;
			muta_desc = _("��������ƤμԤ�Ƭ�ˤ򵯤������Ȥ��Ǥ��롣", "You can give everything around you a headache.");
			break;
		case 60: case 61:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_PANIC_HIT;
			muta_desc = _("������ť���ε�ʬ��ʬ����褦�ˤʤä���", "You suddenly understand how thieves feel.");
			break;
			
		case 62: case 63: case 64:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_DAZZLE;
			muta_desc = _("����������ȯ����ǽ�Ϥ�������", "You gain the ability to emit dazzling lights.");
			break;
			
		case 65: case 66: case 67:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_LASER_EYE;
			muta_desc = _("���ʤ����ܤϰ�־Ƥ��դ�����", "Your eyes burn for a moment.");
			break;
			
		case 68: case 69:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_RECALL;
			muta_desc = _("���������ۡ��ॷ�å��ˤʤä���������ľ�ä���", "You feel briefly homesick, but it passes.");
			break;
			
		case 70:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_BANISH;
			muta_desc = _("�������ܤ���Ϥ��������줿��", "You feel a holy wrath fill you.");
			break;
			
		case 71: case 72:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_COLD_TOUCH;
			muta_desc = _("���ʤ���ξ��ϤȤƤ��䤿���ʤä���", "Your hands get very cold.");
			break;
			
		case 73: case 74:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_LAUNCHER;
			muta_desc = _("���ʤ���ʪ���ꤲ���Ϥ��ʤ궯���ʤä��������롣", "Your throwing arm feels much stronger.");
			break;
			
		case 75:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_BERS_RAGE;
			muta_desc = _("���ʤ��϶�˽����ȯ��򵯤����褦�ˤʤä���", "You become subject to fits of berserk rage!");
			break;
			
		case 76:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_COWARDICE;
			muta_desc = _("�������ʤ����餤���¤ˤʤä���", "You become an incredible coward!");
			break;
			
		case 77:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_RTELEPORT;
			muta_desc = _("���ʤ��ΰ��֤������Գ���ˤʤä���", "Your position seems very uncertain...");
			break;
			
		case 78:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_ALCOHOL;
			muta_desc = _("���ʤ��ϥ��륳�����ʬ�礹��褦�ˤʤä���", "Your body starts producing alcohol!");
			break;
			
		case 79:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_HALLU;
			muta_desc = _("���ʤ��ϸ��Ф������������������˿����줿��", "You are afflicted by a hallucinatory insanity!");
			break;
			
		case 80:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_FLATULENT;
			muta_desc = _( "���ʤ���������ǽ�ʶ��������򤳤��褦�ˤʤä���",  "You become subject to uncontrollable flatulence.");

			break;
		case 81: case 82:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_SCOR_TAIL;
			muta_desc = _( "������ο����������Ƥ�����",  "You grow a scorpion tail!");

			break;
		case 83: case 84:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_HORNS;
			muta_desc = _( "�ۤ˳Ѥ���������",  "Horns pop forth into your forehead!");

			break;
		case 85: case 86:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_BEAK;
			muta_desc = _( "�����Ԥ����������Х����Ѳ�������",  "Your mouth turns into a sharp, powerful beak!");

			break;
		case 87: case 88:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_ATT_DEMON;
			muta_desc = _( "���������դ���褦�ˤʤä���",  "You start attracting demons.");

			break;
		case 89:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_PROD_MANA;
			muta_desc = _( "���ʤ���������ǽ����ˡ�Υ��ͥ륮����ȯ������褦�ˤʤä���",  "You start producing magical energy uncontrollably.");

			break;
		case 90: case 91:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_SPEED_FLUX;
			muta_desc = _( "���ʤ����ݵ���ˤʤä���",  "You become manic-depressive.");

			break;
		case 92: case 93:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_BANISH_ALL;
			muta_desc = _( "�������Ϥ����ʤ����ظ������Ǥ��뵤�����롣",  "You feel a terrifying power lurking behind you.");

			break;
		case 94:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_EAT_LIGHT;
			muta_desc = _( "���ʤ��ϥ��󥴥ꥢ��Ȥ˴�̯�ʿƤ��ߤ�Ф���褦�ˤʤä���",  "You feel a strange kinship with Ungoliant.");

			break;
		case 95: case 96:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_TRUNK;
			muta_desc = _( "���ʤ���ɡ�Ͽ��Ӥƾݤ�ɡ�Τ褦�ˤʤä���",  "Your nose grows into an elephant-like trunk.");

			break;
		case 97:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_ATT_ANIMAL;
			muta_desc = _( "ưʪ������դ���褦�ˤʤä���",  "You start attracting animals.");

			break;
		case 98:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_TENTACLES;
			muta_desc = _( "�ٰ��ʿ��꤬�Τ�ξ¦�������Ƥ�����",  "Evil-looking tentacles sprout from your sides.");

			break;
		case 99:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_RAW_CHAOS;
			muta_desc = _( "���Ϥζ��֤��԰���ˤʤä��������롣",  "You feel the universe is less stable around you.");

			break;
		case 100: case 101: case 102:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_NORMALITY;
			muta_desc = _( "���ʤ��ϴ�̯�ʤۤ����̤ˤʤä��������롣",  "You feel strangely normal.");

			break;
		case 103:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_WRAITH;
			muta_desc = _( "���ʤ���ͩ�β���������β������ꤹ��褦�ˤʤä���",  "You start to fade in and out of the physical world.");

			break;
		case 104:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_POLY_WOUND;
			muta_desc = _( "���ʤ��ϥ��������Ϥ��Ť������������Ǥ���Τ򴶤�����",  "You feel forces of chaos entering your old scars.");

			break;
		case 105:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_WASTING;
			muta_desc = _( "���ʤ������������ޤ�������¤ˤ����ä���",  "You suddenly contract a horrible wasting disease.");

			break;
		case 106:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_ATT_DRAGON;
			muta_desc = _( "���ʤ��ϥɥ饴�������Ĥ���褦�ˤʤä���",  "You start attracting dragons.");

			break;
		case 107: case 108:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_WEIRD_MIND;
			muta_desc = _( "���ʤ��λ׹ͤ������������������˸����Ϥ᤿��",  "Your thoughts suddenly take off in strange directions.");

			break;
		case 109:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_NAUSEA;
			muta_desc = _( "���ޤ��ԥ��ԥ����Ϥ��᤿��",  "Your stomach starts to roil nauseously.");

			break;
		case 110: case 111:
			/* Chaos warriors already have a chaos deity */
			if (p_ptr->pclass != CLASS_CHAOS_WARRIOR)
			{
				muta_class = &(p_ptr->muta2);
				muta_which = MUT2_CHAOS_GIFT;
			muta_desc = _( "���ʤ��ϥ������μ������դ�椯�褦�ˤʤä���",  "You attract the notice of a chaos deity!");

			}
			break;
		case 112:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_WALK_SHAD;
			muta_desc = _( "���ʤ��ϸ��¤���Τ褦�������ȴ�����褦�ˤʤä���",  "You feel like reality is as thin as paper.");

			break;
		case 113: case 114:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_WARNING;
			muta_desc = _( "���ʤ��������ѥ�Υ����ˤʤä��������롣",  "You suddenly feel paranoid.");

			break;
		case 115:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_INVULN;
			muta_desc = _( "���ʤ��Ͻ�ʡ���졢̵Ũ���֤ˤʤ�ȯ��򵯤����褦�ˤʤä���",  "You are blessed with fits of invulnerability.");

			break;
		case 116: case 117:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_SP_TO_HP;
			muta_desc = _( "��ˡ�μ�����ȯ��򵯤����褦�ˤʤä���",  "You are subject to fits of magical healing.");

			break;
		case 118:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_HP_TO_SP;
			muta_desc = _( "�ˤߤ�ȼ���������Ʋ���ȯ��򵯤����褦�ˤʤä���",  "You are subject to fits of painful clarity.");

			break;
		case 119:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_DISARM;
			muta_desc = _( "���ʤ��εӤ�Ĺ�������ܤˤʤä���",  "Your feet grow to four times their former size.");

			break;
		case 120: case 121: case 122:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_HYPER_STR;
			muta_desc = _( "Ķ��Ū�˶����ʤä���",  "You turn into a superhuman he-man!");

			break;
		case 123: case 124: case 125:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_PUNY;
			muta_desc = _( "��������äƤ��ޤä�...",  "Your muscles wither away...");

			break;
		case 126: case 127: case 128:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_HYPER_INT;
			muta_desc = _( "���ʤ���Ǿ�����Υ���ԥ塼���˿ʲ�������",  "Your brain evolves into a living computer!");

			break;
		case 129: case 130: case 131:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_MORONIC;
			muta_desc = _( "Ǿ����̤��Ƥ��ޤä�...",  "Your brain withers away...");

			break;
		case 132: case 133:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_RESILIENT;
			muta_desc = _( "�³���ƥ��դˤʤä���",  "You become extraordinarily resilient.");

			break;
		case 134: case 135:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_XTRA_FAT;
			muta_desc = _( "���ʤ��ϵ������������餤���ä���",  "You become sickeningly fat!");

			break;
		case 136: case 137:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_ALBINO;
			muta_desc = _( "����ӥΤˤʤä����夯�ʤä���������...",  "You turn into an albino! You feel frail...");

			break;
		case 138: case 139: case 140:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_FLESH_ROT;
			muta_desc = _( "���ʤ������Τ����Ԥ����µ��˿����줿��",  "Your flesh is afflicted by a rotting disease!");

			break;
		case 141: case 142:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_SILLY_VOI;
			muta_desc = _( "������ȴ���ʥ����������ˤʤä���",  "Your voice turns into a ridiculous squeak!");

			break;
		case 143: case 144:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_BLANK_FAC;
			muta_desc = _( "�Τäڤ�ܤ��ˤʤä���",  "Your face becomes completely featureless!");

			break;
		case 145:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_ILL_NORM;
			muta_desc = _( "���ΰ¤餰���Ƥ�Ǥ��Ф��褦�ˤʤä���",  "You start projecting a reassuring image.");

			break;
		case 146: case 147: case 148:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_XTRA_EYES;
			muta_desc = _( "��������Ĥ��ܤ����褿��",  "You grow an extra pair of eyes!");

			break;
		case 149: case 150:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_MAGIC_RES;
			muta_desc = _( "��ˡ�ؤ��������Ĥ�����",  "You become resistant to magic.");

			break;
		case 151: case 152: case 153:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_XTRA_NOIS;
			muta_desc = _( "���ʤ��ϴ�̯�ʲ���Ω�ƻϤ᤿��",  "You start making strange noise!");

			break;
		case 154: case 155: case 156:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_INFRAVIS;
			muta_desc = _( "�ֳ������Ϥ���������",  "Your infravision is improved.");

			break;
		case 157: case 158:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_XTRA_LEGS;
			muta_desc = _( "���������ܤ�­�������Ƥ�����",  "You grow an extra pair of legs!");

			break;
		case 159: case 160:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_SHORT_LEG;
			muta_desc = _( "­��û���͵��ˤʤäƤ��ޤä���",  "Your legs turn into short stubs!");

			break;
		case 161: case 162:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_ELEC_TOUC;
			muta_desc = _( "��ɤ���ή��ή��Ϥ᤿��",  "Electricity starts running through you!");

			break;
		case 163: case 164:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_FIRE_BODY;
			muta_desc = _( "���ʤ����Τϱ�ˤĤĤޤ�Ƥ��롣",  "Your body is enveloped in flames!");

			break;
		case 165: case 166: case 167:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_WART_SKIN;
			muta_desc = _( "�������������ܥ��ܤ�����ˤǤ�����",  "Disgusting warts appear everywhere on you!");

			break;
		case 168: case 169: case 170:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_SCALES;
			muta_desc = _( "ȩ�������ڤ��Ѥ�ä���",  "Your skin turns into black scales!");

			break;
		case 171: case 172:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_IRON_SKIN;
			muta_desc = _( "���ʤ���ȩ��Ŵ�ˤʤä���",  "Your skin turns to steel!");

			break;
		case 173: case 174:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_WINGS;
			muta_desc = _( "����˱�����������",  "You grow a pair of wings.");

			break;
		case 175: case 176: case 177:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_FEARLESS;
			muta_desc = _( "�������ݤ��Τ餺�ˤʤä���",  "You become completely fearless.");

			break;
		case 178: case 179:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_REGEN;
			muta_desc = _( "��®�˲������Ϥ᤿��",  "You start regenerating.");

			break;
		case 180: case 181:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_ESP;
			muta_desc = _( "�ƥ�ѥ�����ǽ�Ϥ�������",  "You develop a telepathic ability!");

			break;
		case 182: case 183: case 184:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_LIMBER;
			muta_desc = _( "���������ʤ䤫�ˤʤä���",  "Your muscles become limber.");

			break;
		case 185: case 186: case 187:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_ARTHRITIS;
			muta_desc = _( "���᤬�����ˤ߽Ф�����",  "Your joints suddenly hurt.");

			break;
		case 188:
			if (p_ptr->pseikaku == SEIKAKU_LUCKY) break;
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_BAD_LUCK;
			muta_desc = _( "���դ����������������餬���ʤ���Ȥ�ޤ���...",  "There is a malignant black aura surrounding you...");

			break;
		case 189:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_VULN_ELEM;
			muta_desc = _( "̯��̵�����ˤʤä��������롣",  "You feel strangely exposed.");

			break;
		case 190: case 191: case 192:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_MOTION;
			muta_desc = _( "�Τ�ư�������Τˤʤä���",  "You move with new assurance.");

			break;
		case 193:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_GOOD_LUCK;
			muta_desc = _( "���ῼ���򤤥����餬���ʤ���Ȥ�ޤ���...",  "There is a benevolent white aura surrounding you...");

			break;
		default:
			muta_class = NULL;
			muta_which = 0;
		}

		if (muta_class && muta_which)
		{
			if (!(*muta_class & muta_which))
			{
				muta_chosen = TRUE;
			}
		}
		if (muta_chosen == TRUE) break;
	}

	if (!muta_chosen)
	{
		msg_print(_("���̤ˤʤä��������롣", "You feel normal."));

		return FALSE;
	}
	else
	{
		chg_virtue(V_CHANCE, 1);

		/*
		  some races are apt to gain specified mutations
		  This should be allowed only if "choose_mut" is 0.
							--- henkma
		*/
		if(!choose_mut){
			if (p_ptr->prace == RACE_VAMPIRE &&
			  !(p_ptr->muta1 & MUT1_HYPN_GAZE) &&
			   (randint1(10) < 7))
			{
				muta_class = &(p_ptr->muta1);
				muta_which = MUT1_HYPN_GAZE;
			muta_desc = _( "�㤬����Ū�ˤʤä�...",  "Your eyes look mesmerizing...");

			}

			else if (p_ptr->prace == RACE_IMP &&
				 !(p_ptr->muta2 & MUT2_HORNS) &&
				 (randint1(10) < 7))
			  {
				muta_class = &(p_ptr->muta2);
				muta_which = MUT2_HORNS;
			muta_desc = _( "�Ѥ��ۤ��������Ƥ�����",  "Horns pop forth into your forehead!");

			}

			else if (p_ptr->prace == RACE_YEEK &&
				!(p_ptr->muta1 & MUT1_SHRIEK) &&
				(randint1(10) < 7))
			{
				muta_class = &(p_ptr->muta1);
				muta_which = MUT1_SHRIEK;
			muta_desc = _( "���������ʤ궯���ʤä���",  "Your vocal cords get much tougher.");

			}

			else if (p_ptr->prace == RACE_BEASTMAN &&
				!(p_ptr->muta1 & MUT1_POLYMORPH) &&
				(randint1(10) < 2))
			{
				muta_class = &(p_ptr->muta1);
				muta_which = MUT1_POLYMORPH;
			muta_desc = _( "���ʤ������Τ��Ѳ��Ǥ���褦�ˤʤä���",  "Your body seems mutable.");

			}

			else if (p_ptr->prace == RACE_MIND_FLAYER &&
				!(p_ptr->muta2 & MUT2_TENTACLES) &&
				(randint1(10) < 7))
			{
				muta_class = &(p_ptr->muta2);
				muta_which = MUT2_TENTACLES;
			muta_desc = _( "�ٰ��ʿ��꤬���μ������������",  "Evil-looking tentacles sprout from your mouth.");

			}
		}
		msg_print(_("�����Ѱۤ�����", "You mutate!"));

		msg_print(muta_desc);
		*muta_class |= muta_which;

		if (muta_class == &(p_ptr->muta3))
		{
			if (muta_which == MUT3_PUNY)
			{
				if (p_ptr->muta3 & MUT3_HYPER_STR)
				{
					msg_print(_("���ʤ��Ϥ⤦Ķ��Ū�˶����Ϥʤ���", "You no longer feel super-strong!"));

					p_ptr->muta3 &= ~(MUT3_HYPER_STR);
				}
			}
			else if (muta_which == MUT3_HYPER_STR)
			{
				if (p_ptr->muta3 & MUT3_PUNY)
				{
					msg_print(_("���ʤ��Ϥ⤦����ǤϤʤ���", "You no longer feel puny!"));

					p_ptr->muta3 &= ~(MUT3_PUNY);
				}
			}
			else if (muta_which == MUT3_MORONIC)
			{
				if (p_ptr->muta3 & MUT3_HYPER_INT)
				{
					msg_print(_("���ʤ���Ǿ�Ϥ⤦���Υ���ԥ塼���ǤϤʤ���", "Your brain is no longer a living computer."));

					p_ptr->muta3 &= ~(MUT3_HYPER_INT);
				}
			}
			else if (muta_which == MUT3_HYPER_INT)
			{
				if (p_ptr->muta3 & MUT3_MORONIC)
				{
					msg_print(_("���ʤ��Ϥ⤦��������ǤϤʤ���", "You are no longer moronic."));

					p_ptr->muta3 &= ~(MUT3_MORONIC);
				}
			}
			else if (muta_which == MUT3_IRON_SKIN)
			{
				if (p_ptr->muta3 & MUT3_SCALES)
				{
					msg_print(_("�ڤ��ʤ��ʤä���", "You lose your scales."));

					p_ptr->muta3 &= ~(MUT3_SCALES);
				}
				if (p_ptr->muta3 & MUT3_FLESH_ROT)
				{
					msg_print(_("���Τ����𤷤ʤ��ʤä���", "Your flesh rots no longer."));

					p_ptr->muta3 &= ~(MUT3_FLESH_ROT);
				}
				if (p_ptr->muta3 & MUT3_WART_SKIN)
				{
					msg_print(_("ȩ�Υ��ܥ��ܤ��ʤ��ʤä���", "You lose your warts."));

					p_ptr->muta3 &= ~(MUT3_WART_SKIN);
				}
			}
			else if (muta_which == MUT3_WART_SKIN || muta_which == MUT3_SCALES
				|| muta_which == MUT3_FLESH_ROT)
			{
				if (p_ptr->muta3 & MUT3_IRON_SKIN)
				{
					msg_print(_("���ʤ���ȩ�Ϥ⤦Ŵ�ǤϤʤ���", "Your skin is no longer made of steel."));

					p_ptr->muta3 &= ~(MUT3_IRON_SKIN);
				}
			}
			else if (muta_which == MUT3_FEARLESS)
			{
				if (p_ptr->muta2 & MUT2_COWARDICE)
				{
					msg_print(_("���¤Ǥʤ��ʤä���", "You are no longer cowardly."));

					p_ptr->muta2 &= ~(MUT2_COWARDICE);
				}
			}
			else if (muta_which == MUT3_FLESH_ROT)
			{
				if (p_ptr->muta3 & MUT3_REGEN)
				{
					msg_print(_("��®�˲������ʤ��ʤä���", "You stop regenerating."));

					p_ptr->muta3 &= ~(MUT3_REGEN);
				}
			}
			else if (muta_which == MUT3_REGEN)
			{
				if (p_ptr->muta3 & MUT3_FLESH_ROT)
				{
					msg_print(_("���Τ����𤷤ʤ��ʤä���", "Your flesh stops rotting."));

					p_ptr->muta3 &= ~(MUT3_FLESH_ROT);
				}
			}
			else if (muta_which == MUT3_LIMBER)
			{
				if (p_ptr->muta3 & MUT3_ARTHRITIS)
				{
					msg_print(_("���᤬�ˤ��ʤ��ʤä���", "Your joints stop hurting."));

					p_ptr->muta3 &= ~(MUT3_ARTHRITIS);
				}
			}
			else if (muta_which == MUT3_ARTHRITIS)
			{
				if (p_ptr->muta3 & MUT3_LIMBER)
				{
					msg_print(_("���ʤ��Ϥ��ʤ䤫�Ǥʤ��ʤä���", "You no longer feel limber."));

					p_ptr->muta3 &= ~(MUT3_LIMBER);
				}
			}
		}
		else if (muta_class == &(p_ptr->muta2))
		{
			if (muta_which == MUT2_COWARDICE)
			{
				if (p_ptr->muta3 & MUT3_FEARLESS)
				{
					msg_print(_("�����Τ餺�Ǥʤ��ʤä���", "You no longer feel fearless."));

					p_ptr->muta3 &= ~(MUT3_FEARLESS);
				}
			}
			if (muta_which == MUT2_BEAK)
			{
				if (p_ptr->muta2 & MUT2_TRUNK)
				{
					msg_print(_("���ʤ���ɡ�Ϥ⤦�ݤ�ɡ�Τ褦�ǤϤʤ��ʤä���", "Your nose is no longer elephantine."));

					p_ptr->muta2 &= ~(MUT2_TRUNK);
				}
			}
			if (muta_which == MUT2_TRUNK)
			{
				if (p_ptr->muta2 & MUT2_BEAK)
				{
					msg_print(_("�Ť������Х����ʤ��ʤä���", "You no longer have a hard beak."));

					p_ptr->muta2 &= ~(MUT2_BEAK);
				}
			}
		}

		mutant_regenerate_mod = calc_mutant_regenerate_mod();
		p_ptr->update |= PU_BONUS;
		handle_stuff();
		return TRUE;
	}
}

/*!
 * @brief �ץ쥤�䡼���������Ѱۤ������
 * @param choose_mut ���������������Ѱۤ�ID��0�ʤ�Х�����˾õ�
 * @return �ʤ�
 */
bool lose_mutation(int choose_mut)
{
	int attempts_left = 20;
	cptr muta_desc = "";
	bool muta_chosen = FALSE;
	u32b muta_which = 0;
	u32b *muta_class = NULL;

	if (choose_mut) attempts_left = 1;

	while (attempts_left--)
	{
		switch (choose_mut ? choose_mut : randint1(193))
		{
		case 1: case 2: case 3: case 4:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_SPIT_ACID;
			muta_desc = _( "����᤭������ǽ�Ϥ򼺤ä���",  "You lose the ability to spit acid.");

			break;
		case 5: case 6: case 7:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_BR_FIRE;
			muta_desc = _( "��Υ֥쥹���Ǥ�ǽ�Ϥ򼺤ä���",  "You lose the ability to breathe fire.");

			break;
		case 8: case 9:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_HYPN_GAZE;
			muta_desc = _( "���ʤ����ܤϤĤޤ�ʤ��ܤˤʤä���",  "Your eyes look uninteresting.");

			break;
		case 10: case 11:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_TELEKINES;
			muta_desc = _( "ǰư�Ϥ�ʪ��ư����ǽ�Ϥ򼺤ä���",  "You lose the ability to move objects telekinetically.");

			break;
		case 12: case 13: case 14:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_VTELEPORT;
			muta_desc = _( "��ʬ�ΰջפǥƥ�ݡ��Ȥ���ǽ�Ϥ򼺤ä���",  "You lose the power of teleportation at will.");

			break;
		case 15: case 16:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_MIND_BLST;
			muta_desc = _( "���������ǽ�Ϥ򼺤ä���",  "You lose the power of Mind Blast.");

			break;
		case 17: case 18:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_RADIATION;
			muta_desc = _( "���ʤ�������ǽ��ȯ�����ʤ��ʤä���",  "You stop emitting hard radiation.");

			break;
		case 19: case 20:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_VAMPIRISM;
			muta_desc = _( "�۷��ǽ�Ϥ򼺤ä���",  "You are no longer vampiric.");

			break;
		case 21: case 22: case 23:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_SMELL_MET;
			muta_desc = _( "��°�ν������̤��ʤ��ʤä���",  "You no longer smell a metallic odor.");

			break;
		case 24: case 25: case 26: case 27:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_SMELL_MON;
			muta_desc = _( "�Է�ʥ�󥹥����ν������̤��ʤ��ʤä���",  "You no longer smell filthy monsters.");

			break;
		case 28: case 29: case 30:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_BLINK;
			muta_desc = _( "���Υ�ƥ�ݡ��Ȥ�ǽ�Ϥ򼺤ä���",  "You lose the power of minor teleportation.");

			break;
		case 31: case 32:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_EAT_ROCK;
			muta_desc = _( "�ɤ���̣�������˸����ʤ��ʤä���",  "The walls look unappetizing.");

			break;
		case 33: case 34:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_SWAP_POS;
			muta_desc = _( "���ʤ��ϼ�ʬ�η���α�ޤ봶�������롣",  "You feel like staying in your own shoes.");

			break;
		case 35: case 36: case 37:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_SHRIEK;
			muta_desc = _( "���ʤ��������ϼ夯�ʤä���",  "Your vocal cords get much weaker.");

			break;
		case 38: case 39: case 40:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_ILLUMINE;
			muta_desc = _( "���������뤯�Ȥ餹���Ȥ�����ʤ��ʤä���",  "You can no longer light up rooms with your presence.");

			break;
		case 41: case 42:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_DET_CURSE;
			muta_desc = _( "�ٰ�����ˡ�򴶤����ʤ��ʤä���",  "You can no longer feel evil magics.");

			break;
		case 43: case 44: case 45:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_BERSERK;
			muta_desc = _( "����Ǥ�����򴶤��ʤ��ʤä���",  "You no longer feel a controlled rage.");

			break;
		case 46:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_POLYMORPH;
			muta_desc = _( "���ʤ����Τϰ��ꤷ���褦�˸����롣",  "Your body seems stable.");

			break;
		case 47: case 48:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_MIDAS_TCH;
			muta_desc = _( "�ߥ����μ��ǽ�Ϥ򼺤ä���",  "You lose the Midas touch.");

			break;
		case 49:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_GROW_MOLD;
			muta_desc = _( "�������Ӥ������ˤʤä���",  "You feel a sudden dislike for mold.");

			break;
		case 50: case 51: case 52:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_RESIST;
			muta_desc = _( "���Ĥ��פ��ʤä��������롣",  "You feel like you might be vulnerable.");

			break;
		case 53: case 54: case 55:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_EARTHQUAKE;
			muta_desc = _( "���󥸥������ǽ�Ϥ򼺤ä���",  "You lose the ability to wreck the dungeon.");

			break;
		case 56:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_EAT_MAGIC;
			muta_desc = _( "��ˡ�Υ����ƥ�Ϥ⤦��̣�������˸����ʤ��ʤä���",  "Your magic items no longer look delicious.");

			break;
		case 57: case 58:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_WEIGH_MAG;
			muta_desc = _( "���Ϥ򴶤����ʤ��ʤä���",  "You no longer sense magic.");

			break;
		case 59:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_STERILITY;
			muta_desc = _( "��������ΰ��Ȥ���©��ʹ��������",  "You hear a massed sigh of relief.");

			break;
		case 60: case 61:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_PANIC_HIT;
			muta_desc = _( "����������ķ�٤뵤ʬ���ʤ��ʤä���",  "You no longer feel jumpy.");

			break;
		case 62: case 63: case 64:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_DAZZLE;
			muta_desc = _( "�ޤФ椤������ȯ����ǽ�Ϥ򼺤ä���",  "You lose the ability to emit dazzling lights.");

			break;
		case 65: case 66: case 67:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_LASER_EYE;
			muta_desc = _( "�㤬�����δ־Ƥ��դ��ơ��ˤߤ��¤餤����",  "Your eyes burn for a moment, then feel soothed.");

			break;
		case 68: case 69:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_RECALL;
			muta_desc = _( "�����δ֥ۡ��ॷ�å��ˤʤä���",  "You feel briefly homesick.");

			break;
		case 70:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_BANISH;
			muta_desc = _( "�������ܤ���Ϥ򴶤��ʤ��ʤä���",  "You no longer feel a holy wrath.");

			break;
		case 71: case 72:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_COLD_TOUCH;
			muta_desc = _( "�꤬�Ȥ����ʤä���",  "Your hands warm up.");

			break;
		case 73: case 74:
			muta_class = &(p_ptr->muta1);
			muta_which = MUT1_LAUNCHER;
			muta_desc = _( "ʪ���ꤲ��꤬�夯�ʤä��������롣",  "Your throwing arm feels much weaker.");

			break;
		case 75:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_BERS_RAGE;
			muta_desc = _( "��˽����ȯ��ˤ��餵��ʤ��ʤä���",  "You are no longer subject to fits of berserk rage!");

			break;
		case 76:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_COWARDICE;
			muta_desc = _( "�⤦�����������ۤɲ��¤ǤϤʤ��ʤä���",  "You are no longer an incredible coward!");

			break;
		case 77:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_RTELEPORT;
			muta_desc = _( "���ʤ��ΰ��֤Ϥ�����Ū�ˤʤä���",  "Your position seems more certain.");

			break;
		case 78:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_ALCOHOL;
			muta_desc = _( "���ʤ��ϥ��륳�����ʬ�礷�ʤ��ʤä���",  "Your body stops producing alcohol!");

			break;
		case 79:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_HALLU;
			muta_desc = _( "���Ф�Ҥ������������㳲�򵯤����ʤ��ʤä���",  "You are no longer afflicted by a hallucinatory insanity!");

			break;
		case 80:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_FLATULENT;
			muta_desc = _( "�⤦���������Ϥ����ʤ��ʤä���",  "You are no longer subject to uncontrollable flatulence.");

			break;
		case 81: case 82:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_SCOR_TAIL;
			muta_desc = _( "������ο������ʤ��ʤä���",  "You lose your scorpion tail!");

			break;
		case 83: case 84:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_HORNS;
			muta_desc = _( "�ۤ���Ѥ��ä�����",  "Your horns vanish from your forehead!");

			break;
		case 85: case 86:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_BEAK;
			muta_desc = _( "�������̤���ä���",  "Your mouth reverts to normal!");

			break;
		case 87: case 88:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_ATT_DEMON;
			muta_desc = _( "�ǡ���������󤻤ʤ��ʤä���",  "You stop attracting demons.");

			break;
		case 89:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_PROD_MANA;
			muta_desc = _( "������ǽ����ˡ�Υ��ͥ륮����ȯ�����ʤ��ʤä���",  "You stop producing magical energy uncontrollably.");

			break;
		case 90: case 91:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_SPEED_FLUX;
			muta_desc = _( "�ݵ���Ǥʤ��ʤä���",  "You are no longer manic-depressive.");

			break;
		case 92: case 93:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_BANISH_ALL;
			muta_desc = _( "�ظ�˶������Ϥ򴶤��ʤ��ʤä���",  "You no longer feel a terrifying power lurking behind you.");

			break;
		case 94:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_EAT_LIGHT;
			muta_desc = _( "���������뤤�ȴ����롣",  "You feel the world's a brighter place.");

			break;
		case 95: case 96:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_TRUNK;
			muta_desc = _( "ɡ�����̤�Ĺ������ä���",  "Your nose returns to a normal length.");

			break;
		case 97:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_ATT_ANIMAL;
			muta_desc = _( "ưʪ������󤻤ʤ��ʤä���",  "You stop attracting animals.");

			break;
		case 98:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_TENTACLES;
			muta_desc = _( "���꤬�ä�����",  "Your tentacles vanish from your sides.");

			break;
		case 99:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_RAW_CHAOS;
			muta_desc = _( "���Ϥζ��֤����ꤷ���������롣",  "You feel the universe is more stable around you.");

			break;
		case 100: case 101: case 102:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_NORMALITY;
			muta_desc = _( "���̤˴�̯�ʴ��������롣",  "You feel normally strange.");

			break;
		case 103:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_WRAITH;
			muta_desc = _( "���ʤ���ʪ�������ˤ��ä���¸�ߤ��Ƥ��롣",  "You are firmly in the physical world.");

			break;
		case 104:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_POLY_WOUND;
			muta_desc = _( "�Ť������饫�������Ϥ���äƤ��ä���",  "You feel forces of chaos departing your old scars.");

			break;
		case 105:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_WASTING;
			muta_desc = _( "�����ޤ�������¤����ä���",  "You are cured of the horrible wasting disease!");

			break;
		case 106:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_ATT_DRAGON;
			muta_desc = _( "�ɥ饴�������󤻤ʤ��ʤä���",  "You stop attracting dragons.");

			break;
		case 107: case 108:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_WEIRD_MIND;
			muta_desc = _( "�׹ͤ��������������ä���",  "Your thoughts return to boring paths.");

			break;
		case 109:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_NAUSEA;
			muta_desc = _( "�ߤ���ڻ���ʤ��ʤä���",  "Your stomach stops roiling.");

			break;
		case 110: case 111:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_CHAOS_GIFT;
			muta_desc = _( "���٤ο����ζ�̣��椫�ʤ��ʤä���",  "You lose the attention of the chaos deities.");

			break;
		case 112:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_WALK_SHAD;
			muta_desc = _( "ʪ�������������Ƥ��뵤�����롣",  "You feel like you're trapped in reality.");

			break;
		case 113: case 114:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_WARNING;
			muta_desc = _( "�ѥ�Υ����Ǥʤ��ʤä���",  "You no longer feel paranoid.");

			break;
		case 115:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_INVULN;
			muta_desc = _( "̵Ũ���֤�ȯ��򵯤����ʤ��ʤä���",  "You are no longer blessed with fits of invulnerability.");

			break;
		case 116: case 117:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_SP_TO_HP;
			muta_desc = _( "��ˡ�μ�����ȯ��˽����ʤ��ʤä���",  "You are no longer subject to fits of magical healing.");

			break;
		case 118:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_HP_TO_SP;
			muta_desc = _( "�ˤߤ�ȼ���������Ʋ���ȯ��˽����ʤ��ʤä���",  "You are no longer subject to fits of painful clarity.");

			break;
		case 119:
			muta_class = &(p_ptr->muta2);
			muta_which = MUT2_DISARM;
			muta_desc = _( "�Ӥ������礭������ä���",  "Your feet shrink to their former size.");

			break;
		case 120: case 121: case 122:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_HYPER_STR;
			muta_desc = _( "���������̤���ä���",  "Your muscles revert to normal.");

			break;
		case 123: case 124: case 125:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_PUNY;
			muta_desc = _( "���������̤���ä���",  "Your muscles revert to normal.");

			break;
		case 126: case 127: case 128:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_HYPER_INT;
			muta_desc = _( "Ǿ�����̤���ä���",  "Your brain reverts to normal.");

			break;
		case 129: case 130: case 131:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_MORONIC;
			muta_desc = _( "Ǿ�����̤���ä���",  "Your brain reverts to normal.");

			break;
		case 132: case 133:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_RESILIENT;
			muta_desc = _( "���̤ξ��פ�����ä���",  "You become ordinarily resilient again.");

			break;
		case 134: case 135:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_XTRA_FAT;
			muta_desc = _( "����Ū�ʥ������åȤ�����������",  "You benefit from a miracle diet!");

			break;
		case 136: case 137:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_ALBINO;
			muta_desc = _( "����ӥΤǤʤ��ʤä���",  "You are no longer an albino!");

			break;
		case 138: case 139: case 140:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_FLESH_ROT;
			muta_desc = _( "���Τ����Ԥ������µ������ä���",  "Your flesh is no longer afflicted by a rotting disease!");

			break;
		case 141: case 142:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_SILLY_VOI;
			muta_desc = _( "���������̤���ä���",  "Your voice returns to normal.");

			break;
		case 143: case 144:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_BLANK_FAC;
			muta_desc = _( "�����ɡ����ä���",  "Your facial features return.");

			break;
		case 145:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_ILL_NORM;
			muta_desc = _( "�����¤餰���Ƥ�Ǥ��Ф��ʤ��ʤä���",  "You stop projecting a reassuring image.");

			break;
		case 146: case 147: case 148:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_XTRA_EYES;
			muta_desc = _( ";ʬ���ܤ��ä��Ƥ��ޤä���",  "Your extra eyes vanish!");

			break;
		case 149: case 150:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_MAGIC_RES;
			muta_desc = _( "��ˡ�˼夯�ʤä���",  "You become susceptible to magic again.");

			break;
		case 151: case 152: case 153:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_XTRA_NOIS;
			muta_desc = _( "��̯�ʲ���Ω�Ƥʤ��ʤä���",  "You stop making strange noise!");

			break;
		case 154: case 155: case 156:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_INFRAVIS;
			muta_desc = _( "�ֳ������Ϥ��������",  "Your infravision is degraded.");

			break;
		case 157: case 158:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_XTRA_LEGS;
			muta_desc = _( ";ʬ�ʵӤ��ä��Ƥ��ޤä���",  "Your extra legs disappear!");

			break;
		case 159: case 160:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_SHORT_LEG;
			muta_desc = _( "�Ӥ�Ĺ�������̤���ä���",  "Your legs lengthen to normal.");

			break;
		case 161: case 162:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_ELEC_TOUC;
			muta_desc = _( "�Τ���ή��ή��ʤ��ʤä���",  "Electricity stops running through you.");

			break;
		case 163: case 164:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_FIRE_BODY;
			muta_desc = _( "�Τ������ޤ�ʤ��ʤä���",  "Your body is no longer enveloped in flames.");

			break;
		case 165: case 166: case 167:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_WART_SKIN;
			muta_desc = _( "���ܥ��ܤ��ä�����",  "Your warts disappear!");

			break;
		case 168: case 169: case 170:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_SCALES;
			muta_desc = _( "�ڤ��ä�����",  "Your scales vanish!");

			break;
		case 171: case 172:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_IRON_SKIN;
			muta_desc = _( "ȩ�����ˤ�ɤä���",  "Your skin reverts to flesh!");

			break;
		case 173: case 174:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_WINGS;
			muta_desc = _( "����α���������������",  "Your wings fall off.");

			break;
		case 175: case 176: case 177:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_FEARLESS;
			muta_desc = _( "�ƤӶ��ݤ򴶤���褦�ˤʤä���",  "You begin to feel fear again.");

			break;
		case 178: case 179:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_REGEN;
			muta_desc = _( "��®�������ʤ��ʤä���",  "You stop regenerating.");

			break;
		case 180: case 181:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_ESP;
			muta_desc = _( "�ƥ�ѥ�����ǽ�Ϥ򼺤ä���",  "You lose your telepathic ability!");

			break;
		case 182: case 183: case 184:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_LIMBER;
			muta_desc = _( "�������Ť��ʤä���",  "Your muscles stiffen.");

			break;
		case 185: case 186: case 187:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_ARTHRITIS;
			muta_desc = _( "���᤬�ˤ��ʤ��ʤä���",  "Your joints stop hurting.");

			break;
		case 188:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_BAD_LUCK;
			muta_desc = _( "����������ϱ������ƾä�����",  "Your black aura swirls and fades.");

			break;
		case 189:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_VULN_ELEM;
			muta_desc = _( "̵�����ʴ����Ϥʤ��ʤä���",  "You feel less exposed.");

			break;
		case 190: case 191: case 192:
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_MOTION;
			muta_desc = _( "ư������Τ����ʤ��ʤä���",  "You move with less assurance.");

			break;
		case 193:
			if (p_ptr->pseikaku == SEIKAKU_LUCKY) break;
			muta_class = &(p_ptr->muta3);
			muta_which = MUT3_GOOD_LUCK;
			muta_desc = _( "�򤤥�����ϵ����ƾä�����",  "Your white aura shimmers and fades.");

			break;
		default:
			muta_class = NULL;
			muta_which = 0;
		}

		if (muta_class && muta_which)
		{
			if (*(muta_class) & muta_which)
			{
				muta_chosen = TRUE;
			}
		}
		if (muta_chosen == TRUE) break;
	}

	if (!muta_chosen)
	{
		return FALSE;
	}
	else
	{
		msg_print(muta_desc);
		*(muta_class) &= ~(muta_which);

		p_ptr->update |= PU_BONUS;
		handle_stuff();
		mutant_regenerate_mod = calc_mutant_regenerate_mod();
		return TRUE;
	}
}

/*!
 * @brief �ե�����ݥ��󥿤��̤��������Ѱۤΰ�������Ϥ���
 * @param OutFile ������ե�����ݥ���
 * @return �ʤ�
 */
void dump_mutations(FILE *OutFile)
{
	if (!OutFile) return;

	if (p_ptr->muta1)
	{
		if (p_ptr->muta1 & MUT1_SPIT_ACID)
		{
			fprintf(OutFile, _(" ���ʤ��ϻ���᤭�����뤳�Ȥ��Ǥ��롣(���᡼�� ��٥�X1)\n", " You can spit acid (dam lvl).\n"));

		}
		if (p_ptr->muta1 & MUT1_BR_FIRE)
		{
			fprintf(OutFile, _(" ���ʤ��ϱ�Υ֥쥹���Ǥ����Ȥ��Ǥ��롣(���᡼�� ��٥�X2)\n", " You can breathe fire (dam lvl * 2).\n"));

		}
		if (p_ptr->muta1 & MUT1_HYPN_GAZE)
		{
			fprintf(OutFile, _(" ���ʤ����ˤߤϺ�̲���̤��ġ�\n", " Your gaze is hypnotic.\n"));

		}
		if (p_ptr->muta1 & MUT1_TELEKINES)
		{
			fprintf(OutFile, _(" ���ʤ���ǰư�Ϥ��äƤ��롣\n", " You are telekinetic.\n"));

		}
		if (p_ptr->muta1 & MUT1_VTELEPORT)
		{
			fprintf(OutFile, _(" ���ʤ��ϼ�ʬ�ΰջפǥƥ�ݡ��ȤǤ��롣\n", " You can teleport at will.\n"));

		}
		if (p_ptr->muta1 & MUT1_MIND_BLST)
		{
			fprintf(OutFile, _(" ���ʤ���Ũ����������Ǥ��롣\n", " You can Mind Blast your enemies.\n"));

		}
		if (p_ptr->muta1 & MUT1_RADIATION)
		{
			fprintf(OutFile, _(" ���ʤ��ϼ�ʬ�ΰջפ�����ǽ��ȯ�����뤳�Ȥ��Ǥ��롣\n", " You can emit hard radiation at will.\n"));

		}
		if (p_ptr->muta1 & MUT1_VAMPIRISM)
		{
			fprintf(OutFile, _(" ���ʤ��ϵ۷쵴�Τ褦��Ũ������̿�Ϥ�ۼ����뤳�Ȥ��Ǥ��롣\n", " You can drain life from a foe like a vampire.\n"));

		}
		if (p_ptr->muta1 & MUT1_SMELL_MET)
		{
			fprintf(OutFile, _(" ���ʤ��϶᤯�ˤ��뵮��°�򤫤�ʬ���뤳�Ȥ��Ǥ��롣\n", " You can smell nearby precious metal.\n"));

		}
		if (p_ptr->muta1 & MUT1_SMELL_MON)
		{
			fprintf(OutFile, _(" ���ʤ��϶᤯�Υ�󥹥�����¸�ߤ򤫤�ʬ���뤳�Ȥ��Ǥ��롣\n", " You can smell nearby monsters.\n"));

		}
		if (p_ptr->muta1 & MUT1_BLINK)
		{
			fprintf(OutFile, _(" ���ʤ���û����Υ��ƥ�ݡ��ȤǤ��롣\n", " You can teleport yourself short distances.\n"));

		}
		if (p_ptr->muta1 & MUT1_EAT_ROCK)
		{
			fprintf(OutFile, _(" ���ʤ��ϹŤ���򿩤٤뤳�Ȥ��Ǥ��롣\n", " You can consume solid rock.\n"));

		}
		if (p_ptr->muta1 & MUT1_SWAP_POS)
		{
			fprintf(OutFile, _(" ���ʤ���¾�μԤȾ��������ؤ�뤳�Ȥ��Ǥ��롣\n", " You can switch locations with another being.\n"));

		}
		if (p_ptr->muta1 & MUT1_SHRIEK)
		{
			fprintf(OutFile, _(" ���ʤ��ϿȤ��Ӥ����Ķ�������ȯ���뤳�Ȥ��Ǥ��롣\n", " You can emit a horrible shriek.\n"));

		}
		if (p_ptr->muta1 & MUT1_ILLUMINE)
		{
			fprintf(OutFile, _(" ���ʤ������뤤�������Ĥ��Ȥ��Ǥ��롣\n", " You can emit bright light.\n"));

		}
		if (p_ptr->muta1 & MUT1_DET_CURSE)
		{
			fprintf(OutFile, _(" ���ʤ��ϼٰ�����ˡ�δ��򴶤��Ȥ뤳�Ȥ��Ǥ��롣\n", " You can feel the danger of evil magic.\n"));

		}
		if (p_ptr->muta1 & MUT1_BERSERK)
		{
			fprintf(OutFile, _(" ���ʤ��ϼ�ʬ�ΰջפǶ�����Ʈ���֤ˤʤ뤳�Ȥ��Ǥ��롣\n", " You can drive yourself into a berserk frenzy.\n"));

		}
		if (p_ptr->muta1 & MUT1_POLYMORPH)
		{
			fprintf(OutFile, _(" ���ʤ��ϼ�ʬ�ΰջ֤��Ѳ��Ǥ��롣\n", " You can polymorph yourself at will.\n"));

		}
		if (p_ptr->muta1 & MUT1_MIDAS_TCH)
		{
			fprintf(OutFile, _(" ���ʤ����̾異���ƥ�����Ѥ��뤳�Ȥ��Ǥ��롣\n", " You can turn ordinary items to gold.\n"));

		}
		if (p_ptr->muta1 & MUT1_GROW_MOLD)
		{
			fprintf(OutFile, _(" ���ʤ��ϼ��Ϥ˥��Υ������䤹���Ȥ��Ǥ��롣\n", " You can cause mold to grow near you.\n"));

		}
		if (p_ptr->muta1 & MUT1_RESIST)
		{
			fprintf(OutFile, _(" ���ʤ��ϸ��Ǥι�����Ф��ƿȤ�Ť����뤳�Ȥ��Ǥ��롣\n", " You can harden yourself to the ravages of the elements.\n"));

		}
		if (p_ptr->muta1 & MUT1_EARTHQUAKE)
		{
			fprintf(OutFile, _(" ���ʤ��ϼ��ϤΥ��󥸥������������뤳�Ȥ��Ǥ��롣\n", " You can bring down the dungeon around your ears.\n"));

		}
		if (p_ptr->muta1 & MUT1_EAT_MAGIC)
		{
			fprintf(OutFile, _(" ���ʤ�����ˡ�Υ��ͥ륮����ʬ��ʪ�Ȥ��ƻ��ѤǤ��롣\n", " You can consume magic energy for your own use.\n"));

		}
		if (p_ptr->muta1 & MUT1_WEIGH_MAG)
		{
			fprintf(OutFile, _(" ���ʤ��ϼ�ʬ�˱ƶ���Ϳ������ˡ���Ϥ򴶤��뤳�Ȥ��Ǥ��롣\n", " You can feel the strength of the magics affecting you.\n"));

		}
		if (p_ptr->muta1 & MUT1_STERILITY)
		{
			fprintf(OutFile, _(" ���ʤ��Ͻ���Ū������ǽ�򵯤������Ȥ��Ǥ��롣\n", " You can cause mass impotence.\n"));

		}
		if (p_ptr->muta1 & MUT1_PANIC_HIT)
		{
			fprintf(OutFile, _(" ���ʤ��Ϲ��⤷����Ȥ��뤿��ƨ���뤳�Ȥ��Ǥ��롣\n", " You can run for your life after hitting something.\n"));

		}
		if (p_ptr->muta1 & MUT1_DAZZLE)
		{
			fprintf(OutFile, _(" ���ʤ��Ϻ�������ܤ��������������ǽ��ȯ�����뤳�Ȥ��Ǥ��롣 \n", " You can emit confusing, blinding radiation.\n"));

		}
		if (p_ptr->muta1 & MUT1_LASER_EYE)
		{
			fprintf(OutFile, _(" ���ʤ����ܤ���졼����������ȯ�ͤ��뤳�Ȥ��Ǥ��롣\n", " Your eyes can fire laser beams.\n"));

		}
		if (p_ptr->muta1 & MUT1_RECALL)
		{
			fprintf(OutFile, _(" ���ʤ��ϳ��ȥ��󥸥��δ֤�Ԥ��褹�뤳�Ȥ��Ǥ��롣\n", " You can travel between town and the depths.\n"));

		}
		if (p_ptr->muta1 & MUT1_BANISH)
		{
			fprintf(OutFile, _(" ���ʤ��ϼٰ��ʥ�󥹥������Ϲ�����Ȥ����Ȥ��Ǥ��롣\n", " You can send evil creatures directly to Hell.\n"));

		}
		if (p_ptr->muta1 & MUT1_COLD_TOUCH)
		{
			fprintf(OutFile, _(" ���ʤ���ʪ�򿨤ä���餻�뤳�Ȥ��Ǥ��롣\n", " You can freeze things with a touch.\n"));

		}
		if (p_ptr->muta1 & MUT1_LAUNCHER)
		{
			fprintf(OutFile, _(" ���ʤ��ϥ����ƥ���϶����ꤲ�뤳�Ȥ��Ǥ��롣\n", " You can hurl objects with great force.\n"));

		}
	}

	if (p_ptr->muta2)
	{
		if (p_ptr->muta2 & MUT2_BERS_RAGE)
		{
			fprintf(OutFile, _(" ���ʤ��϶���β���ȯ��򵯤�����\n", " You are subject to berserker fits.\n"));

		}
		if (p_ptr->muta2 & MUT2_COWARDICE)
		{
			fprintf(OutFile, _(" ���ʤ��ϻ������¤ˤʤ롣\n", " You are subject to cowardice.\n"));

		}
		if (p_ptr->muta2 & MUT2_RTELEPORT)
		{
			fprintf(OutFile, _(" ���ʤ��ϥ�����˥ƥ�ݡ��Ȥ��롣\n", " You are teleporting randomly.\n"));

		}
		if (p_ptr->muta2 & MUT2_ALCOHOL)
		{
			fprintf(OutFile, _(" ���ʤ����Τϥ��륳�����ʬ�礹�롣\n", " Your body produces alcohol.\n"));

		}
		if (p_ptr->muta2 & MUT2_HALLU)
		{
			fprintf(OutFile, _(" ���ʤ��ϸ��Ф������������������˿�����Ƥ��롣\n", " You have a hallucinatory insanity.\n"));

		}
		if (p_ptr->muta2 & MUT2_FLATULENT)
		{
			fprintf(OutFile, _(" ���ʤ�������Ǥ��ʤ����������򤳤���\n", " You are subject to uncontrollable flatulence.\n"));

		}
		if (p_ptr->muta2 & MUT2_PROD_MANA)
		{
			fprintf(OutFile, _(" ���ʤ���������ǽ����ˡ�Υ��ͥ륮����ȯ���Ƥ��롣\n", " You are producing magical energy uncontrollably.\n"));

		}
		if (p_ptr->muta2 & MUT2_ATT_DEMON)
		{
			fprintf(OutFile, _(" ���ʤ��ϥǡ���������Ĥ��롣\n", " You attract demons.\n"));

		}
		if (p_ptr->muta2 & MUT2_SCOR_TAIL)
		{
			fprintf(OutFile, _(" ���ʤ��ϥ�����ο����������Ƥ��롣(�ǡ����᡼�� 3d7)\n", " You have a scorpion tail (poison, 3d7).\n"));

		}
		if (p_ptr->muta2 & MUT2_HORNS)
		{
			fprintf(OutFile, _(" ���ʤ��ϳѤ������Ƥ��롣(���᡼�� 2d6)\n", " You have horns (dam. 2d6).\n"));

		}
		if (p_ptr->muta2 & MUT2_BEAK)
		{
			fprintf(OutFile, _(" ���ʤ��ϥ����Х��������Ƥ��롣(���᡼�� 2d4)\n", " You have a beak (dam. 2d4).\n"));

		}
		if (p_ptr->muta2 & MUT2_SPEED_FLUX)
		{
			fprintf(OutFile, _(" ���ʤ��ϥ�������᤯ư�������٤�ư�����ꤹ�롣\n", " You move faster or slower randomly.\n"));

		}
		if (p_ptr->muta2 & MUT2_BANISH_ALL)
		{
			fprintf(OutFile, _(" ���ʤ��ϻ����᤯�Υ�󥹥�������Ǥ����롣\n", " You sometimes cause nearby creatures to vanish.\n"));

		}
		if (p_ptr->muta2 & MUT2_EAT_LIGHT)
		{
			fprintf(OutFile, _(" ���ʤ��ϻ������Ϥθ���ۼ����Ʊ��ܤˤ��롣\n", " You sometimes feed off of the light around you.\n"));

		}
		if (p_ptr->muta2 & MUT2_TRUNK)
		{
			fprintf(OutFile, _(" ���ʤ��ϾݤΤ褦��ɡ����äƤ��롣(���᡼�� 1d4)\n", " You have an elephantine trunk (dam 1d4).\n"));

		}
		if (p_ptr->muta2 & MUT2_ATT_ANIMAL)
		{
			fprintf(OutFile, _(" ���ʤ���ưʪ������Ĥ��롣\n", " You attract animals.\n"));

		}
		if (p_ptr->muta2 & MUT2_TENTACLES)
		{
			fprintf(OutFile, _(" ���ʤ��ϼٰ��ʿ������äƤ��롣(���᡼�� 2d5)\n", " You have evil looking tentacles (dam 2d5).\n"));

		}
		if (p_ptr->muta2 & MUT2_RAW_CHAOS)
		{
			fprintf(OutFile, _(" ���ʤ��Ϥ��Ф��н㥫��������ޤ�롣\n", " You occasionally are surrounded with raw chaos.\n"));

		}
		if (p_ptr->muta2 & MUT2_NORMALITY)
		{
			fprintf(OutFile, _(" ���ʤ����Ѱۤ��Ƥ��������������Ƥ��Ƥ��롣\n", " You may be mutated, but you're recovering.\n"));

		}
		if (p_ptr->muta2 & MUT2_WRAITH)
		{
			fprintf(OutFile, _(" ���ʤ������Τ�ͩ�β���������β������ꤹ�롣\n", " You fade in and out of physical reality.\n"));

		}
		if (p_ptr->muta2 & MUT2_POLY_WOUND)
		{
			fprintf(OutFile, _(" ���ʤ��η򹯤ϥ��������Ϥ˱ƶ�������롣\n", " Your health is subject to chaotic forces.\n"));

		}
		if (p_ptr->muta2 & MUT2_WASTING)
		{
			fprintf(OutFile, _(" ���ʤ��Ͽ�夹�붲�����µ��ˤ����äƤ��롣\n", " You have a horrible wasting disease.\n"));

		}
		if (p_ptr->muta2 & MUT2_ATT_DRAGON)
		{
			fprintf(OutFile, _(" ���ʤ��ϥɥ饴�������Ĥ��롣\n", " You attract dragons.\n"));

		}
		if (p_ptr->muta2 & MUT2_WEIRD_MIND)
		{
			fprintf(OutFile, _(" ���ʤ��������ϥ�����˳��礷����̾������ꤷ�Ƥ��롣\n", " Your mind randomly expands and contracts.\n"));

		}
		if (p_ptr->muta2 & MUT2_NAUSEA)
		{
			fprintf(OutFile, _(" ���ʤ��ΰߤ���������夭���ʤ���\n", " You have a seriously upset stomach.\n"));

		}
		if (p_ptr->muta2 & MUT2_CHAOS_GIFT)
		{
			fprintf(OutFile, _(" ���ʤ��ϥ������μ��⤫��˫���򤦤��Ȥ롣\n", " Chaos deities give you gifts.\n"));

		}
		if (p_ptr->muta2 & MUT2_WALK_SHAD)
		{
			fprintf(OutFile, _(" ���ʤ��Ϥ��Ф���¾�Ρֱơפ��¤����ࡣ\n", " You occasionally stumble into other shadows.\n"));

		}
		if (p_ptr->muta2 & MUT2_WARNING)
		{
			fprintf(OutFile, _(" ���ʤ���Ũ�˴ؤ���ٹ�򴶤��롣\n", " You receive warnings about your foes.\n"));

		}
		if (p_ptr->muta2 & MUT2_INVULN)
		{
			fprintf(OutFile, _(" ���ʤ��ϻ����餱�Τ餺�ʵ�ʬ�ˤʤ롣\n", " You occasionally feel invincible.\n"));

		}
		if (p_ptr->muta2 & MUT2_SP_TO_HP)
		{
			fprintf(OutFile, _(" ���ʤ��ϻ����줬�����ˤɤä�ή��롣\n", " Your blood sometimes rushes to your muscles.\n"));

		}
		if (p_ptr->muta2 & MUT2_HP_TO_SP)
		{
			fprintf(OutFile, _(" ���ʤ��ϻ���Ƭ�˷줬�ɤä�ή��롣\n", " Your blood sometimes rushes to your head.\n"));

		}
		if (p_ptr->muta2 & MUT2_DISARM)
		{
			fprintf(OutFile, _(" ���ʤ��Ϥ褯�ĤޤŤ���ʪ����Ȥ���\n", " You occasionally stumble and drop things.\n"));

		}
	}

	if (p_ptr->muta3)
	{
		if (p_ptr->muta3 & MUT3_HYPER_STR)
		{
			fprintf(OutFile, _(" ���ʤ���Ķ��Ū�˶�����(����+4)\n", " You are superhumanly strong (+4 STR).\n"));

		}
		if (p_ptr->muta3 & MUT3_PUNY)
		{
			fprintf(OutFile, _(" ���ʤ��ϵ������(����-4)\n", " You are puny (-4 STR).\n"));

		}
		if (p_ptr->muta3 & MUT3_HYPER_INT)
		{
			fprintf(OutFile, _(" ���ʤ���Ǿ�����Υ���ԥ塼������(��ǽ������+4)\n", " Your brain is a living computer (+4 INT/WIS).\n"));

		}
		if (p_ptr->muta3 & MUT3_MORONIC)
		{
			fprintf(OutFile, _(" ���ʤ��������������(��ǽ������-4)\n", " You are moronic (-4 INT/WIS).\n"));

		}
		if (p_ptr->muta3 & MUT3_RESILIENT)
		{
			fprintf(OutFile, _(" ���ʤ����Τ����������٤�Ǥ��롣(�ѵ�+4)\n", " You are very resilient (+4 CON).\n"));

		}
		if (p_ptr->muta3 & MUT3_XTRA_FAT)
		{
			fprintf(OutFile, _(" ���ʤ��϶�ü�����äƤ��롣(�ѵ�+2,���ԡ���-2)\n", " You are extremely fat (+2 CON, -2 speed).\n"));

		}
		if (p_ptr->muta3 & MUT3_ALBINO)
		{
			fprintf(OutFile, _(" ���ʤ��ϥ���ӥΤ���(�ѵ�-4)\n", " You are albino (-4 CON).\n"));

		}
		if (p_ptr->muta3 & MUT3_FLESH_ROT)
		{
			fprintf(OutFile, _(" ���ʤ������Τ����Ԥ��Ƥ��롣(�ѵ�-2,̥��-1)\n", " Your flesh is rotting (-2 CON, -1 CHR).\n"));

		}
		if (p_ptr->muta3 & MUT3_SILLY_VOI)
		{
			fprintf(OutFile, _(" ���ʤ������ϴ�ȴ���ʥ�������������(̥��-4)\n", " Your voice is a silly squeak (-4 CHR).\n"));

		}
		if (p_ptr->muta3 & MUT3_BLANK_FAC)
		{
			fprintf(OutFile, _(" ���ʤ��ϤΤäڤ�ܤ�����(̥��-1)\n", " Your face is featureless (-1 CHR).\n"));

		}
		if (p_ptr->muta3 & MUT3_ILL_NORM)
		{
			fprintf(OutFile, _(" ���ʤ��ϸ��Ƥ�ʤ���Ƥ��롣\n", " Your appearance is masked with illusion.\n"));

		}
		if (p_ptr->muta3 & MUT3_XTRA_EYES)
		{
			fprintf(OutFile, _(" ���ʤ���;ʬ����Ĥ��ܤ���äƤ��롣(õ��+15)\n", " You have an extra pair of eyes (+15 search).\n"));

		}
		if (p_ptr->muta3 & MUT3_MAGIC_RES)
		{
			fprintf(OutFile, _(" ���ʤ�����ˡ�ؤ��������äƤ��롣\n", " You are resistant to magic.\n"));

		}
		if (p_ptr->muta3 & MUT3_XTRA_NOIS)
		{
			fprintf(OutFile, _(" ���ʤ����Ѥʲ���ȯ���Ƥ��롣(��̩-3)\n", " You make a lot of strange noise (-3 stealth).\n"));

		}
		if (p_ptr->muta3 & MUT3_INFRAVIS)
		{
			fprintf(OutFile, _(" ���ʤ��������餷���ֳ������Ϥ���äƤ��롣(+3)\n", " You have remarkable infravision (+3).\n"));

		}
		if (p_ptr->muta3 & MUT3_XTRA_LEGS)
		{
			fprintf(OutFile, _(" ���ʤ���;ʬ�����ܤ�­�������Ƥ��롣(��®+3)\n", " You have an extra pair of legs (+3 speed).\n"));

		}
		if (p_ptr->muta3 & MUT3_SHORT_LEG)
		{
			fprintf(OutFile, _(" ���ʤ���­��û���͵�����(��®-3)\n", " Your legs are short stubs (-3 speed).\n"));

		}
		if (p_ptr->muta3 & MUT3_ELEC_TOUC)
		{
			fprintf(OutFile, _(" ���ʤ��η�ɤˤ���ή��ή��Ƥ��롣\n", " Electricity is running through your veins.\n"));

		}
		if (p_ptr->muta3 & MUT3_FIRE_BODY)
		{
			fprintf(OutFile, _(" ���ʤ����Τϱ�ˤĤĤޤ�Ƥ��롣\n", " Your body is enveloped in flames.\n"));

		}
		if (p_ptr->muta3 & MUT3_WART_SKIN)
		{
			fprintf(OutFile, _(" ���ʤ���ȩ�ϥ��ܤ�����Ƥ��롣(̥��-2, AC+5)\n", " Your skin is covered with warts (-2 CHR, +5 AC).\n"));

		}
		if (p_ptr->muta3 & MUT3_SCALES)
		{
			fprintf(OutFile, _(" ���ʤ���ȩ���ڤˤʤäƤ��롣(̥��-1, AC+10)\n", " Your skin has turned into scales (-1 CHR, +10 AC).\n"));

		}
		if (p_ptr->muta3 & MUT3_IRON_SKIN)
		{
			fprintf(OutFile, _(" ���ʤ���ȩ��Ŵ�ǤǤ��Ƥ��롣(����-1, AC+25)\n", " Your skin is made of steel (-1 DEX, +25 AC).\n"));

		}
		if (p_ptr->muta3 & MUT3_WINGS)
		{
			fprintf(OutFile, _(" ���ʤ��ϱ�����äƤ��롣\n", " You have wings.\n"));

		}
		if (p_ptr->muta3 & MUT3_FEARLESS)
		{
			fprintf(OutFile, _(" ���ʤ����������ݤ򴶤��ʤ���\n", " You are completely fearless.\n"));

		}
		if (p_ptr->muta3 & MUT3_REGEN)
		{
			fprintf(OutFile, _(" ���ʤ��ϵ�®�˲������롣\n", " You are regenerating.\n"));

		}
		if (p_ptr->muta3 & MUT3_ESP)
		{
			fprintf(OutFile, _(" ���ʤ��ϥƥ�ѥ�������äƤ��롣\n", " You are telepathic.\n"));

		}
		if (p_ptr->muta3 & MUT3_LIMBER)
		{
			fprintf(OutFile, _(" ���ʤ����Τ����ˤ��ʤ䤫����(����+3)\n", " Your body is very limber (+3 DEX).\n"));

		}
		if (p_ptr->muta3 & MUT3_ARTHRITIS)
		{
			fprintf(OutFile, _(" ���ʤ��Ϥ��Ĥ������ˤߤ򴶤��Ƥ��롣(����-3)\n", " Your joints ache constantly (-3 DEX).\n"));

		}
		if (p_ptr->muta3 & MUT3_VULN_ELEM)
		{
			fprintf(OutFile, _(" ���ʤ��ϸ��Ǥι���˼夤��\n", " You are susceptible to damage from the elements.\n"));

		}
		if (p_ptr->muta3 & MUT3_MOTION)
		{
			fprintf(OutFile, _(" ���ʤ���ư������Τ��϶�����(��̩+1)\n", " Your movements are precise and forceful (+1 STL).\n"));

		}
		if (p_ptr->muta3 & MUT3_GOOD_LUCK)
		{
			fprintf(OutFile, _(" ���ʤ����򤤥�����ˤĤĤޤ�Ƥ��롣\n", " There is a white aura surrounding you.\n"));

		}
		if (p_ptr->muta3 & MUT3_BAD_LUCK)
		{
			fprintf(OutFile, _(" ���ʤ��Ϲ���������ˤĤĤޤ�Ƥ��롣\n", " There is a black aura surrounding you.\n"));

		}
	}
}

/*!
 * @brief �����Ѱ�ɽ�����ޥ�ɤμ��� / List mutations we have...
 * @return �ʤ�
 */
void do_cmd_knowledge_mutations(void)
{
	FILE *fff;
	char file_name[1024];

	/* Open a new file */
	fff = my_fopen_temp(file_name, 1024);

	/* Dump the mutations to file */
	if (fff) dump_mutations(fff);

	/* Close the file */
	my_fclose(fff);

	/* Display the file contents */
	show_file(TRUE, file_name, _("�����Ѱ�", "Mutations"), 0, 0);

	/* Remove the file */
	fd_kill(file_name);
}

/*!
 * @brief ���ʤ�32�ӥå������Υӥåȿ����֤���
 * @param x �ӥåȿ���Ĵ�٤����ѿ�
 * @return �ӥåȿ�
 */
int count_bits(u32b x)
{
	int n = 0;

	if (x) do
	{
		n++;
	}
	while (0 != (x = x&(x-1)));

	return (n);
}

/*!
 * @brief ���ߥץ쥤�䡼���Ƥ��������Ѱۤο����֤���
 * @return �������Ƥ��������Ѱۤο�
 */
static int count_mutations(void)
{
	return (count_bits(p_ptr->muta1) +
		count_bits(p_ptr->muta2) +
		count_bits(p_ptr->muta3));
}


/*!
 * @brief �����Ѱۤˤ�뼫�������ڥʥ�ƥ���ѡ�������ͤ��֤� /
 * Return the modifier to the regeneration rate (in percent)
 * @return �ڥʥ�ƥ�����(%)
 */
int calc_mutant_regenerate_mod(void)
{
	int regen;
	int mod = 10;
	int count = count_mutations();

	/*
	 * Beastman get 10 "free" mutations and
	 * only 5% decrease per additional mutation
	 */

	if (p_ptr->pseikaku == SEIKAKU_LUCKY) count--;
	if (p_ptr->prace == RACE_BEASTMAN)
	{
		count -= 10;
		mod = 5;
	}

	/* No negative modifier */
	if (count <= 0) return 100;

	regen = 100 - count * mod;

	/* Max. 90% decrease in regeneration speed */
	if (regen < 10) regen = 10;

	return (regen);
}

/*!
 * @brief �����Ѱۥ쥤������Ǹ���Ȥ�����Ԥä��ݤ˲Τ���Ѥ���ߤ��� /
 * @return �ʤ�
 */
void mutation_stop_mouth()
{
	if (music_singing_any()) stop_singing();
	if (hex_spelling_any()) stop_hex_spell_all();
}


/*!
 * @brief �����ѰۤΥ쥤�������̼���
 * @return ȯư�����������Ѱۥ쥤������ID
 * @return �쥤������¹Ԥ������TRUE������󥻥뤷�����FALSE���֤�
 */
bool mutation_power_aux(u32b power)
{
	int     dir = 0;
	int     lvl = p_ptr->lev;


	switch (power)
	{
		case MUT1_SPIT_ACID:
			if (!get_aim_dir(&dir)) return FALSE;
			mutation_stop_mouth();
			msg_print(_("�����Ǥ�������...", "You spit acid..."));

			fire_ball(GF_ACID, dir, lvl, 1 + (lvl / 30));
			break;

		case MUT1_BR_FIRE:
			if (!get_aim_dir(&dir)) return FALSE;
			mutation_stop_mouth();
			msg_print(_("���ʤ��ϲб�Υ֥쥹���Ǥ���...", "You breathe fire..."));

			fire_ball(GF_FIRE, dir, lvl * 2, 1 + (lvl / 20));
			break;

		case MUT1_HYPN_GAZE:
			if (!get_aim_dir(&dir)) return FALSE;
			msg_print(_("���ʤ����ܤϸ���Ū�ˤʤä�...", "Your eyes look mesmerizing..."));

			(void)charm_monster(dir, lvl);
			break;

		case MUT1_TELEKINES:
			if (!get_aim_dir(&dir)) return FALSE;
			msg_print(_("���椷�Ƥ���...", "You concentrate..."));

			fetch(dir, lvl * 10, TRUE);
			break;

		case MUT1_VTELEPORT:
			msg_print(_("���椷�Ƥ���...", "You concentrate..."));

			teleport_player(10 + 4 * lvl, 0L);
			break;

		case MUT1_MIND_BLST:
			if (!get_aim_dir(&dir)) return FALSE;
			msg_print(_("���椷�Ƥ���...", "You concentrate..."));

			fire_bolt(GF_PSI, dir, damroll(3 + ((lvl - 1) / 5), 3));
			break;

		case MUT1_RADIATION:
			msg_print(_("�Τ�������ǽ��ȯ��������", "Radiation flows from your body!"));

			fire_ball(GF_NUKE, 0, (lvl * 2), 3 + (lvl / 20));
			break;

		case MUT1_VAMPIRISM:
			{
				int x, y, dummy;
				cave_type *c_ptr;

				/* Only works on adjacent monsters */
				if (!get_rep_dir2(&dir)) return FALSE;
				y = p_ptr->y + ddy[dir];
				x = p_ptr->x + ddx[dir];
				c_ptr = &cave[y][x];

				mutation_stop_mouth();

				if (!(c_ptr->m_idx))
				{
					msg_print(_("����ʤ����˳��ߤĤ�����", "You bite into thin air!"));

					break;
				}

				msg_print(_("���ʤ��ϥ˥��Ȥ��Ʋ��त��...", "You grin and bare your fangs..."));


				dummy = lvl * 2;

				if (drain_life(dir, dummy))
				{
					if (p_ptr->food < PY_FOOD_FULL)
						/* No heal if we are "full" */
						(void)hp_player(dummy);
					else
						msg_print(_("���ʤ��϶�ʢ�ǤϤ���ޤ���", "You were not hungry."));

					/* Gain nutritional sustenance: 150/hp drained */
					/* A Food ration gives 5000 food points (by contrast) */
					/* Don't ever get more than "Full" this way */
					/* But if we ARE Gorged,  it won't cure us */
					dummy = p_ptr->food + MIN(5000, 100 * dummy);
					if (p_ptr->food < PY_FOOD_MAX)   /* Not gorged already */
						(void)set_food(dummy >= PY_FOOD_MAX ? PY_FOOD_MAX-1 : dummy);
				}
				else
					msg_print(_("�������Ҥɤ�̣����", "Yechh. That tastes foul."));

			}
			break;

		case MUT1_SMELL_MET:
			mutation_stop_mouth();
			(void)detect_treasure(DETECT_RAD_DEFAULT);
			break;

		case MUT1_SMELL_MON:
			mutation_stop_mouth();
			(void)detect_monsters_normal(DETECT_RAD_DEFAULT);
			break;

		case MUT1_BLINK:
			teleport_player(10, 0L);
			break;

		case MUT1_EAT_ROCK:
			{
				int x, y;
				cave_type *c_ptr;
				feature_type *f_ptr, *mimic_f_ptr;

				if (!get_rep_dir2(&dir)) return FALSE;
				y = p_ptr->y + ddy[dir];
				x = p_ptr->x + ddx[dir];
				c_ptr = &cave[y][x];
				f_ptr = &f_info[c_ptr->feat];
				mimic_f_ptr = &f_info[get_feat_mimic(c_ptr)];

				mutation_stop_mouth();

				if (!have_flag(mimic_f_ptr->flags, FF_HURT_ROCK))
				{
					msg_print(_("�����Ϸ��Ͽ��٤��ʤ���", "You cannot eat this feature."));
					break;
				}
				else if (have_flag(f_ptr->flags, FF_PERMANENT))
				{
					msg_format(_("���Ƥá�����%s�Ϥ��ʤ��λ����Ť���", "Ouch!  This %s is harder than your teeth!"), f_name + mimic_f_ptr->name);
					break;
				}
				else if (c_ptr->m_idx)
				{
					monster_type *m_ptr = &m_list[c_ptr->m_idx];
					msg_print(_("���������⤷�Ƥ��ޤ���", "There's something in the way!"));

					if (!m_ptr->ml || !is_pet(m_ptr)) py_attack(y, x, 0);
					break;
				}
				else if (have_flag(f_ptr->flags, FF_TREE))
				{
					msg_print(_("�ڤ�̣�Ϲ�������ʤ���", "You don't like the woody taste!"));
					break;
				}
				else if (have_flag(f_ptr->flags, FF_GLASS))
				{
					msg_print(_("���饹��̣�Ϲ�������ʤ���", "You don't like the glassy taste!"));
					break;
				}
				else if (have_flag(f_ptr->flags, FF_DOOR) || have_flag(f_ptr->flags, FF_CAN_DIG))
				{
					(void)set_food(p_ptr->food + 3000);
				}
				else if (have_flag(f_ptr->flags, FF_MAY_HAVE_GOLD) || have_flag(f_ptr->flags, FF_HAS_GOLD))
				{
					(void)set_food(p_ptr->food + 5000);
				}
				else
				{
					msg_format(_("����%s�ϤȤƤ⤪��������", "This %s is very filling!"), f_name + mimic_f_ptr->name);
					(void)set_food(p_ptr->food + 10000);
				}

				/* Destroy the wall */
				cave_alter_feat(y, x, FF_HURT_ROCK);

				/* Move the player */
				(void)move_player_effect(y, x, MPE_DONT_PICKUP);
			}
			break;

		case MUT1_SWAP_POS:
			project_length = -1;
			if (!get_aim_dir(&dir))
			{
				project_length = 0;
				return FALSE;
			}
			(void)teleport_swap(dir);
			project_length = 0;
			break;

		case MUT1_SHRIEK:
			mutation_stop_mouth();
			(void)fire_ball(GF_SOUND, 0, 2 * lvl, 8);
			(void)aggravate_monsters(0);
			break;

		case MUT1_ILLUMINE:
			(void)lite_area(damroll(2, (lvl / 2)), (lvl / 10) + 1);
			break;

		case MUT1_DET_CURSE:
			{
				int i;

				for (i = 0; i < INVEN_TOTAL; i++)
				{
					object_type *o_ptr = &inventory[i];

					if (!o_ptr->k_idx) continue;
					if (!object_is_cursed(o_ptr)) continue;

					o_ptr->feeling = FEEL_CURSED;
				}
			}
			break;

		case MUT1_BERSERK:
			(void)set_shero(randint1(25) + 25, FALSE);
			(void)hp_player(30);
			(void)set_afraid(0);
			break;

		case MUT1_POLYMORPH:
			if (!get_check(_("�ѿȤ��ޤ���������Ǥ�����", "You will polymorph your self. Are you sure? "))) return FALSE;
			do_poly_self();
			break;

		case MUT1_MIDAS_TCH:
			if (!alchemy()) return FALSE;
			break;

		/* Summon pet molds around the player */
		case MUT1_GROW_MOLD:
			{
				int i;
				for (i = 0; i < 8; i++)
				{
					summon_specific(-1, p_ptr->y, p_ptr->x, lvl, SUMMON_MOLD, PM_FORCE_PET);
				}
			}
			break;

		case MUT1_RESIST:
			{
				int num = lvl / 10;
				int dur = randint1(20) + 20;

				if (randint0(5) < num)
				{
					(void)set_oppose_acid(dur, FALSE);
					num--;
				}
				if (randint0(4) < num)
				{
					(void)set_oppose_elec(dur, FALSE);
					num--;
				}
				if (randint0(3) < num)
				{
					(void)set_oppose_fire(dur, FALSE);
					num--;
				}
				if (randint0(2) < num)
				{
					(void)set_oppose_cold(dur, FALSE);
					num--;
				}
				if (num)
				{
					(void)set_oppose_pois(dur, FALSE);
					num--;
				}
			}
			break;

		case MUT1_EARTHQUAKE:
			(void)earthquake(p_ptr->y, p_ptr->x, 10);
			break;

		case MUT1_EAT_MAGIC:
			if (!eat_magic(p_ptr->lev * 2)) return FALSE;
			break;

		case MUT1_WEIGH_MAG:
			report_magics();
			break;

		case MUT1_STERILITY:
			/* Fake a population explosion. */
#ifdef JP
			msg_print("����Ƭ���ˤ��ʤä���");
			take_hit(DAMAGE_LOSELIFE, randint1(17) + 17, "���ߤ򶯤�����ϫ", -1);
#else
			msg_print("You suddenly have a headache!");
			take_hit(DAMAGE_LOSELIFE, randint1(17) + 17, "the strain of forcing abstinence", -1);
#endif

			num_repro += MAX_REPRO;
			break;

		case MUT1_PANIC_HIT:
			{
				int x, y;

				if (!get_rep_dir2(&dir)) return FALSE;
				y = p_ptr->y + ddy[dir];
				x = p_ptr->x + ddx[dir];
				if (cave[y][x].m_idx)
				{
					py_attack(y, x, 0);
					if (randint0(p_ptr->skill_dis) < 7)
						msg_print(_("���ޤ�ƨ�����ʤ��ä���", "You failed to teleport."));
					else teleport_player(30, 0L);
				}
				else
				{
					msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "You don't see any monster in this direction"));

					msg_print(NULL);
				}
			}
			break;

		case MUT1_DAZZLE:
			stun_monsters(lvl * 4);
			confuse_monsters(lvl * 4);
			turn_monsters(lvl * 4);
			break;

		case MUT1_LASER_EYE:
			if (!get_aim_dir(&dir)) return FALSE;
			fire_beam(GF_LITE, dir, 2 * lvl);
			break;

		case MUT1_RECALL:
			if (!word_of_recall()) return FALSE;
			break;

		case MUT1_BANISH:
			{
				int x, y;
				cave_type *c_ptr;
				monster_type *m_ptr;
				monster_race *r_ptr;

				if (!get_rep_dir2(&dir)) return FALSE;
				y = p_ptr->y + ddy[dir];
				x = p_ptr->x + ddx[dir];
				c_ptr = &cave[y][x];

				if (!c_ptr->m_idx)
				{
					msg_print(_("�ٰ���¸�ߤ򴶤��Ȥ�ޤ���", "You sense no evil there!"));

					break;
				}

				m_ptr = &m_list[c_ptr->m_idx];
				r_ptr = &r_info[m_ptr->r_idx];

				if ((r_ptr->flags3 & RF3_EVIL) &&
				    !(r_ptr->flags1 & RF1_QUESTOR) &&
				    !(r_ptr->flags1 & RF1_UNIQUE) &&
				    !p_ptr->inside_arena && !p_ptr->inside_quest &&
					(r_ptr->level < randint1(p_ptr->lev+50)) &&
					!(m_ptr->mflag2 & MFLAG2_NOGENO))
				{
					if (record_named_pet && is_pet(m_ptr) && m_ptr->nickname)
					{
						char m_name[80];

						monster_desc(m_name, m_ptr, MD_INDEF_VISIBLE);
						do_cmd_write_nikki(NIKKI_NAMED_PET, RECORD_NAMED_PET_GENOCIDE, m_name);
					}

					/* Delete the monster, rather than killing it. */
					delete_monster_idx(c_ptr->m_idx);
					msg_print(_("���μٰ��ʥ�󥹥�����β��������ȤȤ�˾ä���ä���", "The evil creature vanishes in a puff of sulfurous smoke!"));

				}
				else
				{
					msg_print(_("����ϸ��̤��ʤ��ä���", "Your invocation is ineffectual!"));

					if (one_in_(13)) m_ptr->mflag2 |= MFLAG2_NOGENO;
				}
			}
			break;

		case MUT1_COLD_TOUCH:
			{
				int x, y;
				cave_type *c_ptr;

				if (!get_rep_dir2(&dir)) return FALSE;
				y = p_ptr->y + ddy[dir];
				x = p_ptr->x + ddx[dir];
				c_ptr = &cave[y][x];

				if (!c_ptr->m_idx)
				{
					msg_print(_("���ʤ��ϲ���ʤ����Ǽ�򿶤ä���", "You wave your hands in the air."));

					break;
				}
				fire_bolt(GF_COLD, dir, 2 * lvl);
			}
			break;

		/* XXX_XXX_XXX Hack!  MUT1_LAUNCHER is negative, see above */
		case 3: /* MUT1_LAUNCHER */
			/* Gives a multiplier of 2 at first, up to 3 at 40th */
			if (!do_cmd_throw_aux(2 + lvl / 40, FALSE, -1)) return FALSE;
			break;

		default:
			p_ptr->energy_use = 0;
			msg_format(_("ǽ�� %s �ϼ�������Ƥ��ޤ���", "Power %s not implemented. Oops."), power);
	}

	return TRUE;
}
