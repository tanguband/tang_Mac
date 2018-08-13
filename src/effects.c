/*!
 * @file effects.c
 * @brief �ץ쥤�䡼�Υ��ơ��������� / effects of various "objects"
 * @date 2014/01/01
 * @author
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke\n
 *\n
 * This software may be copied and distributed for educational, research,\n
 * and not for profit purposes provided that this copyright and statement\n
 * are included in all such copies.  Other copyrights may also apply.\n
 *\n
 * 2013 Deskull rearranged comment for Doxygen.\n
 */


#include "angband.h"

/*!
 * @brief �ץ쥤�䡼�η�³��ư�����ꤹ�롣
 * @param typ ��³��ư��ID\n
 * #ACTION_NONE / #ACTION_SEARCH / #ACTION_REST / #ACTION_LEARN / #ACTION_FISH / #ACTION_KAMAE / #ACTION_KATA / #ACTION_SING / #ACTION_HAYAGAKE / #ACTION_SPELL ��������
 * @return �ʤ�
 */
void set_action(ACTION_IDX typ)
{
	int prev_typ = p_ptr->action;

	if (typ == prev_typ)
	{
		return;
	}
	else
	{
		switch (prev_typ)
		{
			case ACTION_SEARCH:
			{
				msg_print(_("õ�����᤿��", "You no longer walk carefully."));
				p_ptr->redraw |= (PR_SPEED);
				break;
			}
			case ACTION_REST:
			{
				resting = 0;
				break;
			}
			case ACTION_LEARN:
			{
				msg_print(_("�ؽ����᤿��", "You stop Learning"));
				new_mane = FALSE;
				break;
			}
			case ACTION_KAMAE:
			{
				msg_print(_("������Ȥ�����", "You stop assuming the posture."));
				p_ptr->special_defense &= ~(KAMAE_MASK);
				break;
			}
			case ACTION_KATA:
			{
				msg_print(_("������������", "You stop assuming the posture."));
				p_ptr->special_defense &= ~(KATA_MASK);
				p_ptr->update |= (PU_MONSTERS);
				p_ptr->redraw |= (PR_STATUS);
				break;
			}
			case ACTION_SING:
			{
				msg_print(_("�Τ��Τ��᤿��", "You stop singing."));
				break;
			}
			case ACTION_HAYAGAKE:
			{
				msg_print(_("­���Ť��ʤä���", "You are no longer walking extremely fast."));
				p_ptr->energy_use = 100;
				break;
			}
			case ACTION_SPELL:
			{
				msg_print(_("��ʸ�αӾ������Ǥ�����", "You stopped spelling all spells."));
				break;
			}
		}
	}

	p_ptr->action = typ;

	/* If we are requested other action, stop singing */
	if (prev_typ == ACTION_SING) stop_singing();
	if (prev_typ == ACTION_SPELL) stop_hex_spell();

	switch (p_ptr->action)
	{
		case ACTION_SEARCH:
		{
			msg_print(_("��տ����⤭�Ϥ᤿��", "You begin to walk carefully."));
			p_ptr->redraw |= (PR_SPEED);
			break;
		}
		case ACTION_LEARN:
		{
			msg_print(_("�ؽ���Ϥ᤿��", "You begin Learning"));
			break;
		}
		case ACTION_FISH:
		{
			msg_print(_("���̤˻���餷��������", "You begin fishing..."));
			break;
		}
		case ACTION_HAYAGAKE:
		{
			msg_print(_("­�����Τ褦�˷ڤ��ʤä���", "You begin to walk extremely fast."));
			break;
		}
		default:
		{
			break;
		}
	}

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Redraw the state */
	p_ptr->redraw |= (PR_STATE);
}

/*!
 * @brief �ץ쥤�䡼�����Ƥλ��¸��̤�ꥻ�åȤ��롣 / reset timed flags
 * @return �ʤ�
 */
void reset_tim_flags(void)
{
	p_ptr->fast = 0;            /* Timed -- Fast */
	p_ptr->lightspeed = 0;
	p_ptr->slow = 0;            /* Timed -- Slow */
	p_ptr->blind = 0;           /* Timed -- Blindness */
	p_ptr->paralyzed = 0;       /* Timed -- Paralysis */
	p_ptr->confused = 0;        /* Timed -- Confusion */
	p_ptr->afraid = 0;          /* Timed -- Fear */
	p_ptr->image = 0;           /* Timed -- Hallucination */
	p_ptr->poisoned = 0;        /* Timed -- Poisoned */
	p_ptr->cut = 0;             /* Timed -- Cut */
	p_ptr->stun = 0;            /* Timed -- Stun */

	p_ptr->protevil = 0;        /* Timed -- Protection */
	p_ptr->invuln = 0;          /* Timed -- Invulnerable */
	p_ptr->ult_res = 0;
	p_ptr->hero = 0;            /* Timed -- Heroism */
	p_ptr->shero = 0;           /* Timed -- Super Heroism */
	p_ptr->shield = 0;          /* Timed -- Shield Spell */
	p_ptr->blessed = 0;         /* Timed -- Blessed */
	p_ptr->tim_invis = 0;       /* Timed -- Invisibility */
	p_ptr->tim_infra = 0;       /* Timed -- Infra Vision */
	p_ptr->tim_regen = 0;       /* Timed -- Regeneration */
	p_ptr->tim_stealth = 0;     /* Timed -- Stealth */
	p_ptr->tim_esp = 0;
	p_ptr->wraith_form = 0;     /* Timed -- Wraith Form */
	p_ptr->tim_levitation = 0;
	p_ptr->tim_sh_touki = 0;
	p_ptr->tim_sh_fire = 0;
	p_ptr->tim_sh_holy = 0;
	p_ptr->tim_eyeeye = 0;
	p_ptr->magicdef = 0;
	p_ptr->resist_magic = 0;
	p_ptr->tsuyoshi = 0;
	p_ptr->kabenuke = 0;
	p_ptr->tim_res_nether = 0;
	p_ptr->tim_res_time = 0;
	p_ptr->tim_mimic = 0;
	p_ptr->mimic_form = 0;
	p_ptr->tim_reflect = 0;
	p_ptr->multishadow = 0;
	p_ptr->dustrobe = 0;
	p_ptr->action = ACTION_NONE;


	p_ptr->oppose_acid = 0;     /* Timed -- oppose acid */
	p_ptr->oppose_elec = 0;     /* Timed -- oppose lightning */
	p_ptr->oppose_fire = 0;     /* Timed -- oppose heat */
	p_ptr->oppose_cold = 0;     /* Timed -- oppose cold */
	p_ptr->oppose_pois = 0;     /* Timed -- oppose poison */

	p_ptr->word_recall = 0;
	p_ptr->alter_reality = 0;
	p_ptr->sutemi = FALSE;
	p_ptr->counter = FALSE;
	p_ptr->ele_attack = 0;
	p_ptr->ele_immune = 0;
	p_ptr->special_attack = 0L;
	p_ptr->special_defense = 0L;

	while(p_ptr->energy_need < 0) p_ptr->energy_need += ENERGY_NEED();
	world_player = FALSE;

	if (prace_is_(RACE_DEMON) && (p_ptr->lev > 44)) p_ptr->oppose_fire = 1;
	if ((p_ptr->pclass == CLASS_NINJA) && (p_ptr->lev > 44)) p_ptr->oppose_pois = 1;
	if (p_ptr->pclass == CLASS_BERSERKER) p_ptr->shero = 1;

	if (p_ptr->riding)
	{
		(void)set_monster_fast(p_ptr->riding, 0);
		(void)set_monster_slow(p_ptr->riding, 0);
		(void)set_monster_invulner(p_ptr->riding, 0, FALSE);
	}

	if (p_ptr->pclass == CLASS_BARD)
	{
		SINGING_SONG_EFFECT(p_ptr) = 0;
		SINGING_SONG_ID(p_ptr) = 0;
	}
}

/*!
 * @brief �ץ쥤�䡼�����Ͼõ���̤�Ϳ���롣
 * @return �ʤ�
 */
void dispel_player(void)
{
	(void)set_fast(0, TRUE);
	(void)set_lightspeed(0, TRUE);
	(void)set_slow(0, TRUE);
	(void)set_shield(0, TRUE);
	(void)set_blessed(0, TRUE);
	(void)set_tsuyoshi(0, TRUE);
	(void)set_hero(0, TRUE);
	(void)set_shero(0, TRUE);
	(void)set_protevil(0, TRUE);
	(void)set_invuln(0, TRUE);
	(void)set_wraith_form(0, TRUE);
	(void)set_kabenuke(0, TRUE);
	(void)set_tim_res_nether(0, TRUE);
	(void)set_tim_res_time(0, TRUE);
	/* by henkma */
	(void)set_tim_reflect(0,TRUE);
	(void)set_multishadow(0,TRUE);
	(void)set_dustrobe(0,TRUE);

	(void)set_tim_invis(0, TRUE);
	(void)set_tim_infra(0, TRUE);
	(void)set_tim_esp(0, TRUE);
	(void)set_tim_regen(0, TRUE);
	(void)set_tim_stealth(0, TRUE);
	(void)set_tim_levitation(0, TRUE);
	(void)set_tim_sh_touki(0, TRUE);
	(void)set_tim_sh_fire(0, TRUE);
	(void)set_tim_sh_holy(0, TRUE);
	(void)set_tim_eyeeye(0, TRUE);
	(void)set_magicdef(0, TRUE);
	(void)set_resist_magic(0, TRUE);
	(void)set_oppose_acid(0, TRUE);
	(void)set_oppose_elec(0, TRUE);
	(void)set_oppose_fire(0, TRUE);
	(void)set_oppose_cold(0, TRUE);
	(void)set_oppose_pois(0, TRUE);
	(void)set_ultimate_res(0, TRUE);
	(void)set_mimic(0, 0, TRUE);
	(void)set_ele_attack(0, 0);
	(void)set_ele_immune(0, 0);

	/* Cancel glowing hands */
	if (p_ptr->special_attack & ATTACK_CONFUSE)
	{
		p_ptr->special_attack &= ~(ATTACK_CONFUSE);
		msg_print(_("��ε������ʤ��ʤä���", "Your hands stop glowing."));
	}

	if (music_singing_any() || hex_spelling_any())
	{
		cptr str = (music_singing_any()) ? _("��", "singing") : _("��ʸ", "spelling");
		INTERUPTING_SONG_EFFECT(p_ptr) = SINGING_SONG_EFFECT(p_ptr);
		SINGING_SONG_EFFECT(p_ptr) = MUSIC_NONE;
		msg_format(_("%s�����ڤ줿��", "Your %s is interrupted."), str);
		p_ptr->action = ACTION_NONE;

		/* Recalculate bonuses */
		p_ptr->update |= (PU_BONUS | PU_HP);

		/* Redraw map */
		p_ptr->redraw |= (PR_MAP | PR_STATUS | PR_STATE);

		/* Update monsters */
		p_ptr->update |= (PU_MONSTERS);

		/* Window stuff */
		p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);

		p_ptr->energy_need += ENERGY_NEED();
	}
}


/*!
 * @brief �ѿȸ��̤η�³���֤��ѿ���򥻥åȤ��� / Set "p_ptr->tim_mimic", and "p_ptr->mimic_form", notice observable changes
 * @param v ��³����
 * @param p �ѿ�����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_mimic(TIME_EFFECT v, IDX p, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->tim_mimic && (p_ptr->mimic_form == p) && !do_dec)
		{
			if (p_ptr->tim_mimic > v) return FALSE;
		}
		else if ((!p_ptr->tim_mimic) || (p_ptr->mimic_form != p))
		{
			msg_print(_("��ʬ���Τ��Ѥ�äƤ椯�Τ򴶤�����", "You feel that your body changes."));
			p_ptr->mimic_form = p;
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->tim_mimic)
		{
			msg_print(_("�ѿȤ��򤱤���", "You are no longer transformed."));
			if (p_ptr->mimic_form == MIMIC_DEMON) set_oppose_fire(0, TRUE);
			p_ptr->mimic_form=0;
			notice = TRUE;
			p = 0;
		}
	}

	/* Use the value */
	p_ptr->tim_mimic = v;

	/* Nothing to notice */
	if (!notice)
		return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 1);

	/* Redraw title */
	p_ptr->redraw |= (PR_BASIC | PR_STATUS);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS | PU_HP);

	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ���ܤη�³���֤򥻥åȤ��� / Set "p_ptr->blind", notice observable changes
 * @param v ��³����
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 * @details
 * Note the use of "PU_UN_LITE" and "PU_UN_VIEW", which is needed to\n
 * memorize any terrain features which suddenly become "visible".\n
 * Note that blindness is currently the only thing which can affect\n
 * "player_can_see_bold()".\n
 */
