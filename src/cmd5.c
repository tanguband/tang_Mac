/*!
 *  @file cmd5.c
 *  @brief �ץ쥤�䡼����ˡ�˴ؤ��륳�ޥ�ɤμ��� / Spell/Prayer commands
 *  @date 2014/01/02
 *  @author
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.  Other copyrights may also apply.
 */


#include "angband.h"

/*!
 * @brief �ΰ���ˡ�˱����Ƶ�ǽ��̾�Τ��֤���
 * @param tval ��ˡ���tval
 * @return �ΰ���ˡ�ε�ǽ̾�Τ��ݴɤ���ʸ����ݥ���
 */
cptr spell_category_name(OBJECT_TYPE_VALUE tval)
{
	switch (tval)
	{
#ifdef JP
	case TV_HISSATSU_BOOK:
		return "ɬ����";
	case TV_LIFE_BOOK:
		return "����";
	case TV_MUSIC_BOOK:
		return "��";
	default:
		return "��ʸ";
#else
	case TV_HISSATSU_BOOK:
		return "art";
	case TV_LIFE_BOOK:
		return "prayer";
	case TV_MUSIC_BOOK:
		return "song";
	default:
		return "spell";
#endif
	}
}


bool select_the_force = FALSE;

/*!
 * @brief �ΰ���ˡ�α������ؽ����������򤹤륤�󥿡��ե���������
 * Allow user to choose a spell/prayer from the given book.
 * @param sn ���򤷤���ˡID���֤����ȥݥ���
 * @param prompt ��ˡ�����Ѥ���ݤ�ư��ɽ��
 * @param sval ��ƻ���sval
 * @param learned ����/��������ʤ��TRUE���ؽ������ʤ�FALSE
 * @param use_realm ��ˡ�ΰ�ID
 * @return
 * <pre>
 * If a valid spell is chosen, saves it in '*sn' and returns TRUE
 * If the user hits escape, returns FALSE, and set '*sn' to -1
 * If there are no legal choices, returns FALSE, and sets '*sn' to -2
 * The "prompt" should be "cast", "recite", or "study"
 * The "known" should be TRUE for cast/pray, FALSE for study
 * </pre>
 */
