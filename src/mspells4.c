/*!
 * @file mspells4.c
 * @brief ���ڥ�¹Խ��� / Spell launch
 * @date 2014/07/14
 * @author Habu
 */


#include "angband.h"

/*!
* @brief ��󥹥���ID���ꡢ��󥹥���̾��m_name���������� /
* @param m_idx ��󥹥���ID
* @param m_name ��󥹥���̾�����Ϥ�������
*/
void monster_name(MONSTER_IDX m_idx, char* m_name)
{
	monster_type	*m_ptr = &m_list[m_idx];
	monster_desc(m_name, m_ptr, 0x00);
}

/*!
* @brief ��󥹥���2�Τ��ץ쥤�䡼�ζ᤯�˵�뤫��Ƚ�� /
* @param m_idx ��󥹥���ID������
* @param t_idx ��󥹥���ID������
* @return ��󥹥���2�ΤΤɤ��餫���ץ쥤�䡼�ζ᤯�˵���TRUE���ɤ����󤱤��FALSE���֤���
*/
bool monster_near_player(MONSTER_IDX m_idx, IDX t_idx)
{
	monster_type	*m_ptr = &m_list[m_idx];
	monster_type	*t_ptr = &m_list[t_idx];
	return (m_ptr->cdis <= MAX_SIGHT) || (t_ptr->cdis <= MAX_SIGHT);
}

/*!
* @brief �ץ쥤�䡼����󥹥����򸫤뤳�Ȥ��Ǥ��뤫��Ƚ�� /
* @param m_idx ��󥹥���ID
* @return �ץ쥤�䡼����󥹥����򸫤뤳�Ȥ��Ǥ���ʤ�TRUE�������Ǥʤ����FALSE���֤���
*/
bool see_monster(MONSTER_IDX m_idx)
{
	monster_type	*m_ptr = &m_list[m_idx];
	return is_seen(m_ptr);
}

/*!
* @brief ��󥹥����ξ�������ʸ������ˡ�ǳؽ��Ǥ��뤫Ƚ�ꤹ�� /
* @param m_idx ��󥹥���ID
* @return �ץ쥤�䡼������ˡ�ǳؽ��Ǥ���ʤ�TRUE�������Ǥʤ����FALSE���֤���
*/
bool spell_learnable(MONSTER_IDX m_idx)
{
	monster_type	*m_ptr = &m_list[m_idx];
	/* Extract the "see-able-ness" */
	bool seen = (!p_ptr->blind && m_ptr->ml);

	bool maneable = player_has_los_bold(m_ptr->fy, m_ptr->fx);
	return (seen && maneable && !world_monster);
}

/*!
* @brief ��󥹥���ID�����󥹥����Υ�٥��������� /
* @param m_idx ��󥹥���ID
* @return ��󥹥����Υ�٥�
*/
int monster_level_idx(MONSTER_IDX m_idx)
{
	monster_type	*m_ptr = &m_list[m_idx];
	monster_race	*r_ptr = &r_info[m_ptr->r_idx];
	int rlev = ((r_ptr->level >= 1) ? r_ptr->level : 1);
	return rlev;
}

/*!
* @brief ��󥹥���ID����POWERFUL�ե饰��̵ͭ��������� /
* @param m_idx ��󥹥���ID
* @return POWERFUL�ե饰�������TRUE���ʤ����FALSE���֤���
*/
bool monster_is_powerful(MONSTER_IDX m_idx)
{
	monster_type	*m_ptr = &m_list[m_idx];
	monster_race	*r_ptr = &r_info[m_ptr->r_idx];
	bool powerful = r_ptr->flags2 & RF2_POWERFUL ? TRUE : FALSE;
	return powerful;
}

/*!
* @brief ��󥹥�������ˡ���������ǽ�Ǥ��뤫��Ƚ�ꤹ�� /
* @param m_idx ��󥹥���ID
* @return ������ǽ�Ǥ����PM_ALLOW_UNIQUE���֤���
*/
u32b monster_u_mode(MONSTER_IDX m_idx)
{
	u32b u_mode = 0L;
	monster_type	*m_ptr = &m_list[m_idx];
	bool pet = is_pet(m_ptr);
	if (!pet) u_mode |= PM_ALLOW_UNIQUE;
	return u_mode;
}

/*!
* @brief ��󥹥����򵯤��� /
* @param t_idx ��󥹥���ID
*/
void monster_wakeup(IDX t_idx)
{
	(void)set_monster_csleep(t_idx, 0);
}

/*!
 * @brief ��󥹥�������ʸ�ԻȤ���ݤΥ�å�����������������Ѵؿ� /
 * @param m_idx ��ʸ�򾧤����󥹥���ID
 * @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
 * @param msg1 msg_flag��TRUE�ǡ��ץ쥤�䡼���оݤȤ�����Υ�å�����
 * @param msg2 msg_flag��TRUE�ǡ���󥹥������оݤȤ�����Υ�å�����
 * @param msg3 msg_flag��FALSE�ǡ��ץ쥤�䡼���оݤȤ�����Υ�å�����
 * @param msg4 msg_flag��FALSE�ǡ���󥹥������оݤȤ�����Υ�å�����
 * @param msg_flag_aux ��å�������ʬ�����뤿��Υե饰
 * @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
 */
static void monspell_message_base(MONSTER_IDX m_idx, IDX t_idx, cptr msg1, cptr msg2, cptr msg3, cptr msg4, bool msg_flag_aux, int TARGET_TYPE)
{
	bool known = monster_near_player(m_idx, t_idx);
	bool see_either = see_monster(m_idx) || see_monster(t_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	char m_name[80], t_name[80];
	monster_name(m_idx, m_name);
	monster_name(t_idx, t_name);

	if (mon_to_player || (mon_to_mon && known && see_either))
		disturb(1, 1);

	if (msg_flag_aux)
	{
		if (mon_to_player)
			msg_format(msg1, m_name);
		else if (mon_to_mon && known && see_either)
			msg_format(msg2, m_name);
	}
	else
	{
		if (mon_to_player)
		{
			msg_format(msg3, m_name);
		}
		else if (mon_to_mon && known && see_either)
		{
			msg_format(msg4, m_name, t_name);
		}
	}

	if (mon_to_mon && known && !see_either)
		mon_fight = TRUE;
}

/*!
* @brief ��󥹥�������ʸ�ԻȤ���ݤΥ�å�����������������Ѵؿ������ܻ����̾���Υ�å��������ڤ��ؤ��롣 /
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param msg1 �ץ쥤�䡼�����ܾ��֤Υ�å�����
* @param msg2 �ץ쥤�䡼�����ܤǤʤ����ץ쥤�䡼���оݤȤ�����Υ�å�����
* @param msg3 �ץ쥤�䡼�����ܤǤʤ�����󥹥����оݤȤ�����Υ�å�����
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
*/
void monspell_message(MONSTER_IDX m_idx, IDX t_idx, cptr msg1, cptr msg2, cptr msg3, int TARGET_TYPE)
{
	monspell_message_base(m_idx, t_idx, msg1, msg1, msg2, msg3, p_ptr->blind > 0, TARGET_TYPE);
}

/*!
* @brief ��󥹥�������ʸ�ԻȤ���ݤΥ�å�����������������Ѵؿ����Х�󥹥������Хץ쥤�䡼�Υ�å��������ڤ��ؤ��롣 /
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param msg1 �ץ쥤�䡼���оݤȤ�����Υ�å�����
* @param msg2 ��󥹥����оݤȤ�����Υ�å�����
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
*/
void simple_monspell_message(MONSTER_IDX m_idx, IDX t_idx, cptr msg1, cptr msg2, int TARGET_TYPE)
{
	monspell_message_base(m_idx, t_idx, msg1, msg2, msg1, msg2, p_ptr->blind > 0, TARGET_TYPE);
}

/*!
 * @brief RF4_SHRIEK�ν��������ӡ� /
 * @param m_idx ��ʸ�򾧤����󥹥���ID
 * @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
 * @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
 */
void spell_RF4_SHRIEK(MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	simple_monspell_message(m_idx, t_idx,
		_("%^s������⤤���ڤ����򤢤�����", "%^s makes a high pitched shriek."),
		_("%^s��%s�˸����äƶ������", "%^s shrieks at %s."),
		TARGET_TYPE);

	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		aggravate_monsters(m_idx);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER)
	{
		monster_wakeup(t_idx);
	}
}

/*!
 * @brief RF4_DISPEL�ν��������Ͼõ /
 * @param m_idx ��ʸ�򾧤����󥹥���ID
 * @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
 * @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
 */
void spell_RF4_DISPEL(MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	char m_name[80], t_name[80];
	monster_name(m_idx, m_name);
	monster_name(t_idx, t_name);

	monspell_message(m_idx, t_idx,
		_("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."),
		_("%^s�����Ͼõ�μ�ʸ��ǰ������", "%^s invokes a dispel magic."),
		_("%^s��%s���Ф������Ͼõ�μ�ʸ��ǰ������", "%^s invokes a dispel magic at %s."),
		TARGET_TYPE);

	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		dispel_player();
		if (p_ptr->riding) dispel_monster_status(p_ptr->riding);

		if ((p_ptr->pseikaku == SEIKAKU_COMBAT) || (inventory[INVEN_BOW].name1 == ART_CRIMSON))
			msg_print(_("���䤬�ä��ʡ�", ""));

		learn_spell(MS_DISPEL);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER)
	{
		if (t_idx == p_ptr->riding) dispel_player();
		dispel_monster_status(t_idx);
	}
}

/*!
* @brief RF4_ROCKET�ν��������åȡ� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF4_ROCKET(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s��������ͤä���", "%^s shoots something."),
		_("%^s�����åȤ�ȯ�ͤ�����", "%^s fires a rocket."),
		_("%^s��%s�˥��åȤ�ȯ�ͤ�����", "%^s fires a rocket at %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_ROCKET), m_idx, DAM_ROLL);
	breath(y, x, m_idx, GF_ROCKET, dam, 2, FALSE, MS_ROCKET, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
		update_smart_learn(m_idx, DRS_SHARD);
	return dam;
}

/*!
* @brief RF4_SHOOT�ν������ͷ⡣ /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF4_SHOOT(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx,int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s����̯�ʲ���ȯ������", "%^s makes a strange noise."),
		_("%^s��������ä���", "%^s fires an arrow."),
		_("%^s��%s��������ä���", "%^s fires an arrow at %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_SHOOT), m_idx, DAM_ROLL);
	bolt(m_idx, y, x, GF_ARROW, dam, MS_SHOOT, TARGET_TYPE);
	sound(SOUND_SHOOT);

	return dam;
}

/*!
* @brief RF4_BR_*�ν������Ƽ�֥쥹�� /
* @param GF_TYPE �֥쥹��°��
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF4_BREATH(int GF_TYPE, POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam, ms_type, drs_type = 0;
	cptr type_s;
	bool smart_learn_aux = TRUE;
	monster_type	*m_ptr = &m_list[m_idx];
	bool known = monster_near_player(m_idx, t_idx);
	bool see_either = see_monster(m_idx) || see_monster(t_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	char m_name[80], t_name[80];
	monster_name(m_idx, m_name);
	monster_name(t_idx, t_name);

	switch (GF_TYPE)
	{
	case GF_ACID:
		dam = monspell_damage((MS_BR_ACID), m_idx, DAM_ROLL);
		type_s = _("��", "acid");
		ms_type = MS_BR_ACID;
		drs_type = DRS_ACID;
		break;
	case GF_ELEC:
		dam = monspell_damage((MS_BR_ELEC), m_idx, DAM_ROLL);
		type_s = _("���", "lightning");
		ms_type = MS_BR_ELEC;
		drs_type = DRS_ELEC;
		break;
	case GF_FIRE:
		dam = monspell_damage((MS_BR_FIRE), m_idx, DAM_ROLL);
		type_s = _("�б�", "fire");
		ms_type = MS_BR_FIRE;
		drs_type = DRS_FIRE;
		break;
	case GF_COLD:
		dam = monspell_damage((MS_BR_COLD), m_idx, DAM_ROLL);
		type_s = _("�䵤", "frost");
		ms_type = MS_BR_COLD;
		drs_type = DRS_COLD;
		break;
	case GF_POIS:
		dam = monspell_damage((MS_BR_POIS), m_idx, DAM_ROLL);
		type_s = _("����", "gas");
		ms_type = MS_BR_POIS;
		drs_type = DRS_POIS;
		break;
	case GF_NETHER:
		dam = monspell_damage((MS_BR_NETHER), m_idx, DAM_ROLL);
		type_s = _("�Ϲ�", "nether");
		ms_type = MS_BR_NETHER;
		drs_type = DRS_NETH;
		break;
	case GF_LITE:
		dam = monspell_damage((MS_BR_LITE), m_idx, DAM_ROLL);
		type_s = _("����", "light");
		ms_type = MS_BR_LITE;
		drs_type = DRS_LITE;
		break;
	case GF_DARK:
		dam = monspell_damage((MS_BR_DARK), m_idx, DAM_ROLL);
		type_s = _("�Ź�", "darkness");
		ms_type = MS_BR_DARK;
		drs_type = DRS_DARK;
		break;
	case GF_CONFUSION:
		dam = monspell_damage((MS_BR_CONF), m_idx, DAM_ROLL);
		type_s = _("����", "confusion");
		ms_type = MS_BR_CONF;
		drs_type = DRS_CONF;
		break;
	case GF_SOUND:
		dam = monspell_damage((MS_BR_SOUND), m_idx, DAM_ROLL);
		type_s = _("�첻", "sound");
		ms_type = MS_BR_SOUND;
		drs_type = DRS_SOUND;
		break;
	case GF_CHAOS:
		dam = monspell_damage((MS_BR_CHAOS), m_idx, DAM_ROLL);
		type_s = _("������", "chaos");
		ms_type = MS_BR_CHAOS;
		drs_type = DRS_CHAOS;
		break;
	case GF_DISENCHANT:
		dam = monspell_damage((MS_BR_DISEN), m_idx, DAM_ROLL);
		type_s = _("����", "disenchantment");
		ms_type = MS_BR_DISEN;
		drs_type = DRS_DISEN;
		break;
	case GF_NEXUS:
		dam = monspell_damage((MS_BR_NEXUS), m_idx, DAM_ROLL);
		type_s = _("���̺���", "nexus");
		ms_type = MS_BR_NEXUS;
		drs_type = DRS_NEXUS;
		break;
	case GF_TIME:
		dam = monspell_damage((MS_BR_TIME), m_idx, DAM_ROLL);
		type_s = _("���ֵ�ž", "time");
		ms_type = MS_BR_TIME;
		smart_learn_aux = FALSE;
		break;
	case GF_INERTIAL:
		dam = monspell_damage((MS_BR_INERTIA), m_idx, DAM_ROLL);
		type_s = _("����", "inertia");
		ms_type = MS_BR_INERTIA;
		smart_learn_aux = FALSE;
		break;
	case GF_GRAVITY:
		dam = monspell_damage((MS_BR_GRAVITY), m_idx, DAM_ROLL);
		type_s = _("����", "gravity");
		ms_type = MS_BR_GRAVITY;
		smart_learn_aux = FALSE;
		break;
	case GF_SHARDS:
		dam = monspell_damage((MS_BR_SHARDS), m_idx, DAM_ROLL);
		type_s = _("����", "shards");
		ms_type = MS_BR_SHARDS;
		drs_type = DRS_SHARD;
		break;
	case GF_PLASMA:
		dam = monspell_damage((MS_BR_PLASMA), m_idx, DAM_ROLL);
		type_s = _("�ץ饺��", "plasma");
		ms_type = MS_BR_PLASMA;
		smart_learn_aux = FALSE;
		break;
	case GF_FORCE:
		dam = monspell_damage((MS_BR_FORCE), m_idx, DAM_ROLL);
		type_s = _("�ե�����", "force");
		ms_type = MS_BR_FORCE;
		smart_learn_aux = FALSE;
		break;
	case GF_MANA:
		dam = monspell_damage((MS_BR_MANA), m_idx, DAM_ROLL);
		type_s = _("����", "mana");
		ms_type = MS_BR_MANA;
		smart_learn_aux = FALSE;
		break;
	case GF_NUKE:
		dam = monspell_damage((MS_BR_NUKE), m_idx, DAM_ROLL);
		type_s = _("�������Ѵ�ʪ", "toxic waste");
		ms_type = MS_BR_NUKE;
		drs_type = DRS_POIS;
		break;
	case GF_DISINTEGRATE:
		dam = monspell_damage((MS_BR_DISI), m_idx, DAM_ROLL);
		type_s = _("ʬ��", "disintegration");
		ms_type = MS_BR_DISI;
		smart_learn_aux = FALSE;
		break;
	default:
		/* Do not reach here */
		dam = 0;
		type_s = _("����", "Unknown");
		ms_type = MS_BR_ACID;
		smart_learn_aux = FALSE;
		break;
	}

	if (mon_to_player || (mon_to_mon && known && see_either))
		disturb(1, 1);

	if (m_ptr->r_idx == MON_JAIAN && GF_TYPE == GF_SOUND)
	{
		msg_format(_("�֥ܥ�����������������", "'Booooeeeeee'"));
	}
	else if (m_ptr->r_idx == MON_BOTEI && GF_TYPE == GF_SHARDS)
	{
		msg_format(_("�֥���ӥ륫�å�����������", "'Boty-Build cutter!!!'"));
	}
	else if (p_ptr->blind)
	{
		if (mon_to_player || (mon_to_mon && known && see_either))
			msg_format(_("%^s�������Υ֥쥹���Ǥ�����", "%^s breathes."), m_name);
	}
	else
	{
		if (mon_to_player)
		{
			msg_format(_("%^s��%^s�Υ֥쥹���Ǥ�����", "%^s breathes %^s."), m_name, type_s);
		}
		else if (mon_to_mon && known && see_either)
		{
			_(msg_format("%^s��%^s��%^s�Υ֥쥹���Ǥ�����", m_name, t_name, type_s),
			  msg_format("%^s breathes %^s at %^s.", m_name, type_s, t_name));
		}
	}

	if (mon_to_mon && known && !see_either)
		mon_fight = TRUE;

	sound(SOUND_BREATH);
	breath(y, x, m_idx, GF_TYPE, dam, 0, TRUE, ms_type, TARGET_TYPE);
	if (smart_learn_aux && mon_to_player)
		update_smart_learn(m_idx, drs_type);

	return dam;
}