bool set_blind(TIME_EFFECT v)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (!p_ptr->blind)
		{
			if (p_ptr->prace == RACE_ANDROID)
			{
				msg_print(_("���󥵡�����줿��", "You are blind!"));
			}
			else
			{
				msg_print(_("�ܤ������ʤ��ʤäƤ��ޤä���", "You are blind!"));
			}

			notice = TRUE;
			chg_virtue(V_ENLIGHTEN, -1);
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->blind)
		{
			if (p_ptr->prace == RACE_ANDROID)
			{
				msg_print(_("���󥵡������줷����", "You can see again."));
			}
			else
			{
				msg_print(_("��ä��ܤ�������褦�ˤʤä���", "You can see again."));
			}

			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->blind = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Fully update the visuals */
	p_ptr->update |= (PU_UN_VIEW | PU_UN_LITE | PU_VIEW | PU_LITE | PU_MONSTERS | PU_MON_LITE);

	/* Redraw map */
	p_ptr->redraw |= (PR_MAP);

	/* Window stuff */
	p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}


/*!
 * @brief ����η�³���֤򥻥åȤ��� / Set "p_ptr->confused", notice observable changes
 * @param v ��³����
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_confused(TIME_EFFECT v)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (!p_ptr->confused)
		{
			msg_print(_("���ʤ��Ϻ��𤷤���", "You are confused!"));

			if (p_ptr->action == ACTION_LEARN)
			{
				msg_print(_("�ؽ���³�����ʤ���", "You cannot continue Learning!"));
				new_mane = FALSE;

				p_ptr->redraw |= (PR_STATE);
				p_ptr->action = ACTION_NONE;
			}
			if (p_ptr->action == ACTION_KAMAE)
			{
				msg_print(_("�������Ȥ�����", "Your posture gets loose."));
				p_ptr->special_defense &= ~(KAMAE_MASK);
				p_ptr->update |= (PU_BONUS);
				p_ptr->redraw |= (PR_STATE);
				p_ptr->action = ACTION_NONE;
			}
			else if (p_ptr->action == ACTION_KATA)
			{
				msg_print(_("�������줿��", "Your posture gets loose."));
				p_ptr->special_defense &= ~(KATA_MASK);
				p_ptr->update |= (PU_BONUS);
				p_ptr->update |= (PU_MONSTERS);
				p_ptr->redraw |= (PR_STATE);
				p_ptr->redraw |= (PR_STATUS);
				p_ptr->action = ACTION_NONE;
			}

			/* Sniper */
			if (p_ptr->concent) reset_concentration(TRUE);

			/* Hex */
			if (hex_spelling_any()) stop_hex_spell_all();

			notice = TRUE;
			p_ptr->counter = FALSE;
			chg_virtue(V_HARMONY, -1);
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->confused)
		{
			msg_print(_("��äȺ��𤬤����ޤä���", "You feel less confused now."));
			p_ptr->special_attack &= ~(ATTACK_SUIKEN);
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->confused = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}