static int get_spell(SPELL_IDX *sn, cptr prompt, OBJECT_SUBTYPE_VALUE sval, bool learned, REALM_IDX use_realm)
{
	int         i;
	SPELL_IDX   spell = -1;
	int         num = 0;
	int         ask = TRUE;
	MANA_POINT  need_mana;
	SPELL_IDX   spells[64];
	bool        flag, redraw, okay;
	char        choice;
	const magic_type  *s_ptr;
	char        out_val[160];
	cptr        p;
	COMMAND_CODE code;
#ifdef JP
	char jverb_buf[128];
#endif
	int menu_line = (use_menu ? 1 : 0);

#ifdef ALLOW_REPEAT /* TNB */

	/* Get the spell, if available */
	if (repeat_pull(&code))
	{
		*sn = (SPELL_IDX)code;
		/* Verify the spell */
		if (spell_okay(*sn, learned, FALSE, use_realm))
		{
			/* Success */
			return (TRUE);
		}
	}

#endif /* ALLOW_REPEAT -- TNB */

	p = spell_category_name(mp_ptr->spell_book);

	/* Extract spells */
	for (spell = 0; spell < 32; spell++)
	{
		/* Check for this spell */
		if ((fake_spell_flags[sval] & (1L << spell)))
		{
			/* Collect this spell */
			spells[num++] = spell;
		}
	}

	/* Assume no usable spells */
	okay = FALSE;

	/* Assume no spells available */
	(*sn) = -2;

	/* Check for "okay" spells */
	for (i = 0; i < num; i++)
	{
		/* Look for "okay" spells */
		if (spell_okay(spells[i], learned, FALSE, use_realm)) okay = TRUE;
	}

	/* No "okay" spells */
	if (!okay) return (FALSE);
	if (((use_realm) != p_ptr->realm1) && ((use_realm) != p_ptr->realm2) && (p_ptr->pclass != CLASS_SORCERER) && (p_ptr->pclass != CLASS_RED_MAGE)) return FALSE;
	if (((p_ptr->pclass == CLASS_SORCERER) || (p_ptr->pclass == CLASS_RED_MAGE)) && !is_magic(use_realm)) return FALSE;
	if ((p_ptr->pclass == CLASS_RED_MAGE) && ((use_realm) != REALM_ARCANE) && (sval > 1)) return FALSE;

	/* Assume cancelled */
	*sn = (-1);

	/* Nothing chosen yet */
	flag = FALSE;

	/* No redraw yet */
	redraw = FALSE;

	/* Show choices */
	p_ptr->window |= (PW_SPELL);

	/* Window stuff */
	window_stuff();

	/* Build a prompt (accept all spells) */
#ifdef JP
	jverb( prompt, jverb_buf, JVERB_AND );
	(void) strnfmt(out_val, 78, "(%^s:%c-%c, '*'�ǰ���, ESC������) �ɤ�%s��%^s�ޤ���? ",
		p, I2A(0), I2A(num - 1), p, jverb_buf );
#else
	(void)strnfmt(out_val, 78, "(%^ss %c-%c, *=List, ESC=exit) %^s which %s? ",
		p, I2A(0), I2A(num - 1), prompt, p);
#endif

	/* Get a spell from the user */

	choice = (always_show_list || use_menu) ? ESCAPE : 1;
	while (!flag)
	{
		if (choice == ESCAPE) choice = ' '; 
		else if (!get_com(out_val, &choice, TRUE))break;

		if (use_menu && choice != ' ')
		{
			switch (choice)
			{
				case '0':
				{
					screen_load();
					return FALSE;
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
			if (menu_line > num) menu_line -= num;
			/* Display a list of spells */
			print_spells(menu_line, spells, num, 1, 15, use_realm);
			if (ask) continue;
		}
		else
		{
			/* Request redraw */
			if ((choice == ' ') || (choice == '*') || (choice == '?'))
			{
				/* Show the list */
				if (!redraw)
				{
					/* Show list */
					redraw = TRUE;

					/* Save the screen */
					screen_save();

					/* Display a list of spells */
					print_spells(menu_line, spells, num, 1, 15, use_realm);
				}

				/* Hide the list */
				else
				{
					if (use_menu) continue;

					/* Hide list */
					redraw = FALSE;

					/* Restore the screen */
					screen_load();
				}

				/* Redo asking */
				continue;
			}


			/* Note verify */
			ask = (isupper(choice));

			/* Lowercase */
			if (ask) choice = (char)tolower(choice);

			/* Extract request */
			i = (islower(choice) ? A2I(choice) : -1);
		}

		/* Totally Illegal */
		if ((i < 0) || (i >= num))
		{
			bell();
			continue;
		}

		/* Save the spell index */
		spell = spells[i];

		/* Require "okay" spells */
		if (!spell_okay(spell, learned, FALSE, use_realm))
		{
			bell();
#ifdef JP
			msg_format("����%s��%s���ȤϤǤ��ޤ���", p, prompt);
#else
			msg_format("You may not %s that %s.", prompt, p);
#endif

			continue;
		}

		/* Verify it */
		if (ask)
		{
			char tmp_val[160];

			/* Access the spell */
			if (!is_magic(use_realm))
			{
				s_ptr = &technic_info[use_realm - MIN_TECHNIC][spell];
			}
			else
			{
				s_ptr = &mp_ptr->info[use_realm - 1][spell];
			}

			/* Extract mana consumption rate */
			if (use_realm == REALM_HISSATSU)
			{
				need_mana = s_ptr->smana;
			}
			else
			{
				need_mana = mod_need_mana(s_ptr->smana, spell, use_realm);
			}

			/* Prompt */
#ifdef JP
			jverb( prompt, jverb_buf, JVERB_AND );
			/* �����ڤ��ؤ���ǽ���б� */
			(void) strnfmt(tmp_val, 78, "%s(MP%d, ����Ψ%d%%)��%s�ޤ���? ",
				do_spell(use_realm, spell, SPELL_NAME), need_mana,
				       spell_chance(spell, use_realm),jverb_buf);
#else
			(void)strnfmt(tmp_val, 78, "%^s %s (%d mana, %d%% fail)? ",
				prompt, do_spell(use_realm, spell, SPELL_NAME), need_mana,
				spell_chance(spell, use_realm));
#endif


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
	if (!flag) return FALSE;

	/* Save the choice */
	(*sn) = spell;

#ifdef ALLOW_REPEAT /* TNB */

	repeat_push((COMMAND_CODE)spell);

#endif /* ALLOW_REPEAT -- TNB */

	/* Success */
	return TRUE;
}

/*!
 * @brief ���֥������Ȥ��ץ쥤�䡼�����Ѳ�ǽ����ƻ�񤫤ɤ�����Ƚ�ꤹ��
 * @param o_ptr Ƚ�ꤷ�������ֲ񥸥����Ȥι�¤�λ��ȥݥ���
 * @return �ؽ��Ǥ�����ƻ��ʤ��TRUE���֤�
 */
static bool item_tester_learn_spell(object_type *o_ptr)
{
	s32b choices = realm_choices2[p_ptr->pclass];

	if (p_ptr->pclass == CLASS_PRIEST)
	{
		if (is_good_realm(p_ptr->realm1))
		{
			choices &= ~(CH_DEATH | CH_DAEMON);
		}
		else
		{
			choices &= ~(CH_LIFE | CH_CRUSADE);
		}
	}

	if ((o_ptr->tval < TV_LIFE_BOOK) || (o_ptr->tval > (TV_LIFE_BOOK + MAX_REALM - 1))) return (FALSE);
	if ((o_ptr->tval == TV_MUSIC_BOOK) && (p_ptr->pclass == CLASS_BARD)) return (TRUE);
	else if (!is_magic(tval2realm(o_ptr->tval))) return FALSE;
	if ((REALM1_BOOK == o_ptr->tval) || (REALM2_BOOK == o_ptr->tval)) return (TRUE);
	if (choices & (0x0001 << (tval2realm(o_ptr->tval) - 1))) return (TRUE);
	return (FALSE);
}

/*!
 * @brief �ץ쥤�䡼����ƻ���������äƤ��ʤ�����Ƚ�ꤹ��
 * @return ��ƻ���������äƤ��ʤ��ʤ�TRUE���֤�
 */
static bool player_has_no_spellbooks(void)
{
	int         i;
	object_type *o_ptr;

	for (i = 0; i < INVEN_PACK; i++)
	{
		o_ptr = &inventory[i];
		if (o_ptr->k_idx && check_book_realm(o_ptr->tval, o_ptr->sval)) return FALSE;
	}

	for (i = cave[p_ptr->y][p_ptr->x].o_idx; i; i = o_ptr->next_o_idx)
	{
		o_ptr = &o_list[i];
		if (o_ptr->k_idx && (o_ptr->marked & OM_FOUND) && check_book_realm(o_ptr->tval, o_ptr->sval)) return FALSE;
	}

	return TRUE;
}

/*!
 * @brief �ץ쥤�䡼�ο��Ȥ������ѻդλ����ΰ���ˡ�������Ѥ��ڤ괹��������Υ��󥿡��ե�����
 * @param browse_only ��ˡ�ȵ�ǽ�α�����Ԥ��ʤ��TRUE
 * @return ��ƻ���������äƤ��ʤ��ʤ�TRUE���֤�
 */
static void confirm_use_force(bool browse_only)
{
	char which;
	COMMAND_CODE code;

#ifdef ALLOW_REPEAT

	/* Get the item index */
	if (repeat_pull(&code) && (code == INVEN_FORCE))
	{
		browse_only ? do_cmd_mind_browse() : do_cmd_mind();
		return;
	}

#endif /* ALLOW_REPEAT */

	/* Show the prompt */
	prt(_("('w'������, ESC) 'w'��ESC�򲡤��Ƥ��������� ", "(w for the Force, ESC) Hit 'w' or ESC. "), 0, 0);

	while (1)
	{
		/* Get a key */
		which = inkey();

		if (which == ESCAPE) break;
		else if (which == 'w')
		{

#ifdef ALLOW_REPEAT

			repeat_push(INVEN_FORCE);

#endif /* ALLOW_REPEAT */

			break;
		}
	}

	/* Clear the prompt line */
	prt("", 0, 0);

	if (which == 'w')
	{
		browse_only ? do_cmd_mind_browse() : do_cmd_mind();
	}
}


/*!
 * @brief �ץ쥤�䡼����ˡ�ȵ�ǽ��������륳�ޥ�ɤΥᥤ��롼���� /
 * Peruse the spells/prayers in a book
 * @return �ʤ�
 * @details
 * <pre>
 * Note that *all* spells in the book are listed
 *
 * Note that browsing is allowed while confused or blind,
 * and in the dark, primarily to allow browsing in stores.
 * </pre>
 */
void do_cmd_browse(void)
{
	OBJECT_IDX item;
	OBJECT_SUBTYPE_VALUE sval;
	REALM_IDX use_realm = 0;
	int j, line;
	SPELL_IDX spell = -1;
	int num = 0;

	SPELL_IDX spells[64];
	char temp[62*4];

	object_type	*o_ptr;

	cptr q, s;

	/* Warriors are illiterate */
	if (!(p_ptr->realm1 || p_ptr->realm2) && (p_ptr->pclass != CLASS_SORCERER) && (p_ptr->pclass != CLASS_RED_MAGE))
	{
		msg_print(_("�ܤ��ɤळ�Ȥ��Ǥ��ʤ���", "You cannot read books!"));
		return;
	}

	if (p_ptr->special_defense & KATA_MUSOU)
	{
		set_action(ACTION_NONE);
	}

	if (p_ptr->pclass == CLASS_FORCETRAINER)
	{
		if (player_has_no_spellbooks())
		{
			confirm_use_force(TRUE);
			return;
		}
		select_the_force = TRUE;
	}

	/* Restrict choices to "useful" books */
	if (p_ptr->realm2 == REALM_NONE) item_tester_tval = mp_ptr->spell_book;
	else item_tester_hook = item_tester_learn_spell;

	/* Get an item */
	q = _("�ɤ��ܤ��ɤߤޤ���? ", "Browse which book? ");
	s = _("�ɤ���ܤ��ʤ���", "You have no books that you can read.");

	if (!get_item(&item, q, s, (USE_INVEN | USE_FLOOR)))
	{
		select_the_force = FALSE;
		return;
	}
	select_the_force = FALSE;

	if (item == INVEN_FORCE) /* the_force */
	{
		do_cmd_mind_browse();
		return;
	}

	/* Get the item (in the pack) */
	else if (item >= 0)
	{
		o_ptr = &inventory[item];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - item];
	}

	/* Access the item's sval */
	sval = o_ptr->sval;

	use_realm = tval2realm(o_ptr->tval);

	/* Track the object kind */
	object_kind_track(o_ptr->k_idx);

	/* Hack -- Handle stuff */
	handle_stuff();


	/* Extract spells */
	for (spell = 0; spell < 32; spell++)
	{
		/* Check for this spell */
		if ((fake_spell_flags[sval] & (1L << spell)))
		{
			/* Collect this spell */
			spells[num++] = spell;
		}
	}


	/* Save the screen */
	screen_save();

	/* Clear the top line */
	prt("", 0, 0);

	/* Keep browsing spells.  Exit browsing on cancel. */
	while(TRUE)
	{
		/* Ask for a spell, allow cancel */
		if (!get_spell(&spell, _("�ɤ�", "browse"), o_ptr->sval, TRUE, use_realm))
		{
			/* If cancelled, leave immediately. */
			if (spell == -1) break;

			/* Display a list of spells */
			print_spells(0, spells, num, 1, 15, use_realm);

			/* Notify that there's nothing to see, and wait. */
			if (use_realm == REALM_HISSATSU)
				prt(_("�ɤ�뵻���ʤ���", "No techniques to browse."), 0, 0);
			else
				prt(_("�ɤ���ʸ���ʤ���", "No spells to browse."), 0, 0);
			(void)inkey();


			/* Restore the screen */
			screen_load();

			return;
		}

		/* Clear lines, position cursor  (really should use strlen here) */
		Term_erase(14, 14, 255);
		Term_erase(14, 13, 255);
		Term_erase(14, 12, 255);
		Term_erase(14, 11, 255);

		roff_to_buf(do_spell(use_realm, spell, SPELL_DESC), 62, temp, sizeof(temp));

		for (j = 0, line = 11; temp[j]; j += 1 + strlen(&temp[j]))
		{
			prt(&temp[j], line, 15);
			line++;
		}
	}

	/* Restore the screen */
	screen_load();
}

/*!
 * @brief �ץ쥤�䡼��������ˡ�ΰ���ѹ����� /
 * @param next_realm �ѹ������ˡ�ΰ�ID
 * @return �ʤ�
 */
static void change_realm2(CHARACTER_IDX next_realm)
{
	int i, j = 0;
	char tmp[80];

	for (i = 0; i < 64; i++)
	{
		p_ptr->spell_order[j] = p_ptr->spell_order[i];
		if (p_ptr->spell_order[i] < 32) j++;
	}
	for (; j < 64; j++)
		p_ptr->spell_order[j] = 99;

	for (i = 32; i < 64; i++)
	{
		p_ptr->spell_exp[i] = SPELL_EXP_UNSKILLED;
	}
	p_ptr->spell_learned2 = 0L;
	p_ptr->spell_worked2 = 0L;
	p_ptr->spell_forgotten2 = 0L;

	sprintf(tmp,_("��ˡ���ΰ��%s����%s���ѹ�������", "change magic realm from %s to %s."), realm_names[p_ptr->realm2], realm_names[next_realm]);
	do_cmd_write_nikki(NIKKI_BUNSHOU, 0, tmp);
	p_ptr->old_realm |= 1 << (p_ptr->realm2-1);
	p_ptr->realm2 = next_realm;

	p_ptr->notice |= (PN_REORDER);
	p_ptr->update |= (PU_SPELLS);
	handle_stuff();

	/* Load an autopick preference file */
	autopick_load_pref(FALSE);
}


/*!
 * @brief ��ˡ��ؽ����륳�ޥ�ɤΥᥤ��롼���� /
 * Study a book to gain a new spell/prayer
 * @return �ʤ�
 */
void do_cmd_study(void)
{
	int	i;
	OBJECT_IDX item;
	OBJECT_SUBTYPE_VALUE sval;
	int	increment = 0;
	bool    learned = FALSE;

	/* Spells of realm2 will have an increment of +32 */
	SPELL_IDX spell = -1;

	cptr p = spell_category_name(mp_ptr->spell_book);

	object_type *o_ptr;

	cptr q, s;

	if (!p_ptr->realm1)
	{
		msg_print(_("�ܤ��ɤळ�Ȥ��Ǥ��ʤ���", "You cannot read books!"));
		return;
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
		msg_format(_("������%s��Ф��뤳�ȤϤǤ��ʤ���", "You cannot learn any new %ss!"), p);
		return;
	}

	if (p_ptr->special_defense & KATA_MUSOU)
	{
		set_action(ACTION_NONE);
	}

#ifdef JP
	if( p_ptr->new_spells < 10 ){
		msg_format("���� %d �Ĥ�%s��ؤ٤롣", p_ptr->new_spells, p);
	}else{
		msg_format("���� %d �Ĥ�%s��ؤ٤롣", p_ptr->new_spells, p);
	}
#else
	msg_format("You can learn %d new %s%s.", p_ptr->new_spells, p,
		(p_ptr->new_spells == 1?"":"s"));
#endif

	msg_print(NULL);


	/* Restrict choices to "useful" books */
	if (p_ptr->realm2 == REALM_NONE) item_tester_tval = mp_ptr->spell_book;
	else item_tester_hook = item_tester_learn_spell;

	/* Get an item */
	q = _("�ɤ��ܤ���ؤӤޤ���? ", "Study which book? ");
	s = _("�ɤ���ܤ��ʤ���", "You have no books that you can read.");

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

	/* Access the item's sval */
	sval = o_ptr->sval;

	if (o_ptr->tval == REALM2_BOOK) increment = 32;
	else if (o_ptr->tval != REALM1_BOOK)
	{
		if (!get_check(_("��������ˡ���ΰ���ѹ����ޤ�����", "Really, change magic realm? "))) return;
		change_realm2(tval2realm(o_ptr->tval));
		increment = 32;
	}

	/* Track the object kind */
	object_kind_track(o_ptr->k_idx);

	/* Hack -- Handle stuff */
	handle_stuff();

	/* Mage -- Learn a selected spell */
	if (mp_ptr->spell_book != TV_LIFE_BOOK)
	{
		/* Ask for a spell, allow cancel */
#ifdef JP
		if (!get_spell(&spell, "�ؤ�", sval, FALSE, o_ptr->tval - TV_LIFE_BOOK + 1)
			&& (spell == -1)) return;
#else
		if (!get_spell(&spell, "study", sval, FALSE, o_ptr->tval - TV_LIFE_BOOK + 1)
			&& (spell == -1)) return;
#endif

	}

	/* Priest -- Learn a random prayer */
	else
	{
		int k = 0;

		int gift = -1;

		/* Extract spells */
		for (spell = 0; spell < 32; spell++)
		{
			/* Check spells in the book */
			if ((fake_spell_flags[sval] & (1L << spell)))
			{
				/* Skip non "okay" prayers */
				if (!spell_okay(spell, FALSE, TRUE,
					(increment ? p_ptr->realm2 : p_ptr->realm1))) continue;

				/* Hack -- Prepare the randomizer */
				k++;

				/* Hack -- Apply the randomizer */
				if (one_in_(k)) gift = spell;
			}
		}

		/* Accept gift */
		spell = gift;
	}

	/* Nothing to study */
	if (spell < 0)
	{
		/* Message */
		msg_format(_("�����ܤˤϳؤ֤٤�%s���ʤ���", "You cannot learn any %ss in that book."), p);

		/* Abort */
		return;
	}


	if (increment) spell += increment;

	/* Learn the spell */
	if (spell < 32)
	{
		if (p_ptr->spell_learned1 & (1L << spell)) learned = TRUE;
		else p_ptr->spell_learned1 |= (1L << spell);
	}
	else
	{
		if (p_ptr->spell_learned2 & (1L << (spell - 32))) learned = TRUE;
		else p_ptr->spell_learned2 |= (1L << (spell - 32));
	}

	if (learned)
	{
		int max_exp = (spell < 32) ? SPELL_EXP_MASTER : SPELL_EXP_EXPERT;
		int old_exp = p_ptr->spell_exp[spell];
		int new_rank = EXP_LEVEL_UNSKILLED;
		cptr name = do_spell(increment ? p_ptr->realm2 : p_ptr->realm1, spell%32, SPELL_NAME);

		if (old_exp >= max_exp)
		{
			msg_format(_("����%s�ϴ����˻Ȥ����ʤ���Τǳؤ�ɬ�פϤʤ���", "You don't need to study this %s anymore."), p);
			return;
		}
#ifdef JP
		if (!get_check(format("%s��%s�򤵤�˳ؤӤޤ���������Ǥ�����", name, p)))
#else
		if (!get_check(format("You will study a %s of %s again. Are you sure? ", p, name)))
#endif
		{
			return;
		}
		else if (old_exp >= SPELL_EXP_EXPERT)
		{
			p_ptr->spell_exp[spell] = SPELL_EXP_MASTER;
			new_rank = EXP_LEVEL_MASTER;
		}
		else if (old_exp >= SPELL_EXP_SKILLED)
		{
			if (spell >= 32) p_ptr->spell_exp[spell] = SPELL_EXP_EXPERT;
			else p_ptr->spell_exp[spell] += SPELL_EXP_EXPERT - SPELL_EXP_SKILLED;
			new_rank = EXP_LEVEL_EXPERT;
		}
		else if (old_exp >= SPELL_EXP_BEGINNER)
		{
			p_ptr->spell_exp[spell] = SPELL_EXP_SKILLED + (old_exp - SPELL_EXP_BEGINNER) * 2 / 3;
			new_rank = EXP_LEVEL_SKILLED;
		}
		else
		{
			p_ptr->spell_exp[spell] = SPELL_EXP_BEGINNER + old_exp / 3;
			new_rank = EXP_LEVEL_BEGINNER;
		}
		msg_format(_("%s�ν����٤�%s�˾夬�ä���", "Your proficiency of %s is now %s rank."), name, exp_level_str[new_rank]);
	}
	else
	{
		/* Find the next open entry in "p_ptr->spell_order[]" */
		for (i = 0; i < 64; i++)
		{
			/* Stop at the first empty space */
			if (p_ptr->spell_order[i] == 99) break;
		}

		/* Add the spell to the known list */
		p_ptr->spell_order[i++] = spell;

		/* Mention the result */
#ifdef JP
		/* �����ڤ��ؤ���ǽ���б� */
		if (mp_ptr->spell_book == TV_MUSIC_BOOK)
		{
			msg_format("%s��ؤ����",
				    do_spell(increment ? p_ptr->realm2 : p_ptr->realm1, spell % 32, SPELL_NAME));
		}
		else
		{
			msg_format("%s��%s��ؤ����",
				    do_spell(increment ? p_ptr->realm2 : p_ptr->realm1, spell % 32, SPELL_NAME) ,p);
		}
#else
		msg_format("You have learned the %s of %s.",
			p, do_spell(increment ? p_ptr->realm2 : p_ptr->realm1, spell % 32, SPELL_NAME));
#endif
	}

	/* Take a turn */
	p_ptr->energy_use = 100;

	switch (mp_ptr->spell_book)
	{
	case TV_LIFE_BOOK:
		chg_virtue(V_FAITH, 1);
		break;
	case TV_DEATH_BOOK:
		chg_virtue(V_UNLIFE, 1);
		break;
	case TV_NATURE_BOOK:
		chg_virtue(V_NATURE, 1);
		break;
	default:
		chg_virtue(V_KNOWLEDGE, 1);
		break;
	}

	/* Sound */
	sound(SOUND_STUDY);

	/* One less spell available */
	p_ptr->learned_spells++;
#if 0
	/* Message if needed */
	if (p_ptr->new_spells)
	{
		/* Message */
#ifdef JP
		if (p_ptr->new_spells < 10) msg_format("���� %d �Ĥ�%s��ؤ٤롣", p_ptr->new_spells, p);
		else msg_format("���� %d �Ĥ�%s��ؤ٤롣", p_ptr->new_spells, p);
#else
		msg_format("You can learn %d more %s%s.", p_ptr->new_spells, p,
		           (p_ptr->new_spells != 1) ? "s" : "");
#endif
	}
#endif

	/* Update Study */
	p_ptr->update |= (PU_SPELLS);
	update_stuff();

	/* Redraw object recall */
	p_ptr->window |= (PW_OBJECT);
}

/*!
 * @brief ��̿���ء��¤Ӥ˥�����Ū�ʸ��̤�ȯư
 * @param spell ������ʸ��̤����򤹤뤿��δ��ID
 * @return �ʤ�
 */
static void wild_magic(int spell)
{
	int counter = 0;
	int type = SUMMON_MOLD + randint0(6);

	if (type < SUMMON_MOLD) type = SUMMON_MOLD;
	else if (type > SUMMON_MIMIC) type = SUMMON_MIMIC;

	switch (randint1(spell) + randint1(8) + 1)
	{
	case 1:
	case 2:
	case 3:
		teleport_player(10, TELEPORT_PASSIVE);
		break;
	case 4:
	case 5:
	case 6:
		teleport_player(100, TELEPORT_PASSIVE);
		break;
	case 7:
	case 8:
		teleport_player(200, TELEPORT_PASSIVE);
		break;
	case 9:
	case 10:
	case 11:
		unlite_area(10, 3);
		break;
	case 12:
	case 13:
	case 14:
		lite_area(damroll(2, 3), 2);
		break;
	case 15:
		destroy_doors_touch();
		break;
	case 16: case 17:
		wall_breaker();
	case 18:
		sleep_monsters_touch();
		break;
	case 19:
	case 20:
		trap_creation(p_ptr->y, p_ptr->x);
		break;
	case 21:
	case 22:
		door_creation();
		break;
	case 23:
	case 24:
	case 25:
		aggravate_monsters(0);
		break;
	case 26:
		earthquake(p_ptr->y, p_ptr->x, 5);
		break;
	case 27:
	case 28:
		(void)gain_random_mutation(0);
		break;
	case 29:
	case 30:
		apply_disenchant(1);
		break;
	case 31:
		lose_all_info();
		break;
	case 32:
		fire_ball(GF_CHAOS, 0, spell + 5, 1 + (spell / 10));
		break;
	case 33:
		wall_stone();
		break;
	case 34:
	case 35:
		while (counter++ < 8)
		{
			(void)summon_specific(0, p_ptr->y, p_ptr->x, (dun_level * 3) / 2, type, (PM_ALLOW_GROUP | PM_NO_PET));
		}
		break;
	case 36:
	case 37:
		activate_hi_summon(p_ptr->y, p_ptr->x, FALSE);
		break;
	case 38:
		(void)summon_cyber(-1, p_ptr->y, p_ptr->x);
		break;
	default:
		{
			int count = 0;
			(void)activate_ty_curse(FALSE, &count);
			break;
		}
	}

	return;
}


/*!
 * @brief ��ˡ��Ӿ����륳�ޥ�ɤΥᥤ��롼���� /
 * Cast a spell
 * @return �ʤ�
 */
void do_cmd_cast(void)
{
	OBJECT_IDX item;
	OBJECT_SUBTYPE_VALUE sval;
	SPELL_IDX spell;
	REALM_IDX realm;
	int	chance;
	int	increment = 0;
	REALM_IDX use_realm;
	MANA_POINT need_mana;

	cptr prayer;
	object_type	*o_ptr;
	const magic_type *s_ptr;
	cptr q, s;

	bool over_exerted = FALSE;

	/* Require spell ability */
	if (!p_ptr->realm1 && (p_ptr->pclass != CLASS_SORCERER) && (p_ptr->pclass != CLASS_RED_MAGE))
	{
		msg_print(_("��ʸ�򾧤����ʤ���", "You cannot cast spells!"));
		return;
	}

	/* Require lite */
	if (p_ptr->blind || no_lite())
	{
		if (p_ptr->pclass == CLASS_FORCETRAINER) confirm_use_force(FALSE);
		else
		{
			msg_print(_("�ܤ������ʤ���", "You cannot see!"));
			flush();
		}
		return;
	}

	/* Not when confused */
	if (p_ptr->confused)
	{
		msg_print(_("���𤷤Ƥ��ƾ������ʤ���", "You are too confused!"));
		flush();
		return;
	}

	/* Hex */
	if (p_ptr->realm1 == REALM_HEX)
	{
		if (hex_spell_fully())
		{
			bool flag = FALSE;
			msg_print(_("����ʾ忷������ʸ��Ӿ����뤳�ȤϤǤ��ʤ���", "Can not spell new spells more."));
			flush();
			if (p_ptr->lev >= 35) flag = stop_hex_spell();
			if (!flag) return;
		}
	}

	if (p_ptr->pclass == CLASS_FORCETRAINER)
	{
		if (player_has_no_spellbooks())
		{
			confirm_use_force(FALSE);
			return;
		}
		select_the_force = TRUE;
	}

	prayer = spell_category_name(mp_ptr->spell_book);

	/* Restrict choices to spell books */
	item_tester_tval = mp_ptr->spell_book;

	/* Get an item */
	q = _("�ɤμ�ʸ���Ȥ��ޤ���? ", "Use which book? ");
	s = _("��ʸ�񤬤ʤ���", "You have no spell books!");

	if (!get_item(&item, q, s, (USE_INVEN | USE_FLOOR)))
	{
		select_the_force = FALSE;
		return;
	}
	select_the_force = FALSE;

	if (item == INVEN_FORCE) /* the_force */
	{
		do_cmd_mind();
		return;
	}

	/* Get the item (in the pack) */
	else if (item >= 0)
	{
		o_ptr = &inventory[item];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - item];
	}

	/* Access the item's sval */
	sval = o_ptr->sval;

	if ((p_ptr->pclass != CLASS_SORCERER) && (p_ptr->pclass != CLASS_RED_MAGE) && (o_ptr->tval == REALM2_BOOK)) increment = 32;


	/* Track the object kind */
	object_kind_track(o_ptr->k_idx);

	/* Hack -- Handle stuff */
	handle_stuff();

	if ((p_ptr->pclass == CLASS_SORCERER) || (p_ptr->pclass == CLASS_RED_MAGE))
		realm = o_ptr->tval - TV_LIFE_BOOK + 1;
	else if (increment) realm = p_ptr->realm2;
	else realm = p_ptr->realm1;

	/* Ask for a spell */
#ifdef JP
	if (!get_spell(&spell,  
				((mp_ptr->spell_book == TV_LIFE_BOOK) ? "�Ӿ�����" : (mp_ptr->spell_book == TV_MUSIC_BOOK) ? "�Τ�" : "������"), 
		       sval, TRUE, realm))
	{
		if (spell == -2) msg_format("�����ܤˤ��ΤäƤ���%s���ʤ���", prayer);
		return;
	}
#else
	if (!get_spell(&spell, ((mp_ptr->spell_book == TV_LIFE_BOOK) ? "recite" : "cast"),
		sval, TRUE, realm))
	{
		if (spell == -2)
			msg_format("You don't know any %ss in that book.", prayer);
		return;
	}
#endif


	use_realm = tval2realm(o_ptr->tval);

	/* Hex */
	if (use_realm == REALM_HEX)
	{
		if (hex_spelling(spell))
		{
			msg_print(_("���μ�ʸ�Ϥ��Ǥ˱Ӿ������", "You are already casting it."));
			return;
		}
	}

	if (!is_magic(use_realm))
	{
		s_ptr = &technic_info[use_realm - MIN_TECHNIC][spell];
	}
	else
	{
		s_ptr = &mp_ptr->info[realm - 1][spell];
	}

	/* Extract mana consumption rate */
	need_mana = mod_need_mana(s_ptr->smana, spell, realm);

	/* Verify "dangerous" spells */
	if (need_mana > p_ptr->csp)
	{
		if (flush_failure) flush();

		/* Warning */
#ifdef JP
msg_format("����%s��%s�Τ˽�ʬ�ʥޥ��å��ݥ���Ȥ��ʤ���",prayer,
 ((mp_ptr->spell_book == TV_LIFE_BOOK) ? "�Ӿ�����" : (mp_ptr->spell_book == TV_LIFE_BOOK) ? "�Τ�" : "������"));
#else
		msg_format("You do not have enough mana to %s this %s.",
			((mp_ptr->spell_book == TV_LIFE_BOOK) ? "recite" : "cast"),
			prayer);
#endif


		if (!over_exert) return;

		/* Verify */
		if (!get_check_strict(_("����Ǥ�ĩ�路�ޤ���? ", "Attempt it anyway? "), CHECK_OKAY_CANCEL)) return;
	}

	/* Spell failure chance */
	chance = spell_chance(spell, use_realm);

	/* Sufficient mana */
	if (need_mana <= p_ptr->csp)
	{
		/* Use some mana */
		p_ptr->csp -= need_mana;
	}
	else over_exerted = TRUE;

	/* Redraw mana */
	p_ptr->redraw |= (PR_MANA);

	/* Failed spell */
	if (randint0(100) < chance)
	{
		if (flush_failure) flush();
		
		msg_format(_("%s�򤦤ޤ��������ʤ��ä���", "You failed to get the %s off!"), prayer);
		sound(SOUND_FAIL);

		switch (realm)
		{
		case REALM_LIFE:
			if (randint1(100) < chance) chg_virtue(V_VITALITY, -1);
			break;
		case REALM_DEATH:
			if (randint1(100) < chance) chg_virtue(V_UNLIFE, -1);
			break;
		case REALM_NATURE:
			if (randint1(100) < chance) chg_virtue(V_NATURE, -1);
			break;
		case REALM_DAEMON:
			if (randint1(100) < chance) chg_virtue(V_JUSTICE, 1);
			break;
		case REALM_CRUSADE:
			if (randint1(100) < chance) chg_virtue(V_JUSTICE, -1);
			break;
		case REALM_HEX:
			if (randint1(100) < chance) chg_virtue(V_COMPASSION, -1);
			break;
		default:
			if (randint1(100) < chance) chg_virtue(V_KNOWLEDGE, -1);
			break;
		}

		/* Failure casting may activate some side effect */
		do_spell(realm, spell, SPELL_FAIL);


		if ((o_ptr->tval == TV_CHAOS_BOOK) && (randint1(100) < spell))
		{
			msg_print(_("������Ū�ʸ��̤�ȯ��������", "You produce a chaotic effect!"));
			wild_magic(spell);
		}
		else if ((o_ptr->tval == TV_DEATH_BOOK) && (randint1(100) < spell))
		{
			if ((sval == 3) && one_in_(2))
			{
				sanity_blast(0, TRUE);
			}
			else
			{
				msg_print(_("�ˤ���", "It hurts!"));
				take_hit(DAMAGE_LOSELIFE, damroll(o_ptr->sval + 1, 6), _("�Ź���ˡ�ε�ή", "a miscast Death spell"), -1);

				if ((spell > 15) && one_in_(6) && !p_ptr->hold_exp)
					lose_exp(spell * 250);
			}
		}
		else if ((o_ptr->tval == TV_MUSIC_BOOK) && (randint1(200) < spell))
		{
			msg_print(_("����ʲ���������", "An infernal sound echoed."));
			aggravate_monsters(0);
		}
		if (randint1(100) >= chance)
			chg_virtue(V_CHANCE,-1);
	}

	/* Process spell */
	else
	{
		/* Canceled spells cost neither a turn nor mana */
		if (!do_spell(realm, spell, SPELL_CAST)) return;

		if (randint1(100) < chance)
			chg_virtue(V_CHANCE,1);

		/* A spell was cast */
		if (!(increment ?
		    (p_ptr->spell_worked2 & (1L << spell)) :
		    (p_ptr->spell_worked1 & (1L << spell)))
		    && (p_ptr->pclass != CLASS_SORCERER)
		    && (p_ptr->pclass != CLASS_RED_MAGE))
		{
			int e = s_ptr->sexp;

			/* The spell worked */
			if (realm == p_ptr->realm1)
			{
				p_ptr->spell_worked1 |= (1L << spell);
			}
			else
			{
				p_ptr->spell_worked2 |= (1L << spell);
			}

			/* Gain experience */
			gain_exp(e * s_ptr->slevel);

			/* Redraw object recall */
			p_ptr->window |= (PW_OBJECT);

			switch (realm)
			{
			case REALM_LIFE:
				chg_virtue(V_TEMPERANCE, 1);
				chg_virtue(V_COMPASSION, 1);
				chg_virtue(V_VITALITY, 1);
				chg_virtue(V_DILIGENCE, 1);
				break;
			case REALM_DEATH:
				chg_virtue(V_UNLIFE, 1);
				chg_virtue(V_JUSTICE, -1);
				chg_virtue(V_FAITH, -1);
				chg_virtue(V_VITALITY, -1);
				break;
			case REALM_DAEMON:
				chg_virtue(V_JUSTICE, -1);
				chg_virtue(V_FAITH, -1);
				chg_virtue(V_HONOUR, -1);
				chg_virtue(V_TEMPERANCE, -1);
				break;
			case REALM_CRUSADE:
				chg_virtue(V_FAITH, 1);
				chg_virtue(V_JUSTICE, 1);
				chg_virtue(V_SACRIFICE, 1);
				chg_virtue(V_HONOUR, 1);
				break;
			case REALM_NATURE:
				chg_virtue(V_NATURE, 1);
				chg_virtue(V_HARMONY, 1);
				break;
			case REALM_HEX:
				chg_virtue(V_JUSTICE, -1);
				chg_virtue(V_FAITH, -1);
				chg_virtue(V_HONOUR, -1);
				chg_virtue(V_COMPASSION, -1);
				break;
			default:
				chg_virtue(V_KNOWLEDGE, 1);
				break;
			}
		}
		switch (realm)
		{
		case REALM_LIFE:
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_TEMPERANCE, 1);
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_COMPASSION, 1);
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_VITALITY, 1);
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_DILIGENCE, 1);
			break;
		case REALM_DEATH:
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_UNLIFE, 1);
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_JUSTICE, -1);
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_FAITH, -1);
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_VITALITY, -1);
			break;
		case REALM_DAEMON:
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_JUSTICE, -1);
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_FAITH, -1);
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_HONOUR, -1);
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_TEMPERANCE, -1);
			break;
		case REALM_CRUSADE:
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_FAITH, 1);
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_JUSTICE, 1);
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_SACRIFICE, 1);
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_HONOUR, 1);
			break;
		case REALM_NATURE:
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_NATURE, 1);
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_HARMONY, 1);
			break;
		case REALM_HEX:
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_JUSTICE, -1);
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_FAITH, -1);
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_HONOUR, -1);
			if (randint1(100 + p_ptr->lev) < need_mana) chg_virtue(V_COMPASSION, -1);
			break;
		}
		if (mp_ptr->spell_xtra & MAGIC_GAIN_EXP)
		{
			s16b cur_exp = p_ptr->spell_exp[(increment ? 32 : 0)+spell];
			s16b exp_gain = 0;

			if (cur_exp < SPELL_EXP_BEGINNER)
				exp_gain += 300; /* #tang 60 -> 300*/
			else if (cur_exp < SPELL_EXP_SKILLED)
			{
				if ((dun_level > 2) && ((dun_level * 3 + 10) > p_ptr->lev)) /* #tang 4 -> 2 , dun_level -> dun_level*3 */
					exp_gain = 40; /* #tang 8 -> 40*/
			}
			else if (cur_exp < SPELL_EXP_EXPERT)
			{
				if (((dun_level * 3 + 5) > p_ptr->lev) && ((dun_level * 3 + 5) > s_ptr->slevel)) /* #tang dun_level -> dun_level*3 */
					exp_gain = 10; /* #tang 2 -> 10*/
			}
			else if ((cur_exp < SPELL_EXP_MASTER) && !increment)
			{
				if (((dun_level * 3 + 5) > p_ptr->lev) && (dun_level * 3 > s_ptr->slevel)) /* #tang dun_level -> dun_level*3 */
					exp_gain = 5; /* #tang 1 -> 5*/
			}
			p_ptr->spell_exp[(increment ? 32 : 0) + spell] += exp_gain;
		}
	}

	/* Take a turn */
	p_ptr->energy_use = 100;


	/* Over-exert the player */
	if(over_exerted)
	{
		int oops = need_mana;

		/* No mana left */
		p_ptr->csp = 0;
		p_ptr->csp_frac = 0;

		/* Message */
		msg_print(_("�������椷�����Ƶ��򼺤äƤ��ޤä���", "You faint from the effort!"));

		/* Hack -- Bypass free action */
		(void)set_paralyzed(p_ptr->paralyzed + randint1(5 * oops + 1));

		switch (realm)
		{
		case REALM_LIFE:
			chg_virtue(V_VITALITY, -10);
			break;
		case REALM_DEATH:
			chg_virtue(V_UNLIFE, -10);
			break;
		case REALM_DAEMON:
			chg_virtue(V_JUSTICE, 10);
			break;
		case REALM_NATURE:
			chg_virtue(V_NATURE, -10);
			break;
		case REALM_CRUSADE:
			chg_virtue(V_JUSTICE, -10);
			break;
		case REALM_HEX:
			chg_virtue(V_COMPASSION, 10);
			break;
		default:
			chg_virtue(V_KNOWLEDGE, -10);
			break;
		}

		/* Damage CON (possibly permanently) */
		if (randint0(100) < 50)
		{
			bool perm = (randint0(100) < 25);

			/* Message */
			msg_print(_("�Τ򰭤����Ƥ��ޤä���", "You have damaged your health!"));

			/* Reduce constitution */
			(void)dec_stat(A_CON, 15 + randint1(10), perm);
		}
	}

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
	p_ptr->window |= (PW_SPELL);
}

