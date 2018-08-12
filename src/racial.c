/*!
 * @file racial.c
 * @brief �쥤�����������Ѱۤε�ǽ���� / Racial powers (and mutations)
 * @date 2014/01/08
 * @author
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke\n
 * This software may be copied and distributed for educational, research,\n
 * and not for profit purposes provided that this copyright and statement\n
 * are included in all such copies.  Other copyrights may also apply.\n
 * 2014 Deskull rearranged comment for Doxygen. \n
 */

#include "angband.h"

/*!
 * @brief �оݤΥ����ƥब��䥯���ܥ�����κ����ˤʤ뤫���֤���/
 * Hook to determine if an object is contertible in an arrow/bolt
 * @param o_ptr ���֥������Ȥι�¤�Τλ��ȥݥ��󥿡�
 * @return �����ˤǤ���ʤ�TRUE���֤�
 */
static bool item_tester_hook_convertible(object_type *o_ptr)
{
	if((o_ptr->tval==TV_JUNK) || (o_ptr->tval==TV_SKELETON)) return TRUE;

	if ((o_ptr->tval == TV_CORPSE) && (o_ptr->sval == SV_SKELETON)) return TRUE;
	/* Assume not */
	return (FALSE);
}

/*!
 * @brief �쥤��������/�����¤�׽��� / do_cmd_cast calls this function if the player's class is 'archer'.
 * Hook to determine if an object is contertible in an arrow/bolt
 * @return ��¤��ºݤ˹Ԥä���TRUE������󥻥뤷����FALSE���֤�
 */
static bool do_cmd_archer(void)
{
	int ext=0;
	char ch;

	object_type	forge;
	object_type     *q_ptr;

	char com[80];
	char o_name[MAX_NLEN];

	q_ptr = &forge;

	if(p_ptr->lev >= 20)
		sprintf(com, _("[S]��, [A]��, [B]�����ܥ����� :", "Create [S]hots, Create [A]rrow or Create [B]olt ?"));
	else if(p_ptr->lev >= 10)
		sprintf(com, _("[S]��, [A]��:", "Create [S]hots or Create [A]rrow ?"));
	else
		sprintf(com, _("[S]��:", "Create [S]hots ?"));

	if (p_ptr->confused)
	{
		msg_print(_("���𤷤Ƥ롪", "You are too confused!"));
		return FALSE;
	}

	if (p_ptr->blind)
	{
		msg_print(_("�ܤ������ʤ���", "You are blind!"));
		return FALSE;
	}

	while (TRUE)
	{
		if (!get_com(com, &ch, TRUE))
		{
			return FALSE;
		}
		if (ch == 'S' || ch == 's')
		{
			ext = 1;
			break;
		}
		if ((ch == 'A' || ch == 'a')&&(p_ptr->lev >= 10))
		{
			ext = 2;
			break;
		}
		if ((ch == 'B' || ch == 'b')&&(p_ptr->lev >= 20))
		{
			ext = 3;
			break;
		}
	}

	/**********Create shots*********/
	if (ext == 1)
	{
		int x,y, dir;
		cave_type *c_ptr;

		if (!get_rep_dir(&dir, FALSE)) return FALSE;
		y = p_ptr->y + ddy[dir];
		x = p_ptr->x + ddx[dir];
		c_ptr = &cave[y][x];

		if (!have_flag(f_info[get_feat_mimic(c_ptr)].flags, FF_CAN_DIG))
		{
			msg_print(_("�����ˤϴ��Ф��ʤ���", "You need pile of rubble."));
			return FALSE;
		}
		else if (!cave_have_flag_grid(c_ptr, FF_CAN_DIG) || !cave_have_flag_grid(c_ptr, FF_HURT_ROCK))
		{
			msg_print(_("�Ť����������ʤ��ä���", "You failed to make ammo."));
		}
		else
		{
			s16b slot;

			/* Get local object */
			q_ptr = &forge;

			/* Hack -- Give the player some small firestones */
			object_prep(q_ptr, lookup_kind(TV_SHOT, (OBJECT_SUBTYPE_VALUE)m_bonus(1, p_ptr->lev) + 1));
			q_ptr->number = (byte)rand_range(15,30);
			object_aware(q_ptr);
			object_known(q_ptr);
			apply_magic(q_ptr, p_ptr->lev, AM_NO_FIXED_ART);
			q_ptr->discount = 99;

			slot = inven_carry(q_ptr);

			object_desc(o_name, q_ptr, 0);
			msg_format(_("%s���ä���", "You make some ammo."), o_name);

			/* Auto-inscription */
			if (slot >= 0) autopick_alter_item(slot, FALSE);

			/* Destroy the wall */
			cave_alter_feat(y, x, FF_HURT_ROCK);

			p_ptr->update |= (PU_FLOW);
		}
	}
	/**********Create arrows*********/
	else if (ext == 2)
	{
		OBJECT_IDX item;
		cptr q, s;
		s16b slot;

		item_tester_hook = item_tester_hook_convertible;

		/* Get an item */
		q = _("�ɤΥ����ƥफ����ޤ����� ", "Convert which item? ");
		s = _("��������äƤ��ʤ���", "You have no item to convert.");
		if (!get_item(&item, q, s, (USE_INVEN | USE_FLOOR))) return FALSE;

		/* Get the item (in the pack) */
		if (item >= 0)
		{
			q_ptr = &inventory[item];
		}

		/* Get the item (on the floor) */
		else
		{
			q_ptr = &o_list[0 - item];
		}

		/* Get local object */
		q_ptr = &forge;

		/* Hack -- Give the player some small firestones */
		object_prep(q_ptr, lookup_kind(TV_ARROW, (OBJECT_SUBTYPE_VALUE)m_bonus(1, p_ptr->lev)+ 1));
		q_ptr->number = (byte)rand_range(5, 10);
		object_aware(q_ptr);
		object_known(q_ptr);
		apply_magic(q_ptr, p_ptr->lev, AM_NO_FIXED_ART);

		q_ptr->discount = 99;

		object_desc(o_name, q_ptr, 0);
		msg_format(_("%s���ä���", "You make some ammo."), o_name);

		if (item >= 0)
		{
			inven_item_increase(item, -1);
			inven_item_describe(item);
			inven_item_optimize(item);
		}
		else
		{
			floor_item_increase(0 - item, -1);
			floor_item_describe(0 - item);
			floor_item_optimize(0 - item);
		}

		slot = inven_carry(q_ptr);

		/* Auto-inscription */
		if (slot >= 0) autopick_alter_item(slot, FALSE);
	}
	/**********Create bolts*********/
	else if (ext == 3)
	{
		OBJECT_IDX item;
		cptr q, s;
		s16b slot;

		item_tester_hook = item_tester_hook_convertible;

		/* Get an item */
		q = _("�ɤΥ����ƥफ����ޤ����� ", "Convert which item? ");
		s = _("��������äƤ��ʤ���", "You have no item to convert.");
		if (!get_item(&item, q, s, (USE_INVEN | USE_FLOOR))) return FALSE;

		/* Get the item (in the pack) */
		if (item >= 0)
		{
			q_ptr = &inventory[item];
		}

		/* Get the item (on the floor) */
		else
		{
			q_ptr = &o_list[0 - item];
		}

		/* Get local object */
		q_ptr = &forge;

		/* Hack -- Give the player some small firestones */
		object_prep(q_ptr, lookup_kind(TV_BOLT, (OBJECT_SUBTYPE_VALUE)m_bonus(1, p_ptr->lev)+1));
		q_ptr->number = (byte)rand_range(4, 8);
		object_aware(q_ptr);
		object_known(q_ptr);
		apply_magic(q_ptr, p_ptr->lev, AM_NO_FIXED_ART);

		q_ptr->discount = 99;

		object_desc(o_name, q_ptr, 0);
		msg_format(_("%s���ä���", "You make some ammo."), o_name);

		if (item >= 0)
		{
			inven_item_increase(item, -1);
			inven_item_describe(item);
			inven_item_optimize(item);
		}
		else
		{
			floor_item_increase(0 - item, -1);
			floor_item_describe(0 - item);
			floor_item_optimize(0 - item);
		}

		slot = inven_carry(q_ptr);

		/* Auto-inscription */
		if (slot >= 0) autopick_alter_item(slot, FALSE);
	}
	return TRUE;
}

/*!
 * @brief ��ƻ��ѻդ����ϼ����߽���
 * @return �����ߤ�¹Ԥ�����TRUE������󥻥뤷����FALSE���֤�
 */
bool gain_magic(void)
{
	OBJECT_IDX item;
	PARAMETER_VALUE pval;
	int ext = 0;
	cptr q, s;
	object_type *o_ptr;
	char o_name[MAX_NLEN];

	/* Only accept legal items */
	item_tester_hook = item_tester_hook_recharge;

	/* Get an item */
	q = _("�ɤΥ����ƥ�����Ϥ�����ߤޤ���? ", "Gain power of which item? ");
	s = _("���Ϥ������륢���ƥब�ʤ���", "You have nothing to gain power.");

	if (!get_item(&item, q, s, (USE_INVEN | USE_FLOOR))) return (FALSE);

	/* Get the item (in the pack) */
	if (item >= 0)
	{
		o_ptr = &inventory[item];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - item];
	}

	if (o_ptr->tval == TV_STAFF && o_ptr->sval == SV_STAFF_NOTHING)
	{
		msg_print(_("���ξ�ˤ�ȯư�ΰ٤�ǽ�Ϥϲ�������äƤ��ʤ��褦����", "This staff doesn't have any magical ability."));
		return FALSE;
	}


	if (!object_is_known(o_ptr))
	{
		msg_print(_("���ꤵ��Ƥ��ʤ��ȼ�����ʤ���", "You need to identify before absorbing."));
		return FALSE;
	}

	if (o_ptr->timeout)
	{
		msg_print(_("��Ŷ��Υ����ƥ�ϼ�����ʤ���", "This item is still charging."));
		return FALSE;
	}

	pval = o_ptr->pval;
	if (o_ptr->tval == TV_ROD)
		ext = 72;
	else if (o_ptr->tval == TV_WAND)
		ext = 36;

	if (o_ptr->tval == TV_ROD)
	{
		p_ptr->magic_num2[o_ptr->sval + ext] += (MAGIC_NUM2)o_ptr->number;
		if (p_ptr->magic_num2[o_ptr->sval + ext] > 99) p_ptr->magic_num2[o_ptr->sval + ext] = 99;
	}
	else
	{
		int num;
		for (num = o_ptr->number; num; num--)
		{
			int gain_num = pval;
			if (o_ptr->tval == TV_WAND) gain_num = (pval + num - 1) / num;
			if (p_ptr->magic_num2[o_ptr->sval + ext])
			{
				gain_num *= 256;
				gain_num = (gain_num/3 + randint0(gain_num/3)) / 256;
				if (gain_num < 1) gain_num = 1;
			}
			p_ptr->magic_num2[o_ptr->sval + ext] += (MAGIC_NUM2)gain_num;
			if (p_ptr->magic_num2[o_ptr->sval + ext] > 99) p_ptr->magic_num2[o_ptr->sval + ext] = 99;
			p_ptr->magic_num1[o_ptr->sval + ext] += pval * 0x10000;
			if (p_ptr->magic_num1[o_ptr->sval + ext] > 99 * 0x10000) p_ptr->magic_num1[o_ptr->sval + ext] = 99 * 0x10000;
			if (p_ptr->magic_num1[o_ptr->sval + ext] > p_ptr->magic_num2[o_ptr->sval + ext] * 0x10000) p_ptr->magic_num1[o_ptr->sval + ext] = p_ptr->magic_num2[o_ptr->sval + ext] * 0x10000;
			if (o_ptr->tval == TV_WAND) pval -= (pval + num - 1) / num;
		}
	}

	object_desc(o_name, o_ptr, 0);
	/* Message */
	msg_format(_("%s�����Ϥ���������", "You absorb magic of %s."), o_name);

	/* Eliminate the item (from the pack) */
	if (item >= 0)
	{
		inven_item_increase(item, -999);
		inven_item_describe(item);
		inven_item_optimize(item);
	}

	/* Eliminate the item (from the floor) */
	else
	{
		floor_item_increase(0 - item, -999);
		floor_item_describe(0 - item);
		floor_item_optimize(0 - item);
	}
	p_ptr->energy_use = 100;
	return TRUE;
}