/*!
* @brief RF4_BA_NUKE�ν���������ǽ�塣 /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF4_BA_NUKE(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s������ǽ������ä���", "%^s casts a ball of radiation."),
		_("%^s��%s������ǽ������ä���", "%^s casts a ball of radiation at %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_BALL_NUKE), m_idx, DAM_ROLL);
	breath(y, x, m_idx, GF_NUKE, dam, 2, FALSE, MS_BALL_NUKE, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
		update_smart_learn(m_idx, DRS_POIS);

	return dam;
}

/*!
* @brief RF4_BA_CHAO�ν���������륹�� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF4_BA_CHAO(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s���������ˤĤ֤䤤����", "%^s mumbles frighteningly."),
		_("%^s������륹�����ä���", "%^s invokes a raw Logrus."),
		_("%^s��%s�˽���륹�����ä���", "%^s invokes raw Logrus upon %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_BALL_CHAOS), m_idx, DAM_ROLL);
	breath(y, x, m_idx, GF_CHAOS, dam, 4, FALSE, MS_BALL_CHAOS, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
		update_smart_learn(m_idx, DRS_CHAOS);

	return dam;
}

/*!
* @brief RF5_BA_ACID�ν����������åɡ��ܡ��롣 /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BA_ACID(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam, rad;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s�������åɡ��ܡ���μ�ʸ�򾧤�����", "%^s casts an acid ball."),
		_("%^s��%s�˸����äƥ����åɡ��ܡ���μ�ʸ�򾧤�����",
		  "%^s casts an acid ball at %s."),
		TARGET_TYPE);

	rad = monster_is_powerful(m_idx) ? 4 : 2;
	dam = monspell_damage((MS_BALL_ACID), m_idx, DAM_ROLL);
	breath(y, x, m_idx, GF_ACID, dam, rad, FALSE, MS_BALL_ACID, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
		update_smart_learn(m_idx, DRS_ACID);

	return dam;
}

/*!
* @brief RF5_BA_ELEC�ν���������������ܡ��롣 /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BA_ELEC(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam, rad;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s��������������ܡ���μ�ʸ�򾧤�����", "%^s casts a lightning ball."),
		_("%^s��%s�˸����äƥ���������ܡ���μ�ʸ�򾧤�����", 
		  "%^s casts a lightning ball at %s."),
		TARGET_TYPE);

	rad = monster_is_powerful(m_idx) ? 4 : 2;
	dam = monspell_damage((MS_BALL_ELEC), m_idx, DAM_ROLL);
	breath(y, x, m_idx, GF_ELEC, dam, rad, FALSE, MS_BALL_ELEC, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
		update_smart_learn(m_idx, DRS_ELEC);

	return dam;
}

/*!
* @brief RF5_BA_FIRE�ν������ե��������ܡ��롣 /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BA_FIRE(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam, rad;
	monster_type	*m_ptr = &m_list[m_idx];

	if (m_ptr->r_idx == MON_ROLENTO)
	{
		monspell_message(m_idx, t_idx,
			_("%s���������ꤲ����", "%^s throws something."),
			_("%s�ϼ����Ƥ��ꤲ����", "%^s throws a hand grenade."),
			_("%^s��%^s�˸����äƼ����Ƥ��ꤲ����", "%^s throws a hand grenade."),
			TARGET_TYPE);
	}
	else
	{
		monspell_message(m_idx, t_idx,
			_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
			_("%^s���ե��������ܡ���μ�ʸ�򾧤�����", "%^s casts a fire ball."),
			_("%^s��%s�˸����äƥե��������ܡ���μ�ʸ�򾧤�����",
			"%^s casts a fire ball at %s."),
			TARGET_TYPE);
	}
	rad = monster_is_powerful(m_idx) ? 4 : 2;
	dam = monspell_damage((MS_BALL_FIRE), m_idx, DAM_ROLL);
	breath(y, x, m_idx, GF_FIRE, dam, rad, FALSE, MS_BALL_FIRE, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
		update_smart_learn(m_idx, DRS_FIRE);

	return dam;
}

/*!
* @brief RF5_BA_COLD�ν��������������ܡ��롣 /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BA_COLD(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam, rad;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s�����������ܡ���μ�ʸ�򾧤�����", "%^s casts a frost ball."),
		_("%^s��%s�˸����äƥ��������ܡ���μ�ʸ�򾧤�����",
		"%^s casts a frost ball at %s."),
		TARGET_TYPE);

	rad = monster_is_powerful(m_idx) ? 4 : 2;
	dam = monspell_damage((MS_BALL_COLD), m_idx, DAM_ROLL);
	breath(y, x, m_idx, GF_COLD, dam, rad, FALSE, MS_BALL_COLD, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
		update_smart_learn(m_idx, DRS_COLD);

	return dam;
}

/*!
* @brief RF5_BA_POIS�ν������������� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BA_POIS(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s���������μ�ʸ�򾧤�����", "%^s casts a stinking cloud."),
		_("%^s��%s�˸����äư������μ�ʸ�򾧤�����", "%^s casts a stinking cloud at %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_BALL_POIS), m_idx, DAM_ROLL);
	breath(y, x, m_idx, GF_POIS, dam, 2, FALSE, MS_BALL_POIS, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
		update_smart_learn(m_idx, DRS_POIS);

	return dam;
}

/*!
* @brief RF5_BA_NETH�ν������Ϲ��塣 /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BA_NETH(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s���Ϲ���μ�ʸ�򾧤�����", "%^s casts a nether ball."),
		_("%^s��%s�˸����ä��Ϲ���μ�ʸ�򾧤�����", "%^s casts a nether ball at %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_BALL_NETHER), m_idx, DAM_ROLL);
	breath(y, x, m_idx, GF_NETHER, dam, 2, FALSE, MS_BALL_NETHER, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
		update_smart_learn(m_idx, DRS_NETH);

	return dam;
}

/*!
* @brief RF5_BA_WATE�ν������������������ܡ��롣 /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BA_WATE(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;
	bool known = monster_near_player(m_idx, t_idx);
	bool see_either = see_monster(m_idx) || see_monster(t_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	char t_name[80];
	monster_name(t_idx, t_name);


	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s��ή���褦�ʿȿ���򤷤���", "%^s gestures fluidly."),
		_("%^s��%s���Ф���ή���褦�ʿȿ���򤷤���", "%^s gestures fluidly at %s."),
		TARGET_TYPE);

	if (mon_to_player)
	{
		msg_format(_("���ʤ��ϱ������˰��߹��ޤ줿��", "You are engulfed in a whirlpool."));
	}
	else if (mon_to_mon && known && see_either && !p_ptr->blind)
	{
		msg_format(_("%^s�ϱ����˰��߹��ޤ줿��", "%^s is engulfed in a whirlpool."), t_name);
	}

	dam = monspell_damage((MS_BALL_WATER), m_idx, DAM_ROLL);
	breath(y, x, m_idx, GF_WATER, dam, 4, FALSE, MS_BALL_WATER, TARGET_TYPE);
	return dam;
}

/*!
* @brief RF5_BA_MANA�ν��������Ϥ��� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BA_MANA(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."),
		_("%^s�����Ϥ���μ�ʸ��ǰ������", "%^s invokes a mana storm."),
		_("%^s��%s���Ф������Ϥ���μ�ʸ��ǰ������", "%^s invokes a mana storm upon %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_BALL_MANA), m_idx, DAM_ROLL);
	breath(y, x, m_idx, GF_MANA, dam, 4, FALSE, MS_BALL_MANA, TARGET_TYPE);
	return dam;
}

/*!
* @brief RF5_BA_DARK�ν������Ź����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BA_DARK(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."),
		_("%^s���Ź�����μ�ʸ��ǰ������", "%^s invokes a darkness storm."),
		_("%^s��%s���Ф��ưŹ�����μ�ʸ��ǰ������", "%^s invokes a darkness storm upon %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_BALL_DARK), m_idx, DAM_ROLL);
	breath(y, x, m_idx, GF_DARK, dam, 4, FALSE, MS_BALL_DARK, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
		update_smart_learn(m_idx, DRS_DARK);

	return dam;
}

/*!
* @brief RF5_DRAIN_MANA�ν��������ϵۼ��� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_DRAIN_MANA(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;
	char m_name[80], t_name[80];
	monster_name(m_idx, m_name);
	monster_name(t_idx, t_name);


	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		disturb(1, 1);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER && see_monster(m_idx))
	{ 
		/* Basic message */
		msg_format(_("%^s���������ͥ륮����%s����ۤ��Ȥä���", "%^s draws psychic energy from %s."), m_name, t_name);
	}

	dam = monspell_damage((MS_DRAIN_MANA), m_idx, DAM_ROLL);
	breath(y, x, m_idx, GF_DRAIN_MANA, dam, 0, FALSE, MS_DRAIN_MANA, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
		update_smart_learn(m_idx, DRS_MANA);
	
	return dam;
}

/*!
* @brief RF5_MIND_BLAST�ν������������⡣ /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_MIND_BLAST(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	monster_type	*m_ptr = &m_list[m_idx];
	bool seen = (!p_ptr->blind && m_ptr->ml);
	HIT_POINT dam;
	char m_name[80], t_name[80];
	monster_name(m_idx, m_name);
	monster_name(t_idx, t_name);


	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		disturb(1, 1);
		if (!seen)
			msg_print(_("���������ʤ���������ǰ�����äƤ���褦����", "You feel something focusing on your mind."));
		else
			msg_format(_("%^s�����ʤ���Ʒ�򤸤äȤˤ��Ǥ��롣", "%^s gazes deep into your eyes."), m_name);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER && see_monster(m_idx))
	{
		msg_format(_("%^s��%s�򤸤ä��ˤ����", "%^s gazes intently at %s."), m_name, t_name);
	}

	dam = monspell_damage((MS_MIND_BLAST), m_idx, DAM_ROLL);
	breath(y, x, m_idx, GF_MIND_BLAST, dam, 0, FALSE, MS_MIND_BLAST, TARGET_TYPE);
	return dam;
}

/*!
* @brief RF5_BRAIN_SMASH�ν�����Ǿ���⡣ /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BRAIN_SMASH(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	monster_type	*m_ptr = &m_list[m_idx];
	bool seen = (!p_ptr->blind && m_ptr->ml);
	HIT_POINT dam;
	char m_name[80], t_name[80];
	monster_name(m_idx, m_name);
	monster_name(t_idx, t_name);


	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		disturb(1, 1);
		if (!seen)
			msg_print(_("���������ʤ���������ǰ�����äƤ���褦����", "You feel something focusing on your mind."));
		else
			msg_format(_("%^s�����ʤ���Ʒ�򤸤äȤˤ��Ǥ��롣", "%^s gazes deep into your eyes."), m_name);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER && see_monster(m_idx))
	{
		msg_format(_("%^s��%s�򤸤ä��ˤ����", "%^s gazes intently at %s."), m_name, t_name);
	}

	dam = monspell_damage((MS_BRAIN_SMASH), m_idx, DAM_ROLL);
	breath(y, x, m_idx, GF_BRAIN_SMASH, dam, 0, FALSE, MS_BRAIN_SMASH, TARGET_TYPE);
	return dam;
}

/*!
* @brief RF5_CAUSE_*�Υ�å����������ؿ� /
* @param GF_TYPE ����˻��Ѥ���°��
* @param dam ����˻��Ѥ�����᡼����
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param msg1 �Хץ쥤�䡼�����ܻ���å�����
* @param msg2 �Хץ쥤�䡼�������ܻ���å�����
* @param msg3 �Х�󥹥����Υ�å�����
* @param MS_TYPE ��ʸ���ֹ�
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
void spell_RF5_CAUSE(int GF_TYPE, HIT_POINT dam, POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, cptr msg1, cptr msg2, cptr msg3, int MS_TYPE, int TARGET_TYPE)
{
	char m_name[80], t_name[80];
	monster_name(m_idx, m_name);
	monster_name(t_idx, t_name);

	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		disturb(1, 1);
		if (p_ptr->blind)
			msg_format(msg1, m_name);
		else
			msg_format(msg2, m_name);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER)
	{
		if (see_monster(m_idx))
		{
			msg_format(msg3, m_name, t_name);
		}
		else
		{
			mon_fight = TRUE;
		}
	}
	breath(y, x, m_idx, GF_TYPE, dam, 0, FALSE, MS_TYPE, TARGET_TYPE);
}

/*!
* @brief RF5_CAUSE_1�ν������ڽ��μ����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_CAUSE_1(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	cptr msg1, msg2, msg3;
	HIT_POINT dam;
	dam = monspell_damage((MS_CAUSE_1), m_idx, DAM_ROLL);

	msg1 = _("%^s��������Ĥ֤䤤����", "%^s mumbles.");
	msg2 = _("%^s�����ʤ���ؤ����Ƽ��ä���", "%^s points at you and curses.");
	msg3 = _("%^s��%s��ؤ����Ƽ����򤫤�����", "%^s points at %s and curses.");
	
	spell_RF5_CAUSE(GF_CAUSE_1, dam, y, x, m_idx, t_idx, msg1, msg2, msg3, MS_CAUSE_1, TARGET_TYPE);
	return dam;
}

/*!
* @brief RF5_CAUSE_2�ν������Ž��μ����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_CAUSE_2(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	cptr msg1, msg2, msg3;
	HIT_POINT dam;
	dam = monspell_damage((MS_CAUSE_2), m_idx, DAM_ROLL);

	msg1 = _("%^s��������Ĥ֤䤤����", "%^s mumbles.");
	msg2 = _("%^s�����ʤ���ؤ����ƶ������˼��ä���", "%^s points at you and curses horribly.");
	msg3 = _("%^s��%s��ؤ����ƶ������˼����򤫤�����", "%^s points at %s and curses horribly.");

	spell_RF5_CAUSE(GF_CAUSE_2, dam, y, x, m_idx, t_idx, msg1, msg2, msg3, MS_CAUSE_2, TARGET_TYPE);
	return dam;
}

/*!
* @brief RF5_CAUSE_3�ν�������̿���μ����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_CAUSE_3(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	cptr msg1, msg2, msg3;
	HIT_POINT dam;
	dam = monspell_damage((MS_CAUSE_3), m_idx, DAM_ROLL);

	msg1 = _("%^s�������������Ƕ������", "%^s mumbles loudly.");
	msg2 = _("%^s�����ʤ���ؤ����ƶ������˼�ʸ�򾧤�����", "%^s points at you, incanting terribly!");
	msg3 = _("%^s��%s��ؤ������������˼�ʸ�򾧤�����", "%^s points at %s, incanting terribly!");

	spell_RF5_CAUSE(GF_CAUSE_3, dam, y, x, m_idx, t_idx, msg1, msg2, msg3, MS_CAUSE_3, TARGET_TYPE);
	return dam;
}

/*!
* @brief RF5_CAUSE_4�ν������빦���ͤ��� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_CAUSE_4(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	cptr msg1, msg2, msg3;
	HIT_POINT dam;
	dam = monspell_damage((MS_CAUSE_4), m_idx, DAM_ROLL);

	msg1 = _("%^s���֤����ϴ��˻��Ǥ���פȶ������", "%^s screams the word 'DIE!'");
	msg2 = _("%^s�����ʤ����빦���ͤ��ơ֤����ϴ��˻��Ǥ���פȶ������", "%^s points at you, screaming the word DIE!");
	msg3 = _("%^s��%s���빦���ͤ��ơ��֤����ϴ��˻��Ǥ���פȶ������", "%^s points at %s, screaming the word, 'DIE!'");

	spell_RF5_CAUSE(GF_CAUSE_4, dam, y, x, m_idx, t_idx, msg1, msg2, msg3, MS_CAUSE_4, TARGET_TYPE);
	return dam;
}

/*!
* @brief RF5_BO_ACID�ν����������åɡ��ܥ�ȡ� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BO_ACID(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s�������åɡ��ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a acid bolt."),
		_("%s��%s�˸����äƥ����åɡ��ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts an acid bolt at %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_BOLT_ACID), m_idx, DAM_ROLL);
	bolt(m_idx, y, x, GF_ACID, dam, MS_BOLT_ACID, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		update_smart_learn(m_idx, DRS_ACID);
		update_smart_learn(m_idx, DRS_REFLECT);
	}
	return dam;
}

/*!
* @brief RF5_BO_ELEC�ν���������������ܥ�ȡ� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BO_ELEC(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s������������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a lightning bolt."),
		_("%^s��%s�˸����äƥ���������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a lightning bolt at %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_BOLT_ELEC), m_idx, DAM_ROLL);
	bolt(m_idx, y, x, GF_ELEC, dam, MS_BOLT_ELEC, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		update_smart_learn(m_idx, DRS_ELEC);
		update_smart_learn(m_idx, DRS_REFLECT);
	}
	return dam;
}

/*!
* @brief RF5_BO_FIRE�ν������ե��������ܥ�ȡ� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BO_FIRE(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s���ե��������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a fire bolt."),
		_("%^s��%s�˸����äƥե��������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a fire bolt at %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_BOLT_FIRE), m_idx, DAM_ROLL);
	bolt(m_idx, y, x, GF_FIRE, dam, MS_BOLT_FIRE, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		update_smart_learn(m_idx, DRS_FIRE);
		update_smart_learn(m_idx, DRS_REFLECT);
	}
	return dam;
}

/*!
* @brief RF5_BO_COLD�ν��������������ܥ�ȡ� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BO_COLD(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s�����������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a frost bolt."),
		_("%^s��%s�˸����äƥ��������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a frost bolt at %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_BOLT_COLD), m_idx, DAM_ROLL);
	bolt(m_idx, y, x, GF_COLD, dam, MS_BOLT_COLD, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		update_smart_learn(m_idx, DRS_COLD);
		update_smart_learn(m_idx, DRS_REFLECT);
	}
	return dam;
}

/*!
* @brief RF5_BA_LITE�ν������������С����ȡ� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BA_LITE(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."),
		_("%^s���������С����Ȥμ�ʸ��ǰ������", "%^s invokes a starburst."),
		_("%^s��%s���Ф��ƥ������С����Ȥμ�ʸ��ǰ������", "%^s invokes a starburst upon %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_STARBURST), m_idx, DAM_ROLL);
	breath(y, x, m_idx, GF_LITE, dam, 4, FALSE, MS_STARBURST, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
		update_smart_learn(m_idx, DRS_LITE);

	return dam;
}

/*!
* @brief RF5_BO_NETH�ν������Ϲ����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BO_NETH(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s���Ϲ�����μ�ʸ�򾧤�����", "%^s casts a nether bolt."),
		_("%^s��%s�˸����ä��Ϲ�����μ�ʸ�򾧤�����", "%^s casts a nether bolt at %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_BOLT_NETHER), m_idx, DAM_ROLL);
	bolt(m_idx, y, x, GF_NETHER, dam, MS_BOLT_NETHER, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		update_smart_learn(m_idx, DRS_NETH);
		update_smart_learn(m_idx, DRS_REFLECT);
	}
	return dam;
}

/*!
* @brief RF5_BO_WATE�ν������������������ܥ�ȡ� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BO_WATE(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s���������������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a water bolt."),
		_("%^s��%s�˸����äƥ������������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a water bolt at %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_BOLT_WATER), m_idx, DAM_ROLL);
	bolt(m_idx, y, x, GF_WATER, dam, MS_BOLT_WATER, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		update_smart_learn(m_idx, DRS_REFLECT);
	}
	return dam;
}

/*!
* @brief RF5_BO_MANA�ν��������Ϥ��� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BO_MANA(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s�����Ϥ���μ�ʸ�򾧤�����", "%^s casts a mana bolt."),
		_("%^s��%s�˸����ä����Ϥ���μ�ʸ�򾧤�����", "%^s casts a mana bolt at %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_BOLT_MANA), m_idx, DAM_ROLL);
	bolt(m_idx, y, x, GF_MANA, dam, MS_BOLT_MANA, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		update_smart_learn(m_idx, DRS_REFLECT);
	}
	return dam;
}

/*!
* @brief RF5_BO_PLAS�ν������ץ饺�ޡ��ܥ�ȡ� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BO_PLAS(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s���ץ饺�ޡ��ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a plasma bolt."),
		_("%^s��%s�˸����äƥץ饺�ޡ��ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a plasma bolt at %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_BOLT_PLASMA), m_idx, DAM_ROLL);
	bolt(m_idx, y, x, GF_PLASMA, dam, MS_BOLT_PLASMA, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		update_smart_learn(m_idx, DRS_REFLECT);
	}
	return dam;
}

/*!
* @brief RF5_BO_ICEE�ν������˴����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_BO_ICEE(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s���˴�����μ�ʸ�򾧤�����", "%^s casts an ice bolt."),
		_("%^s��%s�˸����äƶ˴�����μ�ʸ�򾧤�����", "%^s casts an ice bolt at %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_BOLT_ICE), m_idx, DAM_ROLL);
	bolt(m_idx, y, x, GF_ICE, dam, MS_BOLT_ICE, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		update_smart_learn(m_idx, DRS_COLD);
		update_smart_learn(m_idx, DRS_REFLECT);
	}
	return dam;
}

/*!
* @brief RF5_MISSILE�ν������ޥ��å����ߥ����롣 /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF5_MISSILE(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s���ޥ��å����ߥ�����μ�ʸ�򾧤�����", "%^s casts a magic missile."),
		_("%^s��%s�˸����äƥޥ��å����ߥ�����μ�ʸ�򾧤�����", "%^s casts a magic missile at %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_MAGIC_MISSILE), m_idx, DAM_ROLL);
	bolt(m_idx, y, x, GF_MISSILE, dam, MS_MAGIC_MISSILE, TARGET_TYPE);
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		update_smart_learn(m_idx, DRS_REFLECT);
	}
	return dam;
}

/*!
* @brief ���ְ۾��ʸ�Υ�å����������ؿ��� /
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param msg1 �Хץ쥤�䡼�ʤ����ܻ���å��������Х�󥹥����ʤ��̾����å�������
* @param msg2 �Хץ쥤�䡼�ʤ������ܻ���å��������Х�󥹥����ʤ�����ͭ��å�������
* @param msg3 �Хץ쥤�䡼�ʤ�����ͭ��å��������Х�󥹥����ʤ��񹳻���å�������
* @param msg4 �Хץ쥤�䡼�ʤ��񹳻���å��������Х�󥹥����ʤ���������å�������
* @param resist ������̵ͭ��Ƚ�̤���ե饰
* @param saving_throw �񹳤�����������Ƚ�̤���ե饰
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
*/
void spell_badstatus_message(MONSTER_IDX m_idx, IDX t_idx, cptr msg1, cptr msg2, cptr msg3, cptr msg4, bool resist, bool saving_throw, int TARGET_TYPE)
{
	bool see_either = see_monster(m_idx) || see_monster(t_idx);
	bool see_t = see_monster(t_idx);
	bool known = monster_near_player(m_idx, t_idx);
	char m_name[80], t_name[80];
	monster_name(m_idx, m_name);
	monster_name(t_idx, t_name);

	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		disturb(1, 1);
		if (p_ptr->blind)
			msg_format(msg1, m_name);
		else
			msg_format(msg2, m_name);

		if (resist)
		{
			msg_print(msg3);
		}
		else if (saving_throw)
		{
			msg_print(msg4);
		}
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER)
	{
		if (known)
		{
			if (see_either)
			{
				msg_format(msg1, m_name, t_name);
			}
			else
			{
				mon_fight = TRUE;
			}
		}

		if (resist)
		{
			if (see_t) msg_format(msg2, t_name);
		}
		else if (saving_throw)
		{
			if (see_t) msg_format(msg3, t_name);
		}
		else
		{
			if (see_t) msg_format(msg4, t_name);
		}
		monster_wakeup(t_idx);
	}
}