/*!
 * @brief �ڥåȤˤʤäƤ����󥹥����򥽡��Ȥ��뤿�����ӽ���
 * @param u ��󥹥����ι�¤������
 * @param v ̤����
 * @param a ����оݤΥ�󥹥���ID1
 * @param b ����оݤΥ�󥹥���ID2
 * @return 2���ܤ���ʤ��TRUE���֤�
 */
static bool ang_sort_comp_pet_dismiss(vptr u, vptr v, int a, int b)
{
	u16b *who = (u16b*)(u);

	int w1 = who[a];
	int w2 = who[b];

	monster_type *m_ptr1 = &m_list[w1];
	monster_type *m_ptr2 = &m_list[w2];
	monster_race *r_ptr1 = &r_info[m_ptr1->r_idx];
	monster_race *r_ptr2 = &r_info[m_ptr2->r_idx];

	/* Unused */
	(void)v;

	if (w1 == p_ptr->riding) return TRUE;
	if (w2 == p_ptr->riding) return FALSE;

	if (m_ptr1->nickname && !m_ptr2->nickname) return TRUE;
	if (m_ptr2->nickname && !m_ptr1->nickname) return FALSE;

	if (!m_ptr1->parent_m_idx && m_ptr2->parent_m_idx) return TRUE;
	if (!m_ptr2->parent_m_idx && m_ptr1->parent_m_idx) return FALSE;

	if ((r_ptr1->flags1 & RF1_UNIQUE) && !(r_ptr2->flags1 & RF1_UNIQUE)) return TRUE;
	if ((r_ptr2->flags1 & RF1_UNIQUE) && !(r_ptr1->flags1 & RF1_UNIQUE)) return FALSE;

	if (r_ptr1->level > r_ptr2->level) return TRUE;
	if (r_ptr2->level > r_ptr1->level) return FALSE;

	if (m_ptr1->hp > m_ptr2->hp) return TRUE;
	if (m_ptr2->hp > m_ptr1->hp) return FALSE;
	
	return w1 <= w2;
}