/*!
 * @brief ��ˡ�ϥ��ޥ�ɤ�¹ԤǤ��뤫��Ƚ����֤�
 * @return ��ˡ�ϥ��ޥ�ɤ���Ѳ�ǽ�ʤ�TRUE���Բ�ǽ�ʤ����ͳ���å�����ɽ������FALSE���֤���
 */
static bool can_do_cmd_cast(void)
{
	if (dun_level && (d_info[dungeon_type].flags1 & DF1_NO_MAGIC))
	{
		msg_print(_("���󥸥����ˡ��ۼ�������", "The dungeon absorbs all attempted magic!"));
		msg_print(NULL);
		return FALSE;
	}
	else if (p_ptr->anti_magic)
	{
		msg_print(_("ȿ��ˡ�Хꥢ����ˡ����⤷����", "An anti-magic shell disrupts your magic!"));
		return FALSE;
	}
	else if (p_ptr->shero)
	{
		msg_format(_("����β����Ƥ���Ƭ�����ʤ���", "You cannot think directly!"));
		return FALSE;
	}
	else
		return TRUE;
}

/*!
 * @brief �����Τι����������
 * @return �������Ѳ���������TRUE��������ǽ������󥻥뤷����FALSE���֤���
 */
static bool choose_kamae(void)
{
	char choice;
	int new_kamae = 0;
	int i;
	char buf[80];

	if (p_ptr->confused)
	{
		msg_print(_("���𤷤Ƥ��ƹ������ʤ���", "Too confused."));
		return FALSE;
	}

	/* Save screen */
	screen_save();
	prt(_(" a) ������Ȥ�", " a) No form"), 2, 20);

	for (i = 0; i < MAX_KAMAE; i++)
	{
		if (p_ptr->lev >= kamae_shurui[i].min_level)
		{
			sprintf(buf," %c) %-12s  %s",I2A(i+1), kamae_shurui[i].desc, kamae_shurui[i].info);
			prt(buf, 3+i, 20);
		}
	}

	prt("", 1, 0);
	prt(_("        �ɤι�����Ȥ�ޤ�����", "        Choose Form: "), 1, 14);

	while(1)
	{
		choice = inkey();

		if (choice == ESCAPE)
		{
			screen_load();
			return FALSE;
		}
		else if ((choice == 'a') || (choice == 'A'))
		{
			if (p_ptr->action == ACTION_KAMAE)
			{
				set_action(ACTION_NONE);
			}
			else
				msg_print(_("��Ȥ�ȹ����Ƥ��ʤ���", "You are not assuming a posture."));
			screen_load();
			return TRUE;
		}
		else if ((choice == 'b') || (choice == 'B'))
		{
			new_kamae = 0;
			break;
		}
		else if (((choice == 'c') || (choice == 'C')) && (p_ptr->lev > 29))
		{
			new_kamae = 1;
			break;
		}
		else if (((choice == 'd') || (choice == 'D')) && (p_ptr->lev > 34))
		{
			new_kamae = 2;
			break;
		}
		else if (((choice == 'e') || (choice == 'E')) && (p_ptr->lev > 39))
		{
			new_kamae = 3;
			break;
		}
	}
	set_action(ACTION_KAMAE);

	if (p_ptr->special_defense & (KAMAE_GENBU << new_kamae))
	{
		msg_print(_("����ľ������", "You reassume a posture."));
	}
	else
	{
		p_ptr->special_defense &= ~(KAMAE_MASK);
		p_ptr->update |= (PU_BONUS);
		p_ptr->redraw |= (PR_STATE);
		msg_format(_("%s�ι�����Ȥä���", "You assume a posture of %s form."),kamae_shurui[new_kamae].desc);
		p_ptr->special_defense |= (KAMAE_GENBU << new_kamae);
	}
	p_ptr->redraw |= PR_STATE;
	screen_load();
	return TRUE;
}

/*!
 * @brief ���ѲȤη��������
 * @return �����Ѳ���������TRUE�����ι�����ǽ������󥻥뤷����FALSE���֤���
 */
static bool choose_kata(void)
{
	char choice;
	int new_kata = 0;
	int i;
	char buf[80];

	if (p_ptr->confused)
	{
		msg_print(_("���𤷤Ƥ��ƹ������ʤ���", "Too confused."));
		return FALSE;
	}

	if (p_ptr->stun)
	{
		msg_print(_("�ռ����Ϥä���Ȥ��ʤ���", "You are not clear headed"));
		return FALSE;
	}

	if (p_ptr->afraid)
	{
		msg_print(_("�Τ��̤��ƹ������ʤ���", "You are trembling with fear!"));
		return FALSE;
	}

	/* Save screen */
	screen_save();
	prt(_(" a) ��������", " a) No Form"), 2, 20);

	for (i = 0; i < MAX_KATA; i++)
	{
		if (p_ptr->lev >= kata_shurui[i].min_level)
		{
			sprintf(buf,_(" %c) %s�η�    %s", " %c) Form of %-12s  %s"),I2A(i+1), kata_shurui[i].desc, kata_shurui[i].info);
			prt(buf, 3+i, 20);
		}
	}

	prt("", 1, 0);
	prt(_("        �ɤη��ǹ����ޤ�����", "        Choose Form: "), 1, 14);

	while(1)
	{
		choice = inkey();

		if (choice == ESCAPE)
		{
			screen_load();
			return FALSE;
		}
		else if ((choice == 'a') || (choice == 'A'))
		{
			if (p_ptr->action == ACTION_KATA)
			{
				set_action(ACTION_NONE);
			}
			else
				msg_print(_("��Ȥ�ȹ����Ƥ��ʤ���", "You are not assuming posture."));
			screen_load();
			return TRUE;
		}
		else if ((choice == 'b') || (choice == 'B'))
		{
			new_kata = 0;
			break;
		}
		else if (((choice == 'c') || (choice == 'C')) && (p_ptr->lev > 29))
		{
			new_kata = 1;
			break;
		}
		else if (((choice == 'd') || (choice == 'D')) && (p_ptr->lev > 34))
		{
			new_kata = 2;
			break;
		}
		else if (((choice == 'e') || (choice == 'E')) && (p_ptr->lev > 39))
		{
			new_kata = 3;
			break;
		}
	}
	set_action(ACTION_KATA);

	if (p_ptr->special_defense & (KATA_IAI << new_kata))
	{
		msg_print(_("����ľ������", "You reassume a posture."));
	}
	else
	{
		p_ptr->special_defense &= ~(KATA_MASK);
		p_ptr->update |= (PU_BONUS);
		p_ptr->update |= (PU_MONSTERS);
		msg_format(_("%s�η��ǹ�������", "You assume a posture of %s form."),kata_shurui[new_kata].desc);
		p_ptr->special_defense |= (KATA_IAI << new_kata);
	}
	p_ptr->redraw |= (PR_STATE);
	p_ptr->redraw |= (PR_STATUS);
	screen_load();
	return TRUE;
}


/*!
 * @brief �쥤����롦�ѥ�����typedef
 */
typedef struct power_desc_type power_desc_type;

/*!
 * @brief �쥤����롦�ѥ����ι�¤�����
 */
struct power_desc_type
{
	char name[80];
	PLAYER_LEVEL level;
	int  cost;
	int  stat;
	int  fail;
	int  number;
};


/*!
 * @brief �쥤����롦�ѥ��ȯư����Ψ��׻����� / Returns the chance to activate a racial power/mutation
 * @param pd_ptr ȯư�������쥤����롦�ѥ����ι�¤�λ��ȥݥ���
 * @return ����Ψ(%)���֤�
 */
static int racial_chance(power_desc_type *pd_ptr)
{
	PLAYER_LEVEL min_level  = pd_ptr->level;
	int  difficulty = pd_ptr->fail;

	int i;
	int val;
	int sum = 0;
	int stat = p_ptr->stat_cur[pd_ptr->stat];

	/* No chance for success */
	if ((p_ptr->lev < min_level) || p_ptr->confused)
	{
		return (0);
	}

	if (difficulty == 0) return 100;

	/* Calculate difficulty */
	if (p_ptr->stun)
	{
		difficulty += p_ptr->stun;
	}
	else if (p_ptr->lev > min_level)
	{
		int lev_adj = ((p_ptr->lev - min_level) / 3);
		if (lev_adj > 10) lev_adj = 10;
		difficulty -= lev_adj;
	}

	if (difficulty < 5) difficulty = 5;

	/* We only need halfs of the difficulty */
	difficulty = difficulty / 2;

	for (i = 1; i <= stat; i++)
	{
		val = i - difficulty;
		if (val > 0)
			sum += (val <= difficulty) ? val : difficulty;
	}

	if (difficulty == 0)
		return (100);
	else
		return (((sum * 100) / difficulty) / stat);
}


static int  racial_cost;

/*!
 * @brief �쥤����롦�ѥ��ȯư��Ƚ�����
 * @param pd_ptr ȯư�������쥤����롦�ѥ����ι�¤�λ��ȥݥ���
 * @return
 * ȯư�����ʤ��1��ȯư���Ԥʤ��-1������󥻥�ʤ��0���֤���
 * return value indicates that we have succesfully used the power 1: Succeeded, 0: Cancelled, -1: Failed
 */
static int racial_aux(power_desc_type *pd_ptr)
{
	s16b min_level  = pd_ptr->level;
	int  use_stat   = pd_ptr->stat;
	int  difficulty = pd_ptr->fail;
	int  use_hp = 0;

	racial_cost = pd_ptr->cost;

	/* Not enough mana - use hp */
	if (p_ptr->csp < racial_cost) use_hp = racial_cost - p_ptr->csp;

	/* Power is not available yet */
	if (p_ptr->lev < min_level)
	{
		msg_format(_("����ǽ�Ϥ���Ѥ���ˤϥ�٥� %d ��ã���Ƥ��ʤ���Фʤ�ޤ���", 
					 "You need to attain level %d to use this power."), min_level);

		p_ptr->energy_use = 0;
		return 0;
	}

	/* Too confused */
	else if (p_ptr->confused)
	{
		msg_print(_("���𤷤Ƥ��Ƥ���ǽ�ϤϻȤ��ʤ���", "You are too confused to use this power."));
		p_ptr->energy_use = 0;
		return 0;
	}

	/* Risk death? */
	else if (p_ptr->chp < use_hp)
	{
		if (!get_check(_("�����˺��ο�夷�����֤Ǥ���ǽ�Ϥ�Ȥ��ޤ�����", "Really use the power in your weakened state? ")))
		{
			p_ptr->energy_use = 0;
			return 0;
		}
	}

	/* Else attempt to do it! */

	if (difficulty)
	{
		if (p_ptr->stun)
		{
			difficulty += p_ptr->stun;
		}
		else if (p_ptr->lev > min_level)
		{
			int lev_adj = ((p_ptr->lev - min_level) / 3);
			if (lev_adj > 10) lev_adj = 10;
			difficulty -= lev_adj;
		}

		if (difficulty < 5) difficulty = 5;
	}

	/* take time and pay the price */
	p_ptr->energy_use = 100;

	/* Success? */
	if (randint1(p_ptr->stat_cur[use_stat]) >=
	    ((difficulty / 2) + randint1(difficulty / 2)))
	{
		return 1;
	}

	if (flush_failure) flush();
	msg_print(_("��ʬ�˽���Ǥ��ʤ��ä���", "You've failed to concentrate hard enough."));

	return -1;
}