/*!
 * @brief RF5_SCARE�ν��������ݡ� /
 * @param m_idx ��ʸ�򾧤����󥹥���ID
 * @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
 * @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
 */
void spell_RF5_SCARE(MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	monster_type	*t_ptr = &m_list[t_idx];
	monster_race	*tr_ptr = &r_info[t_ptr->r_idx];
	int rlev = monster_level_idx(m_idx);
	bool resist, saving_throw;

	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		resist = p_ptr->resist_fear;
		saving_throw = (randint0(100 + rlev / 2) < p_ptr->skill_sav);
		spell_badstatus_message(m_idx, t_idx,
			_("%^s��������Ĥ֤䤯�ȡ��������ʲ���ʹ��������", "%^s mumbles, and you hear scary noises."),
			_("%^s���������ʸ��Ф���Ф�����", "%^s casts a fearful illusion."),
			_("���������ݤ˿�����ʤ��ä���", "You refuse to be frightened."),
			_("���������ݤ˿�����ʤ��ä���", "You refuse to be frightened."),
			resist, saving_throw, TARGET_TYPE);

		if (!resist && !saving_throw)
		{
			(void)set_afraid(p_ptr->afraid + randint0(4) + 4);
		}
		learn_spell(MS_SCARE);
		update_smart_learn(m_idx, DRS_FEAR);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER)
	{
		resist = tr_ptr->flags3 & RF3_NO_FEAR;
		saving_throw = (tr_ptr->level > randint1((rlev - 10) < 1 ? 1 : (rlev - 10)) + 10);

		spell_badstatus_message(m_idx, t_idx, 
			_("%^s���������ʸ��Ф���Ф�����", "%^s casts a fearful illusion in front of %s."),
			_("%^s�϶��ݤ򴶤��ʤ���", "%^s refuses to be frightened."),
			_("%^s�϶��ݤ򴶤��ʤ���", "%^s refuses to be frightened."),
			_("%^s�϶��ݤ���ƨ���Ф�����", "%^s flees in terror!"),
			resist, saving_throw, TARGET_TYPE);

		if (!resist && !saving_throw)
		{
			set_monster_monfear(t_idx, MON_MONFEAR(t_ptr) + randint0(4) + 4);
		}
	}
}

/*!
 * @brief RF5_BLIND�ν��������ܡ� /
 * @param m_idx ��ʸ�򾧤����󥹥���ID
 * @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
 * @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
 */
void spell_RF5_BLIND(MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	monster_type	*t_ptr = &m_list[t_idx];
	monster_race	*tr_ptr = &r_info[t_ptr->r_idx];
	int rlev = monster_level_idx(m_idx);
	bool resist, saving_throw;

	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		resist = p_ptr->resist_blind;
		saving_throw = (randint0(100 + rlev / 2) < p_ptr->skill_sav);
		spell_badstatus_message(m_idx, t_idx,
			_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
			_("%^s����ʸ�򾧤��Ƥ��ʤ����ܤ򤯤�ޤ�����", "%^s casts a spell, burning your eyes!"),
			_("���������̤��ʤ��ä���", "You are unaffected!"),
			_("���������Ϥ�ķ���֤�����", "You resist the effects!"),
			resist, saving_throw, TARGET_TYPE);

		if (!resist && !saving_throw)
		{
			(void)set_blind(12 + randint0(4));
		}
		learn_spell(MS_BLIND);
		update_smart_learn(m_idx, DRS_BLIND);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER)
	{
		cptr msg1;
		char t_name[80];
		monster_name(t_idx, t_name);
		
		if (streq(t_name, "it"))
		{
			msg1 = _("%s�ϼ�ʸ�򾧤���%s���ܤ�Ƥ��դ�������", "%^s casts a spell, burning %ss eyes.");
		}
		else
		{
			msg1 = _("%s�ϼ�ʸ�򾧤���%s���ܤ�Ƥ��դ�������", "%^s casts a spell, burning %s's eyes.");
		}

		resist = tr_ptr->flags3 & RF3_NO_CONF;
		saving_throw = (tr_ptr->level > randint1((rlev - 10) < 1 ? 1 : (rlev - 10)) + 10);

		spell_badstatus_message(m_idx, t_idx,
			msg1,
			_("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected."),
			_("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected."),
			_("%^s���ܤ������ʤ��ʤä��� ", "%^s is blinded!"),
			resist, saving_throw, TARGET_TYPE);

		if (!resist && !saving_throw)
		{
			(void)set_monster_confused(t_idx, MON_CONFUSED(t_ptr) + 12 + randint0(4));
		}
	}
}

/*!
 * @brief RF5_CONF�ν���������/
 * @param m_idx ��ʸ�򾧤����󥹥���ID
 * @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
 * @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
 */
void spell_RF5_CONF(MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	monster_type	*t_ptr = &m_list[t_idx];
	monster_race	*tr_ptr = &r_info[t_ptr->r_idx];
	int rlev = monster_level_idx(m_idx);
	bool resist, saving_throw;

	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		resist = p_ptr->resist_conf;
		saving_throw = (randint0(100 + rlev / 2) < p_ptr->skill_sav);
		spell_badstatus_message(m_idx, t_idx,
			_("%^s��������Ĥ֤䤯�ȡ�Ƭ��Ǻ�ޤ�����������", "%^s mumbles, and you hear puzzling noises."),
			_("%^s��Ͷ��Ū�ʸ��Ф���Ф�����", "%^s creates a mesmerising illusion."),
			_("���������ФˤϤ��ޤ���ʤ��ä���", "You disbelieve the feeble spell."),
			_("���������ФˤϤ��ޤ���ʤ��ä���", "You disbelieve the feeble spell."),
			resist, saving_throw, TARGET_TYPE);

		if (!resist && !saving_throw)
		{
			(void)set_confused(p_ptr->confused + randint0(4) + 4);
		}
		learn_spell(MS_CONF);
		update_smart_learn(m_idx, DRS_CONF);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER)
	{
		resist = tr_ptr->flags3 & RF3_NO_CONF;
		saving_throw = (tr_ptr->level > randint1((rlev - 10) < 1 ? 1 : (rlev - 10)) + 10);

		spell_badstatus_message(m_idx, t_idx,
			_("%^s��%s�����˸���Ū�ʸ���Ĥ���Ф�����", "%^s casts a mesmerizing illusion in front of %s."),
			_("%^s���Ǥ蘆��ʤ��ä���", "%^s disbelieves the feeble spell."),
			_("%^s���Ǥ蘆��ʤ��ä���", "%^s disbelieves the feeble spell."),
			_("%^s�Ϻ��𤷤��褦����", "%^s seems confused."),
			resist, saving_throw, TARGET_TYPE);

		if (!resist && !saving_throw)
		{
			(void)set_monster_confused(t_idx, MON_CONFUSED(t_ptr) + 12 + randint0(4));
		}
	}
}

/*!
 * @brief RF5_SLOW�ν�������®�� /
 * @param m_idx ��ʸ�򾧤����󥹥���ID
 * @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
 * @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
 */
void spell_RF5_SLOW(MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	monster_type	*t_ptr = &m_list[t_idx];
	monster_race	*tr_ptr = &r_info[t_ptr->r_idx];
	int rlev = monster_level_idx(m_idx);
	bool resist, saving_throw;

	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		resist = p_ptr->resist_conf;
		saving_throw = (randint0(100 + rlev / 2) < p_ptr->skill_sav);
		spell_badstatus_message(m_idx, t_idx,
			_("%^s�����ʤ��ζ��Ϥ�ۤ�����Ȥ�����", "%^s drains power from your muscles!"),
			_("%^s�����ʤ��ζ��Ϥ�ۤ�����Ȥ�����", "%^s drains power from your muscles!"),
			_("���������̤��ʤ��ä���", "You are unaffected!"),
			_("���������Ϥ�ķ���֤�����", "You resist the effects!"),
			resist, saving_throw, TARGET_TYPE);

		if (!resist && !saving_throw)
		{
			(void)set_slow(p_ptr->slow + randint0(4) + 4, FALSE);
		}
		learn_spell(MS_SLOW);
		update_smart_learn(m_idx, DRS_FREE);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER)
	{
		cptr msg1;
		char t_name[80];
		monster_name(t_idx, t_name);

		if (streq(t_name, "it"))
		{
			msg1 = _("%s��%s�ζ��������Ϥ�ۤ��Ȥä���", "%^s drains power from %ss muscles.");
		}
		else
		{
			msg1 = _("%s��%s�ζ��������Ϥ�ۤ��Ȥä���", "%^s drains power from %s's muscles.");
		}

		resist = tr_ptr->flags1 & RF1_UNIQUE;
		saving_throw = (tr_ptr->level > randint1((rlev - 10) < 1 ? 1 : (rlev - 10)) + 10);

		spell_badstatus_message(m_idx, t_idx,
			msg1,
			_("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected."),
			_("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected."),
			_("%s��ư�����٤��ʤä���", "%^s starts moving slower."),
			resist, saving_throw, TARGET_TYPE);

		if (!resist && !saving_throw)
		{
			set_monster_slow(t_idx, MON_SLOW(t_ptr) + 50);
		}
	}
}

/*!
 * @brief RF5_HOLD�ν��������㡣 /
 * @param m_idx ��ʸ�򾧤����󥹥���ID
 * @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
 * @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
 */
void spell_RF5_HOLD(MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	monster_type	*t_ptr = &m_list[t_idx];
	monster_race	*tr_ptr = &r_info[t_ptr->r_idx];
	int rlev = monster_level_idx(m_idx);
	bool resist, saving_throw;

	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		resist = p_ptr->free_act;
		saving_throw = (randint0(100 + rlev / 2) < p_ptr->skill_sav);
		spell_badstatus_message(m_idx, t_idx,
			_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
			_("%^s�����ʤ����ܤ򤸤äȸ��Ĥ᤿��", "%^s stares deep into your eyes!"),
			_("���������̤��ʤ��ä���", "You are unaffected!"),
			_("���������Ϥ�ķ���֤�����", "You resist the effects!"),
			resist, saving_throw, TARGET_TYPE);

		if (!resist && !saving_throw)
		{
			(void)set_paralyzed(p_ptr->paralyzed + randint0(4) + 4);
		}
		learn_spell(MS_SLEEP);
		update_smart_learn(m_idx, DRS_FREE);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER)
	{
		resist = (tr_ptr->flags1 & RF1_UNIQUE) || (tr_ptr->flags3 & RF3_NO_STUN);
		saving_throw = (tr_ptr->level > randint1((rlev - 10) < 1 ? 1 : (rlev - 10)) + 10);

		spell_badstatus_message(m_idx, t_idx,
			_("%^s��%s�򤸤äȸ��Ĥ᤿��", "%^s stares intently at %s."),
			_("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected."),
			_("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected."), 
			_("%^s�����㤷����", "%^s is paralyzed!"),
			resist, saving_throw, TARGET_TYPE);

		if (!resist && !saving_throw)
		{
			(void)set_monster_stunned(t_idx, MON_STUNNED(t_ptr) + randint1(4) + 4);
		}
	}
}

/*!
* @brief RF6_HASTE�ν�������®�� /
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
*/
void spell_RF6_HASTE(MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	bool see_m = see_monster(m_idx);
	monster_type	*m_ptr = &m_list[m_idx];
	char m_name[80];
	monster_name(m_idx, m_name);

	monspell_message_base(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s����ʬ���Τ�ǰ�����ä���", "%^s concentrates on %s body."),
		_("%^s����ʬ���Τ�ǰ�����ä���", "%^s concentrates on %s body."),
		_("%^s����ʬ���Τ�ǰ�����ä���", "%^s concentrates on %s body."),
		p_ptr->blind > 0, TARGET_TYPE);

	/* Allow quick speed increases to base+10 */
	if (set_monster_fast(m_idx, MON_FAST(m_ptr) + 100))
	{
		if (TARGET_TYPE == MONSTER_TO_PLAYER ||
			(TARGET_TYPE == MONSTER_TO_MONSTER && see_m))
			msg_format(_("%^s��ư����®���ʤä���", "%^s starts moving faster."), m_name);
	}
}

/*!
* @brief RF6_HAND_DOOM�ν��������Ǥμꡣ /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF6_HAND_DOOM(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam = 0;

	simple_monspell_message(m_idx, t_idx,
		_("%^s��<���Ǥμ�>�����ä���", "%^s invokes the Hand of Doom!"),
		_("%^s��%s��<���Ǥμ�>�����ä���", "%^s invokes the Hand of Doom upon %s!"),
		TARGET_TYPE);

	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		dam = monspell_damage((MS_HAND_DOOM), m_idx, DAM_ROLL);
		breath(y, x, m_idx, GF_HAND_DOOM, dam, 0, FALSE, MS_HAND_DOOM, MONSTER_TO_PLAYER);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER)
	{
		dam = 20; /* Dummy power */
		breath(y, x, m_idx, GF_HAND_DOOM, dam, 0, FALSE, MS_HAND_DOOM, MONSTER_TO_MONSTER);
	}
	return dam;
}

/*!
* @brief RF6_HEAL�ν����������� /
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
*/
void spell_RF6_HEAL(MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	monster_type	*m_ptr = &m_list[m_idx];
	int rlev = monster_level_idx(m_idx);
	bool seen = (!p_ptr->blind && m_ptr->ml);
	char m_name[80];
	monster_name(m_idx, m_name);

	disturb(1, 1);

	/* Message */
	monspell_message_base(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s�ϼ�ʬ�ν���ǰ���椷����", "%^s concentrates on %s wounds."),
		_("%^s����ʬ�ν��˽��椷����", "%^s concentrates on %s wounds."),
		_("%^s�ϼ�ʬ�ν���ǰ���椷����", "%^s concentrates on %s wounds."),
		p_ptr->blind > 0, TARGET_TYPE);

	/* Heal some */
	m_ptr->hp += (rlev * 6);

	/* Fully healed */
	if (m_ptr->hp >= m_ptr->maxhp)
	{
		/* Fully healed */
		m_ptr->hp = m_ptr->maxhp;

		/* Message */
		monspell_message_base(m_idx, t_idx,
			_("%^s�ϴ����˼��ä��褦����", "%^s sounds completely healed!"),
			_("%^s�ϴ����˼��ä��褦����", "%^s sounds completely healed!"),
			_("%^s�ϴ����˼��ä���", "%^s looks completely healed!"),
			_("%^s�ϴ����˼��ä���", "%^s looks completely healed!"),
			!seen, TARGET_TYPE);
	}

	/* Partially healed */
	else
	{
		/* Message */
		monspell_message_base(m_idx, t_idx,
			_("%^s�����Ϥ���������褦����", "%^s sounds healthier."),
			_("%^s�����Ϥ���������褦����", "%^s sounds healthier."),
			_("%^s�����Ϥ���������褦����", "%^s looks healthier."),
			_("%^s�����Ϥ���������褦����", "%^s looks healthier."),
			!seen, TARGET_TYPE);
	}

	/* Redraw (later) if needed */
	if (p_ptr->health_who == m_idx) p_ptr->redraw |= (PR_HEALTH);
	if (p_ptr->riding == m_idx) p_ptr->redraw |= (PR_UHEALTH);

	/* Cancel fear */
	if (MON_MONFEAR(m_ptr))
	{
		/* Cancel fear */
		(void)set_monster_monfear(m_idx, 0);

		/* Message */
		if (see_monster(m_idx))
			msg_format(_("%^s��ͦ�������ᤷ����", "%^s recovers %s courage."), m_name);
	}
}