/*!
 * @brief �ڥåȤ�����°���˱������ݻ������Ȥ�����׻�����
 * @param m_ptr �׻����Ȥʤ��󥹥����ι�¤�λ��ȥݥ���
 * @param inc m_ptr�ǻ��ꤷ����󥹥�����ݻ������ȷ׻��˲ä���ʤ�TRUE�������ʤ�FALSE�����
 * @return �ʤ�
 */
void check_pets_num_and_align(monster_type *m_ptr, bool inc)
{
	s32b old_friend_align = friend_align;
	monster_race *r_ptr = &r_info[m_ptr->r_idx];

	if (inc)
	{
		total_friends++;
		if (r_ptr->flags3 & RF3_GOOD) friend_align += r_ptr->level;
		if (r_ptr->flags3 & RF3_EVIL) friend_align -= r_ptr->level;
	}
	else
	{
		total_friends--;
		if (r_ptr->flags3 & RF3_GOOD) friend_align -= r_ptr->level;
		if (r_ptr->flags3 & RF3_EVIL) friend_align += r_ptr->level;
	}

	if (old_friend_align != friend_align) p_ptr->update |= (PU_BONUS);
}

/*!
 * @brief �ڥåȤΰݻ������ȷ׻�
 * @return �ݻ�������(%)
 */
