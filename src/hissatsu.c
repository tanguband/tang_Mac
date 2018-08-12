/*!
 * @file hissatsu.c
 * @brief ���Ѥμ��� / Blade arts
 * @date 2014/01/17
 * @author
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke\n
 * This software may be copied and distributed for educational, research,\n
 * and not for profit purposes provided that this copyright and statement\n
 * are included in all such copies.  Other copyrights may also apply.\n
 * 2014 Deskull rearranged comment for Doxygen.\n
 */

#include "angband.h"

#define TECHNIC_HISSATSU (REALM_HISSATSU - MIN_TECHNIC)


/*!
 * @brief ���Ѳ�ǽ�ʷ��Ѥ����򤹤� /
 * Allow user to choose a blade arts.
 * @param sn ���򤷤��ü쵻ǽID������󥻥�ξ��-1������������ξ��-2���֤�
 * @return ȯư��ǽ����ˡ�����򤷤����TRUE������󥻥���������������򤬹Ԥ�줿���FALSE���֤���
 * @details
 * If a valid spell is chosen, saves it in '*sn' and returns TRUE\n
 * If the user hits escape, returns FALSE, and set '*sn' to -1\n
 * If there are no legal choices, returns FALSE, and sets '*sn' to -2\n
 *\n
 * The "prompt" should be "cast", "recite", or "study"\n
 * The "known" should be TRUE for cast/pray, FALSE for study\n
 *\n
 * nb: This function has a (trivial) display bug which will be obvious\n
 * when you run it. It's probably easy to fix but I haven't tried,\n
 * sorry.\n
 */