/*!
 * @brief �Ǥη�³���֤򥻥åȤ��� / Set "p_ptr->poisoned", notice observable changes
 * @param v ��³����
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_poisoned(TIME_EFFECT v)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (!p_ptr->poisoned)
		{
			msg_print(_("�Ǥ˿�����Ƥ��ޤä���", "You are poisoned!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->poisoned)
		{
			msg_print(_("��ä��Ǥ��ˤߤ��ʤ��ʤä���", "You are no longer poisoned."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->poisoned = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}


/*!
 * @brief ���ݤη�³���֤򥻥åȤ��� / Set "p_ptr->afraid", notice observable changes
 * @param v ��³����
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_afraid(TIME_EFFECT v)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (!p_ptr->afraid)
		{
			msg_print(_("���⤫�ⶲ���ʤäƤ�����", "You are terrified!"));

			if (p_ptr->special_defense & KATA_MASK)
			{
				msg_print(_("�������줿��", "Your posture gets loose."));
				p_ptr->special_defense &= ~(KATA_MASK);
				p_ptr->update |= (PU_BONUS);
				p_ptr->update |= (PU_MONSTERS);
				p_ptr->redraw |= (PR_STATE);
				p_ptr->redraw |= (PR_STATUS);
				p_ptr->action = ACTION_NONE;
			}

			notice = TRUE;
			p_ptr->counter = FALSE;
			chg_virtue(V_VALOUR, -1);
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->afraid)
		{
			msg_print(_("��äȶ��ݤ򿶤�ʧ�ä���", "You feel bolder now."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->afraid = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ����η�³���֤򥻥åȤ��� / Set "p_ptr->paralyzed", notice observable changes
 * @param v ��³����
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_paralyzed(TIME_EFFECT v)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (!p_ptr->paralyzed)
		{
			msg_print(_("�Τ����㤷�Ƥ��ޤä���", "You are paralyzed!"));
			/* Sniper */
			if (p_ptr->concent) reset_concentration(TRUE);

			/* Hex */
			if (hex_spelling_any()) stop_hex_spell_all();

			p_ptr->counter = FALSE;
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->paralyzed)
		{
			msg_print(_("��ä�ư����褦�ˤʤä���", "You can move again."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->paralyzed = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Redraw the state */
	p_ptr->redraw |= (PR_STATE);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ���Фη�³���֤򥻥åȤ��� / Set "p_ptr->image", notice observable changes
 * @param v ��³����
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 * @details Note that we must redraw the map when hallucination changes.
 */
bool set_image(TIME_EFFECT v)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;


	/* Open */
	if (v)
	{
		set_tsuyoshi(0, TRUE);
		if (!p_ptr->image)
		{
			msg_print(_("��������⤫�������˸����롪", "Oh, wow! Everything looks so cosmic now!"));

			/* Sniper */
			if (p_ptr->concent) reset_concentration(TRUE);

			p_ptr->counter = FALSE;
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->image)
		{
			msg_print(_("��äȤϤä����ʪ��������褦�ˤʤä���", "You can see clearly again."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->image = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 1);

	/* Redraw map */
	p_ptr->redraw |= (PR_MAP);

	/* Update the health bar */
	p_ptr->redraw |= (PR_HEALTH | PR_UHEALTH);

	/* Update monsters */
	p_ptr->update |= (PU_MONSTERS);

	/* Window stuff */
	p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ��®�η�³���֤򥻥åȤ��� / Set "p_ptr->fast", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_fast(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->fast && !do_dec)
		{
			if (p_ptr->fast > v) return FALSE;
		}
		else if (!IS_FAST() && !p_ptr->lightspeed)
		{
			msg_print(_("���᤯ư����褦�ˤʤä���", "You feel yourself moving much faster!"));
			notice = TRUE;
			chg_virtue(V_PATIENCE, -1);
			chg_virtue(V_DILIGENCE, 1);
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->fast && !p_ptr->lightspeed && !music_singing(MUSIC_SPEED) && !music_singing(MUSIC_SHERO))
		{
			msg_print(_("ư�������ᤵ���ʤ��ʤä��褦����", "You feel yourself slow down."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->fast = v;

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ��®��ư�η�³���֤򥻥åȤ��� / Set "p_ptr->lightspeed", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_lightspeed(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	if (p_ptr->wild_mode) v = 0;

	/* Open */
	if (v)
	{
		if (p_ptr->lightspeed && !do_dec)
		{
			if (p_ptr->lightspeed > v) return FALSE;
		}
		else if (!p_ptr->lightspeed)
		{
			msg_print(_("�������᤯ư����褦�ˤʤä���", "You feel yourself moving extremely faster!"));
			notice = TRUE;
			chg_virtue(V_PATIENCE, -1);
			chg_virtue(V_DILIGENCE, 1);
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->lightspeed)
		{
			msg_print(_("ư�������ᤵ���ʤ��ʤä��褦����", "You feel yourself slow down."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->lightspeed = v;

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ��®�η�³���֤򥻥åȤ��� / Set "p_ptr->slow", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_slow(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->slow && !do_dec)
		{
			if (p_ptr->slow > v) return FALSE;
		}
		else if (!p_ptr->slow)
		{
			msg_print(_("�Τ�ư�����٤��ʤäƤ��ޤä���", "You feel yourself moving slower!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->slow)
		{
			msg_print(_("ư�����٤����ʤ��ʤä��褦����", "You feel yourself speed up."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->slow = v;

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}


/*!
 * @brief ȩ�в��η�³���֤򥻥åȤ��� / Set "p_ptr->shield", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_shield(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->shield && !do_dec)
		{
			if (p_ptr->shield > v) return FALSE;
		}
		else if (!p_ptr->shield)
		{
			msg_print(_("ȩ���Фˤʤä���", "Your skin turns to stone."));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->shield)
		{
			msg_print(_("ȩ��������ä���", "Your skin returns to normal."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->shield = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}


/*!
 * @brief �Ĥ֤��η�³���֤򥻥åȤ��� / Set "p_ptr->tsubureru", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_tsubureru(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->tsubureru && !do_dec)
		{
			if (p_ptr->tsubureru > v) return FALSE;
		}
		else if (!p_ptr->tsubureru)
		{
			msg_print(_("���˿��Ӥ���", "Your body expands horizontally."));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->tsubureru)
		{
			msg_print(_("�⤦���˿��ӤƤ��ʤ���", "Your body returns to normal."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->tsubureru = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}


/*!
 * @brief ��ˡ�γ��η�³���֤򥻥åȤ��� / Set "p_ptr->magicdef", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_magicdef(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->magicdef && !do_dec)
		{
			if (p_ptr->magicdef > v) return FALSE;
		}
		else if (!p_ptr->magicdef)
		{
			msg_print(_("��ˡ���ɸ��Ϥ��������褦�ʵ������롣", "You feel more resistant to magic."));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->magicdef)
		{
			msg_print(_("��ˡ���ɸ��Ϥ�������ä���", "You feel less resistant to magic."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->magicdef = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ��ʡ�η�³���֤򥻥åȤ��� / Set "p_ptr->blessed", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_blessed(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->blessed && !do_dec)
		{
			if (p_ptr->blessed > v) return FALSE;
		}
		else if (!IS_BLESSED())
		{
			msg_print(_("���ʵ�ʬ�ˤʤä���", "You feel righteous!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->blessed && !music_singing(MUSIC_BLESS))
		{
			msg_print(_("���ʵ�ʬ���ä���������", "The prayer has expired."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->blessed = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}


/*!
 * @brief �ε����Ȥη�³���֤򥻥åȤ��� / Set "p_ptr->hero", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_hero(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->hero && !do_dec)
		{
			if (p_ptr->hero > v) return FALSE;
		}
		else if (!IS_HERO())
		{
			msg_print(_("�ҡ����ˤʤä��������롪", "You feel like a hero!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->hero && !music_singing(MUSIC_HERO) && !music_singing(MUSIC_SHERO))
		{
			msg_print(_("�ҡ����ε�ʬ���ä���������", "The heroism wears off."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->hero = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Recalculate hitpoints */
	p_ptr->update |= (PU_HP);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ����β��η�³���֤򥻥åȤ��� / Set "p_ptr->shero", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_shero(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	if (p_ptr->pclass == CLASS_BERSERKER) v = 1;
	/* Open */
	if (v)
	{
		if (p_ptr->shero && !do_dec)
		{
			if (p_ptr->shero > v) return FALSE;
		}
		else if (!p_ptr->shero)
		{
			msg_print(_("��٤�ޥ�����ˤʤä��������롪", "You feel like a killing machine!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->shero)
		{
			msg_print(_("���ڤʵ��������ä���������", "You feel less Berserk."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->shero = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Recalculate hitpoints */
	p_ptr->update |= (PU_HP);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief �мٰ��볦�η�³���֤򥻥åȤ��� / Set "p_ptr->protevil", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_protevil(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->protevil && !do_dec)
		{
			if (p_ptr->protevil > v) return FALSE;
		}
		else if (!p_ptr->protevil)
		{
			msg_print(_("�ٰ��ʤ�¸�ߤ������Ƥ���褦�ʴ��������롪", "You feel safe from evil!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->protevil)
		{
			msg_print(_("�ٰ��ʤ�¸�ߤ������Ƥ��봶�����ʤ��ʤä���", "You no longer feel safe from evil."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->protevil = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ͩ�β��η�³���֤򥻥åȤ��� / Set "p_ptr->wraith_form", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_wraith_form(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->wraith_form && !do_dec)
		{
			if (p_ptr->wraith_form > v) return FALSE;
		}
		else if (!p_ptr->wraith_form)
		{
			msg_print(_("ʪ������Υ���ͩ���Τ褦��¸�ߤˤʤä���", "You leave the physical world and turn into a wraith-being!"));
			notice = TRUE;
			chg_virtue(V_UNLIFE, 3);
			chg_virtue(V_HONOUR, -2);
			chg_virtue(V_SACRIFICE, -2);
			chg_virtue(V_VALOUR, -5);

			/* Redraw map */
			p_ptr->redraw |= (PR_MAP);

			/* Update monsters */
			p_ptr->update |= (PU_MONSTERS);

			/* Window stuff */
			p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->wraith_form)
		{
			msg_print(_("��Ʃ���ˤʤä����������롣", "You feel opaque."));
			notice = TRUE;

			/* Redraw map */
			p_ptr->redraw |= (PR_MAP);

			/* Update monsters */
			p_ptr->update |= (PU_MONSTERS);

			/* Window stuff */
			p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);
		}
	}

	/* Use the value */
	p_ptr->wraith_form = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);

}

/*!
 * @brief ̵����η�³���֤򥻥åȤ��� / Set "p_ptr->invuln", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_invuln(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->invuln && !do_dec)
		{
			if (p_ptr->invuln > v) return FALSE;
		}
		else if (!IS_INVULN())
		{
			msg_print(_("̵Ũ����", "Invulnerability!"));
			notice = TRUE;

			chg_virtue(V_UNLIFE, -2);
			chg_virtue(V_HONOUR, -2);
			chg_virtue(V_SACRIFICE, -3);
			chg_virtue(V_VALOUR, -5);

			/* Redraw map */
			p_ptr->redraw |= (PR_MAP);

			/* Update monsters */
			p_ptr->update |= (PU_MONSTERS);

			/* Window stuff */
			p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->invuln && !music_singing(MUSIC_INVULN))
		{
			msg_print(_("̵Ũ�ǤϤʤ��ʤä���", "The invulnerability wears off."));
			notice = TRUE;

			/* Redraw map */
			p_ptr->redraw |= (PR_MAP);

			/* Update monsters */
			p_ptr->update |= (PU_MONSTERS);

			/* Window stuff */
			p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);

			p_ptr->energy_need += ENERGY_NEED();
		}
	}

	/* Use the value */
	p_ptr->invuln = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ����ESP�η�³���֤򥻥åȤ��� / Set "p_ptr->tim_esp", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_tim_esp(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->tim_esp && !do_dec)
		{
			if (p_ptr->tim_esp > v) return FALSE;
		}
		else if (!IS_TIM_ESP())
		{
			msg_print(_("�ռ��������ä��������롪", "You feel your consciousness expand!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->tim_esp && !music_singing(MUSIC_MIND))
		{
			msg_print(_("�ռ��ϸ�����ä���", "Your consciousness contracts again."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->tim_esp = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Update the monsters */
	p_ptr->update |= (PU_MONSTERS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ����Ʃ����η�³���֤򥻥åȤ��� / Set "p_ptr->tim_invis", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_tim_invis(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->tim_invis && !do_dec)
		{
			if (p_ptr->tim_invis > v) return FALSE;
		}
		else if (!p_ptr->tim_invis)
		{
			msg_print(_("�ܤ������Ҵ��ˤʤä��������롪", "Your eyes feel very sensitive!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->tim_invis)
		{
			msg_print(_("�ܤ��Ҵ������ʤ��ʤä��褦����", "Your eyes feel less sensitive."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->tim_invis = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Update the monsters */
	p_ptr->update |= (PU_MONSTERS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief �����ֳ������Ϥη�³���֤򥻥åȤ��� / Set "p_ptr->tim_infra", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_tim_infra(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->tim_infra && !do_dec)
		{
			if (p_ptr->tim_infra > v) return FALSE;
		}
		else if (!p_ptr->tim_infra)
		{
			msg_print(_("�ܤ������ȵ����Ϥ᤿��", "Your eyes begin to tingle!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->tim_infra)
		{
			msg_print(_("�ܤε������ʤ��ʤä���", "Your eyes stop tingling."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->tim_infra = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Update the monsters */
	p_ptr->update |= (PU_MONSTERS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ���µ޲����η�³���֤򥻥åȤ��� / Set "p_ptr->tim_regen", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_tim_regen(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->tim_regen && !do_dec)
		{
			if (p_ptr->tim_regen > v) return FALSE;
		}
		else if (!p_ptr->tim_regen)
		{
			msg_print(_("�����Ϥ��夬�ä���", "You feel yourself regenerating quickly!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->tim_regen)
		{
			msg_print(_("���᤯�������봶�����ʤ��ʤä���", "You feel yourself regenerating slowly."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->tim_regen = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ��̩�βΤη�³���֤򥻥åȤ��� / Set "p_ptr->tim_stealth", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_tim_stealth(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->tim_stealth && !do_dec)
		{
			if (p_ptr->tim_stealth > v) return FALSE;
		}
		else if (!IS_TIM_STEALTH())
		{
			msg_print(_("­�����������ʤä���", "You begin to walk silently!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->tim_stealth && !music_singing(MUSIC_STEALTH))
		{
			msg_print(_("­�����礭���ʤä���", "You no longer walk silently."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->tim_stealth = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief Ķ��̩���֤򥻥åȤ���
 * @param set TRUE�ʤ��Ķ��̩���֤ˤʤ롣
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_superstealth(bool set)
{
	bool notice = FALSE;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (set)
	{
		if (!(p_ptr->special_defense & NINJA_S_STEALTH))
		{
			if (cave[p_ptr->y][p_ptr->x].info & CAVE_MNLT)
			{
				msg_print(_("Ũ���ܤ��������Ƥ����ʤ�������줿��", "You are mantled in weak shadow from ordinary eyes."));
				p_ptr->monlite = p_ptr->old_monlite = TRUE;
			}
			else
			{
				msg_print(_("Ũ���ܤ���Ƥ����ʤ�������줿��", "You are mantled in shadow from ordinary eyes!"));
				p_ptr->monlite = p_ptr->old_monlite = FALSE;
			}

			notice = TRUE;

			/* Use the value */
			p_ptr->special_defense |= NINJA_S_STEALTH;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->special_defense & NINJA_S_STEALTH)
		{
			msg_print(_("�Ƥ�Ũ���ܤˤ��餵���褦�ˤʤä���", "You are exposed to common sight once more."));
			notice = TRUE;

			/* Use the value */
			p_ptr->special_defense &= ~(NINJA_S_STEALTH);
		}
	}

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Result */
	return (TRUE);
}

/*!
 * @brief ���Ū��ͷ�η�³���֤򥻥åȤ��� / Set "p_ptr->tim_levitation", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_tim_levitation(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->tim_levitation && !do_dec)
		{
			if (p_ptr->tim_levitation > v) return FALSE;
		}
		else if (!p_ptr->tim_levitation)
		{
			msg_print(_("�Τ�����⤭�Ϥ᤿��", "You begin to fly!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->tim_levitation)
		{
			msg_print(_("�⤦����⤫�٤ʤ��ʤä���", "You stop flying."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->tim_levitation = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ���ŪƮ���Υ�����η�³���֤򥻥åȤ��� / Set "p_ptr->tim_sh_touki", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_tim_sh_touki(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->tim_sh_touki && !do_dec)
		{
			if (p_ptr->tim_sh_touki > v) return FALSE;
		}
		else if (!p_ptr->tim_sh_touki)
		{
			msg_print(_("�Τ�Ʈ���Υ������ʤ��줿��", "You have enveloped by the aura of the Force!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->tim_sh_touki)
		{
			msg_print(_("Ʈ�����ä�����", "Aura of the Force disappeared."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->tim_sh_touki = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ���Ū�б�Υ�����η�³���֤򥻥åȤ��� / Set "p_ptr->tim_sh_fire", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_tim_sh_fire(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->tim_sh_fire && !do_dec)
		{
			if (p_ptr->tim_sh_fire > v) return FALSE;
		}
		else if (!p_ptr->tim_sh_fire)
		{
			msg_print(_("�Τ���Υ������ʤ��줿��", "You have enveloped by fiery aura!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->tim_sh_fire)
		{
			msg_print(_("��Υ����餬�ä�����", "Fiery aura disappeared."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->tim_sh_fire = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ���Ū���ʤ�Υ�����η�³���֤򥻥åȤ��� / Set "p_ptr->tim_sh_holy", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_tim_sh_holy(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->tim_sh_holy && !do_dec)
		{
			if (p_ptr->tim_sh_holy > v) return FALSE;
		}
		else if (!p_ptr->tim_sh_holy)
		{
			msg_print(_("�Τ����ʤ륪�����ʤ��줿��", "You have enveloped by holy aura!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->tim_sh_holy)
		{
			msg_print(_("���ʤ륪���餬�ä�����", "Holy aura disappeared."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->tim_sh_holy = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief �ܤˤ��ܤ�λĤ���֤򥻥åȤ��� / Set "p_ptr->tim_eyeeye", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_tim_eyeeye(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->tim_eyeeye && !do_dec)
		{
			if (p_ptr->tim_eyeeye > v) return FALSE;
		}
		else if (!p_ptr->tim_eyeeye)
		{
			msg_print(_("ˡ�μ���ˤʤä�����������", "You feel like a keeper of commandments!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->tim_eyeeye)
		{
			msg_print(_("Ĩȳ�򼹹Ԥ��뤳�Ȥ��Ǥ��ʤ��ʤä���", "You no longer feel like a keeper."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->tim_eyeeye = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}


/*!
 * @brief ���Ū��ˡ�ɸ�η�³���֤򥻥åȤ��� / Set "p_ptr->resist_magic", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_resist_magic(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->resist_magic && !do_dec)
		{
			if (p_ptr->resist_magic > v) return FALSE;
		}
		else if (!p_ptr->resist_magic)
		{
			msg_print(_("��ˡ�ؤ��������Ĥ�����", "You have been protected from magic!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->resist_magic)
		{
			msg_print(_("��ˡ�˼夯�ʤä���", "You are no longer protected from magic."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->resist_magic = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ���Ūȿ�ͤη�³���֤򥻥åȤ��� / Set "p_ptr->tim_reflect", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_tim_reflect(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->tim_reflect && !do_dec)
		{
			if (p_ptr->tim_reflect > v) return FALSE;
		}
		else if (!p_ptr->tim_reflect)
		{
			msg_print(_("�Τ�ɽ�̤��꤫�ˤʤä��������롣", "Your body becames smooth."));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->tim_reflect)
		{
			msg_print(_("�Τ�ɽ�̤��꤫�Ǥʤ��ʤä���", "Your body is no longer smooth."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->tim_reflect = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}


/*
 * Set "p_ptr->multishadow", notice observable changes
 */
bool set_multishadow(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->multishadow && !do_dec)
		{
			if (p_ptr->multishadow > v) return FALSE;
		}
		else if (!p_ptr->multishadow)
		{
			msg_print(_("���ʤ��μ���˸��Ƥ����ޤ줿��", "Your Shadow enveloped you."));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->multishadow)
		{
			msg_print(_("���Ƥ��ä�����", "Your Shadow disappears."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->multishadow = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ���Ū���ҤΥ�����η�³���֤򥻥åȤ��� / Set "p_ptr->dustrobe", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_dustrobe(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->dustrobe && !do_dec)
		{
			if (p_ptr->dustrobe > v) return FALSE;
		}
		else if (!p_ptr->dustrobe)
		{
			msg_print(_("�Τ����Υ������ʤ��줿��", "You were enveloped by mirror shards."));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->dustrobe)
		{
			msg_print(_("���Υ����餬�ä�����", "The mirror shards disappear."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->dustrobe = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ���Ū��ȴ���η�³���֤򥻥åȤ��� / Set "p_ptr->kabenuke", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_kabenuke(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->kabenuke && !do_dec)
		{
			if (p_ptr->kabenuke > v) return FALSE;
		}
		else if (!p_ptr->kabenuke)
		{
			msg_print(_("�Τ�Ⱦʪ���ξ��֤ˤʤä���", "You became ethereal form."));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->kabenuke)
		{
			msg_print(_("�Τ�ʪ����������", "You are no longer in an ethereal form."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->kabenuke = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief �����췻����η�³���֤򥻥åȤ��� / Set "p_ptr->tsuyoshi", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_tsuyoshi(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->tsuyoshi && !do_dec)
		{
			if (p_ptr->tsuyoshi > v) return FALSE;
		}
		else if (!p_ptr->tsuyoshi)
		{
			msg_print(_("�֥����췻���󡪡�", "Brother OKURE!"));
			notice = TRUE;
			chg_virtue(V_VITALITY, 2);
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->tsuyoshi)
		{
			msg_print(_("���Τ���®�ˤ��ܤ�Ǥ��ä���", "Your body had quickly shriveled."));

			(void)dec_stat(A_CON, 20, TRUE);
			(void)dec_stat(A_STR, 20, TRUE);

			notice = TRUE;
			chg_virtue(V_VITALITY, -3);
		}
	}

	/* Use the value */
	p_ptr->tsuyoshi = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Recalculate hitpoints */
	p_ptr->update |= (PU_HP);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ���Ū���ǥ��쥤�η�³���֤򥻥åȤ��� / Set a temporary elemental brand. Clear all other brands. Print status messages. -LM-
 * @param attack_type ���쥤�Υ�����ID
 * @param v ��³����
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_ele_attack(u32b attack_type, TIME_EFFECT v)
{
	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	/* Clear all elemental attacks (only one is allowed at a time). */
	if ((p_ptr->special_attack & (ATTACK_ACID)) && (attack_type != ATTACK_ACID))
	{
		p_ptr->special_attack &= ~(ATTACK_ACID);
		msg_print(_("���ǹ���Ǥ��ʤ��ʤä���", "Your temporary acidic brand fades away."));
	}
	if ((p_ptr->special_attack & (ATTACK_ELEC)) && (attack_type != ATTACK_ELEC))
	{
		p_ptr->special_attack &= ~(ATTACK_ELEC);
		msg_print(_("�ŷ�ǹ���Ǥ��ʤ��ʤä���", "Your temporary electrical brand fades away."));
	}
	if ((p_ptr->special_attack & (ATTACK_FIRE)) && (attack_type != ATTACK_FIRE))
	{
		p_ptr->special_attack &= ~(ATTACK_FIRE);
		msg_print(_("�б�ǹ���Ǥ��ʤ��ʤä���", "Your temporary fiery brand fades away."));
	}
	if ((p_ptr->special_attack & (ATTACK_COLD)) && (attack_type != ATTACK_COLD))
	{
		p_ptr->special_attack &= ~(ATTACK_COLD);
		msg_print(_("�䵤�ǹ���Ǥ��ʤ��ʤä���", "Your temporary frost brand fades away."));
	}
	if ((p_ptr->special_attack & (ATTACK_POIS)) && (attack_type != ATTACK_POIS))
	{
		p_ptr->special_attack &= ~(ATTACK_POIS);
		msg_print(_("�Ǥǹ���Ǥ��ʤ��ʤä���", "Your temporary poison brand fades away."));
	}

	if ((v) && (attack_type))
	{
		/* Set attack type. */
		p_ptr->special_attack |= (attack_type);

		/* Set duration. */
		p_ptr->ele_attack = v;

		/* Message. */
#ifdef JP
		msg_format("%s�ǹ���Ǥ���褦�ˤʤä���",
			     ((attack_type == ATTACK_ACID) ? "��" :
			      ((attack_type == ATTACK_ELEC) ? "�ŷ�" :
			       ((attack_type == ATTACK_FIRE) ? "�б�" : 
				((attack_type == ATTACK_COLD) ? "�䵤" : 
				 ((attack_type == ATTACK_POIS) ? "��" : 
					"(�ʤ�)"))))));
#else
		msg_format("For a while, the blows you deal will %s",
			     ((attack_type == ATTACK_ACID) ? "melt with acid!" :
			      ((attack_type == ATTACK_ELEC) ? "shock your foes!" :
			       ((attack_type == ATTACK_FIRE) ? "burn with fire!" : 
				((attack_type == ATTACK_COLD) ? "chill to the bone!" : 
				 ((attack_type == ATTACK_POIS) ? "poison your enemies!" : 
					"do nothing special."))))));
#endif
	}

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	return (TRUE);
}

/*!
 * @brief ���Ū�����ȱ֤η�³���֤򥻥åȤ��� / Set a temporary elemental brand.  Clear all other brands.  Print status messages. -LM-
 * @param immune_type �ȱ֤Υ�����ID
 * @param v ��³����
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_ele_immune(u32b immune_type, TIME_EFFECT v)
{
	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	/* Clear all elemental attacks (only one is allowed at a time). */
	if ((p_ptr->special_defense & (DEFENSE_ACID)) && (immune_type != DEFENSE_ACID))
	{
		p_ptr->special_defense &= ~(DEFENSE_ACID);
		msg_print(_("���ι���ǽ��Ĥ�����褦�ˤʤä�����", "You are no longer immune to acid."));
	}
	if ((p_ptr->special_defense & (DEFENSE_ELEC)) && (immune_type != DEFENSE_ELEC))
	{
		p_ptr->special_defense &= ~(DEFENSE_ELEC);
		msg_print(_("�ŷ�ι���ǽ��Ĥ�����褦�ˤʤä�����", "You are no longer immune to electricity."));
	}
	if ((p_ptr->special_defense & (DEFENSE_FIRE)) && (immune_type != DEFENSE_FIRE))
	{
		p_ptr->special_defense &= ~(DEFENSE_FIRE);
		msg_print(_("�б�ι���ǽ��Ĥ�����褦�ˤʤä�����", "You are no longer immune to fire."));
	}
	if ((p_ptr->special_defense & (DEFENSE_COLD)) && (immune_type != DEFENSE_COLD))
	{
		p_ptr->special_defense &= ~(DEFENSE_COLD);
		msg_print(_("�䵤�ι���ǽ��Ĥ�����褦�ˤʤä�����", "You are no longer immune to cold."));
	}
	if ((p_ptr->special_defense & (DEFENSE_POIS)) && (immune_type != DEFENSE_POIS))
	{
		p_ptr->special_defense &= ~(DEFENSE_POIS);
		msg_print(_("�Ǥι���ǽ��Ĥ�����褦�ˤʤä�����", "You are no longer immune to poison."));
	}

	if ((v) && (immune_type))
	{
		/* Set attack type. */
		p_ptr->special_defense |= (immune_type);

		/* Set duration. */
		p_ptr->ele_immune = v;

		/* Message. */
#ifdef JP
		msg_format("%s�ι��������Ĥ��ʤ��ʤä���",
			     ((immune_type == DEFENSE_ACID) ? "��" :
			      ((immune_type == DEFENSE_ELEC) ? "�ŷ�" :
			       ((immune_type == DEFENSE_FIRE) ? "�б�" : 
				((immune_type == DEFENSE_COLD) ? "�䵤" : 
				 ((immune_type == DEFENSE_POIS) ? "��" : 
					"(�ʤ�)"))))));
#else
		msg_format("For a while, You are immune to %s",
			     ((immune_type == DEFENSE_ACID) ? "acid!" :
			      ((immune_type == DEFENSE_ELEC) ? "electricity!" :
			       ((immune_type == DEFENSE_FIRE) ? "fire!" : 
				((immune_type == DEFENSE_COLD) ? "cold!" : 
				 ((immune_type == DEFENSE_POIS) ? "poison!" : 
					"do nothing special."))))));
#endif
	}

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	return (TRUE);
}

/*!
 * @brief ���Ū�������η�³���֤򥻥åȤ��� / Set "p_ptr->oppose_acid", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_oppose_acid(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->oppose_acid && !do_dec)
		{
			if (p_ptr->oppose_acid > v) return FALSE;
		}
		else if (!IS_OPPOSE_ACID())
		{
			msg_print(_("���ؤ��������Ĥ����������롪", "You feel resistant to acid!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->oppose_acid && !music_singing(MUSIC_RESIST) && !(p_ptr->special_defense & KATA_MUSOU))
		{
			msg_print(_("���ؤ����������줿�������롣", "You feel less resistant to acid."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->oppose_acid = v;

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ���Ū�ŷ������η�³���֤򥻥åȤ��� / Set "p_ptr->oppose_elec", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_oppose_elec(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->oppose_elec && !do_dec)
		{
			if (p_ptr->oppose_elec > v) return FALSE;
		}
		else if (!IS_OPPOSE_ELEC())
		{
			msg_print(_("�ŷ�ؤ��������Ĥ����������롪", "You feel resistant to electricity!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->oppose_elec && !music_singing(MUSIC_RESIST) && !(p_ptr->special_defense & KATA_MUSOU))
		{
			msg_print(_("�ŷ�ؤ����������줿�������롣", "You feel less resistant to electricity."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->oppose_elec = v;

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ���Ū�б������η�³���֤򥻥åȤ��� / Set "p_ptr->oppose_fire", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_oppose_fire(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	if ((prace_is_(RACE_DEMON) && (p_ptr->lev > 44)) || (p_ptr->mimic_form == MIMIC_DEMON)) v = 1;
	/* Open */
	if (v)
	{
		if (p_ptr->oppose_fire && !do_dec)
		{
			if (p_ptr->oppose_fire > v) return FALSE;
		}
		else if (!IS_OPPOSE_FIRE())
		{
			msg_print(_("�Фؤ��������Ĥ����������롪", "You feel resistant to fire!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->oppose_fire && !music_singing(MUSIC_RESIST) && !(p_ptr->special_defense & KATA_MUSOU))
		{
			msg_print(_("�Фؤ����������줿�������롣", "You feel less resistant to fire."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->oppose_fire = v;

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ���Ū�䵤�����η�³���֤򥻥åȤ��� / Set "p_ptr->oppose_cold", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_oppose_cold(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->oppose_cold && !do_dec)
		{
			if (p_ptr->oppose_cold > v) return FALSE;
		}
		else if (!IS_OPPOSE_COLD())
		{
			msg_print(_("�䵤�ؤ��������Ĥ����������롪", "You feel resistant to cold!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->oppose_cold && !music_singing(MUSIC_RESIST) && !(p_ptr->special_defense & KATA_MUSOU))
		{
			msg_print(_("�䵤�ؤ����������줿�������롣", "You feel less resistant to cold."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->oppose_cold = v;

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ���Ū�������η�³���֤򥻥åȤ��� / Set "p_ptr->oppose_pois", notice observable changes
 * @param v ��³����
 * @param do_dec ���ߤη�³���֤��Ĺ���ͤΤ߾�񤭤���
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 */
bool set_oppose_pois(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if ((p_ptr->pclass == CLASS_NINJA) && (p_ptr->lev > 44)) v = 1;
	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->oppose_pois && !do_dec)
		{
			if (p_ptr->oppose_pois > v) return FALSE;
		}
		else if (!IS_OPPOSE_POIS())
		{
			msg_print(_("�Ǥؤ��������Ĥ����������롪", "You feel resistant to poison!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->oppose_pois && !music_singing(MUSIC_RESIST) && !(p_ptr->special_defense & KATA_MUSOU))
		{
			msg_print(_("�Ǥؤ����������줿�������롣", "You feel less resistant to poison."));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->oppose_pois = v;

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ۯ۰�η�³���֤򥻥åȤ��� / Set "p_ptr->stun", notice observable changes
 * @param v ��³����
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 * @details
 * Note the special code to only notice "range" changes.
 */
bool set_stun(TIME_EFFECT v)
{
	int old_aux, new_aux;
	bool notice = FALSE;


	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	if (prace_is_(RACE_GOLEM) || ((p_ptr->pclass == CLASS_BERSERKER) && (p_ptr->lev > 34))) v = 0;

	/* Knocked out */
	if (p_ptr->stun > 100)
	{
		old_aux = 3;
	}

	/* Heavy stun */
	else if (p_ptr->stun > 50)
	{
		old_aux = 2;
	}

	/* Stun */
	else if (p_ptr->stun > 0)
	{
		old_aux = 1;
	}

	/* None */
	else
	{
		old_aux = 0;
	}

	/* Knocked out */
	if (v > 100)
	{
		new_aux = 3;
	}

	/* Heavy stun */
	else if (v > 50)
	{
		new_aux = 2;
	}

	/* Stun */
	else if (v > 0)
	{
		new_aux = 1;
	}

	/* None */
	else
	{
		new_aux = 0;
	}

	/* Increase cut */
	if (new_aux > old_aux)
	{
		/* Describe the state */
		switch (new_aux)
		{
			/* Stun */
			case 1: msg_print(_("�ռ����⤦���Ȥ��Ƥ�����", "You have been stunned.")); break;

			/* Heavy stun */
			case 2: msg_print(_("�ռ����Ҥɤ��⤦���Ȥ��Ƥ�����", "You have been heavily stunned.")); break;

			/* Knocked out */
			case 3: msg_print(_("Ƭ�����饯�餷�ưռ�����Τ��Ƥ�����", "You have been knocked out.")); break;
		}

		if (randint1(1000) < v || one_in_(16))
		{
			msg_print(_("����褦��Ƭ�ˤ����롣", "A vicious blow hits your head."));

			if (one_in_(3))
			{
				if (!p_ptr->sustain_int) (void)do_dec_stat(A_INT);
				if (!p_ptr->sustain_wis) (void)do_dec_stat(A_WIS);
			}
			else if (one_in_(2))
			{
				if (!p_ptr->sustain_int) (void)do_dec_stat(A_INT);
			}
			else
			{
				if (!p_ptr->sustain_wis) (void)do_dec_stat(A_WIS);
			}
		}
		if (p_ptr->special_defense & KATA_MASK)
		{
			msg_print(_("�������줿��", "Your posture gets loose."));
			p_ptr->special_defense &= ~(KATA_MASK);
			p_ptr->update |= (PU_BONUS);
			p_ptr->update |= (PU_MONSTERS);
			p_ptr->redraw |= (PR_STATE);
			p_ptr->redraw |= (PR_STATUS);
			p_ptr->action = ACTION_NONE;
		}

		/* Sniper */
		if (p_ptr->concent) reset_concentration(TRUE);

		/* Hex */
		if (hex_spelling_any()) stop_hex_spell_all();

		/* Notice */
		notice = TRUE;
	}

	/* Decrease cut */
	else if (new_aux < old_aux)
	{
		/* Describe the state */
		switch (new_aux)
		{
			/* None */
		  case 0:
			msg_print(_("��ä�ۯ۰���֤������������", "You are no longer stunned."));

			if (disturb_state) disturb(0, 0);
			break;
		}

		/* Notice */
		notice = TRUE;
	}

	/* Use the value */
	p_ptr->stun = v;

	/* No change */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Redraw the "stun" */
	p_ptr->redraw |= (PR_STUN);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}


/*!
 * @brief �з�η�³���֤򥻥åȤ��� / Set "p_ptr->cut", notice observable changes
 * @param v ��³����
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 * @details
 * Note the special code to only notice "range" changes.
 */
bool set_cut(TIME_EFFECT v)
{
	int old_aux, new_aux;
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	if ((p_ptr->prace == RACE_GOLEM ||
	    p_ptr->prace == RACE_SKELETON ||
	    p_ptr->prace == RACE_SPECTRE ||
		(p_ptr->prace == RACE_ZOMBIE && p_ptr->lev > 11)) &&
	    !p_ptr->mimic_form)
		v = 0;

	/* Mortal wound */
	if (p_ptr->cut > 1000)
	{
		old_aux = 7;
	}

	/* Deep gash */
	else if (p_ptr->cut > 200)
	{
		old_aux = 6;
	}

	/* Severe cut */
	else if (p_ptr->cut > 100)
	{
		old_aux = 5;
	}

	/* Nasty cut */
	else if (p_ptr->cut > 50)
	{
		old_aux = 4;
	}

	/* Bad cut */
	else if (p_ptr->cut > 25)
	{
		old_aux = 3;
	}

	/* Light cut */
	else if (p_ptr->cut > 10)
	{
		old_aux = 2;
	}

	/* Graze */
	else if (p_ptr->cut > 0)
	{
		old_aux = 1;
	}

	/* None */
	else
	{
		old_aux = 0;
	}

	/* Mortal wound */
	if (v > 1000)
	{
		new_aux = 7;
	}

	/* Deep gash */
	else if (v > 200)
	{
		new_aux = 6;
	}

	/* Severe cut */
	else if (v > 100)
	{
		new_aux = 5;
	}

	/* Nasty cut */
	else if (v > 50)
	{
		new_aux = 4;
	}

	/* Bad cut */
	else if (v > 25)
	{
		new_aux = 3;
	}

	/* Light cut */
	else if (v > 10)
	{
		new_aux = 2;
	}

	/* Graze */
	else if (v > 0)
	{
		new_aux = 1;
	}

	/* None */
	else
	{
		new_aux = 0;
	}

	/* Increase cut */
	if (new_aux > old_aux)
	{
		/* Describe the state */
		switch (new_aux)
		{
			/* Graze */
			case 1: msg_print(_("�����������äƤ��ޤä���", "You have been given a graze.")); break;

			/* Light cut */
			case 2: msg_print(_("�ڤ�������äƤ��ޤä���", "You have been given a light cut.")); break;

			/* Bad cut */
			case 3: msg_print(_("�Ҥɤ�������äƤ��ޤä���", "You have been given a bad cut.")); break;

			/* Nasty cut */
			case 4: msg_print(_("���Ѥʽ�����äƤ��ޤä���", "You have been given a nasty cut.")); break;

			/* Severe cut */
			case 5: msg_print(_("����ʽ�����äƤ��ޤä���", "You have been given a severe cut.")); break;

			/* Deep gash */
			case 6: msg_print(_("�Ҥɤ��������äƤ��ޤä���", "You have been given a deep gash.")); break;

			/* Mortal wound */
			case 7: msg_print(_("��̿Ū�ʽ�����äƤ��ޤä���", "You have been given a mortal wound.")); break;
		}

		/* Notice */
		notice = TRUE;

		if (randint1(1000) < v || one_in_(16))
		{
			if (!p_ptr->sustain_chr)
			{
				msg_print(_("�Ҥɤ����פ��ĤäƤ��ޤä���", "You have been horribly scarred."));
				do_dec_stat(A_CHR);
			}
		}
	}

	/* Decrease cut */
	else if (new_aux < old_aux)
	{
		/* Describe the state */
		switch (new_aux)
		{
			/* None */
			case 0:
			msg_format(_("��ä�%s��", "You are no longer bleeding."), p_ptr->prace == RACE_ANDROID ? "���椬ľ�ä�" : "�з줬�ߤޤä�");

			if (disturb_state) disturb(0, 0);
			break;
		}

		/* Notice */
		notice = TRUE;
	}

	/* Use the value */
	p_ptr->cut = v;

	/* No change */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Redraw the "cut" */
	p_ptr->redraw |= (PR_CUT);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief ��ʢ���֤򥻥åȤ��� / Set "p_ptr->food", notice observable changes
 * @param v ��³����
 * @return ���ơ������˱ƶ���ڤܤ��Ѳ������ä����TRUE���֤���
 * @details
 * Set "", notice observable changes\n
 *\n
 * The "p_ptr->food" variable can get as large as 20000, allowing the
 * addition of the most "filling" item, Elvish Waybread, which adds
 * 7500 food units, without overflowing the 32767 maximum limit.\n
 *\n
 * Perhaps we should disturb the player with various messages,
 * especially messages about hunger status changes.  XXX XXX XXX\n
 *\n
 * Digestion of food is handled in "dungeon.c", in which, normally,
 * the player digests about 20 food units per 100 game turns, more
 * when "fast", more when "regenerating", less with "slow digestion",
 * but when the player is "gorged", he digests 100 food units per 10
 * game turns, or a full 1000 food units per 100 game turns.\n
 *\n
 * Note that the player's speed is reduced by 10 units while gorged,
 * so if the player eats a single food ration (5000 food units) when
 * full (15000 food units), he will be gorged for (5000/100)*10 = 500
 * game turns, or 500/(100/5) = 25 player turns (if nothing else is
 * affecting the player speed).\n
 */
bool set_food(TIME_EFFECT v)
{
	int old_aux, new_aux;

	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 20000) ? 20000 : (v < 0) ? 0 : v;

	/* Fainting / Starving */
	if (p_ptr->food < PY_FOOD_FAINT)
	{
		old_aux = 0;
	}

	/* Weak */
	else if (p_ptr->food < PY_FOOD_WEAK)
	{
		old_aux = 1;
	}

	/* Hungry */
	else if (p_ptr->food < PY_FOOD_ALERT)
	{
		old_aux = 2;
	}

	/* Normal */
	else if (p_ptr->food < PY_FOOD_FULL)
	{
		old_aux = 3;
	}

	/* Full */
	else if (p_ptr->food < PY_FOOD_MAX)
	{
		old_aux = 4;
	}

	/* Gorged */
	else
	{
		old_aux = 5;
	}

	/* Fainting / Starving */
	if (v < PY_FOOD_FAINT)
	{
		new_aux = 0;
	}

	/* Weak */
	else if (v < PY_FOOD_WEAK)
	{
		new_aux = 1;
	}

	/* Hungry */
	else if (v < PY_FOOD_ALERT)
	{
		new_aux = 2;
	}

	/* Normal */
	else if (v < PY_FOOD_FULL)
	{
		new_aux = 3;
	}

	/* Full */
	else if (v < PY_FOOD_MAX)
	{
		new_aux = 4;
	}

	/* Gorged */
	else
	{
		new_aux = 5;
	}

	if (old_aux < 1 && new_aux > 0)
		chg_virtue(V_PATIENCE, 2);
	else if (old_aux < 3 && (old_aux != new_aux))
		chg_virtue(V_PATIENCE, 1);
	if (old_aux == 2)
		chg_virtue(V_TEMPERANCE, 1);
	if (old_aux == 0)
		chg_virtue(V_TEMPERANCE, -1);

	/* Food increase */
	if (new_aux > old_aux)
	{
		/* Describe the state */
		switch (new_aux)
		{
			/* Weak */
			case 1: msg_print(_("�ޤ���ʢ���ݤ줽������", "You are still weak.")); break;

			/* Hungry */
			case 2: msg_print(_("�ޤ���ʢ����", "You are still hungry.")); break;

			/* Normal */
			case 3: msg_print(_("��ʢ���������ޤä���", "You are no longer hungry.")); break;

			/* Full */
			case 4: msg_print(_("��ʢ����", "You are full!")); break;

			/* Bloated */
			case 5:
			msg_print(_("���ٲ᤮����", "You have gorged yourself!"));
			chg_virtue(V_HARMONY, -1);
			chg_virtue(V_PATIENCE, -1);
			chg_virtue(V_TEMPERANCE, -2);

			break;
		}

		/* Change */
		notice = TRUE;
	}

	/* Food decrease */
	else if (new_aux < old_aux)
	{
		/* Describe the state */
		switch (new_aux)
		{
			/* Fainting / Starving */
			case 0: msg_print(_("���ޤ�ˤ��ʢ�ǵ��򼺤äƤ��ޤä���", "You are getting faint from hunger!")); break;

			/* Weak */
			case 1: msg_print(_("��ʢ���������ݤ줽������", "You are getting weak from hunger!")); break;

			/* Hungry */
			case 2: msg_print(_("��ʢ�������Ƥ�����", "You are getting hungry.")); break;

			/* Normal */
			case 3: msg_print(_("��ʢ�����ʤ��ʤä���", "You are no longer full.")); break;

			/* Full */
			case 4: msg_print(_("��äȤ�ʢ�����Ĥ��ʤ��ʤä���", "You are no longer gorged.")); break;
		}

		if (p_ptr->wild_mode && (new_aux < 2))
		{
			change_wild_mode();
		}

		/* Change */
		notice = TRUE;
	}

	/* Use the value */
	p_ptr->food = v;

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Redraw hunger */
	p_ptr->redraw |= (PR_HUNGER);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

/*!
 * @brief �ץ쥤�䡼�δ���ǽ���ͤ����ä����� / Increases a stat by one randomized level -RAK-
 * @param stat �徺�����륹�ơ�����ID
 * @return �ºݤ˾徺�������TRUE���֤���
 * @details
 * Note that this function (used by stat potions) now restores\n
 * the stat BEFORE increasing it.\n
 */
bool inc_stat(int stat)
{
	BASE_STATUS value, gain;

	/* Then augment the current/max stat */
	value = p_ptr->stat_cur[stat];

	/* Cannot go above 18/100 */
	if (value < p_ptr->stat_max_max[stat])
	{
		/* Gain one (sometimes two) points */
		if (value < 18)
		{
			gain = ((randint0(100) < 75) ? 1 : 2);
			value += gain;
		}

		/* Gain 1/6 to 1/3 of distance to 18/100 */
		else if (value < (p_ptr->stat_max_max[stat]-2))
		{
			/* Approximate gain value */
			gain = (((p_ptr->stat_max_max[stat]) - value) / 2 + 3) / 2;

			/* Paranoia */
			if (gain < 1) gain = 1;

			/* Apply the bonus */
			value += randint1(gain) + gain / 2;

			/* Maximal value */
			if (value > (p_ptr->stat_max_max[stat]-1)) value = p_ptr->stat_max_max[stat]-1;
		}

		/* Gain one point at a time */
		else
		{
			value++;
		}

		/* Save the new value */
		p_ptr->stat_cur[stat] = value;

		/* Bring up the maximum too */
		if (value > p_ptr->stat_max[stat])
		{
			p_ptr->stat_max[stat] = value;
		}

		/* Recalculate bonuses */
		p_ptr->update |= (PU_BONUS);

		/* Success */
		return (TRUE);
	}

	/* Nothing to gain */
	return (FALSE);
}

/*!
 * @brief �ץ쥤�䡼�δ���ǽ���ͤ򸺾������� / Decreases a stat by an amount indended to vary from 0 to 100 percent.
 * @param stat ���������륹�ơ�����ID
 * @param amount ���������������
 * @param permanent TRUE�ʤ�и��ߤκ����ͤ򸺾�������
 * @return �ºݤ˸����������TRUE���֤���
 * @details
 *\n
 * Amount could be a little higher in extreme cases to mangle very high\n
 * stats from massive assaults.  -CWS\n
 *\n
 * Note that "permanent" means that the *given* amount is permanent,\n
 * not that the new value becomes permanent.  This may not work exactly\n
 * as expected, due to "weirdness" in the algorithm, but in general,\n
 * if your stat is already drained, the "max" value will not drop all\n
 * the way down to the "cur" value.\n
 */
bool dec_stat(int stat, int amount, int permanent)
{
	BASE_STATUS cur, max;
	int loss, same;
	bool res = FALSE;


	/* Acquire current value */
	cur = p_ptr->stat_cur[stat];
	max = p_ptr->stat_max[stat];

	/* Note when the values are identical */
	same = (cur == max);

	/* Damage "current" value */
	if (cur > 3)
	{
		/* Handle "low" values */
		if (cur <= 18)
		{
			if (amount > 90) cur--;
			if (amount > 50) cur--;
			if (amount > 20) cur--;
			cur--;
		}

		/* Handle "high" values */
		else
		{
			/* Hack -- Decrement by a random amount between one-quarter */
			/* and one-half of the stat bonus times the percentage, with a */
			/* minimum damage of half the percentage. -CWS */
			loss = (((cur-18) / 2 + 1) / 2 + 1);

			/* Paranoia */
			if (loss < 1) loss = 1;

			/* Randomize the loss */
			loss = ((randint1(loss) + loss) * amount) / 100;

			/* Maximal loss */
			if (loss < amount/2) loss = amount/2;

			/* Lose some points */
			cur = cur - loss;

			/* Hack -- Only reduce stat to 17 sometimes */
			if (cur < 18) cur = (amount <= 20) ? 18 : 17;
		}

		/* Prevent illegal values */
		if (cur < 3) cur = 3;

		/* Something happened */
		if (cur != p_ptr->stat_cur[stat]) res = TRUE;
	}

	/* Damage "max" value */
	if (permanent && (max > 3))
	{
		chg_virtue(V_SACRIFICE, 1);
		if (stat == A_WIS || stat == A_INT)
			chg_virtue(V_ENLIGHTEN, -2);

		/* Handle "low" values */
		if (max <= 18)
		{
			if (amount > 90) max--;
			if (amount > 50) max--;
			if (amount > 20) max--;
			max--;
		}

		/* Handle "high" values */
		else
		{
			/* Hack -- Decrement by a random amount between one-quarter */
			/* and one-half of the stat bonus times the percentage, with a */
			/* minimum damage of half the percentage. -CWS */
			loss = (((max-18) / 2 + 1) / 2 + 1);
			loss = ((randint1(loss) + loss) * amount) / 100;
			if (loss < amount/2) loss = amount/2;

			/* Lose some points */
			max = max - loss;

			/* Hack -- Only reduce stat to 17 sometimes */
			if (max < 18) max = (amount <= 20) ? 18 : 17;
		}

		/* Hack -- keep it clean */
		if (same || (max < cur)) max = cur;

		/* Something happened */
		if (max != p_ptr->stat_max[stat]) res = TRUE;
	}

	/* Apply changes */
	if (res)
	{
		/* Actually set the stat to its new value. */
		p_ptr->stat_cur[stat] = cur;
		p_ptr->stat_max[stat] = max;

		/* Redisplay the stats later */
		p_ptr->redraw |= (PR_STATS);

		/* Recalculate bonuses */
		p_ptr->update |= (PU_BONUS);
	}

	/* Done */
	return (res);
}


/*!
 * @brief �ץ쥤�䡼�δ���ǽ���ͤ���������� / Restore a stat.  Return TRUE only if this actually makes a difference.
 * @param stat �������ơ�����ID
 * @return �ºݤ˲����������TRUE���֤���
 */
bool res_stat(int stat)
{
	/* Restore if needed */
	if (p_ptr->stat_cur[stat] != p_ptr->stat_max[stat])
	{
		/* Restore */
		p_ptr->stat_cur[stat] = p_ptr->stat_max[stat];

		/* Recalculate bonuses */
		p_ptr->update |= (PU_BONUS);

		/* Redisplay the stats later */
		p_ptr->redraw |= (PR_STATS);

		/* Success */
		return (TRUE);
	}

	/* Nothing to restore */
	return (FALSE);
}


/*
 * Increase players hit points, notice effects
 */
bool hp_player(int num)
{
	int vir;
	vir = virtue_number(V_VITALITY);
	if (vir)
	{
		num = num * (p_ptr->virtues[vir - 1] + 1250) / 1250;
	}
	/* Healing needed */
	if (p_ptr->chp < p_ptr->mhp)
	{
		if ((num > 0) && (p_ptr->chp < (p_ptr->mhp/3)))
			chg_virtue(V_TEMPERANCE, 1);
		/* Gain hitpoints */
		p_ptr->chp += num;

		/* Enforce maximum */
		if (p_ptr->chp >= p_ptr->mhp)
		{
			p_ptr->chp = p_ptr->mhp;
			p_ptr->chp_frac = 0;
		}

		/* Redraw */
		p_ptr->redraw |= (PR_HP);

		/* Window stuff */
		p_ptr->window |= (PW_PLAYER);

		/* Heal 0-4 */
		if (num < 5)
		{
			msg_print(_("������ʬ���ɤ��ʤä���", "You feel a little better."));
		}

		/* Heal 5-14 */
		else if (num < 15)
		{
			msg_print(_("��ʬ���ɤ��ʤä���", "You feel better."));
		}

		/* Heal 15-34 */
		else if (num < 35)
		{
			msg_print(_("�ȤƤⵤʬ���ɤ��ʤä���", "You feel much better."));
		}

		/* Heal 35+ */
		else
		{
			msg_print(_("�Ҥ��礦�˵�ʬ���ɤ��ʤä���", "You feel very good."));
		}

		/* Notice */
		return (TRUE);
	}

	/* Ignore */
	return (FALSE);
}


/*
 * Array of stat "descriptions"
 */
static cptr desc_stat_pos[] =
{
_("����", "strong"),
_("��Ū��", "smart"),
_("����", "wise"),
_("���Ѥ�", "dextrous"),
_("�򹯤�", "healthy"),
_("������", "cute")
};


/*
 * Array of stat "descriptions"
 */
static cptr desc_stat_neg[] =
{
#ifdef JP
"�夯",
"̵�Τ�",
"�򤫤�",
"�Դ��Ѥ�",
"�Է򹯤�",
"����"
#else
	"weak",
	"stupid",
	"naive",
	"clumsy",
	"sickly",
	"ugly"
#endif

};


/*
 * Lose a "point"
 */
bool do_dec_stat(int stat)
{
	bool sust = FALSE;

	/* Access the "sustain" */
	switch (stat)
	{
		case A_STR: if (p_ptr->sustain_str) sust = TRUE; break;
		case A_INT: if (p_ptr->sustain_int) sust = TRUE; break;
		case A_WIS: if (p_ptr->sustain_wis) sust = TRUE; break;
		case A_DEX: if (p_ptr->sustain_dex) sust = TRUE; break;
		case A_CON: if (p_ptr->sustain_con) sust = TRUE; break;
		case A_CHR: if (p_ptr->sustain_chr) sust = TRUE; break;
	}

	/* Sustain */
	if (sust && (!ironman_nightmare || randint0(13)))
	{
		/* Message */
		msg_format(_("%s�ʤä������������������˸�����ä���", "You feel %s for a moment, but the feeling passes."),
					desc_stat_neg[stat]);

		/* Notice effect */
		return (TRUE);
	}

	/* Attempt to reduce the stat */
	if (dec_stat(stat, 10, (ironman_nightmare && !randint0(13))))
	{
		/* Message */
		msg_format(_("�Ҥɤ�%s�ʤä��������롣", "You feel very %s."), desc_stat_neg[stat]);

		/* Notice effect */
		return (TRUE);
	}

	/* Nothing obvious */
	return (FALSE);
}


/*
 * Restore lost "points" in a stat
 */
bool do_res_stat(int stat)
{
	/* Attempt to increase */
	if (res_stat(stat))
	{
		/* Message */
		msg_format(_("���̤��%s�ʤä��������롣", "You feel less %s."), desc_stat_pos[stat]);

		/* Notice */
		return (TRUE);
	}

	/* Nothing obvious */
	return (FALSE);
}


/*
 * Gain a "point" in a stat
 */
bool do_inc_stat(int stat)
{
	bool res;

	/* Restore strength */
	res = res_stat(stat);

	/* Attempt to increase */
	if (inc_stat(stat))
	{
		if (stat == A_WIS)
		{
			chg_virtue(V_ENLIGHTEN, 1);
			chg_virtue(V_FAITH, 1);
		}
		else if (stat == A_INT)
		{
			chg_virtue(V_KNOWLEDGE, 1);
			chg_virtue(V_ENLIGHTEN, 1);
		}
		else if (stat == A_CON)
			chg_virtue(V_VITALITY, 1);

		/* Message */
		msg_format(_("������ȤƤ�%s�ʤä���", "Wow!  You feel very %s!"), desc_stat_pos[stat]);

		/* Notice */
		return (TRUE);
	}

	/* Restoration worked */
	if (res)
	{
		/* Message */
		msg_format(_("���̤��%s�ʤä��������롣", "You feel less %s."), desc_stat_pos[stat]);

		/* Notice */
		return (TRUE);
	}

	/* Nothing obvious */
	return (FALSE);
}


/*
 * Restores any drained experience
 */
bool restore_level(void)
{
	/* Restore experience */
	if (p_ptr->exp < p_ptr->max_exp)
	{
		/* Message */
		msg_print(_("�и��ͤ���äƤ����������롣", "You feel your experience returning."));

		/* Restore the experience */
		p_ptr->exp = p_ptr->max_exp;

		/* Check the experience */
		check_experience();

		/* Did something */
		return (TRUE);
	}

	/* No effect */
	return (FALSE);
}


/*
 * Forget everything
 */
bool lose_all_info(void)
{
	int i;

	chg_virtue(V_KNOWLEDGE, -5);
	chg_virtue(V_ENLIGHTEN, -5);

	/* Forget info about objects */
	for (i = 0; i < INVEN_TOTAL; i++)
	{
		object_type *o_ptr = &inventory[i];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Allow "protection" by the MENTAL flag */
		if (o_ptr->ident & (IDENT_MENTAL)) continue;

		/* Remove "default inscriptions" */
		o_ptr->feeling = FEEL_NONE;

		/* Hack -- Clear the "empty" flag */
		o_ptr->ident &= ~(IDENT_EMPTY);

		/* Hack -- Clear the "known" flag */
		o_ptr->ident &= ~(IDENT_KNOWN);

		/* Hack -- Clear the "felt" flag */
		o_ptr->ident &= ~(IDENT_SENSE);
	}

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);

	/* Mega-Hack -- Forget the map */
	wiz_dark();

	/* It worked */
	return (TRUE);
}


void do_poly_wounds(void)
{
	/* Changed to always provide at least _some_ healing */
	s16b wounds = p_ptr->cut;
	s16b hit_p = (p_ptr->mhp - p_ptr->chp);
	s16b change = damroll(p_ptr->lev, 5);
	bool Nasty_effect = one_in_(5);

	if (!(wounds || hit_p || Nasty_effect)) return;

	msg_print(_("�������ڤ���Τ��Ѳ�������", "Your wounds are polymorphed into less serious ones."));
	hp_player(change);
	if (Nasty_effect)
	{
		msg_print(_("�����ʽ����Ǥ�����", "A new wound was created!"));
		take_hit(DAMAGE_LOSELIFE, change / 2, _("�Ѳ�������", "a polymorphed wound"), -1);
		set_cut(change);
	}
	else
	{
		set_cut(p_ptr->cut - (change / 2));
	}
}


/*
 * Change player race
 */
void change_race(CHARACTER_IDX new_race, cptr effect_msg)
{
	cptr title = race_info[new_race].title;
	int  old_race = p_ptr->prace;

#ifdef JP
	msg_format("���ʤ���%s%s���Ѳ�������", effect_msg, title);
#else
	msg_format("You turn into %s %s%s!", (!effect_msg[0] && is_a_vowel(title[0]) ? "an" : "a"), effect_msg, title);
#endif

	chg_virtue(V_CHANCE, 2);

	if (p_ptr->prace < 32)
	{
		p_ptr->old_race1 |= 1L << p_ptr->prace;
	}
	else
	{
		p_ptr->old_race2 |= 1L << (p_ptr->prace-32);
	}
	p_ptr->prace = new_race;
	rp_ptr = &race_info[p_ptr->prace];

	/* Experience factor */
	p_ptr->expfact = rp_ptr->r_exp + cp_ptr->c_exp;

	/*
	 * The speed bonus of Klackons and Sprites are disabled
	 * and the experience penalty is decreased.
	 */
	if (((p_ptr->pclass == CLASS_MONK) || (p_ptr->pclass == CLASS_FORCETRAINER) || (p_ptr->pclass == CLASS_NINJA)) && ((p_ptr->prace == RACE_KLACKON) || (p_ptr->prace == RACE_SPRITE)))
		p_ptr->expfact -= 15;

	/* Get character's height and weight */
	get_height_weight();

	/* Hitdice */
	if (p_ptr->pclass == CLASS_SORCERER)
		p_ptr->hitdie = rp_ptr->r_mhp/2 + cp_ptr->c_mhp + ap_ptr->a_mhp;
	else
		p_ptr->hitdie = rp_ptr->r_mhp + cp_ptr->c_mhp + ap_ptr->a_mhp;

	do_cmd_rerate(FALSE);

	/* The experience level may be modified */
	check_experience();

	p_ptr->redraw |= (PR_BASIC);

	p_ptr->update |= (PU_BONUS);

	handle_stuff();

	/* Load an autopick preference file */
	if (old_race != p_ptr->prace) autopick_load_pref(FALSE);

	/* Player's graphic tile may change */
	lite_spot(p_ptr->y, p_ptr->x);
}


void do_poly_self(void)
{
	int power = p_ptr->lev;

	msg_print(_("���ʤ����Ѳ���ˬ��򴶤���...", "You feel a change coming over you..."));
	chg_virtue(V_CHANCE, 1);

	if ((power > randint0(20)) && one_in_(3) && (p_ptr->prace != RACE_ANDROID))
	{
		char effect_msg[80] = "";
		CHARACTER_IDX new_race;

		/* Some form of racial polymorph... */
		power -= 10;

		if ((power > randint0(5)) && one_in_(4))
		{
			/* sex change */
			power -= 2;

			if (p_ptr->psex == SEX_MALE)
			{
				p_ptr->psex = SEX_FEMALE;
				sp_ptr = &sex_info[p_ptr->psex];
				sprintf(effect_msg, _("������", "female "));
			}
			else
			{
				p_ptr->psex = SEX_MALE;
				sp_ptr = &sex_info[p_ptr->psex];
				sprintf(effect_msg, _("������", "male "));
			}
		}

		if ((power > randint0(30)) && one_in_(5))
		{
			int tmp = 0;

			/* Harmful deformity */
			power -= 15;

			while (tmp < 6)
			{
				if (one_in_(2))
				{
					(void)dec_stat(tmp, randint1(6) + 6, one_in_(3));
					power -= 1;
				}
				tmp++;
			}

			/* Deformities are discriminated against! */
			(void)dec_stat(A_CHR, randint1(6), TRUE);

			if (effect_msg[0])
			{
				char tmp_msg[10];
				sprintf(tmp_msg,_("%s", "%s "),effect_msg);
				sprintf(effect_msg,_("�����%s", "deformed %s "),tmp_msg);
			}
			else
			{
				sprintf(effect_msg,_("�����", "deformed "));
			}
		}

		while ((power > randint0(20)) && one_in_(10))
		{
			/* Polymorph into a less mutated form */
			power -= 10;

			if (!lose_mutation(0))
			msg_print(_("��̯�ʤ��餤���̤ˤʤä��������롣", "You feel oddly normal."));
		}

		do
		{
			new_race = (CHARACTER_IDX)randint0(MAX_RACES);
		}
		while ((new_race == p_ptr->prace) || (new_race == RACE_ANDROID));

		change_race(new_race, effect_msg);
	}

	if ((power > randint0(30)) && one_in_(6))
	{
		int tmp = 0;

		/* Abomination! */
		power -= 20;
		msg_format(_("%s�ι������Ѳ�������", "Your internal organs are rearranged!"), p_ptr->prace == RACE_ANDROID ? "����" : "��¡");

		while (tmp < 6)
		{
			(void)dec_stat(tmp, randint1(6) + 6, one_in_(3));
			tmp++;
		}
		if (one_in_(6))
		{
			msg_print(_("���ߤλѤ������Ƥ����ΤϺ���ʤ褦����", "You find living difficult in your present form!"));
			take_hit(DAMAGE_LOSELIFE, damroll(randint1(10), p_ptr->lev), _("��̿Ū�������Ѱ�", "a lethal mutation"), -1);

			power -= 10;
		}
	}

	if ((power > randint0(20)) && one_in_(4))
	{
		power -= 10;

		get_max_stats();
		do_cmd_rerate(FALSE);
	}

	while ((power > randint0(15)) && one_in_(3))
	{
		power -= 7;
		(void)gain_random_mutation(0);
	}

	if (power > randint0(5))
	{
		power -= 5;
		do_poly_wounds();
	}

	/* Note: earlier deductions may have left power < 0 already. */
	while (power > 0)
	{
		mutate_player();
		power--;
	}
}


/*
 * Decreases players hit points and sets death flag if necessary
 *
 * XXX XXX XXX Invulnerability needs to be changed into a "shield"
 *
 * XXX XXX XXX Hack -- this function allows the user to save (or quit)
 * the game when he dies, since the "You die." message is shown before
 * setting the player to "dead".
 */

int take_hit(int damage_type, HIT_POINT damage, cptr hit_from, int monspell)
{
	int old_chp = p_ptr->chp;

	char death_message[1024];
	char tmp[1024];

	int warning = (p_ptr->mhp * hitpoint_warn / 10);

	/* Paranoia */
	if (p_ptr->is_dead) return 0;

	if (p_ptr->sutemi) damage *= 2;
	if (p_ptr->special_defense & KATA_IAI) damage += (damage + 4) / 5;

	if (easy_band) damage = (damage+1)/2;

	if (damage_type != DAMAGE_USELIFE)
	{
		/* Disturb */
		disturb(1, 1);
		if (auto_more)
		{
			now_damaged = TRUE;
		}
	}

	if (monspell >= 0) learn_spell(monspell);

	/* Mega-Hack -- Apply "invulnerability" */
	if ((damage_type != DAMAGE_USELIFE) && (damage_type != DAMAGE_LOSELIFE))
	{
		if (IS_INVULN() && (damage < 9000))
		{
			if (damage_type == DAMAGE_FORCE)
			{
				msg_print(_("�Хꥢ���ڤ������줿��", "The attack cuts your shield of invulnerability open!"));
			}
			else if (one_in_(PENETRATE_INVULNERABILITY))
			{
				msg_print(_("̵Ũ�ΥХꥢ���ˤäƹ��⤵�줿��", "The attack penetrates your shield of invulnerability!"));
			}
			else
			{
				return 0;
			}
		}

		if (CHECK_MULTISHADOW())
		{
			if (damage_type == DAMAGE_FORCE)
			{
				msg_print(_("���Ƥ��Ȥ��Τ��ڤ������줿��", "The attack hits Shadow together with you!"));
			}
			else if (damage_type == DAMAGE_ATTACK)
			{
				msg_print(_("����ϸ��Ƥ�̿�椷�����ʤ��ˤ��Ϥ��ʤ��ä���", "The attack hits Shadow, you are unharmed!"));
				return 0;
			}
		}

		if (p_ptr->wraith_form)
		{
			if (damage_type == DAMAGE_FORCE)
			{
				msg_print(_("Ⱦʪ�����Τ��ڤ������줿��", "The attack cuts through your ethereal body!"));
			}
			else
			{
				damage /= 2;
				if ((damage == 0) && one_in_(2)) damage = 1;
			}
		}

		if (p_ptr->special_defense & KATA_MUSOU)
		{
			damage /= 2;
			if ((damage == 0) && one_in_(2)) damage = 1;
		}
	} /* not if LOSELIFE USELIFE */

	/* Hurt the player */
	p_ptr->chp -= damage;
	if(damage_type == DAMAGE_GENO && p_ptr->chp < 0)
	{
		damage += p_ptr->chp;
		p_ptr->chp = 0;
	}

	/* Display the hitpoints */
	p_ptr->redraw |= (PR_HP);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);

	if (damage_type != DAMAGE_GENO && p_ptr->chp == 0)
	{
		chg_virtue(V_SACRIFICE, 1);
		chg_virtue(V_CHANCE, 2);
	}

	/* Dead player */
	if (p_ptr->chp < 0)
	{
		bool android = (p_ptr->prace == RACE_ANDROID ? TRUE : FALSE);

#ifdef JP       /* �������˶�����λ���ƻ�����Ǥ��ʤ����Ƥߤ� by Habu */
		if (!cheat_save)
			if(!save_player()) msg_print("�����ּ��ԡ�");
#endif

		/* Sound */
		sound(SOUND_DEATH);

		chg_virtue(V_SACRIFICE, 10);

		handle_stuff();

		/* Leaving */
		p_ptr->leaving = TRUE;

		/* Note death */
		p_ptr->is_dead = TRUE;

		if (p_ptr->inside_arena)
		{
			cptr m_name = r_name+r_info[arena_info[p_ptr->arena_number].r_idx].name;
			msg_format(_("���ʤ���%s�������Ԥ��ä���", "You are beaten by %s."), m_name);
			msg_print(NULL);
			if (record_arena) do_cmd_write_nikki(NIKKI_ARENA, -1 - p_ptr->arena_number, m_name);
		}
		else
		{
			int q_idx = quest_number(dun_level);
			bool seppuku = streq(hit_from, "Seppuku");
			bool winning_seppuku = p_ptr->total_winner && seppuku;

			play_music(TERM_XTRA_MUSIC_BASIC, MUSIC_BASIC_GAMEOVER);

#ifdef WORLD_SCORE
			/* Make screen dump */
			screen_dump = make_screen_dump();
#endif

			/* Note cause of death */
			if (seppuku)
			{
				strcpy(p_ptr->died_from, hit_from);
#ifdef JP
				if (!winning_seppuku) strcpy(p_ptr->died_from, "��ʢ");
#endif
			}
			else
			{
				char dummy[1024];
#ifdef JP
				sprintf(dummy, "%s%s%s", !p_ptr->paralyzed ? "" : p_ptr->free_act ? "Ħ�����֤�" : "������֤�", p_ptr->image ? "���Ф��Ĥ��" : "", hit_from);
#else
				sprintf(dummy, "%s%s", hit_from, !p_ptr->paralyzed ? "" : " while helpless");
#endif
				my_strcpy(p_ptr->died_from, dummy, sizeof p_ptr->died_from);
			}

			/* No longer a winner */
			p_ptr->total_winner = FALSE;

			if (winning_seppuku)
			{
				do_cmd_write_nikki(NIKKI_BUNSHOU, 0, _("�����θ���ʢ������", "did Seppuku after the winning."));
			}
			else
			{
				char buf[20];

				if (p_ptr->inside_arena)
					strcpy(buf,_("���꡼��", "in the Arena"));
				else if (!dun_level)
					strcpy(buf,_("�Ͼ�", "on the surface"));
				else if (q_idx && (is_fixed_quest_idx(q_idx) &&
				         !((q_idx == QUEST_OBERON) || (q_idx == QUEST_SERPENT))))
					strcpy(buf,_("��������", "in a quest"));
				else
					sprintf(buf,_("%d��", "level %d"), (int)dun_level);

				sprintf(tmp, _("%s��%s�˻����줿��", "killed by %s %s."), buf, p_ptr->died_from);
				do_cmd_write_nikki(NIKKI_BUNSHOU, 0, tmp);
			}

			do_cmd_write_nikki(NIKKI_GAMESTART, 1, _("-------- �����४���С� --------", "--------   Game  Over   --------"));
			do_cmd_write_nikki(NIKKI_BUNSHOU, 1, "\n\n\n\n");

			flush();

			if (get_check_strict(_("���̤���¸���ޤ�����", "Dump the screen? "), CHECK_NO_HISTORY))
			{
				do_cmd_save_screen();
			}

			flush();

			/* Initialize "last message" buffer */
			if (p_ptr->last_message) string_free(p_ptr->last_message);
			p_ptr->last_message = NULL;

			/* Hack -- Note death */
			if (!last_words)
			{
#ifdef JP
				msg_format("���ʤ���%s�ޤ�����", android ? "����" : "���");
#else
				msg_print(android ? "You are broken." : "You die.");
#endif

				msg_print(NULL);
			}
			else
			{
				if (winning_seppuku)
				{
					get_rnd_line(_("seppuku_j.txt", "seppuku.txt"), 0, death_message);
				}
				else
				{
					get_rnd_line(_("death_j.txt", "death.txt"), 0, death_message);
				}

				do
				{
#ifdef JP
					while (!get_string(winning_seppuku ? "�����ζ�: " : "������ζ���: ", death_message, 1024)) ;
#else
					while (!get_string("Last word: ", death_message, 1024)) ;
#endif
				}
				while (winning_seppuku && !get_check_strict(_("������Ǥ�����", "Are you sure? "), CHECK_NO_HISTORY));

				if (death_message[0] == '\0')
				{
#ifdef JP
					strcpy(death_message, format("���ʤ���%s�ޤ�����", android ? "����" : "���"));
#else
					strcpy(death_message, android ? "You are broken." : "You die.");
#endif
				}
				else p_ptr->last_message = string_make(death_message);

#ifdef JP
				if (winning_seppuku)
				{
					int i, len;
					int w = Term->wid;
					int h = Term->hgt;
					int msg_pos_x[9] = {  5,  7,  9, 12,  14,  17,  19,  21, 23};
					int msg_pos_y[9] = {  3,  4,  5,  4,   5,   4,   5,   6,  4};
					cptr str;
					char* str2;

					Term_clear();

					/* ������ */
					for (i = 0; i < 40; i++)
						Term_putstr(randint0(w / 2) * 2, randint0(h), 2, TERM_VIOLET, "��");

					str = death_message;
					if (strncmp(str, "��", 2) == 0) str += 2;

					str2 = my_strstr(str, "��");
					if (str2 != NULL) *str2 = '\0';

					i = 0;
					while (i < 9)
					{
						str2 = my_strstr(str, " ");
						if (str2 == NULL) len = strlen(str);
						else len = str2 - str;

						if (len != 0)
						{
							Term_putstr_v(w * 3 / 4 - 2 - msg_pos_x[i] * 2, msg_pos_y[i], len,
							TERM_WHITE, str);
							if (str2 == NULL) break;
							i++;
						}
						str = str2 + 1;
						if (*str == 0) break;
					}

					/* Hide cursor */
					Term_putstr(w-1, h-1, 1, TERM_WHITE, " ");

					flush();
#ifdef WORLD_SCORE
					/* Make screen dump */
					screen_dump = make_screen_dump();
#endif

					/* Wait a key press */
					(void)inkey();
				}
				else
#endif
					msg_print(death_message);
			}
		}

		/* Dead */
		return damage;
	}

	handle_stuff();

	/* Hitpoint warning */
	if (p_ptr->chp < warning)
	{
		/* Hack -- bell on first notice */
		if (old_chp > warning) bell();

		sound(SOUND_WARN);

		if (record_danger && (old_chp > warning))
		{
			if (p_ptr->image && damage_type == DAMAGE_ATTACK)
				hit_from = _("����", "something");

			sprintf(tmp,_("%s�ˤ�äƥԥ���˴٤ä���", "A critical situation because of %s."),hit_from);
			do_cmd_write_nikki(NIKKI_BUNSHOU, 0, tmp);
		}

		if (auto_more)
		{
			/* stop auto_more even if DAMAGE_USELIFE */
			now_damaged = TRUE;
		}

		/* Message */
		msg_print(_("*** �ٹ�:��ҥåȡ��ݥ���ȡ� ***", "*** LOW HITPOINT WARNING! ***"));
		msg_print(NULL);
		flush();
	}
	if (p_ptr->wild_mode && !p_ptr->leaving && (p_ptr->chp < MAX(warning, p_ptr->mhp/5)))
	{
		change_wild_mode();
	}
	return damage;
}


/*
 * Gain experience
 */
void gain_exp_64(s32b amount, u32b amount_frac)
{
	if (p_ptr->is_dead) return;

	if (p_ptr->prace == RACE_ANDROID) return;

	/* Gain some experience */
	s64b_add(&(p_ptr->exp), &(p_ptr->exp_frac), amount, amount_frac);

	/* Slowly recover from experience drainage */
	if (p_ptr->exp < p_ptr->max_exp)
	{
		/* Gain max experience (20%) (was 10%) */
		p_ptr->max_exp += amount / 5;
	}

	/* Check Experience */
	check_experience();
}


/*
 * Gain experience
 */
void gain_exp(s32b amount)
{
	gain_exp_64(amount, 0L);
}


void calc_android_exp(void)
{
	int i;
	u32b total_exp = 0;
	if (p_ptr->is_dead) return;

	if (p_ptr->prace != RACE_ANDROID) return;

	for (i = INVEN_RARM; i < INVEN_TOTAL; i++)
	{
		object_type *o_ptr = &inventory[i];
		object_type forge;
		object_type *q_ptr = &forge;
		u32b value, exp;
		int level = MAX(k_info[o_ptr->k_idx].level * 6 - 8, 1); /* #tang k_info[o_ptr->k_idx].level -> k_info[o_ptr->k_idx].level*6 */

		if ((i == INVEN_RIGHT) || (i == INVEN_LEFT) || (i == INVEN_NECK) || (i == INVEN_LITE)) continue;
		if (!o_ptr->k_idx) continue;

		/* Wipe the object */
		object_wipe(q_ptr);

		object_copy(q_ptr, o_ptr);
		q_ptr->discount = 0;
		q_ptr->curse_flags = 0L;

		if (object_is_fixed_artifact(o_ptr))
		{
			level = (level + MAX(a_info[o_ptr->name1].level - 8, 5)) / 2;
			level += MIN(20, a_info[o_ptr->name1].rarity/(a_info[o_ptr->name1].gen_flags & TRG_INSTA_ART ? 10 : 3));
		}
		else if (object_is_ego(o_ptr))
		{
			level += MAX(3, (e_info[o_ptr->name2].rating - 5)/2);
		}
		else if (o_ptr->art_name)
		{
			s32b total_flags = flag_cost(o_ptr, o_ptr->pval);
			int fake_level;

			if (!object_is_weapon_ammo(o_ptr))
			{
				/* For armors */
				if (total_flags < 15000) fake_level = 10;
				else if (total_flags < 35000) fake_level = 25;
				else fake_level = 40;
			}
			else
			{
				/* For weapons */
				if (total_flags < 20000) fake_level = 10;
				else if (total_flags < 45000) fake_level = 25;
				else fake_level = 40;
			}

			level = MAX(level, (level + MAX(fake_level - 8, 5)) / 2 + 3);
		}

		value = object_value_real(q_ptr);

		if (value <= 0) continue;
		if ((o_ptr->tval == TV_SOFT_ARMOR) && (o_ptr->sval == SV_ABUNAI_MIZUGI) && (p_ptr->pseikaku != SEIKAKU_SEXY)) value /= 32;
		if (value > 5000000L) value = 5000000L;
		if ((o_ptr->tval == TV_DRAG_ARMOR) || (o_ptr->tval == TV_CARD)) level /= 2;

		if (object_is_artifact(o_ptr) || object_is_ego(o_ptr) ||
		    (o_ptr->tval == TV_DRAG_ARMOR) ||
		    ((o_ptr->tval == TV_HELM) && (o_ptr->sval == SV_DRAGON_HELM)) ||
		    ((o_ptr->tval == TV_SHIELD) && (o_ptr->sval == SV_DRAGON_SHIELD)) ||
		    ((o_ptr->tval == TV_GLOVES) && (o_ptr->sval == SV_SET_OF_DRAGON_GLOVES)) ||
		    ((o_ptr->tval == TV_BOOTS) && (o_ptr->sval == SV_PAIR_OF_DRAGON_GREAVE)) ||
		    ((o_ptr->tval == TV_SWORD) && (o_ptr->sval == SV_DIAMOND_EDGE)))
		{
			if (level > 65) level = 35 + (level - 65) / 5;
			else if (level > 35) level = 25 + (level - 35) / 3;
			else if (level > 15) level = 15 + (level - 15) / 2;
			exp = MIN(100000L, value) / 2 * level * level;
			if (value > 100000L)
				exp += (value - 100000L) / 8 * level * level;
		}
		else
		{
			exp = MIN(100000L, value) * level;
			if (value > 100000L)
				exp += (value - 100000L) / 4  * level;
		}
		if ((((i == INVEN_RARM) || (i == INVEN_LARM)) && (buki_motteruka(i))) || (i == INVEN_BOW)) total_exp += exp / 48;
		else total_exp += exp / 16;
		if (i == INVEN_BODY) total_exp += exp / 32;
	}
	p_ptr->exp = p_ptr->max_exp = total_exp;

	/* Check Experience */
	check_experience();
}


/*
 * Lose experience
 */
void lose_exp(s32b amount)
{
	if (p_ptr->prace == RACE_ANDROID) return;

	/* Never drop below zero experience */
	if (amount > p_ptr->exp) amount = p_ptr->exp;

	/* Lose some experience */
	p_ptr->exp -= amount;

	/* Check Experience */
	check_experience();
}


/*
 * Drain experience
 * If resisted to draining, return FALSE
 */
bool drain_exp(s32b drain, s32b slip, int hold_exp_prob)
{
	/* Androids and their mimics are never drained */
	if (p_ptr->prace == RACE_ANDROID) return FALSE;

	if (p_ptr->hold_exp && (randint0(100) < hold_exp_prob))
	{
		/* Hold experience */
		msg_print(_("���������ʤηи��ͤ��꤭�ä���", "You keep hold of your experience!"));
		return FALSE;
	}

	/* Hold experience failed */
	if (p_ptr->hold_exp)
	{
		msg_print(_("�и��ͤ򾯤��ۤ����줿�������롪", "You feel your experience slipping away!"));
		lose_exp(slip);
	}
	else
	{
		msg_print(_("�и��ͤ��Τ���ۤ����줿�������롪", "You feel your experience draining away!"));
		lose_exp(drain);
	}

	return TRUE;
}


bool set_ultimate_res(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->ult_res && !do_dec)
		{
			if (p_ptr->ult_res > v) return FALSE;
		}
		else if (!p_ptr->ult_res)
		{
			msg_print(_("�����뤳�Ȥ��Ф����������Ĥ����������롪", "You feel resistant!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->ult_res)
		{
			msg_print(_("�����뤳�Ȥ��Ф������������줿�������롣", "You feel less resistant"));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->ult_res = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

bool set_tim_res_nether(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->tim_res_nether && !do_dec)
		{
			if (p_ptr->tim_res_nether > v) return FALSE;
		}
		else if (!p_ptr->tim_res_nether)
		{
			msg_print(_("�Ϲ����Ϥ��Ф����������Ĥ����������롪", "You feel nether resistant!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->tim_res_nether)
		{
			msg_print(_("�Ϲ����Ϥ��Ф������������줿�������롣", "You feel less nether resistant"));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->tim_res_nether = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}

bool set_tim_res_time(TIME_EFFECT v, bool do_dec)
{
	bool notice = FALSE;

	/* Hack -- Force good values */
	v = (v > 10000) ? 10000 : (v < 0) ? 0 : v;

	if (p_ptr->is_dead) return FALSE;

	/* Open */
	if (v)
	{
		if (p_ptr->tim_res_time && !do_dec)
		{
			if (p_ptr->tim_res_time > v) return FALSE;
		}
		else if (!p_ptr->tim_res_time)
		{
			msg_print(_("���ֵ�ž���Ϥ��Ф����������Ĥ����������롪", "You feel time resistant!"));
			notice = TRUE;
		}
	}

	/* Shut */
	else
	{
		if (p_ptr->tim_res_time)
		{
			msg_print(_("���ֵ�ž���Ϥ��Ф������������줿�������롣", "You feel less time resistant"));
			notice = TRUE;
		}
	}

	/* Use the value */
	p_ptr->tim_res_time = v;

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);

	/* Nothing to notice */
	if (!notice) return (FALSE);

	/* Disturb */
	if (disturb_state) disturb(0, 0);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Result */
	return (TRUE);
}


/*
 * Choose a warrior-mage elemental attack. -LM-
 */
bool choose_ele_attack(void)
{
	int num;

	char choice;

	if (!buki_motteruka(INVEN_RARM) && !buki_motteruka(INVEN_LARM))
	{
		msg_format(_("��������ʤ�����ˡ���ϻȤ��ʤ���", "You cannot use temporary branding with no weapon."));
		return FALSE;
	}

	/* Save screen */
	screen_save();

	num = (p_ptr->lev - 20) / 5;
	c_prt(TERM_RED,    _("        a) �ƴ�", "        a) Fire Brand"), 2, 14);

	if (num >= 2) 
		c_prt(TERM_L_WHITE,_("        b) ���", "        b) Cold Brand"), 3, 14);
	else 
		prt("", 3, 14);
	
	if (num >= 3) 
		c_prt(TERM_GREEN,  _("        c) �ǻ�", "        c) Poison Brand"), 4, 14);
	else 
		prt("", 4, 14);

	if (num >= 4) 
		c_prt(TERM_L_DARK, _("        d) �ϲ�", "        d) Acid Brand"), 5, 14);
	else 
		prt("", 5, 14);

	if (num >= 5) 
		c_prt(TERM_BLUE,   _("        e) �ŷ�", "        e) Elec Brand"), 6, 14);
	else 
		prt("", 6, 14);

	prt("", 7, 14);
	prt("", 8, 14);
	prt("", 9, 14);

	prt("", 1, 0);
	prt(_("        �ɤθ��ǹ���򤷤ޤ�����", "        Choose a temporary elemental brand "), 1, 14);

	choice = inkey();

	if ((choice == 'a') || (choice == 'A')) 
		set_ele_attack(ATTACK_FIRE, p_ptr->lev/2 + randint1(p_ptr->lev/2));
	else if (((choice == 'b') || (choice == 'B')) && (num >= 2))
		set_ele_attack(ATTACK_COLD, p_ptr->lev/2 + randint1(p_ptr->lev/2));
	else if (((choice == 'c') || (choice == 'C')) && (num >= 3))
		set_ele_attack(ATTACK_POIS, p_ptr->lev/2 + randint1(p_ptr->lev/2));
	else if (((choice == 'd') || (choice == 'D')) && (num >= 4))
		set_ele_attack(ATTACK_ACID, p_ptr->lev/2 + randint1(p_ptr->lev/2));
	else if (((choice == 'e') || (choice == 'E')) && (num >= 5))
		set_ele_attack(ATTACK_ELEC, p_ptr->lev/2 + randint1(p_ptr->lev/2));
	else
	{
		msg_print(_("��ˡ����Ȥ��Τ��᤿��", "You cancel the temporary branding."));
		screen_load();
		return FALSE;
	}
	/* Load screen */
	screen_load();
	return TRUE;
}


/*
 * Choose a elemental immune. -LM-
 */
bool choose_ele_immune(TIME_EFFECT immune_turn)
{
	char choice;

	/* Save screen */
	screen_save();

	c_prt(TERM_RED,    _("        a) �б�", "        a) Immune Fire"), 2, 14);
	c_prt(TERM_L_WHITE,_("        b) �䵤", "        b) Immune Cold"), 3, 14);
	c_prt(TERM_L_DARK, _("        c) ��", "        c) Immune Acid"), 4, 14);
	c_prt(TERM_BLUE,   _("        d) �ŷ�", "        d) Immune Elec"), 5, 14);

	prt("", 6, 14);
	prt("", 7, 14);
	prt("", 8, 14);
	prt("", 9, 14);

	prt("", 1, 0);
	prt(_("        �ɤθ��Ǥ��ȱ֤�Ĥ��ޤ�����", "        Choose a temporary elemental immune "), 1, 14);

	choice = inkey();

	if ((choice == 'a') || (choice == 'A')) 
		set_ele_immune(DEFENSE_FIRE, immune_turn);
	else if ((choice == 'b') || (choice == 'B'))
		set_ele_immune(DEFENSE_COLD, immune_turn);
	else if ((choice == 'c') || (choice == 'C'))
		set_ele_immune(DEFENSE_ACID, immune_turn);
	else if ((choice == 'd') || (choice == 'D'))
		set_ele_immune(DEFENSE_ELEC, immune_turn);
	else
	{
		msg_print(_("�ȱ֤��դ���Τ��᤿��", "You cancel the temporary immune."));
		screen_load();
		return FALSE;
	}
	/* Load screen */
	screen_load();
	return TRUE;
}