int calculate_upkeep(void)
{
	s32b old_friend_align = friend_align;
	MONSTER_IDX m_idx;
	bool have_a_unique = FALSE;
	s32b total_friend_levels = 0;

	total_friends = 0;
	friend_align = 0;

	for (m_idx = m_max - 1; m_idx >=1; m_idx--)
	{
		monster_type *m_ptr;
		monster_race *r_ptr;
		
		m_ptr = &m_list[m_idx];
		if (!m_ptr->r_idx) continue;
		r_ptr = &r_info[m_ptr->r_idx];

		if (is_pet(m_ptr))
		{
			total_friends++;
			if (r_ptr->flags1 & RF1_UNIQUE)
			{
				if (p_ptr->pclass == CLASS_CAVALRY)
				{
					if (p_ptr->riding == m_idx)
						total_friend_levels += (r_ptr->level+5)*2;
					else if (!have_a_unique && (r_info[m_ptr->r_idx].flags7 & RF7_RIDING))
						total_friend_levels += (r_ptr->level+5)*7/2;
					else
						total_friend_levels += (r_ptr->level+5)*10;
					have_a_unique = TRUE;
				}
				else
					total_friend_levels += (r_ptr->level+5)*10;
			}
			else
				total_friend_levels += r_ptr->level;

			/* Determine pet alignment */
			if (r_ptr->flags3 & RF3_GOOD) friend_align += r_ptr->level;
			if (r_ptr->flags3 & RF3_EVIL) friend_align -= r_ptr->level;
		}
	}
	if (old_friend_align != friend_align) p_ptr->update |= (PU_BONUS);
	if (total_friends)
	{
		int upkeep_factor;
		upkeep_factor = (total_friend_levels - (p_ptr->lev * 80 / (cp_ptr->pet_upkeep_div)));
		if (upkeep_factor < 0) upkeep_factor = 0;
		if (upkeep_factor > 1000) upkeep_factor = 1000;
		return upkeep_factor;
	}
	else
		return 0;
}

/*!
 * @brief �ڥåȤ������륳�ޥ�ɤΥᥤ��롼����
 * @return �ʤ�
 */
void do_cmd_pet_dismiss(void)
{
	monster_type	*m_ptr;
	bool		all_pets = FALSE;
	MONSTER_IDX pet_ctr;
	int i;
	int Dismissed = 0;

	MONSTER_IDX *who;
	u16b dummy_why;
	int max_pet = 0;
	bool_hack cu, cv;

	cu = Term->scr->cu;
	cv = Term->scr->cv;
	Term->scr->cu = 0;
	Term->scr->cv = 1;

	/* Allocate the "who" array */
	C_MAKE(who, max_m_idx, MONSTER_IDX);

	/* Process the monsters (backwards) */
	for (pet_ctr = m_max - 1; pet_ctr >= 1; pet_ctr--)
	{
		if (is_pet(&m_list[pet_ctr]))
			who[max_pet++] = pet_ctr;
	}

	/* Select the sort method */
	ang_sort_comp = ang_sort_comp_pet_dismiss;
	ang_sort_swap = ang_sort_swap_hook;

	ang_sort(who, &dummy_why, max_pet);

	/* Process the monsters (backwards) */
	for (i = 0; i < max_pet; i++)
	{
		bool delete_this;
		char friend_name[80];
		bool kakunin;

		/* Access the monster */
		pet_ctr = who[i];
		m_ptr = &m_list[pet_ctr];

		delete_this = FALSE;
		kakunin = ((pet_ctr == p_ptr->riding) || (m_ptr->nickname));
		monster_desc(friend_name, m_ptr, MD_ASSUME_VISIBLE);

		if (!all_pets)
		{
			/* Hack -- health bar for this monster */
			health_track(pet_ctr);

			/* Hack -- handle stuff */
			handle_stuff();
			
			msg_format(_("%s�������ޤ����� [Yes/No/Unnamed (%d��)]","Dismiss %s? [Yes/No/Unnamed (%d remain)]"), friend_name, max_pet - i);
			
			if (m_ptr->ml)
				move_cursor_relative(m_ptr->fy, m_ptr->fx);

			while (TRUE)
			{
				char ch = inkey();

				if (ch == 'Y' || ch == 'y')
				{
					delete_this = TRUE;

					if (kakunin)
					{
						msg_format(_("�����ˤ�����Ǥ����� (%s) ","Are you sure? (%s) "), friend_name);
						ch = inkey();
						if (ch != 'Y' && ch != 'y')
							delete_this = FALSE;
					}
					break;
				}

				if (ch == 'U' || ch == 'u')
				{
					all_pets = TRUE;
					break;
				}

				if (ch == ESCAPE || ch == 'N' || ch == 'n')
					break;

				bell();
			}
		}

		if ((all_pets && !kakunin) || (!all_pets && delete_this))
		{
			if (record_named_pet && m_ptr->nickname)
			{
				char m_name[80];

				monster_desc(m_name, m_ptr, MD_INDEF_VISIBLE);
				do_cmd_write_nikki(NIKKI_NAMED_PET, RECORD_NAMED_PET_DISMISS, m_name);
			}

			if (pet_ctr == p_ptr->riding)
			{
				msg_format(_("%s����ߤꤿ��","You have got off %s. "), friend_name);

				p_ptr->riding = 0;

				/* Update the monsters */
				p_ptr->update |= (PU_BONUS | PU_MONSTERS);
				p_ptr->redraw |= (PR_EXTRA | PR_UHEALTH);
			}

			/* HACK : Add the line to message buffer */
			msg_format(_("%s ����������","Dismissed %s."), friend_name);
			p_ptr->window |= (PW_MESSAGE);
			window_stuff();

			delete_monster_idx(pet_ctr);
			Dismissed++;
		}
	}

	Term->scr->cu = cu;
	Term->scr->cv = cv;
	Term_fresh();

	C_KILL(who, max_m_idx, MONSTER_IDX);

#ifdef JP
	msg_format("%d �ΤΥڥåȤ������ޤ�����", Dismissed);
#else
	msg_format("You have dismissed %d pet%s.", Dismissed,
		   (Dismissed == 1 ? "" : "s"));
#endif
	if (Dismissed == 0 && all_pets)
		msg_print(_("'U'nnamed �ϡ����ϰʳ���̾���Τʤ��ڥåȤ��������Ʋ������ޤ���", "'U'nnamed means all your pets except named pets and your mount."));
}