/*!
* @brief RF6_INVULNER�ν�����̵Ũ�� /
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
*/
void spell_RF6_INVULNER(MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	monster_type	*m_ptr = &m_list[m_idx];
	bool seen = (!p_ptr->blind && m_ptr->ml);

	/* Message */
	monspell_message_base(m_idx, t_idx,
			_("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."),
			_("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."),
			_("%s��̵���ε�μ�ʸ�򾧤�����", "%^s casts a Globe of Invulnerability."),
			_("%s��̵���ε�μ�ʸ�򾧤�����", "%^s casts a Globe of Invulnerability."),
			!seen, TARGET_TYPE);

	if (!MON_INVULNER(m_ptr)) (void)set_monster_invulner(m_idx, randint1(4) + 4, FALSE);
}

/*!
* @brief RF6_BLINK�ν��������硼�ȡ��ƥ�ݡ��ȡ� /
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
*/
void spell_RF6_BLINK(MONSTER_IDX m_idx, int TARGET_TYPE)
{
	char m_name[80];
	monster_name(m_idx, m_name);
	
	if (TARGET_TYPE==MONSTER_TO_PLAYER)
		disturb(1, 1);

	if (teleport_barrier(m_idx))
	{
		if(see_monster(m_idx))
			msg_format(_("��ˡ�ΥХꥢ��%^s�Υƥ�ݡ��Ȥ���⤷����",
						 "Magic barrier obstructs teleporting of %^s."), m_name);
	}
	else
	{
		if(see_monster(m_idx))
			msg_format(_("%^s���ֻ��˾ä�����", "%^s blinks away."), m_name);

		teleport_away(m_idx, 10, 0L);

		if (TARGET_TYPE==MONSTER_TO_PLAYER)
			p_ptr->update |= (PU_MONSTERS);
	}
}

/*!
* @brief RF6_TPORT�ν������ƥ�ݡ��ȡ� /
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
*/
void spell_RF6_TPORT(MONSTER_IDX m_idx, int TARGET_TYPE)
{	
	char m_name[80];
	monster_name(m_idx, m_name);
	
	if (TARGET_TYPE==MONSTER_TO_PLAYER)
		disturb(1, 1);
	if (teleport_barrier(m_idx))
	{
		if(see_monster(m_idx))
			msg_format(_("��ˡ�ΥХꥢ��%^s�Υƥ�ݡ��Ȥ���⤷����",
						 "Magic barrier obstructs teleporting of %^s."), m_name);
	}
	else
	{
		if(see_monster(m_idx))
			msg_format(_("%^s���ƥ�ݡ��Ȥ�����", "%^s teleports away."), m_name);

		teleport_away_followable(m_idx);
	}
}

/*!
* @brief RF6_WORLD�ν���������ߤ�롣 /
* @param m_idx ��ʸ�򾧤����󥹥���ID
*/
HIT_POINT spell_RF6_WORLD(MONSTER_IDX m_idx)
{
	monster_type	*m_ptr = &m_list[m_idx];
	int who = 0;
	char m_name[80];
	monster_name(m_idx, m_name);

	disturb(1, 1);
	if (m_ptr->r_idx == MON_DIO) who = 1;
	else if (m_ptr->r_idx == MON_WONG) who = 3;
	if (!process_the_world(randint1(2) + 2, who, TRUE)) return (FALSE);
	return who;
}

/*!
* @brief �С��Ρ��롦��ѡ��Ȥ�RF6_SPECIAL�ν�����ʬ�������Ρ� /
* @param m_idx ��ʸ�򾧤����󥹥���ID
*/
HIT_POINT spell_RF6_SPECIAL_BANORLUPART(MONSTER_IDX m_idx)
{
	monster_type *m_ptr = &m_list[m_idx];
	HIT_POINT dummy_hp, dummy_maxhp;
	MONSTER_IDX k;
	POSITION dummy_y = m_ptr->fy;
	POSITION dummy_x = m_ptr->fx;
	BIT_FLAGS mode = 0L;

	switch(m_ptr->r_idx)
	{
		case MON_BANORLUPART:
			dummy_hp = (m_ptr->hp + 1) / 2;
			dummy_maxhp = m_ptr->maxhp / 2;
			
			if (p_ptr->inside_arena || p_ptr->inside_battle || !summon_possible(m_ptr->fy, m_ptr->fx)) 
				return -1;

			delete_monster_idx(cave[m_ptr->fy][m_ptr->fx].m_idx);
			summon_named_creature(0, dummy_y, dummy_x, MON_BANOR, mode);
			m_list[hack_m_idx_ii].hp = dummy_hp;
			m_list[hack_m_idx_ii].maxhp = dummy_maxhp;
			summon_named_creature(0, dummy_y, dummy_x, MON_LUPART, mode);
			m_list[hack_m_idx_ii].hp = dummy_hp;
			m_list[hack_m_idx_ii].maxhp = dummy_maxhp;

			msg_print(_("�إС��Ρ��롦��ѡ��ȡ٤�ʬ��������","Banor=Rupart splits in two person!"));
			break;
		
		case MON_BANOR:
		case MON_LUPART:
			dummy_hp = 0;
			dummy_maxhp = 0;

			if (!r_info[MON_BANOR].cur_num || !r_info[MON_LUPART].cur_num) 
				return -1;

			for (k = 1; k < m_max; k++)
			{
				if (m_list[k].r_idx == MON_BANOR || m_list[k].r_idx == MON_LUPART)
				{
					dummy_hp += m_list[k].hp;
					dummy_maxhp += m_list[k].maxhp;
					if (m_list[k].r_idx != m_ptr->r_idx)
					{
						dummy_y = m_list[k].fy;
						dummy_x = m_list[k].fx;
					}
					delete_monster_idx(k);
				}
			}
			summon_named_creature(0, dummy_y, dummy_x, MON_BANORLUPART, mode);
			m_list[hack_m_idx_ii].hp = dummy_hp;
			m_list[hack_m_idx_ii].maxhp = dummy_maxhp;

			msg_print(_("�إС��Ρ���٤ȡإ�ѡ��ȡ٤����Τ�����", "Banor and Rupart combine into one!"));
			break;
	}
	return 0;
}

/*!
* @brief ����Ȥ�RF6_SPECIAL�ν����������Ƥξ����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF6_SPECIAL_ROLENTO(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	int count = 0, k;
	int num = 1 + randint1(3);
	BIT_FLAGS mode = 0L;
	
	monspell_message(m_idx, t_idx,
		_("%^s���������̤��ꤲ����", "%^s spreads something."),
		_("%^s�ϼ����Ƥ�Ф�ޤ�����", "%^s throws some hand grenades."),
		_("%^s�ϼ����Ƥ�Ф�ޤ�����", "%^s throws some hand grenades."),
		TARGET_TYPE);

	for (k = 0; k < num; k++)
	{
		count += summon_named_creature(m_idx, y, x, MON_SHURYUUDAN, mode);
	}
	
	if (p_ptr->blind && count)
		msg_print(_("¿���Τ�Τ��ֶ�ˤФ�ޤ���벻�����롣", "You hear many things are scattered nearby."));
	
	return 0;
}

/*!
* @brief B����ܥ��RF6_SPECIAL�ν������ꤲ��Ȥ����⡣ /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF6_SPECIAL_B(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam = -1;
	monster_type	*m_ptr = &m_list[m_idx];
	monster_type	*t_ptr = &m_list[t_idx];
	monster_race	*tr_ptr = &r_info[t_ptr->r_idx];
	bool monster_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	bool monster_to_monster = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool direct = player_bold(y, x);
	char m_name[80];
	monster_name(m_idx, m_name);

	disturb(1, 1);
	if (one_in_(3) || !direct)
	{		
		simple_monspell_message(m_idx, t_idx,
			_("%^s�������볦����ä���!", "%^s suddenly go out of your sight!"),
			_("%^s�������޾徺���ƻ볦����ä���!", "%^s suddenly go out of your sight!"),
			TARGET_TYPE);
				
		teleport_away(m_idx, 10, TELEPORT_NONMAGICAL);
		p_ptr->update |= (PU_MONSTERS);
	}
	else
	{
		int get_damage = 0;
		bool fear; /* dummy */
	
		simple_monspell_message(m_idx, t_idx,
			_("%^s�����ʤ����Ϥ�Ƕ��椫���ꤲ��Ȥ�����", "%^s holds you, and drops from the sky."),
			_("%^s��%s���Ϥ�Ƕ��椫���ꤲ��Ȥ�����", "%^s holds %s, and drops from the sky."),
			TARGET_TYPE);

		dam = damroll(4, 8);

		if (monster_to_player || t_idx == p_ptr->riding)
			teleport_player_to(m_ptr->fy, m_ptr->fx, TELEPORT_NONMAGICAL | TELEPORT_PASSIVE);
		else 
			teleport_monster_to(t_idx, m_ptr->fy, m_ptr->fx, 100, TELEPORT_NONMAGICAL | TELEPORT_PASSIVE);

		sound(SOUND_FALL);

		if ((monster_to_player && p_ptr->levitation) ||
			(monster_to_monster && (tr_ptr->flags7 & RF7_CAN_FLY)))
		{
			simple_monspell_message(m_idx, t_idx,
				_("���ʤ����Ť������Ϥ�����", "You float gently down to the ground."),
				_("%^s���Ť������Ϥ�����", "%^s floats gently down to the ground."),
				TARGET_TYPE);
		}
		else
		{
			simple_monspell_message(m_idx, t_idx,
				_("���ʤ������̤�á���Ĥ���줿��", "You crashed into the ground."),
				_("%^s�����̤�á���Ĥ���줿��", "%^s crashed into the ground."),
				TARGET_TYPE);
			dam += damroll(6, 8);
		}

		if(monster_to_player ||
		   (monster_to_monster && p_ptr->riding == t_idx))
		{
			/* Mega hack -- this special action deals damage to the player. Therefore the code of "eyeeye" is necessary.
			-- henkma
			*/
			get_damage = take_hit(DAMAGE_NOESCAPE, dam, m_name, -1);
			if (p_ptr->tim_eyeeye && get_damage > 0 && !p_ptr->is_dead)
			{
				char m_name_self[80];
				/* hisself */
				monster_desc(m_name_self, m_ptr, MD_PRON_VISIBLE | MD_POSSESSIVE | MD_OBJECTIVE);

				msg_format(_("���⤬%s���Ȥ���Ĥ�����", "The attack of %s has wounded %s!"), m_name, m_name_self);

				project(0, 0, m_ptr->fy, m_ptr->fx, get_damage, GF_MISSILE, PROJECT_KILL, -1);
				set_tim_eyeeye(p_ptr->tim_eyeeye - 5, TRUE);
			}
		}

		if(monster_to_player && p_ptr->riding)
			mon_take_hit_mon(p_ptr->riding, dam, &fear, extract_note_dies(real_r_ptr(&m_list[p_ptr->riding])), m_idx);

		if(monster_to_monster)
			mon_take_hit_mon(t_idx, dam, &fear, extract_note_dies(real_r_ptr(t_ptr)), m_idx);
	}
	return dam;
}

/*!
* @brief RF6_SPECIAL�ν�������󥹥����μ���ˤ�äƼ½����˿���ʬ���롣 /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF6_SPECIAL(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	monster_type	*m_ptr = &m_list[m_idx];
	monster_race	*r_ptr = &r_info[m_ptr->r_idx];

	disturb(1, 1);
	switch (m_ptr->r_idx)
	{
		case MON_OHMU:
			/* Moved to process_monster(), like multiplication */
			return -1;

		case MON_BANORLUPART:
		case MON_BANOR:
		case MON_LUPART:
			return spell_RF6_SPECIAL_BANORLUPART(m_idx);

		case MON_ROLENTO:
			return spell_RF6_SPECIAL_ROLENTO(y, x, m_idx, t_idx, TARGET_TYPE);
			break;

		default:
		if (r_ptr->d_char == 'B')
		{
			return spell_RF6_SPECIAL_B(y, x, m_idx, t_idx, TARGET_TYPE);
			break;
		}

		/* Something is wrong */
		else return -1;
	}
}

/*!
* @brief RF6_TELE_TO�ν������ƥ�ݡ��ȡ��Хå��� /
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
void spell_RF6_TELE_TO(MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	monster_type	*m_ptr = &m_list[m_idx];
	monster_type	*t_ptr = &m_list[t_idx];
	monster_race	*tr_ptr = &r_info[t_ptr->r_idx];

	simple_monspell_message(m_idx, t_idx,
		_("%^s�����ʤ�������ᤷ����", "%^s commands you to return."),
		_("%^s��%s������ᤷ����", "%^s commands %s to return."),
		TARGET_TYPE);
	
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		teleport_player_to(m_ptr->fy, m_ptr->fx, TELEPORT_PASSIVE);
		learn_spell(MS_TELE_TO);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER)
	{
		bool resists_tele = FALSE;
		char t_name[80];
		monster_name(t_idx, t_name);

		if (tr_ptr->flagsr & RFR_RES_TELE)
		{
			if ((tr_ptr->flags1 & RF1_UNIQUE) || (tr_ptr->flagsr & RFR_RES_ALL))
			{
				if (is_original_ap_and_seen(t_ptr)) tr_ptr->r_flagsr |= RFR_RES_TELE;
				if (see_monster(t_idx))
				{
					msg_format(_("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected!"), t_name);
				}
				resists_tele = TRUE;
			}
			else if (tr_ptr->level > randint1(100))
			{
				if (is_original_ap_and_seen(t_ptr)) tr_ptr->r_flagsr |= RFR_RES_TELE;
				if (see_monster(t_idx))
				{
					msg_format(_("%^s����������äƤ��롪", "%^s resists!"), t_name);
				}
				resists_tele = TRUE;
			}
		}

		if (!resists_tele)
		{
			if (t_idx == p_ptr->riding) 
				teleport_player_to(m_ptr->fy, m_ptr->fx, TELEPORT_PASSIVE);
			else 
				teleport_monster_to(t_idx, m_ptr->fy, m_ptr->fx, 100, TELEPORT_PASSIVE);
		}
		monster_wakeup(t_idx);
	}
}

/*!
* @brief RF6_TELE_AWAY�ν������ƥ�ݡ��ȡ����������� /
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
void spell_RF6_TELE_AWAY(MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	monster_type	*t_ptr = &m_list[t_idx];
	monster_race	*tr_ptr = &r_info[t_ptr->r_idx];

	simple_monspell_message(m_idx, t_idx,
		_("%^s�˥ƥ�ݡ��Ȥ�����줿��", "%^s teleports you away."),
		_("%^s��%s��ƥ�ݡ��Ȥ�������", "%^s teleports %s away."),
		TARGET_TYPE);
	
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		if ((p_ptr->pseikaku == SEIKAKU_COMBAT) || (inventory[INVEN_BOW].name1 == ART_CRIMSON))
			msg_print(_("���ä���", ""));
		
		learn_spell(MS_TELE_AWAY);
		teleport_player_away(m_idx, 100);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER)
	{
		bool resists_tele = FALSE;
		char t_name[80];
		monster_name(t_idx, t_name);

		if (tr_ptr->flagsr & RFR_RES_TELE)
		{
			if ((tr_ptr->flags1 & RF1_UNIQUE) || (tr_ptr->flagsr & RFR_RES_ALL))
			{
				if (is_original_ap_and_seen(t_ptr)) tr_ptr->r_flagsr |= RFR_RES_TELE;
				if (see_monster(t_idx))
				{
					msg_format(_("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected!"), t_name);
				}
				resists_tele = TRUE;
			}
			else if (tr_ptr->level > randint1(100))
			{
				if (is_original_ap_and_seen(t_ptr)) tr_ptr->r_flagsr |= RFR_RES_TELE;
				if (see_monster(t_idx))
				{
					msg_format(_("%^s����������äƤ��롪", "%^s resists!"), t_name);
				}
				resists_tele = TRUE;
			}
		}

		if (!resists_tele)
		{
			if (t_idx == p_ptr->riding) 
				teleport_player_away(m_idx, MAX_SIGHT * 2 + 5);
			else 
				teleport_away(t_idx, MAX_SIGHT * 2 + 5, TELEPORT_PASSIVE);
		}
		monster_wakeup(t_idx);
	}
}

/*!
* @brief RF6_TELE_LEVEL�ν������ƥ�ݡ��ȡ���٥롣 /
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
void spell_RF6_TELE_LEVEL(MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	monster_type	*t_ptr = &m_list[t_idx];
	monster_race	*tr_ptr = &r_info[t_ptr->r_idx];
	int rlev = monster_level_idx(m_idx);
	bool resist, saving_throw;

	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		resist = p_ptr->resist_nexus;
		saving_throw = (randint0(100 + rlev / 2) < p_ptr->skill_sav);
		spell_badstatus_message(m_idx, t_idx,
			_("%^s��������̯�ʸ��դ�Ĥ֤䤤����", "%^s mumbles strangely."),
			_("%^s�����ʤ���­��ؤ�������", "%^s gestures at your feet."),
			_("���������̤��ʤ��ä���", "You are unaffected!"),
			_("���������Ϥ�ķ���֤�����", "You resist the effects!"),
			resist, saving_throw, TARGET_TYPE);

		if (!resist && !saving_throw)
		{
			teleport_level(0);
		}
		learn_spell(MS_TELE_LEVEL);
		update_smart_learn(m_idx, DRS_NEXUS);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER)
	{
		resist = tr_ptr->flagsr & (RFR_EFF_RES_NEXU_MASK | RFR_RES_TELE);
		saving_throw = (tr_ptr->flags1 & RF1_QUESTOR) ||
					   (tr_ptr->level > randint1((rlev - 10) < 1 ? 1 : (rlev - 10)) + 10);

		spell_badstatus_message(m_idx, t_idx, 
			_("%^s��%s��­��ؤ�������", "%^s gestures at %s's feet."),
			_("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected!"),
			_("%^s�ϸ��Ϥ�ķ���֤�����", "%^s resist the effects!"),
			"",
			resist, saving_throw, TARGET_TYPE);

		if (!resist && !saving_throw)
		{
			teleport_level((t_idx == p_ptr->riding) ? 0 : t_idx);
		}
	}
}

/*!
* @brief RF6_PSY_SPEAR�ν��������η��� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ���᡼���̤��֤���
*/
HIT_POINT spell_RF6_PSY_SPEAR(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	HIT_POINT dam;

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s�����η������ä���", "%^s throw a Psycho-Spear."),
		_("%^s��%s�˸����äƸ��η������ä���", "%^s throw a Psycho-spear at %s."),
		TARGET_TYPE);

	dam = monspell_damage((MS_PSY_SPEAR), m_idx, DAM_ROLL);
	beam(m_idx, y, x, GF_PSY_SPEAR, dam, MS_PSY_SPEAR, MONSTER_TO_PLAYER);
	return dam;
}