static int get_hissatsu_power(SPELL_IDX *sn)
{
	SPELL_IDX i;
	int j = 0;
	int             num = 0;
	int             y = 1;
	int             x = 15;
	int             plev = p_ptr->lev;
	int             ask = TRUE;
	char            choice;
	char            out_val[160];
	SPELL_IDX sentaku[32];
	cptr            p = _("ɬ����", "special attack");
	COMMAND_CODE code;
	magic_type spell;
	bool            flag, redraw;
	int menu_line = (use_menu ? 1 : 0);

	/* Assume cancelled */
	*sn = (-1);

#ifdef ALLOW_REPEAT /* TNB */

	/* Get the spell, if available */
	if (repeat_pull(&code))
	{
		*sn = (SPELL_IDX)code;
		/* Verify the spell */
		if (technic_info[TECHNIC_HISSATSU][*sn].slevel <= plev)
		{
			/* Success */
			return (TRUE);
		}
	}

#endif /* ALLOW_REPEAT -- TNB */

	/* Nothing chosen yet */
	flag = FALSE;

	/* No redraw yet */
	redraw = FALSE;

	for (i = 0; i < 32; i++)
	{
		if (technic_info[TECHNIC_HISSATSU][i].slevel <= PY_MAX_LEVEL)
		{
			sentaku[num] = i;
			num++;
		}
	}

	/* Build a prompt (accept all spells) */
	(void) strnfmt(out_val, 78, 
		       _("(%^s %c-%c, '*'�ǰ���, ESC) �ɤ�%s��Ȥ��ޤ�����", "(%^ss %c-%c, *=List, ESC=exit) Use which %s? "),
		       p, I2A(0), "abcdefghijklmnopqrstuvwxyz012345"[num-1], p);

	if (use_menu) screen_save();

	/* Get a spell from the user */

	choice= always_show_list ? ESCAPE:1 ;
	while (!flag)
	{
		if(choice==ESCAPE) choice = ' '; 
		else if( !get_com(out_val, &choice, FALSE) )break;

		if (use_menu && choice != ' ')
		{
			switch(choice)
			{
				case '0':
				{
					screen_load();
					return (FALSE);
				}

				case '8':
				case 'k':
				case 'K':
				{
					do
					{
						menu_line += 31;
						if (menu_line > 32) menu_line -= 32;
					} while(!(p_ptr->spell_learned1 & (1L << (menu_line-1))));
					break;
				}

				case '2':
				case 'j':
				case 'J':
				{
					do
					{
						menu_line++;
						if (menu_line > 32) menu_line -= 32;
					} while(!(p_ptr->spell_learned1 & (1L << (menu_line-1))));
					break;
				}

				case '4':
				case 'h':
				case 'H':
				case '6':
				case 'l':
				case 'L':
				{
					bool reverse = FALSE;
					if ((choice == '4') || (choice == 'h') || (choice == 'H')) reverse = TRUE;
					if (menu_line > 16)
					{
						menu_line -= 16;
						reverse = TRUE;
					}
					else menu_line+=16;
					while(!(p_ptr->spell_learned1 & (1L << (menu_line-1))))
					{
						if (reverse)
						{
							menu_line--;
							if (menu_line < 2) reverse = FALSE;
						}
						else
						{
							menu_line++;
							if (menu_line > 31) reverse = TRUE;
						}
					}
					break;
				}

				case 'x':
				case 'X':
				case '\r':
				case '\n':
				{
					i = menu_line - 1;
					ask = FALSE;
					break;
				}
			}
		}
		/* Request redraw */
		if ((choice == ' ') || (choice == '*') || (choice == '?') || (use_menu && ask))
		{
			/* Show the list */
			if (!redraw || use_menu)
			{
				char psi_desc[80];
				int line;

				/* Show list */
				redraw = TRUE;

				/* Save the screen */
				if (!use_menu) screen_save();

				/* Display a list of spells */
				prt("", y, x);
				put_str(_("̾��              Lv  MP      ̾��              Lv  MP ", 
						  "name              Lv  SP      name              Lv  SP "), y, x + 5);
				prt("", y+1, x);
				/* Dump the spells */
				for (i = 0, line = 0; i < 32; i++)
				{
					spell = technic_info[TECHNIC_HISSATSU][i];

					if (spell.slevel > PY_MAX_LEVEL) continue;
					line++;
					if (!(p_ptr->spell_learned1 >> i)) break;

					/* Access the spell */
					if (spell.slevel > plev)   continue;
					if (!(p_ptr->spell_learned1 & (1L << i))) continue;
					if (use_menu)
					{
						if (i == (menu_line-1))
							strcpy(psi_desc, _("  ��", "  > "));
						else strcpy(psi_desc, "    ");
						
					}
					else
					{
						char letter;
						if (line <= 26)
							letter = I2A(line-1);
						else
							letter = '0' + line - 27;
						sprintf(psi_desc, "  %c)",letter);
					}

					/* Dump the spell --(-- */
					strcat(psi_desc, format(" %-18s%2d %3d",
						do_spell(REALM_HISSATSU, i, SPELL_NAME),
						spell.slevel, spell.smana));
					prt(psi_desc, y + (line%17) + (line >= 17), x+(line/17)*30);
					prt("", y + (line%17) + (line >= 17) + 1, x+(line/17)*30);
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
		if ((i < 0) || (i >= 32) || !(p_ptr->spell_learned1 & (1 << sentaku[i])))
		{
			bell();
			continue;
		}

		j = sentaku[i];

		/* Verify it */
		if (ask)
		{
			char tmp_val[160];

			/* Prompt */
			(void) strnfmt(tmp_val, 78, _("%s��Ȥ��ޤ�����", "Use %s? "), do_spell(REALM_HISSATSU, j, SPELL_NAME));

			/* Belay that order */
			if (!get_check(tmp_val)) continue;
		}

		/* Stop the loop */
		flag = TRUE;
	}

	/* Restore the screen */
	if (redraw) screen_load();

	/* Show choices */
	p_ptr->window |= (PW_SPELL);

	/* Window stuff */
	window_stuff();


	/* Abort if needed */
	if (!flag) return (FALSE);

	/* Save the choice */
	(*sn) = j;

#ifdef ALLOW_REPEAT /* TNB */

	repeat_push((COMMAND_CODE)j);

#endif /* ALLOW_REPEAT -- TNB */

	/* Success */
	return (TRUE);
}


/*!
 * @brief ���ѥ��ޥ�ɤΥᥤ��롼����
 * @return �ʤ�
 */
void do_cmd_hissatsu(void)
{
	SPELL_IDX       n = 0;
	magic_type      spell;


	/* not if confused */
	if (p_ptr->confused)
	{
		msg_print(_("���𤷤Ƥ��ƽ���Ǥ��ʤ���", "You are too confused!"));
		return;
	}
	if (!buki_motteruka(INVEN_RARM) && !buki_motteruka(INVEN_LARM))
	{
		if (flush_failure) flush();
		msg_print(_("��������ʤ���ɬ�����ϻȤ��ʤ���", "You need to wield a weapon!"));
		return;
	}
	if (!p_ptr->spell_learned1)
	{
		msg_print(_("���⵻���Τ�ʤ���", "You don't know any special attacks."));
		return;
	}

	if (p_ptr->special_defense & KATA_MASK)
	{
		set_action(ACTION_NONE);
	}

	/* get power */
	if (!get_hissatsu_power(&n)) return;

	spell = technic_info[TECHNIC_HISSATSU][n];

	/* Verify "dangerous" spells */
	if (spell.smana > p_ptr->csp)
	{
		if (flush_failure) flush();
		/* Warning */
		msg_print(_("�ͣФ�­��ޤ���", "You do not have enough mana to use this power."));
		msg_print(NULL);
		return;
	}

	sound(SOUND_ZAP);

	/* Cast the spell */
	if (!do_spell(REALM_HISSATSU, n, SPELL_CAST)) return;

	/* Take a turn */
	p_ptr->energy_use = 100;

	/* Use some mana */
	p_ptr->csp -= spell.smana;

	/* Limit */
	if (p_ptr->csp < 0) p_ptr->csp = 0;

	/* Redraw mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
	p_ptr->window |= (PW_SPELL);
}


/*!
 * @brief ���ѥ��ޥ�ɤγؽ�
 * @return �ʤ�
 */
void do_cmd_gain_hissatsu(void)
{
	OBJECT_IDX item;
	int i, j;

	object_type *o_ptr;
	cptr q, s;

	bool gain = FALSE;

	if (p_ptr->special_defense & (KATA_MUSOU | KATA_KOUKIJIN))
	{
		set_action(ACTION_NONE);
	}

	if (p_ptr->blind || no_lite())
	{
		msg_print(_("�ܤ������ʤ���", "You cannot see!"));
		return;
	}

	if (p_ptr->confused)
	{
		msg_print(_("���𤷤Ƥ����ɤ�ʤ���", "You are too confused!"));
		return;
	}

	if (!(p_ptr->new_spells))
	{
		msg_print(_("������ɬ������Ф��뤳�ȤϤǤ��ʤ���", "You cannot learn any new special attacks!"));
		return;
	}

#ifdef JP
	if( p_ptr->new_spells < 10 ){
		msg_format("���� %d �Ĥ�ɬ������ؤ٤롣", p_ptr->new_spells);
	}else{
		msg_format("���� %d �Ĥ�ɬ������ؤ٤롣", p_ptr->new_spells);
	}
#else
	msg_format("You can learn %d new special attack%s.", p_ptr->new_spells,
		(p_ptr->new_spells == 1?"":"s"));
#endif

	item_tester_tval = TV_HISSATSU_BOOK;

	/* Get an item */
	q = _("�ɤν񤫤�ؤӤޤ���? ", "Study which book? ");
	s = _("�ɤ��񤬤ʤ���", "You have no books that you can read.");

	if (!get_item(&item, q, s, (USE_INVEN | USE_FLOOR))) return;

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

	for (i = o_ptr->sval * 8; i < o_ptr->sval * 8 + 8; i++)
	{
		if (p_ptr->spell_learned1 & (1L << i)) continue;
		if (technic_info[TECHNIC_HISSATSU][i].slevel > p_ptr->lev) continue;

		p_ptr->spell_learned1 |= (1L << i);
		p_ptr->spell_worked1 |= (1L << i);
		msg_format(_("%s�ε���Ф�����", "You have learned the special attack of %s."), do_spell(REALM_HISSATSU, i, SPELL_NAME));
		for (j = 0; j < 64; j++)
		{
			/* Stop at the first empty space */
			if (p_ptr->spell_order[j] == 99) break;
		}
		p_ptr->spell_order[j] = i;
		gain = TRUE;
	}

	/* No gain ... */
	if (!gain)
		msg_print(_("����Ф����ʤ��ä���", "You were not able to learn any special attacks."));

	/* Take a turn */
	else
		p_ptr->energy_use = 100;

	p_ptr->update |= (PU_SPELLS);
}


/*!
 * @brief ���ѤΥ��쥤��Ψ�׻���Ԥ� /
 * Calcurate magnification of hissatsu technics
 * @param mult ���ѤΥ��쥤���̰����˻��Ф��Ƥ���¿���Ǥ���Ψ(/10��)
 * @param flgs ���Ѥ˻��Ѥ������Υ��쥤�ե饰����
 * @param m_ptr ��ɸ�Ȥʤ��󥹥����ι�¤�λ��ȥݥ���
 * @param mode ���ѤΥ��쥤��ID
 * @return ���쥤����Ψ(/10��)
 */
MULTIPLY mult_hissatsu(MULTIPLY mult, BIT_FLAGS *flgs, monster_type *m_ptr, BIT_FLAGS mode)
{
	monster_race *r_ptr = &r_info[m_ptr->r_idx];

	/* Burning Strike (Fire) */
	if (mode == HISSATSU_FIRE)
	{
		/* Notice immunity */
		if (r_ptr->flagsr & RFR_EFF_IM_FIRE_MASK)
		{
			if (is_original_ap_and_seen(m_ptr))
			{
				r_ptr->r_flagsr |= (r_ptr->flagsr & RFR_EFF_IM_FIRE_MASK);
			}
		}

		/* Otherwise, take the damage */
		else if (have_flag(flgs, TR_BRAND_FIRE))
		{
			if (r_ptr->flags3 & RF3_HURT_FIRE)
			{
				if (mult < 70) mult = 70;
				if (is_original_ap_and_seen(m_ptr))
				{
					r_ptr->r_flags3 |= RF3_HURT_FIRE;
				}
			}
			else if (mult < 35) mult = 35;
		}
		else
		{
			if (r_ptr->flags3 & RF3_HURT_FIRE)
			{
				if (mult < 50) mult = 50;
				if (is_original_ap_and_seen(m_ptr))
				{
					r_ptr->r_flags3 |= RF3_HURT_FIRE;
				}
			}
			else if (mult < 25) mult = 25;
		}
	}

	/* Serpent's Tongue (Poison) */
	if (mode == HISSATSU_POISON)
	{
		/* Notice immunity */
		if (r_ptr->flagsr & RFR_EFF_IM_POIS_MASK)
		{
			if (is_original_ap_and_seen(m_ptr))
			{
				r_ptr->r_flagsr |= (r_ptr->flagsr & RFR_EFF_IM_POIS_MASK);
			}
		}

		/* Otherwise, take the damage */
		else if (have_flag(flgs, TR_BRAND_POIS))
		{
			if (mult < 35) mult = 35;
		}
		else
		{
			if (mult < 25) mult = 25;
		}
	}

	/* Zammaken (Nonliving Evil) */
	if (mode == HISSATSU_ZANMA)
	{
		if (!monster_living(r_ptr) && (r_ptr->flags3 & RF3_EVIL))
		{
			if (mult < 15) mult = 25;
			else if (mult < 50) mult = MIN(50, mult+20);
		}
	}

	/* Rock Smash (Hurt Rock) */
	if (mode == HISSATSU_HAGAN)
	{
		if (r_ptr->flags3 & RF3_HURT_ROCK)
		{
			if (is_original_ap_and_seen(m_ptr))
			{
				r_ptr->r_flags3 |= RF3_HURT_ROCK;
			}
			if (mult == 10) mult = 40;
			else if (mult < 60) mult = 60;
		}
	}

	/* Midare-Setsugekka (Cold) */
	if (mode == HISSATSU_COLD)
	{
		/* Notice immunity */
		if (r_ptr->flagsr & RFR_EFF_IM_COLD_MASK)
		{
			if (is_original_ap_and_seen(m_ptr))
			{
				r_ptr->r_flagsr |= (r_ptr->flagsr & RFR_EFF_IM_COLD_MASK);
			}
		}
		/* Otherwise, take the damage */
		else if (have_flag(flgs, TR_BRAND_COLD))
		{
			if (r_ptr->flags3 & RF3_HURT_COLD)
			{
				if (mult < 70) mult = 70;
				if (is_original_ap_and_seen(m_ptr))
				{
					r_ptr->r_flags3 |= RF3_HURT_COLD;
				}
			}
			else if (mult < 35) mult = 35;
		}
		else
		{
			if (r_ptr->flags3 & RF3_HURT_COLD)
			{
				if (mult < 50) mult = 50;
				if (is_original_ap_and_seen(m_ptr))
				{
					r_ptr->r_flags3 |= RF3_HURT_COLD;
				}
			}
			else if (mult < 25) mult = 25;
		}
	}

	/* Lightning Eagle (Elec) */
	if (mode == HISSATSU_ELEC)
	{
		/* Notice immunity */
		if (r_ptr->flagsr & RFR_EFF_IM_ELEC_MASK)
		{
			if (is_original_ap_and_seen(m_ptr))
			{
				r_ptr->r_flagsr |= (r_ptr->flagsr & RFR_EFF_IM_ELEC_MASK);
			}
		}

		/* Otherwise, take the damage */
		else if (have_flag(flgs, TR_BRAND_ELEC))
		{
			if (mult < 70) mult = 70;
		}
		else
		{
			if (mult < 50) mult = 50;
		}
	}

	/* Bloody Maelstrom */
	if ((mode == HISSATSU_SEKIRYUKA) && p_ptr->cut && monster_living(r_ptr))
	{
		MULTIPLY tmp = MIN(100, MAX(10, p_ptr->cut / 10));
		if (mult < tmp) mult = tmp;
	}

	/* Keiun-Kininken */
	if (mode == HISSATSU_UNDEAD)
	{
		if (r_ptr->flags3 & RF3_UNDEAD)
		{
			if (is_original_ap_and_seen(m_ptr))
			{
				r_ptr->r_flags3 |= RF3_UNDEAD;
			}
			if (mult == 10) mult = 70;
			else if (mult < 140) mult = MIN(140, mult+60);
		}
		if (mult == 10) mult = 40;
		else if (mult < 60) mult = MIN(60, mult+30);
	}

	if (mult > 150) mult = 150;

	return mult;
}