/*!
 * @brief �ץ쥤�䡼�ε���/���Ͻ���Ƚ��
 * @param c_ptr �ץ쥤�䡼�ΰ�ư��ޥ��ι�¤�λ��ȥݥ���
 * @param now_riding TRUE�ʤ鲼�Ͻ�����FALSE�ʤ�е������
 * @return ��ǽ�ʤ��TRUE���֤�
 */
static bool player_can_ride_aux(cave_type *c_ptr, bool now_riding)
{
	bool p_can_enter;
	bool old_character_xtra = character_xtra;
	MONSTER_IDX old_riding = p_ptr->riding;
	bool old_riding_ryoute = p_ptr->riding_ryoute;
	bool old_old_riding_ryoute = p_ptr->old_riding_ryoute;
	bool old_pf_ryoute = (p_ptr->pet_extra_flags & PF_RYOUTE) ? TRUE : FALSE;

	/* Hack -- prevent "icky" message */
	character_xtra = TRUE;

	if (now_riding) p_ptr->riding = c_ptr->m_idx;
	else
	{
		p_ptr->riding = 0;
		p_ptr->pet_extra_flags &= ~(PF_RYOUTE);
		p_ptr->riding_ryoute = p_ptr->old_riding_ryoute = FALSE;
	}

	calc_bonuses();

	p_can_enter = player_can_enter(c_ptr->feat, CEM_P_CAN_ENTER_PATTERN);

	p_ptr->riding = old_riding;
	if (old_pf_ryoute) p_ptr->pet_extra_flags |= (PF_RYOUTE);
	else p_ptr->pet_extra_flags &= ~(PF_RYOUTE);
	p_ptr->riding_ryoute = old_riding_ryoute;
	p_ptr->old_riding_ryoute = old_old_riding_ryoute;

	calc_bonuses();

	character_xtra = old_character_xtra;

	return p_can_enter;
}

/*!
 * @brief �ץ쥤�䡼������Ƚ�����
 * @param dam ����Ƚ���ȯ�����ݤ˼��������᡼����
 * @param force TRUE�ʤ�ж���Ū�����Ϥ���
 * @return �ºݤ����Ϥ�����TRUE���֤�
 */
bool rakuba(HIT_POINT dam, bool force)
{
	int i, y, x, oy, ox;
	int sn = 0, sy = 0, sx = 0;
	char m_name[80];
	monster_type *m_ptr = &m_list[p_ptr->riding];
	monster_race *r_ptr = &r_info[m_ptr->r_idx];
	bool fall_dam = FALSE;

	if (!p_ptr->riding) return FALSE;
	if (p_ptr->wild_mode) return FALSE;

	if (dam >= 0 || force)
	{
		if (!force)
		{
			int cur = p_ptr->skill_exp[GINOU_RIDING];
			int max = s_info[p_ptr->pclass].s_max[GINOU_RIDING];
			int ridinglevel = r_ptr->level * 3; /* #tang r_ptr->level -> r_ptr->level*3 */

			/* ���ϤΤ��䤹�� */
			int rakubalevel = r_ptr->level * 3; /* #tang r_ptr->level -> r_ptr->level*3 */
			if (p_ptr->riding_ryoute) rakubalevel += 20;

			if ((cur < max) && (max > 1000) &&
			    (dam / 2 + ridinglevel) > (cur / 30 + 10))
			{
				int inc = 0;

				if (ridinglevel > (cur / 100 + 15))
					inc += 1 + (ridinglevel - cur / 100 - 15);
				else
					inc += 1;

				p_ptr->skill_exp[GINOU_RIDING] = MIN(max, cur + inc);
			}

			/* ��٥���㤤���Ϥ�������Ϥ��ˤ��� */
			if (randint0(dam / 2 + rakubalevel * 2) < cur / 30 + 10)
			{
				if ((((p_ptr->pclass == CLASS_BEASTMASTER) || (p_ptr->pclass == CLASS_CAVALRY)) && !p_ptr->riding_ryoute) || !one_in_(p_ptr->lev*(p_ptr->riding_ryoute ? 2 : 3) + 30))
				{
					return FALSE;
				}
			}
		}

		/* Check around the player */
		for (i = 0; i < 8; i++)
		{
			cave_type *c_ptr;

			/* Access the location */
			y = p_ptr->y + ddy_ddd[i];
			x = p_ptr->x + ddx_ddd[i];

			c_ptr = &cave[y][x];

			if (c_ptr->m_idx) continue;

			/* Skip non-empty grids */
			if (!cave_have_flag_grid(c_ptr, FF_MOVE) && !cave_have_flag_grid(c_ptr, FF_CAN_FLY))
			{
				if (!player_can_ride_aux(c_ptr, FALSE)) continue;
			}

			if (cave_have_flag_grid(c_ptr, FF_PATTERN)) continue;

			/* Count "safe" grids */
			sn++;

			/* Randomize choice */
			if (randint0(sn) > 0) continue;

			/* Save the safe location */
			sy = y; sx = x;
		}
		if (!sn)
		{
			monster_desc(m_name, m_ptr, 0);
#ifdef JP
msg_format("%s���鿶����Ȥ��줽���ˤʤäơ��ɤˤ֤Ĥ��ä���",m_name);
			take_hit(DAMAGE_NOESCAPE, r_ptr->level+3, "�ɤؤξ���", -1);
#else
			msg_format("You have nearly fallen from %s, but bumped into wall.",m_name);
			take_hit(DAMAGE_NOESCAPE, r_ptr->level+3, "bumping into wall", -1);
#endif
			return FALSE;
		}

		oy = p_ptr->y;
		ox = p_ptr->x;

		p_ptr->y = sy;
		p_ptr->x = sx;

		/* Redraw the old spot */
		lite_spot(oy, ox);

		/* Redraw the new spot */
		lite_spot(p_ptr->y, p_ptr->x);

		/* Check for new panel */
		verify_panel();
	}

	p_ptr->riding = 0;
	p_ptr->pet_extra_flags &= ~(PF_RYOUTE);
	p_ptr->riding_ryoute = p_ptr->old_riding_ryoute = FALSE;

	calc_bonuses();

	p_ptr->update |= (PU_BONUS);

	/* Update stuff */
	p_ptr->update |= (PU_VIEW | PU_LITE | PU_FLOW | PU_MON_LITE | PU_MONSTERS);

	/* Window stuff */
	p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);

	p_ptr->redraw |= (PR_EXTRA);

	/* Update health track of mount */
	p_ptr->redraw |= (PR_UHEALTH);

	if (p_ptr->levitation && !force)
	{
		monster_desc(m_name, m_ptr, 0);
		msg_format(_("%s�����������������Ǥ��ޤ�������Ω��ľ�������Ϥ�����", "You are thrown from %s, but make a good landing."),m_name);
	}
	else
	{
		take_hit(DAMAGE_NOESCAPE, r_ptr->level+3, _("����", "Falling from riding"), -1);
		fall_dam = TRUE;
	}

	/* Move the player */
	if (sy && !p_ptr->is_dead)
		(void)move_player_effect(p_ptr->y, p_ptr->x, MPE_DONT_PICKUP | MPE_DONT_SWAP_MON);

	return fall_dam;
}

/*!
 * @brief �ڥåȤ��鵳��/���Ϥ��륳�ޥ�ɤΥᥤ��롼���� /
 * @param force ����Ū�˵���/���Ϥ���ʤ��TRUE
 * @return ����/���ϤǤ�����TRUE
 */