/*!
* @brief RF6_DARKNESS�ν������Ű�or������ /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
*/
void spell_RF6_DARKNESS(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	monster_type	*m_ptr = &m_list[m_idx];
	monster_type	*t_ptr = &m_list[t_idx];
	monster_race	*r_ptr = &r_info[m_ptr->r_idx];
	bool can_use_lite_area = FALSE;
	bool monster_to_monster = TARGET_TYPE == MONSTER_TO_MONSTER;
	bool monster_to_player = TARGET_TYPE == MONSTER_TO_PLAYER;
	char t_name[80];
	monster_name(t_idx, t_name);

	if ((p_ptr->pclass == CLASS_NINJA) &&
		!(r_ptr->flags3 & (RF3_UNDEAD | RF3_HURT_LITE)) &&
		!(r_ptr->flags7 & RF7_DARK_MASK))
		can_use_lite_area = TRUE;

	if(monster_to_monster && !is_hostile(t_ptr))
		can_use_lite_area = FALSE;

	
	if (can_use_lite_area)
	{
		monspell_message(m_idx, t_idx,
			_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
			_("%^s���դ�����뤯�Ȥ餷����", "%^s cast a spell to light up."),
			_("%^s���դ�����뤯�Ȥ餷����", "%^s cast a spell to light up."),
			TARGET_TYPE);

		if (see_monster(t_idx) && monster_to_monster)
		{
			msg_format(_("%^s���򤤸�����ޤ줿��", "%^s is surrounded by a white light."), t_name);
		}
	}
	else
	{
		monspell_message(m_idx, t_idx,
			_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
			_("%^s���ŰǤ���Ǽ�򿶤ä���", "%^s gestures in shadow."),
			_("%^s���ŰǤ���Ǽ�򿶤ä���", "%^s gestures in shadow."),
			TARGET_TYPE);

		if (see_monster(t_idx) && monster_to_monster)
		{
			msg_format(_("%^s�ϰŰǤ���ޤ줿��", "%^s is surrounded by darkness."), t_name);
		}
	}

	if(monster_to_player)
	{
		if (can_use_lite_area)
		{
			(void)lite_area(0, 3);
		}
		else
		{
			learn_spell(MS_DARKNESS);
			(void)unlite_area(0, 3);
		}
	}
	else if(monster_to_monster)
	{
		if (can_use_lite_area)
		{
			(void)project(m_idx, 3, y, x, 0, GF_LITE_WEAK, PROJECT_GRID | PROJECT_KILL, -1);
			lite_room(y, x);
		}
		else
		{
			(void)project(m_idx, 3, y, x, 0, GF_DARK_WEAK, PROJECT_GRID | PROJECT_KILL, MS_DARKNESS);
			unlite_room(y, x);
		}
	}
}

/*!
* @brief RF6_TRAPS�ν������ȥ�åס� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
*/
void spell_RF6_TRAPS(POSITION y, POSITION x, MONSTER_IDX m_idx)
{
	char m_name[80];
	monster_name(m_idx, m_name);
	disturb(1, 1);

	if (p_ptr->blind)
		msg_format(_("%^s��������Ĥ֤䤤�Ƽٰ������Ф����",
		"%^s mumbles, and then cackles evilly."), m_name);
	else
		msg_format(_("%^s����ʸ�򾧤��Ƽٰ������Ф����",
		"%^s casts a spell and cackles evilly."), m_name);

	learn_spell(MS_MAKE_TRAP);
	(void)trap_creation(y, x);
}

/*!
* @brief RF6_FORGET�ν����������õ /
* @param m_idx ��ʸ�򾧤����󥹥���ID
*/
void spell_RF6_FORGET(MONSTER_IDX m_idx)
{
	int rlev = monster_level_idx(m_idx);
	char m_name[80];
	monster_name(m_idx, m_name);

	disturb(1, 1);

	msg_format(_("%^s�����ʤ��ε�����õ�褦�Ȥ��Ƥ��롣",
		"%^s tries to blank your mind."), m_name);

	if (randint0(100 + rlev / 2) < p_ptr->skill_sav)
	{
		msg_print(_("���������Ϥ�ķ���֤�����", "You resist the effects!"));
	}
	else if (lose_all_info())
	{
		msg_print(_("����������Ƥ��ޤä���", "Your memories fade away."));
	}
	learn_spell(MS_FORGET);
}


/*!
* @brief RF6_RAISE_DEAD�ν�����������衣 /
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
*/
void spell_RF6_RAISE_DEAD(MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	monster_type	*m_ptr = &m_list[m_idx];

	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s���������μ�ʸ�򾧤�����", "%^s casts a spell to revive corpses."),
		_("%^s���������μ�ʸ�򾧤�����", "%^s casts a spell to revive corpses."),
		TARGET_TYPE);

	animate_dead(m_idx, m_ptr->fy, m_ptr->fx);
}


/*!
* @brief �뾤���ν����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param rlev ��ʸ�򾧤����󥹥����Υ�٥�
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @return ����������󥹥����ο����֤���
*/
MONSTER_NUMBER summon_EAGLE(POSITION y, POSITION x, int rlev, MONSTER_IDX m_idx)
{
	int k, count = 0;	
	int num = 4 + randint1(3);
	for (k = 0; k < num; k++)
	{
		count += summon_specific(m_idx, y, x, rlev, SUMMON_EAGLES, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
	}
	return count;
}

/*!
 * @brief ���󥿡��ͥåȡ��������ץ����������ν����� /
 * @param y �оݤ�������y��ɸ
 * @param x �оݤ�������x��ɸ
 * @param rlev ��ʸ�򾧤����󥹥����Υ�٥�
 * @param m_idx ��ʸ�򾧤����󥹥���ID
 * @return ����������󥹥����ο����֤���
 */
MONSTER_NUMBER summon_IE(POSITION y, POSITION x, int rlev, MONSTER_IDX m_idx)
{
	BIT_FLAGS mode = 0L;
	int k, count = 0;	
	int num = 2 + randint1(1 + rlev / 20);
	for (k = 0; k < num; k++)
	{
		count += summon_named_creature(m_idx, y, x, MON_IE, mode);
	}
	return count;
}

/*!
 * @brief ���󥸥�󡦥����ǥ����󾤴��ν����� /
 * @param y �оݤ�������y��ɸ
 * @param x �оݤ�������x��ɸ
 * @param rlev ��ʸ�򾧤����󥹥����Υ�٥�
 * @param m_idx ��ʸ�򾧤����󥹥���ID
 * @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
 * @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
 * @return ����������󥹥����ο����֤���
 */
MONSTER_NUMBER summon_Guardian(POSITION y, POSITION x, int rlev, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	int k, count = 0;	
	int num = 2 + randint1(3);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);

	if (r_info[MON_JORMUNGAND].cur_num < r_info[MON_JORMUNGAND].max_num && one_in_(6))
	{
		simple_monspell_message(m_idx, t_idx,
			_("���̤���夬�᤭�Ф�����", "Water blew off from the ground!"),
			_("���̤���夬�᤭�Ф�����", "Water blew off from the ground!"),
			TARGET_TYPE);

		if(mon_to_player)
			fire_ball_hide(GF_WATER_FLOW, 0, 3, 8);
		else if(mon_to_mon)
			project(t_idx, 8, y, x, 3, GF_WATER_FLOW, PROJECT_GRID | PROJECT_HIDE, -1);
	}

	for (k = 0; k < num; k++)
	{
		count += summon_specific(m_idx, y, x, rlev, SUMMON_GUARDIANS, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
	}
	return count;
}

/*!
* @brief ��å��Υ����󾤴��ν����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param rlev ��ʸ�򾧤����󥹥����Υ�٥�
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @return ����������󥹥����ο����֤���
*/
MONSTER_NUMBER summon_LOCK_CLONE(POSITION y, POSITION x, DEPTH rlev, MONSTER_IDX m_idx)
{
	BIT_FLAGS mode = 0L;
	int k, count = 0;
	int num = randint1(3);
	rlev = rlev;
	for (k = 0; k < num; k++)
	{
		count += summon_named_creature(m_idx, y, x, MON_LOCKE_CLONE, mode);
	}
	return count;
}

/*!
* @brief ����߾����ν����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param rlev ��ʸ�򾧤����󥹥����Υ�٥�
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @return ����������󥹥����ο����֤���
*/
MONSTER_NUMBER summon_LOUSE(POSITION y, POSITION x, int rlev, MONSTER_IDX m_idx)
{
	int k, count = 0;	
	int num = 2 + randint1(3);
	for (k = 0; k < num; k++)
	{
		count += summon_specific(m_idx, y, x, rlev, SUMMON_LOUSE, PM_ALLOW_GROUP);
	}
	return count;
}

/*!
* @brief �߱群�����̾������Ʊ����ܥ�Υ�󥹥����򾤴����롣 /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param rlev ��ʸ�򾧤����󥹥����Υ�٥�
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @return ����������󥹥����ο����֤���
*/
MONSTER_NUMBER summon_Kin(POSITION y, POSITION x, int rlev, MONSTER_IDX m_idx)
{
	int k, count = 0;
	monster_type	*m_ptr = &m_list[m_idx];
	monster_race	*r_ptr = &r_info[m_ptr->r_idx];
	summon_kin_type = r_ptr->d_char; /* Big hack */

	for (k = 0; k < 4; k++)
	{
		count += summon_specific(m_idx, y, x, rlev, SUMMON_KIN, PM_ALLOW_GROUP);
	}
	return count;
}

/*!
* @brief RF6_S_KIN�ν������߱群�������Ѥ����󥹥����μ���ˤ�ꡢ�½�����ʬ�������롣 /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ����������󥹥����ο����֤���
*/
void spell_RF6_S_KIN(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	bool known = monster_near_player(m_idx, t_idx);
	bool see_either = see_monster(m_idx) || see_monster(t_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	monster_type	*m_ptr = &m_list[m_idx];
	monster_race	*r_ptr = &r_info[m_ptr->r_idx];
	int rlev = monster_level_idx(m_idx);
	int count = 0;
	char m_name[80], t_name[80], m_poss[80];
	monster_name(m_idx, m_name);
	monster_name(t_idx, t_name);
	monster_desc(m_poss, m_ptr, MD_PRON_VISIBLE | MD_POSSESSIVE);

	disturb(1, 1);
	if (m_ptr->r_idx == MON_SERPENT || m_ptr->r_idx == MON_ZOMBI_SERPENT)
	{
		monspell_message(m_idx, t_idx,
			_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
			_("%^s�����󥸥��μ�򾤴�������", "%^s magically summons guardians of dungeons."),
			_("%^s�����󥸥��μ�򾤴�������", "%^s magically summons guardians of dungeons."),
			TARGET_TYPE);
	}
	else
	{
		if (mon_to_player || (mon_to_mon && known && see_either))
			disturb(1, 1);

		if (p_ptr->blind)
		{
			if (mon_to_player)
				msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
		}
		else
		{
			if (mon_to_player || (mon_to_mon && known && see_either))
			{
				_(msg_format("%s����ˡ��%s�򾤴�������", m_name, ((r_ptr->flags1 & RF1_UNIQUE) ? "�겼" : "���")),
				  msg_format("%^s magically summons %s %s.", m_name, m_poss, ((r_ptr->flags1 & RF1_UNIQUE) ? "minions" : "kin")));
			}
		}

		if (mon_to_mon && known && !see_either)
			mon_fight = TRUE;
	}

	switch (m_ptr->r_idx)
	{
		case MON_MENELDOR:
		case MON_GWAIHIR:
		case MON_THORONDOR:
			count += summon_EAGLE(y, x, rlev, m_idx);
			break;

		case MON_BULLGATES:
			count += summon_IE(y, x, rlev, m_idx);
			break;

		case MON_SERPENT:
		case MON_ZOMBI_SERPENT:
			count += summon_Guardian(y, x, rlev, m_idx, t_idx, TARGET_TYPE);
			break;
			
		case MON_CALDARM:
			count += summon_LOCK_CLONE(y, x, rlev, m_idx);
			break;

		case MON_LOUSY:
			count += summon_LOUSE(y, x, rlev, m_idx);
			break;

		default:
			count += summon_Kin(y, x, rlev, m_idx);
			break;
	}
	
	if (p_ptr->blind && count && mon_to_player)
		msg_print(_("¿���Τ�Τ��ֶ�˸��줿�������롣", "You hear many things appear nearby."));

	if (known && !see_monster(t_idx) && count && mon_to_mon)
		mon_fight = TRUE;
}

/*!
* @brief RF6_S_CYBER�ν����������С����ǡ���󾤴��� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ����������󥹥����ο����֤���
*/
void spell_RF6_S_CYBER(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	int count = 0;
	monster_type	*m_ptr = &m_list[m_idx];
	int rlev = monster_level_idx(m_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	
	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s�������С��ǡ����򾤴�������", "%^s magically summons Cyberdemons!"),
		_("%^s�������С��ǡ����򾤴�������", "%^s magically summons Cyberdemons!"),
		TARGET_TYPE);

	if (is_friendly(m_ptr) && mon_to_mon)
	{
		count += summon_specific(m_idx, y, x, rlev, SUMMON_CYBER, (PM_ALLOW_GROUP));
	}
	else
	{
		count += summon_cyber(m_idx, y, x);
	}

	if (p_ptr->blind && count && mon_to_player)
		msg_print(_("�Ÿ���­�����᤯��ʹ�����롣", "You hear heavy steps nearby."));
	
	if (monster_near_player(m_idx, t_idx) && !see_monster(t_idx) && count && mon_to_mon)
		mon_fight = TRUE;
}

/*!
* @brief RF6_S_MONSTER�ν�������󥹥������ξ����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ����������󥹥����ο����֤���
*/
void spell_RF6_S_MONSTER(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	int count = 0, k;
	int rlev = monster_level_idx(m_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	
	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s����ˡ����֤򾤴�������", "%^s magically summons help!"),
		_("%^s����ˡ����֤򾤴�������", "%^s magically summons help!"),
		TARGET_TYPE);

	for (k = 0; k < 1; k++)
	{
		if(mon_to_player)
			count += summon_specific(m_idx, y, x, rlev, 0, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));

		if(mon_to_mon)
			count += summon_specific(m_idx, y, x, rlev, 0, (monster_u_mode(m_idx)));
	}

	if (p_ptr->blind && count && mon_to_player)
		msg_print(_("�������ֶ�˸��줿�������롣", "You hear something appear nearby."));
	
	if (monster_near_player(m_idx, t_idx) && !see_monster(t_idx) && count && mon_to_mon)
		mon_fight = TRUE;
}

/*!
* @brief RF6_S_MONSTERS�ν�������󥹥���ʣ�������� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ����������󥹥����ο����֤���
*/
void spell_RF6_S_MONSTERS(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	int count = 0, k;
	int rlev = monster_level_idx(m_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	
	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s����ˡ�ǥ�󥹥����򾤴�������", "%^s magically summons monsters!"),
		_("%^s����ˡ�ǥ�󥹥����򾤴�������", "%^s magically summons monsters!"),
		TARGET_TYPE);
	
	for (k = 0; k < S_NUM_6; k++)
	{
		if(mon_to_player)
			count += summon_specific(m_idx, y, x, rlev, 0, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));

		if(mon_to_mon)
			count += summon_specific(m_idx, y, x, rlev, 0, (PM_ALLOW_GROUP | monster_u_mode(m_idx)));
	}

	if (p_ptr->blind && count && mon_to_player)
		msg_print(_("¿���Τ�Τ��ֶ�˸��줿�������롣", "You hear many things appear nearby."));
	
	if (monster_near_player(m_idx, t_idx) && !see_monster(t_idx) && count && mon_to_mon)
		mon_fight = TRUE;
}

/*!
* @brief RF6_S_ANT�ν��������꾤���� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ����������󥹥����ο����֤���
*/
void spell_RF6_S_ANT(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	int count = 0, k;
	int rlev = monster_level_idx(m_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	
	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s����ˡ�ǥ���򾤴�������", "%^s magically summons ants."),
		_("%^s����ˡ�ǥ���򾤴�������", "%^s magically summons ants."),
		TARGET_TYPE);
	
	for (k = 0; k < S_NUM_6; k++)
	{
		count += summon_specific(m_idx, y, x, rlev, SUMMON_ANT, PM_ALLOW_GROUP);
	}

	if (p_ptr->blind && count && mon_to_player)
		msg_print(_("¿���Τ�Τ��ֶ�˸��줿�������롣", "You hear many things appear nearby."));
	
	if (monster_near_player(m_idx, t_idx) && !see_monster(t_idx) && count && mon_to_mon)
		mon_fight = TRUE;
}

/*!
* @brief RF6_S_SPIDER�ν��������⾤���� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ����������󥹥����ο����֤���
*/
void spell_RF6_S_SPIDER(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	int count = 0, k;
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	int rlev = monster_level_idx(m_idx);
	
	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s����ˡ�ǥ���򾤴�������", "%^s magically summons spiders."),
		_("%^s����ˡ�ǥ���򾤴�������", "%^s magically summons spiders."),
		TARGET_TYPE);
	
	for (k = 0; k < S_NUM_6; k++)
	{
		count += summon_specific(m_idx, y, x, rlev, SUMMON_SPIDER, PM_ALLOW_GROUP);
	}

	if (p_ptr->blind && count && mon_to_player)
		msg_print(_("¿���Τ�Τ��ֶ�˸��줿�������롣", "You hear many things appear nearby."));
	
	if (monster_near_player(m_idx, t_idx) && !see_monster(t_idx) && count && mon_to_mon)
		mon_fight = TRUE;
}

/*!
* @brief RF6_S_HOUND�ν������ϥ���ɾ����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ����������󥹥����ο����֤���
*/
void spell_RF6_S_HOUND(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	int count = 0, k;
	int rlev = monster_level_idx(m_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	
	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s����ˡ�ǥϥ���ɤ򾤴�������", "%^s magically summons hounds."),
		_("%^s����ˡ�ǥϥ���ɤ򾤴�������", "%^s magically summons hounds."),
		TARGET_TYPE);
	
	for (k = 0; k < S_NUM_4; k++)
	{
		count += summon_specific(m_idx, y, x, rlev, SUMMON_HOUND, PM_ALLOW_GROUP);
	}

	if (p_ptr->blind && count && mon_to_player)
		msg_print(_("¿���Τ�Τ��ֶ�˸��줿�������롣", "You hear many things appear nearby."));
	
	if (monster_near_player(m_idx, t_idx) && !see_monster(t_idx) && count && mon_to_mon)
		mon_fight = TRUE;
}