/*!
 * @brief �쥤����롦�ѥȯư���˸���Ȥ���³Ū�ʱӾ����������Ǥ���
 * @return �ʤ�
 */
void ratial_stop_mouth(void)
{
	if (music_singing_any()) stop_singing();
	if (hex_spelling_any()) stop_hex_spell_all();
}

/*!
 * @brief �쥤����롦�ѥȯư����
 * @param command ȯư����쥤������ID
 * @return ������ºݤ˼¹Ԥ�������TRUE������󥻥뤷�����FALSE���֤���
 */
static bool cmd_racial_power_aux(s32b command)
{
	s16b        plev = p_ptr->lev;
	int         dir = 0;

	if (command <= -3)
	{
		switch (p_ptr->pclass)
		{
		case CLASS_WARRIOR:
		{
			int y = 0, x = 0, i;
			cave_type       *c_ptr;

			for (i = 0; i < 6; i++)
			{
				dir = randint0(8);
				y = p_ptr->y + ddy_ddd[dir];
				x = p_ptr->x + ddx_ddd[dir];
				c_ptr = &cave[y][x];

				/* Hack -- attack monsters */
				if (c_ptr->m_idx)
					py_attack(y, x, 0);
				else
				{
					msg_print(_("���⤬���򤭤ä���", "You attack the empty air."));
				}
			}
			break;
		}
		case CLASS_HIGH_MAGE:
		if (p_ptr->realm1 == REALM_HEX)
		{
			bool retval = stop_hex_spell();
			if (retval) p_ptr->energy_use = 10;
			return (retval);
		}
		case CLASS_MAGE:
		/* case CLASS_HIGH_MAGE: */
		case CLASS_SORCERER:
		{
			if (!eat_magic(p_ptr->lev * 2)) return FALSE;
			break;
		}
		case CLASS_PRIEST:
		{
			if (is_good_realm(p_ptr->realm1))
			{
				if (!bless_weapon()) return FALSE;
			}
			else
			{
				(void)dispel_monsters(plev * 4);
				turn_monsters(plev * 4);
				banish_monsters(plev * 4);
			}
			break;
		}
		case CLASS_ROGUE:
		{
			int x, y;

			if (!get_rep_dir(&dir, FALSE)) return FALSE;
			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];
			if (cave[y][x].m_idx)
			{
				py_attack(y, x, 0);
				if (randint0(p_ptr->skill_dis) < 7)
					msg_print(_("���ޤ�ƨ�����ʤ��ä���", "You are failed to run away."));
				else teleport_player(30, 0L);
			}
			else
			{
				msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "You don't see any monster in this direction"));
				msg_print(NULL);
			}
			break;
		}
		case CLASS_RANGER:
		case CLASS_SNIPER:
		{
			msg_print(_("Ũ��Ĵ������...", "You examine your foes..."));
			probing();
			break;
		}
		case CLASS_PALADIN:
		{
			if (!get_aim_dir(&dir)) return FALSE;
			fire_beam(is_good_realm(p_ptr->realm1) ? GF_HOLY_FIRE : GF_HELL_FIRE,
			          dir, plev * 3);
			break;
		}
		case CLASS_WARRIOR_MAGE:
		{
			if (command == -3)
			{
				int gain_sp = take_hit(DAMAGE_USELIFE, p_ptr->lev, 
								_("�ȣФ���ͣФؤ�̵�Ť��Ѵ�", "thoughtless convertion from HP to SP"), -1) / 5;
				if (gain_sp)
				{
					p_ptr->csp += gain_sp;
					if (p_ptr->csp > p_ptr->msp)
					{
						p_ptr->csp = p_ptr->msp;
						p_ptr->csp_frac = 0;
					}
				}
				else
				{
					msg_print(_("�Ѵ��˼��Ԥ�����", "You failed to convert."));
				}
			}
			else if (command == -4)
			{
				if (p_ptr->csp >= p_ptr->lev / 5)
				{
					p_ptr->csp -= p_ptr->lev / 5;
					hp_player(p_ptr->lev);
				}
				else
				{
					msg_print(_("�Ѵ��˼��Ԥ�����", "You failed to convert."));
				}
			}

			/* Redraw mana and hp */
			p_ptr->redraw |= (PR_HP | PR_MANA);

			break;
		}
		case CLASS_CHAOS_WARRIOR:
		{
			msg_print(_("�դ���ˤ��...", "You glare nearby monsters..."));
			slow_monsters(p_ptr->lev);
			stun_monsters(p_ptr->lev * 4);
			confuse_monsters(p_ptr->lev * 4);
			turn_monsters(p_ptr->lev * 4);
			stasis_monsters(p_ptr->lev * 4);
			break;
		}
		case CLASS_MONK:
		{
			if (!(empty_hands(TRUE) & EMPTY_HAND_RARM))
			{
				msg_print(_("�Ǽꤸ��ʤ��ȤǤ��ޤ���", "You need to be bare hand."));
				return FALSE;
			}
			if (p_ptr->riding)
			{
				msg_print(_("������ϤǤ��ޤ���", "You need to get off a pet."));
				return FALSE;
			}

			if (command == -3)
			{
				if (!choose_kamae()) return FALSE;
				p_ptr->update |= (PU_BONUS);
			}
			else if (command == -4)
			{
				int x, y;

				if (!get_rep_dir(&dir, FALSE)) return FALSE;
				y = p_ptr->y + ddy[dir];
				x = p_ptr->x + ddx[dir];
				if (cave[y][x].m_idx)
				{
					if (one_in_(2)) 
						msg_print(_("������������������������������������������������������", 
									"Ahhhtatatatatatatatatatatatatatataatatatatattaaaaa!!!!"));
					else
						msg_print(_("���饪�饪�饪�饪�饪�饪�饪�饪�饪�饪�饪�顪����",
									"Oraoraoraoraoraoraoraoraoraoraoraoraoraoraoraoraora!!!!"));

					py_attack(y, x, 0);
					if (cave[y][x].m_idx)
					{
						handle_stuff();
						py_attack(y, x, 0);
					}
					p_ptr->energy_need += ENERGY_NEED();
				}
				else
				{
					msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "You don't see any monster in this direction"));
					msg_print(NULL);
				}
			}
			break;
		}
		case CLASS_MINDCRAFTER:
		case CLASS_FORCETRAINER:
		{
			if (total_friends)
			{
				msg_print(_("���ϥڥåȤ���뤳�Ȥ˽��椷�Ƥ��ʤ��ȡ�", "You need concentration on the pets now."));
				return FALSE;
			}
			msg_print(_("����Ƭ���ϥå��ꤷ����", "You feel your head clear a little."));

			p_ptr->csp += (3 + p_ptr->lev/20);
			if (p_ptr->csp >= p_ptr->msp)
			{
				p_ptr->csp = p_ptr->msp;
				p_ptr->csp_frac = 0;
			}

			/* Redraw mana */
			p_ptr->redraw |= (PR_MANA);
			break;
		}
		case CLASS_TOURIST:
		{
			if (command == -3)
			{
				if (!get_aim_dir(&dir)) return FALSE;
				project_length = 1;
				fire_beam(GF_PHOTO, dir, 1);
			}
			else if (command == -4)
			{
				if (!identify_fully(FALSE)) return FALSE;
			}
			break;
		}
		case CLASS_IMITATOR:
		{
			handle_stuff();
			if (!do_cmd_mane(TRUE)) return FALSE;
			break;
		}
		case CLASS_BEASTMASTER:
		{
			if (command == -3)
			{
				if (!get_aim_dir(&dir)) return FALSE;
				(void)fire_ball_hide(GF_CONTROL_LIVING, dir, p_ptr->lev, 0);
			}
			else if (command == -4)
			{
				project_hack(GF_CONTROL_LIVING, p_ptr->lev);
			}
			break;
		}
		case CLASS_ARCHER:
		{
			if (!do_cmd_archer()) return FALSE;
			break;
		}
		case CLASS_MAGIC_EATER:
		{
			if (command == -3) {
				if (!gain_magic()) return FALSE;
			} else if (command == -4) {
				if (!can_do_cmd_cast()) return FALSE;
				if (!do_cmd_magic_eater(FALSE, TRUE)) return FALSE;
			}
			break;
		}
		case CLASS_BARD:
		{
			/* Singing is already stopped */
			if (!SINGING_SONG_EFFECT(p_ptr) && !INTERUPTING_SONG_EFFECT(p_ptr)) return FALSE;

			stop_singing();
			p_ptr->energy_use = 10;
			break;
		}
		case CLASS_RED_MAGE:
		{
			if (!can_do_cmd_cast()) return FALSE;
			handle_stuff();
			do_cmd_cast();
			handle_stuff();
			if (!p_ptr->paralyzed && can_do_cmd_cast())
				do_cmd_cast();
			break;
		}
		case CLASS_SAMURAI:
		{
			if (command == -3)
			{
				int max_csp = MAX(p_ptr->msp*4, p_ptr->lev*5+5);

				if (total_friends)
				{
					msg_print(_("���ϥڥåȤ���뤳�Ȥ˽��椷�Ƥ��ʤ��ȡ�", "You need concentration on the pets now."));
					return FALSE;
				}
				if (p_ptr->special_defense & KATA_MASK)
				{
					msg_print(_("���Ϲ����˽��椷�Ƥ��롣", "You need concentration on your form."));
					return FALSE;
				}
				msg_print(_("�������椷�Ƶ��礤��ί�᤿��", "You concentrate to charge your power."));

				p_ptr->csp += p_ptr->msp / 2;
				if (p_ptr->csp >= max_csp)
				{
					p_ptr->csp = max_csp;
					p_ptr->csp_frac = 0;
				}

				/* Redraw mana */
				p_ptr->redraw |= (PR_MANA);
			}
			else if (command == -4)
			{
				if (!buki_motteruka(INVEN_RARM) && !buki_motteruka(INVEN_LARM))
				{
					msg_print(_("��������ʤ��Ȥ����ޤ���", "You need to wield a weapon."));
					return FALSE;
				}
				if (!choose_kata()) return FALSE;
				p_ptr->update |= (PU_BONUS);
			}
			break;
		}
		case CLASS_BLUE_MAGE:
		{
			if (p_ptr->action == ACTION_LEARN)
			{
				set_action(ACTION_NONE);
			}
			else
			{
				set_action(ACTION_LEARN);
			}
			p_ptr->energy_use = 0;
			break;
		}
		case CLASS_CAVALRY:
		{
			char m_name[80];
			monster_type *m_ptr;
			monster_race *r_ptr;
			int rlev;

			if (p_ptr->riding)
			{
				msg_print(_("���Ͼ��������", "You ARE riding."));
				return FALSE;
			}
			if (!do_riding(TRUE)) return TRUE;
			m_ptr = &m_list[p_ptr->riding];
			r_ptr = &r_info[m_ptr->r_idx];
			monster_desc(m_name, m_ptr, 0);
			msg_format(_("%s�˾�ä���", "You ride on %s."),m_name);
			if (is_pet(m_ptr)) break;
			rlev = r_ptr->level;
			if (r_ptr->flags1 & RF1_UNIQUE) rlev = rlev * 3 / 2;
			if (rlev > 60) rlev = 60+(rlev-60)/2;
			if ((randint1(p_ptr->skill_exp[GINOU_RIDING] / 120 + p_ptr->lev * 2 / 3) > rlev)
			    && one_in_(2) && !p_ptr->inside_arena && !p_ptr->inside_battle
			    && !(r_ptr->flags7 & (RF7_GUARDIAN)) && !(r_ptr->flags1 & (RF1_QUESTOR))
			    && (rlev < p_ptr->lev * 3 / 2 + randint0(p_ptr->lev / 5)))
			{
				msg_format(_("%s���ʤ�������", "You tame %s."),m_name);
				set_pet(m_ptr);
			}
			else
			{
				msg_format(_("%s�˿�����Ȥ��줿��", "You have thrown off by %s."),m_name);
				rakuba(1,TRUE);

				/* Paranoia */
				/* ���Ͻ����˼��Ԥ��Ƥ�Ȥˤ������ϲ�� */
				p_ptr->riding = 0;
			}
			break;
		}
		case CLASS_BERSERKER:
		{
			if (!word_of_recall()) return FALSE;
			break;
		}
		case CLASS_SMITH:
		{
			if (p_ptr->lev > 29)
			{
				if (!identify_fully(TRUE)) return FALSE;
			}
			else
			{
				if (!ident_spell(TRUE)) return FALSE;
			}
			break;
		}
		case CLASS_MIRROR_MASTER:
		{
			if (command == -3)
			{
				/* Explode all mirrors */
				remove_all_mirrors(TRUE);
			}
			else if (command == -4)
			{
				if (total_friends)
				{
					msg_print(_("���ϥڥåȤ���뤳�Ȥ˽��椷�Ƥ��ʤ��ȡ�", "You need concentration on the pets now."));
					return FALSE;
				}
				if (is_mirror_grid(&cave[p_ptr->y][p_ptr->x]))
				{
					msg_print(_("����Ƭ���ϥå��ꤷ����", "You feel your head clear a little."));

					p_ptr->csp += (5 + p_ptr->lev * p_ptr->lev / 100);
					if (p_ptr->csp >= p_ptr->msp)
					{
						p_ptr->csp = p_ptr->msp;
						p_ptr->csp_frac = 0;
					}

					/* Redraw mana */
					p_ptr->redraw |= (PR_MANA);
				}
				else
				{
					msg_print(_("���ξ�Ǥʤ��Ƚ���Ǥ��ʤ���", "Here are not any mirrors!"));
				}
			}
			break;
		}
		case CLASS_NINJA:
		{
			if (p_ptr->action == ACTION_HAYAGAKE)
			{
				set_action(ACTION_NONE);
			}
			else
			{
				cave_type *c_ptr = &cave[p_ptr->y][p_ptr->x];
				feature_type *f_ptr = &f_info[c_ptr->feat];

				if (!have_flag(f_ptr->flags, FF_PROJECT) ||
				    (!p_ptr->levitation && have_flag(f_ptr->flags, FF_DEEP)))
				{
					msg_print(_("�����Ǥ����᤯ư���ʤ���", "You cannot run in here."));
				}
				else
				{
					set_action(ACTION_HAYAGAKE);
				}
			}


			p_ptr->energy_use = 0;
			break;
		}

		}
	}
	else if (p_ptr->mimic_form)
	{
		switch (p_ptr->mimic_form)
		{
		case MIMIC_DEMON:
		case MIMIC_DEMON_LORD:
		{
			int type = (one_in_(2) ? GF_NETHER : GF_FIRE);
			if (!get_aim_dir(&dir)) return FALSE;
			ratial_stop_mouth();
#ifdef JP
			msg_format("���ʤ���%s�Υ֥쥹���Ǥ�����",((type == GF_NETHER) ? "�Ϲ�" : "�б�"));
#else
			msg_format("You breathe %s.",((type == GF_NETHER) ? "nether" : "fire"));
#endif

			fire_breath(type, dir, plev * 3, (plev / 15) + 1);
			break;
		}
		case MIMIC_VAMPIRE:
			if (d_info[dungeon_type].flags1 & DF1_NO_MELEE)
			{
				msg_print(_("�ʤ������⤹�뤳�Ȥ��Ǥ��ʤ���", "Something prevent you from attacking."));
				return FALSE;
			}
			else
			{
				int y, x, dummy = 0;
				cave_type *c_ptr;

				/* Only works on adjacent monsters */
				if (!get_rep_dir(&dir, FALSE)) return FALSE;   /* was get_aim_dir */
				y = p_ptr->y + ddy[dir];
				x = p_ptr->x + ddx[dir];
				c_ptr = &cave[y][x];

				ratial_stop_mouth();

				if (!c_ptr->m_idx)
				{
					msg_print(_("����ʤ����˳��ߤĤ�����", "You bite into thin air!"));
					break;
				}

				msg_print(_("���ʤ��ϥ˥��Ȥ��Ʋ��त��...", "You grin and bare your fangs..."));
				dummy = plev + randint1(plev) * MAX(1, plev / 10);   /* Dmg */
				if (hypodynamic_bolt(dir, dummy))
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
						(void)set_food(dummy >= PY_FOOD_MAX ? PY_FOOD_MAX - 1 : dummy);
				}
				else
					msg_print(_("�������Ҥɤ�̣����", "Yechh. That tastes foul."));
			}
			break;
		}
	}

	else 
	{

	switch (p_ptr->prace)
	{
		case RACE_DWARF:
			msg_print(_("���Ϥ�Ĵ�٤���", "You examine your surroundings."));
			(void)detect_traps(DETECT_RAD_DEFAULT, TRUE);
			(void)detect_doors(DETECT_RAD_DEFAULT);
			(void)detect_stairs(DETECT_RAD_DEFAULT);
			break;

		case RACE_HOBBIT:
			{
				object_type *q_ptr;
				object_type forge;

				/* Get local object */
				q_ptr = &forge;

				/* Create the food ration */
				object_prep(q_ptr, lookup_kind(TV_FOOD, SV_FOOD_RATION));

				/* Drop the object from heaven */
				(void)drop_near(q_ptr, -1, p_ptr->y, p_ptr->x);
				msg_print(_("�������������ƺ�ä���", "You cook some food."));
			}
			break;

		case RACE_GNOME:
			msg_print(_("�ѥá�", "Blink!"));
			teleport_player(10, 0L);
			break;

		case RACE_HALF_ORC:
			msg_print(_("ͦ����Ф�����", "You play tough."));
			(void)set_afraid(0);
			break;

		case RACE_HALF_TROLL:
			msg_print(_("����������", "RAAAGH!"));
			(void)set_afraid(0);
			(void)set_shero(10 + randint1(plev), FALSE);
			(void)hp_player(30);
			break;

		case RACE_AMBERITE:
			if (command == -1)
			{
				msg_print(_("���ʤ����⤭����Ϥ᤿��", "You start walking around. "));
				alter_reality();
			}
			else if (command == -2)
			{
				msg_print(_("���ʤ��ϡ֥ѥ�����פ򿴤������Ƥ��ξ���⤤��...", "You picture the Pattern in your mind and walk it..."));

				(void)set_poisoned(0);
				(void)set_image(0);
				(void)set_stun(0);
				(void)set_cut(0);
				(void)set_blind(0);
				(void)set_afraid(0);
				(void)do_res_stat(A_STR);
				(void)do_res_stat(A_INT);
				(void)do_res_stat(A_WIS);
				(void)do_res_stat(A_DEX);
				(void)do_res_stat(A_CON);
				(void)do_res_stat(A_CHR);
				(void)restore_level();
			}
			break;

		case RACE_BARBARIAN:
			msg_print(_("������������", "Raaagh!"));
			(void)set_afraid(0);
			(void)set_shero(10 + randint1(plev), FALSE);
			(void)hp_player(30);
			break;

		case RACE_HALF_OGRE:
			msg_print(_("��ȯ�Υ롼��򿵽Ť˻ųݤ���...", "You carefully set an explosive rune..."));
			explosive_rune();
			break;

		case RACE_HALF_GIANT:
			if (!get_aim_dir(&dir)) return FALSE;
			(void)wall_to_mud(dir, 20 + randint1(30));
			break;

		case RACE_HALF_TITAN:
			msg_print(_("Ũ��Ĵ������...", "You examine your foes..."));
			probing();
			break;

		case RACE_CYCLOPS:
			if (!get_aim_dir(&dir)) return FALSE;
			msg_print(_("����ʴ���ꤲ����", "You throw a huge boulder."));
			fire_bolt(GF_MISSILE, dir, (3 * plev) / 2);
			break;

		case RACE_YEEK:
			if (!get_aim_dir(&dir)) return FALSE;
			ratial_stop_mouth();
			msg_print(_("�Ȥ��Ӥ����Ķ�������夲����", "You make a horrible scream!"));
			(void)fear_monster(dir, plev);
			break;

		case RACE_KLACKON:
			if (!get_aim_dir(&dir)) return FALSE;
			ratial_stop_mouth();
			msg_print(_("�����Ǥ�����", "You spit acid."));
			if (plev < 25) fire_bolt(GF_ACID, dir, plev);
			else fire_ball(GF_ACID, dir, plev, 2);
			break;

		case RACE_KOBOLD:
			if (!get_aim_dir(&dir)) return FALSE;
			msg_print(_("�ǤΥ����Ĥ��ꤲ����", "You throw a dart of poison."));
			fire_bolt(GF_POIS, dir, plev);
			break;

		case RACE_NIBELUNG:
			msg_print(_("���Ϥ�Ĵ��������", "You examine your surroundings."));
			(void)detect_traps(DETECT_RAD_DEFAULT, TRUE);
			(void)detect_doors(DETECT_RAD_DEFAULT);
			(void)detect_stairs(DETECT_RAD_DEFAULT);
			break;

		case RACE_DARK_ELF:
			if (!get_aim_dir(&dir)) return FALSE;
			msg_print(_("�ޥ��å����ߥ���������ä���", "You cast a magic missile."));
			fire_bolt_or_beam(10, GF_MISSILE, dir,
			    damroll(3 + ((plev - 1) / 5), 4));
			break;

		case RACE_DRACONIAN:
			{
				int  Type = (one_in_(3) ? GF_COLD : GF_FIRE);
#ifdef JP
				cptr Type_desc = ((Type == GF_COLD) ? "�䵤" : "��");
#else
				cptr Type_desc = ((Type == GF_COLD) ? "cold" : "fire");
#endif

				if (!get_aim_dir(&dir)) return FALSE;

				if (randint1(100) < plev)
				{
					switch (p_ptr->pclass)
					{
						case CLASS_WARRIOR:
						case CLASS_BERSERKER:
						case CLASS_RANGER:
						case CLASS_TOURIST:
						case CLASS_IMITATOR:
						case CLASS_ARCHER:
						case CLASS_SMITH:
							if (one_in_(3))
							{
								Type = GF_MISSILE;
								Type_desc = _("�������", "the elements");
							}
							else
							{
								Type = GF_SHARDS;
								Type_desc = _("����", "shards");
							}
							break;
						case CLASS_MAGE:
						case CLASS_WARRIOR_MAGE:
						case CLASS_HIGH_MAGE:
						case CLASS_SORCERER:
						case CLASS_MAGIC_EATER:
						case CLASS_RED_MAGE:
						case CLASS_BLUE_MAGE:
						case CLASS_MIRROR_MASTER:
							if (one_in_(3))
							{
								Type = GF_MANA;
								Type_desc = _("����", "mana");
							}
							else
							{
								Type = GF_DISENCHANT;
								Type_desc = _("����", "disenchantment");
							}
							break;
						case CLASS_CHAOS_WARRIOR:
							if (!one_in_(3))
							{
								Type = GF_CONFUSION;
								Type_desc = _("����", "confusion");
							}
							else
							{
								Type = GF_CHAOS;
								Type_desc = _("������", "chaos");
							}
							break;
						case CLASS_MONK:
						case CLASS_SAMURAI:
						case CLASS_FORCETRAINER:
							if (!one_in_(3))
							{
								Type = GF_CONFUSION;
								Type_desc = _("����", "confusion");
							}
							else
							{
								Type = GF_SOUND;
								Type_desc = _("�첻", "sound");
							}
							break;
						case CLASS_MINDCRAFTER:
							if (!one_in_(3))
							{
								Type = GF_CONFUSION;
								Type_desc = _("����", "confusion");
							}
							else
							{
								Type = GF_PSI;
								Type_desc = _("�������ͥ륮��", "mental energy");
							}
							break;
						case CLASS_PRIEST:
						case CLASS_PALADIN:
							if (one_in_(3))
							{
								Type = GF_HELL_FIRE;
								Type_desc = _("�Ϲ��ι��", "hellfire");
							}
							else
							{
								Type = GF_HOLY_FIRE;
								Type_desc = _("���ʤ��", "holy fire");
							}
							break;
						case CLASS_ROGUE:
						case CLASS_NINJA:
							if (one_in_(3))
							{
								Type = GF_DARK;
								Type_desc = _("�Ź�", "darkness");
							}
							else
							{
								Type = GF_POIS;
								Type_desc = _("��", "poison");
							}
							break;
						case CLASS_BARD:
							if (!one_in_(3))
							{
								Type = GF_SOUND;
								Type_desc = _("�첻", "sound");
							}
							else
							{
								Type = GF_CONFUSION;
								Type_desc = _("����", "confusion");
							}
							break;
					}
				}

				ratial_stop_mouth();
				msg_format(_("���ʤ���%s�Υ֥쥹���Ǥ�����", "You breathe %s."), Type_desc);

				fire_ball(Type, dir, plev * 2,
				    -(plev / 15) - 1);
			}
			break;

		case RACE_MIND_FLAYER:
			if (!get_aim_dir(&dir)) return FALSE;
			msg_print(_("���ʤ��Ͻ��椷���ܤ��֤�������...", "You concentrate and your eyes glow red..."));
			fire_bolt(GF_PSI, dir, plev);
			break;

		case RACE_IMP:
			if (!get_aim_dir(&dir)) return FALSE;
			if (plev >= 30)
			{
				msg_print(_("�ե��������ܡ�������ä���", "You cast a ball of fire."));
				fire_ball(GF_FIRE, dir, plev, 2);
			}
			else
			{
				msg_print(_("�ե��������ܥ�Ȥ����ä���", "You cast a bolt of fire."));
				fire_bolt(GF_FIRE, dir, plev);
			}
			break;

		case RACE_GOLEM:
			(void)set_shield(randint1(20) + 30, FALSE);
			break;

		case RACE_SKELETON:
		case RACE_ZOMBIE:
			msg_print(_("���ʤ��ϼ��ä����ͥ륮�������᤽���Ȼ�ߤ���", "You attempt to restore your lost energies."));
			(void)restore_level();
			break;

		case RACE_VAMPIRE:
			if (d_info[dungeon_type].flags1 & DF1_NO_MELEE)
			{
				msg_print(_("�ʤ������⤹�뤳�Ȥ��Ǥ��ʤ���", "Something prevent you from attacking."));
				return FALSE;
			}
			else
			{
				int y, x, dummy = 0;
				cave_type *c_ptr;

				/* Only works on adjacent monsters */
				if (!get_rep_dir(&dir,FALSE)) return FALSE;   /* was get_aim_dir */
				y = p_ptr->y + ddy[dir];
				x = p_ptr->x + ddx[dir];
				c_ptr = &cave[y][x];

				ratial_stop_mouth();

				if (!c_ptr->m_idx)
				{
					msg_print(_("����ʤ����˳��ߤĤ�����", "You bite into thin air!"));
					break;
				}

				msg_print(_("���ʤ��ϥ˥��Ȥ��Ʋ��त��...", "You grin and bare your fangs..."));
				dummy = plev + randint1(plev) * MAX(1, plev / 10);   /* Dmg */
				if (hypodynamic_bolt(dir, dummy))
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
						(void)set_food(dummy >= PY_FOOD_MAX ? PY_FOOD_MAX - 1 : dummy);
				}
				else
					msg_print(_("�������Ҥɤ�̣����", "Yechh. That tastes foul."));
			}
			break;

		case RACE_SPECTRE:
			if (!get_aim_dir(&dir)) return FALSE;
			ratial_stop_mouth();
			msg_print(_("���ʤ��Ϥ��ɤ��ɤ����������򤢤�����", "You emit an eldritch howl!"));
			(void)fear_monster(dir, plev);
			break;

		case RACE_SPRITE:
			msg_print(_("���ʤ�����ˡ��ʴ���ꤲ�Ĥ���...", "You throw some magic dust..."));
			if (plev < 25) sleep_monsters_touch();
			else (void)sleep_monsters(plev);
			break;

		case RACE_DEMON:
			{
				int type = (one_in_(2) ? GF_NETHER : GF_FIRE);
				if (!get_aim_dir(&dir)) return FALSE;
				ratial_stop_mouth();
#ifdef JP
				msg_format("���ʤ���%s�Υ֥쥹���Ǥ�����",((type == GF_NETHER) ? "�Ϲ�" : "�б�"));
#else
				msg_format("You breathe %s.",((type == GF_NETHER) ? "nether" : "fire"));
#endif

				fire_breath(type, dir, plev * 3, (plev / 15) + 1);
			}
			break;

		case RACE_KUTAR:
			(void)set_tsubureru(randint1(20) + 30, FALSE);
			break;

		case RACE_ANDROID:
			if (!get_aim_dir(&dir)) return FALSE;
			if (plev < 10)
			{
				msg_print(_("�쥤�����ȯ�ͤ�����", "You fire your ray gun."));
				fire_bolt(GF_MISSILE, dir, (plev+1) / 2);
			}
			else if (plev < 25)
			{
				msg_print(_("�֥饹������ȯ�ͤ�����", "You fire your blaster."));
				fire_bolt(GF_MISSILE, dir, plev);
			}
			else if (plev < 35)
			{
				msg_print(_("�Х�������ȯ�ͤ�����", "You fire your bazooka."));
				fire_ball(GF_MISSILE, dir, plev * 2, 2);
			}
			else if (plev < 45)
			{
				msg_print(_("�ӡ��७��Υ��ȯ�ͤ�����", "You fire a beam cannon."));
				fire_beam(GF_MISSILE, dir, plev * 2);
			}
			else
			{
				msg_print(_("���åȤ�ȯ�ͤ�����", "You fire a rocket."));
				fire_rocket(GF_ROCKET, dir, plev * 5, 2);
			}
			break;

		default:
			msg_print(_("���μ�²���ü��ǽ�Ϥ���äƤ��ޤ���", "This race has no bonus power."));
			p_ptr->energy_use = 0;
	}
	}
	return TRUE;
}