bool do_riding(bool force)
{
	int x, y, dir = 0;
	cave_type *c_ptr;
	monster_type *m_ptr;

	if (!get_rep_dir2(&dir)) return FALSE;
	y = p_ptr->y + ddy[dir];
	x = p_ptr->x + ddx[dir];
	c_ptr = &cave[y][x];

	if (p_ptr->special_defense & KATA_MUSOU) set_action(ACTION_NONE);

	if (p_ptr->riding)
	{
		/* Skip non-empty grids */
		if (!player_can_ride_aux(c_ptr, FALSE))
		{
			msg_print(_("������ˤϹߤ���ޤ���", "You cannot go to that direction."));
			return FALSE;
		}

		if (!pattern_seq(p_ptr->y, p_ptr->x, y, x)) return FALSE;

		if (c_ptr->m_idx)
		{
			/* Take a turn */
			p_ptr->energy_use = 100;

			/* Message */
			msg_print(_("��󥹥�����Ω���դ����äƤ��롪", "There is a monster in the way!"));

			py_attack(y, x, 0);
			return FALSE;
		}

		p_ptr->riding = 0;
		p_ptr->pet_extra_flags &= ~(PF_RYOUTE);
		p_ptr->riding_ryoute = p_ptr->old_riding_ryoute = FALSE;
	}
	else
	{
		if (p_ptr->confused)
		{
			msg_print(_("���𤷤Ƥ��ƾ��ʤ���", "You are too confused!"));
			return FALSE;
		}

		m_ptr = &m_list[c_ptr->m_idx];

		if (!c_ptr->m_idx || !m_ptr->ml)
		{
			msg_print(_("���ξ��ˤϥ�󥹥����Ϥ��ޤ���", "Here is no monster."));
			return FALSE;
		}
		if (!is_pet(m_ptr) && !force)
		{
			msg_print(_("���Υ�󥹥����ϥڥåȤǤϤ���ޤ���", "That monster is not a pet."));
			return FALSE;
		}
		if (!(r_info[m_ptr->r_idx].flags7 & RF7_RIDING))
		{
			msg_print(_("���Υ�󥹥����ˤϾ��ʤ���������", "This monster doesn't seem suitable for riding."));
			return FALSE;
		}

		if (!pattern_seq(p_ptr->y, p_ptr->x, y, x)) return FALSE;

		if (!player_can_ride_aux(c_ptr, TRUE))
		{
			/* Feature code (applying "mimic" field) */
			feature_type *f_ptr = &f_info[get_feat_mimic(c_ptr)];
#ifdef JP
			msg_format("���Υ�󥹥�����%s��%s�ˤ��롣", f_name + f_ptr->name,
			           ((!have_flag(f_ptr->flags, FF_MOVE) && !have_flag(f_ptr->flags, FF_CAN_FLY)) ||
			            (!have_flag(f_ptr->flags, FF_LOS) && !have_flag(f_ptr->flags, FF_TREE))) ?
			           "��" : "��");
#else
			msg_format("This monster is %s the %s.",
			           ((!have_flag(f_ptr->flags, FF_MOVE) && !have_flag(f_ptr->flags, FF_CAN_FLY)) ||
			            (!have_flag(f_ptr->flags, FF_LOS) && !have_flag(f_ptr->flags, FF_TREE))) ?
			           "in" : "on", f_name + f_ptr->name);
#endif

			return FALSE;
		}
		if (r_info[m_ptr->r_idx].level * 3 > randint1((p_ptr->skill_exp[GINOU_RIDING] / 50 + p_ptr->lev / 2 + 20))) /* #tang r_info[m_ptr->r_idx].level -> r_info[m_ptr->r_idx].level*3 */
		{
			msg_print(_("���ޤ����ʤ��ä���", "You failed to ride."));
			p_ptr->energy_use = 100;
			return FALSE;
		}

		if (MON_CSLEEP(m_ptr))
		{
			char m_name[80];
			monster_desc(m_name, m_ptr, 0);
			(void)set_monster_csleep(c_ptr->m_idx, 0);
			msg_format(_("%s�򵯤�������", "You have waked %s up."), m_name);
		}

		if (p_ptr->action == ACTION_KAMAE) set_action(ACTION_NONE);

		p_ptr->riding = c_ptr->m_idx;

		/* Hack -- remove tracked monster */
		if (p_ptr->riding == p_ptr->health_who) health_track(0);
	}

	p_ptr->energy_use = 100;

	/* Mega-Hack -- Forget the view and lite */
	p_ptr->update |= (PU_UN_VIEW | PU_UN_LITE);

	/* Update the monsters */
	p_ptr->update |= (PU_BONUS);

	/* Redraw map */
	p_ptr->redraw |= (PR_MAP | PR_EXTRA);

	p_ptr->redraw |= (PR_UHEALTH);

	/* Move the player */
	(void)move_player_effect(y, x, MPE_HANDLE_STUFF | MPE_ENERGY_USE | MPE_DONT_PICKUP | MPE_DONT_SWAP_MON);

	return TRUE;
}

/*!
 * @brief �ڥåȤ�̾����Ĥ��륳�ޥ�ɤΥᥤ��롼����
 * @return �ʤ�
 */
static void do_name_pet(void)
{
	monster_type *m_ptr;
	char out_val[20];
	char m_name[80];
	bool old_name = FALSE;
	bool old_target_pet = target_pet;

	target_pet = TRUE;
	if (!target_set(TARGET_KILL))
	{
		target_pet = old_target_pet;
		return;
	}
	target_pet = old_target_pet;

	if (cave[target_row][target_col].m_idx)
	{
		m_ptr = &m_list[cave[target_row][target_col].m_idx];

		if (!is_pet(m_ptr))
		{
			/* Message */
			msg_print(_("���Υ�󥹥����ϥڥåȤǤϤʤ���", "This monster is not a pet."));
			return;
		}
		if (r_info[m_ptr->r_idx].flags1 & RF1_UNIQUE)
		{
			msg_print(_("���Υ�󥹥�����̾�����Ѥ����ʤ���", "You cannot change name of this monster!"));
			return;
		}
		monster_desc(m_name, m_ptr, 0);

		/* Message */
		msg_format(_("%s��̾����Ĥ��롣", "Name %s."), m_name);
		msg_print(NULL);

		/* Start with nothing */
		strcpy(out_val, "");

		/* Use old inscription */
		if (m_ptr->nickname)
		{
			/* Start with the old inscription */
			strcpy(out_val, quark_str(m_ptr->nickname));
			old_name = TRUE;
		}

		/* Get a new inscription (possibly empty) */
		if (get_string(_("̾��: ", "Name: "), out_val, 15))
		{
			if (out_val[0])
			{
				/* Save the inscription */
				m_ptr->nickname = quark_add(out_val);
				if (record_named_pet)
				{
					monster_desc(m_name, m_ptr, MD_INDEF_VISIBLE);
					do_cmd_write_nikki(NIKKI_NAMED_PET, RECORD_NAMED_PET_NAME, m_name);
				}
			}
			else
			{
				if (record_named_pet && old_name)
				{
					monster_desc(m_name, m_ptr, MD_INDEF_VISIBLE);
					do_cmd_write_nikki(NIKKI_NAMED_PET, RECORD_NAMED_PET_UNNAME, m_name);
				}
				m_ptr->nickname = 0;
			}
		}
	}
}


/*!
 * @brief �ڥåȤ˴ؤ��륳�ޥ�ɥꥹ�ȤΥᥤ��롼���� /
 * Issue a pet command
 * @return �ʤ�
 */
