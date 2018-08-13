/*!
 * @file artifact.c
 * @brief �����ƥ��ե����Ȥ������ȴ��� / Artifact code
 * @date 2013/12/11
 * @author
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke\n
 * This software may be copied and distributed for educational, research, and\n
 * not for profit purposes provided that this copyright and statement are\n
 * included in all such copies.\n
 * 2013 Deskull rearranged comment for Doxygen.
 */

#include "angband.h"

static int suppression_evil_dam(object_type *o_ptr);
static int weakening_artifact(object_type *o_ptr);


/* Chance of using syllables to form the name instead of the "template" files */
#define SINDARIN_NAME   100 /*!< �����ॢ���ƥ��ե����Ȥ˥��������ä�Ĥ����Ψ */ /* #tang 10 -> 100*/
#define TABLE_NAME      100 /*!< �����ॢ���ƥ��ե����Ȥ˴����ä�Ĥ����Ψ(���Τˤ� TABLE_NAME - SINDARIN_NAME %)�Ȥʤ� */ /* #tang 20 -> 100*/
#define A_CURSED        99 /*!< 1/n�γ�Ψ�������δ�ʪ�ʳ��Υ����ॢ���ƥ��ե����Ȥ������Ĥ��ˤʤ롣 */ /* #tang 13 -> 99*/
#define WEIRD_LUCK      3 /*!< 1/n�γ�Ψ��random_resistance()�ν�����Х����������������Ĥ���create_artifact��4��Ķ����pval�����Ĥ���롣*/ /* #tang 12 -> 3*/
#define BIAS_LUCK       10 /*!< 1/n�γ�Ψ��random_resistance()���ղä��븵���������ȱ֤ˤʤ� */ /* #tang 20 -> 10*/
#define IM_LUCK         4 /*!< 1/n�γ�Ψ��random_resistance()��ʣ���ȱ֤ν���������Ƚ������ */ /* #tang 7 -> 4*/

/*! @note
 * Bias luck needs to be higher than weird luck,
 * since it is usually tested several times...
 */

#define ACTIVATION_CHANCE 3 /*!< 1/n�γ�Ψ�ǥ����ॢ���ƥ��ե����Ȥ�ȯư���ղä���롣�������ɶ�Ϥ����1/2 */


/*!
 * @brief �оݤΥ��֥������Ȥ˥�����ʾ�����������ղä��롣/ Choose one random high resistance
 * @details ��ʣ���޻ߤϤʤ���������ǡ��������Ź������ҡ����ܡ������Ϲ������̺��𡢥����������������ݤΤ����줫��
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return �ʤ�
 */
void one_high_resistance(object_type *o_ptr)
{
	switch (randint0(12))
	{
		case  0: add_flag(o_ptr->art_flags, TR_RES_POIS);   break;
		case  1: add_flag(o_ptr->art_flags, TR_RES_LITE);   break;
		case  2: add_flag(o_ptr->art_flags, TR_RES_DARK);   break;
		case  3: add_flag(o_ptr->art_flags, TR_RES_SHARDS); break;
		case  4: add_flag(o_ptr->art_flags, TR_RES_BLIND);  break;
		case  5: add_flag(o_ptr->art_flags, TR_RES_CONF);   break;
		case  6: add_flag(o_ptr->art_flags, TR_RES_SOUND);  break;
		case  7: add_flag(o_ptr->art_flags, TR_RES_NETHER); break;
		case  8: add_flag(o_ptr->art_flags, TR_RES_NEXUS);  break;
		case  9: add_flag(o_ptr->art_flags, TR_RES_CHAOS);  break;
		case 10: add_flag(o_ptr->art_flags, TR_RES_DISEN);  break;
		case 11: add_flag(o_ptr->art_flags, TR_RES_FEAR);   break;
	}
}

/*!
 * @brief �оݤΥ��֥������Ȥ˲��Ԥλ��ظ����ξ�����������ղä��롣/ Choose one random high resistance
 * @details ������������Ź������ҡ����ܡ������Ϲ������̺��𡢥����������ݤǤ���
 * ���Ԥλ��ؤˤ��餫����Ĥ��Ƥ���������one_high_resistance()�������������ΤǤ��롣
 * �������ղä��Τ�Τ˽�ʣ���޻ߤϤʤ���
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return �ʤ�
 */
void one_lordly_high_resistance(object_type *o_ptr)
{
	switch (randint0(10))
	{
		case 0: add_flag(o_ptr->art_flags, TR_RES_LITE);   break;
		case 1: add_flag(o_ptr->art_flags, TR_RES_DARK);   break;
		case 2: add_flag(o_ptr->art_flags, TR_RES_SHARDS); break;
		case 3: add_flag(o_ptr->art_flags, TR_RES_BLIND);  break;
		case 4: add_flag(o_ptr->art_flags, TR_RES_CONF);   break;
		case 5: add_flag(o_ptr->art_flags, TR_RES_SOUND);  break;
		case 6: add_flag(o_ptr->art_flags, TR_RES_NETHER); break;
		case 7: add_flag(o_ptr->art_flags, TR_RES_NEXUS);  break;
		case 8: add_flag(o_ptr->art_flags, TR_RES_CHAOS);  break;
		case 9: add_flag(o_ptr->art_flags, TR_RES_FEAR);   break;
	}
}

/*!
 * @brief �оݤΥ��֥������Ȥ˸������������ղä��롣/ Choose one random element resistance
 * @details ����ϲбꡢ�䵤���ŷ⡢���Τ����줫�Ǥ��ꡢ��ʣ���޻ߤϤʤ���
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return �ʤ�
 */
void one_ele_resistance(object_type *o_ptr)
{
	switch (randint0(4))
	{
		case  0: add_flag(o_ptr->art_flags, TR_RES_ACID); break;
		case  1: add_flag(o_ptr->art_flags, TR_RES_ELEC); break;
		case  2: add_flag(o_ptr->art_flags, TR_RES_COLD); break;
		case  3: add_flag(o_ptr->art_flags, TR_RES_FIRE); break;
	}
}

/*!
 * @brief �оݤΥ��֥������Ȥ˥ɥ饴�����������������������ղä��롣/ Choose one random element or poison resistance
 * @details �����1/7�γ�Ψ���ǡ�6/7�γ�Ψ�ǲбꡢ�䵤���ŷ⡢���Τ����줫(one_ele_resistance()�Υ�����)�Ǥ��ꡢ��ʣ���޻ߤϤʤ���
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return �ʤ�
 */
void one_dragon_ele_resistance(object_type *o_ptr)
{
	if (one_in_(7))
	{
		add_flag(o_ptr->art_flags, TR_RES_POIS);
	}
	else
	{
		one_ele_resistance(o_ptr);
	}
}

/*!
 * @brief �оݤΥ��֥������Ȥ˼夤ESP�����ղä��롣/ Choose one lower rank esp
 * @details �����ưʪ������ǥåɡ����⡢���������ȥ�롢��͡�
 * �ɥ饴�󡢿ʹ֡����ɡ���ˡ���ESP�Τ����줫�Ǥ��ꡢ��ʣ���޻ߤϤʤ���
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return �ʤ�
 */
void one_low_esp(object_type *o_ptr)
{
	switch (randint1(10))
	{
		case 1:  add_flag(o_ptr->art_flags, TR_ESP_ANIMAL);   break;
		case 2:  add_flag(o_ptr->art_flags, TR_ESP_UNDEAD);   break;
		case 3:  add_flag(o_ptr->art_flags, TR_ESP_DEMON);   break;
		case 4:  add_flag(o_ptr->art_flags, TR_ESP_ORC);   break;
		case 5:  add_flag(o_ptr->art_flags, TR_ESP_TROLL);   break;
		case 6:  add_flag(o_ptr->art_flags, TR_ESP_GIANT);   break;
		case 7:  add_flag(o_ptr->art_flags, TR_ESP_DRAGON);   break;
		case 8:  add_flag(o_ptr->art_flags, TR_ESP_HUMAN);   break;
		case 9:  add_flag(o_ptr->art_flags, TR_ESP_GOOD);   break;
		case 10: add_flag(o_ptr->art_flags, TR_ESP_UNIQUE);   break;
	}
}


/*!
 * @brief �оݤΥ��֥������Ȥ����������ղä��롣/ Choose one random resistance
 * @details 1/3�Ǹ�������(one_ele_resistance())��2/3�Ǿ������(one_high_resistance)
 * �򥳡��뤹�롣��ʣ���޻ߤϤʤ���
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return �ʤ�
 */
void one_resistance(object_type *o_ptr)
{
	if (one_in_(3))
	{
		one_ele_resistance(o_ptr);
	}
	else
	{
		one_high_resistance(o_ptr);
	}
}


/*!
 * @brief �оݤΥ��֥������Ȥ�ǽ�Ϥ����ղä��롣/ Choose one random ability
 * @details �������ͷ���ʵ׸���+1��Ʃ���롢�ٹ��پò����޲����������Τ餺���и��Ͱݻ��Τ����줫��
 * ��ʣ���޻ߤϤʤ���
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return �ʤ�
 */
void one_ability(object_type *o_ptr)
{
	switch (randint0(10))
	{
	case 0: add_flag(o_ptr->art_flags, TR_LEVITATION);  break;
	case 1: add_flag(o_ptr->art_flags, TR_LITE_1);      break;
	case 2: add_flag(o_ptr->art_flags, TR_SEE_INVIS);   break;
	case 3: add_flag(o_ptr->art_flags, TR_WARNING);     break;
	case 4: add_flag(o_ptr->art_flags, TR_SLOW_DIGEST); break;
	case 5: add_flag(o_ptr->art_flags, TR_REGEN);       break;
	case 6: add_flag(o_ptr->art_flags, TR_FREE_ACT);    break;
	case 7: add_flag(o_ptr->art_flags, TR_HOLD_EXP);   break;
	case 8:
	case 9:
		one_low_esp(o_ptr);
		break;
	}
}

/*!
 * @brief �оݤΥ��֥������Ȥ�ȯư�����ղä��롣/ Choose one random activation
 * @details ����¿���������ॢ���ƥ��ե����ȤΥХ������ˤϰ��ڰ�¸������
 * while�롼�פˤ�빽¤��ǽ��Ū�˶��Ϥʤ�Τۤɳ�Ψ����Ȥ��Ƥ��롣
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return �ʤ�
 */
void one_activation(object_type *o_ptr)
{
	int type = 0;
	int chance = 0;

	while (randint1(100) >= chance)
	{
		type = randint1(255);
		switch (type)
		{
			case ACT_SUNLIGHT:
			case ACT_BO_MISS_1:
			case ACT_BA_POIS_1:
			case ACT_BO_ELEC_1:
			case ACT_BO_ACID_1:
			case ACT_BO_COLD_1:
			case ACT_BO_FIRE_1:
			case ACT_CONFUSE:
			case ACT_SLEEP:
			case ACT_QUAKE:
			case ACT_CURE_LW:
			case ACT_CURE_MW:
			case ACT_CURE_POISON:
			case ACT_BERSERK:
			case ACT_LIGHT:
			case ACT_MAP_LIGHT:
			case ACT_DEST_DOOR:
			case ACT_STONE_MUD:
			case ACT_TELEPORT:
				chance = 101;
				break;
			case ACT_BA_COLD_1:
			case ACT_BA_FIRE_1:
			case ACT_HYPODYNAMIA_1:
			case ACT_TELE_AWAY:
			case ACT_ESP:
			case ACT_RESIST_ALL:
			case ACT_DETECT_ALL:
			case ACT_RECALL:
			case ACT_SATIATE:
			case ACT_RECHARGE:
				chance = 85;
				break;
			case ACT_TERROR:
			case ACT_PROT_EVIL:
			case ACT_ID_PLAIN:
				chance = 75;
				break;
			case ACT_HYPODYNAMIA_2:
			case ACT_DRAIN_1:
			case ACT_BO_MISS_2:
			case ACT_BA_FIRE_2:
			case ACT_REST_EXP:
				chance = 66;
				break;
			case ACT_BA_FIRE_3:
			case ACT_BA_COLD_3:
			case ACT_BA_ELEC_3:
			case ACT_WHIRLWIND:
			case ACT_DRAIN_2:
			case ACT_CHARM_ANIMAL:
				chance = 50;
				break;
			case ACT_SUMMON_ANIMAL:
				chance = 40;
				break;
			case ACT_DISP_EVIL:
			case ACT_BA_MISS_3:
			case ACT_DISP_GOOD:
			case ACT_BANISH_EVIL:
			case ACT_GENOCIDE:
			case ACT_MASS_GENO:
			case ACT_CHARM_UNDEAD:
			case ACT_CHARM_OTHER:
			case ACT_SUMMON_PHANTOM:
			case ACT_REST_ALL:
			case ACT_RUNE_EXPLO:
				chance = 33;
				break;
			case ACT_CALL_CHAOS:
			case ACT_ROCKET:
			case ACT_CHARM_ANIMALS:
			case ACT_CHARM_OTHERS:
			case ACT_SUMMON_ELEMENTAL:
			case ACT_CURE_700:
			case ACT_SPEED:
			case ACT_ID_FULL:
			case ACT_RUNE_PROT:
				chance = 25;
				break;
			case ACT_CURE_1000:
			case ACT_XTRA_SPEED:
			case ACT_DETECT_XTRA:
			case ACT_DIM_DOOR:
				chance = 10;
				break;
			case ACT_SUMMON_UNDEAD:
			case ACT_SUMMON_DEMON:
			case ACT_WRAITH:
			case ACT_INVULN:
			case ACT_ALCHEMY:
				chance = 5;
				break;
			default:
				chance = 0;
		}
	}

	/* A type was chosen... */
	o_ptr->xtra2 = (byte_hack)type;
	add_flag(o_ptr->art_flags, TR_ACTIVATE);
	o_ptr->timeout = 0;
}

/*!
 * @brief �����ॢ���ƥ��ե����������桢�оݤΥ��֥������Ȥ�����Υ����ƥ��ե����Ȥˤ���в������/ generation process of cursed artifact.
 * @details pval��AC��̿�桢���᡼�������ξ�硢���ȿž�ξ�1d4���������������Ť������������ե饰��ɬ���ղá�
 * ��ʡ��̵������Ψ�˱����ơ��ʱ�μ��������Ťα�ǰ���и��͵ۼ����夤�����η�³Ū�ղá����������η�³Ū�ղá�HP�ۼ��μ�����
 * MP�ۼ��μ�������ƥ�ݡ��ȡ�ȿ�ƥ�ݡ��ȡ�ȿ��ˡ��Ĥ��롣
 * @attention �ץ쥤�䡼�ο��Ȱ�¸�������ꡣ
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return �ʤ�
 */
static void curse_artifact(object_type * o_ptr)
{
	if (o_ptr->pval > 0) o_ptr->pval = 0 - (o_ptr->pval + randint1(4));
	if (o_ptr->to_a > 0) o_ptr->to_a = 0 - (o_ptr->to_a + randint1(4));
	if (o_ptr->to_h > 0) o_ptr->to_h = 0 - (o_ptr->to_h + randint1(4));
	if (o_ptr->to_d > 0) o_ptr->to_d = 0 - (o_ptr->to_d + randint1(4));

	o_ptr->curse_flags |= (TRC_HEAVY_CURSE | TRC_CURSED);
	remove_flag(o_ptr->art_flags, TR_BLESSED);

	if (one_in_(4)) o_ptr->curse_flags |= TRC_PERMA_CURSE;
	if (one_in_(3)) add_flag(o_ptr->art_flags, TR_TY_CURSE);
	if (one_in_(2)) add_flag(o_ptr->art_flags, TR_AGGRAVATE);
	if (one_in_(3)) add_flag(o_ptr->art_flags, TR_DRAIN_EXP);
	if (one_in_(6)) add_flag(o_ptr->art_flags, TR_ADD_L_CURSE);
	if (one_in_(9)) add_flag(o_ptr->art_flags, TR_ADD_H_CURSE);
	if (one_in_(9)) add_flag(o_ptr->art_flags, TR_DRAIN_HP);
	if (one_in_(9)) add_flag(o_ptr->art_flags, TR_DRAIN_MANA);
	if (one_in_(2)) add_flag(o_ptr->art_flags, TR_TELEPORT);
	else if (one_in_(3)) add_flag(o_ptr->art_flags, TR_NO_TELE);

	if ((p_ptr->pclass != CLASS_WARRIOR) && (p_ptr->pclass != CLASS_ARCHER) && (p_ptr->pclass != CLASS_CAVALRY) && (p_ptr->pclass != CLASS_BERSERKER) && (p_ptr->pclass != CLASS_SMITH) && one_in_(3))
		add_flag(o_ptr->art_flags, TR_NO_MAGIC);
}

/*!
 * @brief �����ॢ���ƥ��ե����������桢�оݤΥ��֥������Ȥ�pvalǽ�Ϥ��ղä��롣/ Add one pval on generation of randam artifact.
 * @details ͥ��Ū���ղä����pval�������ॢ���ƥ��ե����ȥХ������˰�¸����¸�ߤ��롣
 * ��§Ū��������ϡ����ϡ����������Ѥ����ѵס�̥�ϡ�õ������̩���ֳ������ϡ���®�����Τߺη����ɲù�����������롣
 * @attention ���֥������Ȥ�tval��sval�˰�¸�����ϡ��ɥ����ǥ��󥰽��������롣
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return �ʤ�
 */