/*!
 * @brief �쥤����롦�ѥ���ޥ�ɤΥᥤ��롼���� / Allow user to choose a power (racial / mutation) to activate
 * @return �ʤ�
 */
void do_cmd_racial_power(void)
{
	power_desc_type power_desc[36];
	int num;
	COMMAND_CODE i = 0;
	int ask = TRUE;
	PLAYER_LEVEL lvl = p_ptr->lev;
	bool            flag, redraw, cast = FALSE;
	bool            warrior = ((p_ptr->pclass == CLASS_WARRIOR || p_ptr->pclass == CLASS_BERSERKER) ? TRUE : FALSE);
	char            choice;
	char            out_val[160];
	int menu_line = (use_menu ? 1 : 0);


	for (num = 0; num < 36; num++)
	{
		strcpy(power_desc[num].name, "");
		power_desc[num].number = 0;
	}

	num = 0;

	if (p_ptr->confused)
	{
		msg_print(_("���𤷤Ƥ����ü�ǽ�Ϥ�Ȥ��ޤ���", "You are too confused to use any powers!"));
		p_ptr->energy_use = 0;
		return;
	}

	if (p_ptr->special_defense & (KATA_MUSOU | KATA_KOUKIJIN))
	{
		set_action(ACTION_NONE);
	}

	switch (p_ptr->pclass)
	{
	case CLASS_WARRIOR:
	{
		strcpy(power_desc[num].name, _("������", "Sword Dancing"));
		power_desc[num].level = 40;
		power_desc[num].cost = 75;
		power_desc[num].stat = A_DEX;
		power_desc[num].fail = 35;
		power_desc[num++].number = -3;
		break;
	}
	case CLASS_HIGH_MAGE:
	if (p_ptr->realm1 == REALM_HEX)
	{
		strcpy(power_desc[num].name, _("�Ӿ������", "Stop spelling"));
		power_desc[num].level = 1;
		power_desc[num].cost = 0;
		power_desc[num].stat = A_INT;
		power_desc[num].fail = 0;
		power_desc[num++].number = -3;
		break;
	}
	case CLASS_MAGE:
	/* case CLASS_HIGH_MAGE: */
	case CLASS_SORCERER:
	{
		strcpy(power_desc[num].name, _("���Ͽ���", "Eat Magic"));
		power_desc[num].level = 25;
		power_desc[num].cost = 1;
		power_desc[num].stat = A_INT;
		power_desc[num].fail = 25;
		power_desc[num++].number = -3;
		break;
	}
	case CLASS_PRIEST:
	{
		if (is_good_realm(p_ptr->realm1))
		{
			strcpy(power_desc[num].name, _("����ʡ", "Bless Weapon"));
			power_desc[num].level = 35;
			power_desc[num].cost = 70;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 50;
			power_desc[num++].number = -3;
		}
		else
		{
			strcpy(power_desc[num].name, _("����", "Evocation"));
			power_desc[num].level = 42;
			power_desc[num].cost = 40;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 35;
			power_desc[num++].number = -3;
		}
		break;
	}
	case CLASS_ROGUE:
	{
		strcpy(power_desc[num].name, _("�ҥåȡ���������", "Hit and Away"));
		power_desc[num].level = 8;
		power_desc[num].cost = 12;
		power_desc[num].stat = A_DEX;
		power_desc[num].fail = 14;
		power_desc[num++].number = -3;
		break;
	}
	case CLASS_RANGER:
	case CLASS_SNIPER:
	{
		strcpy(power_desc[num].name, _("��󥹥���Ĵ��", "Probe Monster"));
		power_desc[num].level = 15;
		power_desc[num].cost = 20;
		power_desc[num].stat = A_INT;
		power_desc[num].fail = 12;
		power_desc[num++].number = -3;
		break;
	}
	case CLASS_PALADIN:
	{
		if (is_good_realm(p_ptr->realm1))
		{
			strcpy(power_desc[num].name, _("�ۡ��꡼�����", "Holy Lance"));
			power_desc[num].level = 30;
			power_desc[num].cost = 30;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 30;
			power_desc[num++].number = -3;
		}
		else
		{
			strcpy(power_desc[num].name, _("�إ롦���", "Hell Lance"));
			power_desc[num].level = 30;
			power_desc[num].cost = 30;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 30;
			power_desc[num++].number = -3;
		}
		break;
	}
	case CLASS_WARRIOR_MAGE:
	{
		strcpy(power_desc[num].name, _("�Ѵ�: �ȣТ��ͣ�", "Convert HP to SP"));
		power_desc[num].level = 25;
		power_desc[num].cost = 0;
		power_desc[num].stat = A_INT;
		power_desc[num].fail = 10;
		power_desc[num++].number = -3;
			
		strcpy(power_desc[num].name, _("�Ѵ�: �ͣТ��ȣ�", "Convert SP to HP"));
		power_desc[num].level = 25;
		power_desc[num].cost = 0;
		power_desc[num].stat = A_INT;
		power_desc[num].fail = 10;
		power_desc[num++].number = -4;
		break;
	}
	case CLASS_CHAOS_WARRIOR:
	{
		strcpy(power_desc[num].name, _("���Ǥθ�", "Confusing Light"));
		power_desc[num].level = 40;
		power_desc[num].cost = 50;
		power_desc[num].stat = A_INT;
		power_desc[num].fail = 25;
		power_desc[num++].number = -3;
		break;
	}
	case CLASS_MONK:
	{
		strcpy(power_desc[num].name, _("������", "Assume a Posture"));
		power_desc[num].level = 25;
		power_desc[num].cost = 0;
		power_desc[num].stat = A_DEX;
		power_desc[num].fail = 0;
		power_desc[num++].number = -3;
			
		strcpy(power_desc[num].name, _("ɴ����", "Double Attack"));
		power_desc[num].level = 30;
		power_desc[num].cost = 30;
		power_desc[num].stat = A_STR;
		power_desc[num].fail = 20;
		power_desc[num++].number = -4;
		break;
	}
	case CLASS_MINDCRAFTER:
	case CLASS_FORCETRAINER:
	{
		strcpy(power_desc[num].name, _("�����߿�", "Clear Mind"));
		power_desc[num].level = 15;
		power_desc[num].cost = 0;
		power_desc[num].stat = A_WIS;
		power_desc[num].fail = 10;
		power_desc[num++].number = -3;
		break;
	}
	case CLASS_TOURIST:
	{
		strcpy(power_desc[num].name, _("�̿�����", "Take a Photograph"));
		power_desc[num].level = 1;
		power_desc[num].cost = 0;
		power_desc[num].stat = A_DEX;
		power_desc[num].fail = 0;
		power_desc[num++].number = -3;
		
		strcpy(power_desc[num].name, _("��������", "Identify True"));
		power_desc[num].level = 25;
		power_desc[num].cost = 20;
		power_desc[num].stat = A_INT;
		power_desc[num].fail = 20;
		power_desc[num++].number = -4;
		break;
	}
	case CLASS_IMITATOR:
	{
		strcpy(power_desc[num].name, _("���֤�", "Double Revenge"));
		power_desc[num].level = 30;
		power_desc[num].cost = 100;
		power_desc[num].stat = A_DEX;
		power_desc[num].fail = 30;
		power_desc[num++].number = -3;
		break;
	}
	case CLASS_BEASTMASTER:
	{
		strcpy(power_desc[num].name, _("��ʪ����", "Dominate a Living Thing"));
		power_desc[num].level = 1;
		power_desc[num].cost = (p_ptr->lev+3)/4;
		power_desc[num].stat = A_CHR;
		power_desc[num].fail = 10;
		power_desc[num++].number = -3;
		
		strcpy(power_desc[num].name, _("������ʪ����", "Dominate Living Things"));
		power_desc[num].level = 30;
		power_desc[num].cost = (p_ptr->lev+20)/2;
		power_desc[num].stat = A_CHR;
		power_desc[num].fail = 10;
		power_desc[num++].number = -4;
		break;
	}
	case CLASS_ARCHER:
	{
		strcpy(power_desc[num].name, _("��/�����¤", "Create Ammo"));
		power_desc[num].level = 1;
		power_desc[num].cost = 0;
		power_desc[num].stat = A_DEX;
		power_desc[num].fail = 0;
		power_desc[num++].number = -3;
		break;
	}
	case CLASS_MAGIC_EATER:
	{
		strcpy(power_desc[num].name, _("���Ϥμ�����", "Absorb Magic"));
		power_desc[num].level = 1;
		power_desc[num].cost = 0;
		power_desc[num].stat = A_INT;
		power_desc[num].fail = 0;
		power_desc[num++].number = -3;

		strcpy(power_desc[num].name, _("����ȯư", "Powerful Activation"));
		power_desc[num].level = 10;
		power_desc[num].cost = 10 + (lvl - 10) / 2;
		power_desc[num].stat = A_INT;
		power_desc[num].fail = 0;
		power_desc[num++].number = -4;
		break;
	}
	case CLASS_BARD:
	{
		strcpy(power_desc[num].name, _("�Τ�ߤ��", "Stop Singing"));
		power_desc[num].level = 1;
		power_desc[num].cost = 0;
		power_desc[num].stat = A_CHR;
		power_desc[num].fail = 0;
		power_desc[num++].number = -3;
		break;
	}
	case CLASS_RED_MAGE:
	{
		strcpy(power_desc[num].name, _("Ϣ³��", "Double Magic"));
		power_desc[num].level = 48;
		power_desc[num].cost = 20;
		power_desc[num].stat = A_INT;
		power_desc[num].fail = 0;
		power_desc[num++].number = -3;
		break;
	}
	case CLASS_SAMURAI:
	{
		strcpy(power_desc[num].name, _("���礤����", "Concentration"));
		power_desc[num].level = 1;
		power_desc[num].cost = 0;
		power_desc[num].stat = A_WIS;
		power_desc[num].fail = 0;
		power_desc[num++].number = -3;
		
		strcpy(power_desc[num].name, _("��", "Assume a Posture"));
		power_desc[num].level = 25;
		power_desc[num].cost = 0;
		power_desc[num].stat = A_DEX;
		power_desc[num].fail = 0;
		power_desc[num++].number = -4;
		break;
	}
	case CLASS_BLUE_MAGE:
	{
		strcpy(power_desc[num].name, _("�顼�˥�", "Learning"));
		power_desc[num].level = 1;
		power_desc[num].cost = 0;
		power_desc[num].stat = A_INT;
		power_desc[num].fail = 0;
		power_desc[num++].number = -3;
		break;
	}
	case CLASS_CAVALRY:
	{
		strcpy(power_desc[num].name, _("���Ϥʤ餷", "Rodeo"));
		power_desc[num].level = 10;
		power_desc[num].cost = 0;
		power_desc[num].stat = A_STR;
		power_desc[num].fail = 10;
		power_desc[num++].number = -3;
		break;
	}
	case CLASS_BERSERKER:
	{
		strcpy(power_desc[num].name, _("����", "Recall"));
		power_desc[num].level = 10;
		power_desc[num].cost = 10;
		power_desc[num].stat = A_DEX;
		power_desc[num].fail = 20;
		power_desc[num++].number = -3;
		break;
	}
	case CLASS_MIRROR_MASTER:
	{
		strcpy(power_desc[num].name, _("�����", "Break Mirrors"));
		power_desc[num].level = 1;
		power_desc[num].cost = 0;
		power_desc[num].stat = A_INT;
		power_desc[num].fail = 0;
		power_desc[num++].number = -3;
		
		strcpy(power_desc[num].name, _("�ſ�", "Mirror Concentration"));
		power_desc[num].level = 30;
		power_desc[num].cost = 0;
		power_desc[num].stat = A_INT;
		power_desc[num].fail = 20;
		power_desc[num++].number = -4;
		break;
	}
	case CLASS_SMITH:
	{
		strcpy(power_desc[num].name, _("������", "Judgment"));
		power_desc[num].level = 5;
		power_desc[num].cost = 15;
		power_desc[num].stat = A_INT;
		power_desc[num].fail = 20;
		power_desc[num++].number = -3;
		break;
	}
	case CLASS_NINJA:
	{
		strcpy(power_desc[num].name, _("®�", "Quick Walk"));
		power_desc[num].level = 20;
		power_desc[num].cost = 0;
		power_desc[num].stat = A_DEX;
		power_desc[num].fail = 0;
		power_desc[num++].number = -3;
		break;
	}
	default:
		strcpy(power_desc[0].name, _("(�ʤ�)", "(none)"));
	}

	if (p_ptr->mimic_form)
	{
		switch (p_ptr->mimic_form)
		{
		case MIMIC_DEMON:
		case MIMIC_DEMON_LORD:
			sprintf(power_desc[num].name, _("�Ϲ�/�б�Υ֥쥹 (���᡼�� %d)", "Nether or Fire Breath (dam %d)"), lvl * 3);
			power_desc[num].level = 15;
			power_desc[num].cost = 10+lvl/3;
			power_desc[num].stat = A_CON;
			power_desc[num].fail = 20;
			power_desc[num++].number = -1;
			break;
		case MIMIC_VAMPIRE:
			strcpy(power_desc[num].name, _("�۷�", "Vampiric Drain"));
			power_desc[num].level = 2;
			power_desc[num].cost = 1 + (lvl / 3);
			power_desc[num].stat = A_CON;
			power_desc[num].fail = 9;
			power_desc[num++].number = -1;
			break;
		}
	}
	else
	{
	switch (p_ptr->prace)
	{
		case RACE_DWARF:
			strcpy(power_desc[num].name, _("�ɥ���� ����", "Detect Doors+Traps"));
			power_desc[num].level = 5;
			power_desc[num].cost = 5;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 12;
			power_desc[num++].number = -1;
			break;
		case RACE_NIBELUNG:
			strcpy(power_desc[num].name, _("�ɥ���� ����", "Detect Doors+Traps"));
			power_desc[num].level = 10;
			power_desc[num].cost = 5;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 10;
			power_desc[num++].number = -1;
			break;
		case RACE_HOBBIT:
			strcpy(power_desc[num].name, _("��������", "Create Food"));
			power_desc[num].level = 15;
			power_desc[num].cost = 10;
			power_desc[num].stat = A_INT;
			power_desc[num].fail = 10;
			power_desc[num++].number = -1;
			break;
		case RACE_GNOME:
			sprintf(power_desc[num].name, _("���硼�ȡ��ƥ�ݡ���", "Blink"));
			power_desc[num].level = 5;
			power_desc[num].cost = 5;
			power_desc[num].stat = A_INT;
			power_desc[num].fail = 12;
			power_desc[num++].number = -1;
			break;
		case RACE_HALF_ORC:
			strcpy(power_desc[num].name, _("���ݽ���", "Remove Fear"));
			power_desc[num].level = 3;
			power_desc[num].cost = 5;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = warrior ? 5 : 10;
			power_desc[num++].number = -1;
			break;
		case RACE_HALF_TROLL:
			strcpy(power_desc[num].name, _("����β�", "Berserk"));
			power_desc[num].level = 10;
			power_desc[num].cost = 12;
			power_desc[num].stat = A_STR;
			power_desc[num].fail = warrior ? 6 : 12;
			power_desc[num++].number = -1;
			break;
		case RACE_BARBARIAN:
			strcpy(power_desc[num].name, _("����β�", "Berserk"));
			power_desc[num].level = 8;
			power_desc[num].cost = 10;
			power_desc[num].stat = A_STR;
			power_desc[num].fail = warrior ? 6 : 12;
			power_desc[num++].number = -1;
			break;
		case RACE_AMBERITE:
			strcpy(power_desc[num].name, _("����ɥ������ե�", "Shadow Shifting"));
			power_desc[num].level = 30;
			power_desc[num].cost = 50;
			power_desc[num].stat = A_INT;
			power_desc[num].fail = 50;
			power_desc[num++].number = -1;
			
			strcpy(power_desc[num].name, _("�ѥ����󡦥�������", "Pattern Mindwalking"));
			power_desc[num].level = 40;
			power_desc[num].cost = 75;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 50;
			power_desc[num++].number = -2;
			break;
		case RACE_HALF_OGRE:
			strcpy(power_desc[num].name, _("��ȯ�Υ롼��", "Explosive Rune"));
			power_desc[num].level = 25;
			power_desc[num].cost = 35;
			power_desc[num].stat = A_INT;
			power_desc[num].fail = 15;
			power_desc[num++].number = -1;
			break;
		case RACE_HALF_GIANT:
			strcpy(power_desc[num].name, _("�����ϲ�", "Stone to Mud"));
			power_desc[num].level = 20;
			power_desc[num].cost = 10;
			power_desc[num].stat = A_STR;
			power_desc[num].fail = 12;
			power_desc[num++].number = -1;
			break;
		case RACE_HALF_TITAN:
			strcpy(power_desc[num].name, _("������󡦥�󥹥���", "Probing"));
			power_desc[num].level = 15;
			power_desc[num].cost = 10;
			power_desc[num].stat = A_INT;
			power_desc[num].fail = 12;
			power_desc[num++].number = -1;
			break;
		case RACE_CYCLOPS:
			sprintf(power_desc[num].name, _("�����ꤲ�ʥ��᡼�� %d��", "Throw Boulder (dam %d)"), (3 * lvl) / 2);
			power_desc[num].level = 20;
			power_desc[num].cost = 15;
			power_desc[num].stat = A_STR;
			power_desc[num].fail = 12;
			power_desc[num++].number = -1;
			break;
		case RACE_YEEK:
			strcpy(power_desc[num].name, _("��󥹥�������", "Scare Monster"));
			power_desc[num].level = 15;
			power_desc[num].cost = 15;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 10;
			power_desc[num++].number = -1;
			break;
		case RACE_SPECTRE:
			strcpy(power_desc[num].name, _("��󥹥�������", "Scare Monster"));
			power_desc[num].level = 4;
			power_desc[num].cost = 6;
			power_desc[num].stat = A_INT;
			power_desc[num].fail = 3;
			power_desc[num++].number = -1;
			break;
		case RACE_KLACKON:
			sprintf(power_desc[num].name, _("������ (���᡼�� %d)", "Spit Acid (dam %d)"), lvl);
			power_desc[num].level = 9;
			power_desc[num].cost = 9;
			power_desc[num].stat = A_DEX;
			power_desc[num].fail = 14;
			power_desc[num++].number = -1;
			break;
		case RACE_KOBOLD:
			sprintf(power_desc[num].name, _("�ǤΥ����� (���᡼�� %d)", "Poison Dart (dam %d)"), lvl);
			power_desc[num].level = 12;
			power_desc[num].cost = 8;
			power_desc[num].stat = A_DEX;
			power_desc[num].fail = 14;
			power_desc[num++].number = -1;
			break;
		case RACE_DARK_ELF:
			sprintf(power_desc[num].name, _("�ޥ��å����ߥ����� (���᡼�� %dd%d)", "Magic Missile (dm %dd%d)"), 3 + ((lvl - 1) / 5), 4);
			power_desc[num].level = 2;
			power_desc[num].cost = 2;
			power_desc[num].stat = A_INT;
			power_desc[num].fail = 9;
			power_desc[num++].number = -1;
			break;
		case RACE_DRACONIAN:
			sprintf(power_desc[num].name, _("�֥쥹 (���᡼�� %d)", "Breath Weapon (dam %d)"), lvl * 2);
			power_desc[num].level = 1;
			power_desc[num].cost = lvl;
			power_desc[num].stat = A_CON;
			power_desc[num].fail = 12;
			power_desc[num++].number = -1;
			break;
		case RACE_MIND_FLAYER:
			sprintf(power_desc[num].name, _("�������� (���᡼�� %d)", "Mind Blast (dam %d)"), lvl);
			power_desc[num].level = 15;
			power_desc[num].cost = 12;
			power_desc[num].stat = A_INT;
			power_desc[num].fail = 14;
			power_desc[num++].number = -1;
			break;
		case RACE_IMP:
			sprintf(power_desc[num].name, _("�ե��������ܥ��/�ܡ��� (���᡼�� %d)", "Fire Bolt/Ball (dam %d)"), lvl);
			power_desc[num].level = 9;
			power_desc[num].cost = 15;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 15;
			power_desc[num++].number = -1;
			break;
		case RACE_GOLEM:
			strcpy(power_desc[num].name, _("ȩ�в� (���� 1d20+30)", "Stone Skin (dur 1d20+30)"));
			power_desc[num].level = 20;
			power_desc[num].cost = 15;
			power_desc[num].stat = A_CON;
			power_desc[num].fail = 8;
			power_desc[num++].number = -1;
			break;
		case RACE_SKELETON:
		case RACE_ZOMBIE:
			strcpy(power_desc[num].name, _("�и�������", "Restore Experience"));
			power_desc[num].level = 30;
			power_desc[num].cost = 30;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 18;
			power_desc[num++].number = -1;
			break;
		case RACE_VAMPIRE:
			strcpy(power_desc[num].name, _("�۷�", "Vampiric Drain"));
			power_desc[num].level = 2;
			power_desc[num].cost = 1 + (lvl / 3);
			power_desc[num].stat = A_CON;
			power_desc[num].fail = 9;
			power_desc[num++].number = -1;
			break;
		case RACE_SPRITE:
			strcpy(power_desc[num].name, _("̲��ʴ", "Sleeping Dust"));
			power_desc[num].level = 12;
			power_desc[num].cost = 12;
			power_desc[num].stat = A_INT;
			power_desc[num].fail = 15;
			power_desc[num++].number = -1;
			break;
		case RACE_DEMON:
			sprintf(power_desc[num].name, _("�Ϲ�/�б�Υ֥쥹 (���᡼�� %d)", "Nether or Fire Breath (dam %d)"), lvl * 3);
			power_desc[num].level = 15;
			power_desc[num].cost = 10+lvl/3;
			power_desc[num].stat = A_CON;
			power_desc[num].fail = 20;
			power_desc[num++].number = -1;
			break;
		case RACE_KUTAR:
			strcpy(power_desc[num].name, _("���˿��Ӥ�", "Expand Horizontally (dur 30+1d20)"));
			power_desc[num].level = 20;
			power_desc[num].cost = 15;
			power_desc[num].stat = A_CHR;
			power_desc[num].fail = 8;
			power_desc[num++].number = -1;
			break;
		case RACE_ANDROID:
			if (p_ptr->lev < 10)
			{
				strcpy(power_desc[num].name, _("�쥤����", "Ray Gun"));
				power_desc[num].level = 1;
				power_desc[num].cost = 7;
				power_desc[num].fail = 8;
			}
			else if (p_ptr->lev < 25)
			{
				strcpy(power_desc[num].name, _("�֥饹����", "Blaster"));
				power_desc[num].level = 10;
				power_desc[num].cost = 13;
				power_desc[num].fail = 10;
			}
			else if (p_ptr->lev < 35)
			{
				strcpy(power_desc[num].name, _("�Х�����", "Bazooka"));
				power_desc[num].level = 25;
				power_desc[num].cost = 26;
				power_desc[num].fail = 12;
			}
			else if (p_ptr->lev < 45)
			{
				strcpy(power_desc[num].name, _("�ӡ��७��Υ�", "Beam Cannon"));
				power_desc[num].level = 35;
				power_desc[num].cost = 40;
				power_desc[num].fail = 15;
			}
			else
			{
				strcpy(power_desc[num].name, _("���å�", "Rocket"));
				power_desc[num].level = 45;
				power_desc[num].cost = 60;
				power_desc[num].fail = 18;
			}
			power_desc[num].stat = A_STR;
			power_desc[num++].number = -1;
			break;
		default:
		{
			break;
		}
	}
	}

	if (p_ptr->muta1)
	{
		if (p_ptr->muta1 & MUT1_SPIT_ACID)
		{
			strcpy(power_desc[num].name, _("������", "Spit Acid"));
			power_desc[num].level = 9;
			power_desc[num].cost = 9;
			power_desc[num].stat = A_DEX;
			power_desc[num].fail = 15;
			power_desc[num++].number = MUT1_SPIT_ACID;
		}

		if (p_ptr->muta1 & MUT1_BR_FIRE)
		{
			strcpy(power_desc[num].name, _("��Υ֥쥹", "Fire Breath"));
			power_desc[num].level = 20;
			power_desc[num].cost = lvl;
			power_desc[num].stat = A_CON;
			power_desc[num].fail = 18;
			power_desc[num++].number = MUT1_BR_FIRE;
		}

		if (p_ptr->muta1 & MUT1_HYPN_GAZE)
		{
			strcpy(power_desc[num].name, _("��̲�ˤ�", "Hypnotic Gaze"));
			power_desc[num].level = 12;
			power_desc[num].cost = 12;
			power_desc[num].stat = A_CHR;
			power_desc[num].fail = 18;
			power_desc[num++].number = MUT1_HYPN_GAZE;
		}

		if (p_ptr->muta1 & MUT1_TELEKINES)
		{
			strcpy(power_desc[num].name, _("ǰư��", "Telekinesis"));
			power_desc[num].level = 9;
			power_desc[num].cost = 9;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 14;
			power_desc[num++].number = MUT1_TELEKINES;
		}

		if (p_ptr->muta1 & MUT1_VTELEPORT)
		{
			strcpy(power_desc[num].name, _("�ƥ�ݡ���", "Teleport"));
			power_desc[num].level = 7;
			power_desc[num].cost = 7;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 15;
			power_desc[num++].number = MUT1_VTELEPORT;
		}

		if (p_ptr->muta1 & MUT1_MIND_BLST)
		{
			strcpy(power_desc[num].name, _("��������", "Mind Blast"));
			power_desc[num].level = 5;
			power_desc[num].cost = 3;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 15;
			power_desc[num++].number = MUT1_MIND_BLST;
		}

		if (p_ptr->muta1 & MUT1_RADIATION)
		{
			strcpy(power_desc[num].name, _("����ǽ", "Emit Radiation"));
			power_desc[num].level = 15;
			power_desc[num].cost = 15;
			power_desc[num].stat = A_CON;
			power_desc[num].fail = 14;
			power_desc[num++].number = MUT1_RADIATION;
		}

		if (p_ptr->muta1 & MUT1_VAMPIRISM)
		{
			strcpy(power_desc[num].name, _("�۷�", "Vampiric Drain"));
			power_desc[num].level = 2;
			power_desc[num].cost = (1 + (lvl / 3));
			power_desc[num].stat = A_CON;
			power_desc[num].fail = 9;
			power_desc[num++].number = MUT1_VAMPIRISM;
		}

		if (p_ptr->muta1 & MUT1_SMELL_MET)
		{
			strcpy(power_desc[num].name, _("��°�̳�", "Smell Metal"));
			power_desc[num].level = 3;
			power_desc[num].cost = 2;
			power_desc[num].stat = A_INT;
			power_desc[num].fail = 12;
			power_desc[num++].number = MUT1_SMELL_MET;
		}

		if (p_ptr->muta1 & MUT1_SMELL_MON)
		{
			strcpy(power_desc[num].name, _("Ũ���̳�", "Smell Monsters"));
			power_desc[num].level = 5;
			power_desc[num].cost = 4;
			power_desc[num].stat = A_INT;
			power_desc[num].fail = 15;
			power_desc[num++].number = MUT1_SMELL_MON;
		}

		if (p_ptr->muta1 & MUT1_BLINK)
		{
			strcpy(power_desc[num].name, _("���硼�ȡ��ƥ�ݡ���", "Blink"));
			power_desc[num].level = 3;
			power_desc[num].cost = 3;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 12;
			power_desc[num++].number = MUT1_BLINK;
		}

		if (p_ptr->muta1 & MUT1_EAT_ROCK)
		{
			strcpy(power_desc[num].name, _("�俩��", "Eat Rock"));
			power_desc[num].level = 8;
			power_desc[num].cost = 12;
			power_desc[num].stat = A_CON;
			power_desc[num].fail = 18;
			power_desc[num++].number = MUT1_EAT_ROCK;
		}

		if (p_ptr->muta1 & MUT1_SWAP_POS)
		{
			strcpy(power_desc[num].name, _("���ָ�", "Swap Position"));
			power_desc[num].level = 15;
			power_desc[num].cost = 12;
			power_desc[num].stat = A_DEX;
			power_desc[num].fail = 16;
			power_desc[num++].number = MUT1_SWAP_POS;
		}

		if (p_ptr->muta1 & MUT1_SHRIEK)
		{
			strcpy(power_desc[num].name, _("����", "Shriek"));
			power_desc[num].level = 20;
			power_desc[num].cost = 14;
			power_desc[num].stat = A_CON;
			power_desc[num].fail = 16;
			power_desc[num++].number = MUT1_SHRIEK;
		}

		if (p_ptr->muta1 & MUT1_ILLUMINE)
		{
			strcpy(power_desc[num].name, _("����", "Illuminate"));
			power_desc[num].level = 3;
			power_desc[num].cost = 2;
			power_desc[num].stat = A_INT;
			power_desc[num].fail = 10;
			power_desc[num++].number = MUT1_ILLUMINE;
		}

		if (p_ptr->muta1 & MUT1_DET_CURSE)
		{
			strcpy(power_desc[num].name, _("��������", "Detect Curses"));
			power_desc[num].level = 7;
			power_desc[num].cost = 14;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 14;
			power_desc[num++].number = MUT1_DET_CURSE;
		}

		if (p_ptr->muta1 & MUT1_BERSERK)
		{
			strcpy(power_desc[num].name, _("����β�", "Berserk"));
			power_desc[num].level = 8;
			power_desc[num].cost = 8;
			power_desc[num].stat = A_STR;
			power_desc[num].fail = 14;
			power_desc[num++].number = MUT1_BERSERK;
		}

		if (p_ptr->muta1 & MUT1_POLYMORPH)
		{
			strcpy(power_desc[num].name, _("�ѿ�", "Polymorph"));
			power_desc[num].level = 18;
			power_desc[num].cost = 20;
			power_desc[num].stat = A_CON;
			power_desc[num].fail = 18;
			power_desc[num++].number = MUT1_POLYMORPH;
		}

		if (p_ptr->muta1 & MUT1_MIDAS_TCH)
		{
			strcpy(power_desc[num].name, _("�ߥ����μ�", "Midas Touch"));
			power_desc[num].level = 10;
			power_desc[num].cost = 5;
			power_desc[num].stat = A_INT;
			power_desc[num].fail = 12;
			power_desc[num++].number = MUT1_MIDAS_TCH;
		}

		if (p_ptr->muta1 & MUT1_GROW_MOLD)
		{
			strcpy(power_desc[num].name, _("����ȯ��", "Grow Mold"));
			power_desc[num].level = 1;
			power_desc[num].cost = 6;
			power_desc[num].stat = A_CON;
			power_desc[num].fail = 14;
			power_desc[num++].number = MUT1_GROW_MOLD;
		}

		if (p_ptr->muta1 & MUT1_RESIST)
		{
			strcpy(power_desc[num].name, _("�����������", "Resist Elements"));
			power_desc[num].level = 10;
			power_desc[num].cost = 12;
			power_desc[num].stat = A_CON;
			power_desc[num].fail = 12;
			power_desc[num++].number = MUT1_RESIST;
		}

		if (p_ptr->muta1 & MUT1_EARTHQUAKE)
		{
			strcpy(power_desc[num].name, _("�Ͽ�", "Earthquake"));
			power_desc[num].level = 12;
			power_desc[num].cost = 12;
			power_desc[num].stat = A_STR;
			power_desc[num].fail = 16;
			power_desc[num++].number = MUT1_EARTHQUAKE;
		}

		if (p_ptr->muta1 & MUT1_EAT_MAGIC)
		{
			strcpy(power_desc[num].name, _("���Ͽ���", "Eat Magic"));
			power_desc[num].level = 17;
			power_desc[num].cost = 1;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 15;
			power_desc[num++].number = MUT1_EAT_MAGIC;
		}

		if (p_ptr->muta1 & MUT1_WEIGH_MAG)
		{
			strcpy(power_desc[num].name, _("���ϴ���", "Weigh Magic"));
			power_desc[num].level = 6;
			power_desc[num].cost = 6;
			power_desc[num].stat = A_INT;
			power_desc[num].fail = 10;
			power_desc[num++].number = MUT1_WEIGH_MAG;
		}

		if (p_ptr->muta1 & MUT1_STERILITY)
		{
			strcpy(power_desc[num].name, _("�����˻�", "Sterilize"));
			power_desc[num].level = 12;
			power_desc[num].cost = 23;
			power_desc[num].stat = A_CHR;
			power_desc[num].fail = 15;
			power_desc[num++].number = MUT1_STERILITY;
		}

		if (p_ptr->muta1 & MUT1_PANIC_HIT)
		{
			strcpy(power_desc[num].name, _("�ҥåȡ���������", "Panic Hit"));
			power_desc[num].level = 10;
			power_desc[num].cost = 12;
			power_desc[num].stat = A_DEX;
			power_desc[num].fail = 14;
			power_desc[num++].number = MUT1_PANIC_HIT;
		}

		if (p_ptr->muta1 & MUT1_DAZZLE)
		{
			strcpy(power_desc[num].name, _("����", "Dazzle"));
			power_desc[num].level = 7;
			power_desc[num].cost = 15;
			power_desc[num].stat = A_CHR;
			power_desc[num].fail = 8;
			power_desc[num++].number = MUT1_DAZZLE;
		}

		if (p_ptr->muta1 & MUT1_LASER_EYE)
		{
			strcpy(power_desc[num].name, _("�졼����������", "Laser Eye"));
			power_desc[num].level = 7;
			power_desc[num].cost = 10;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 9;
			power_desc[num++].number = MUT1_LASER_EYE;
		}

		if (p_ptr->muta1 & MUT1_RECALL)
		{
			strcpy(power_desc[num].name, _("����", "Recall"));
			power_desc[num].level = 17;
			power_desc[num].cost = 50;
			power_desc[num].stat = A_INT;
			power_desc[num].fail = 16;
			power_desc[num++].number = MUT1_RECALL;
		}

		if (p_ptr->muta1 & MUT1_BANISH)
		{
			strcpy(power_desc[num].name, _("�ٰ�����", "Banish Evil"));
			power_desc[num].level = 25;
			power_desc[num].cost = 25;
			power_desc[num].stat = A_WIS;
			power_desc[num].fail = 18;
			power_desc[num++].number = MUT1_BANISH;
		}

		if (p_ptr->muta1 & MUT1_COLD_TOUCH)
		{
			strcpy(power_desc[num].name, _("���μ�", "Cold Touch"));
			power_desc[num].level = 2;
			power_desc[num].cost = 2;
			power_desc[num].stat = A_CON;
			power_desc[num].fail = 11;
			power_desc[num++].number = MUT1_COLD_TOUCH;
		}

		if (p_ptr->muta1 & MUT1_LAUNCHER)
		{
			strcpy(power_desc[num].name, _("�����ƥ��ꤲ", "Throw Object"));
			power_desc[num].level = 1;
			power_desc[num].cost = lvl;
			power_desc[num].stat = A_STR;
			power_desc[num].fail = 6;
			/* XXX_XXX_XXX Hack! MUT1_LAUNCHER counts as negative... */
			power_desc[num++].number = 3;
		}
	}

	/* Nothing chosen yet */
	flag = FALSE;

	/* No redraw yet */
	redraw = FALSE;

	/* Build a prompt */
	(void) strnfmt(out_val, 78, 
				_("(�ü�ǽ�� %c-%c, *'�ǰ���, ESC������) �ɤ��ü�ǽ�Ϥ�Ȥ��ޤ�����", "(Powers %c-%c, *=List, ESC=exit) Use which power? "),
				I2A(0), (num <= 26) ? I2A(num - 1) : '0' + num - 27);

#ifdef ALLOW_REPEAT
if (!repeat_pull(&i) || i<0 || i>=num) {
#endif /* ALLOW_REPEAT */
	if (use_menu) screen_save();
	 /* Get a spell from the user */

	choice = (always_show_list || use_menu) ? ESCAPE:1;
	while (!flag)
	{
		if( choice==ESCAPE ) choice = ' '; 
		else if( !get_com(out_val, &choice, FALSE) )break; 

		if (use_menu && choice != ' ')
		{
			switch(choice)
			{
				case '0':
				{
					screen_load();
					p_ptr->energy_use = 0;
					return;
				}

				case '8':
				case 'k':
				case 'K':
				{
					menu_line += (num - 1);
					break;
				}

				case '2':
				case 'j':
				case 'J':
				{
					menu_line++;
					break;
				}

				case '6':
				case 'l':
				case 'L':
				case '4':
				case 'h':
				case 'H':
				{
					if (menu_line > 18)
						menu_line -= 18;
					else if (menu_line+18 <= num)
						menu_line += 18;
					break;
				}

				case 'x':
				case 'X':
				case '\r':
				{
					i = menu_line - 1;
					ask = FALSE;
					break;
				}
			}
			if (menu_line > num) menu_line -= num;
		}
		/* Request redraw */
		if ((choice == ' ') || (choice == '*') || (choice == '?') || (use_menu && ask))
		{
			/* Show the list */
			if (!redraw || use_menu)
			{
				byte y = 1, x = 0;
				int ctr = 0;
				char dummy[80];
				char letter;
				int x1, y1;

				strcpy(dummy, "");

				/* Show list */
				redraw = TRUE;

				/* Save the screen */
				if (!use_menu) screen_save();

				/* Print header(s) */
				if (num < 18)
					prt(_("                            Lv   MP ��Ψ", "                            Lv Cost Fail"), y++, x);
				else
				prt(_("                            Lv   MP ��Ψ                            Lv   MP ��Ψ", 
					  "                            Lv Cost Fail                            Lv Cost Fail"), y++, x);


				/* Print list */
				while (ctr < num)
				{
					x1 = ((ctr < 18) ? x : x + 40);
					y1 = ((ctr < 18) ? y + ctr : y + ctr - 18);

					if (use_menu)
					{
						if (ctr == (menu_line-1)) strcpy(dummy, _(" �� ", " >  "));
						else strcpy(dummy, "    ");
					}
					else
					{
						/* letter/number for power selection */
						if (ctr < 26)
							letter = I2A(ctr);
						else
							letter = '0' + ctr - 26;
						sprintf(dummy, " %c) ",letter);
					}
					strcat(dummy, format("%-23.23s %2d %4d %3d%%",
						power_desc[ctr].name, power_desc[ctr].level, power_desc[ctr].cost,
						100 - racial_chance(&power_desc[ctr])));
					prt(dummy, y1, x1);
					ctr++;
				}
			}

			/* Hide the list */
			else
			{
				/* Hide list */
				redraw = FALSE;

				/* Restore the screen */
				screen_load();
			}

			/* Redo asking */
			continue;
		}

		if (!use_menu)
		{
			if (choice == '\r' && num == 1)
			{
				choice = 'a';
			}

			if (isalpha(choice))
			{
				/* Note verify */
				ask = (isupper(choice));

				/* Lowercase */
				if (ask) choice = (char)tolower(choice);

				/* Extract request */
				i = (islower(choice) ? A2I(choice) : -1);
			}
			else
			{
				ask = FALSE; /* Can't uppercase digits */

				i = choice - '0' + 26;
			}
		}

		/* Totally Illegal */
		if ((i < 0) || (i >= num))
		{
			bell();
			continue;
		}

		/* Verify it */
		if (ask)
		{
			char tmp_val[160];

			/* Prompt */
			(void) strnfmt(tmp_val, 78, _("%s��Ȥ��ޤ����� ", "Use %s? "), power_desc[i].name);

			/* Belay that order */
			if (!get_check(tmp_val)) continue;
		}

		/* Stop the loop */
		flag = TRUE;
	}

	/* Restore the screen */
	if (redraw) screen_load();

	/* Abort if needed */
	if (!flag)
	{
		p_ptr->energy_use = 0;
		return;
	}
#ifdef ALLOW_REPEAT
	repeat_push(i);
	} /*if (!repeat_pull(&i) || ...)*/
#endif /* ALLOW_REPEAT */
	switch (racial_aux(&power_desc[i]))
	{
	case 1:
		if (power_desc[i].number < 0)
			cast = cmd_racial_power_aux(power_desc[i].number);
		else
			cast = mutation_power_aux(power_desc[i].number);
		break;
	case 0:
		cast = FALSE;
		break;
	case -1:
		cast = TRUE;
		break;
	}

	if (cast)
	{
		if (racial_cost)
		{
			int actual_racial_cost = racial_cost / 2 + randint1(racial_cost / 2);

			/* If mana is not enough, player consumes hit point! */
			if (p_ptr->csp < actual_racial_cost)
			{
				actual_racial_cost -= p_ptr->csp;
				p_ptr->csp = 0;
				take_hit(DAMAGE_USELIFE, actual_racial_cost, _("���٤ν���", "concentrating too hard"), -1);
			}
			else p_ptr->csp -= actual_racial_cost;

			/* Redraw mana and hp */
			p_ptr->redraw |= (PR_HP | PR_MANA);

			/* Window stuff */
			p_ptr->window |= (PW_PLAYER | PW_SPELL);
		}
	}
	else p_ptr->energy_use = 0;

	/* Success */
	return;
}