void do_cmd_pet(void)
{
	COMMAND_CODE i = 0;
	int			num;
	int			powers[36];
	cptr			power_desc[36];
	bool			flag, redraw;
	char			choice;
	char			out_val[160];
	int			pet_ctr;
	monster_type	*m_ptr;

	PET_COMMAND_IDX mode = 0;

	char buf[160];
	char target_buf[160];

	int menu_line = use_menu ? 1 : 0;

	num = 0;

	power_desc[num] = _("�ڥåȤ�����", "dismiss pets");
	powers[num++] = PET_DISMISS;

#ifdef JP
	sprintf(target_buf, "�ڥåȤΥ������åȤ���� (���ߡ�%s)",
		(pet_t_m_idx ? (p_ptr->image ? "������̯��ʪ" : (r_name + r_info[m_list[pet_t_m_idx].ap_r_idx].name)) : "����ʤ�"));
#else
	sprintf(target_buf, "specify a target of pet (now:%s)",
		(pet_t_m_idx ? (p_ptr->image ? "something strange" : (r_name + r_info[m_list[pet_t_m_idx].ap_r_idx].name)) : "nothing"));
#endif
	power_desc[num] = target_buf;
	powers[num++] = PET_TARGET;
	power_desc[num] = _("�᤯�ˤ���", "stay close");

	if (p_ptr->pet_follow_distance == PET_CLOSE_DIST) mode = num;
	powers[num++] = PET_STAY_CLOSE;
	power_desc[num] = _("�Ĥ����褤", "follow me");

	if (p_ptr->pet_follow_distance == PET_FOLLOW_DIST) mode = num;
	powers[num++] = PET_FOLLOW_ME;
	power_desc[num] = _("Ũ�򸫤Ĥ����ݤ�", "seek and destroy");

	if (p_ptr->pet_follow_distance == PET_DESTROY_DIST) mode = num;
	powers[num++] = PET_SEEK_AND_DESTROY;
	power_desc[num] = _("����Υ��Ƥ���", "give me space");

	if (p_ptr->pet_follow_distance == PET_SPACE_DIST) mode = num;
	powers[num++] = PET_ALLOW_SPACE;
	power_desc[num] = _("Υ��Ƥ���", "stay away");

	if (p_ptr->pet_follow_distance == PET_AWAY_DIST) mode = num;
	powers[num++] = PET_STAY_AWAY;

	if (p_ptr->pet_extra_flags & PF_OPEN_DOORS)
	{
		power_desc[num] = _("�ɥ��򳫤��� (����:ON)", "pets open doors (now On)");
	}
	else
	{
		power_desc[num] = _("�ɥ��򳫤��� (����:OFF)", "pets open doors (now Off)");
	}
	powers[num++] = PET_OPEN_DOORS;

	if (p_ptr->pet_extra_flags & PF_PICKUP_ITEMS)
	{
		power_desc[num] = _("�����ƥ�򽦤� (����:ON)", "pets pick up items (now On)");
	}
	else
	{
		power_desc[num] = _("�����ƥ�򽦤� (����:OFF)", "pets pick up items (now Off)");
	}
	powers[num++] = PET_TAKE_ITEMS;

	if (p_ptr->pet_extra_flags & PF_TELEPORT)
	{
		power_desc[num] = _("�ƥ�ݡ��ȷ���ˡ��Ȥ� (����:ON)", "allow teleport (now On)");
	}
	else
	{
		power_desc[num] = _("�ƥ�ݡ��ȷ���ˡ��Ȥ� (����:OFF)", "allow teleport (now Off)");
	}
	powers[num++] = PET_TELEPORT;

	if (p_ptr->pet_extra_flags & PF_ATTACK_SPELL)
	{
		power_desc[num] = _("������ˡ��Ȥ� (����:ON)", "allow cast attack spell (now On)");
	}
	else
	{
		power_desc[num] = _("������ˡ��Ȥ� (����:OFF)", "allow cast attack spell (now Off)");
	}
	powers[num++] = PET_ATTACK_SPELL;

	if (p_ptr->pet_extra_flags & PF_SUMMON_SPELL)
	{
		power_desc[num] = _("������ˡ��Ȥ� (����:ON)", "allow cast summon spell (now On)");
	}
	else
	{
		power_desc[num] = _("������ˡ��Ȥ� (����:OFF)", "allow cast summon spell (now Off)");
	}
	powers[num++] = PET_SUMMON_SPELL;

	if (p_ptr->pet_extra_flags & PF_BALL_SPELL)
	{
		power_desc[num] = _("�ץ쥤�䡼�򴬤������ϰ���ˡ��Ȥ� (����:ON)", "allow involve player in area spell (now On)");
	}
	else
	{
		power_desc[num] = _("�ץ쥤�䡼�򴬤������ϰ���ˡ��Ȥ� (����:OFF)", "allow involve player in area spell (now Off)");
	}
	powers[num++] = PET_BALL_SPELL;

	if (p_ptr->riding)
	{
		power_desc[num] = _("�ڥåȤ���ߤ��", "get off a pet");
	}
	else
	{
		power_desc[num] = _("�ڥåȤ˾��", "ride a pet");
	}
	powers[num++] = PET_RIDING;
	power_desc[num] = _("�ڥåȤ�̾����Ĥ���", "name pets");
	powers[num++] = PET_NAME;

	if (p_ptr->riding)
	{
		if ((p_ptr->migite && (empty_hands(FALSE) == EMPTY_HAND_LARM) &&
		     object_allow_two_hands_wielding(&inventory[INVEN_RARM])) ||
		    (p_ptr->hidarite && (empty_hands(FALSE) == EMPTY_HAND_RARM) &&
			 object_allow_two_hands_wielding(&inventory[INVEN_LARM])))
		{
			if (p_ptr->pet_extra_flags & PF_RYOUTE)
			{
				power_desc[num] = _("�����Ҽ�ǻ���", "use one hand to control a riding pet");
			}
			else
			{
				power_desc[num] = _("����ξ��ǻ���", "use both hands for a weapon");
			}

			powers[num++] = PET_RYOUTE;
		}
		else
		{
			switch (p_ptr->pclass)
			{
			case CLASS_MONK:
			case CLASS_FORCETRAINER:
			case CLASS_BERSERKER:
				if (empty_hands(FALSE) == (EMPTY_HAND_RARM | EMPTY_HAND_LARM))
				{
					if (p_ptr->pet_extra_flags & PF_RYOUTE)
					{
						power_desc[num] = _("�Ҽ�ǳ�Ʈ����", "use one hand to control a riding pet");
					}
					else
					{
						power_desc[num] = _("ξ��ǳ�Ʈ����", "use both hands for melee");
					}

					powers[num++] = PET_RYOUTE;
				}
				else if ((empty_hands(FALSE) != EMPTY_HAND_NONE) && !buki_motteruka(INVEN_RARM) && !buki_motteruka(INVEN_LARM))
				{
					if (p_ptr->pet_extra_flags & PF_RYOUTE)
					{
						power_desc[num] = _("��Ʈ��Ԥ�ʤ�", "use one hand to control a riding pet");
					}
					else
					{
						power_desc[num] = _("��Ʈ��Ԥ�", "use one hand for melee");
					}

					powers[num++] = PET_RYOUTE;
				}
				break;
			}
		}
	}

#ifdef ALLOW_REPEAT
	if (!(repeat_pull(&i) && (i >= 0) && (i < num)))
	{
#endif /* ALLOW_REPEAT */

	/* Nothing chosen yet */
	flag = FALSE;

	/* No redraw yet */
	redraw = FALSE;

	if (use_menu)
	{
		/* Save the screen */
		screen_save();

		/* Build a prompt */
		strnfmt(out_val, 78, _("(���ޥ�ɡ�ESC=��λ) ���ޥ�ɤ�����Ǥ�������:", "(Command, ESC=exit) Choose command from menu."));
	}
	else
	{
		/* Build a prompt */
		strnfmt(out_val, 78,
		        _("(���ޥ�� %c-%c��'*'=������ESC=��λ) ���ޥ�ɤ�����Ǥ�������:", "(Command %c-%c, *=List, ESC=exit) Select a command: "),
		        I2A(0), I2A(num - 1));
	}

	choice = (always_show_list || use_menu) ? ESCAPE : 1;

	/* Get a command from the user */
	while (!flag)
	{
		int ask = TRUE;

		if (choice == ESCAPE) choice = ' ';
		else if (!get_com(out_val, &choice, TRUE)) break;

		if (use_menu && (choice != ' '))
		{
			switch (choice)
			{
			case '0':
				screen_load();
				return;

			case '8':
			case 'k':
			case 'K':
				menu_line += (num - 1);
				break;

			case '2':
			case 'j':
			case 'J':
				menu_line++;
				break;

			case '4':
			case 'h':
			case 'H':
				menu_line = 1;
				break;

			case '6':
			case 'l':
			case 'L':
				menu_line = num;
				break;

			case 'x':
			case 'X':
			case '\r':
			case '\n':
				i = menu_line - 1;
				ask = FALSE;
				break;
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
				PET_COMMAND_IDX ctr = 0;

				/* Show list */
				redraw = TRUE;

				/* Save the screen */
				if (!use_menu) screen_save();

				prt("", y++, x);

				/* Print list */
				for (ctr = 0; ctr < num; ctr++)
				{
					/* Letter/number for power selection */
					if (use_menu) 
						sprintf(buf, "%c%s ", (ctr == mode) ? '*' : ' ', (ctr == (menu_line - 1)) ? _("��", "> ") : "  ");
					else 
						sprintf(buf, "%c%c) ", (ctr == mode) ? '*' : ' ', I2A(ctr));

					strcat(buf, power_desc[ctr]);

					prt(buf, y + ctr, x);
				}

				prt("", y + MIN(ctr, 17), x);
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
			/* Note verify */
			ask = (isupper(choice));

			/* Lowercase */
			if (ask) choice = (char)tolower(choice);

			/* Extract request */
			i = (islower(choice) ? A2I(choice) : -1);
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
			/* Prompt */
			strnfmt(buf, 78, _("%s��Ȥ��ޤ����� ", "Use %s? "), power_desc[i]);

			/* Belay that order */
			if (!get_check(buf)) continue;
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
	}
#endif /* ALLOW_REPEAT */

	switch (powers[i])
	{
		case PET_DISMISS: /* Dismiss pets */
		{
			/* Check pets (backwards) */
			for (pet_ctr = m_max - 1; pet_ctr >= 1; pet_ctr--)
			{
				/* Player has pet */
				if (is_pet(&m_list[pet_ctr])) break;
			}

			if (!pet_ctr)
			{
				msg_print(_("�ڥåȤ����ʤ���", "You have no pets!"));
				break;
			}
			do_cmd_pet_dismiss();
			(void)calculate_upkeep();
			break;
		}
		case PET_TARGET:
		{
			project_length = -1;
			if (!target_set(TARGET_KILL)) pet_t_m_idx = 0;
			else
			{
				cave_type *c_ptr = &cave[target_row][target_col];
				if (c_ptr->m_idx && (m_list[c_ptr->m_idx].ml))
				{
					pet_t_m_idx = cave[target_row][target_col].m_idx;
					p_ptr->pet_follow_distance = PET_DESTROY_DIST;
				}
				else pet_t_m_idx = 0;
			}
			project_length = 0;

			break;
		}
		/* Call pets */
		case PET_STAY_CLOSE:
		{
			p_ptr->pet_follow_distance = PET_CLOSE_DIST;
			pet_t_m_idx = 0;
			break;
		}
		/* "Follow Me" */
		case PET_FOLLOW_ME:
		{
			p_ptr->pet_follow_distance = PET_FOLLOW_DIST;
			pet_t_m_idx = 0;
			break;
		}
		/* "Seek and destoy" */
		case PET_SEEK_AND_DESTROY:
		{
			p_ptr->pet_follow_distance = PET_DESTROY_DIST;
			break;
		}
		/* "Give me space" */
		case PET_ALLOW_SPACE:
		{
			p_ptr->pet_follow_distance = PET_SPACE_DIST;
			break;
		}
		/* "Stay away" */
		case PET_STAY_AWAY:
		{
			p_ptr->pet_follow_distance = PET_AWAY_DIST;
			break;
		}
		/* flag - allow pets to open doors */
		case PET_OPEN_DOORS:
		{
			if (p_ptr->pet_extra_flags & PF_OPEN_DOORS) p_ptr->pet_extra_flags &= ~(PF_OPEN_DOORS);
			else p_ptr->pet_extra_flags |= (PF_OPEN_DOORS);
			break;
		}
		/* flag - allow pets to pickup items */
		case PET_TAKE_ITEMS:
		{
			if (p_ptr->pet_extra_flags & PF_PICKUP_ITEMS)
			{
				p_ptr->pet_extra_flags &= ~(PF_PICKUP_ITEMS);
				for (pet_ctr = m_max - 1; pet_ctr >= 1; pet_ctr--)
				{
					/* Access the monster */
					m_ptr = &m_list[pet_ctr];

					if (is_pet(m_ptr))
					{
						monster_drop_carried_objects(m_ptr);
					}
				}
			}
			else p_ptr->pet_extra_flags |= (PF_PICKUP_ITEMS);

			break;
		}
		/* flag - allow pets to teleport */
		case PET_TELEPORT:
		{
			if (p_ptr->pet_extra_flags & PF_TELEPORT) p_ptr->pet_extra_flags &= ~(PF_TELEPORT);
			else p_ptr->pet_extra_flags |= (PF_TELEPORT);
			break;
		}
		/* flag - allow pets to cast attack spell */
		case PET_ATTACK_SPELL:
		{
			if (p_ptr->pet_extra_flags & PF_ATTACK_SPELL) p_ptr->pet_extra_flags &= ~(PF_ATTACK_SPELL);
			else p_ptr->pet_extra_flags |= (PF_ATTACK_SPELL);
			break;
		}
		/* flag - allow pets to cast attack spell */
		case PET_SUMMON_SPELL:
		{
			if (p_ptr->pet_extra_flags & PF_SUMMON_SPELL) p_ptr->pet_extra_flags &= ~(PF_SUMMON_SPELL);
			else p_ptr->pet_extra_flags |= (PF_SUMMON_SPELL);
			break;
		}
		/* flag - allow pets to cast attack spell */
		case PET_BALL_SPELL:
		{
			if (p_ptr->pet_extra_flags & PF_BALL_SPELL) p_ptr->pet_extra_flags &= ~(PF_BALL_SPELL);
			else p_ptr->pet_extra_flags |= (PF_BALL_SPELL);
			break;
		}

		case PET_RIDING:
		{
			(void)do_riding(FALSE);
			break;
		}

		case PET_NAME:
		{
			do_name_pet();
			break;
		}

		case PET_RYOUTE:
		{
			if (p_ptr->pet_extra_flags & PF_RYOUTE) p_ptr->pet_extra_flags &= ~(PF_RYOUTE);
			else p_ptr->pet_extra_flags |= (PF_RYOUTE);
			p_ptr->update |= (PU_BONUS);
			handle_stuff();
			break;
		}
	}
}