static void random_plus(object_type * o_ptr)
{
	int this_type = (object_is_weapon_ammo(o_ptr) ? 23 : 19);

	switch (o_ptr->artifact_bias)
	{
	case BIAS_WARRIOR:
		if (!(have_flag(o_ptr->art_flags, TR_STR)))
		{
			add_flag(o_ptr->art_flags, TR_STR);
			if (one_in_(2)) return;
		}

		if (!(have_flag(o_ptr->art_flags, TR_CON)))
		{
			add_flag(o_ptr->art_flags, TR_CON);
			if (one_in_(2)) return;
		}

		if (!(have_flag(o_ptr->art_flags, TR_DEX)))
		{
			add_flag(o_ptr->art_flags, TR_DEX);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_MAGE:
		if (!(have_flag(o_ptr->art_flags, TR_INT)))
		{
			add_flag(o_ptr->art_flags, TR_INT);
			if (one_in_(2)) return;
		}
		if ((o_ptr->tval == TV_GLOVES) && !(have_flag(o_ptr->art_flags, TR_MAGIC_MASTERY)))
		{
			add_flag(o_ptr->art_flags, TR_MAGIC_MASTERY);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_PRIESTLY:
		if (!(have_flag(o_ptr->art_flags, TR_WIS)))
		{
			add_flag(o_ptr->art_flags, TR_WIS);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_RANGER:
		if (!(have_flag(o_ptr->art_flags, TR_DEX)))
		{
			add_flag(o_ptr->art_flags, TR_DEX);
			if (one_in_(2)) return;
		}

		if (!(have_flag(o_ptr->art_flags, TR_CON)))
		{
			add_flag(o_ptr->art_flags, TR_CON);
			if (one_in_(2)) return;
		}

		if (!(have_flag(o_ptr->art_flags, TR_STR)))
		{
			add_flag(o_ptr->art_flags, TR_STR);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_ROGUE:
		if (!(have_flag(o_ptr->art_flags, TR_STEALTH)))
		{
			add_flag(o_ptr->art_flags, TR_STEALTH);
			if (one_in_(2)) return;
		}
		if (!(have_flag(o_ptr->art_flags, TR_SEARCH)))
		{
			add_flag(o_ptr->art_flags, TR_SEARCH);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_STR:
		if (!(have_flag(o_ptr->art_flags, TR_STR)))
		{
			add_flag(o_ptr->art_flags, TR_STR);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_WIS:
		if (!(have_flag(o_ptr->art_flags, TR_WIS)))
		{
			add_flag(o_ptr->art_flags, TR_WIS);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_INT:
		if (!(have_flag(o_ptr->art_flags, TR_INT)))
		{
			add_flag(o_ptr->art_flags, TR_INT);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_DEX:
		if (!(have_flag(o_ptr->art_flags, TR_DEX)))
		{
			add_flag(o_ptr->art_flags, TR_DEX);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_CON:
		if (!(have_flag(o_ptr->art_flags, TR_CON)))
		{
			add_flag(o_ptr->art_flags, TR_CON);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_CHR:
		if (!(have_flag(o_ptr->art_flags, TR_CHR)))
		{
			add_flag(o_ptr->art_flags, TR_CHR);
			if (one_in_(2)) return;
		}
		break;
	}

	if ((o_ptr->artifact_bias == BIAS_MAGE || o_ptr->artifact_bias == BIAS_PRIESTLY) && (o_ptr->tval == TV_SOFT_ARMOR) && (o_ptr->sval == SV_ROBE))
	{
		if (!(have_flag(o_ptr->art_flags, TR_DEC_MANA)) && one_in_(3))
		{
			add_flag(o_ptr->art_flags, TR_DEC_MANA);
			if (one_in_(2)) return;
		}
	}

	switch (randint1(this_type))
	{
	case 1: case 2:
		add_flag(o_ptr->art_flags, TR_STR);
		if (!o_ptr->artifact_bias && !one_in_(13))
			o_ptr->artifact_bias = BIAS_STR;
		else if (!o_ptr->artifact_bias && one_in_(7))
			o_ptr->artifact_bias = BIAS_WARRIOR;
		break;
	case 3: case 4:
		add_flag(o_ptr->art_flags, TR_INT);
		if (!o_ptr->artifact_bias && !one_in_(13))
			o_ptr->artifact_bias = BIAS_INT;
		else if (!o_ptr->artifact_bias && one_in_(7))
			o_ptr->artifact_bias = BIAS_MAGE;
		break;
	case 5: case 6:
		add_flag(o_ptr->art_flags, TR_WIS);
		if (!o_ptr->artifact_bias && !one_in_(13))
			o_ptr->artifact_bias = BIAS_WIS;
		else if (!o_ptr->artifact_bias && one_in_(7))
			o_ptr->artifact_bias = BIAS_PRIESTLY;
		break;
	case 7: case 8:
		add_flag(o_ptr->art_flags, TR_DEX);
		if (!o_ptr->artifact_bias && !one_in_(13))
			o_ptr->artifact_bias = BIAS_DEX;
		else if (!o_ptr->artifact_bias && one_in_(7))
			o_ptr->artifact_bias = BIAS_ROGUE;
		break;
	case 9: case 10:
		add_flag(o_ptr->art_flags, TR_CON);
		if (!o_ptr->artifact_bias && !one_in_(13))
			o_ptr->artifact_bias = BIAS_CON;
		else if (!o_ptr->artifact_bias && one_in_(9))
			o_ptr->artifact_bias = BIAS_RANGER;
		break;
	case 11: case 12:
		add_flag(o_ptr->art_flags, TR_CHR);
		if (!o_ptr->artifact_bias && !one_in_(13))
			o_ptr->artifact_bias = BIAS_CHR;
		break;
	case 13: case 14:
		add_flag(o_ptr->art_flags, TR_STEALTH);
		if (!o_ptr->artifact_bias && one_in_(3))
			o_ptr->artifact_bias = BIAS_ROGUE;
		break;
	case 15: case 16:
		add_flag(o_ptr->art_flags, TR_SEARCH);
		if (!o_ptr->artifact_bias && one_in_(9))
			o_ptr->artifact_bias = BIAS_RANGER;
		break;
	case 17: case 18:
		add_flag(o_ptr->art_flags, TR_INFRA);
		break;
	case 19:
		add_flag(o_ptr->art_flags, TR_SPEED);
		if (!o_ptr->artifact_bias && one_in_(11))
			o_ptr->artifact_bias = BIAS_ROGUE;
		break;
	case 20: case 21:
		add_flag(o_ptr->art_flags, TR_TUNNEL);
		break;
	case 22: case 23:
		if (o_ptr->tval == TV_BOW) random_plus(o_ptr);
		else
		{
			add_flag(o_ptr->art_flags, TR_BLOWS);
			if (!o_ptr->artifact_bias && one_in_(11))
				o_ptr->artifact_bias = BIAS_WARRIOR;
		}
		break;
	}
}

/*!
 * @brief �����ॢ���ƥ��ե����������桢�оݤΥ��֥������Ȥ��������ղä��롣/ Add one resistance on generation of randam artifact.
 * @details ͥ��Ū���ղä���������������ॢ���ƥ��ե����ȥХ������˰�¸����¸�ߤ��롣
 * ��§Ū����ϲбꡢ�䵤���ŷ⡢���ʰʾ��ȱ֤β�ǽ���⤢��ˡ�
 * �ǡ��������Ź������ҡ��첻�����ܡ������Ϲ��������������������ݡ��Х����顢�䵤�����顢�ŷ⥪���顢ȿ�͡�
 * ��ηϥХ������Τ�ȿ���Ĥ���
 * @attention ���֥������Ȥ�tval��sval�˰�¸�����ϡ��ɥ����ǥ��󥰽��������롣
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return �ʤ�
 */
static void random_resistance(object_type * o_ptr)
{
	switch (o_ptr->artifact_bias)
	{
	case BIAS_ACID:
		if (!(have_flag(o_ptr->art_flags, TR_RES_ACID)))
		{
			add_flag(o_ptr->art_flags, TR_RES_ACID);
			if (one_in_(2)) return;
		}
		if (one_in_(BIAS_LUCK) && !(have_flag(o_ptr->art_flags, TR_IM_ACID)))
		{
			add_flag(o_ptr->art_flags, TR_IM_ACID);
			if (!one_in_(IM_LUCK))
			{
				remove_flag(o_ptr->art_flags, TR_IM_ELEC);
				remove_flag(o_ptr->art_flags, TR_IM_COLD);
				remove_flag(o_ptr->art_flags, TR_IM_FIRE);
			}
			if (one_in_(2)) return;
		}
		break;

	case BIAS_ELEC:
		if (!(have_flag(o_ptr->art_flags, TR_RES_ELEC)))
		{
			add_flag(o_ptr->art_flags, TR_RES_ELEC);
			if (one_in_(2)) return;
		}
		if ((o_ptr->tval >= TV_CLOAK) && (o_ptr->tval <= TV_HARD_ARMOR) &&
		    !(have_flag(o_ptr->art_flags, TR_SH_ELEC)))
		{
			add_flag(o_ptr->art_flags, TR_SH_ELEC);
			if (one_in_(2)) return;
		}
		if (one_in_(BIAS_LUCK) && !(have_flag(o_ptr->art_flags, TR_IM_ELEC)))
		{
			add_flag(o_ptr->art_flags, TR_IM_ELEC);
			if (!one_in_(IM_LUCK))
			{
				remove_flag(o_ptr->art_flags, TR_IM_ACID);
				remove_flag(o_ptr->art_flags, TR_IM_COLD);
				remove_flag(o_ptr->art_flags, TR_IM_FIRE);
			}

			if (one_in_(2)) return;
		}
		break;

	case BIAS_FIRE:
		if (!(have_flag(o_ptr->art_flags, TR_RES_FIRE)))
		{
			add_flag(o_ptr->art_flags, TR_RES_FIRE);
			if (one_in_(2)) return;
		}
		if ((o_ptr->tval >= TV_CLOAK) &&
		    (o_ptr->tval <= TV_HARD_ARMOR) &&
		    !(have_flag(o_ptr->art_flags, TR_SH_FIRE)))
		{
			add_flag(o_ptr->art_flags, TR_SH_FIRE);
			if (one_in_(2)) return;
		}
		if (one_in_(BIAS_LUCK) &&
		    !(have_flag(o_ptr->art_flags, TR_IM_FIRE)))
		{
			add_flag(o_ptr->art_flags, TR_IM_FIRE);
			if (!one_in_(IM_LUCK))
			{
				remove_flag(o_ptr->art_flags, TR_IM_ELEC);
				remove_flag(o_ptr->art_flags, TR_IM_COLD);
				remove_flag(o_ptr->art_flags, TR_IM_ACID);
			}
			if (one_in_(2)) return;
		}
		break;

	case BIAS_COLD:
		if (!(have_flag(o_ptr->art_flags, TR_RES_COLD)))
		{
			add_flag(o_ptr->art_flags, TR_RES_COLD);
			if (one_in_(2)) return;
		}
		if ((o_ptr->tval >= TV_CLOAK) &&
		    (o_ptr->tval <= TV_HARD_ARMOR) &&
		    !(have_flag(o_ptr->art_flags, TR_SH_COLD)))
		{
			add_flag(o_ptr->art_flags, TR_SH_COLD);
			if (one_in_(2)) return;
		}
		if (one_in_(BIAS_LUCK) && !(have_flag(o_ptr->art_flags, TR_IM_COLD)))
		{
			add_flag(o_ptr->art_flags, TR_IM_COLD);
			if (!one_in_(IM_LUCK))
			{
				remove_flag(o_ptr->art_flags, TR_IM_ELEC);
				remove_flag(o_ptr->art_flags, TR_IM_ACID);
				remove_flag(o_ptr->art_flags, TR_IM_FIRE);
			}
			if (one_in_(2)) return;
		}
		break;

	case BIAS_POIS:
		if (!(have_flag(o_ptr->art_flags, TR_RES_POIS)))
		{
			add_flag(o_ptr->art_flags, TR_RES_POIS);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_WARRIOR:
		if (!one_in_(3) && (!(have_flag(o_ptr->art_flags, TR_RES_FEAR))))
		{
			add_flag(o_ptr->art_flags, TR_RES_FEAR);
			if (one_in_(2)) return;
		}
		if (one_in_(3) && (!(have_flag(o_ptr->art_flags, TR_NO_MAGIC))))
		{
			add_flag(o_ptr->art_flags, TR_NO_MAGIC);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_NECROMANTIC:
		if (!(have_flag(o_ptr->art_flags, TR_RES_NETHER)))
		{
			add_flag(o_ptr->art_flags, TR_RES_NETHER);
			if (one_in_(2)) return;
		}
		if (!(have_flag(o_ptr->art_flags, TR_RES_POIS)))
		{
			add_flag(o_ptr->art_flags, TR_RES_POIS);
			if (one_in_(2)) return;
		}
		if (!(have_flag(o_ptr->art_flags, TR_RES_DARK)))
		{
			add_flag(o_ptr->art_flags, TR_RES_DARK);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_CHAOS:
		if (!(have_flag(o_ptr->art_flags, TR_RES_CHAOS)))
		{
			add_flag(o_ptr->art_flags, TR_RES_CHAOS);
			if (one_in_(2)) return;
		}
		if (!(have_flag(o_ptr->art_flags, TR_RES_CONF)))
		{
			add_flag(o_ptr->art_flags, TR_RES_CONF);
			if (one_in_(2)) return;
		}
		if (!(have_flag(o_ptr->art_flags, TR_RES_DISEN)))
		{
			add_flag(o_ptr->art_flags, TR_RES_DISEN);
			if (one_in_(2)) return;
		}
		break;
	}

	switch (randint1(42))
	{
		case 1:
			if (!one_in_(WEIRD_LUCK))
				random_resistance(o_ptr);
			else
			{
				add_flag(o_ptr->art_flags, TR_IM_ACID);
				if (!o_ptr->artifact_bias)
					o_ptr->artifact_bias = BIAS_ACID;
			}
			break;
		case 2:
			if (!one_in_(WEIRD_LUCK))
				random_resistance(o_ptr);
			else
			{
				add_flag(o_ptr->art_flags, TR_IM_ELEC);
				if (!o_ptr->artifact_bias)
					o_ptr->artifact_bias = BIAS_ELEC;
			}
			break;
		case 3:
			if (!one_in_(WEIRD_LUCK))
				random_resistance(o_ptr);
			else
			{
				add_flag(o_ptr->art_flags, TR_IM_COLD);
				if (!o_ptr->artifact_bias)
					o_ptr->artifact_bias = BIAS_COLD;
			}
			break;
		case 4:
			if (!one_in_(WEIRD_LUCK))
				random_resistance(o_ptr);
			else
			{
				add_flag(o_ptr->art_flags, TR_IM_FIRE);
				if (!o_ptr->artifact_bias)
					o_ptr->artifact_bias = BIAS_FIRE;
			}
			break;
		case 5:
		case 6:
		case 13:
			add_flag(o_ptr->art_flags, TR_RES_ACID);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_ACID;
			break;
		case 7:
		case 8:
		case 14:
			add_flag(o_ptr->art_flags, TR_RES_ELEC);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_ELEC;
			break;
		case 9:
		case 10:
		case 15:
			add_flag(o_ptr->art_flags, TR_RES_FIRE);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_FIRE;
			break;
		case 11:
		case 12:
		case 16:
			add_flag(o_ptr->art_flags, TR_RES_COLD);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_COLD;
			break;
		case 17:
		case 18:
			add_flag(o_ptr->art_flags, TR_RES_POIS);
			if (!o_ptr->artifact_bias && !one_in_(4))
				o_ptr->artifact_bias = BIAS_POIS;
			else if (!o_ptr->artifact_bias && one_in_(2))
				o_ptr->artifact_bias = BIAS_NECROMANTIC;
			else if (!o_ptr->artifact_bias && one_in_(2))
				o_ptr->artifact_bias = BIAS_ROGUE;
			break;
		case 19:
		case 20:
			add_flag(o_ptr->art_flags, TR_RES_FEAR);
			if (!o_ptr->artifact_bias && one_in_(3))
				o_ptr->artifact_bias = BIAS_WARRIOR;
			break;
		case 21:
			add_flag(o_ptr->art_flags, TR_RES_LITE);
			break;
		case 22:
			add_flag(o_ptr->art_flags, TR_RES_DARK);
			break;
		case 23:
		case 24:
			add_flag(o_ptr->art_flags, TR_RES_BLIND);
			break;
		case 25:
		case 26:
			add_flag(o_ptr->art_flags, TR_RES_CONF);
			if (!o_ptr->artifact_bias && one_in_(6))
				o_ptr->artifact_bias = BIAS_CHAOS;
			break;
		case 27:
		case 28:
			add_flag(o_ptr->art_flags, TR_RES_SOUND);
			break;
		case 29:
		case 30:
			add_flag(o_ptr->art_flags, TR_RES_SHARDS);
			break;
		case 31:
		case 32:
			add_flag(o_ptr->art_flags, TR_RES_NETHER);
			if (!o_ptr->artifact_bias && one_in_(3))
				o_ptr->artifact_bias = BIAS_NECROMANTIC;
			break;
		case 33:
		case 34:
			add_flag(o_ptr->art_flags, TR_RES_NEXUS);
			break;
		case 35:
		case 36:
			add_flag(o_ptr->art_flags, TR_RES_CHAOS);
			if (!o_ptr->artifact_bias && one_in_(2))
				o_ptr->artifact_bias = BIAS_CHAOS;
			break;
		case 37:
		case 38:
			add_flag(o_ptr->art_flags, TR_RES_DISEN);
			break;
		case 39:
			if (o_ptr->tval >= TV_CLOAK && o_ptr->tval <= TV_HARD_ARMOR)
				add_flag(o_ptr->art_flags, TR_SH_ELEC);
			else
				random_resistance(o_ptr);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_ELEC;
			break;
		case 40:
			if (o_ptr->tval >= TV_CLOAK && o_ptr->tval <= TV_HARD_ARMOR)
				add_flag(o_ptr->art_flags, TR_SH_FIRE);
			else
				random_resistance(o_ptr);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_FIRE;
			break;
		case 41:
			if (o_ptr->tval == TV_SHIELD || o_ptr->tval == TV_CLOAK ||
			    o_ptr->tval == TV_HELM || o_ptr->tval == TV_HARD_ARMOR)
				add_flag(o_ptr->art_flags, TR_REFLECT);
			else
				random_resistance(o_ptr);
			break;
		case 42:
			if (o_ptr->tval >= TV_CLOAK && o_ptr->tval <= TV_HARD_ARMOR)
				add_flag(o_ptr->art_flags, TR_SH_COLD);
			else
				random_resistance(o_ptr);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_COLD;
			break;
	}
}


/*!
 * @brief �����ॢ���ƥ��ե����������桢�оݤΥ��֥������Ȥˤ���¾�������ղä��롣/ Add one misc flag on generation of randam artifact.
 * @details ͥ��Ū���ղä���������������ॢ���ƥ��ե����ȥХ������˰�¸����¸�ߤ��롣
 * ��§Ū����ϳƼ�ǽ�ϰݻ����ʵ׸���+1�������Τ餺���и��Ͱݻ�����ͷ��Ʃ���롢�޲������پò���
 * ��ƥ�ݡ��ȡ�ȿ��ˡ��ȿ�ƥ�ݡ��ȡ��ٹ𡢥ƥ�ѥ������Ƽ�ESP�����������˻�٤������
 * @attention ���֥������Ȥ�tval��sval�˰�¸�����ϡ��ɥ����ǥ��󥰽��������롣
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return �ʤ�
 */
static void random_misc(object_type * o_ptr)
{
	switch (o_ptr->artifact_bias)
	{
	case BIAS_RANGER:
		if (!(have_flag(o_ptr->art_flags, TR_SUST_CON)))
		{
			add_flag(o_ptr->art_flags, TR_SUST_CON);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_STR:
		if (!(have_flag(o_ptr->art_flags, TR_SUST_STR)))
		{
			add_flag(o_ptr->art_flags, TR_SUST_STR);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_WIS:
		if (!(have_flag(o_ptr->art_flags, TR_SUST_WIS)))
		{
			add_flag(o_ptr->art_flags, TR_SUST_WIS);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_INT:
		if (!(have_flag(o_ptr->art_flags, TR_SUST_INT)))
		{
			add_flag(o_ptr->art_flags, TR_SUST_INT);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_DEX:
		if (!(have_flag(o_ptr->art_flags, TR_SUST_DEX)))
		{
			add_flag(o_ptr->art_flags, TR_SUST_DEX);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_CON:
		if (!(have_flag(o_ptr->art_flags, TR_SUST_CON)))
		{
			add_flag(o_ptr->art_flags, TR_SUST_CON);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_CHR:
		if (!(have_flag(o_ptr->art_flags, TR_SUST_CHR)))
		{
			add_flag(o_ptr->art_flags, TR_SUST_CHR);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_CHAOS:
		if (!(have_flag(o_ptr->art_flags, TR_TELEPORT)))
		{
			add_flag(o_ptr->art_flags, TR_TELEPORT);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_FIRE:
		if (!(have_flag(o_ptr->art_flags, TR_LITE_1)))
		{
			add_flag(o_ptr->art_flags, TR_LITE_1); /* Freebie */
		}
		break;
	}

	switch (randint1(33))
	{
		case 1:
			add_flag(o_ptr->art_flags, TR_SUST_STR);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_STR;
			break;
		case 2:
			add_flag(o_ptr->art_flags, TR_SUST_INT);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_INT;
			break;
		case 3:
			add_flag(o_ptr->art_flags, TR_SUST_WIS);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_WIS;
			break;
		case 4:
			add_flag(o_ptr->art_flags, TR_SUST_DEX);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_DEX;
			break;
		case 5:
			add_flag(o_ptr->art_flags, TR_SUST_CON);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_CON;
			break;
		case 6:
			add_flag(o_ptr->art_flags, TR_SUST_CHR);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_CHR;
			break;
		case 7:
		case 8:
		case 14:
			add_flag(o_ptr->art_flags, TR_FREE_ACT);
			break;
		case 9:
			add_flag(o_ptr->art_flags, TR_HOLD_EXP);
			if (!o_ptr->artifact_bias && one_in_(5))
				o_ptr->artifact_bias = BIAS_PRIESTLY;
			else if (!o_ptr->artifact_bias && one_in_(6))
				o_ptr->artifact_bias = BIAS_NECROMANTIC;
			break;
		case 10:
		case 11:
			add_flag(o_ptr->art_flags, TR_LITE_1);
			break;
		case 12:
		case 13:
			add_flag(o_ptr->art_flags, TR_LEVITATION);
			break;
		case 15:
		case 16:
		case 17:
			add_flag(o_ptr->art_flags, TR_SEE_INVIS);
			break;
		case 19:
		case 20:
			add_flag(o_ptr->art_flags, TR_SLOW_DIGEST);
			break;
		case 21:
		case 22:
			add_flag(o_ptr->art_flags, TR_REGEN);
			break;
		case 23:
			add_flag(o_ptr->art_flags, TR_TELEPORT);
			break;
		case 24:
		case 25:
		case 26:
			if (object_is_armour(o_ptr))
				random_misc(o_ptr);
			else
			{
				o_ptr->to_a = 4 + randint1(11);
			}
			break;
		case 27:
		case 28:
		case 29:
		{
			HIT_PROB bonus_h;
			HIT_POINT bonus_d;
			add_flag(o_ptr->art_flags, TR_SHOW_MODS);
			bonus_h = 4 + (HIT_PROB)(randint1(11));
			bonus_d = 4 + (HIT_POINT)(randint1(11));
			if ((o_ptr->tval != TV_SWORD) && (o_ptr->tval != TV_POLEARM) && (o_ptr->tval != TV_HAFTED) && (o_ptr->tval != TV_DIGGING) && (o_ptr->tval != TV_GLOVES) && (o_ptr->tval != TV_RING))
			{
				bonus_h /= 2;
				bonus_d /= 2;
			}
			o_ptr->to_h += bonus_h;
			o_ptr->to_d += bonus_d;
			break;
		}
		case 30:
			add_flag(o_ptr->art_flags, TR_NO_MAGIC);
			break;
		case 31:
			add_flag(o_ptr->art_flags, TR_NO_TELE);
			break;
		case 32:
			add_flag(o_ptr->art_flags, TR_WARNING);
			break;

		case 18:
			switch (randint1(3))
			{
			case 1:
				add_flag(o_ptr->art_flags, TR_ESP_EVIL);
				if (!o_ptr->artifact_bias && one_in_(3))
					o_ptr->artifact_bias = BIAS_LAW;
				break;
			case 2:
				add_flag(o_ptr->art_flags, TR_ESP_NONLIVING);
				if (!o_ptr->artifact_bias && one_in_(3))
					o_ptr->artifact_bias = BIAS_MAGE;
				break;
			case 3:
				add_flag(o_ptr->art_flags, TR_TELEPATHY);
				if (!o_ptr->artifact_bias && one_in_(9))
					o_ptr->artifact_bias = BIAS_MAGE;
				break;
			}
			break;

		case 33:
		{
			int idx[3];
			int n = randint1(3);

			idx[0] = randint1(10);

			idx[1] = randint1(9);
			if (idx[1] >= idx[0]) idx[1]++;

			idx[2] = randint1(8);
			if (idx[2] >= idx[0]) idx[2]++;
			if (idx[2] >= idx[1]) idx[2]++;

			while (n--) switch (idx[n])
			{
			case 1:
				add_flag(o_ptr->art_flags, TR_ESP_ANIMAL);
				if (!o_ptr->artifact_bias && one_in_(4))
					o_ptr->artifact_bias = BIAS_RANGER;
				break;
			case 2:
				add_flag(o_ptr->art_flags, TR_ESP_UNDEAD);
				if (!o_ptr->artifact_bias && one_in_(3))
					o_ptr->artifact_bias = BIAS_PRIESTLY;
				else if (!o_ptr->artifact_bias && one_in_(6))
					o_ptr->artifact_bias = BIAS_NECROMANTIC;
				break;
			case 3:
				add_flag(o_ptr->art_flags, TR_ESP_DEMON);
				break;
			case 4:
				add_flag(o_ptr->art_flags, TR_ESP_ORC);
				break;
			case 5:
				add_flag(o_ptr->art_flags, TR_ESP_TROLL);
				break;
			case 6:
				add_flag(o_ptr->art_flags, TR_ESP_GIANT);
				break;
			case 7:
				add_flag(o_ptr->art_flags, TR_ESP_DRAGON);
				break;
			case 8:
				add_flag(o_ptr->art_flags, TR_ESP_HUMAN);
				if (!o_ptr->artifact_bias && one_in_(6))
					o_ptr->artifact_bias = BIAS_ROGUE;
				break;
			case 9:
				add_flag(o_ptr->art_flags, TR_ESP_GOOD);
				if (!o_ptr->artifact_bias && one_in_(3))
					o_ptr->artifact_bias = BIAS_LAW;
				break;
			case 10:
				add_flag(o_ptr->art_flags, TR_ESP_UNIQUE);
				if (!o_ptr->artifact_bias && one_in_(3))
					o_ptr->artifact_bias = BIAS_LAW;
				break;
			}
			break;
		}
	}
}

/*!
 * @brief �����ॢ���ƥ��ե����������桢�оݤΥ��֥������Ȥ˥��쥤���̤��ղä��롣/ Add one slaying on generation of randam artifact.
 * @details ͥ��Ū���ղä���������������ॢ���ƥ��ե����ȥХ������˰�¸����¸�ߤ��롣
 * ��§Ū����϶��ϼ͡���®�͡����ٸ��̡��۷���̡���ʡ����ڳ���䤹�����ƴ�����롢�ŷ⡢�ϲ��ǻ���
 * ưʪ���쥤���ٰ����쥤�����⥹�쥤���Իॹ�쥤�����������쥤���ȥ�륹�쥤����ͥ��쥤���ɥ饴�󥹥쥤��
 * *�ɥ饴�󥹥쥤*���ʹ֥��쥤���ڤ�̣���Ͽ̡����ϡ�
 * @attention ���֥������Ȥ�tval��sval�˰�¸�����ϡ��ɥ����ǥ��󥰽��������롣
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return �ʤ�
 */
static void random_slay(object_type *o_ptr)
{
	if (o_ptr->tval == TV_BOW)
	{
		switch (randint1(6))
		{
			case 1:
			case 2:
			case 3:
				add_flag(o_ptr->art_flags, TR_XTRA_MIGHT);
				if (!one_in_(7)) remove_flag(o_ptr->art_flags, TR_XTRA_SHOTS);
				if (!o_ptr->artifact_bias && one_in_(9))
					o_ptr->artifact_bias = BIAS_RANGER;
				break;
			default:
				add_flag(o_ptr->art_flags, TR_XTRA_SHOTS);
				if (!one_in_(7)) remove_flag(o_ptr->art_flags, TR_XTRA_MIGHT);
				if (!o_ptr->artifact_bias && one_in_(9))
					o_ptr->artifact_bias = BIAS_RANGER;
			break;
		}

		return;
	}

	switch (o_ptr->artifact_bias)
	{
	case BIAS_CHAOS:
		if (!(have_flag(o_ptr->art_flags, TR_CHAOTIC)))
		{
			add_flag(o_ptr->art_flags, TR_CHAOTIC);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_PRIESTLY:
		if((o_ptr->tval == TV_SWORD || o_ptr->tval == TV_POLEARM) &&
		   !(have_flag(o_ptr->art_flags, TR_BLESSED)))
		{
			/* A free power for "priestly" random artifacts */
			add_flag(o_ptr->art_flags, TR_BLESSED);
		}
		break;

	case BIAS_NECROMANTIC:
		if (!(have_flag(o_ptr->art_flags, TR_VAMPIRIC)))
		{
			add_flag(o_ptr->art_flags, TR_VAMPIRIC);
			if (one_in_(2)) return;
		}
		if (!(have_flag(o_ptr->art_flags, TR_BRAND_POIS)) && one_in_(2))
		{
			add_flag(o_ptr->art_flags, TR_BRAND_POIS);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_RANGER:
		if (!(have_flag(o_ptr->art_flags, TR_SLAY_ANIMAL)))
		{
			add_flag(o_ptr->art_flags, TR_SLAY_ANIMAL);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_ROGUE:
		if ((((o_ptr->tval == TV_SWORD) && (o_ptr->sval == SV_DAGGER)) ||
		     ((o_ptr->tval == TV_POLEARM) && (o_ptr->sval == SV_SPEAR))) &&
			 !(have_flag(o_ptr->art_flags, TR_THROW)))
		{
			/* Free power for rogues... */
			add_flag(o_ptr->art_flags, TR_THROW);
		}
		if (!(have_flag(o_ptr->art_flags, TR_BRAND_POIS)))
		{
			add_flag(o_ptr->art_flags, TR_BRAND_POIS);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_POIS:
		if (!(have_flag(o_ptr->art_flags, TR_BRAND_POIS)))
		{
			add_flag(o_ptr->art_flags, TR_BRAND_POIS);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_FIRE:
		if (!(have_flag(o_ptr->art_flags, TR_BRAND_FIRE)))
		{
			add_flag(o_ptr->art_flags, TR_BRAND_FIRE);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_COLD:
		if (!(have_flag(o_ptr->art_flags, TR_BRAND_COLD)))
		{
			add_flag(o_ptr->art_flags, TR_BRAND_COLD);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_ELEC:
		if (!(have_flag(o_ptr->art_flags, TR_BRAND_ELEC)))
		{
			add_flag(o_ptr->art_flags, TR_BRAND_ELEC);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_ACID:
		if (!(have_flag(o_ptr->art_flags, TR_BRAND_ACID)))
		{
			add_flag(o_ptr->art_flags, TR_BRAND_ACID);
			if (one_in_(2)) return;
		}
		break;

	case BIAS_LAW:
		if (!(have_flag(o_ptr->art_flags, TR_SLAY_EVIL)))
		{
			add_flag(o_ptr->art_flags, TR_SLAY_EVIL);
			if (one_in_(2)) return;
		}
		if (!(have_flag(o_ptr->art_flags, TR_SLAY_UNDEAD)))
		{
			add_flag(o_ptr->art_flags, TR_SLAY_UNDEAD);
			if (one_in_(2)) return;
		}
		if (!(have_flag(o_ptr->art_flags, TR_SLAY_DEMON)))
		{
			add_flag(o_ptr->art_flags, TR_SLAY_DEMON);
			if (one_in_(2)) return;
		}
		break;
	}

	switch (randint1(36))
	{
		case 1:
		case 2:
			if (one_in_(4))
			{
				add_flag(o_ptr->art_flags, TR_KILL_ANIMAL);
			}
			else
			{
				add_flag(o_ptr->art_flags, TR_SLAY_ANIMAL);
			}
			break;
		case 3:
		case 4:
			if (one_in_(4))
			{
				add_flag(o_ptr->art_flags, TR_KILL_EVIL);
			}
			else
			{
			add_flag(o_ptr->art_flags, TR_SLAY_EVIL); 
			}
			if (!o_ptr->artifact_bias && one_in_(2))
				o_ptr->artifact_bias = BIAS_LAW;
			else if (!o_ptr->artifact_bias && one_in_(9))
				o_ptr->artifact_bias = BIAS_PRIESTLY;
			break;
		case 5:
		case 6:
			if (one_in_(4))
			{
				add_flag(o_ptr->art_flags, TR_KILL_UNDEAD);
			}
			else
			{
				add_flag(o_ptr->art_flags, TR_SLAY_UNDEAD);
			}
			if (!o_ptr->artifact_bias && one_in_(9))
				o_ptr->artifact_bias = BIAS_PRIESTLY;
			break;
		case 7:
		case 8:
			if (one_in_(4))
			{
				add_flag(o_ptr->art_flags, TR_KILL_DEMON);
			}
			else
			{
				add_flag(o_ptr->art_flags, TR_SLAY_DEMON);
			}
			if (!o_ptr->artifact_bias && one_in_(9))
				o_ptr->artifact_bias = BIAS_PRIESTLY;
			break;
		case 9:
		case 10:
			if (one_in_(4))
			{
				add_flag(o_ptr->art_flags, TR_KILL_ORC);
			}
			else
			{
				add_flag(o_ptr->art_flags, TR_SLAY_ORC);
			}
			break;
		case 11:
		case 12:
			if (one_in_(4))
			{
				add_flag(o_ptr->art_flags, TR_KILL_TROLL);
			}
			else
			{
				add_flag(o_ptr->art_flags, TR_SLAY_TROLL);
			}
			break;
		case 13:
		case 14:
			if (one_in_(4))
			{
				add_flag(o_ptr->art_flags, TR_KILL_GIANT);
			}
			else
			{
				add_flag(o_ptr->art_flags, TR_SLAY_GIANT);
			}
			break;
		case 15:
		case 16:
			add_flag(o_ptr->art_flags, TR_SLAY_DRAGON);
			break;
		case 17:
			add_flag(o_ptr->art_flags, TR_KILL_DRAGON);
			break;
		case 18:
		case 19:
			if (o_ptr->tval == TV_SWORD)
			{
				add_flag(o_ptr->art_flags, TR_VORPAL);
				if (!o_ptr->artifact_bias && one_in_(9))
					o_ptr->artifact_bias = BIAS_WARRIOR;
			}
			else
				random_slay(o_ptr);
			break;
		case 20:
			add_flag(o_ptr->art_flags, TR_IMPACT);
			break;
		case 21:
		case 22:
			add_flag(o_ptr->art_flags, TR_BRAND_FIRE);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_FIRE;
			break;
		case 23:
		case 24:
			add_flag(o_ptr->art_flags, TR_BRAND_COLD);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_COLD;
			break;
		case 25:
		case 26:
			add_flag(o_ptr->art_flags, TR_BRAND_ELEC);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_ELEC;
			break;
		case 27:
		case 28:
			add_flag(o_ptr->art_flags, TR_BRAND_ACID);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_ACID;
			break;
		case 29:
		case 30:
			add_flag(o_ptr->art_flags, TR_BRAND_POIS);
			if (!o_ptr->artifact_bias && !one_in_(3))
				o_ptr->artifact_bias = BIAS_POIS;
			else if (!o_ptr->artifact_bias && one_in_(6))
				o_ptr->artifact_bias = BIAS_NECROMANTIC;
			else if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_ROGUE;
			break;
		case 31:
			add_flag(o_ptr->art_flags, TR_VAMPIRIC);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_NECROMANTIC;
			break;
		case 32:
			add_flag(o_ptr->art_flags, TR_FORCE_WEAPON);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = (one_in_(2) ? BIAS_MAGE : BIAS_PRIESTLY);
			break;
		case 33:
		case 34:
			if (one_in_(4))
			{
				add_flag(o_ptr->art_flags, TR_KILL_HUMAN);
			}
			else
			{
				add_flag(o_ptr->art_flags, TR_SLAY_HUMAN);
			}
			break;
		default:
			add_flag(o_ptr->art_flags, TR_CHAOTIC);
			if (!o_ptr->artifact_bias)
				o_ptr->artifact_bias = BIAS_CHAOS;
			break;
	}
}

/*!
 * @brief �����ॢ���ƥ��ե����������桢�оݤΥ��֥������Ȥ˥Х������˰�¸����ȯư��Ϳ���롣/ Add one activaton of randam artifact depend on bias.
 * @details �Х�������̵����硢�����ΥХ������γ�Ψ�ˤ�äƤ� one_ability() �˽������ܹԤ��롣
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return �ʤ�
 */
static void give_activation_power(object_type *o_ptr)
{
	int type = 0, chance = 0;

	switch (o_ptr->artifact_bias)
	{
		case BIAS_ELEC:
			if (!one_in_(3))
			{
				type = ACT_BO_ELEC_1;
			}
			else if (!one_in_(5))
			{
				type = ACT_BA_ELEC_2;
			}
			else
			{
				type = ACT_BA_ELEC_3;
			}
			chance = 101;
			break;

		case BIAS_POIS:
			type = ACT_BA_POIS_1;
			chance = 101;
			break;

		case BIAS_FIRE:
			if (!one_in_(3))
			{
				type = ACT_BO_FIRE_1;
			}
			else if (!one_in_(5))
			{
				type = ACT_BA_FIRE_1;
			}
			else
			{
				type = ACT_BA_FIRE_2;
			}
			chance = 101;
			break;

		case BIAS_COLD:
			chance = 101;
			if (!one_in_(3))
				type = ACT_BO_COLD_1;
			else if (!one_in_(3))
				type = ACT_BA_COLD_1;
			else if (!one_in_(3))
				type = ACT_BA_COLD_2;
			else
				type = ACT_BA_COLD_3;
			break;

		case BIAS_CHAOS:
			chance = 50;
			if (one_in_(6))
				type = ACT_SUMMON_DEMON;
			else
				type = ACT_CALL_CHAOS;
			break;

		case BIAS_PRIESTLY:
			chance = 101;

			if (one_in_(13))
				type = ACT_CHARM_UNDEAD;
			else if (one_in_(12))
				type = ACT_BANISH_EVIL;
			else if (one_in_(11))
				type = ACT_DISP_EVIL;
			else if (one_in_(10))
				type = ACT_PROT_EVIL;
			else if (one_in_(9))
				type = ACT_CURE_1000;
			else if (one_in_(8))
				type = ACT_CURE_700;
			else if (one_in_(7))
				type = ACT_REST_ALL;
			else if (one_in_(6))
				type = ACT_REST_EXP;
			else
				type = ACT_CURE_MW;
			break;

		case BIAS_NECROMANTIC:
			chance = 101;
			if (one_in_(66))
				type = ACT_WRAITH;
			else if (one_in_(13))
				type = ACT_DISP_GOOD;
			else if (one_in_(9))
				type = ACT_MASS_GENO;
			else if (one_in_(8))
				type = ACT_GENOCIDE;
			else if (one_in_(13))
				type = ACT_SUMMON_UNDEAD;
			else if (one_in_(9))
				type = ACT_DRAIN_2;
			else if (one_in_(6))
				type = ACT_CHARM_UNDEAD;
			else
				type = ACT_DRAIN_1;
			break;

		case BIAS_LAW:
			chance = 101;
			if (one_in_(8))
				type = ACT_BANISH_EVIL;
			else if (one_in_(4))
				type = ACT_DISP_EVIL;
			else
				type = ACT_PROT_EVIL;
			break;

		case BIAS_ROGUE:
			chance = 101;
			if (one_in_(50))
				type = ACT_SPEED;
			else if (one_in_(4))
				type = ACT_SLEEP;
			else if (one_in_(3))
				type = ACT_DETECT_ALL;
			else if (one_in_(8))
				type = ACT_ID_FULL;
			else
				type = ACT_ID_PLAIN;
			break;

		case BIAS_MAGE:
			chance = 66;
			if (one_in_(20))
				type = ACT_SUMMON_ELEMENTAL;
			else if (one_in_(10))
				type = ACT_SUMMON_PHANTOM;
			else if (one_in_(5))
				type = ACT_RUNE_EXPLO;
			else
				type = ACT_ESP;
			break;

		case BIAS_WARRIOR:
			chance = 80;
			if (one_in_(100))
				type = ACT_INVULN;
			else
				type = ACT_BERSERK;
			break;

		case BIAS_RANGER:
			chance = 101;
			if (one_in_(20))
				type = ACT_CHARM_ANIMALS;
			else if (one_in_(7))
				type = ACT_SUMMON_ANIMAL;
			else if (one_in_(6))
				type = ACT_CHARM_ANIMAL;
			else if (one_in_(4))
				type = ACT_RESIST_ALL;
			else if (one_in_(3))
				type = ACT_SATIATE;
			else
				type = ACT_CURE_POISON;
			break;
	}

	if (!type || (randint1(100) >= chance))
	{
		one_activation(o_ptr);
		return;
	}

	/* A type was chosen... */
	o_ptr->xtra2 = (byte_hack)type;
	add_flag(o_ptr->art_flags, TR_ACTIVATE);
	o_ptr->timeout = 0;
}

/*!
 * @brief �����ॢ���ƥ��ե����������桢�оݤΥ��֥������Ȥ�̾����Ϳ���롣/ Set name of randomartifact.
 * @details ��Ψ�ˤ�äơ����������á������á�����̾�Τ����줫��Ĥ�Ϳ�����롣
 * @param o_ptr ������Υ����ƥ໲�ȥݥ���
 * @param return_name ̾�����֤������ʸ���󻲾ȥݥ���
 * @param armour �оݤΥ��֥������Ȥ��ɶ��ݤ�
 * @param power �äδ��Ȥʤ륪�֥������Ȥβ��ͥ�٥�(0=������1=��̡�2=��̡�3�ʾ�=���)
 * @return �ʤ�
 */
static void get_random_name(object_type *o_ptr, char *return_name, bool armour, int power)
{
	int prob = randint1(100);

	if (prob <= SINDARIN_NAME)
	{
		get_table_sindarin(return_name);
	}
	else if (prob <= TABLE_NAME)
	{
		get_table_name(return_name);
	}
	else
	{
		cptr filename;

		switch (armour)
		{
			case 1:
				switch (power)
				{
					case 0:
						filename = _("a_cursed_j.txt", "a_cursed.txt");
						break;
					case 1:
						filename = _("a_low_j.txt", "a_low.txt");
						break;
					case 2:
						filename = _("a_med_j.txt", "a_med.txt");
						break;
					default:
						filename = _("a_high_j.txt", "a_high.txt");
				}
				break;
			default:
				switch (power)
				{
					case 0:
						filename = _("w_cursed_j.txt", "w_cursed.txt");
						break;
					case 1:
						filename = _("w_low_j.txt", "w_low.txt");
						break;
					case 2:
						filename = _("w_med_j.txt", "w_med.txt");
						break;
					default:
						filename = _("w_high_j.txt", "w_high.txt");
				}
		}

		(void)get_rnd_line(filename, o_ptr->artifact_bias, return_name);
#ifdef JP
		 if (return_name[0] == 0) get_table_name(return_name);
#endif
	}
}

/*!
 * @brief �����ॢ���ƥ��ե����������Υᥤ��롼����
 * @details �����������Ѥ�Ǥ��륪�֥������Ȥι�¤�Τ򡢥����ƥ��ե����ȤȤ��ƶ������롣
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @param a_scroll �����ƥ��ե����������δ�ʪ��ν����������Υ����ƥ��ե����Ȥ������оݳ��Ȥʤ롣
 * @return ���TRUE(1)���֤�
 */
bool create_artifact(object_type *o_ptr, bool a_scroll)
{
	char    new_name[1024];
	int     has_pval = 0;
	int     powers = randint1(5) + 1;
	int     max_powers;
	int     max_type = (object_is_weapon_ammo(o_ptr) ? 7 : 5);
	int     power_level;
	s32b    total_flags;
	bool    a_cursed = FALSE;
	int     warrior_artifact_bias = 0;
	int i;

	/* Reset artifact bias */
	o_ptr->artifact_bias = 0;

	/* Nuke enchantments */
	o_ptr->name1 = 0;
	o_ptr->name2 = 0;

	for (i = 0; i < TR_FLAG_SIZE; i++)
		o_ptr->art_flags[i] |= k_info[o_ptr->k_idx].flags[i];

	if (o_ptr->pval) has_pval = TRUE;

	if (a_scroll && one_in_(4))
	{
		switch (p_ptr->pclass)
		{
			case CLASS_WARRIOR:
			case CLASS_BERSERKER:
			case CLASS_ARCHER:
			case CLASS_SAMURAI:
			case CLASS_CAVALRY:
			case CLASS_SMITH:
				o_ptr->artifact_bias = BIAS_WARRIOR;
				break;
			case CLASS_MAGE:
			case CLASS_HIGH_MAGE:
			case CLASS_SORCERER:
			case CLASS_MAGIC_EATER:
			case CLASS_BLUE_MAGE:
				o_ptr->artifact_bias = BIAS_MAGE;
				break;
			case CLASS_PRIEST:
				o_ptr->artifact_bias = BIAS_PRIESTLY;
				break;
			case CLASS_ROGUE:
			case CLASS_NINJA:
				o_ptr->artifact_bias = BIAS_ROGUE;
				warrior_artifact_bias = 25;
				break;
			case CLASS_RANGER:
			case CLASS_SNIPER:
				o_ptr->artifact_bias = BIAS_RANGER;
				warrior_artifact_bias = 30;
				break;
			case CLASS_PALADIN:
				o_ptr->artifact_bias = BIAS_PRIESTLY;
				warrior_artifact_bias = 40;
				break;
			case CLASS_WARRIOR_MAGE:
			case CLASS_RED_MAGE:
				o_ptr->artifact_bias = BIAS_MAGE;
				warrior_artifact_bias = 40;
				break;
			case CLASS_CHAOS_WARRIOR:
				o_ptr->artifact_bias = BIAS_CHAOS;
				warrior_artifact_bias = 40;
				break;
			case CLASS_MONK:
			case CLASS_FORCETRAINER:
				o_ptr->artifact_bias = BIAS_PRIESTLY;
				break;
			case CLASS_MINDCRAFTER:
			case CLASS_BARD:
				if (randint1(5) > 2) o_ptr->artifact_bias = BIAS_PRIESTLY;
				break;
			case CLASS_TOURIST:
				if (randint1(5) > 2) o_ptr->artifact_bias = BIAS_WARRIOR;
				break;
			case CLASS_IMITATOR:
				if (randint1(2) > 1) o_ptr->artifact_bias = BIAS_RANGER;
				break;
			case CLASS_BEASTMASTER:
				o_ptr->artifact_bias = BIAS_CHR;
				warrior_artifact_bias = 50;
				break;
			case CLASS_MIRROR_MASTER:
				if (randint1(4) > 1) 
				{
				    o_ptr->artifact_bias = BIAS_MAGE;
				}
				else
				{
				    o_ptr->artifact_bias = BIAS_ROGUE;
				}
				break;
		}
	}

	if (a_scroll && (randint1(100) <= warrior_artifact_bias))
		o_ptr->artifact_bias = BIAS_WARRIOR;

	strcpy(new_name, "");

	if (!a_scroll && one_in_(A_CURSED))
		a_cursed = TRUE;
	if (((o_ptr->tval == TV_AMULET) || (o_ptr->tval == TV_RING)) && object_is_cursed(o_ptr))
		a_cursed = TRUE;

	while (one_in_(powers) || one_in_(7) || one_in_(10))
		powers++;

	if (!a_cursed && one_in_(WEIRD_LUCK))
		powers *= 2;

	if (a_cursed) powers /= 2;

	max_powers = powers * 20; /* #tang powers -> powers * 20 */
	/* Main loop */
	while (powers--)
	{
		switch (randint1(max_type))
		{
			case 1: case 2:
				random_plus(o_ptr);
				has_pval = TRUE;
				break;
			case 3: case 4:
				if (one_in_(2) && object_is_weapon_ammo(o_ptr) && (o_ptr->tval != TV_BOW))
				{
					if (a_cursed && !one_in_(13)) break;
					if (one_in_(13))
					{
						if (one_in_(o_ptr->ds+4)) o_ptr->ds++;
					}
					else
					{
						if (one_in_(o_ptr->dd+1)) o_ptr->dd++;
					}
				}
				else
					random_resistance(o_ptr);
				break;
			case 5:
				random_misc(o_ptr);
				break;
			case 6: case 7:
				random_slay(o_ptr);
				break;
			default:
				if (p_ptr->wizard) msg_print("Switch error in create_artifact!");
				powers++;
		}
	};

	if (has_pval)
	{
#if 0
		add_flag(o_ptr->art_flags, TR_SHOW_MODS);

		/* This one commented out by gw's request... */
		if (!a_scroll)
			add_flag(o_ptr->art_flags, TR_HIDE_TYPE);
#endif

		if (have_flag(o_ptr->art_flags, TR_BLOWS))
		{
			o_ptr->pval = randint1(3); /* #tang 2 -> 3 */
			if ((o_ptr->tval == TV_SWORD) && (o_ptr->sval == SV_HAYABUSA))
				o_ptr->pval++;
		}
		else
		{
			do
			{
				o_ptr->pval++;
			}
			while (o_ptr->pval < randint1(5) || one_in_(o_ptr->pval));
		}

		if ((o_ptr->pval > 5) && !one_in_(WEIRD_LUCK)) /* #tang 4 -> 5 */
			o_ptr->pval = 5; /* #tang 4 -> 5 */
	}


	/* give it some plusses... */
	if (object_is_armour(o_ptr))
		o_ptr->to_a += randint1(o_ptr->to_a > 19 ? 1 : 20 - o_ptr->to_a);
	else if (object_is_weapon_ammo(o_ptr))
	{
		o_ptr->to_h += randint1(o_ptr->to_h > 19 ? 1 : 20 - o_ptr->to_h);
		o_ptr->to_d += randint1(o_ptr->to_d > 19 ? 1 : 20 - o_ptr->to_d);
		if ((have_flag(o_ptr->art_flags, TR_WIS)) && (o_ptr->pval > 0)) add_flag(o_ptr->art_flags, TR_BLESSED);
	}

	/* Just to be sure */
	add_flag(o_ptr->art_flags, TR_IGNORE_ACID);
	add_flag(o_ptr->art_flags, TR_IGNORE_ELEC);
	add_flag(o_ptr->art_flags, TR_IGNORE_FIRE);
	add_flag(o_ptr->art_flags, TR_IGNORE_COLD);

	total_flags = flag_cost(o_ptr, o_ptr->pval);

	if (a_cursed) curse_artifact(o_ptr);

	if (!a_cursed &&
	    one_in_(object_is_armour(o_ptr) ? ACTIVATION_CHANCE * 2 : ACTIVATION_CHANCE))
	{
		o_ptr->xtra2 = 0;
		give_activation_power(o_ptr);
	}

	if (object_is_armour(o_ptr))
	{
		while ((o_ptr->to_d+o_ptr->to_h) > 20)
		{
			if (one_in_(o_ptr->to_d) && one_in_(o_ptr->to_h)) break;
			o_ptr->to_d -= (s16b)randint0(3);
			o_ptr->to_h -= (s16b)randint0(3);
		}
		while ((o_ptr->to_d+o_ptr->to_h) > 10)
		{
			if (one_in_(o_ptr->to_d) || one_in_(o_ptr->to_h)) break;
			o_ptr->to_d -= (s16b)randint0(3);
			o_ptr->to_h -= (s16b)randint0(3);
		}
	}

	if (((o_ptr->artifact_bias == BIAS_MAGE) || (o_ptr->artifact_bias == BIAS_INT)) && (o_ptr->tval == TV_GLOVES)) add_flag(o_ptr->art_flags, TR_FREE_ACT);

	if ((o_ptr->tval == TV_SWORD) && (o_ptr->sval == SV_DOKUBARI))
	{
		o_ptr->to_h = 0;
		o_ptr->to_d = 0;
		remove_flag(o_ptr->art_flags, TR_BLOWS);
		remove_flag(o_ptr->art_flags, TR_FORCE_WEAPON);
		remove_flag(o_ptr->art_flags, TR_SLAY_ANIMAL);
		remove_flag(o_ptr->art_flags, TR_SLAY_EVIL);
		remove_flag(o_ptr->art_flags, TR_SLAY_UNDEAD);
		remove_flag(o_ptr->art_flags, TR_SLAY_DEMON);
		remove_flag(o_ptr->art_flags, TR_SLAY_ORC);
		remove_flag(o_ptr->art_flags, TR_SLAY_TROLL);
		remove_flag(o_ptr->art_flags, TR_SLAY_GIANT);
		remove_flag(o_ptr->art_flags, TR_SLAY_DRAGON);
		remove_flag(o_ptr->art_flags, TR_KILL_DRAGON);
		remove_flag(o_ptr->art_flags, TR_SLAY_HUMAN);
		remove_flag(o_ptr->art_flags, TR_VORPAL);
		remove_flag(o_ptr->art_flags, TR_BRAND_POIS);
		remove_flag(o_ptr->art_flags, TR_BRAND_ACID);
		remove_flag(o_ptr->art_flags, TR_BRAND_ELEC);
		remove_flag(o_ptr->art_flags, TR_BRAND_FIRE);
		remove_flag(o_ptr->art_flags, TR_BRAND_COLD);
	}

	if (!object_is_weapon_ammo(o_ptr))
	{
		/* For armors */
		if (a_cursed) power_level = 0;
		else if (total_flags < 10000) power_level = 1; /* #tang 15000 -> 10000 */
		else if (total_flags < 30000) power_level = 2; /* #tang 35000 -> 30000 */
		else power_level = 3;
	}

	else
	{
		/* For weapons */
		if (a_cursed) power_level = 0;
		else if (total_flags < 15000) power_level = 1; /* #tang 20000 -> 15000 */
		else if (total_flags < 35000) power_level = 2; /* #tang 45000 -> 35000 */
		else power_level = 3;
	}

	if (a_scroll)
	{
		char dummy_name[80] = "";
		cptr ask_msg = _("���Υ����ƥ��ե����Ȥ򲿤�̾�դ��ޤ�����", "What do you want to call the artifact? ");

		/* Identify it fully */
		object_aware(o_ptr);
		object_known(o_ptr);

		/* Mark the item as fully known */
		o_ptr->ident |= (IDENT_MENTAL);

		/* For being treated as random artifact in screen_object() */
		o_ptr->art_name = quark_add("");

		(void)screen_object(o_ptr, 0L);

		if (!get_string(ask_msg, dummy_name, sizeof dummy_name)
		    || !dummy_name[0])
		{
			/* Cancelled */
			if (one_in_(2))
			{
				get_table_sindarin_aux(dummy_name);
			}
			else
			{
				get_table_name_aux(dummy_name);
			}
		}
		sprintf(new_name, _("��%s��", "'%s'"), dummy_name);
		chg_virtue(V_INDIVIDUALISM, 2);
		chg_virtue(V_ENCHANT, 5);
	}
	else
	{
		get_random_name(o_ptr, new_name, object_is_armour(o_ptr), power_level);
	}

	/* Save the inscription */
	o_ptr->art_name = quark_add(new_name);

	msg_format_wizard(CHEAT_OBJECT, _("�ѥ %d �� ����%ld �Υ����ॢ���ƥ��ե��������� �Х������ϡ�%s��",
		"Random artifact generated - Power:%d Value:%d Bias:%s."), max_powers, total_flags, artifact_bias_name[o_ptr->artifact_bias]);

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP);

	return TRUE;
}

/*!
 * @brief ���֥������Ȥ���ǽ��ȯưID��������롣
 * @details �����Ĥ��Υ��������������Ƥ���ȯư���̤��顢
 * ����դˤ����Ϳ����ꥢ���ƥ��ե����ȡ䥨��������ॢ���ƥ��ե����ȡ�١��������ƥ��ͥ���̤��������Ƥ�����
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return ȯư���̤�ID���֤�
 */
int activation_index(object_type *o_ptr)
{
	/* Give priority to weaponsmith's essential activations */
	if (object_is_smith(o_ptr))
	{
		switch (o_ptr->xtra3 - 1)
		{
		case ESSENCE_TMP_RES_ACID: return ACT_RESIST_ACID;
		case ESSENCE_TMP_RES_ELEC: return ACT_RESIST_ELEC;
		case ESSENCE_TMP_RES_FIRE: return ACT_RESIST_FIRE;
		case ESSENCE_TMP_RES_COLD: return ACT_RESIST_COLD;
		case TR_IMPACT: return ACT_QUAKE;
		}
	}

	if (object_is_fixed_artifact(o_ptr))
	{
		if (have_flag(a_info[o_ptr->name1].flags, TR_ACTIVATE))
		{
			return a_info[o_ptr->name1].act_idx;
		}
	}
	if (object_is_ego(o_ptr))
	{
		if (have_flag(e_info[o_ptr->name2].flags, TR_ACTIVATE))
		{
			return e_info[o_ptr->name2].act_idx;
		}
	}
	if (!object_is_random_artifact(o_ptr))
	{
		if (have_flag(k_info[o_ptr->k_idx].flags, TR_ACTIVATE))
		{
			return k_info[o_ptr->k_idx].act_idx;
		}
	}

	return o_ptr->xtra2;
}

/*!
 * @brief ���֥������Ȥ���ȯư���̹�¤�ΤΥݥ��󥿤�������롣
 * @details activation_index() �ؿ��η�̤��黲�Ȥ��롣
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return ȯư���̹�¤�ΤΥݥ��󥿤��֤�
 */
const activation_type* find_activation_info(object_type *o_ptr)
{
	const int index = activation_index(o_ptr);
	const activation_type* p;

	for (p = activation_info; p->flag != NULL; ++ p) {
		if (p->index == index)
		{
			return p;
		}
	}

	return NULL;
}

/*!
 * @brief ȯư�ˤ��֥쥹��°���򥢥��ƥ�������������򤷡��¹Ԥ�������롣/ Dragon breath activation
 * @details �оݤȤʤ������� dragonbreath_info �ơ��֥�򻲾ȤΤ��ȡ�
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return ȯư�¹Ԥ�������֤���
 */
static bool activate_dragon_breath(object_type *o_ptr)
{
	u32b flgs[TR_FLAG_SIZE]; /* for resistance flags */
	int type[20];
	cptr name[20];
	int i, dir, t, n = 0;

	if (!get_aim_dir(&dir)) return FALSE;

	object_flags(o_ptr, flgs);

	for (i = 0; dragonbreath_info[i].flag != 0; i++)
	{
		if (have_flag(flgs, dragonbreath_info[i].flag))
		{
			type[n] = dragonbreath_info[i].type;
			name[n] = dragonbreath_info[i].name;
			n++;
		}
	}

	/* Paranoia */
	if (n == 0) return FALSE;

	/* Stop speaking */
	if (music_singing_any()) stop_singing();
	if (hex_spelling_any()) stop_hex_spell_all();

	t = randint0(n);
	msg_format(_("���ʤ���%s�Υ֥쥹���Ǥ�����", "You breathe %s."), name[t]);
	fire_breath(type[t], dir, 250, 4);

	return TRUE;
}

/*!
 * @brief �����ƥ��ȯư���̤�������롣
 * @details activate_random_artifact()�Ȥ���Ƥ��뤬���ºݤ���ȯư�����礵�줿��
 * @todo �ޤ򸫤ƴؿ�̾�������뤳�ȡ�
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @return ȯư�¹Ԥ�������֤���
 */
bool activate_random_artifact(object_type *o_ptr)
{
	PLAYER_LEVEL plev = p_ptr->lev;
	int k, dummy = 0;
	DIRECTION dir;
	cptr name = k_name + k_info[o_ptr->k_idx].name;
	const activation_type* const act_ptr = find_activation_info(o_ptr);

	/* Paranoia */
	if (!act_ptr) {
		/* Maybe forgot adding information to activation_info table ? */
		msg_print("Activation information is not found.");
		return FALSE;
	}

	/* Activate for attack */
	switch (act_ptr->index)
	{
		case ACT_SUNLIGHT:
		{
			if (!get_aim_dir(&dir)) return FALSE;
			msg_print(_("���۸����������줿��", "A line of sunlight appears."));
			(void)lite_line(dir, damroll(6, 8));
			break;
		}

		case ACT_BO_MISS_1:
		{
			msg_print(_("��������������餤�����뤯�����Ƥ���...", "It glows extremely brightly..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_bolt(GF_MISSILE, dir, damroll(2, 6));
			break;
		}

		case ACT_BA_POIS_1:
		{
			msg_print(_("�����ǻ�п���̮ư���Ƥ���...","It throbs deep green..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_ball(GF_POIS, dir, 12, 3);
			break;
		}

		case ACT_BO_ELEC_1:
		{
			msg_print(_("����ϲв֤�ʤ��줿...", "It is covered in sparks..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_bolt(GF_ELEC, dir, damroll(4, 8));
			break;
		}

		case ACT_BO_ACID_1:
		{
			msg_print(_("����ϻ���ʤ��줿...","It is covered in acid..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_bolt(GF_ACID, dir, damroll(5, 8));
			break;
		}

		case ACT_BO_COLD_1:
		{
			msg_print(_("���������ʤ��줿...","It is covered in frost..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_bolt(GF_COLD, dir, damroll(6, 8));
			break;
		}

		case ACT_BO_FIRE_1:
		{
			msg_print(_("����ϱ��ʤ��줿...","It is covered in fire..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_bolt(GF_FIRE, dir, damroll(9, 8));
			break;
		}

		case ACT_BA_COLD_1:
		{
			msg_print(_("���������ʤ��줿...","It is covered in frost..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_ball(GF_COLD, dir, 48, 2);
			break;
		}
		
		case ACT_BA_COLD_2:
		{
			msg_print(_("������Ĥ��㤷��������...", "It glows an intense blue..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_ball(GF_COLD, dir, 100, 2);
			break;
		}
		
		case ACT_BA_COLD_3:
		{
			msg_print(_("���뤯�򿧤˵����Ƥ���...", "It glows bright white..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_ball(GF_COLD, dir, 400, 3);
			break;
		}

		case ACT_BA_FIRE_1:
		{
			msg_print(_("������֤��㤷��������...","It glows an intense red..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_ball(GF_FIRE, dir, 72, 2);
			break;
		}
		
		case ACT_BA_FIRE_2:
		{
			msg_format(_("%s����꤬�᤭�Ф���...", "The %s rages in fire..."), name);
			if (!get_aim_dir(&dir)) return FALSE;
			fire_ball(GF_FIRE, dir, 120, 3);
			break;
		}
		
		case ACT_BA_FIRE_3:
		{
			msg_print(_("���ֿ��˵����Ƥ���...", "It glows deep red..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_ball(GF_FIRE, dir, 300, 3);
			break;
		}
		
		case ACT_BA_FIRE_4:
		{
			msg_print(_("������֤��㤷��������...","It glows an intense red..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_ball(GF_FIRE, dir, 100, 2);
			break;
		}
		
		case ACT_BA_ELEC_2:
		{
			msg_print(_("�ŵ����ѥ��ѥ�����Ω�Ƥ�...","It crackles with electricity..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_ball(GF_ELEC, dir, 100, 3);
			break;
		}
		
		case ACT_BA_ELEC_3:
		{
			msg_print(_("���Ŀ��˵����Ƥ���...", "It glows deep blue..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_ball(GF_ELEC, dir, 500, 3);
			break;
		}
		
		case ACT_BA_ACID_1:
		{
			msg_print(_("����Ϲ����㤷��������...","It glows an intense black..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_ball(GF_ACID, dir, 100, 2);
			break;
		}
		
		case ACT_BA_NUKE_1:
		{
			msg_print(_("������Ф˷㤷��������...","It glows an intense green..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_ball(GF_NUKE, dir, 100, 2);
			break;
		}
		
		case ACT_HYPODYNAMIA_1:
		{
			msg_format(_("���ʤ���%s��Ũ�����Ủ���褦̿������", "You order the %s to strangle your opponent."), name);
			if (!get_aim_dir(&dir)) return FALSE;
			if (hypodynamic_bolt(dir, 100))
			break;
		}

		case ACT_HYPODYNAMIA_2:
		{
			msg_print(_("���������Ƥ���...", "It glows black..."));
			if (!get_aim_dir(&dir)) return FALSE;
			hypodynamic_bolt(dir, 120);
			break;
		}

		case ACT_DRAIN_1:
		{
			if (!get_aim_dir(&dir)) return FALSE;
			for (dummy = 0; dummy < 3; dummy++)
			{
				if (hypodynamic_bolt(dir, 50))
				hp_player(50);
			}
			break;
		}

		case ACT_BO_MISS_2:
		{
			msg_print(_("��ˡ�Υȥ������줿...", "It grows magical spikes..."));
			if (!get_aim_dir(&dir)) return FALSE;
			fire_bolt(GF_ARROW, dir, 150);
			break;
		}

		case ACT_WHIRLWIND:
		{
			{
				int y = 0, x = 0;
				cave_type       *c_ptr;
				monster_type    *m_ptr;

				for (dir = 0; dir <= 9; dir++)
				{
					y = p_ptr->y + ddy[dir];
					x = p_ptr->x + ddx[dir];
					c_ptr = &cave[y][x];

					/* Get the monster */
					m_ptr = &m_list[c_ptr->m_idx];

					/* Hack -- attack monsters */
					if (c_ptr->m_idx && (m_ptr->ml || cave_have_flag_bold(y, x, FF_PROJECT)))
						py_attack(y, x, 0);
				}
			}
			break;
		}

		case ACT_DRAIN_2:
		{
			if (!get_aim_dir(&dir)) return FALSE;
			for (dummy = 0; dummy < 3; dummy++)
			{
				if (hypodynamic_bolt(dir, 100))
				hp_player(100);
			}
			break;
		}


		case ACT_CALL_CHAOS:
		{
			msg_print(_("�͡��ʿ��βв֤�ȯ���Ƥ���...","It glows in scintillating colours..."));
			call_chaos();
			break;
		}

		case ACT_ROCKET:
		{
			if (!get_aim_dir(&dir)) return FALSE;
			msg_print(_("���åȤ�ȯ�ͤ�����", "You launch a rocket!"));
			fire_ball(GF_ROCKET, dir, 250 + plev*3, 2);
			break;
		}

		case ACT_DISP_EVIL:
		{
			msg_print(_("������ʷ�ϵ�����������...", "It floods the area with goodness..."));
			dispel_evil(p_ptr->lev * 5);
			break;
		}

		case ACT_BA_MISS_3:
		{
			if (!get_aim_dir(&dir)) return FALSE;
			msg_print(_("���ʤ��ϥ�����ȤΥ֥쥹���Ǥ�����", "You breathe the elements."));
			fire_breath(GF_MISSILE, dir, 300, 4);
			break;
		}

		case ACT_DISP_GOOD:
		{
			msg_print(_("�ٰ���ʷ�ϵ�����������...", "It floods the area with evil..."));
			dispel_good(p_ptr->lev * 5);
			break;
		}

		case ACT_BO_MANA:
		{
			msg_format(_("%s����ˡ�Υȥ������줿...", "The %s grows magical spikes..."), name);
			if (!get_aim_dir(&dir)) return FALSE;
			fire_bolt(GF_ARROW, dir, 150);
			break;
		}

		case ACT_BA_WATER:
		{
			msg_format(_("%s�������Ŀ��˸�ư���Ƥ���...", "The %s throbs deep blue..."), name);
			if (!get_aim_dir(&dir)) return FALSE;
			fire_ball(GF_WATER, dir, 200, 3);
			break;
		}

		case ACT_BA_DARK:
		{
			msg_format(_("%s�������Ǥ�ʤ��줿...","The %s is coverd in pitch-darkness..."), name);
			if (!get_aim_dir(&dir)) return FALSE;
			fire_ball(GF_DARK, dir, 250, 4);
			break;
		}

		case ACT_BA_MANA:
		{
			msg_format(_("%s�����򤯸��ä�������", "The %s glows pale..."), name);
			if (!get_aim_dir(&dir)) return FALSE;
			fire_ball(GF_MANA, dir, 250, 4);
			break;
		}

		case ACT_PESTICIDE:
		{
			msg_print(_("���ʤ��ϳ������ݤ�����","You exterminate small life."));
			(void)dispel_monsters(4);
			break;
		}

		case ACT_BLINDING_LIGHT:
		{
			msg_format(_("%s�����������ǵ�����...", "The %s gleams with blinding light..."), name);
			fire_ball(GF_LITE, 0, 300, 6);
			confuse_monsters(3 * p_ptr->lev / 2);
			break;
		}

		case ACT_BIZARRE:
		{
			msg_format(_("%s�ϼ����˵�����...", "The %s glows intensely black..."), name);
			if (!get_aim_dir(&dir)) return FALSE;
			ring_of_power(dir);
			break;
		}

		case ACT_CAST_BA_STAR:
		{
			HIT_POINT num = damroll(5, 3);
			POSITION y = 0, x = 0;
			int attempts;
			msg_format(_("%s����ʤ�ʤ��줿...","The %s is surrounded by lightning..."), name);
			for (k = 0; k < num; k++)
			{
				attempts = 1000;

				while (attempts--)
				{
					scatter(&y, &x, p_ptr->y, p_ptr->x, 4, 0);

					if (!cave_have_flag_bold(y, x, FF_PROJECT)) continue;

					if (!player_bold(y, x)) break;
				}

				project(0, 3, y, x, 150, GF_ELEC,
							(PROJECT_THRU | PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL), -1);
			}

			break;
		}

		case ACT_BLADETURNER:
		{
			if (!get_aim_dir(&dir)) return FALSE;
			msg_print(_("���ʤ��ϥ�����ȤΥ֥쥹���Ǥ�����", "You breathe the elements."));
			fire_breath(GF_MISSILE, dir, 300, 4);
			msg_print(_("�����͡��ʿ��˵�����...", "Your armor glows many colours..."));
			(void)set_afraid(0);
			(void)set_hero(randint1(50) + 50, FALSE);
			(void)hp_player(10);
			(void)set_blessed(randint1(50) + 50, FALSE);
			(void)set_oppose_acid(randint1(50) + 50, FALSE);
			(void)set_oppose_elec(randint1(50) + 50, FALSE);
			(void)set_oppose_fire(randint1(50) + 50, FALSE);
			(void)set_oppose_cold(randint1(50) + 50, FALSE);
			(void)set_oppose_pois(randint1(50) + 50, FALSE);
			break;
		}

		case ACT_BR_FIRE:
		{
			if (!get_aim_dir(&dir)) return FALSE;
			fire_breath(GF_FIRE, dir, 200, 2);
			if ((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_FLAMES))
			{
				(void)set_oppose_fire(randint1(20) + 20, FALSE);
			}
			break;
		}
		case ACT_BR_COLD:
		{
			if (!get_aim_dir(&dir)) return FALSE;
			fire_breath(GF_COLD, dir, 200, 2);
			if ((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_ICE))
			{
				(void)set_oppose_cold(randint1(20) + 20, FALSE);
			}
			break;
		}
		case ACT_BR_DRAGON:
		{
			if (!activate_dragon_breath(o_ptr)) return FALSE;
			break;
		}

		/* Activate for other offensive action */

		case ACT_CONFUSE:
		{
			msg_print(_("�͡��ʿ��βв֤�ȯ���Ƥ���...", "It glows in scintillating colours..."));
			if (!get_aim_dir(&dir)) return FALSE;
			confuse_monster(dir, 20);
			break;
		}

		case ACT_SLEEP:
		{
			msg_print(_("���Ŀ��˵����Ƥ���...", "It glows deep blue..."));
			sleep_monsters_touch();
			break;
		}

		case ACT_QUAKE:
		{
			earthquake(p_ptr->y, p_ptr->x, 5);
			break;
		}

		case ACT_TERROR:
		{
			turn_monsters(40 + p_ptr->lev);
			break;
		}

		case ACT_TELE_AWAY:
		{
			if (!get_aim_dir(&dir)) return FALSE;
			(void)fire_beam(GF_AWAY_ALL, dir, plev);
			break;
		}

		case ACT_BANISH_EVIL:
		{
			if (banish_evil(100))
			{
				msg_print(_("�����ƥ��ե����Ȥ��Ϥ��ٰ����Ǥ�ʧ�ä���", "The power of the artifact banishes evil!"));
			}
			break;
		}

		case ACT_GENOCIDE:
		{
			msg_print(_("���Ŀ��˵����Ƥ���...", "It glows deep blue..."));
			(void)symbol_genocide(200, TRUE);
			break;
		}

		case ACT_MASS_GENO:
		{
			msg_print(_("�Ҥɤ��Ԥ�����ή��Ф�...", "It lets out a long, shrill note..."));
			(void)mass_genocide(200, TRUE);
			break;
		}

		case ACT_SCARE_AREA:
		{
			if (music_singing_any()) stop_singing();
			if (hex_spelling_any()) stop_hex_spell_all();
			msg_print(_("���ʤ����϶���������᤭�Ĥ餷�������Ϥ�Ũ���̤���äƤ���!",
					"You wind a mighty blast; your enemies tremble!"));
			(void)turn_monsters((3 * p_ptr->lev / 2) + 10);
			break;
		}

		case ACT_AGGRAVATE:
		{
			if (o_ptr->name1 == ART_HYOUSIGI)
			{
				msg_print(_("����ڤ��Ǥä���", "You beat Your wooden clappers."));
			}
			else
			{
				msg_format(_("%s���Բ���ʪ����Ω�Ƥ���","The %s sounds an unpleasant noise."), name);
			}
			aggravate_monsters(0);
			break;
		}

		/* Activate for summoning / charming */

		case ACT_CHARM_ANIMAL:
		{
			if (!get_aim_dir(&dir)) return FALSE;
			(void)charm_animal(dir, plev * 2);
			break;
		}

		case ACT_CHARM_UNDEAD:
		{
			if (!get_aim_dir(&dir)) return FALSE;
			(void)control_one_undead(dir, plev * 2);
			break;
		}

		case ACT_CHARM_OTHER:
		{
			if (!get_aim_dir(&dir)) return FALSE;
			(void)charm_monster(dir, plev * 2);
			break;
		}

		case ACT_CHARM_ANIMALS:
		{
			(void)charm_animals(plev * 2);
			break;
		}

		case ACT_CHARM_OTHERS:
		{
			charm_monsters(plev * 2);
			break;
		}

		case ACT_SUMMON_ANIMAL:
		{
			(void)summon_specific(-1, p_ptr->y, p_ptr->x, plev, SUMMON_ANIMAL_RANGER, (PM_ALLOW_GROUP | PM_FORCE_PET));
			break;
		}

		case ACT_SUMMON_PHANTOM:
		{
			msg_print(_("����򾤴�������", "You summon a phantasmal servant."));
			(void)summon_specific(-1, p_ptr->y, p_ptr->x, dun_level, SUMMON_PHANTOM, (PM_ALLOW_GROUP | PM_FORCE_PET));
			break;
		}

		case ACT_SUMMON_ELEMENTAL:
		{
			bool pet = one_in_(3);
			BIT_FLAGS mode = 0L;

			if (!(pet && (plev < 50))) mode |= PM_ALLOW_GROUP;
			if (pet) mode |= PM_FORCE_PET;
			else mode |= PM_NO_PET;

			if (summon_specific((pet ? -1 : 0), p_ptr->y, p_ptr->x, ((plev * 3) / 2), SUMMON_ELEMENTAL, mode))
			{
				msg_print(_("�����󥿥뤬���줿...", "An elemental materializes..."));
				if (pet)
					msg_print(_("���ʤ����������Ƥ���褦����", "It seems obedient to you."));
				else
					msg_print(_("����򥳥�ȥ���Ǥ��ʤ��ä���", "You fail to control it!"));
			}

			break;
		}

		case ACT_SUMMON_DEMON:
		{
			bool pet = one_in_(3);
			BIT_FLAGS mode = 0L;

			if (!(pet && (plev < 50))) mode |= PM_ALLOW_GROUP;
			if (pet) mode |= PM_FORCE_PET;
			else mode |= PM_NO_PET;

			if (summon_specific((pet ? -1 : 0), p_ptr->y, p_ptr->x, ((plev * 3) / 2), SUMMON_DEMON, mode))
			{
				msg_print(_("β���ΰ���������������", "The area fills with a stench of sulphur and brimstone."));
				if (pet)
					msg_print(_("�֤��ѤǤ������ޤ�����������͡�", "'What is thy bidding... Master?'"));
				else
					msg_print(_("��NON SERVIAM! Wretch! �����κ���ĺ��������", "'NON SERVIAM! Wretch! I shall feast on thy mortal soul!'"));
			}

			break;
		}

		case ACT_SUMMON_UNDEAD:
		{
			bool pet = one_in_(3);
			int type;
			BIT_FLAGS mode = 0L;

			type = (plev > 47 ? SUMMON_HI_UNDEAD : SUMMON_UNDEAD);

			if (!pet || ((plev > 24) && one_in_(3))) mode |= PM_ALLOW_GROUP;
			if (pet) mode |= PM_FORCE_PET;
			else mode |= (PM_ALLOW_UNIQUE | PM_NO_PET);

			if (summon_specific((pet ? -1 : 0), p_ptr->y, p_ptr->x, ((plev * 3) / 2), type, mode))
			{
				msg_print(_("�䤿���������ʤ��μ���˿᤭�Ϥ᤿����������Խ��򱿤�Ǥ���...",
						"Cold winds begin to blow around you, carrying with them the stench of decay..."));
				if (pet)
				msg_print(_("�Ť��λऻ��Զ������ʤ��˻Ť��뤿���ڤ���ᴤä���",
						"Ancient, long-dead forms arise from the ground to serve you!"));
				else
				msg_print(_("��Ԥ�ᴤä���̲���˸���뤢�ʤ���ȳ���뤿��ˡ�",
						"'The dead arise... to punish you for disturbing them!'"));
			}

			break;
		}

		case ACT_SUMMON_HOUND:
		{
			BIT_FLAGS mode = PM_ALLOW_GROUP;
			bool pet = !one_in_(5);
			if (pet) mode |= PM_FORCE_PET;
			else mode |= PM_NO_PET;

			if (summon_specific((pet ? -1 : 0), p_ptr->y, p_ptr->x, ((p_ptr->lev * 3) / 2), SUMMON_HOUND, mode))
			{

				if (pet)
					msg_print(_("�ϥ���ɤ����ʤ��β��ͤȤ��ƽи�������",
						"A group of hounds appear as your servant."));
				else
					msg_print(_("�ϥ���ɤϤ��ʤ��˲������Ƥ��롪",
						"A group of hounds appear as your enemy!"));
			}

			break;
		}

		case ACT_SUMMON_DAWN:
		{
			msg_print(_("�Ǥλ��Ĥ򾤴�������","You summon the Legion of the Dawn."));
			(void)summon_specific(-1, p_ptr->y, p_ptr->x, dun_level, SUMMON_DAWN, (PM_ALLOW_GROUP | PM_FORCE_PET));
			break;
		}

		case ACT_SUMMON_OCTOPUS:
		{
			BIT_FLAGS mode = PM_ALLOW_GROUP;
			bool pet = !one_in_(5);
			if (pet) mode |= PM_FORCE_PET;

			if (summon_named_creature(0, p_ptr->y, p_ptr->x, MON_JIZOTAKO, mode))
			{
				if (pet)
					msg_print(_("�������ʤ��β��ͤȤ��ƽи�������", "A group of octopuses appear as your servant."));
				else
					msg_print(_("���Ϥ��ʤ����ˤ�Ǥ��롪", "A group of octopuses appear as your enemy!"));
			}

			break;
		}

		/* Activate for healing */

		case ACT_CHOIR_SINGS:
		{
			msg_print(_("ŷ��βΤ�ʹ������...", "A heavenly choir sings..."));
			(void)set_poisoned(0);
			(void)set_cut(0);
			(void)set_stun(0);
			(void)set_confused(0);
			(void)set_blind(0);
			(void)set_afraid(0);
			(void)set_hero(randint1(25) + 25, FALSE);
			(void)hp_player(777);
			break;
		}

		case ACT_CURE_LW:
		{
			(void)set_afraid(0);
			(void)hp_player(30);
			break;
		}

		case ACT_CURE_MW:
		{
			msg_print(_("���翧�θ���ȯ���Ƥ���...", "It radiates deep purple..."));
			hp_player(damroll(4, 8));
			(void)set_cut((p_ptr->cut / 2) - 50);
			break;
		}

		case ACT_CURE_POISON:
		{
			msg_print(_("���Ŀ��˵����Ƥ���...", "It glows deep blue..."));
			(void)set_afraid(0);
			(void)set_poisoned(0);
			break;
		}

		case ACT_REST_EXP:
		{
			msg_print(_("���Ȥ˵����Ƥ���...", "It glows a deep red..."));
			restore_level();
			break;
		}

		case ACT_REST_ALL:
		{
			msg_print(_("ǻ�п��˵����Ƥ���...", "It glows a deep green..."));
			(void)do_res_stat(A_STR);
			(void)do_res_stat(A_INT);
			(void)do_res_stat(A_WIS);
			(void)do_res_stat(A_DEX);
			(void)do_res_stat(A_CON);
			(void)do_res_stat(A_CHR);
			(void)restore_level();
			break;
		}

		case ACT_CURE_700:
		{
			msg_print(_("���Ŀ��˵����Ƥ���...","It glows deep blue..."));
			msg_print(_("������Ȥ�����ư����������...","You feel a warm tingling inside..."));
			(void)hp_player(700);
			(void)set_cut(0);
			break;
		}

		case ACT_CURE_1000:
		{
			msg_print(_("�����뤯�����Ƥ���...","It glows a bright white..."));
			msg_print(_("�Ҥ��礦�˵�ʬ���褤...","You feel much better..."));
			(void)hp_player(1000);
			(void)set_cut(0);
			break;
		}

		case ACT_CURING:
		{
			msg_format(_("%s��ͥ�������������...", "the %s cures you affectionately ..."), name);
			(void)set_poisoned(0);
			(void)set_confused(0);
			(void)set_blind(0);
			(void)set_stun(0);
			(void)set_cut(0);
			(void)set_image(0);

			break;
		}

		case ACT_CURE_MANA_FULL:
		{
			msg_format(_("%s�����򤯸��ä�������","The %s glows pale..."), name);
			if (p_ptr->pclass == CLASS_MAGIC_EATER)
			{
				int i;
				for (i = 0; i < EATER_EXT*2; i++)
				{
					p_ptr->magic_num1[i] += (p_ptr->magic_num2[i] < 10) ? EATER_CHARGE * 3 : p_ptr->magic_num2[i]*EATER_CHARGE/3;
					if (p_ptr->magic_num1[i] > p_ptr->magic_num2[i]*EATER_CHARGE) p_ptr->magic_num1[i] = p_ptr->magic_num2[i]*EATER_CHARGE;
				}
				for (; i < EATER_EXT*3; i++)
				{
					KIND_OBJECT_IDX k_idx = lookup_kind(TV_ROD, i-EATER_EXT*2);
					p_ptr->magic_num1[i] -= ((p_ptr->magic_num2[i] < 10) ? EATER_ROD_CHARGE*3 : p_ptr->magic_num2[i]*EATER_ROD_CHARGE/3)*k_info[k_idx].pval;
					if (p_ptr->magic_num1[i] < 0) p_ptr->magic_num1[i] = 0;
				}
				msg_print(_("Ƭ���ϥå���Ȥ�����", "You feel your head clear."));
				p_ptr->window |= (PW_PLAYER);
			}
			else if (p_ptr->csp < p_ptr->msp)
			{
				p_ptr->csp = p_ptr->msp;
				p_ptr->csp_frac = 0;
				msg_print(_("Ƭ���ϥå���Ȥ�����", "You feel your head clear."));
				p_ptr->redraw |= (PR_MANA);
				p_ptr->window |= (PW_PLAYER);
				p_ptr->window |= (PW_SPELL);
			}
			break;
		}

		/* Activate for timed effect */

		case ACT_ESP:
		{
			(void)set_tim_esp(randint1(30) + 25, FALSE);
			break;
		}

		case ACT_BERSERK:
		{
			(void)set_afraid(0);
			(void)set_shero(randint1(25) + 25, FALSE);
			/* (void)set_afraid(0);
			(void)set_hero(randint1(50) + 50, FALSE);
			(void)set_blessed(randint1(50) + 50, FALSE);
			o_ptr->timeout = 100 + randint1(100); */
			break;
		}

		case ACT_PROT_EVIL:
		{
			msg_format(_("%s����Ԥ�����ή��Ф�...", "The %s lets out a shrill wail..."), name);
			k = 3 * p_ptr->lev;
			(void)set_protevil(randint1(25) + k, FALSE);
			break;
		}

		case ACT_RESIST_ALL:
		{
			msg_print(_("�͡��ʿ��˵����Ƥ���...", "It glows many colours..."));
			(void)set_oppose_acid(randint1(40) + 40, FALSE);
			(void)set_oppose_elec(randint1(40) + 40, FALSE);
			(void)set_oppose_fire(randint1(40) + 40, FALSE);
			(void)set_oppose_cold(randint1(40) + 40, FALSE);
			(void)set_oppose_pois(randint1(40) + 40, FALSE);
			break;
		}

		case ACT_SPEED:
		{
			msg_print(_("���뤯�п��˵����Ƥ���...", "It glows bright green..."));
			(void)set_fast(randint1(20) + 20, FALSE);
			break;
		}

		case ACT_XTRA_SPEED:
		{
			msg_print(_("���뤯�����Ƥ���...", "It glows brightly..."));
			(void)set_fast(randint1(75) + 75, FALSE);
			break;
		}

		case ACT_WRAITH:
		{
			set_wraith_form(randint1(plev / 2) + (plev / 2), FALSE);
			break;
		}

		case ACT_INVULN:
		{
			(void)set_invuln(randint1(8) + 8, FALSE);
			break;
		}

		case ACT_HELO:
		{
			(void)set_afraid(0);
			set_hero(randint1(25)+25, FALSE);
			hp_player(10);
			break;
		}

		case ACT_HELO_SPEED:
		{
			(void)set_fast(randint1(50) + 50, FALSE);
			hp_player(10);
			set_afraid(0);
			set_hero(randint1(50) + 50, FALSE);
			break;
		}

		case ACT_RESIST_ACID:
		{
			msg_format(_("%s������������...", "The %s grows black."), name);
			if ((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_ACID))
			{
				if (!get_aim_dir(&dir)) return FALSE;
				fire_ball(GF_ACID, dir, 100, 2);
			}
			(void)set_oppose_acid(randint1(20) + 20, FALSE);
			break;
		}

		case ACT_RESIST_FIRE:
		{
			msg_format(_("%s���֤�������...","The %s grows red."), name);
			if ((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_FLAMES))
			{
				if (!get_aim_dir(&dir)) return FALSE;
				fire_ball(GF_FIRE, dir, 100, 2);
			}
			(void)set_oppose_fire(randint1(20) + 20, FALSE);
			break;
		}

		case ACT_RESIST_COLD:
		{
			msg_format(_("%s���򤯵�����...","The %s grows white.") , name);
			if ((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_ICE))
			{
				if (!get_aim_dir(&dir)) return FALSE;
				fire_ball(GF_COLD, dir, 100, 2);
			}
			(void)set_oppose_cold(randint1(20) + 20, FALSE);
			break;
		}

		case ACT_RESIST_ELEC:
		{
			msg_format(_("%s���Ĥ�������...", "The %s grows blue."), name);
			if ((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_ELEC))
			{
				if (!get_aim_dir(&dir)) return FALSE;
				fire_ball(GF_ELEC, dir, 100, 2);
			}
			(void)set_oppose_elec(randint1(20) + 20, FALSE);
			break;
		}

		case ACT_RESIST_POIS:
		{
			msg_format(_("%s���Ф˵�����...", "The %s grows green."), name);
			(void)set_oppose_pois(randint1(20) + 20, FALSE);
			break;
		}

		/* Activate for general purpose effect (detection etc.) */

		case ACT_LIGHT:
		{
			msg_format(_("%s����������������դ�Ф�...", "The %s wells with clear light..."), name);
			lite_area(damroll(2, 15), 3);
			break;
		}

		case ACT_MAP_LIGHT:
		{
			msg_print(_("������������...", "It shines brightly..."));
			map_area(DETECT_RAD_MAP);
			lite_area(damroll(2, 15), 3);
			break;
		}

		case ACT_DETECT_ALL:
		{
			msg_print(_("�����뤯�����Ƥ���...", "It glows bright white..."));
			msg_print(_("���˥��᡼�����⤫��Ǥ���...", "An image forms in your mind..."));
			detect_all(DETECT_RAD_DEFAULT);
			break;
		}

		case ACT_DETECT_XTRA:
		{
			msg_print(_("���뤯�����Ƥ���...", "It glows brightly..."));
			detect_all(DETECT_RAD_DEFAULT);
			probing();
			identify_fully(FALSE);
			break;
		}

		case ACT_ID_FULL:
		{
			msg_print(_("�����������Ƥ���...", "It glows yellow..."));
			identify_fully(FALSE);
			break;
		}

		case ACT_ID_PLAIN:
		{
			if (!ident_spell(FALSE)) return FALSE;
			break;
		}

		case ACT_RUNE_EXPLO:
		{
			msg_print(_("���뤤�ֿ��˵����Ƥ���...", "It glows bright red..."));
			explosive_rune();
			break;
		}

		case ACT_RUNE_PROT:
		{
			msg_print(_("�֥롼�����뤯�����Ƥ���...", "It glows light blue..."));
			warding_glyph();
			break;
		}

		case ACT_SATIATE:
		{
			(void)set_food(PY_FOOD_MAX - 1);
			break;
		}

		case ACT_DEST_DOOR:
		{
			msg_print(_("���뤤�ֿ��˵����Ƥ���...", "It glows bright red..."));
			destroy_doors_touch();
			break;
		}

		case ACT_STONE_MUD:
		{
			msg_print(_("��ư���Ƥ���...", "It pulsates..."));
			if (!get_aim_dir(&dir)) return FALSE;
			wall_to_mud(dir, 20 + randint1(30));
			break;
		}

		case ACT_RECHARGE:
		{
			recharge(130);
			break;
		}

		case ACT_ALCHEMY:
		{
			msg_print(_("���뤤�����˵����Ƥ���...", "It glows bright yellow..."));
			(void)alchemy();
			break;
		}

		case ACT_DIM_DOOR:
		{
			msg_print(_("�������⤬����������Ū�Ϥ�����ǲ�������", "You open a dimensional gate. Choose a destination."));
			if (!dimension_door()) return FALSE;
			break;
		}


		case ACT_TELEPORT:
		{
			msg_print(_("����ζ��֤��Ĥ�Ǥ���...", "It twists space around you..."));
			teleport_player(100, 0L);
			break;
		}

		case ACT_RECALL:
		{
			msg_print(_("���餫���򿧤˵����Ƥ���...", "It glows soft white..."));
			if (!word_of_recall()) return FALSE;
			break;
		}

		case ACT_JUDGE:
		{
			msg_format(_("%s���֤����뤯���ä���", "The %s flashes bright red!"), name);
			chg_virtue(V_KNOWLEDGE, 1);
			chg_virtue(V_ENLIGHTEN, 1);
			wiz_lite(FALSE);
			
			msg_format(_("%s�Ϥ��ʤ������Ϥ�å�ä�...", "The %s drains your vitality..."), name);
			take_hit(DAMAGE_LOSELIFE, damroll(3,8), _("��Ƚ������", "the Jewel of Judgement"), -1);
			
			(void)detect_traps(DETECT_RAD_DEFAULT, TRUE);
			(void)detect_doors(DETECT_RAD_DEFAULT);
			(void)detect_stairs(DETECT_RAD_DEFAULT);
			
			if (get_check(_("���Ԥ��Ϥ�Ȥ��ޤ�����", "Activate recall? ")))
			{
				(void)word_of_recall();
			}

			break;
		}

		case ACT_TELEKINESIS:
		{
			if (!get_aim_dir(&dir)) return FALSE;
			msg_format(_("%s�򿭤Ф�����", "You stretched your %s."), name);
			fetch(dir, 500, TRUE);
			break;
		}

		case ACT_DETECT_UNIQUE:
		{
			int i;
			monster_type *m_ptr;
			monster_race *r_ptr;
			msg_print(_("��̯�ʾ�꤬Ƭ������⤫���������", "Some strange places show up in your mind. And you see ..."));
			/* Process the monsters (backwards) */
			for (i = m_max - 1; i >= 1; i--)
			{
				/* Access the monster */
				m_ptr = &m_list[i];

				/* Ignore "dead" monsters */
				if (!m_ptr->r_idx) continue;

				r_ptr = &r_info[m_ptr->r_idx];

				if(r_ptr->flags1 & RF1_UNIQUE)
				{
					msg_format(_("%s�� ", "%s. "),r_name + r_ptr->name);
				}
			}
			break;
		}

		case ACT_ESCAPE:
		{
			switch (randint1(13))
			{
			case 1: case 2: case 3: case 4: case 5:
				teleport_player(10, 0L);
				break;
			case 6: case 7: case 8: case 9: case 10:
				teleport_player(222, 0L);
				break;
			case 11: case 12:
				(void)stair_creation();
				break;
			default:
				if (get_check(_("���γ�����ޤ�����", "Leave this level? ")))
				{
					if (autosave_l) do_cmd_save_game(TRUE);

					/* Leaving */
					p_ptr->leaving = TRUE;
				}
			}
			break;
		}

		case ACT_DISP_CURSE_XTRA:
		{
			msg_format(_("%s�����¤�Ȥ餷�Ф�...", "The %s exhibits the truth..."), name);
			if (remove_all_curse())
			{
				msg_print(_("ï���˸�����Ƥ���褦�ʵ������롣", "You feel as if someone is watching over you."));
			}
			(void)probing();
			break;
		}

		case ACT_BRAND_FIRE_BOLTS:
		{
			msg_format(_("%s�����Ȥ˵�����...", "Your %s glows deep red..."), name);
			(void)brand_bolts();
			break;
		}

		case ACT_RECHARGE_XTRA:
		{
			msg_format(_("%s���򤯵�����������", "The %s gleams with blinding light..."), name);
			if (!recharge(1000)) return FALSE;
			break;
		}

		case ACT_LORE:
		{
			msg_print(_("�Ф������줿��̩��̤��Ф���������", "The stone reveals hidden mysteries..."));
			if (!ident_spell(FALSE)) return FALSE;

			if (mp_ptr->spell_book)
			{
				/* Sufficient mana */
				if (20 <= p_ptr->csp)
				{
					/* Use some mana */
					p_ptr->csp -= 20;
				}

				/* Over-exert the player */
				else
				{
					int oops = 20 - p_ptr->csp;

					/* No mana left */
					p_ptr->csp = 0;
					p_ptr->csp_frac = 0;

					/* Message */
					msg_print(_("�Ф�����Ǥ��ʤ���", "You are too weak to control the stone!"));
					/* Hack -- Bypass free action */
					(void)set_paralyzed(p_ptr->paralyzed +
						randint1(5 * oops + 1));

					/* Confusing. */
					(void)set_confused(p_ptr->confused +
						randint1(5 * oops + 1));
				}

				/* Redraw mana */
				p_ptr->redraw |= (PR_MANA);
			}
			take_hit(DAMAGE_LOSELIFE, damroll(1, 12), _("������̩", "perilous secrets"), -1);
			/* Confusing. */
			if (one_in_(5)) (void)set_confused(p_ptr->confused +
				randint1(10));

			/* Exercise a little care... */
			if (one_in_(20))
				take_hit(DAMAGE_LOSELIFE, damroll(4, 10), _("������̩", "perilous secrets"), -1);
			break;
		}

		case ACT_SHIKOFUMI:
		{
			msg_print(_("�϶����͸Ԥ�Ƨ�����", "You stamp. (as if you are in a ring.)"));
			(void)set_afraid(0);
			(void)set_hero(randint1(20) + 20, FALSE);
			dispel_evil(p_ptr->lev * 3);
			break;
		}

		case ACT_PHASE_DOOR:
		{
			teleport_player(10, 0L);
			break;
		}

		case ACT_DETECT_ALL_MONS:
		{
			(void)detect_monsters_invis(255);
			(void)detect_monsters_normal(255);
			break;
		}

		case ACT_ULTIMATE_RESIST:
		{
			TIME_EFFECT v = randint1(25)+25;
			(void)set_afraid(0);
			(void)set_hero(v, FALSE);
			(void)hp_player(10);
			(void)set_blessed(v, FALSE);
			(void)set_oppose_acid(v, FALSE);
			(void)set_oppose_elec(v, FALSE);
			(void)set_oppose_fire(v, FALSE);
			(void)set_oppose_cold(v, FALSE);
			(void)set_oppose_pois(v, FALSE);
			(void)set_ultimate_res(v, FALSE);
			break;
		}


		/* Unique activation */
		case ACT_CAST_OFF:
		{
			int inv, t;
			OBJECT_IDX o_idx;
			char o_name[MAX_NLEN];
			object_type forge;

			/* Cast off activated item */
			for (inv = INVEN_RARM; inv <= INVEN_FEET; inv++)
			{
				if (o_ptr == &inventory[inv]) break;
			}

			/* Paranoia */
			if (inv > INVEN_FEET) return FALSE;

			object_copy(&forge, o_ptr);
			inven_item_increase(inv, (0 - o_ptr->number));
			inven_item_optimize(inv);
			o_idx = drop_near(&forge, 0, p_ptr->y, p_ptr->x);
			o_ptr = &o_list[o_idx];

			object_desc(o_name, o_ptr, OD_NAME_ONLY);
			msg_format(_("%s��æ���ΤƤ���", "You cast off %s."), o_name);

			/* Get effects */
			msg_print(_("��ǳ���夬�첶�ξ����衪��", "You say, 'Burn up my cosmo!"));
			t = 20 + randint1(20);
			(void)set_blind(p_ptr->blind + t);
			(void)set_afraid(0);
			(void)set_tim_esp(p_ptr->tim_esp + t, FALSE);
			(void)set_tim_regen(p_ptr->tim_regen + t, FALSE);
			(void)set_hero(p_ptr->hero + t, FALSE);
			(void)set_blessed(p_ptr->blessed + t, FALSE);
			(void)set_fast(p_ptr->fast + t, FALSE);
			(void)set_shero(p_ptr->shero + t, FALSE);
			if (p_ptr->pclass == CLASS_FORCETRAINER)
			{
				P_PTR_KI = plev * 5 + 190;
				msg_print(_("������ȯ�����ˤʤä���", "Your force are immediatly before explosion."));
			}

			break;
		}

		case ACT_FALLING_STAR:
		{
			msg_print(_("���ʤ��������̥����줿��", "You are enchanted by cursed blade..."));
			msg_print(_("�ֶ��ۤ��� ��Τ��Ȥ� ��ϤΤܤ�� ��ᤪ���� ��� �����������", "'Behold the blade arts.'"));
			massacre();
			break;
		}

		case ACT_GRAND_CROSS:
		{
			msg_print(_("�ְǤ˴Ԥ졪��", "You say, 'Return to darkness!'"));
			project(0, 8, p_ptr->y, p_ptr->x, (randint1(100) + 200) * 2, GF_HOLY_FIRE, PROJECT_KILL | PROJECT_ITEM | PROJECT_GRID, -1);
			break;
		}

		case ACT_TELEPORT_LEVEL:
		{
			if (!get_check(_("������¾�γ��˥ƥ�ݡ��Ȥ��ޤ�����", "Are you sure? (Teleport Level)"))) return FALSE;
			teleport_level(0);
			break;
		}

		case ACT_STRAIN_HASTE:
		{
			int t;
			msg_format(_("%s�Ϥ��ʤ������Ϥ�å�ä�...", "The %s drains your vitality..."), name);
			take_hit(DAMAGE_LOSELIFE, damroll(3, 8), _("��®������ϫ", "the strain of haste"), -1);
			t = 25 + randint1(25);
			(void)set_fast(p_ptr->fast + t, FALSE);
			break;
		}

		case ACT_FISHING:
		{
			int x, y;

			if (!get_rep_dir2(&dir)) return FALSE;
			y = p_ptr->y+ddy[dir];
			x = p_ptr->x+ddx[dir];
			tsuri_dir = dir;
			if (!cave_have_flag_bold(y, x, FF_WATER))
			{
				msg_print(_("�����Ͽ��դǤϤʤ���", "There is no fishing place."));
				return FALSE;
			}
			else if (cave[y][x].m_idx)
			{
				char m_name[80];
				monster_desc(m_name, &m_list[cave[y][x].m_idx], 0);
				msg_format(_("%s���������", "%^s is stand in your way."), m_name);
				p_ptr->energy_use = 0;
				return FALSE;
			}
			set_action(ACTION_FISH);
			p_ptr->redraw |= (PR_STATE);
			break;
		}

		case ACT_INROU:
		{
			int count = 0, i;
			monster_type *m_ptr;
			cptr kakusan = "";
			
			if (summon_named_creature(0, p_ptr->y, p_ptr->x, MON_SUKE, PM_FORCE_PET))
			{
				msg_print(_("�ؽ�����٤����줿��", "Suke-san apperars."));
				kakusan = "Suke-san";
				count++;
			}
			if (summon_named_creature(0, p_ptr->y, p_ptr->x, MON_KAKU, PM_FORCE_PET))
			{
				msg_print(_("�سʤ���٤����줿��", "Kaku-san appears."));
				kakusan = "Kaku-san";
				count++;
			}
			if (!count)
			{
				for (i = m_max - 1; i > 0; i--)
				{
					m_ptr = &m_list[i];
					if (!m_ptr->r_idx) continue;
					if (!((m_ptr->r_idx == MON_SUKE) || (m_ptr->r_idx == MON_KAKU))) continue;
					if (!los(m_ptr->fy, m_ptr->fx, p_ptr->y, p_ptr->x)) continue;
					if (!projectable(m_ptr->fy, m_ptr->fx, p_ptr->y, p_ptr->x)) continue;
					count++;
					break;
				}
			}

			if (count)
			{
				msg_format(_("�ּԤɤ⡢�Ҥ����������������Τ�����ɤʤ��Ȥ������롣��", 
							"%^s says 'WHO do you think this person is! Bow your head, down your knees!'"), kakusan);
				sukekaku = TRUE;
				stun_monsters(120);
				confuse_monsters(120);
				turn_monsters(120);
				stasis_monsters(120);
				sukekaku = FALSE;
			}
			else
			{
				msg_print(_("�����������ⵯ���ʤ��ä���", "Nothing happen."));
			}
			break;
		}

		case ACT_MURAMASA:
		{
			/* Only for Muramasa */
			if (o_ptr->name1 != ART_MURAMASA) return FALSE;
			if (get_check(_("�����˻Ȥ��ޤ�����", "Are you sure?!")))
			{
				msg_print(_("¼�����̤���������", "The Muramasa pulsates..."));
				do_inc_stat(A_STR);
				if (one_in_(2))
				{
					msg_print(_("¼���ϲ��줿��", "The Muramasa is destroyed!"));
					curse_weapon_object(TRUE, o_ptr);
				}
			}
			break;
		}

		case ACT_BLOODY_MOON:
		{
			/* Only for Bloody Moon */
			if (o_ptr->name1 != ART_BLOOD) return FALSE;
			msg_print(_("�������뤯������...", "Your scythe glows brightly!"));
			get_bloody_moon_flags(o_ptr);
			if (p_ptr->prace == RACE_ANDROID) calc_android_exp();
			p_ptr->update |= (PU_BONUS | PU_HP);
			break;
		}

		case ACT_CRIMSON:
		{
			int num = 1;
			int i;
			BIT_FLAGS flg = PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL;
			int tx, ty;

			/* Only for Crimson */
			if (o_ptr->name1 != ART_CRIMSON) return FALSE;

			msg_print(_("���ä���������إ���ॾ��٤�֤äѤʤ�����", "I'll fire CRIMSON! SEKKAKUDAKARA!"));

			if (!get_aim_dir(&dir)) return FALSE;

			/* Use the given direction */
			tx = p_ptr->x + 99 * ddx[dir];
			ty = p_ptr->y + 99 * ddy[dir];

			/* Hack -- Use an actual "target" */
			if ((dir == 5) && target_okay())
			{
				tx = target_col;
				ty = target_row;
			}

			if (p_ptr->pclass == CLASS_ARCHER)
			{
				/* Extra shot at level 10 */
				if (p_ptr->lev >= 10) num++;

				/* Extra shot at level 30 */
				if (p_ptr->lev >= 30) num++;

				/* Extra shot at level 45 */
				if (p_ptr->lev >= 45) num++;
			}

			for (i = 0; i < num; i++)
				project(0, p_ptr->lev/20+1, ty, tx, p_ptr->lev*p_ptr->lev*6/50, GF_ROCKET, flg, -1);
			break;
		}

		default:
		{
			msg_format(_("Unknown activation effect: %d.", "Unknown activation effect: %d."), act_ptr->index);
			return FALSE;
		}
	}

	/* Set activation timeout */
	if (act_ptr->timeout.constant >= 0) {
		o_ptr->timeout = (s16b)act_ptr->timeout.constant;
		if (act_ptr->timeout.dice > 0) {
			o_ptr->timeout += randint1(act_ptr->timeout.dice);
		}
	} else {
		/* Activations that have special timeout */
		switch (act_ptr->index) {
		case ACT_BR_FIRE:
			o_ptr->timeout = ((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_FLAMES)) ? 200 : 250;
			break;
		case ACT_BR_COLD:
			o_ptr->timeout = ((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_ICE)) ? 200 : 250;
			break;
		case ACT_TERROR:
			o_ptr->timeout = 3 * (p_ptr->lev + 10);
			break;
		case ACT_MURAMASA:
			/* Nothing to do */
			break;
		default:
			msg_format("Special timeout is not implemented: %d.", act_ptr->index);
			return FALSE;
		}
	}

	return TRUE;
}

/*!
 * @brief ���ꥢ���ƥ��ե����ȡإ֥�åǥ��ࡼ��٤��������ѹ����롣
 * @details ���쥤2d2��ڤ�one_resistance()�ˤ������1d2�pval2������롣
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Ρʥ֥�åǥ��ࡼ��ˤΥݥ���
 * @return �ʤ�
 */
void get_bloody_moon_flags(object_type *o_ptr)
{
	int dummy, i;

	for (i = 0; i < TR_FLAG_SIZE; i++)
		o_ptr->art_flags[i] = a_info[ART_BLOOD].flags[i];

	dummy = randint1(2) + randint1(2);
	for (i = 0; i < dummy; i++)
	{
		int flag = randint0(26);
		if (flag >= 20) add_flag(o_ptr->art_flags, TR_KILL_UNDEAD + flag - 20);
		else if (flag == 19) add_flag(o_ptr->art_flags, TR_KILL_ANIMAL);
		else if (flag == 18) add_flag(o_ptr->art_flags, TR_SLAY_HUMAN);
		else add_flag(o_ptr->art_flags, TR_CHAOTIC + flag);
	}

	dummy = randint1(2);
	for (i = 0; i < dummy; i++) one_resistance(o_ptr);

	for (i = 0; i < 2; i++)
	{
		int tmp = randint0(11);
		if (tmp < 6) add_flag(o_ptr->art_flags, TR_STR + tmp);
		else add_flag(o_ptr->art_flags, TR_STEALTH + tmp - 6);
	}
}

/*!
 * @brief ���ꥢ���ƥ��ե����������������̤ʥϡ��ɥ����ǥ��󥰽�����Ԥ���.
 * @details random_artifact_resistance()�Ȥ��뤬�ºݤϸ��ꥢ���ƥ��ե����ȤǤ��롣
 * �оݤ϶��ݤβ��̡�¼������ӥ�ȥ�Υϡ��ס�ζ���򬡢�֥�åǥ��ࡼ�󡢱��ᡢŷ���α��ᡢ�ߥ�ࡢ
 * ����¾�ɲ������������ɲý�����
 * @attention �ץ쥤�䡼�γƼ凉�ơ������˰�¸�������������롣
 * @todo �ޤ򸫤ƴؿ�̾���ѹ����뤳�ȡ�
 * @param o_ptr �оݤΥ��֥������ȹ�¤�Υݥ���
 * @param a_ptr ����������ꥢ���ƥ��ե����ȹ�¤�Υݥ���
 * @return �ʤ�
 */
void random_artifact_resistance(object_type * o_ptr, artifact_type *a_ptr)
{
	bool give_resistance = FALSE, give_power = FALSE;

	if (o_ptr->name1 == ART_TERROR) /* Terror Mask is for warriors... */
	{
		if (p_ptr->pclass == CLASS_WARRIOR || p_ptr->pclass == CLASS_ARCHER || p_ptr->pclass == CLASS_CAVALRY || p_ptr->pclass == CLASS_BERSERKER)
		{
			give_power = TRUE;
			give_resistance = TRUE;
		}
		else
		{
			add_flag(o_ptr->art_flags, TR_AGGRAVATE);
			add_flag(o_ptr->art_flags, TR_TY_CURSE);
			o_ptr->curse_flags |=
			    (TRC_CURSED | TRC_HEAVY_CURSE);
			o_ptr->curse_flags |= get_curse(2, o_ptr);
			return;
		}
	}

	if (o_ptr->name1 == ART_MURAMASA)
	{
		if (p_ptr->pclass != CLASS_SAMURAI)
		{
			add_flag(o_ptr->art_flags, TR_NO_MAGIC);
			o_ptr->curse_flags |= (TRC_HEAVY_CURSE);
		}
	}

	if (o_ptr->name1 == ART_ROBINTON)
	{
		if (p_ptr->pclass == CLASS_BARD)
		{
			add_flag(o_ptr->art_flags, TR_DEC_MANA);
		}
	}

	if (o_ptr->name1 == ART_XIAOLONG)
	{
		if (p_ptr->pclass == CLASS_MONK)
			add_flag(o_ptr->art_flags, TR_BLOWS);
	}

	if (o_ptr->name1 == ART_BLOOD)
	{
		get_bloody_moon_flags(o_ptr);
	}

	if (o_ptr->name1 == ART_HEAVENLY_MAIDEN)
	{
		if (p_ptr->psex != SEX_FEMALE)
		{
			add_flag(o_ptr->art_flags, TR_AGGRAVATE);
		}
	}

	if (o_ptr->name1 == ART_MILIM)
	{
		if (p_ptr->pseikaku == SEIKAKU_SEXY)
		{
			o_ptr->pval = 3;
			add_flag(o_ptr->art_flags, TR_STR);
			add_flag(o_ptr->art_flags, TR_INT);
			add_flag(o_ptr->art_flags, TR_WIS);
			add_flag(o_ptr->art_flags, TR_DEX);
			add_flag(o_ptr->art_flags, TR_CON);
			add_flag(o_ptr->art_flags, TR_CHR);
		}
	}

	if (a_ptr->gen_flags & (TRG_XTRA_POWER)) give_power = TRUE;
	if (a_ptr->gen_flags & (TRG_XTRA_H_RES)) give_resistance = TRUE;
	if (a_ptr->gen_flags & (TRG_XTRA_RES_OR_POWER))
	{
		/* Give a resistance OR a power */
		if (one_in_(2)) give_resistance = TRUE;
		else give_power = TRUE;
	}

	if (give_power)
	{
		one_ability(o_ptr);
	}

	if (give_resistance)
	{
		one_high_resistance(o_ptr);
	}
}


/*!
 * @brief �ե��λ��ꤵ�줿���֤˸��ꥢ���ƥ��ե����Ȥ��������롣 / Create the artifact of the specified number
 * @details ���ꥢ���ƥ��ե����ȹ�¤�Τ�����ܥ��ơ������򥳥ԡ������塢����κ�ɸ��drop_item()����Ȥ���
 * @param a_idx ����������ꥢ���ƥ��ե����ȹ�¤�Τ�ID
 * @param y �����ƥ����Ȥ�������y��ɸ
 * @param x �����ƥ����Ȥ�������x��ɸ
 * @return �����������������ݤ������Ԥ�ID���������١��������ƥ��������drop_item()�μ��Ի��˵����롣
 * @attention ���ν�����drop_near()������̤θ��ꥢ���ƥ��ե����Ȥ��Ťʤ�ʤ������˰�¸����.
 * ����2�İʾ�¸�߲�ǽ���������ʰʳ��θ��ꥢ���ƥ��ե����Ȥ�����������
 * drop_near()�ؿ����֤��ͤϿ��ѤǤ��ʤ��ʤ�.
 */
bool create_named_art(int a_idx, int y, int x)
{
	object_type forge;
	object_type *q_ptr;
	IDX i;

	artifact_type *a_ptr = &a_info[a_idx];

	/* Get local object */
	q_ptr = &forge;

	/* Ignore "empty" artifacts */
	if (!a_ptr->name) return FALSE;

	/* Acquire the "kind" index */
	i = lookup_kind(a_ptr->tval, a_ptr->sval);

	/* Oops */
	if (!i) return FALSE;

	/* Create the artifact */
	object_prep(q_ptr, i);

	/* Save the name */
	q_ptr->name1 = (byte_hack)a_idx;

	/* Extract the fields */
	q_ptr->pval = a_ptr->pval;
	q_ptr->ac = a_ptr->ac;
	q_ptr->dd = a_ptr->dd;
	q_ptr->ds = a_ptr->ds;
	q_ptr->to_a = a_ptr->to_a;
	q_ptr->to_h = a_ptr->to_h;
	q_ptr->to_d = a_ptr->to_d;
	q_ptr->weight = a_ptr->weight;

	/* Hack -- extract the "cursed" flag */
	if (a_ptr->gen_flags & TRG_CURSED) q_ptr->curse_flags |= (TRC_CURSED);
	if (a_ptr->gen_flags & TRG_HEAVY_CURSE) q_ptr->curse_flags |= (TRC_HEAVY_CURSE);
	if (a_ptr->gen_flags & TRG_PERMA_CURSE) q_ptr->curse_flags |= (TRC_PERMA_CURSE);
	if (a_ptr->gen_flags & (TRG_RANDOM_CURSE0)) q_ptr->curse_flags |= get_curse(0, q_ptr);
	if (a_ptr->gen_flags & (TRG_RANDOM_CURSE1)) q_ptr->curse_flags |= get_curse(1, q_ptr);
	if (a_ptr->gen_flags & (TRG_RANDOM_CURSE2)) q_ptr->curse_flags |= get_curse(2, q_ptr);

	random_artifact_resistance(q_ptr, a_ptr);

	/* Drop the artifact from heaven */
	return drop_near(q_ptr, -1, y, x) ? TRUE : FALSE;
}
/*�м�ʿ�ѥ��᡼���η׻�����*/
int calc_arm_avgdamage(object_type *o_ptr)
{
	u32b flgs[TR_FLAG_SIZE];
	object_flags(o_ptr, flgs);

	HIT_POINT dam, base, s_evil, forced, vorpal;
	dam = base = s_evil = forced = vorpal = 0;

	dam = base = (o_ptr->dd * o_ptr->ds + o_ptr->dd) / 2;

	if(have_flag(flgs, TR_KILL_EVIL))
	{
		dam = s_evil = dam * 7 / 2;
	}
	else if(!have_flag(flgs, TR_KILL_EVIL) && have_flag(flgs, TR_SLAY_EVIL))
	{	
		dam = s_evil = dam * 2;
	}
	else s_evil = dam;

	if (have_flag(flgs, TR_FORCE_WEAPON))
	{
		dam = forced = dam * 3 / 2 + (o_ptr->dd * o_ptr->ds + o_ptr->dd);
	}
	else forced = dam;

	if(have_flag(flgs, TR_VORPAL))
	{
		dam = vorpal = dam * 11 / 9;
	}
	else vorpal = dam;

	dam = dam + o_ptr->to_d;

	msg_format_wizard(CHEAT_OBJECT,"��:%d> �м�:%d> ����:%d> ��:%d> �ǽ�:%d",
		base, s_evil, forced, vorpal, dam);

	return(dam);
}

static int suppression_evil_dam(object_type *o_ptr)
{
	int num = FALSE;
	u32b flgs[TR_FLAG_SIZE];
	object_flags(o_ptr, flgs);

	if (have_flag(flgs, TR_VAMPIRIC))
	{
		if(have_flag(flgs, TR_BLOWS) && (o_ptr->pval == 1) && (calc_arm_avgdamage(o_ptr) > 52))
		{
			num = TRUE;
		}
		else if(have_flag(flgs, TR_BLOWS) && (o_ptr->pval == 2) && (calc_arm_avgdamage(o_ptr) > 43))
		{
			num = TRUE;
		}
		else if( have_flag(flgs, TR_BLOWS) && (o_ptr->pval == 3) && (calc_arm_avgdamage(o_ptr) > 33))
		{
			num = TRUE;
		}
		else if (calc_arm_avgdamage(o_ptr) > 63)
		{
			num = TRUE;
		}
	}
	else
	{
		if (have_flag(flgs, TR_BLOWS) && (o_ptr->pval == 1) && (calc_arm_avgdamage(o_ptr) > 65))
		{
			num = TRUE;
		}
		else if (have_flag(flgs, TR_BLOWS) && (o_ptr->pval == 2) && (calc_arm_avgdamage(o_ptr) > 52))
		{
			num = TRUE;
		}
		else if (have_flag(flgs, TR_BLOWS) && (o_ptr->pval == 3) && (calc_arm_avgdamage(o_ptr) > 40))
		{
			num = TRUE;
		}
		else if (calc_arm_avgdamage(o_ptr) > 75)
		{
			num = TRUE;
		}
	}
	return(num);
}

static int weakening_artifact(object_type *o_ptr)
{
	 KIND_OBJECT_IDX k_idx = lookup_kind(o_ptr->sval, o_ptr->tval);
	 object_kind *k_ptr = &k_info[k_idx];

	 if ((k_ptr->dd < o_ptr->dd) || (k_ptr->ds < o_ptr->ds))
	 {
		int pre_dd = o_ptr->dd;
		int pre_ds = o_ptr->ds;

		if (o_ptr->dd > o_ptr->ds)
		{
			o_ptr->dd--;
		}
		else
		{
			o_ptr->ds--;
		}

		msg_format_wizard(CHEAT_OBJECT, 
			_("����������������ޤ�����%dd%d -> %dd%d", "Dice Supress %dd%d -> %dd%d"),
			pre_dd, pre_ds, o_ptr->dd, o_ptr->ds);
		return 1;
	}
	
	if (o_ptr->to_d > 10)
	{
		int pre_damage = o_ptr->to_d;

		o_ptr->to_d = o_ptr->to_d - damroll(1, 6);
		if (o_ptr->to_d < 10)
		{
			o_ptr->to_d = 10;
		}

		msg_format_wizard(CHEAT_OBJECT,
			_("���᡼����������������ޤ����� %d -> %d", "Plus-Damage Supress %d -> %d"),
			pre_damage, o_ptr->to_d);

		return 1;
	 }
	 return 0;
}