/*!
* @brief RF6_S_HYDRA�ν������ҥɥ龤���� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ����������󥹥����ο����֤���
*/
void spell_RF6_S_HYDRA(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	int count = 0, k;
	int rlev = monster_level_idx(m_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	
	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s����ˡ�ǥҥɥ�򾤴�������", "%^s magically summons hydras."),
		_("%^s����ˡ�ǥҥɥ�򾤴�������", "%^s magically summons hydras."),
		TARGET_TYPE);
	
	for (k = 0; k < S_NUM_4; k++)
	{
		count += summon_specific(m_idx, y, x, rlev, SUMMON_HYDRA, PM_ALLOW_GROUP);
	}

	if (p_ptr->blind && count && mon_to_player)
		msg_print(_("¿���Τ�Τ��ֶ�˸��줿�������롣", "You hear many things appear nearby."));
	
	if (monster_near_player(m_idx, t_idx) && !see_monster(t_idx) && count && mon_to_mon)
		mon_fight = TRUE;
}

/*!
* @brief RF6_S_ANGEL�ν�����ŷ�Ȱ��ξ����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ����������󥹥����ο����֤���
*/
void spell_RF6_S_ANGEL(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	int count = 0, k;
	int num = 1;
	monster_type	*m_ptr = &m_list[m_idx];
	monster_race	*r_ptr = &r_info[m_ptr->r_idx];
	int rlev = monster_level_idx(m_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	
	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s����ˡ��ŷ�Ȥ򾤴�������", "%^s magically summons an angel!"),
		_("%^s����ˡ��ŷ�Ȥ򾤴�������", "%^s magically summons an angel!"),
		TARGET_TYPE);
	
	if ((r_ptr->flags1 & RF1_UNIQUE) && !easy_band)
	{
		num += r_ptr->level / 40;
	}
	
	for (k = 0; k < num; k++)
	{
		count += summon_specific(m_idx, y, x, rlev, SUMMON_ANGEL, PM_ALLOW_GROUP);
	}
	
	if (count < 2)
	{
		if (p_ptr->blind && count)
			msg_print(_("�������ֶ�˸��줿�������롣", "You hear something appear nearby."));
	}
	else
	{
		if (p_ptr->blind)
			msg_print(_("¿���Τ�Τ��ֶ�˸��줿�������롣", "You hear many things appear nearby."));
	}
	
	if (monster_near_player(m_idx, t_idx) && !see_monster(t_idx) && count && mon_to_mon)
		mon_fight = TRUE;
}

/*!
* @brief RF6_S_DEMON�ν������ǡ������ξ����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ����������󥹥����ο����֤���
*/
void spell_RF6_S_DEMON(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	int count = 0, k;
	int rlev = monster_level_idx(m_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	
	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s����ˡ�Ǻ��٤ε���鰭��򾤴�������", "%^s magically summons a demon from the Courts of Chaos!"),
		_("%^s����ˡ�Ǻ��٤ε���鰭��򾤴�������", "%^s magically summons a demon from the Courts of Chaos!"),
		TARGET_TYPE);
	
	for (k = 0; k < 1; k++)
	{
		count += summon_specific(m_idx, y, x, rlev, SUMMON_DEMON, PM_ALLOW_GROUP);
	}
	
	if (p_ptr->blind && count)
		msg_print(_("�������ֶ�˸��줿�������롣", "You hear something appear nearby."));
	
	if (monster_near_player(m_idx, t_idx) && !see_monster(t_idx) && count && mon_to_mon)
		mon_fight = TRUE;
}

/*!
* @brief RF6_S_UNDEAD�ν���������ǥåɰ��ξ����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ����������󥹥����ο����֤���
*/
void spell_RF6_S_UNDEAD(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	int count = 0, k;
	int rlev = monster_level_idx(m_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	
	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s����ˡ�ǥ���ǥåɤζ�Ũ�򾤴�������", "%^s magically summons an undead adversary!"),
		_("%s����ˡ�ǥ���ǥåɤ򾤴�������", "%^s magically summons undead."),
		TARGET_TYPE);
	
	for (k = 0; k < 1; k++)
	{
		count += summon_specific(m_idx, y, x, rlev, SUMMON_UNDEAD, PM_ALLOW_GROUP);
	}
	
	if (p_ptr->blind && count)
		msg_print(_("�������ֶ�˸��줿�������롣", "You hear something appear nearby."));
	
	if (monster_near_player(m_idx, t_idx) && !see_monster(t_idx) && count && mon_to_mon)
		mon_fight = TRUE;
}

/*!
* @brief RF6_S_DRAGON�ν������ɥ饴����ξ����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ����������󥹥����ο����֤���
*/
void spell_RF6_S_DRAGON(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	int count = 0, k;
	int rlev = monster_level_idx(m_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	
	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s����ˡ�ǥɥ饴��򾤴�������", "%^s magically summons a dragon!"),
		_("%^s����ˡ�ǥɥ饴��򾤴�������", "%^s magically summons a dragon!"),
		TARGET_TYPE);
	
	for (k = 0; k < 1; k++)
	{
		count += summon_specific(m_idx, y, x, rlev, SUMMON_DRAGON, PM_ALLOW_GROUP);
	}
	
	if (p_ptr->blind && count)
		msg_print(_("�������ֶ�˸��줿�������롣", "You hear something appear nearby."));
	
	if (monster_near_player(m_idx, t_idx) && !see_monster(t_idx) && count && mon_to_mon)
		mon_fight = TRUE;
}

/*!
* @brief �ʥ��������⾤���ν����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @return ����������󥹥����ο����֤���
*/
MONSTER_NUMBER summon_NAZGUL(POSITION y, POSITION x, MONSTER_IDX m_idx)
{
	BIT_FLAGS mode = 0L;
	int count = 0, k;
	POSITION cy = y;
	POSITION cx = x;
	char m_name[80];
	monster_name(m_idx, m_name);

	if (p_ptr->blind)
		msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
	else
		msg_format(_("%^s����ˡ��ͩ������򾤴�������", "%^s magically summons rangers of Nazgul!"), m_name);

	msg_print(NULL);

	for (k = 0; k < 30; k++)
	{
		if (!summon_possible(cy, cx) || !cave_empty_bold(cy, cx))
		{
			int j;
			for (j = 100; j > 0; j--)
			{
				scatter(&cy, &cx, y, x, 2, 0);
				if (cave_empty_bold(cy, cx)) break;
			}
			if (!j) break;
		}
		if (!cave_empty_bold(cy, cx)) continue;

		if (summon_named_creature(m_idx, cy, cx, MON_NAZGUL, mode))
		{
			y = cy;
			x = cx;
			count++;
			if (count == 1)
				msg_format(_("��ͩ������%d�桢�ʥ����롦�֥�å�����",
				"A Nazgul says 'Nazgul-Rangers Number %d, Nazgul-Black!'"), count);
			else
				msg_format(_("��Ʊ����%d�桢�ʥ����롦�֥�å�����",
				"Another one says 'Number %d, Nazgul-Black!'"), count);

			msg_print(NULL);
		}
	}
	msg_format(_("��%d�ͤ���äơ���󥰥�󥸥㡼����",
		"They say 'The %d meets! We are the Ring-Ranger!'."), count);
	msg_print(NULL);
	return count;
}

/*!
* @brief RF6_S_HI_UNDEAD�ν��������Ϥʥ���ǥåɾ����� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ����������󥹥����ο����֤���
*/
void spell_RF6_S_HI_UNDEAD(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	monster_type	*m_ptr = &m_list[m_idx];
	int rlev = monster_level_idx(m_idx);
	int k, count = 0;
	char m_name[80];
	monster_name(m_idx, m_name);

	disturb(1, 1);

	if (((m_ptr->r_idx == MON_MORGOTH) || (m_ptr->r_idx == MON_SAURON) || (m_ptr->r_idx == MON_ANGMAR)) &&
		((r_info[MON_NAZGUL].cur_num + 2) < r_info[MON_NAZGUL].max_num) &&
		mon_to_player)
	{
		count +=  summon_NAZGUL(y, x, m_idx);
	}
	else
	{	
		monspell_message(m_idx, t_idx,
			_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
			_("%^s����ˡ�Ƕ��Ϥʥ���ǥåɤ򾤴�������", "%^s magically summons greater undead!"),
			_("%s����ˡ�ǥ���ǥåɤ򾤴�������", "%^s magically summons undead."),
			TARGET_TYPE);

		for (k = 0; k < S_NUM_6; k++)
		{
			if(mon_to_player)
				count += summon_specific(m_idx, y, x, rlev, SUMMON_HI_UNDEAD, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));

			if(mon_to_mon)
				count += summon_specific(m_idx, y, x, rlev, SUMMON_HI_UNDEAD, (PM_ALLOW_GROUP | monster_u_mode(m_idx)));
		}
	}
	if (p_ptr->blind && count && mon_to_player)
	{
		msg_print(_("�ֶ�ǲ���¿���Τ�Τ��礤��벻��ʹ�����롣", "You hear many creepy things appear nearby."));
	}
	
	if (monster_near_player(m_idx, t_idx) && !see_monster(t_idx) && count && mon_to_mon)
		mon_fight = TRUE;
}

/*!
* @brief RF6_S_HI_DRAGON�ν���������ɥ饴�󾤴��� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ����������󥹥����ο����֤���
*/
void spell_RF6_S_HI_DRAGON(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	int count = 0, k;
	int rlev = monster_level_idx(m_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	
	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s����ˡ�Ǹ���ɥ饴��򾤴�������", "%^s magically summons ancient dragons!"),
		_("%^s����ˡ�Ǹ���ɥ饴��򾤴�������", "%^s magically summons ancient dragons!"),
		TARGET_TYPE);
	
	for (k = 0; k < S_NUM_4; k++)
	{	
		if(mon_to_player)
			count += summon_specific(m_idx, y, x, rlev, SUMMON_HI_DRAGON, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));

		if(mon_to_mon)
			count += summon_specific(m_idx, y, x, rlev, SUMMON_HI_DRAGON, (PM_ALLOW_GROUP | monster_u_mode(m_idx)));
	}
	
	if (p_ptr->blind && count && mon_to_player)
	{
		msg_print(_("¿�����϶�����Τ��ֶ�˸��줿����ʹ�����롣", "You hear many powerful things appear nearby."));
	}
	
	if (monster_near_player(m_idx, t_idx) && !see_monster(t_idx) && count && mon_to_mon)
		mon_fight = TRUE;
}

/*!
* @brief RF6_S_AMBERITES�ν���������С��β�²������ /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ����������󥹥����ο����֤���
*/
void spell_RF6_S_AMBERITES(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	int count = 0, k;
	int rlev = monster_level_idx(m_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	
	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s������С��β�²�򾤴�������", "%^s magically summons Lords of Amber!"),
		_("%^s������С��β�²�򾤴�������", "%^s magically summons Lords of Amber!"),
		TARGET_TYPE);
	
	for (k = 0; k < S_NUM_4; k++)
	{	
		count += summon_specific(m_idx, y, x, rlev, SUMMON_AMBERITES, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
	}
	
	if (p_ptr->blind && count && mon_to_player)
	{
		msg_print(_("�Ի�μԤ��᤯�˸����Τ�ʹ��������", "You hear immortal beings appear nearby."));
	}
	
	if (monster_near_player(m_idx, t_idx) && !see_monster(t_idx) && count && mon_to_mon)
		mon_fight = TRUE;
}

/*!
* @brief RF6_S_UNIQUE�ν�������ˡ�������󥹥��������� /
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @param TARGET_TYPE �ץ쥤�䡼���оݤȤ�����MONSTER_TO_PLAYER����󥹥������оݤȤ�����MONSTER_TO_MONSTER
* @return ����������󥹥����ο����֤���
*/
void spell_RF6_S_UNIQUE(POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx, int TARGET_TYPE)
{
	int count = 0, k;
	monster_type	*m_ptr = &m_list[m_idx];
	int rlev = monster_level_idx(m_idx);
	bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
	bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	bool uniques_are_summoned = FALSE;
	int non_unique_type = SUMMON_HI_UNDEAD;
	
	monspell_message(m_idx, t_idx,
		_("%^s��������Ĥ֤䤤����", "%^s mumbles."),
		_("%^s����ˡ�����̤ʶ�Ũ�򾤴�������", "%^s magically summons special opponents!"),
		_("%^s����ˡ�����̤ʶ�Ũ�򾤴�������", "%^s magically summons special opponents!"),
		TARGET_TYPE);
	
	for (k = 0; k < S_NUM_4; k++)
	{	
		count += summon_specific(m_idx, y, x, rlev, SUMMON_UNIQUE, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
	}
	
	if (count) uniques_are_summoned = TRUE;

	if ((m_ptr->sub_align & (SUB_ALIGN_GOOD | SUB_ALIGN_EVIL)) == (SUB_ALIGN_GOOD | SUB_ALIGN_EVIL))
		non_unique_type = 0;
	else if (m_ptr->sub_align & SUB_ALIGN_GOOD)
		non_unique_type = SUMMON_ANGEL;

	for (k = count; k < S_NUM_4; k++)
	{
		count += summon_specific(m_idx, y, x, rlev, non_unique_type, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
	}

	if (p_ptr->blind && count && mon_to_player)
	{
		msg_format(_("¿����%s���ֶ�˸��줿����ʹ�����롣", "You hear many %s appear nearby."),
			uniques_are_summoned ? _("�϶������", "powerful things") : _("���", "things"));
	}
	
	if (monster_near_player(m_idx, t_idx) && !see_monster(t_idx) && count && mon_to_mon)
		mon_fight = TRUE;
}



/*!
* @brief ��󥹥�������ץ쥤�䡼�ؤμ�ʸ�ο���ʬ���ؿ��� /
* @param SPELL_NUM ��󥹥�����ˡID
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @return �����ʸ�Υ��᡼�����ޤ��Ͼ���������󥹥����ο����֤�������¾�ξ��0���ʹߤν��������Ǥ���ʤ�-1���֤���
*/
HIT_POINT monspell_to_player(int SPELL_NUM, POSITION y, POSITION x, MONSTER_IDX m_idx)
{
	switch (SPELL_NUM)
	{
	case RF4_SPELL_START + 0:   spell_RF4_SHRIEK(m_idx, 0, MONSTER_TO_PLAYER); break;	/* RF4_SHRIEK */
	case RF4_SPELL_START + 1:   break;   /* RF4_XXX1 */
	case RF4_SPELL_START + 2:   spell_RF4_DISPEL(m_idx, 0, MONSTER_TO_PLAYER); break;	/* RF4_DISPEL */
	case RF4_SPELL_START + 3:   return spell_RF4_ROCKET(y, x, m_idx, 0, MONSTER_TO_PLAYER);  /* RF4_ROCKET */
	case RF4_SPELL_START + 4:   return spell_RF4_SHOOT(y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF4_SHOOT */
	case RF4_SPELL_START + 5:   break;   /* RF4_XXX2 */
	case RF4_SPELL_START + 6:   break;   /* RF4_XXX3 */
	case RF4_SPELL_START + 7:   break;   /* RF4_XXX4 */
	case RF4_SPELL_START + 8:   return spell_RF4_BREATH(GF_ACID, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_ACID */
	case RF4_SPELL_START + 9:   return spell_RF4_BREATH(GF_ELEC, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_ELEC */
	case RF4_SPELL_START + 10:  return spell_RF4_BREATH(GF_FIRE, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_FIRE */
	case RF4_SPELL_START + 11:  return spell_RF4_BREATH(GF_COLD, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_COLD */
	case RF4_SPELL_START + 12:  return spell_RF4_BREATH(GF_POIS, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_POIS */
	case RF4_SPELL_START + 13:  return spell_RF4_BREATH(GF_NETHER, y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF4_BR_NETH */
	case RF4_SPELL_START + 14:  return spell_RF4_BREATH(GF_LITE, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_LITE */
	case RF4_SPELL_START + 15:  return spell_RF4_BREATH(GF_DARK, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_DARK */
	case RF4_SPELL_START + 16:  return spell_RF4_BREATH(GF_CONFUSION, y, x, m_idx, 0, MONSTER_TO_PLAYER);	/* RF4_BR_CONF */
	case RF4_SPELL_START + 17:  return spell_RF4_BREATH(GF_SOUND, y, x, m_idx, 0, MONSTER_TO_PLAYER);	/* RF4_BR_SOUN */
	case RF4_SPELL_START + 18:  return spell_RF4_BREATH(GF_CHAOS, y, x, m_idx, 0, MONSTER_TO_PLAYER);	/* RF4_BR_CHAO */
	case RF4_SPELL_START + 19:  return spell_RF4_BREATH(GF_DISENCHANT, y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF4_BR_DISE */
	case RF4_SPELL_START + 20:  return spell_RF4_BREATH(GF_NEXUS, y, x, m_idx, 0, MONSTER_TO_PLAYER);	/* RF4_BR_NEXU */
	case RF4_SPELL_START + 21:  return spell_RF4_BREATH(GF_TIME, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_TIME */
	case RF4_SPELL_START + 22:  return spell_RF4_BREATH(GF_INERTIAL, y, x, m_idx, 0, MONSTER_TO_PLAYER);  /* RF4_BR_INER */
	case RF4_SPELL_START + 23:  return spell_RF4_BREATH(GF_GRAVITY, y, x, m_idx, 0, MONSTER_TO_PLAYER);  /* RF4_BR_GRAV */
	case RF4_SPELL_START + 24:  return spell_RF4_BREATH(GF_SHARDS, y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF4_BR_SHAR */
	case RF4_SPELL_START + 25:  return spell_RF4_BREATH(GF_PLASMA, y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF4_BR_PLAS */
	case RF4_SPELL_START + 26:  return spell_RF4_BREATH(GF_FORCE, y, x, m_idx, 0, MONSTER_TO_PLAYER);	/* RF4_BR_WALL */
	case RF4_SPELL_START + 27:  return spell_RF4_BREATH(GF_MANA, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_MANA */
	case RF4_SPELL_START + 28:  return spell_RF4_BA_NUKE(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BA_NUKE */
	case RF4_SPELL_START + 29:  return spell_RF4_BREATH(GF_NUKE, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_NUKE */
	case RF4_SPELL_START + 30:  return spell_RF4_BA_CHAO(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BA_CHAO */
	case RF4_SPELL_START + 31:  return spell_RF4_BREATH(GF_DISINTEGRATE, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_DISI */
	case RF5_SPELL_START + 0:  return spell_RF5_BA_ACID(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_ACID */
	case RF5_SPELL_START + 1:  return spell_RF5_BA_ELEC(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_ELEC */
	case RF5_SPELL_START + 2:  return spell_RF5_BA_FIRE(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_FIRE */
	case RF5_SPELL_START + 3:  return spell_RF5_BA_COLD(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_COLD */
	case RF5_SPELL_START + 4:  return spell_RF5_BA_POIS(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_POIS */
	case RF5_SPELL_START + 5:  return spell_RF5_BA_NETH(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_NETH */
	case RF5_SPELL_START + 6:  return spell_RF5_BA_WATE(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_WATE */
	case RF5_SPELL_START + 7:  return spell_RF5_BA_MANA(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_MANA */
	case RF5_SPELL_START + 8:  return spell_RF5_BA_DARK(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_DARK */
	case RF5_SPELL_START + 9:  return spell_RF5_DRAIN_MANA(y, x, m_idx, 0, MONSTER_TO_PLAYER);  /* RF5_DRAIN_MANA */
	case RF5_SPELL_START + 10: return spell_RF5_MIND_BLAST(y, x, m_idx, 0, MONSTER_TO_PLAYER);  /* RF5_MIND_BLAST */
	case RF5_SPELL_START + 11: return spell_RF5_BRAIN_SMASH(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_MIND_BLAST */
	case RF5_SPELL_START + 12: return spell_RF5_CAUSE_1(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_CAUSE_1 */
	case RF5_SPELL_START + 13: return spell_RF5_CAUSE_2(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_CAUSE_2 */
	case RF5_SPELL_START + 14: return spell_RF5_CAUSE_3(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_CAUSE_3 */
	case RF5_SPELL_START + 15: return spell_RF5_CAUSE_4(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_CAUSE_4 */
	case RF5_SPELL_START + 16: return spell_RF5_BO_ACID(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_ACID */
	case RF5_SPELL_START + 17: return spell_RF5_BO_ELEC(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_ELEC */
	case RF5_SPELL_START + 18: return spell_RF5_BO_FIRE(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_FIRE */
	case RF5_SPELL_START + 19: return spell_RF5_BO_COLD(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_COLD */
	case RF5_SPELL_START + 20: return spell_RF5_BA_LITE(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_LITE */
	case RF5_SPELL_START + 21: return spell_RF5_BO_NETH(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_NETH */
	case RF5_SPELL_START + 22: return spell_RF5_BO_WATE(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_WATE */
	case RF5_SPELL_START + 23: return spell_RF5_BO_MANA(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_MANA */
	case RF5_SPELL_START + 24: return spell_RF5_BO_PLAS(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_PLAS */
	case RF5_SPELL_START + 25: return spell_RF5_BO_ICEE(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_ICEE */
	case RF5_SPELL_START + 26: return spell_RF5_MISSILE(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_MISSILE */
	case RF5_SPELL_START + 27: spell_RF5_SCARE(m_idx, 0, MONSTER_TO_PLAYER); break;   /* RF5_SCARE */
	case RF5_SPELL_START + 28: spell_RF5_BLIND(m_idx, 0, MONSTER_TO_PLAYER); break;   /* RF5_BLIND */
	case RF5_SPELL_START + 29: spell_RF5_CONF(m_idx, 0, MONSTER_TO_PLAYER); break;  /* RF5_CONF */
	case RF5_SPELL_START + 30: spell_RF5_SLOW(m_idx, 0, MONSTER_TO_PLAYER); break;  /* RF5_SLOW */
	case RF5_SPELL_START + 31: spell_RF5_HOLD(m_idx, 0, MONSTER_TO_PLAYER); break;  /* RF5_HOLD */
	case RF6_SPELL_START + 0:  spell_RF6_HASTE(m_idx, 0, MONSTER_TO_PLAYER); break;   /* RF6_HASTE */
	case RF6_SPELL_START + 1:  return spell_RF6_HAND_DOOM(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_HAND_DOOM */
	case RF6_SPELL_START + 2:  spell_RF6_HEAL(m_idx, 0, MONSTER_TO_PLAYER); break;	/* RF6_HEAL */
	case RF6_SPELL_START + 3:  spell_RF6_INVULNER(m_idx, 0, MONSTER_TO_PLAYER); break;	/* RF6_INVULNER */
	case RF6_SPELL_START + 4:  spell_RF6_BLINK(m_idx, MONSTER_TO_PLAYER); break;   /* RF6_BLINK */
	case RF6_SPELL_START + 5:  spell_RF6_TPORT(m_idx, MONSTER_TO_PLAYER); break;   /* RF6_TPORT */
	case RF6_SPELL_START + 6:  return spell_RF6_WORLD(m_idx); break;	/* RF6_WORLD */
	case RF6_SPELL_START + 7:  return spell_RF6_SPECIAL(y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF6_SPECIAL */
	case RF6_SPELL_START + 8:  spell_RF6_TELE_TO(m_idx, 0, MONSTER_TO_PLAYER); break; /* RF6_TELE_TO */
	case RF6_SPELL_START + 9:  spell_RF6_TELE_AWAY(m_idx, 0, MONSTER_TO_PLAYER); break;   /* RF6_TELE_AWAY */
	case RF6_SPELL_START + 10: spell_RF6_TELE_LEVEL(m_idx, 0, MONSTER_TO_PLAYER); break;  /* RF6_TELE_LEVEL */
	case RF6_SPELL_START + 11: spell_RF6_PSY_SPEAR(y, x, m_idx, 0, MONSTER_TO_PLAYER); break; /* RF6_PSY_SPEAR */
	case RF6_SPELL_START + 12: spell_RF6_DARKNESS(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;	/* RF6_DARKNESS */
	case RF6_SPELL_START + 13: spell_RF6_TRAPS(y, x, m_idx); break; /* RF6_TRAPS */
	case RF6_SPELL_START + 14: spell_RF6_FORGET(m_idx); break;  /* RF6_FORGET */
	case RF6_SPELL_START + 15: spell_RF6_RAISE_DEAD(m_idx, 0, MONSTER_TO_PLAYER); break;  /* RF6_RAISE_DEAD */
	case RF6_SPELL_START + 16: spell_RF6_S_KIN(y, x, m_idx, 0, MONSTER_TO_PLAYER); break; /* RF6_S_KIN */
	case RF6_SPELL_START + 17: spell_RF6_S_CYBER(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;   /* RF6_S_CYBER */
	case RF6_SPELL_START + 18: spell_RF6_S_MONSTER(y, x, m_idx, 0, MONSTER_TO_PLAYER); break; /* RF6_S_MONSTER */
	case RF6_SPELL_START + 19: spell_RF6_S_MONSTERS(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;	/* RF6_S_MONSTER */
	case RF6_SPELL_START + 20: spell_RF6_S_ANT(y, x, m_idx, 0, MONSTER_TO_PLAYER); break; /* RF6_S_ANT */
	case RF6_SPELL_START + 21: spell_RF6_S_SPIDER(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;  /* RF6_S_SPIDER */
	case RF6_SPELL_START + 22: spell_RF6_S_HOUND(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;   /* RF6_S_HOUND */
	case RF6_SPELL_START + 23: spell_RF6_S_HYDRA(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;   /* RF6_S_HYDRA */
	case RF6_SPELL_START + 24: spell_RF6_S_ANGEL(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;   /* RF6_S_ANGEL */
	case RF6_SPELL_START + 25: spell_RF6_S_DEMON(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;   /* RF6_S_DEMON */
	case RF6_SPELL_START + 26: spell_RF6_S_UNDEAD(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;  /* RF6_S_UNDEAD */
	case RF6_SPELL_START + 27: spell_RF6_S_DRAGON(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;  /* RF6_S_DRAGON */
	case RF6_SPELL_START + 28: spell_RF6_S_HI_UNDEAD(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;   /* RF6_S_HI_UNDEAD */
	case RF6_SPELL_START + 29: spell_RF6_S_HI_DRAGON(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;   /* RF6_S_HI_DRAGON */
	case RF6_SPELL_START + 30: spell_RF6_S_AMBERITES(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;   /* RF6_S_AMBERITES */
	case RF6_SPELL_START + 31: spell_RF6_S_UNIQUE(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;  /* RF6_S_UNIQUE */
	}
	return 0;
}

/*!
* @brief ��󥹥��������󥹥����ؤμ�ʸ�ο���ʬ���ؿ��� /
* @param SPELL_NUM ��󥹥�����ˡID
* @param y �оݤ�������y��ɸ
* @param x �оݤ�������x��ɸ
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param t_idx ��ʸ��������󥹥���ID���ץ쥤�䡼�ξ���dummy��0�Ȥ��롣
* @return �����ʸ�Υ��᡼�����ޤ��Ͼ���������󥹥����ο����֤�������¾�ξ��0���ʹߤν��������Ǥ���ʤ�-1���֤���
*/
HIT_POINT monspell_to_monster(int SPELL_NUM, POSITION y, POSITION x, MONSTER_IDX m_idx, IDX t_idx)
{
	switch (SPELL_NUM)
	{
	case RF4_SPELL_START + 0:   spell_RF4_SHRIEK(m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF4_SHRIEK */
	case RF4_SPELL_START + 1:   return -1;   /* RF4_XXX1 */
	case RF4_SPELL_START + 2:   spell_RF4_DISPEL(m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF4_DISPEL */
	case RF4_SPELL_START + 3:   return spell_RF4_ROCKET(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_ROCKET */
	case RF4_SPELL_START + 4:   return spell_RF4_SHOOT(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);  /* RF4_SHOOT */
	case RF4_SPELL_START + 5:   return -1;   /* RF4_XXX2 */
	case RF4_SPELL_START + 6:   return -1;   /* RF4_XXX3 */
	case RF4_SPELL_START + 7:   return -1;   /* RF4_XXX4 */
	case RF4_SPELL_START + 8:   return spell_RF4_BREATH(GF_ACID, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF4_BR_ACID */
	case RF4_SPELL_START + 9:   return spell_RF4_BREATH(GF_ELEC, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF4_BR_ELEC */
	case RF4_SPELL_START + 10:  return spell_RF4_BREATH(GF_FIRE, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF4_BR_FIRE */
	case RF4_SPELL_START + 11:  return spell_RF4_BREATH(GF_COLD, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF4_BR_COLD */
	case RF4_SPELL_START + 12:  return spell_RF4_BREATH(GF_POIS, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF4_BR_POIS */
	case RF4_SPELL_START + 13:  return spell_RF4_BREATH(GF_NETHER, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);  /* RF4_BR_NETH */
	case RF4_SPELL_START + 14:  return spell_RF4_BREATH(GF_LITE, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF4_BR_LITE */
	case RF4_SPELL_START + 15:  return spell_RF4_BREATH(GF_DARK, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF4_BR_DARK */
	case RF4_SPELL_START + 16:  return spell_RF4_BREATH(GF_CONFUSION, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF4_BR_CONF */
	case RF4_SPELL_START + 17:  return spell_RF4_BREATH(GF_SOUND, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF4_BR_SOUN */
	case RF4_SPELL_START + 18:  return spell_RF4_BREATH(GF_CHAOS, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF4_BR_CHAO */
	case RF4_SPELL_START + 19:  return spell_RF4_BREATH(GF_DISENCHANT, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);  /* RF4_BR_DISE */
	case RF4_SPELL_START + 20:  return spell_RF4_BREATH(GF_NEXUS, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF4_BR_NEXU */
	case RF4_SPELL_START + 21:  return spell_RF4_BREATH(GF_TIME, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF4_BR_TIME */
	case RF4_SPELL_START + 22:  return spell_RF4_BREATH(GF_INERTIAL, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_INER */
	case RF4_SPELL_START + 23:  return spell_RF4_BREATH(GF_GRAVITY, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_GRAV */
	case RF4_SPELL_START + 24:  return spell_RF4_BREATH(GF_SHARDS, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);  /* RF4_BR_SHAR */
	case RF4_SPELL_START + 25:  return spell_RF4_BREATH(GF_PLASMA, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);  /* RF4_BR_PLAS */
	case RF4_SPELL_START + 26:  return spell_RF4_BREATH(GF_FORCE, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF4_BR_WALL */
	case RF4_SPELL_START + 27:  return spell_RF4_BREATH(GF_MANA, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF4_BR_MANA */
	case RF4_SPELL_START + 28:  return spell_RF4_BA_NUKE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF4_BA_NUKE */
	case RF4_SPELL_START + 29:  return spell_RF4_BREATH(GF_NUKE, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF4_BR_NUKE */
	case RF4_SPELL_START + 30:  return spell_RF4_BA_CHAO(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF4_BA_CHAO */
	case RF4_SPELL_START + 31:  return spell_RF4_BREATH(GF_DISINTEGRATE, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF4_BR_DISI */
	case RF5_SPELL_START + 0:  return spell_RF5_BA_ACID(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BA_ACID */
	case RF5_SPELL_START + 1:  return spell_RF5_BA_ELEC(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BA_ELEC */
	case RF5_SPELL_START + 2:  return spell_RF5_BA_FIRE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BA_FIRE */
	case RF5_SPELL_START + 3:  return spell_RF5_BA_COLD(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BA_COLD */
	case RF5_SPELL_START + 4:  return spell_RF5_BA_POIS(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BA_POIS */
	case RF5_SPELL_START + 5:  return spell_RF5_BA_NETH(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BA_NETH */
	case RF5_SPELL_START + 6:  return spell_RF5_BA_WATE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BA_WATE */
	case RF5_SPELL_START + 7:  return spell_RF5_BA_MANA(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BA_MANA */
	case RF5_SPELL_START + 8:  return spell_RF5_BA_DARK(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BA_DARK */
	case RF5_SPELL_START + 9:  return spell_RF5_DRAIN_MANA(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_DRAIN_MANA */
	case RF5_SPELL_START + 10: return spell_RF5_MIND_BLAST(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_MIND_BLAST */
	case RF5_SPELL_START + 11: return spell_RF5_BRAIN_SMASH(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BRAIN_SMASH */
	case RF5_SPELL_START + 12: return spell_RF5_CAUSE_1(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_CAUSE_1 */
	case RF5_SPELL_START + 13: return spell_RF5_CAUSE_2(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_CAUSE_2 */
	case RF5_SPELL_START + 14: return spell_RF5_CAUSE_3(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_CAUSE_3 */
	case RF5_SPELL_START + 15: return spell_RF5_CAUSE_4(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_CAUSE_4 */
	case RF5_SPELL_START + 16: return spell_RF5_BO_ACID(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BO_ACID */
	case RF5_SPELL_START + 17: return spell_RF5_BO_ELEC(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BO_ELEC */
	case RF5_SPELL_START + 18: return spell_RF5_BO_FIRE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BO_FIRE */
	case RF5_SPELL_START + 19: return spell_RF5_BO_COLD(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BO_COLD */
	case RF5_SPELL_START + 20: return spell_RF5_BA_LITE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BA_LITE */
	case RF5_SPELL_START + 21: return spell_RF5_BO_NETH(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BO_NETH */
	case RF5_SPELL_START + 22: return spell_RF5_BO_WATE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BO_WATE */
	case RF5_SPELL_START + 23: return spell_RF5_BO_MANA(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BO_MANA */
	case RF5_SPELL_START + 24: return spell_RF5_BO_PLAS(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BO_PLAS */
	case RF5_SPELL_START + 25: return spell_RF5_BO_ICEE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_BO_ICEE */
	case RF5_SPELL_START + 26: return spell_RF5_MISSILE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);	/* RF5_MISSILE */
	case RF5_SPELL_START + 27: spell_RF5_SCARE(m_idx, t_idx, MONSTER_TO_MONSTER); break;  /* RF5_SCARE */
	case RF5_SPELL_START + 28: spell_RF5_BLIND(m_idx, t_idx, MONSTER_TO_MONSTER); break;  /* RF5_BLIND */
	case RF5_SPELL_START + 29: spell_RF5_CONF(m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF5_CONF */
	case RF5_SPELL_START + 30: spell_RF5_SLOW(m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF5_SLOW */
	case RF5_SPELL_START + 31: spell_RF5_HOLD(m_idx, t_idx, MONSTER_TO_MONSTER); break;  /* RF5_HOLD */
	case RF6_SPELL_START + 0:  spell_RF6_HASTE(m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF6_HASTE */
	case RF6_SPELL_START + 1:  return spell_RF6_HAND_DOOM(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_HAND_DOOM */
	case RF6_SPELL_START + 2:  spell_RF6_HEAL(m_idx, t_idx, MONSTER_TO_MONSTER); break;	/* RF6_HEAL */
	case RF6_SPELL_START + 3:  spell_RF6_INVULNER(m_idx, t_idx, MONSTER_TO_MONSTER); break;	/* RF6_INVULNER */
	case RF6_SPELL_START + 4:  spell_RF6_BLINK(m_idx, MONSTER_TO_MONSTER); break;   /* RF6_BLINK */
	case RF6_SPELL_START + 5:  spell_RF6_TPORT(m_idx, MONSTER_TO_MONSTER); break;   /* RF6_TPORT */
	case RF6_SPELL_START + 6:  return -1; break;	/* RF6_WORLD */
	case RF6_SPELL_START + 7:  return spell_RF6_SPECIAL(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF6_SPECIAL */
	case RF6_SPELL_START + 8:  spell_RF6_TELE_TO(m_idx, t_idx, MONSTER_TO_MONSTER); break; /* RF6_TELE_TO */
	case RF6_SPELL_START + 9:  spell_RF6_TELE_AWAY(m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF6_TELE_AWAY */
	case RF6_SPELL_START + 10: spell_RF6_TELE_LEVEL(m_idx, t_idx, MONSTER_TO_MONSTER); break;  /* RF6_TELE_LEVEL */
	case RF6_SPELL_START + 11: spell_RF6_PSY_SPEAR(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break; /* RF6_PSY_SPEAR */
	case RF6_SPELL_START + 12: spell_RF6_DARKNESS(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;	/* RF6_DARKNESS */
	case RF6_SPELL_START + 13: return -1; /* RF6_TRAPS */
	case RF6_SPELL_START + 14: return -1;  /* RF6_FORGET */
	case RF6_SPELL_START + 15: spell_RF6_RAISE_DEAD(m_idx, t_idx, MONSTER_TO_MONSTER); break;  /* RF6_RAISE_DEAD */
	case RF6_SPELL_START + 16: spell_RF6_S_KIN(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break; /* RF6_S_KIN */
	case RF6_SPELL_START + 17: spell_RF6_S_CYBER(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF6_S_CYBER */
	case RF6_SPELL_START + 18: spell_RF6_S_MONSTER(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break; /* RF6_S_MONSTER */
	case RF6_SPELL_START + 19: spell_RF6_S_MONSTERS(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;	/* RF6_S_MONSTER */
	case RF6_SPELL_START + 20: spell_RF6_S_ANT(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break; /* RF6_S_ANT */
	case RF6_SPELL_START + 21: spell_RF6_S_SPIDER(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;  /* RF6_S_SPIDER */
	case RF6_SPELL_START + 22: spell_RF6_S_HOUND(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF6_S_HOUND */
	case RF6_SPELL_START + 23: spell_RF6_S_HYDRA(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF6_S_HYDRA */
	case RF6_SPELL_START + 24: spell_RF6_S_ANGEL(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF6_S_ANGEL */
	case RF6_SPELL_START + 25: spell_RF6_S_DEMON(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF6_S_DEMON */
	case RF6_SPELL_START + 26: spell_RF6_S_UNDEAD(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;  /* RF6_S_UNDEAD */
	case RF6_SPELL_START + 27: spell_RF6_S_DRAGON(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;  /* RF6_S_DRAGON */
	case RF6_SPELL_START + 28: spell_RF6_S_HI_UNDEAD(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF6_S_HI_UNDEAD */
	case RF6_SPELL_START + 29: spell_RF6_S_HI_DRAGON(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF6_S_HI_DRAGON */
	case RF6_SPELL_START + 30: spell_RF6_S_AMBERITES(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF6_S_AMBERITES */
	case RF6_SPELL_START + 31: spell_RF6_S_UNIQUE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;  /* RF6_S_UNIQUE */
	}
	return 0;
}

/*!
* @brief ��󥹥����λȤ���ʸ�ΰ��Ϥ���ꤹ�� /
* @param dam �����
* @param dice_num ��������
* @param dice_side ��������
* @param mult ��������Ψ
* @param div ��������Ψ
* @param TYPE  DAM_MAX�Ǻ����ͤ��֤���DAM_MIN�ǺǾ��ͤ��֤���DAM_ROLL�ϥ������򿶤ä��ͤ���ꤹ�롣
* @return �����ʸ�Υ��᡼�����֤��������ʸ�ʳ���-1���֤���
*/
HIT_POINT monspell_damage_roll(HIT_POINT dam, int dice_num, int dice_side, int mult, int div, int TYPE)
{
	switch (TYPE)
	{
		case DAM_MAX: dam += maxroll(dice_num, dice_side) * mult / div; break;
		case DAM_MIN: dam += dice_num * 1 * mult / div; break;
		case DAM_ROLL: dam += damroll(dice_num, dice_side) * mult / div; break;
		case DICE_NUM: return dice_num;
		case DICE_SIDE: return dice_side;
		case DICE_MULT: return mult;
		case DICE_DIV: return div;
		case BASE_DAM: return dam;
	}
	if (dam < 1) dam = 1;
	return dam;
}

/*!
* @brief ��󥹥����λȤ���ʸ�ΰ��Ϥ��֤� /
* @param SPELL_NUM ��ʸ�ֹ�
* @param hp ��ʸ�򾧤����󥹥���������
* @param rlev ��ʸ�򾧤����󥹥����Υ�٥�
* @param powerful ��ʸ�򾧤����󥹥�����powerful�ե饰
* @param shoot_dd �ͷ�Υ�������
* @param shoot_ds �ͷ�Υ�������
* @param shoot_base �ͷ�θ��������
* @param TYPE  DAM_MAX�Ǻ����ͤ��֤���DAM_MIN�ǺǾ��ͤ��֤���DAM_ROLL�ϥ������򿶤ä��ͤ���ꤹ�롣
* @return �����ʸ�Υ��᡼�����֤��������ʸ�ʳ���-1���֤���
*/
HIT_POINT monspell_damage_base(int SPELL_NUM, int hp, int rlev, bool powerful, int shoot_dd, int shoot_ds, int shoot_base, int TYPE)
{
	HIT_POINT dam = 0, dice_num = 0, dice_side = 0, mult = 1, div = 1;

	switch (SPELL_NUM)
	{
	case MS_SHRIEK:   return -1;   /* RF4_SHRIEK */
	case MS_XXX1:   return -1;   /* RF4_XXX1 */
	case MS_DISPEL:   return -1;   /* RF4_DISPEL */

		/* RF4_ROCKET */
	case MS_ROCKET:
		dam = (hp / 4) > 800 ? 800 : (hp / 4);
		break;

		/* RF4_SHOOT */
	case MS_SHOOT:
		dice_num = shoot_dd;
		dice_side = shoot_ds;
		dam = shoot_base;
		break;
	case MS_XXX2:   return -1;   /* RF4_XXX2 */
	case MS_XXX3:   return -1;   /* RF4_XXX3 */
	case MS_XXX4:   return -1;   /* RF4_XXX4 */

		/* RF4_BR_ACID */
		/* RF4_BR_ELEC */
		/* RF4_BR_FIRE */
		/* RF4_BR_COLD */
	case MS_BR_ACID:
	case MS_BR_ELEC:
	case MS_BR_FIRE:
	case MS_BR_COLD:
		dam = ((hp / 3) > 1600 ? 1600 : (hp / 3));
		break;

		/* RF4_BR_POIS */
	case MS_BR_POIS:
		dam = ((hp / 3) > 800 ? 800 : (hp / 3));
		break;

		/* RF4_BR_NETH */
	case MS_BR_NETHER:
		dam = ((hp / 6) > 550 ? 550 : (hp / 6));
		break;

		/* RF4_BR_LITE */
		/* RF4_BR_DARK */
	case MS_BR_LITE:
	case MS_BR_DARK:
		dam = ((hp / 6) > 400 ? 400 : (hp / 6));
		break;

		/* RF4_BR_CONF */
		/* RF4_BR_SOUN */
	case MS_BR_CONF:
	case MS_BR_SOUND:
		dam = ((hp / 6) > 450 ? 450 : (hp / 6));
		break;

		/* RF4_BR_CHAO */
	case MS_BR_CHAOS:
		dam = ((hp / 6) > 600 ? 600 : (hp / 6));
		break;

		/* RF4_BR_DISE */
	case MS_BR_DISEN:
		dam = ((hp / 6) > 500 ? 500 : (hp / 6));
		break;

		/* RF4_BR_NEXU */
	case MS_BR_NEXUS:
		dam = ((hp / 3) > 250 ? 250 : (hp / 3));
		break;

		/* RF4_BR_TIME */
	case MS_BR_TIME:
		dam = ((hp / 3) > 150 ? 150 : (hp / 3));
		break;

		/* RF4_BR_INER */
		/* RF4_BR_GRAV */
	case MS_BR_INERTIA:
	case MS_BR_GRAVITY:
		dam = ((hp / 6) > 200 ? 200 : (hp / 6));
		break;

		/* RF4_BR_SHAR */
	case MS_BR_SHARDS:
		dam = ((hp / 6) > 500 ? 500 : (hp / 6));
		break;

		/* RF4_BR_PLAS */
	case MS_BR_PLASMA:
		dam = ((hp / 6) > 150 ? 150 : (hp / 6));
		break;

		/* RF4_BR_WALL */
	case MS_BR_FORCE:
		dam = ((hp / 6) > 200 ? 200 : (hp / 6));
		break;

		/* RF4_BR_MANA */
	case MS_BR_MANA:
		dam = ((hp / 3) > 250 ? 250 : (hp / 3));
		break;

		/* RF4_BA_NUKE */
	case MS_BALL_NUKE:
		mult = powerful ? 2 : 1;
		dam = rlev * (mult / div);
		dice_num = 10;
		dice_side = 6;
		break;

		/* RF4_BR_NUKE */
	case MS_BR_NUKE:
		dam = ((hp / 3) > 800 ? 800 : (hp / 3));
		break;

		/* RF4_BA_CHAO */
	case MS_BALL_CHAOS:
		dam = (powerful ? (rlev * 3) : (rlev * 2));
		dice_num = 10;
		dice_side = 10;
		break;

		/* RF4_BR_DISI */
	case MS_BR_DISI:
		dam = ((hp / 6) > 150 ? 150 : (hp / 6));
		break;

		/* RF5_BA_ACID */
	case MS_BALL_ACID:
		if (powerful)
		{
			dam = (rlev * 4) + 50;
			dice_num = 10;
			dice_side = 10;
		}
		else
		{
			dam = 15;
			dice_num = 1;
			dice_side = rlev * 3;
		}
		break;

		/* RF5_BA_ELEC */
	case MS_BALL_ELEC:
		if (powerful)
		{
			dam = (rlev * 4) + 50;
			dice_num = 10;
			dice_side = 10;
		}
		else
		{
			dam = 8;
			dice_num = 1;
			dice_side = rlev * 3 / 2;
		}
		break;

		/* RF5_BA_FIRE */
	case MS_BALL_FIRE:
		if (powerful)
		{
			dam = (rlev * 4) + 50;
			dice_num = 10;
			dice_side = 10;
		}
		else
		{
			dam = 10;
			dice_num = 1;
			dice_side = rlev * 7 / 2;
		}
		break;

		/* RF5_BA_COLD */
	case MS_BALL_COLD:
		if (powerful)
		{
			dam = (rlev * 4) + 50;
			dice_num = 10;
			dice_side = 10;
		}
		else
		{
			dam = 10;
			dice_num = 1;
			dice_side = rlev * 3 / 2;
		}
		break;

		/* RF5_BA_POIS */
	case MS_BALL_POIS:
		mult = powerful ? 2 : 1;
		dice_num = 12;
		dice_side = 2;
		break;

		/* RF5_BA_NETH */
	case MS_BALL_NETHER:
		dam = 50 + rlev * (powerful ? 2 : 1);
		dice_num = 10;
		dice_side = 10;
		break;

		/* RF5_BA_WATE */
	case MS_BALL_WATER:
		dam = 50;
		dice_num = 1;
		dice_side = powerful ? (rlev * 3) : (rlev * 2);
		break;

		/* RF5_BA_MANA */
		/* RF5_BA_DARK */
	case MS_BALL_MANA:
	case MS_BALL_DARK:
		dam = (rlev * 4) + 50;
		dice_num = 10;
		dice_side = 10;
		break;

		/* RF5_DRAIN_MANA */
	case MS_DRAIN_MANA:
		dam = rlev;
		div = 1;
		dice_num = 1;
		dice_side = rlev;
		break;

		/* RF5_MIND_BLAST */
	case MS_MIND_BLAST:
		dice_num = 7;
		dice_side = 7;
		break;

		/* RF5_BRAIN_SMASH */
	case MS_BRAIN_SMASH:
		dice_num = 12;
		dice_side = 12;
		break;

		/* RF5_CAUSE_1 */
	case MS_CAUSE_1:
		dice_num = 3;
		dice_side = 8;
		break;

		/* RF5_CAUSE_2 */
	case MS_CAUSE_2:
		dice_num = 8;
		dice_side = 8;
		break;

		/* RF5_CAUSE_3 */
	case MS_CAUSE_3:
		dice_num = 10;
		dice_side = 15;
		break;

		/* RF5_CAUSE_4 */
	case MS_CAUSE_4:
		dice_num = 15;
		dice_side = 15;
		break;

		/* RF5_BO_ACID */
	case MS_BOLT_ACID:
		mult = powerful ? 2 : 1;
		dam = rlev / 3 * (mult / div);
		dice_num = 7;
		dice_side = 8;
		break;

		/* RF5_BO_ELEC */
	case MS_BOLT_ELEC:
		mult = powerful ? 2 : 1;
		dam = rlev / 3 * (mult / div);
		dice_num = 4;
		dice_side = 8;
		break;

		/* RF5_BO_FIRE */
	case MS_BOLT_FIRE:
		mult = powerful ? 2 : 1;
		dam = rlev / 3 * (mult / div);
		dice_num = 9;
		dice_side = 8;
		break;

		/* RF5_BO_COLD */
	case MS_BOLT_COLD:
		mult = powerful ? 2 : 1;
		dam = rlev / 3 * (mult / div);
		dice_num = 6;
		dice_side = 8;
		break;

		/* RF5_BA_LITE */
	case MS_STARBURST:
		dam = (rlev * 4) + 50;
		dice_num = 10;
		dice_side = 10;
		break;

		/* RF5_BO_NETH */
	case MS_BOLT_NETHER:
		dam = 30 + (rlev * 4) / (powerful ? 2 : 3);
		dice_num = 5;
		dice_side = 5;
		break;

		/* RF5_BO_WATE */
	case MS_BOLT_WATER:
		dam = (rlev * 3 / (powerful ? 2 : 3));
		dice_num = 10;
		dice_side = 10;
		break;

		/* RF5_BO_MANA */
	case MS_BOLT_MANA:
		dam = 50;
		dice_num = 1;
		dice_side = rlev * 7 / 2;
		break;

		/* RF5_BO_PLAS */
	case MS_BOLT_PLASMA:
		dam = 10 + (rlev * 3 / (powerful ? 2 : 3));
		dice_num = 8;
		dice_side = 7;
		break;

		/* RF5_BO_ICEE */
	case MS_BOLT_ICE:
		dam = (rlev * 3 / (powerful ? 2 : 3));
		dice_num = 6;
		dice_side = 6;
		break;

		/* RF5_MISSILE */
	case MS_MAGIC_MISSILE:
		dam = (rlev / 3);
		dice_num = 2;
		dice_side = 6;
		break;

	case MS_SCARE: return -1;   /* RF5_SCARE */
	case MS_BLIND: return -1;   /* RF5_BLIND */
	case MS_CONF: return -1;   /* RF5_CONF */
	case MS_SLOW: return -1;   /* RF5_SLOW */
	case MS_SLEEP: return -1;   /* RF5_HOLD */
	case MS_SPEED:  return -1;   /* RF6_HASTE */

		/* RF6_HAND_DOOM */
	case MS_HAND_DOOM:
		mult = p_ptr->chp;
		div = 100;
		dam = 40 * (mult / div);
		dice_num = 1;
		dice_side = 20;
		break;

	case MS_HEAL:  return -1;   /* RF6_HEAL */
	case MS_INVULNER:  return -1;   /* RF6_INVULNER */
	case MS_BLINK:  return -1;   /* RF6_BLINK */
	case MS_TELEPORT:  return -1;   /* RF6_TPORT */
	case MS_WORLD:  return -1;   /* RF6_WORLD */
	case MS_SPECIAL:  return -1;   /* RF6_SPECIAL */
	case MS_TELE_TO:  return -1;   /* RF6_TELE_TO */
	case MS_TELE_AWAY:  return -1;   /* RF6_TELE_AWAY */
	case MS_TELE_LEVEL: return -1;   /* RF6_TELE_LEVEL */

		/* RF6_PSY_SPEAR */
	case MS_PSY_SPEAR:
		dam = powerful ? 150 : 100;
		dice_num = 1;
		dice_side = powerful ? (rlev * 2) : (rlev * 3 / 2);
		break;

	case MS_DARKNESS: return -1;   /* RF6_DARKNESS */
	case MS_MAKE_TRAP: return -1;   /* RF6_TRAPS */
	case MS_FORGET: return -1;   /* RF6_FORGET */
	case MS_RAISE_DEAD: return -1;   /* RF6_RAISE_DEAD */
	case MS_S_KIN: return -1;   /* RF6_S_KIN */
	case MS_S_CYBER: return -1;   /* RF6_S_CYBER */
	case MS_S_MONSTER: return -1;   /* RF6_S_MONSTER */
	case MS_S_MONSTERS: return -1;   /* RF6_S_MONSTER */
	case MS_S_ANT: return -1;   /* RF6_S_ANT */
	case MS_S_SPIDER: return -1;   /* RF6_S_SPIDER */
	case MS_S_HOUND: return -1;   /* RF6_S_HOUND */
	case MS_S_HYDRA: return -1;   /* RF6_S_HYDRA */
	case MS_S_ANGEL: return -1;   /* RF6_S_ANGEL */
	case MS_S_DEMON: return -1;   /* RF6_S_DEMON */
	case MS_S_UNDEAD: return -1;   /* RF6_S_UNDEAD */
	case MS_S_DRAGON: return -1;   /* RF6_S_DRAGON */
	case MS_S_HI_UNDEAD: return -1;   /* RF6_S_HI_UNDEAD */
	case MS_S_HI_DRAGON: return -1;   /* RF6_S_HI_DRAGON */
	case MS_S_AMBERITE: return -1;   /* RF6_S_AMBERITES */
	case MS_S_UNIQUE: return -1;   /* RF6_S_UNIQUE */
	}

	return monspell_damage_roll(dam, dice_num, dice_side, mult, div, TYPE);
}


/*!
* @brief ��󥹥����λȤ���ʸ�ΰ��Ϥ��֤� /
* @param SPELL_NUM ��ʸ�ֹ�
* @param m_idx ��ʸ�򾧤����󥹥���ID
* @param TYPE  DAM_MAX�Ǻ����ͤ��֤���DAM_MIN�ǺǾ��ͤ��֤���DAM_ROLL�ϥ������򿶤ä��ͤ���ꤹ�롣
* @return �����ʸ�Υ��᡼�����֤��������ʸ�ʳ���-1���֤���
*/
HIT_POINT monspell_damage(int SPELL_NUM, MONSTER_IDX m_idx, int TYPE)
{
	monster_type	*m_ptr = &m_list[m_idx];
	monster_race	*r_ptr = &r_info[m_ptr->r_idx];
	int hp;
	int rlev = monster_level_idx(m_idx);
	int shoot_dd = r_ptr->blow[0].d_dice;
	int shoot_ds = r_ptr->blow[0].d_side;

	if (TYPE == DAM_ROLL)
	{
		hp = m_ptr->hp;
	}
	else
	{
		hp = m_ptr->max_maxhp;
	} 
	return monspell_damage_base(SPELL_NUM, hp, rlev, monster_is_powerful(m_idx), shoot_dd, shoot_ds, 0, TYPE);
}

/*!
* @brief ��󥹥����λȤ���ʸ�ΰ��Ϥ��֤� /
* @param SPELL_NUM ��ʸ�ֹ�
* @param r_idx ��ʸ�򾧤����󥹥����μ�²ID
* @param TYPE  DAM_MAX�Ǻ����ͤ��֤���DAM_MIN�ǺǾ��ͤ��֤���DAM_ROLL�ϥ������򿶤ä��ͤ���ꤹ�롣
* @return �����ʸ�Υ��᡼�����֤��������ʸ�ʳ���-1���֤���
*/
HIT_POINT monspell_race_damage(int SPELL_NUM, MONRACE_IDX r_idx, int TYPE)
{
	monster_race	*r_ptr = &r_info[r_idx];
	int rlev = ((r_ptr->level >= 1) ? r_ptr->level : 1);
	bool powerful = r_ptr->flags2 & RF2_POWERFUL ? TRUE : FALSE;
	u32b hp = r_ptr->hdice * (ironman_nightmare ? 2 : 1) * r_ptr->hside;
	int shoot_dd = r_ptr->blow[0].d_dice;
	int shoot_ds = r_ptr->blow[0].d_side;

	return monspell_damage_base(SPELL_NUM, MIN(30000, hp), rlev, powerful, shoot_dd, shoot_ds, 0, TYPE);
}

/*!
* @brief ����Ƴ�դλȤ���ʸ�ΰ��Ϥ��֤� /
* @param SPELL_NUM ��ʸ�ֹ�
* @param plev ���Ѥ����٥롣2�ܤ��ư�����
* @param TYPE  DAM_MAX�Ǻ����ͤ��֤���DAM_MIN�ǺǾ��ͤ��֤���DAM_ROLL�ϥ������򿶤ä��ͤ���ꤹ�롣
* @return �����ʸ�Υ��᡼�����֤��������ʸ�ʳ���-1���֤���
*/
HIT_POINT monspell_bluemage_damage(int SPELL_NUM, int plev, int TYPE)
{
	int hp = p_ptr->chp;
	int shoot_dd = 1, shoot_ds = 1, shoot_base = 0;
	object_type *o_ptr = NULL;

	if (buki_motteruka(INVEN_RARM)) o_ptr = &inventory[INVEN_RARM];
	else if (buki_motteruka(INVEN_LARM)) o_ptr = &inventory[INVEN_LARM];

	if (o_ptr)
	{
		shoot_dd = o_ptr->dd;
		shoot_ds = o_ptr->ds;
		shoot_base = o_ptr->to_d;
	}
	return monspell_damage_base(SPELL_NUM, hp, plev * 2, FALSE, shoot_dd, shoot_ds, shoot_base, TYPE);
}