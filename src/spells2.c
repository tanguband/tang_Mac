/*!
 * @file spells2.c
 * @brief ��ˡ���̤μ���/ Spell code (part 2)
 * @date 2014/07/15
 * @author
 * <pre>
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.  Other copyrights may also apply.
 * </pre>
 */

#include "angband.h"
#include "grid.h"

/*!
 * @brief ����ʬ�Ͻ���(Nethack����Υ����ǥ�) / self-knowledge... idea from nethack.
 * @return �ʤ�
 * @details
 * <pre>
 * Useful for determining powers and
 * resistences of items.  It saves the screen, clears it, then starts listing
 * attributes, a screenful at a time.  (There are a LOT of attributes to
 * list.  It will probably take 2 or 3 screens for a powerful character whose
 * using several artifacts...) -CFT
 *
 * It is now a lot more efficient. -BEN-
 *
 * See also "identify_fully()".
 *
 * XXX XXX XXX Use the "show_file()" method, perhaps.
 * </pre>
 */
void self_knowledge(void)
{
	int i = 0, j, k;

	int v_nr = 0;
	char v_string [8] [128];
	char s_string [6] [128];

	u32b flgs[TR_FLAG_SIZE];

	object_type *o_ptr;

	char Dummy[80];
	char buf[2][80];

	cptr info[220];

	int plev = p_ptr->lev;

	int percent;

	for (j = 0; j < TR_FLAG_SIZE; j++)
		flgs[j] = 0L;

	p_ptr->knowledge |= (KNOW_STAT | KNOW_HPRATE);

	strcpy(Dummy, "");

	percent = (int)(((long)p_ptr->player_hp[PY_MAX_LEVEL - 1] * 200L) /
		(2 * p_ptr->hitdie +
		((PY_MAX_LEVEL - 1+3) * (p_ptr->hitdie + 1))));

	sprintf(Dummy, _("���ߤ����ϥ�� : %d/100", "Your current Life Rating is %d/100."), percent);
	strcpy(buf[0], Dummy);
	info[i++] = buf[0];
	info[i++] = "";

	chg_virtue(V_KNOWLEDGE, 1);
	chg_virtue(V_ENLIGHTEN, 1);

	/* Acquire item flags from equipment */
	for (k = INVEN_RARM; k < INVEN_TOTAL; k++)
	{
		u32b tflgs[TR_FLAG_SIZE];

		o_ptr = &inventory[k];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Extract the flags */
		object_flags(o_ptr, tflgs);

		/* Extract flags */
		for (j = 0; j < TR_FLAG_SIZE; j++)
			flgs[j] |= tflgs[j];
	}

	info[i++] = _("ǽ�Ϥκ�����", "Limits of maximum stats");

	for (v_nr = 0; v_nr < 6; v_nr++)
	{
		char stat_desc[80];

		sprintf(stat_desc, "%s 18/%d", stat_names[v_nr], p_ptr->stat_max_max[v_nr]-18);

		strcpy(s_string[v_nr], stat_desc);

		info[i++] = s_string[v_nr];
	}
	info[i++] = "";

	sprintf(Dummy, _("���ߤ�°�� : %s(%ld)", "Your alighnment : %s(%ld)"), your_alignment(), (long int)p_ptr->align);
	strcpy(buf[1], Dummy);
	info[i++] = buf[1];
	for (v_nr = 0; v_nr < 8; v_nr++)
	{
		char v_name [20];
		char vir_desc[80];
		int tester = p_ptr->virtues[v_nr];
	
		strcpy(v_name, virtue[(p_ptr->vir_types[v_nr])-1]);
 
		sprintf(vir_desc, _("���äȡ�%s�ξ���ʤ���", "Oops. No info about %s."), v_name);
		if (tester < -100)
			sprintf(vir_desc, _("[%s]���ж� (%d)", "You are the polar opposite of %s (%d)."),
				v_name, tester);
		else if (tester < -80)
			sprintf(vir_desc, _("[%s]����Ũ (%d)", "You are an arch-enemy of %s (%d)."),
				v_name, tester);
		else if (tester < -60)
			sprintf(vir_desc, _("[%s]�ζ�Ũ (%d)", "You are a bitter enemy of %s (%d)."),
				v_name, tester);
		else if (tester < -40)
			sprintf(vir_desc, _("[%s]��Ũ (%d)", "You are an enemy of %s (%d)."),
				v_name, tester);
		else if (tester < -20)
			sprintf(vir_desc, _("[%s]�κ�� (%d)", "You have sinned against %s (%d)."),
				v_name, tester);
		else if (tester < 0)
			sprintf(vir_desc, _("[%s]����ƻ�� (%d)", "You have strayed from the path of %s (%d)."),
				v_name, tester);
		else if (tester == 0)
			sprintf(vir_desc, _("[%s]����Ω�� (%d)", "You are neutral to %s (%d)."),
				v_name, tester);
		else if (tester < 20)
			sprintf(vir_desc, _("[%s]�ξ����� (%d)", "You are somewhat virtuous in %s (%d)."),
				v_name, tester);
		else if (tester < 40)
			sprintf(vir_desc, _("[%s]�������� (%d)", "You are virtuous in %s (%d)."),
				v_name, tester);
		else if (tester < 60)
			sprintf(vir_desc, _("[%s]�ι����� (%d)", "You are very virtuous in %s (%d)."),
				v_name, tester);
		else if (tester < 80)
			sprintf(vir_desc, _("[%s]���Ƽ� (%d)", "You are a champion of %s (%d)."),
				v_name, tester);
		else if (tester < 100)
			sprintf(vir_desc, _("[%s]�ΰ�����Ƽ� (%d)", "You are a great champion of %s (%d)."),
				v_name, tester);
		else
			sprintf(vir_desc, _("[%s]�ζ񸽼� (%d)", "You are the living embodiment of %s (%d)."),
		v_name, tester);
	
		strcpy(v_string[v_nr], vir_desc);
	
		info[i++] = v_string[v_nr];
	}
	info[i++] = "";
	
	/* Racial powers... */
	if (p_ptr->mimic_form)
	{
		switch (p_ptr->mimic_form)
		{
			case MIMIC_DEMON:
			case MIMIC_DEMON_LORD:
			sprintf(Dummy, _("���ʤ��� %d ���᡼�����Ϲ����б�Υ֥쥹���Ǥ����Ȥ��Ǥ��롣(%d MP)", 
							"You can nether breathe, dam. %d (cost %d)."), 3 * plev, 10+plev/3);

				info[i++] = Dummy;
			break;
		case MIMIC_VAMPIRE:
			if (plev > 1)
			{
				sprintf(Dummy, _("���ʤ���Ũ���� %d-%d HP ����̿�Ϥ�ۼ��Ǥ��롣(%d MP)", 
								"You can steal life from a foe, dam. %d-%d (cost %d)."),
				    plev + MAX(1, plev / 10), plev + plev * MAX(1, plev / 10), 1 + (plev / 3));
				info[i++] = Dummy;
			}
			break;
		}
	}
	else
	{
	switch (p_ptr->prace)
	{
		case RACE_NIBELUNG:
		case RACE_DWARF:
			if (plev > 4)
			info[i++] = _("���ʤ���櫤ȥɥ��ȳ��ʤ��ΤǤ��롣(5 MP)", "You can find traps, doors and stairs (cost 5).");
			break;
		case RACE_HOBBIT:
			if (plev > 14)
			{
				info[i++] = _("���ʤ��Ͽ����������Ǥ��롣(10 MP)", "You can produce food (cost 10).");
			}
			break;
		case RACE_GNOME:
			if (plev > 4)
			{
				sprintf(Dummy, _("���ʤ����ϰ� %d ����˥ƥ�ݡ��ȤǤ��롣(%d MP)", "You can teleport, range %d (cost %d)."),
				    (1 + plev), (5 + (plev / 5)));
				info[i++] = Dummy;
			}
			break;
		case RACE_HALF_ORC:
			if (plev > 2)
				info[i++] = _("���ʤ��϶��ݤ����Ǥ��롣(5 MP)", "You can remove fear (cost 5).");
			break;
		case RACE_HALF_TROLL:
			if (plev > 9)
				info[i++] = _("���ʤ��϶�˽�����뤳�Ȥ��Ǥ��롣(12 MP) ", "You enter berserk fury (cost 12).");
			break;
		case RACE_AMBERITE:
			if (plev > 29)
				info[i++] = _("���ʤ��ϥ���ɥ����եȤ��뤳�Ȥ��Ǥ��롣(50 MP)", "You can Shift Shadows (cost 50).");

			if (plev > 39)
				info[i++] = _("���ʤ��ϡ֥ѥ�����פ򿴤��������⤯���Ȥ��Ǥ��롣(75 MP)", "You can mentally Walk the Pattern (cost 75).");

			break;
		case RACE_BARBARIAN:
			if (plev > 7)
				info[i++] = _("���ʤ��϶�˽�����뤳�Ȥ��Ǥ��롣(10 MP) ", "You can enter berserk fury (cost 10).");

			break;
		case RACE_HALF_OGRE:
			if (plev > 24)
				info[i++] = _("���ʤ�����ȯ�Υ롼���ųݤ��뤳�Ȥ��Ǥ��롣(35 MP)", "You can set an Explosive Rune (cost 35).");

			break;
		case RACE_HALF_GIANT:
			if (plev > 19)
				info[i++] = _("���ʤ����Ф��ɤ�������Ȥ��Ǥ��롣(10 MP)", "You can break stone walls (cost 10).");
			break;
		case RACE_HALF_TITAN:
			if (plev > 34)
				info[i++] = _("���ʤ��ϥ�󥹥����򥹥���󤹤뤳�Ȥ��Ǥ��롣(20 MP)", "You can probe monsters (cost 20).");
			break;
		case RACE_CYCLOPS:
			if (plev > 19)
			{
				sprintf(Dummy, _("���ʤ��� %d ���᡼���δ��Ф��ꤲ�뤳�Ȥ��Ǥ��롣(15 MP)", 
							 "You can throw a boulder, dam. %d (cost 15)."), 3 * plev);
				info[i++] = Dummy;
			}
			break;
		case RACE_YEEK:
			if (plev > 14)
				info[i++] = _("���ʤ��϶��ݤ�Ƥӵ�������������ȯ���뤳�Ȥ��Ǥ��롣(15 MP)", "You can make a terrifying scream (cost 15).");
			break;
		case RACE_KLACKON:
			if (plev > 8)
			{
				sprintf(Dummy, _("���ʤ��� %d ���᡼���λ���᤭�����뤳�Ȥ��Ǥ��롣(9 MP)", "You can spit acid, dam. %d (cost 9)."), plev);
				info[i++] = Dummy;
			}
			break;
		case RACE_KOBOLD:
			if (plev > 11)
			{
				sprintf(Dummy,
						_("���ʤ��� %d ���᡼����������ꤲ�뤳�Ȥ��Ǥ��롣(8 MP)", "You can throw a dart of poison, dam. %d (cost 8)."), plev);
				info[i++] = Dummy;
			}
			break;
		case RACE_DARK_ELF:
			if (plev > 1)
			{
				sprintf(Dummy, _("���ʤ��� %d ���᡼���Υޥ��å����ߥ�����μ�ʸ��Ȥ��롣(2 MP)", "You can cast a Magic Missile, dam %d (cost 2)."),
				    (3 + ((plev-1) / 5)));
				info[i++] = Dummy;
			}
			break;
		case RACE_DRACONIAN:
			sprintf(Dummy, _("���ʤ��� %d ���᡼���Υ֥쥹���Ǥ����Ȥ��Ǥ��롣(%d MP)", "You can breathe, dam. %d (cost %d)."), 2 * plev, plev);
			info[i++] = Dummy;
			break;
		case RACE_MIND_FLAYER:
			if (plev > 14)
				sprintf(Dummy, _("���ʤ��� %d ���᡼������������򤹤뤳�Ȥ��Ǥ��롣(12 MP)", "You can mind blast your enemies, dam %d (cost 12)."), plev);
			info[i++] = Dummy;
			break;
		case RACE_IMP:
			if (plev > 29)
			{
				sprintf(Dummy, _("���ʤ��� %d ���᡼���Υե��������ܡ���μ�ʸ��Ȥ��롣(15 MP)", "You can cast a Fire Ball, dam. %d (cost 15)."), plev);
				info[i++] = Dummy;
			}
			else if (plev > 8)
			{
				sprintf(Dummy, _("���ʤ��� %d ���᡼���Υե��������ܥ�Ȥμ�ʸ��Ȥ��롣(15 MP)", "You can cast a Fire Bolt, dam. %d (cost 15)."), plev);
				info[i++] = Dummy;
			}
			break;
		case RACE_GOLEM:
			if (plev > 19)
				info[i++] = _("���ʤ��� d20+30 ������δ�ȩ���Ф��Ѳ��������롣(15 MP)", "You can turn your skin to stone, dur d20+30 (cost 15).");
			break;
		case RACE_ZOMBIE:
		case RACE_SKELETON:
			if (plev > 29)
				info[i++] = _("���ʤ��ϼ��ä��и��ͤ�������뤳�Ȥ��Ǥ��롣(30 MP)", "You can restore lost experience (cost 30).");
			break;
		case RACE_VAMPIRE:
			if (plev > 1)
			{
				sprintf(Dummy, _("���ʤ���Ũ���� %d-%d HP ����̿�Ϥ�ۼ��Ǥ��롣(%d MP)", "You can steal life from a foe, dam. %d-%d (cost %d)."),
				    plev + MAX(1, plev / 10), plev + plev * MAX(1, plev / 10), 1 + (plev / 3));
				info[i++] = Dummy;
			}
			break;
		case RACE_SPECTRE:
			if (plev > 3)
			{
				info[i++] = _("���ʤ��ϵ㤭�����Ũ���ݤ����뤳�Ȥ��Ǥ��롣(3 MP)", "You can wail to terrify your enemies (cost 3).");
			}
			break;
		case RACE_SPRITE:
			if (plev > 11)
			{
				info[i++] = _("���ʤ���Ũ��̲�餻����ˡ��ʴ���ꤲ�뤳�Ȥ��Ǥ��롣(12 MP)", "You can throw magical dust which induces sleep (cost 12).");
			}
			break;
		case RACE_DEMON:
			sprintf(Dummy, _("���ʤ��� %d ���᡼�����Ϲ����б�Υ֥쥹���Ǥ����Ȥ��Ǥ��롣(%d MP)", 
						"You can breathe nether, dam. %d (cost %d)."), 3 * plev, 10+plev/3);

			info[i++] = Dummy;
			break;
		case RACE_KUTAR:
			if (plev > 19)
				info[i++] = _("���ʤ��� d20+30 ������δֲ��˿��Ӥ뤳�Ȥ��Ǥ��롣(15 MP)", "You can expand horizontally, dur d20+30 (cost 15).");
			break;
		case RACE_ANDROID:
			if (plev < 10)
				sprintf(Dummy, _("���ʤ��� %d ���᡼���Υ쥤������Ĥ��Ȥ��Ǥ��롣(7 MP)", "You can fire a ray gun with damage %d (cost 7)."), 
					(plev + 1) / 2);
			else if (plev < 25)
				sprintf(Dummy, _("���ʤ��� %d ���᡼���Υ֥饹�������Ĥ��Ȥ��Ǥ��롣(13 MP)", "You can fire a blaster with damage %d (cost 13)."), plev);
			else if (plev < 35)
				sprintf(Dummy, _("���ʤ��� %d ���᡼���ΥХ��������Ĥ��Ȥ��Ǥ��롣(26 MP)", "You can fire a bazooka with damage %d (cost 26)."), plev * 2);
			else if (plev < 45)
				sprintf(Dummy, _("���ʤ��� %d ���᡼���Υӡ��७��Υ���Ĥ��Ȥ��Ǥ��롣(40 MP)", 
						"You can fire a beam cannon with damage %d (cost 40)."), plev * 2);
			else
				sprintf(Dummy, _("���ʤ��� %d ���᡼���Υ��åȤ��Ĥ��Ȥ��Ǥ��롣(60 MP)", 
						"You can fire a rocket with damage %d (cost 60)."), plev * 5);

			info[i++] = Dummy;
			break;
		default:
			break;
	}
	}

	switch(p_ptr->pclass)
	{
		case CLASS_WARRIOR:
			if (plev > 39)
			{
				info[i++] = _("���ʤ��ϥ�������������Ф��ƿ��󹶷⤹�뤳�Ȥ��Ǥ��롣(75 MP)", 
							"You can attack some random directions at a time (cost 75).");
			}
			break;
		case CLASS_HIGH_MAGE:
			if (p_ptr->realm1 == REALM_HEX) break;
		case CLASS_MAGE:
		case CLASS_SORCERER:
			if (plev > 24)
			{
				info[i++] = _("���ʤ��ϥ����ƥ�����Ϥ�ۼ����뤳�Ȥ��Ǥ��롣(1 MP)", 
						"You can absorb charges from an item (cost 1).");
			}
			break;
		case CLASS_PRIEST:
			if (is_good_realm(p_ptr->realm1))
			{
				if (plev > 34)
				{
					info[i++] = _("���ʤ��������ʡ���뤳�Ȥ��Ǥ��롣(70 MP)", "You can bless a weapon (cost 70).");
				}
			}
			else
			{
				if (plev > 41)
				{
					info[i++] = _("���ʤ��ϼ���Τ��٤ƤΥ�󥹥����򹶷⤹�뤳�Ȥ��Ǥ��롣(40 MP)", "You can damages all monsters in sight (cost 40).");
				}
			}
			break;
		case CLASS_ROGUE:
			if (plev > 7)
			{
				info[i++] = _("���ʤ��Ϲ��⤷��¨�¤�ƨ���뤳�Ȥ��Ǥ��롣(12 MP)", "You can hit a monster and teleport at a time (cost 12).");
			}
			break;
		case CLASS_RANGER:
			if (plev > 14)
			{
				info[i++] = _("���ʤ��ϲ�ʪ��Ĵ�����뤳�Ȥ��Ǥ��롣(20 MP)", "You can prove monsters (cost 20).");
			}
			break;
		case CLASS_PALADIN:
			if (is_good_realm(p_ptr->realm1))
			{
				if (plev > 29)
				{
					info[i++] = _("���ʤ������ʤ�������Ĥ��Ȥ��Ǥ��롣(30 MP)", "You can fires a holy spear (cost 30).");
				}
			}
			else
			{
				if (plev > 29)
				{
					info[i++] = _("���ʤ�����̿�Ϥ򸺾�������������Ĥ��Ȥ��Ǥ��롣(30 MP)", "You can fires a spear which drains vitality (cost 30).");
				}
			}
			break;
		case CLASS_WARRIOR_MAGE:
			if (plev > 24)
			{
				info[i++] = _("���ʤ��ϣȣФ�ͣФ��Ѵ����뤳�Ȥ��Ǥ��롣(0 MP)", "You can convert HP to SP (cost 0).");
				info[i++] = _("���ʤ��ϣͣФ�ȣФ��Ѵ����뤳�Ȥ��Ǥ��롣(0 MP)", "You can convert SP to HP (cost 0).");
			}
			break;
		case CLASS_CHAOS_WARRIOR:
			if (plev > 39)
			{
				info[i++] = _("���ʤ��ϼ��Ϥ˲�ʪ���Ǥ魯����ȯ�������뤳�Ȥ��Ǥ��롣(50 MP)",
						"You can radiate light which confuses nearby monsters (cost 50).");
			}
			break;
		case CLASS_MONK:
			if (plev > 24)
			{
				info[i++] = _("���ʤ��Ϲ����뤳�Ȥ��Ǥ��롣(0 MP)", "You can assume a posture of special form (cost 0).");
			}
			if (plev > 29)
			{
				info[i++] = _("���ʤ����̾��2�ܤι����Ԥ����Ȥ��Ǥ��롣(30 MP)", "You can perform double attacks in a time (cost 30).");
			}
			break;
		case CLASS_MINDCRAFTER:
		case CLASS_FORCETRAINER:
			if (plev > 14)
			{
				info[i++] = _("���ʤ����������椷�ƣͣФ���������뤳�Ȥ��Ǥ��롣(0 MP)", "You can concentrate to regenerate your mana (cost 0).");
			}
			break;
		case CLASS_TOURIST:
			info[i++] = _("���ʤ��ϼ̿��򻣱Ƥ��뤳�Ȥ��Ǥ��롣(0 MP)", "You can take a photograph (cost 0).");
			if (plev > 24)
			{
				info[i++] = _("���ʤ��ϥ����ƥ�����˴��ꤹ�뤳�Ȥ��Ǥ��롣(20 MP)", "You can *identify* items (cost 20).");
			}
			break;
		case CLASS_IMITATOR:
			if (plev > 29)
			{
				info[i++] = _("���ʤ��ϲ�ʪ���ü칶�����᡼��2�ܤǤޤͤ뤳�Ȥ��Ǥ��롣(100 MP)", 
						"You can imitate monster's special attacks with double damage (cost 100).");
			}
			break;
		case CLASS_BEASTMASTER:
			info[i++] = _("���ʤ���1�Τ���̿�Τ����󥹥�������ۤ��뤳�Ȥ��Ǥ��롣(��٥�/4 MP)", "You can dominate a monster (cost level/4).");
			if (plev > 29)
			{
				info[i++] = _("���ʤ��ϻ볦�����̿�Τ����󥹥�������ۤ��뤳�Ȥ��Ǥ��롣((��٥�+20)/2 MP)", 
						"You can dominate living monsters in sight (cost (level+20)/4).");
			}
			break;
		case CLASS_MAGIC_EATER:
			info[i++] = _("���ʤ��Ͼ�/��ˡ��/��åɤ����Ϥ�ʬ�Τ�Τˤ��뤳�Ȥ��Ǥ��롣", "You can absorb a staff, wand or rod itself.");
			break;
		case CLASS_RED_MAGE:
			if (plev > 47)
			{
				info[i++] = _("���ʤ���1�������2����ˡ�򾧤��뤳�Ȥ��Ǥ��롣(20 MP)", "You can cast two spells in one time (cost 20).");
			}
			break;
		case CLASS_SAMURAI:
			{
				info[i++] = _("���ʤ����������椷�Ƶ��礤��ί��뤳�Ȥ��Ǥ��롣", "You can concentrate to regenerate your mana.");
			}
			if (plev > 24)
			{
				info[i++] = _("���ʤ����ü�ʷ��ǹ����뤳�Ȥ��Ǥ��롣", "You can assume a posture of special form.");
			}
			break;
		case CLASS_BLUE_MAGE:
			info[i++] = _("���ʤ������˻Ȥ�줿��ˡ��ؤ֤��Ȥ��Ǥ��롣", "You can study spells which your enemy casts on you.");
			break;
		case CLASS_CAVALRY:
			if (plev > 9)
			{
				info[i++] = _("���ʤ��ϥ�󥹥����˾�ä�̵�������ڥåȤˤ��뤳�Ȥ��Ǥ��롣", "You can ride on a hostile monster forcibly to turn it into pet.");
			}
			break;
		case CLASS_BERSERKER:
			if (plev > 9)
			{
				info[i++] = _("���ʤ��ϳ��ȥ��󥸥��δ֤�Ԥ��褹�뤳�Ȥ��Ǥ��롣", "You can travel between town and the depths.");
			}
			break;
		case CLASS_MIRROR_MASTER:
			info[i++] = _("���ʤ��϶�����Ф����Ȥ��Ǥ��롣(2 MP)", "You can create a Mirror (cost 2).");
			info[i++] = _("���ʤ��϶����뤳�Ȥ��Ǥ��롣(0 MP)", "You can break distant Mirrors (cost 0).");
			break;
		case CLASS_NINJA:
			if (plev > 19)
			{
				info[i++] = _("���ʤ������᤯��ư���뤳�Ȥ��Ǥ��롣", "You can walk extremery fast.");
			}
			break;
	}

	if (p_ptr->muta1)
	{
		if (p_ptr->muta1 & MUT1_SPIT_ACID)
		{
			info[i++] = _("���ʤ��ϻ���᤭�����뤳�Ȥ��Ǥ��롣(���᡼�� ��٥�X1)", "You can spit acid (dam lvl).");
		}
		if (p_ptr->muta1 & MUT1_BR_FIRE)
		{
			info[i++] = _("���ʤ��ϱ�Υ֥쥹���Ǥ����Ȥ��Ǥ��롣(���᡼�� ��٥�X2)", "You can breathe fire (dam lvl * 2).");
		}
		if (p_ptr->muta1 & MUT1_HYPN_GAZE)
		{
			info[i++] = _("���ʤ����ˤߤϺ�̲���̤��ġ�", "Your gaze is hypnotic.");
		}
		if (p_ptr->muta1 & MUT1_TELEKINES)
		{
			info[i++] = _("���ʤ���ǰư�Ϥ��äƤ��롣", "You are telekinetic.");
		}
		if (p_ptr->muta1 & MUT1_VTELEPORT)
		{
			info[i++] = _("���ʤ��ϼ�ʬ�ΰջפǥƥ�ݡ��ȤǤ��롣", "You can teleport at will.");
		}
		if (p_ptr->muta1 & MUT1_MIND_BLST)
		{
			info[i++] = _("���ʤ������������Ԥ��롣(���᡼�� 312d3)", "You can Mind Blast your enemies (3 to 12d3 dam).");
		}
		if (p_ptr->muta1 & MUT1_RADIATION)
		{
			info[i++] = _("���ʤ��ϼ�ʬ�ΰջפǶ�����������ȯ�����뤳�Ȥ��Ǥ��롣(���᡼�� ��٥�X2)", "You can emit hard radiation at will (dam lvl * 2).");
		}
		if (p_ptr->muta1 & MUT1_VAMPIRISM)
		{
			info[i++] = _("���ʤ��ϵ۷쵴�Τ褦��Ũ������̿�Ϥ�ۼ����뤳�Ȥ��Ǥ��롣(���᡼�� ��٥�X2)", 
						"You can drain life from a foe like a vampire (dam lvl * 2).");
		}
		if (p_ptr->muta1 & MUT1_SMELL_MET)
		{
			info[i++] = _("���ʤ��϶᤯�ˤ��뵮��°�򤫤�ʬ���뤳�Ȥ��Ǥ��롣", "You can smell nearby precious metal.");
		}
		if (p_ptr->muta1 & MUT1_SMELL_MON)
		{
			info[i++] = _("���ʤ��϶᤯�Υ�󥹥�����¸�ߤ򤫤�ʬ���뤳�Ȥ��Ǥ��롣", "You can smell nearby monsters.");
		}
		if (p_ptr->muta1 & MUT1_BLINK)
		{
			info[i++] = _("���ʤ���û����Υ��ƥ�ݡ��ȤǤ��롣", "You can teleport yourself short distances.");
		}
		if (p_ptr->muta1 & MUT1_EAT_ROCK)
		{
			info[i++] = _("���ʤ��ϹŤ���򿩤٤뤳�Ȥ��Ǥ��롣", "You can consume solid rock.");
		}
		if (p_ptr->muta1 & MUT1_SWAP_POS)
		{
			info[i++] = _("���ʤ���¾�μԤȾ��������ؤ�뤳�Ȥ��Ǥ��롣", "You can switch locations with another being.");
		}
		if (p_ptr->muta1 & MUT1_SHRIEK)
		{
			info[i++] = _("���ʤ��ϿȤ��Ӥ����Ķ�������ȯ���뤳�Ȥ��Ǥ��롣(���᡼�� ��٥�X2)", "You can emit a horrible shriek (dam 2 * lvl).");
		}
		if (p_ptr->muta1 & MUT1_ILLUMINE)
		{
			info[i++] = _("���ʤ������뤤�������Ĥ��Ȥ��Ǥ��롣", "You can emit bright light.");
		}
		if (p_ptr->muta1 & MUT1_DET_CURSE)
		{
			info[i++] = _("���ʤ��ϼٰ�����ˡ�δ��򴶤��Ȥ뤳�Ȥ��Ǥ��롣", "You can feel the danger of evil magic.");
		}
		if (p_ptr->muta1 & MUT1_BERSERK)
		{
			info[i++] = _("���ʤ��ϼ�ʬ�ΰջפǶ�����Ʈ���֤ˤʤ뤳�Ȥ��Ǥ��롣", "You can drive yourself into a berserk frenzy.");
		}
		if (p_ptr->muta1 & MUT1_POLYMORPH)
		{
			info[i++] = _("���ʤ��ϼ�ʬ�ΰջ֤��Ѳ��Ǥ��롣", "You can polymorph yourself at will.");
		}
		if (p_ptr->muta1 & MUT1_MIDAS_TCH)
		{
			info[i++] = _("���ʤ����̾異���ƥ�����Ѥ��뤳�Ȥ��Ǥ��롣", "You can turn ordinary items to gold.");
		}
		if (p_ptr->muta1 & MUT1_GROW_MOLD)
		{
			info[i++] = _("���ʤ��ϼ��Ϥ˥��Υ������䤹���Ȥ��Ǥ��롣", "You can cause mold to grow near you.");
		}
		if (p_ptr->muta1 & MUT1_RESIST)
		{
			info[i++] = _("���ʤ��ϸ��Ǥι�����Ф��ƿȤ�Ť����뤳�Ȥ��Ǥ��롣", "You can harden yourself to the ravages of the elements.");
		}
		if (p_ptr->muta1 & MUT1_EARTHQUAKE)
		{
			info[i++] = _("���ʤ��ϼ��ϤΥ��󥸥������������뤳�Ȥ��Ǥ��롣", "You can bring down the dungeon around your ears.");
		}
		if (p_ptr->muta1 & MUT1_EAT_MAGIC)
		{
			info[i++] = _("���ʤ�����ˡ�Υ��ͥ륮����ʬ��ʪ�Ȥ��ƻ��ѤǤ��롣", "You can consume magic energy for your own use.");
		}
		if (p_ptr->muta1 & MUT1_WEIGH_MAG)
		{
			info[i++] = _("���ʤ��ϼ�ʬ�˱ƶ���Ϳ������ˡ���Ϥ򴶤��뤳�Ȥ��Ǥ��롣", "You can feel the strength of the magics affecting you.");
		}
		if (p_ptr->muta1 & MUT1_STERILITY)
		{
			info[i++] = _("���ʤ��Ͻ���Ū������ǽ�򵯤������Ȥ��Ǥ��롣", "You can cause mass impotence.");
		}
		if (p_ptr->muta1 & MUT1_PANIC_HIT)
		{
			info[i++] = _("���ʤ��Ϲ��⤷����Ȥ��뤿��ƨ���뤳�Ȥ��Ǥ��롣", "You can run for your life after hitting something.");
		}
		if (p_ptr->muta1 & MUT1_DAZZLE)
		{
			info[i++] = _("���ʤ��Ϻ�������ܤ��������������ǽ��ȯ�����뤳�Ȥ��Ǥ��롣 ", "You can emit confusing, blinding radiation.");
		}
		if (p_ptr->muta1 & MUT1_LASER_EYE)
		{
			info[i++] = _("���ʤ����ܤ���졼����������ȯ���뤳�Ȥ��Ǥ��롣(���᡼�� ��٥�X2)", "Your eyes can fire laser beams (dam 2 * lvl).");
		}
		if (p_ptr->muta1 & MUT1_RECALL)
		{
			info[i++] = _("���ʤ��ϳ��ȥ��󥸥��δ֤�Ԥ��褹�뤳�Ȥ��Ǥ��롣", "You can travel between town and the depths.");
		}
		if (p_ptr->muta1 & MUT1_BANISH)
		{
			info[i++] = _("���ʤ��ϼٰ��ʥ�󥹥������Ϲ�����Ȥ����Ȥ��Ǥ��롣", "You can send evil creatures directly to Hell.");
		}
		if (p_ptr->muta1 & MUT1_COLD_TOUCH)
		{
			info[i++] = _("���ʤ���Ũ�򿨤ä���餻�뤳�Ȥ��Ǥ��롣(���᡼�� ��٥�X3)", "You can freeze things with a touch (dam 3 * lvl).");
		}
		if (p_ptr->muta1 & MUT1_LAUNCHER)
		{
			info[i++] = _("���ʤ��ϥ����ƥ���϶����ꤲ�뤳�Ȥ��Ǥ��롣", "You can hurl objects with great force.");
		}
	}

	if (p_ptr->muta2)
	{
		if (p_ptr->muta2 & MUT2_BERS_RAGE)
		{
			info[i++] = _("���ʤ��϶���β���ȯ��򵯤�����", "You are subject to berserker fits.");
		}
		if (p_ptr->muta2 & MUT2_COWARDICE)
		{
			info[i++] = _("���ʤ��ϻ������¤ˤʤ롣", "You are subject to cowardice.");
		}
		if (p_ptr->muta2 & MUT2_RTELEPORT)
		{
			info[i++] = _("���ʤ��ϥ�����˥ƥ�ݡ��Ȥ��롣", "You are teleporting randomly.");
		}
		if (p_ptr->muta2 & MUT2_ALCOHOL)
		{
			info[i++] = _("���ʤ����Τϥ��륳�����ʬ�礹�롣", "Your body produces alcohol.");
		}
		if (p_ptr->muta2 & MUT2_HALLU)
		{
			info[i++] = _("���ʤ��ϸ��Ф������������������˿�����Ƥ��롣", "You have a hallucinatory insanity.");
		}
		if (p_ptr->muta2 & MUT2_FLATULENT)
		{
			info[i++] = _("���ʤ�������Ǥ��ʤ����������򤳤���", "You are subject to uncontrollable flatulence.");
		}
		if (p_ptr->muta2 & MUT2_PROD_MANA)
		{
			info[i++] = _("���ʤ���������ǽ����ˡ�Υ��ͥ륮����ȯ���Ƥ��롣", "You are producing magical energy uncontrollably.");
		}
		if (p_ptr->muta2 & MUT2_ATT_DEMON)
		{
			info[i++] = _("���ʤ��ϥǡ���������Ĥ��롣", "You attract demons.");
		}
		if (p_ptr->muta2 & MUT2_SCOR_TAIL)
		{
			info[i++] = _("���ʤ��ϥ�����ο����������Ƥ��롣(�ǡ����᡼�� 3d7)", "You have a scorpion tail (poison, 3d7).");
		}
		if (p_ptr->muta2 & MUT2_HORNS)
		{
			info[i++] = _("���ʤ��ϳѤ������Ƥ��롣(���᡼�� 2d6)", "You have horns (dam. 2d6).");
		}
		if (p_ptr->muta2 & MUT2_BEAK)
		{
			info[i++] = _("���ʤ��ϥ����Х��������Ƥ��롣(���᡼�� 2d4)", "You have a beak (dam. 2d4).");
		}
		if (p_ptr->muta2 & MUT2_SPEED_FLUX)
		{
			info[i++] = _("���ʤ��ϥ�������᤯ư�������٤�ư�����ꤹ�롣", "You move faster or slower randomly.");
		}
		if (p_ptr->muta2 & MUT2_BANISH_ALL)
		{
			info[i++] = _("���ʤ��ϻ����᤯�Υ�󥹥�������Ǥ����롣", "You sometimes cause nearby creatures to vanish.");
		}
		if (p_ptr->muta2 & MUT2_EAT_LIGHT)
		{
			info[i++] = _("���ʤ��ϻ������Ϥθ���ۼ����Ʊ��ܤˤ��롣", "You sometimes feed off of the light around you.");
		}
		if (p_ptr->muta2 & MUT2_TRUNK)
		{
			info[i++] = _("���ʤ��ϾݤΤ褦��ɡ����äƤ��롣(���᡼�� 1d4)", "You have an elephantine trunk (dam 1d4).");
		}
		if (p_ptr->muta2 & MUT2_ATT_ANIMAL)
		{
			info[i++] = _("���ʤ���ưʪ������Ĥ��롣", "You attract animals.");
		}
		if (p_ptr->muta2 & MUT2_TENTACLES)
		{
			info[i++] = _("���ʤ��ϼٰ��ʿ������äƤ��롣(���᡼�� 2d5)", "You have evil looking tentacles (dam 2d5).");
		}
		if (p_ptr->muta2 & MUT2_RAW_CHAOS)
		{
			info[i++] = _("���ʤ��Ϥ��Ф��н㥫��������ޤ�롣", "You occasionally are surrounded with raw chaos.");
		}
		if (p_ptr->muta2 & MUT2_NORMALITY)
		{
			info[i++] = _("���ʤ����Ѱۤ��Ƥ��������������Ƥ��Ƥ��롣", "You may be mutated, but you're recovering.");
		}
		if (p_ptr->muta2 & MUT2_WRAITH)
		{
			info[i++] = _("���ʤ������Τ�ͩ�β���������β������ꤹ�롣", "You fade in and out of physical reality.");
		}
		if (p_ptr->muta2 & MUT2_POLY_WOUND)
		{
			info[i++] = _("���ʤ��η򹯤ϥ��������Ϥ˱ƶ�������롣", "Your health is subject to chaotic forces.");
		}
		if (p_ptr->muta2 & MUT2_WASTING)
		{
			info[i++] = _("���ʤ��Ͽ�夹�붲�����µ��ˤ����äƤ��롣", "You have a horrible wasting disease.");
		}
		if (p_ptr->muta2 & MUT2_ATT_DRAGON)
		{
			info[i++] = _("���ʤ��ϥɥ饴�������Ĥ��롣", "You attract dragons.");
		}
		if (p_ptr->muta2 & MUT2_WEIRD_MIND)
		{
			info[i++] = _("���ʤ��������ϥ�����˳��礷����̾������ꤷ�Ƥ��롣", "Your mind randomly expands and contracts.");
		}
		if (p_ptr->muta2 & MUT2_NAUSEA)
		{
			info[i++] = _("���ʤ��ΰߤ���������夭���ʤ���", "You have a seriously upset stomach.");
		}
		if (p_ptr->muta2 & MUT2_CHAOS_GIFT)
		{
			info[i++] = _("���ʤ��ϥ������μ��⤫��˫���򤦤��Ȥ롣", "Chaos deities give you gifts.");
		}
		if (p_ptr->muta2 & MUT2_WALK_SHAD)
		{
			info[i++] = _("���ʤ��Ϥ��Ф���¾�Ρֱơפ��¤����ࡣ", "You occasionally stumble into other shadows.");
		}
		if (p_ptr->muta2 & MUT2_WARNING)
		{
			info[i++] = _("���ʤ���Ũ�˴ؤ���ٹ�򴶤��롣", "You receive warnings about your foes.");
		}
		if (p_ptr->muta2 & MUT2_INVULN)
		{
			info[i++] = _("���ʤ��ϻ����餱�Τ餺�ʵ�ʬ�ˤʤ롣", "You occasionally feel invincible.");
		}
		if (p_ptr->muta2 & MUT2_SP_TO_HP)
		{
			info[i++] = _("���ʤ��ϻ����줬�����ˤɤä�ή��롣", "Your blood sometimes rushes to your muscles.");
		}
		if (p_ptr->muta2 & MUT2_HP_TO_SP)
		{
			info[i++] = _("���ʤ��ϻ���Ƭ�˷줬�ɤä�ή��롣", "Your blood sometimes rushes to your head.");
		}
		if (p_ptr->muta2 & MUT2_DISARM)
		{
			info[i++] = _("���ʤ��Ϥ褯�ĤޤŤ���ʪ����Ȥ���", "You occasionally stumble and drop things.");
		}
	}

	if (p_ptr->muta3)
	{
		if (p_ptr->muta3 & MUT3_HYPER_STR)
		{
			info[i++] = _("���ʤ���Ķ��Ū�˶�����(����+4)", "You are superhumanly strong (+4 STR).");
		}
		if (p_ptr->muta3 & MUT3_PUNY)
		{
			info[i++] = _("���ʤ��ϵ������(����-4)", "You are puny (-4 STR).");
		}
		if (p_ptr->muta3 & MUT3_HYPER_INT)
		{
			info[i++] = _("���ʤ���Ǿ�����Υ���ԥ塼������(��ǽ������+4)", "Your brain is a living computer (+4 INT/WIS).");
		}
		if (p_ptr->muta3 & MUT3_MORONIC)
		{
			info[i++] = _("���ʤ��������������(��ǽ������-4)", "You are moronic (-4 INT/WIS).");
		}
		if (p_ptr->muta3 & MUT3_RESILIENT)
		{
			info[i++] = _("���ʤ������˥��դ���(�ѵ�+4)", "You are very resilient (+4 CON).");
		}
		if (p_ptr->muta3 & MUT3_XTRA_FAT)
		{
			info[i++] = _("���ʤ��϶�ü�����äƤ��롣(�ѵ�+2,���ԡ���-2)", "You are extremely fat (+2 CON, -2 speed).");
		}
		if (p_ptr->muta3 & MUT3_ALBINO)
		{
			info[i++] = _("���ʤ��ϥ���ӥΤ���(�ѵ�-4)", "You are albino (-4 CON).");
		}
		if (p_ptr->muta3 & MUT3_FLESH_ROT)
		{
			info[i++] = _("���ʤ������Τ����Ԥ��Ƥ��롣(�ѵ�-2,̥��-1)", "Your flesh is rotting (-2 CON, -1 CHR).");
		}
		if (p_ptr->muta3 & MUT3_SILLY_VOI)
		{
			info[i++] = _("���ʤ������ϴ�ȴ���ʥ�������������(̥��-4)", "Your voice is a silly squeak (-4 CHR).");
		}
		if (p_ptr->muta3 & MUT3_BLANK_FAC)
		{
			info[i++] = _("���ʤ��ϤΤäڤ�ܤ�����(̥��-1)", "Your face is featureless (-1 CHR).");
		}
		if (p_ptr->muta3 & MUT3_ILL_NORM)
		{
			info[i++] = _("���ʤ��ϸ��Ƥ�ʤ���Ƥ��롣", "Your appearance is masked with illusion.");
		}
		if (p_ptr->muta3 & MUT3_XTRA_EYES)
		{
			info[i++] = _("���ʤ���;ʬ����Ĥ��ܤ���äƤ��롣(õ��+15)", "You have an extra pair of eyes (+15 search).");
		}
		if (p_ptr->muta3 & MUT3_MAGIC_RES)
		{
			info[i++] = _("���ʤ�����ˡ�ؤ��������äƤ��롣", "You are resistant to magic.");
		}
		if (p_ptr->muta3 & MUT3_XTRA_NOIS)
		{
			info[i++] = _("���ʤ����Ѥʲ���ȯ���Ƥ��롣(��̩-3)", "You make a lot of strange noise (-3 stealth).");
		}
		if (p_ptr->muta3 & MUT3_INFRAVIS)
		{
			info[i++] = _("���ʤ��������餷���ֳ������Ϥ���äƤ��롣(+3)", "You have remarkable infravision (+3).");
		}
		if (p_ptr->muta3 & MUT3_XTRA_LEGS)
		{
			info[i++] = _("���ʤ���;ʬ�����ܤ�­�������Ƥ��롣(��®+3)", "You have an extra pair of legs (+3 speed).");
		}
		if (p_ptr->muta3 & MUT3_SHORT_LEG)
		{
			info[i++] = _("���ʤ���­��û���͵�����(��®-3)", "Your legs are short stubs (-3 speed).");
		}
		if (p_ptr->muta3 & MUT3_ELEC_TOUC)
		{
			info[i++] = _("���ʤ��η�ɤˤ���ή��ή��Ƥ��롣", "Electricity is running through your veins.");
		}
		if (p_ptr->muta3 & MUT3_FIRE_BODY)
		{
			info[i++] = _("���ʤ����Τϱ�ˤĤĤޤ�Ƥ��롣", "Your body is enveloped in flames.");
		}
		if (p_ptr->muta3 & MUT3_WART_SKIN)
		{
			info[i++] = _("���ʤ���ȩ�ϥ��ܤ�����Ƥ��롣(̥��-2, AC+5)", "Your skin is covered with warts (-2 CHR, +5 AC).");
		}
		if (p_ptr->muta3 & MUT3_SCALES)
		{
			info[i++] = _("���ʤ���ȩ���ڤˤʤäƤ��롣(̥��-1, AC+10)", "Your skin has turned into scales (-1 CHR, +10 AC).");
		}
		if (p_ptr->muta3 & MUT3_IRON_SKIN)
		{
			info[i++] = _("���ʤ���ȩ��Ŵ�ǤǤ��Ƥ��롣(����-1, AC+25)", "Your skin is made of steel (-1 DEX, +25 AC).");
		}
		if (p_ptr->muta3 & MUT3_WINGS)
		{
			info[i++] = _("���ʤ��ϱ�����äƤ��롣", "You have wings.");
		}
		if (p_ptr->muta3 & MUT3_FEARLESS)
		{
			/* Unnecessary */
		}
		if (p_ptr->muta3 & MUT3_REGEN)
		{
			/* Unnecessary */
		}
		if (p_ptr->muta3 & MUT3_ESP)
		{
			/* Unnecessary */
		}
		if (p_ptr->muta3 & MUT3_LIMBER)
		{
			info[i++] = _("���ʤ����Τ����ˤ��ʤ䤫����(����+3)", "Your body is very limber (+3 DEX).");
		}
		if (p_ptr->muta3 & MUT3_ARTHRITIS)
		{
			info[i++] = _("���ʤ��Ϥ��Ĥ������ˤߤ򴶤��Ƥ��롣(����-3)", "Your joints ache constantly (-3 DEX).");
		}
		if (p_ptr->muta3 & MUT3_VULN_ELEM)
		{
			info[i++] = _("���ʤ��ϸ��Ǥι���˼夤��", "You are susceptible to damage from the elements.");
		}
		if (p_ptr->muta3 & MUT3_MOTION)
		{
			info[i++] = _("���ʤ���ư������Τ��϶�����(��̩+1)", "Your movements are precise and forceful (+1 STL).");
		}
		if (p_ptr->muta3 & MUT3_GOOD_LUCK)
		{
			info[i++] = _("���ʤ����򤤥�����ˤĤĤޤ�Ƥ��롣", "There is a white aura surrounding you.");
		}
		if (p_ptr->muta3 & MUT3_BAD_LUCK)
		{
			info[i++] = _("���ʤ��Ϲ���������ˤĤĤޤ�Ƥ��롣", "There is a black aura surrounding you.");
		}
	}

	if (p_ptr->blind)
	{
		info[i++] = _("���ʤ����ܤ������ʤ���", "You cannot see.");
	}
	if (p_ptr->confused)
	{
		info[i++] = _("���ʤ��Ϻ��𤷤Ƥ��롣", "You are confused.");
	}
	if (p_ptr->afraid)
	{
		info[i++] = _("���ʤ��϶��ݤ˿�����Ƥ��롣", "You are terrified.");
	}
	if (p_ptr->cut)
	{
		info[i++] = _("���ʤ��Ͻз줷�Ƥ��롣", "You are bleeding.");
	}
	if (p_ptr->stun)
	{
		info[i++] = _("���ʤ��Ϥ⤦���Ȥ��Ƥ��롣", "You are stunned.");
	}
	if (p_ptr->poisoned)
	{
		info[i++] = _("���ʤ����Ǥ˿�����Ƥ��롣", "You are poisoned.");
	}
	if (p_ptr->image)
	{
		info[i++] = _("���ʤ��ϸ��Ф򸫤Ƥ��롣", "You are hallucinating.");
	}
	if (p_ptr->cursed & TRC_TY_CURSE)
	{
		info[i++] = _("���ʤ��ϼٰ��ʱ�ǰ����ޤ�Ƥ��롣", "You carry an ancient foul curse.");
	}
	if (p_ptr->cursed & TRC_AGGRAVATE)
	{
		info[i++] = _("���ʤ��ϥ�󥹥������ܤ餻�Ƥ��롣", "You aggravate monsters.");
	}
	if (p_ptr->cursed & TRC_DRAIN_EXP)
	{
		info[i++] = _("���ʤ��Ϸи��ͤ�ۤ��Ƥ��롣", "You are drained.");
	}
	if (p_ptr->cursed & TRC_SLOW_REGEN)
	{
		info[i++] = _("���ʤ��β����Ϥ������٤���", "You regenerate slowly.");
	}
	if (p_ptr->cursed & TRC_ADD_L_CURSE)
	{
		info[i++] = _("���ʤ��μ夤�����������롣","Your weak curses multiply."); /* ����Ū -- henkma */
	}
	if (p_ptr->cursed & TRC_ADD_H_CURSE)
	{
		info[i++] = _("���ʤ��ζ��������������롣","Your heavy curses multiply."); /* ����Ū -- henkma */
	}
	if (p_ptr->cursed & TRC_CALL_ANIMAL)
	{
		info[i++] = _("���ʤ���ưʪ�������Ƥ��롣", "You attract animals.");
	}
	if (p_ptr->cursed & TRC_CALL_DEMON)
	{
		info[i++] = _("���ʤ��ϰ���������Ƥ��롣", "You attract demons.");
	}
	if (p_ptr->cursed & TRC_CALL_DRAGON)
	{
		info[i++] = _("���ʤ��ϥɥ饴��������Ƥ��롣", "You attract dragons.");
	}
	if (p_ptr->cursed & TRC_COWARDICE)
	{
		info[i++] = _("���ʤ��ϻ������¤ˤʤ롣", "You are subject to cowardice.");
	}
	if (p_ptr->cursed & TRC_TELEPORT)
	{
		info[i++] = _("���ʤ��ΰ��֤ϤҤ��礦���԰������", "Your position is very uncertain.");
	}
	if (p_ptr->cursed & TRC_LOW_MELEE)
	{
		info[i++] = _("���ʤ������Ϲ���򳰤��䤹����", "Your weapon causes you to miss blows.");
	}
	if (p_ptr->cursed & TRC_LOW_AC)
	{
		info[i++] = _("���ʤ��Ϲ��������䤹����", "You are subject to be hit.");
	}
	if (p_ptr->cursed & TRC_LOW_MAGIC)
	{
		info[i++] = _("���ʤ�����ˡ���Ԥ��䤹����", "You are subject to fail spellcasting.");
	}
	if (p_ptr->cursed & TRC_FAST_DIGEST)
	{
		info[i++] = _("���ʤ��Ϥ�����ʢ���ؤ롣", "You have a good appetite.");
	}
	if (p_ptr->cursed & TRC_DRAIN_HP)
	{
		info[i++] = _("���ʤ������Ϥ�ۤ��Ƥ��롣", "You are drained.");
	}
	if (p_ptr->cursed & TRC_DRAIN_MANA)
	{
		info[i++] = _("���ʤ������Ϥ�ۤ��Ƥ��롣", "You brain is drained.");
	}
	if (IS_BLESSED())
	{
		info[i++] = _("���ʤ��Ϲ�餵�򴶤��Ƥ��롣", "You feel rightous.");
	}
	if (IS_HERO())
	{
		info[i++] = _("���ʤ��ϥҡ�����ʬ����", "You feel heroic.");
	}
	if (p_ptr->shero)
	{
		info[i++] = _("���ʤ�����Ʈ������", "You are in a battle rage.");
	}
	if (p_ptr->protevil)
	{
		info[i++] = _("���ʤ��ϼٰ��ʤ�¸�ߤ������Ƥ��롣", "You are protected from evil.");
	}
	if (p_ptr->shield)
	{
		info[i++] = _("���ʤ��Ͽ���Υ�����ɤǼ���Ƥ��롣", "You are protected by a mystic shield.");
	}
	if (IS_INVULN())
	{
		info[i++] = _("���ʤ��ϸ��߽��Ĥ��ʤ���", "You are temporarily invulnerable.");
	}
	if (p_ptr->wraith_form)
	{
		info[i++] = _("���ʤ��ϰ��Ū��ͩ�β����Ƥ��롣", "You are temporarily incorporeal.");
	}
	if (p_ptr->special_attack & ATTACK_CONFUSE)
	{
		info[i++] = _("���ʤ��μ���֤������Ƥ��롣", "Your hands are glowing dull red.");
	}
	if (p_ptr->special_attack & ATTACK_FIRE)
	{
		info[i++] = _("���ʤ��μ�ϲб��ʤ���Ƥ��롣", "You can strike the enemy with flame.");
	}
	if (p_ptr->special_attack & ATTACK_COLD)
	{
		info[i++] = _("���ʤ��μ���䵤��ʤ���Ƥ��롣", "You can strike the enemy with cold.");
	}
	if (p_ptr->special_attack & ATTACK_ACID)
	{
		info[i++] = _("���ʤ��μ�ϻ���ʤ���Ƥ��롣", "You can strike the enemy with acid.");
	}
	if (p_ptr->special_attack & ATTACK_ELEC)
	{
		info[i++] = _("���ʤ��μ���ŷ��ʤ���Ƥ��롣", "You can strike the enemy with electoric shock.");
	}
	if (p_ptr->special_attack & ATTACK_POIS)
	{
		info[i++] = _("���ʤ��μ���Ǥ�ʤ���Ƥ��롣", "You can strike the enemy with poison.");
	}
	switch (p_ptr->action)
	{
		case ACTION_SEARCH:
			info[i++] = _("���ʤ��ϤҤ��礦����տ������Ϥ��Ϥ��Ƥ��롣", "You are looking around very carefully.");
			break;
	}
	if (p_ptr->new_spells)
	{
		info[i++] = _("���ʤ��ϼ�ʸ�䵧���ؤ֤��Ȥ��Ǥ��롣", "You can learn some spells/prayers.");
	}
	if (p_ptr->word_recall)
	{
		info[i++] = _("���ʤ��Ϥ����˵��Ԥ��������", "You will soon be recalled.");
	}
	if (p_ptr->alter_reality)
	{
		info[i++] = _("���ʤ��Ϥ����ˤ���������Υ��������", "You will soon be altered.");
	}
	if (p_ptr->see_infra)
	{
		info[i++] = _("���ʤ���Ʒ���ֳ������Ҵ��Ǥ��롣", "Your eyes are sensitive to infrared light.");
	}
	if (p_ptr->see_inv)
	{
		info[i++] = _("���ʤ���Ʃ���ʥ�󥹥����򸫤뤳�Ȥ��Ǥ��롣", "You can see invisible creatures.");
	}
	if (p_ptr->levitation)
	{
		info[i++] = _("���ʤ������֤��Ȥ��Ǥ��롣", "You can fly.");
	}
	if (p_ptr->free_act)
	{
		info[i++] = _("���ʤ��������Τ餺�θ��̤���äƤ��롣", "You have free action.");
	}
	if (p_ptr->regenerate)
	{
		info[i++] = _("���ʤ������᤯���Ϥ�������롣", "You regenerate quickly.");
	}
	if (p_ptr->slow_digest)
	{
		info[i++] = _("���ʤ��Ͽ��ߤ����ʤ���", "Your appetite is small.");
	}
	if (p_ptr->telepathy)
	{
		info[i++] = _("���ʤ��ϥƥ�ѥ���ǽ�Ϥ���äƤ��롣", "You have ESP.");
	}
	if (p_ptr->esp_animal)
	{
		info[i++] = _("���ʤ��ϼ���������ʪ��¸�ߤ򴶤���ǽ�Ϥ���äƤ��롣", "You sense natural creatures.");
	}
	if (p_ptr->esp_undead)
	{
		info[i++] = _("���ʤ��ϥ���ǥåɤ�¸�ߤ򴶤���ǽ�Ϥ���äƤ��롣", "You sense undead.");
	}
	if (p_ptr->esp_demon)
	{
		info[i++] = _("���ʤ��ϰ����¸�ߤ򴶤���ǽ�Ϥ���äƤ��롣", "You sense demons.");
	}
	if (p_ptr->esp_orc)
	{
		info[i++] = _("���ʤ��ϥ�������¸�ߤ򴶤���ǽ�Ϥ���äƤ��롣", "You sense orcs.");
	}
	if (p_ptr->esp_troll)
	{
		info[i++] = _("���ʤ��ϥȥ���¸�ߤ򴶤���ǽ�Ϥ���äƤ��롣", "You sense trolls.");
	}
	if (p_ptr->esp_giant)
	{
		info[i++] = _("���ʤ��ϵ�ͤ�¸�ߤ򴶤���ǽ�Ϥ���äƤ��롣", "You sense giants.");
	}
	if (p_ptr->esp_dragon)
	{
		info[i++] = _("���ʤ��ϥɥ饴���¸�ߤ򴶤���ǽ�Ϥ���äƤ��롣", "You sense dragons.");
	}
	if (p_ptr->esp_human)
	{
		info[i++] = _("���ʤ��Ͽʹ֤�¸�ߤ򴶤���ǽ�Ϥ���äƤ��롣", "You sense humans.");
	}
	if (p_ptr->esp_evil)
	{
		info[i++] = _("���ʤ��ϼٰ�������ʪ��¸�ߤ򴶤���ǽ�Ϥ���äƤ��롣", "You sense evil creatures.");
	}
	if (p_ptr->esp_good)
	{
		info[i++] = _("���ʤ������ɤ�����ʪ��¸�ߤ򴶤���ǽ�Ϥ���äƤ��롣", "You sense good creatures.");
	}
	if (p_ptr->esp_nonliving)
	{
		info[i++] = _("���ʤ��ϳ�ư����̵��ʪ�Τ�¸�ߤ򴶤���ǽ�Ϥ���äƤ��롣", "You sense non-living creatures.");
	}
	if (p_ptr->esp_unique)
	{
		info[i++] = _("���ʤ������̤ʶ�Ũ��¸�ߤ򴶤���ǽ�Ϥ���äƤ��롣", "You sense unique monsters.");
	}
	if (p_ptr->hold_exp)
	{
		info[i++] = _("���ʤ��ϼ��ʤηи��ͤ򤷤ä���Ȱݻ����롣", "You have a firm hold on your experience.");
	}
	if (p_ptr->reflect)
	{
		info[i++] = _("���ʤ�����μ�ʸ��ȿ�ͤ��롣", "You reflect bolt spells.");
	}
	if (p_ptr->sh_fire)
	{
		info[i++] = _("���ʤ��ϱ�Υ��������ޤ�Ƥ��롣", "You are surrounded with a fiery aura.");
	}
	if (p_ptr->sh_elec)
	{
		info[i++] = _("���ʤ����ŵ�����ޤ�Ƥ��롣", "You are surrounded with electricity.");
	}
	if (p_ptr->sh_cold)
	{
		info[i++] = _("���ʤ����䵤�Υ��������ޤ�Ƥ��롣", "You are surrounded with an aura of coldness.");
	}
	if (p_ptr->tim_sh_holy)
	{
		info[i++] = _("���ʤ������ʤ륪�������ޤ�Ƥ��롣", "You are surrounded with a holy aura.");
	}
	if (p_ptr->tim_sh_touki)
	{
		info[i++] = _("���ʤ���Ʈ���Υ��������ޤ�Ƥ��롣", "You are surrounded with a energy aura.");
	}
	if (p_ptr->anti_magic)
	{
		info[i++] = _("���ʤ���ȿ��ˡ������ɤ���ޤ�Ƥ��롣", "You are surrounded by an anti-magic shell.");
	}
	if (p_ptr->anti_tele)
	{
		info[i++] = _("���ʤ��ϥƥ�ݡ��ȤǤ��ʤ���", "You cannot teleport.");
	}
	if (p_ptr->lite)
	{
		info[i++] = _("���ʤ��ο��Τϸ��äƤ��롣", "You are carrying a permanent light.");
	}
	if (p_ptr->warning)
	{
		info[i++] = _("���ʤ��Ϲ�ư�����˴����Τ��뤳�Ȥ��Ǥ��롣", "You will be warned before dangerous actions.");
	}
	if (p_ptr->dec_mana)
	{
		info[i++] = _("���ʤ��Ͼ��ʤ��������Ϥ���ˡ�򾧤��뤳�Ȥ��Ǥ��롣", "You can cast spells with fewer mana points.");
	}
	if (p_ptr->easy_spell)
	{
		info[i++] = _("���ʤ����㤤����Ψ����ˡ�򾧤��뤳�Ȥ��Ǥ��롣", "Fail rate of your magic is decreased.");
	}
	if (p_ptr->heavy_spell)
	{
		info[i++] = _("���ʤ��Ϲ⤤����Ψ����ˡ�򾧤��ʤ���Ф����ʤ���", "Fail rate of your magic is increased.");
	}
	if (p_ptr->mighty_throw)
	{
		info[i++] = _("���ʤ��϶���ʪ���ꤲ�롣", "You can throw objects powerfully.");
	}

	if (p_ptr->immune_acid)
	{
		info[i++] = _("���ʤ��ϻ����Ф��봰���ʤ��ȱ֤���äƤ��롣", "You are completely immune to acid.");
	}
	else if (p_ptr->resist_acid && IS_OPPOSE_ACID())
	{
		info[i++] = _("���ʤ��ϻ��ؤζ��Ϥ���������äƤ��롣", "You resist acid exceptionally well.");
	}
	else if (p_ptr->resist_acid || IS_OPPOSE_ACID())
	{
		info[i++] = _("���ʤ��ϻ��ؤ���������äƤ��롣", "You are resistant to acid.");
	}

	if (p_ptr->immune_elec)
	{
		info[i++] = _("���ʤ����ŷ���Ф��봰���ʤ��ȱ֤���äƤ��롣", "You are completely immune to lightning.");
	}
	else if (p_ptr->resist_elec && IS_OPPOSE_ELEC())
	{
		info[i++] = _("���ʤ����ŷ�ؤζ��Ϥ���������äƤ��롣", "You resist lightning exceptionally well.");
	}
	else if (p_ptr->resist_elec || IS_OPPOSE_ELEC())
	{
		info[i++] = _("���ʤ����ŷ�ؤ���������äƤ��롣", "You are resistant to lightning.");
	}

	if (prace_is_(RACE_ANDROID) && !p_ptr->immune_elec)
	{
		info[i++] = _("���ʤ����ŷ�˼夤��", "You are susceptible to damage from lightning.");
	}

	if (p_ptr->immune_fire)
	{
		info[i++] = _("���ʤ��ϲФ��Ф��봰���ʤ��ȱ֤���äƤ��롣", "You are completely immune to fire.");
	}
	else if (p_ptr->resist_fire && IS_OPPOSE_FIRE())
	{
		info[i++] = _("���ʤ��ϲФؤζ��Ϥ���������äƤ��롣", "You resist fire exceptionally well.");
	}
	else if (p_ptr->resist_fire || IS_OPPOSE_FIRE())
	{
		info[i++] = _("���ʤ��ϲФؤ���������äƤ��롣", "You are resistant to fire.");
	}

	if (prace_is_(RACE_ENT) && !p_ptr->immune_fire)
	{
		info[i++] = _("���ʤ��ϲФ˼夤��", "You are susceptible to damage from fire.");
	}

	if (p_ptr->immune_cold)
	{
		info[i++] = _("���ʤ����䵤���Ф��봰���ʤ��ȱ֤���äƤ��롣", "You are completely immune to cold.");
	}
	else if (p_ptr->resist_cold && IS_OPPOSE_COLD())
	{
		info[i++] = _("���ʤ����䵤�ؤζ��Ϥ���������äƤ��롣", "You resist cold exceptionally well.");
	}
	else if (p_ptr->resist_cold || IS_OPPOSE_COLD())
	{
		info[i++] = _("���ʤ����䵤�ؤ���������äƤ��롣", "You are resistant to cold.");
	}

	if (p_ptr->resist_pois && IS_OPPOSE_POIS())
	{
		info[i++] = _("���ʤ����Ǥؤζ��Ϥ���������äƤ��롣", "You resist poison exceptionally well.");
	}
	else if (p_ptr->resist_pois || IS_OPPOSE_POIS())
	{
		info[i++] = _("���ʤ����Ǥؤ���������äƤ��롣", "You are resistant to poison.");
	}

	if (p_ptr->resist_lite)
	{
		info[i++] = _("���ʤ��������ؤ���������äƤ��롣", "You are resistant to bright light.");
	}

	if (prace_is_(RACE_VAMPIRE) || prace_is_(RACE_S_FAIRY) || (p_ptr->mimic_form == MIMIC_VAMPIRE))
	{
		info[i++] = _("���ʤ��������˼夤��", "You are susceptible to damage from bright light.");
	}

	if (prace_is_(RACE_VAMPIRE) || (p_ptr->mimic_form == MIMIC_VAMPIRE) || p_ptr->wraith_form)
	{
		info[i++] = _("���ʤ��ϰŹ����Ф��봰���ʤ��ȱ֤���äƤ��롣", "You are completely immune to darkness.");
	}

	else if (p_ptr->resist_dark)
	{
		info[i++] = _("���ʤ��ϰŹ��ؤ���������äƤ��롣", "You are resistant to darkness.");
	}
	if (p_ptr->resist_conf)
	{
		info[i++] = _("���ʤ��Ϻ���ؤ���������äƤ��롣", "You are resistant to confusion.");
	}
	if (p_ptr->resist_sound)
	{
		info[i++] = _("���ʤ��ϲ��Ȥξ׷�ؤ���������äƤ��롣", "You are resistant to sonic attacks.");
	}
	if (p_ptr->resist_disen)
	{
		info[i++] = _("���ʤ��������ؤ���������äƤ��롣", "You are resistant to disenchantment.");
	}
	if (p_ptr->resist_chaos)
	{
		info[i++] = _("���ʤ��ϥ��������Ϥؤ���������äƤ��롣", "You are resistant to chaos.");
	}
	if (p_ptr->resist_shard)
	{
		info[i++] = _("���ʤ������Ҥι���ؤ���������äƤ��롣", "You are resistant to blasts of shards.");
	}
	if (p_ptr->resist_nexus)
	{
		info[i++] = _("���ʤ��ϰ��̺���ι���ؤ���������äƤ��롣", "You are resistant to nexus attacks.");
	}

	if (prace_is_(RACE_SPECTRE))
	{
		info[i++] = _("���ʤ����Ϲ����Ϥ�ۼ��Ǥ��롣", "You can drain nether forces.");
	}
	else if (p_ptr->resist_neth)
	{
		info[i++] = _("���ʤ����Ϲ����Ϥؤ���������äƤ��롣", "You are resistant to nether forces.");
	}
	if (p_ptr->resist_fear)
	{
		info[i++] = _("���ʤ����������ݤ򴶤��ʤ���", "You are completely fearless.");
	}
	if (p_ptr->resist_blind)
	{
		info[i++] = _("���ʤ����ܤ����ܤؤ���������äƤ��롣", "Your eyes are resistant to blindness.");
	}
	if (p_ptr->resist_time)
	{
		info[i++] = _("���ʤ��ϻ��ֵ�ž�ؤ���������äƤ��롣", "You are resistant to time.");
	}

	if (p_ptr->sustain_str)
	{
		info[i++] = _("���ʤ������Ϥϰݻ�����Ƥ��롣", "Your strength is sustained.");
	}
	if (p_ptr->sustain_int)
	{
		info[i++] = _("���ʤ�����ǽ�ϰݻ�����Ƥ��롣", "Your intelligence is sustained.");
	}
	if (p_ptr->sustain_wis)
	{
		info[i++] = _("���ʤ��θ����ϰݻ�����Ƥ��롣", "Your wisdom is sustained.");
	}
	if (p_ptr->sustain_con)
	{
		info[i++] = _("���ʤ����ѵ��Ϥϰݻ�����Ƥ��롣", "Your constitution is sustained.");
	}
	if (p_ptr->sustain_dex)
	{
		info[i++] = _("���ʤ��δ��Ѥ��ϰݻ�����Ƥ��롣", "Your dexterity is sustained.");
	}
	if (p_ptr->sustain_chr)
	{
		info[i++] = _("���ʤ���̥�Ϥϰݻ�����Ƥ��롣", "Your charisma is sustained.");
	}

	if (have_flag(flgs, TR_STR))
	{
		info[i++] = _("���ʤ������Ϥ������ˤ�äƱƶ�������Ƥ��롣", "Your strength is affected by your equipment.");
	}
	if (have_flag(flgs, TR_INT))
	{
		info[i++] = _("���ʤ�����ǽ�������ˤ�äƱƶ�������Ƥ��롣", "Your intelligence is affected by your equipment.");
	}
	if (have_flag(flgs, TR_WIS))
	{
		info[i++] = _("���ʤ��θ����������ˤ�äƱƶ�������Ƥ��롣", "Your wisdom is affected by your equipment.");
	}
	if (have_flag(flgs, TR_DEX))
	{
		info[i++] = _("���ʤ��δ��Ѥ��������ˤ�äƱƶ�������Ƥ��롣", "Your dexterity is affected by your equipment.");
	}
	if (have_flag(flgs, TR_CON))
	{
		info[i++] = _("���ʤ����ѵ��Ϥ������ˤ�äƱƶ�������Ƥ��롣", "Your constitution is affected by your equipment.");
	}
	if (have_flag(flgs, TR_CHR))
	{
		info[i++] = _("���ʤ���̥�Ϥ������ˤ�äƱƶ�������Ƥ��롣", "Your charisma is affected by your equipment.");
	}

	if (have_flag(flgs, TR_STEALTH))
	{
		info[i++] = _("���ʤ��α�̩��ưǽ�Ϥ������ˤ�äƱƶ�������Ƥ��롣", "Your stealth is affected by your equipment.");
	}
	if (have_flag(flgs, TR_SEARCH))
	{
		info[i++] = _("���ʤ���õ��ǽ�Ϥ������ˤ�äƱƶ�������Ƥ��롣", "Your searching ability is affected by your equipment.");
	}
	if (have_flag(flgs, TR_INFRA))
	{
		info[i++] = _("���ʤ����ֳ������Ϥ������ˤ�äƱƶ�������Ƥ��롣", "Your infravision is affected by your equipment.");
	}
	if (have_flag(flgs, TR_TUNNEL))
	{
		info[i++] = _("���ʤ��κη�ǽ�Ϥ������ˤ�äƱƶ�������Ƥ��롣", "Your digging ability is affected by your equipment.");
	}
	if (have_flag(flgs, TR_SPEED))
	{
		info[i++] = _("���ʤ��Υ��ԡ��ɤ������ˤ�äƱƶ�������Ƥ��롣", "Your speed is affected by your equipment.");
	}
	if (have_flag(flgs, TR_BLOWS))
	{
		info[i++] = _("���ʤ��ι���®�٤������ˤ�äƱƶ�������Ƥ��롣", "Your attack speed is affected by your equipment.");
	}


	/* Access the current weapon */
	o_ptr = &inventory[INVEN_RARM];

	/* Analyze the weapon */
	if (o_ptr->k_idx)
	{
		/* Indicate Blessing */
		if (have_flag(flgs, TR_BLESSED))
		{
			info[i++] = _("���ʤ������Ͽ��ν�ʡ������Ƥ��롣", "Your weapon has been blessed by the gods.");
		}

		if (have_flag(flgs, TR_CHAOTIC))
		{
			info[i++] = _("���ʤ������ϥ��륹��ħ��°�����ġ�", "Your weapon is branded with the Sign of Logrus.");
		}

		/* Hack */
		if (have_flag(flgs, TR_IMPACT))
		{
			info[i++] = _("���ʤ��������Ƿ���Ͽ̤�ȯ�����뤳�Ȥ��Ǥ��롣", "The impact of your weapon can cause earthquakes.");
		}

		if (have_flag(flgs, TR_VORPAL))
		{
			info[i++] = _("���ʤ����������˱Ԥ���", "Your weapon is very sharp.");
		}

		if (have_flag(flgs, TR_VAMPIRIC))
		{
			info[i++] = _("���ʤ�������Ũ������̿�Ϥ�ۼ����롣", "Your weapon drains life from your foes.");
		}

		/* Special "Attack Bonuses" */
		if (have_flag(flgs, TR_BRAND_ACID))
		{
			info[i++] = _("���ʤ�������Ũ���Ϥ�����", "Your weapon melts your foes.");
		}
		if (have_flag(flgs, TR_BRAND_ELEC))
		{
			info[i++] = _("���ʤ�������Ũ���Ť����롣", "Your weapon shocks your foes.");
		}
		if (have_flag(flgs, TR_BRAND_FIRE))
		{
			info[i++] = _("���ʤ�������Ũ��ǳ�䤹��", "Your weapon burns your foes.");
		}
		if (have_flag(flgs, TR_BRAND_COLD))
		{
			info[i++] = _("���ʤ�������Ũ����餻�롣", "Your weapon freezes your foes.");
		}
		if (have_flag(flgs, TR_BRAND_POIS))
		{
			info[i++] = _("���ʤ�������Ũ���Ǥǿ�����", "Your weapon poisons your foes.");
		}

		/* Special "slay" flags */
		if (have_flag(flgs, TR_KILL_ANIMAL))
		{
			info[i++] = _("���ʤ�������ưʪ��ŷŨ�Ǥ��롣", "Your weapon is a great bane of animals.");
		}
		else if (have_flag(flgs, TR_SLAY_ANIMAL))
		{
			info[i++] = _("���ʤ�������ưʪ���Ф��ƶ����Ϥ�ȯ�����롣", "Your weapon strikes at animals with extra force.");
		}
		if (have_flag(flgs, TR_KILL_EVIL))
		{
			info[i++] = _("���ʤ������ϼٰ��ʤ�¸�ߤ�ŷŨ�Ǥ��롣", "Your weapon is a great bane of evil.");
		}
		else if (have_flag(flgs, TR_SLAY_EVIL))
		{
			info[i++] = _("���ʤ������ϼٰ��ʤ�¸�ߤ��Ф��ƶ����Ϥ�ȯ�����롣", "Your weapon strikes at evil with extra force.");
		}
		if (have_flag(flgs, TR_KILL_HUMAN))
		{
			info[i++] = _("���ʤ������Ͽʹ֤�ŷŨ�Ǥ��롣", "Your weapon is a great bane of humans.");
		}
		else if (have_flag(flgs, TR_SLAY_HUMAN))
		{
			info[i++] = _("���ʤ������Ͽʹ֤��Ф����ä˶����Ϥ�ȯ�����롣", "Your weapon is especially deadly against humans.");
		}
		if (have_flag(flgs, TR_KILL_UNDEAD))
		{
			info[i++] = _("���ʤ������ϥ���ǥåɤ�ŷŨ�Ǥ��롣", "Your weapon is a great bane of undead.");
		}
		else if (have_flag(flgs, TR_SLAY_UNDEAD))
		{
			info[i++] = _("���ʤ������ϥ���ǥåɤ��Ф��ƿ����ʤ��Ϥ�ȯ�����롣", "Your weapon strikes at undead with holy wrath.");
		}
		if (have_flag(flgs, TR_KILL_DEMON))
		{
			info[i++] = _("���ʤ������ϥǡ�����ŷŨ�Ǥ��롣", "Your weapon is a great bane of demons.");
		}
		else if (have_flag(flgs, TR_SLAY_DEMON))
		{
			info[i++] = _("���ʤ������ϥǡ������Ф��ƿ����ʤ��Ϥ�ȯ�����롣", "Your weapon strikes at demons with holy wrath.");
		}
		if (have_flag(flgs, TR_KILL_ORC))
		{
			info[i++] = _("���ʤ������ϥ�������ŷŨ�Ǥ��롣", "Your weapon is a great bane of orcs.");
		}
		else if (have_flag(flgs, TR_SLAY_ORC))
		{
			info[i++] = _("���ʤ������ϥ��������Ф����ä˶����Ϥ�ȯ�����롣", "Your weapon is especially deadly against orcs.");
		}
		if (have_flag(flgs, TR_KILL_TROLL))
		{
			info[i++] = _("���ʤ������ϥȥ���ŷŨ�Ǥ��롣", "Your weapon is a great bane of trolls.");
		}
		else if (have_flag(flgs, TR_SLAY_TROLL))
		{
			info[i++] = _("���ʤ������ϥȥ����Ф����ä˶����Ϥ�ȯ�����롣", "Your weapon is especially deadly against trolls.");
		}
		if (have_flag(flgs, TR_KILL_GIANT))
		{
			info[i++] = _("���ʤ������ϥ��㥤����Ȥ�ŷŨ�Ǥ��롣", "Your weapon is a great bane of giants.");
		}
		else if (have_flag(flgs, TR_SLAY_GIANT))
		{
			info[i++] = _("���ʤ������ϥ��㥤����Ȥ��Ф����ä˶����Ϥ�ȯ�����롣", "Your weapon is especially deadly against giants.");
		}
		/* Special "kill" flags */
		if (have_flag(flgs, TR_KILL_DRAGON))
		{
			info[i++] = _("���ʤ������ϥɥ饴���ŷŨ�Ǥ��롣", "Your weapon is a great bane of dragons.");
		}
		else if (have_flag(flgs, TR_SLAY_DRAGON))
		{
			info[i++] = _("���ʤ������ϥɥ饴����Ф����ä˶����Ϥ�ȯ�����롣", "Your weapon is especially deadly against dragons.");
		}

		if (have_flag(flgs, TR_FORCE_WEAPON))
		{
			info[i++] = _("���ʤ�������MP��Ȥäƹ��⤹�롣", "Your weapon causes greate damages using your MP.");
		}
		if (have_flag(flgs, TR_THROW))
		{
			info[i++] = _("���ʤ��������ꤲ�䤹����", "Your weapon can be thrown well.");
		}
	}


	/* Save the screen */
	screen_save();

	/* Erase the screen */
	for (k = 1; k < 24; k++) prt("", k, 13);

	/* Label the information */
	prt(_("        ���ʤ��ξ���:", "     Your Attributes:"), 1, 15);

	/* We will print on top of the map (column 13) */
	for (k = 2, j = 0; j < i; j++)
	{
		/* Show the info */
		prt(info[j], k++, 15);

		/* Every 20 entries (lines 2 to 21), start over */
		if ((k == 22) && (j+1 < i))
		{
			prt(_("-- ³�� --", "-- more --"), k, 15);
			inkey();
			for (; k > 2; k--) prt("", k, 15);
		}
	}

	/* Pause */
	prt(_("[���������򲡤��ȥ���������ޤ�]", "[Press any key to continue]"), k, 13);
	inkey();

	/* Restore the screen */
	screen_load();
}

/*!
 * @brief ��ˡ���̻��֤Υ�������˴�Ť���ɽ��ID���֤���
 * @param dur ���̥������
 * @return ���̻��֤�ɽ��ID
 */
static int report_magics_aux(int dur)
{
	if (dur <= 5)
	{
		return 0;
	}
	else if (dur <= 10)
	{
		return 1;
	}
	else if (dur <= 20)
	{
		return 2;
	}
	else if (dur <= 50)
	{
		return 3;
	}
	else if (dur <= 100)
	{
		return 4;
	}
	else if (dur <= 200)
	{
		return 5;
	}
	else
	{
		return 6;
	}
}

static cptr report_magic_durations[] =
{
#ifdef JP
"����û����",
"�����δ�",
"���Ф餯�δ�",
"¿��Ĺ����",
"Ĺ����",
"����Ĺ����",
"�����񤤤ۤ�Ĺ����",
"��󥹥����򹶷⤹��ޤ�"
#else
	"for a short time",
	"for a little while",
	"for a while",
	"for a long while",
	"for a long time",
	"for a very long time",
	"for an incredibly long time",
	"until you hit a monster"
#endif

};

/*!
 * @brief ���ߤΰ��Ū���̰������֤� / Report all currently active magical effects.
 * @return �ʤ�
 */
void report_magics(void)
{
	int     i = 0, j, k;
	char    Dummy[80];
	cptr    info[128];
	int     info2[128];


	if (p_ptr->blind)
	{
		info2[i]  = report_magics_aux(p_ptr->blind);
		info[i++] = _("���ʤ����ܤ������ʤ�", "You cannot see");
	}
	if (p_ptr->confused)
	{
		info2[i]  = report_magics_aux(p_ptr->confused);
		info[i++] = _("���ʤ��Ϻ��𤷤Ƥ���", "You are confused");
	}
	if (p_ptr->afraid)
	{
		info2[i]  = report_magics_aux(p_ptr->afraid);
		info[i++] = _("���ʤ��϶��ݤ˿�����Ƥ���", "You are terrified");
	}
	if (p_ptr->poisoned)
	{
		info2[i]  = report_magics_aux(p_ptr->poisoned);
		info[i++] = _("���ʤ����Ǥ˿�����Ƥ���", "You are poisoned");
	}
	if (p_ptr->image)
	{
		info2[i]  = report_magics_aux(p_ptr->image);
		info[i++] = _("���ʤ��ϸ��Ф򸫤Ƥ���", "You are hallucinating");
	}
	if (p_ptr->blessed)
	{
		info2[i]  = report_magics_aux(p_ptr->blessed);
		info[i++] = _("���ʤ��Ϲ�餵�򴶤��Ƥ���", "You feel rightous");
	}
	if (p_ptr->hero)
	{
		info2[i]  = report_magics_aux(p_ptr->hero);
		info[i++] = _("���ʤ��ϥҡ�����ʬ��", "You feel heroic");
	}
	if (p_ptr->shero)
	{
		info2[i]  = report_magics_aux(p_ptr->shero);
		info[i++] = _("���ʤ�����Ʈ����", "You are in a battle rage");
	}
	if (p_ptr->protevil)
	{
		info2[i]  = report_magics_aux(p_ptr->protevil);
		info[i++] = _("���ʤ��ϼٰ��ʤ�¸�ߤ������Ƥ���", "You are protected from evil");
	}
	if (p_ptr->shield)
	{
		info2[i]  = report_magics_aux(p_ptr->shield);
		info[i++] = _("���ʤ��Ͽ���Υ�����ɤǼ���Ƥ���", "You are protected by a mystic shield");
	}
	if (p_ptr->invuln)
	{
		info2[i]  = report_magics_aux(p_ptr->invuln);
		info[i++] = _("���ʤ���̵Ũ��", "You are invulnerable");
	}
	if (p_ptr->wraith_form)
	{
		info2[i]  = report_magics_aux(p_ptr->wraith_form);
		info[i++] = _("���ʤ���ͩ�β����Ƥ���", "You are incorporeal");
	}
	if (p_ptr->special_attack & ATTACK_CONFUSE)
	{
		info2[i]  = 7;
		info[i++] = _("���ʤ��μ���֤������Ƥ���", "Your hands are glowing dull red.");
	}
	if (p_ptr->word_recall)
	{
		info2[i]  = report_magics_aux(p_ptr->word_recall);
		info[i++] = _("���θ嵢�Ԥξۤ�ȯư����", "You are waiting to be recalled");
	}
	if (p_ptr->alter_reality)
	{
		info2[i]  = report_magics_aux(p_ptr->alter_reality);
		info[i++] = _("���θ帽�����Ƥ�ȯư����", "You waiting to be altered");
	}
	if (p_ptr->oppose_acid)
	{
		info2[i]  = report_magics_aux(p_ptr->oppose_acid);
		info[i++] = _("���ʤ��ϻ��ؤ���������äƤ���", "You are resistant to acid");
	}
	if (p_ptr->oppose_elec)
	{
		info2[i]  = report_magics_aux(p_ptr->oppose_elec);
		info[i++] = _("���ʤ����ŷ�ؤ���������äƤ���", "You are resistant to lightning");
	}
	if (p_ptr->oppose_fire)
	{
		info2[i]  = report_magics_aux(p_ptr->oppose_fire);
		info[i++] = _("���ʤ��ϲФؤ���������äƤ���", "You are resistant to fire");
	}
	if (p_ptr->oppose_cold)
	{
		info2[i]  = report_magics_aux(p_ptr->oppose_cold);
		info[i++] = _("���ʤ����䵤�ؤ���������äƤ���", "You are resistant to cold");
	}
	if (p_ptr->oppose_pois)
	{
		info2[i]  = report_magics_aux(p_ptr->oppose_pois);
		info[i++] = _("���ʤ����Ǥؤ���������äƤ���", "You are resistant to poison");
	}

	/* Save the screen */
	screen_save();

	/* Erase the screen */
	for (k = 1; k < 24; k++) prt("", k, 13);

	/* Label the information */
	prt(_("    ���ߤ����äƤ�����ˡ     :", "     Your Current Magic:"), 1, 15);


	/* We will print on top of the map (column 13) */
	for (k = 2, j = 0; j < i; j++)
	{
		/* Show the info */
		sprintf(Dummy, _("%-28s : ���� - %s ", "%s %s."), info[j],
			report_magic_durations[info2[j]]);
		prt(Dummy, k++, 15);

		/* Every 20 entries (lines 2 to 21), start over */
		if ((k == 22) && (j + 1 < i))
		{
			prt(_("-- ³�� --", "-- more --"), k, 15);
			inkey();
			for (; k > 2; k--) prt("", k, 15);
		}
	}

	/* Pause */
	prt(_("[���������򲡤��ȥ���������ޤ�]", "[Press any key to continue]"), k, 13);
	inkey();

	/* Restore the screen */
	screen_load();
}

/*!
 * @brief �ץ쥤�䡼���դ��Ϸ����Τ���
 * @param range �����ϰ�
 * @param flag �����Ϸ�ID
 * @param known �Ϸ�������ե饰�򳰤��ʤ�TRUE
 * @return ���Ϥ����ä����TRUE���֤�
 */
static bool detect_feat_flag(int range, int flag, bool known)
{
	int       x, y;
	bool      detect = FALSE;
	cave_type *c_ptr;

	if (d_info[dungeon_type].flags1 & DF1_DARKNESS) range /= 3;

	/* Scan the current panel */
	for (y = 1; y < cur_hgt - 1; y++)
	{
		for (x = 1; x <= cur_wid - 1; x++)
		{
			int dist = distance(p_ptr->y, p_ptr->x, y, x);
			if (dist > range) continue;

			/* Access the grid */
			c_ptr = &cave[y][x];

			/* Hack -- Safe */
			if (flag == FF_TRAP)
			{
				/* Mark as detected */
				if (dist <= range && known)
				{
					if (dist <= range - 1) c_ptr->info |= (CAVE_IN_DETECT);

					c_ptr->info &= ~(CAVE_UNSAFE);

					/* Redraw */
					lite_spot(y, x);
				}
			}

			/* Detect flags */
			if (cave_have_flag_grid(c_ptr, flag))
			{
				/* Detect secrets */
				disclose_grid(y, x);

				/* Hack -- Memorize */
				c_ptr->info |= (CAVE_MARK);

				/* Redraw */
				lite_spot(y, x);

				/* Obvious */
				detect = TRUE;
			}
		}
	}

	/* Result */
	return detect;
}


/*!
 * @brief �ץ쥤�䡼���դΥȥ�åפ��Τ��� / Detect all traps on current panel
 * @param range �����ϰ�
 * @param known ���γ��ϰϤ�Ķ����ٹ�ե饰��Ω�Ƥ���TRUE���֤�
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool detect_traps(int range, bool known)
{
	bool detect = detect_feat_flag(range, FF_TRAP, known);

	if (known) p_ptr->dtrap = TRUE;

	if (music_singing(MUSIC_DETECT) && p_ptr->magic_num1[2] > 0) detect = FALSE;

	/* Describe */
	if (detect)
	{
		msg_print(_("�ȥ�åפ�¸�ߤ򴶤��Ȥä���", "You sense the presence of traps!"));
	}

	/* Result */
	return detect;
}


/*!
 * @brief �ץ쥤�䡼���դΥɥ����Τ��� / Detect all doors on current panel
 * @param range �����ϰ�
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool detect_doors(int range)
{
	bool detect = detect_feat_flag(range, FF_DOOR, TRUE);

	if (music_singing(MUSIC_DETECT) && p_ptr->magic_num1[2] > 0) detect = FALSE;

	/* Describe */
	if (detect)
	{
		msg_print(_("�ɥ���¸�ߤ򴶤��Ȥä���", "You sense the presence of doors!"));
	}

	/* Result */
	return detect;
}


/*!
 * @brief �ץ쥤�䡼���դγ��ʤ��Τ��� / Detect all stairs on current panel
 * @param range �����ϰ�
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool detect_stairs(int range)
{
	bool detect = detect_feat_flag(range, FF_STAIRS, TRUE);

	if (music_singing(MUSIC_DETECT) && p_ptr->magic_num1[2] > 0) detect = FALSE;

	/* Describe */
	if (detect)
	{
		msg_print(_("���ʤ�¸�ߤ򴶤��Ȥä���", "You sense the presence of stairs!"));
	}

	/* Result */
	return detect;
}


/*!
 * @brief �ץ쥤�䡼���դ��Ϸ��������Τ��� / Detect any treasure on the current panel
 * @param range �����ϰ�
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool detect_treasure(int range)
{
	bool detect = detect_feat_flag(range, FF_HAS_GOLD, TRUE);

	if (music_singing(MUSIC_DETECT) && p_ptr->magic_num1[2] > 6) detect = FALSE;

	/* Describe */
	if (detect)
	{
		msg_print(_("��¢���줿������¸�ߤ򴶤��Ȥä���", "You sense the presence of buried treasure!"));
	}

	/* Result */
	return detect;
}


/*!
 * @brief �ץ쥤�䡼���դΥ����ƥ�������Τ��� / Detect all "gold" objects on the current panel
 * @param range �����ϰ�
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool detect_objects_gold(int range)
{
	int i, y, x;
	int range2 = range;

	bool detect = FALSE;

	if (d_info[dungeon_type].flags1 & DF1_DARKNESS) range2 /= 3;

	/* Scan objects */
	for (i = 1; i < o_max; i++)
	{
		object_type *o_ptr = &o_list[i];

		/* Skip dead objects */
		if (!o_ptr->k_idx) continue;

		/* Skip held objects */
		if (o_ptr->held_m_idx) continue;

		/* Location */
		y = o_ptr->iy;
		x = o_ptr->ix;

		/* Only detect nearby objects */
		if (distance(p_ptr->y, p_ptr->x, y, x) > range2) continue;

		/* Detect "gold" objects */
		if (o_ptr->tval == TV_GOLD)
		{
			/* Hack -- memorize it */
			o_ptr->marked |= OM_FOUND;

			/* Redraw */
			lite_spot(y, x);

			/* Detect */
			detect = TRUE;
		}
	}

	if (music_singing(MUSIC_DETECT) && p_ptr->magic_num1[2] > 6) detect = FALSE;

	/* Describe */
	if (detect)
	{
		msg_print(_("������¸�ߤ򴶤��Ȥä���", "You sense the presence of treasure!"));
	}

	if (detect_monsters_string(range, "$"))
	{
		detect = TRUE;
	}

	/* Result */
	return (detect);
}


/*!
 * @brief �̾�Υ����ƥ४�֥������Ȥ��Τ��� / Detect all "normal" objects on the current panel
 * @param range �����ϰ�
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool detect_objects_normal(int range)
{
	int i, y, x;
	int range2 = range;

	bool detect = FALSE;

	if (d_info[dungeon_type].flags1 & DF1_DARKNESS) range2 /= 3;

	/* Scan objects */
	for (i = 1; i < o_max; i++)
	{
		object_type *o_ptr = &o_list[i];

		/* Skip dead objects */
		if (!o_ptr->k_idx) continue;

		/* Skip held objects */
		if (o_ptr->held_m_idx) continue;

		/* Location */
		y = o_ptr->iy;
		x = o_ptr->ix;

		/* Only detect nearby objects */
		if (distance(p_ptr->y, p_ptr->x, y, x) > range2) continue;

		/* Detect "real" objects */
		if (o_ptr->tval != TV_GOLD)
		{
			/* Hack -- memorize it */
			o_ptr->marked |= OM_FOUND;

			/* Redraw */
			lite_spot(y, x);

			/* Detect */
			detect = TRUE;
		}
	}

	if (music_singing(MUSIC_DETECT) && p_ptr->magic_num1[2] > 6) detect = FALSE;

	/* Describe */
	if (detect)
	{
		msg_print(_("�����ƥ��¸�ߤ򴶤��Ȥä���", "You sense the presence of objects!"));
	}

	if (detect_monsters_string(range, "!=?|/`"))
	{
		detect = TRUE;
	}

	/* Result */
	return (detect);
}


/*!
 * @brief ��ˡ���̤Τ���Υ����ƥ४�֥������Ȥ��Τ��� / Detect all "magic" objects on the current panel.
 * @param range �����ϰ�
 * @return ���Ϥ����ä����TRUE���֤�
 * @details
 * <pre>
 * This will light up all spaces with "magic" items, including artifacts,
 * ego-items, potions, scrolls, books, rods, wands, staves, amulets, rings,
 * and "enchanted" items of the "good" variety.
 *
 * It can probably be argued that this function is now too powerful.
 * </pre>
 */
bool detect_objects_magic(int range)
{
	int i, y, x, tv;

	bool detect = FALSE;

	if (d_info[dungeon_type].flags1 & DF1_DARKNESS) range /= 3;

	/* Scan all objects */
	for (i = 1; i < o_max; i++)
	{
		object_type *o_ptr = &o_list[i];

		/* Skip dead objects */
		if (!o_ptr->k_idx) continue;

		/* Skip held objects */
		if (o_ptr->held_m_idx) continue;

		/* Location */
		y = o_ptr->iy;
		x = o_ptr->ix;

		/* Only detect nearby objects */
		if (distance(p_ptr->y, p_ptr->x, y, x) > range) continue;

		/* Examine the tval */
		tv = o_ptr->tval;

		/* Artifacts, misc magic items, or enchanted wearables */
		if (object_is_artifact(o_ptr) ||
			object_is_ego(o_ptr) ||
		    (tv == TV_WHISTLE) ||
		    (tv == TV_AMULET) ||
			(tv == TV_RING) ||
		    (tv == TV_STAFF) ||
			(tv == TV_WAND) ||
			(tv == TV_ROD) ||
		    (tv == TV_SCROLL) ||
			(tv == TV_POTION) ||
		    (tv == TV_LIFE_BOOK) ||
			(tv == TV_SORCERY_BOOK) ||
		    (tv == TV_NATURE_BOOK) ||
			(tv == TV_CHAOS_BOOK) ||
		    (tv == TV_DEATH_BOOK) ||
		    (tv == TV_TRUMP_BOOK) ||
			(tv == TV_ARCANE_BOOK) ||
			(tv == TV_CRAFT_BOOK) ||
			(tv == TV_DAEMON_BOOK) ||
			(tv == TV_CRUSADE_BOOK) ||
			(tv == TV_MUSIC_BOOK) ||
			(tv == TV_HISSATSU_BOOK) ||
			(tv == TV_HEX_BOOK) ||
		    ((o_ptr->to_a > 0) || (o_ptr->to_h + o_ptr->to_d > 0)))
		{
			/* Memorize the item */
			o_ptr->marked |= OM_FOUND;

			/* Redraw */
			lite_spot(y, x);

			/* Detect */
			detect = TRUE;
		}
	}

	/* Describe */
	if (detect)
	{
		msg_print(_("��ˡ�Υ����ƥ��¸�ߤ򴶤��Ȥä���", "You sense the presence of magic objects!"));
	}

	/* Return result */
	return (detect);
}


/*!
 * @brief ���̤Υ�󥹥������Τ��� / Detect all "normal" monsters on the current panel
 * @param range �����ϰ�
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool detect_monsters_normal(int range)
{
	int i, y, x;

	bool flag = FALSE;

	if (d_info[dungeon_type].flags1 & DF1_DARKNESS) range /= 3;

	/* Scan monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];
		monster_race *r_ptr = &r_info[m_ptr->r_idx];

		/* Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Only detect nearby monsters */
		if (distance(p_ptr->y, p_ptr->x, y, x) > range) continue;

		/* Detect all non-invisible monsters */
		if (!(r_ptr->flags2 & RF2_INVISIBLE) || p_ptr->see_inv)
		{
			/* Repair visibility later */
			repair_monsters = TRUE;

			/* Hack -- Detect monster */
			m_ptr->mflag2 |= (MFLAG2_MARK | MFLAG2_SHOW);

			/* Update the monster */
			update_mon(i, FALSE);

			/* Detect */
			flag = TRUE;
		}
	}

	if (music_singing(MUSIC_DETECT) && p_ptr->magic_num1[2] > 3) flag = FALSE;

	/* Describe */
	if (flag)
	{
		/* Describe result */
		msg_print(_("��󥹥�����¸�ߤ򴶤��Ȥä���", "You sense the presence of monsters!"));
	}

	/* Result */
	return (flag);
}


/*!
 * @brief �ԲĻ�Υ�󥹥������Τ��� / Detect all "invisible" monsters around the player
 * @param range �����ϰ�
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool detect_monsters_invis(int range)
{
	int i, y, x;
	bool flag = FALSE;

	if (d_info[dungeon_type].flags1 & DF1_DARKNESS) range /= 3;

	/* Scan monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];
		monster_race *r_ptr = &r_info[m_ptr->r_idx];

		/* Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Only detect nearby monsters */
		if (distance(p_ptr->y, p_ptr->x, y, x) > range) continue;

		/* Detect invisible monsters */
		if (r_ptr->flags2 & RF2_INVISIBLE)
		{
			/* Update monster recall window */
			if (p_ptr->monster_race_idx == m_ptr->r_idx)
			{
				/* Window stuff */
				p_ptr->window |= (PW_MONSTER);
			}

			/* Repair visibility later */
			repair_monsters = TRUE;

			/* Hack -- Detect monster */
			m_ptr->mflag2 |= (MFLAG2_MARK | MFLAG2_SHOW);

			/* Update the monster */
			update_mon(i, FALSE);

			/* Detect */
			flag = TRUE;
		}
	}

	if (music_singing(MUSIC_DETECT) && p_ptr->magic_num1[2] > 3) flag = FALSE;

	/* Describe */
	if (flag)
	{
		/* Describe result */
		msg_print(_("Ʃ������ʪ��¸�ߤ򴶤��Ȥä���", "You sense the presence of invisible creatures!"));
	}

	/* Result */
	return (flag);
}

/*!
 * @brief �ٰ��ʥ�󥹥������Τ��� / Detect all "evil" monsters on current panel
 * @param range �����ϰ�
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool detect_monsters_evil(int range)
{
	int i, y, x;
	bool flag = FALSE;

	if (d_info[dungeon_type].flags1 & DF1_DARKNESS) range /= 3;

	/* Scan monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];
		monster_race *r_ptr = &r_info[m_ptr->r_idx];

		/* Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Only detect nearby monsters */
		if (distance(p_ptr->y, p_ptr->x, y, x) > range) continue;

		/* Detect evil monsters */
		if (r_ptr->flags3 & RF3_EVIL)
		{
			if (is_original_ap(m_ptr))
			{
				/* Take note that they are evil */
				r_ptr->r_flags3 |= (RF3_EVIL);

				/* Update monster recall window */
				if (p_ptr->monster_race_idx == m_ptr->r_idx)
				{
					/* Window stuff */
					p_ptr->window |= (PW_MONSTER);
				}
			}

			/* Repair visibility later */
			repair_monsters = TRUE;

			/* Hack -- Detect monster */
			m_ptr->mflag2 |= (MFLAG2_MARK | MFLAG2_SHOW);

			/* Update the monster */
			update_mon(i, FALSE);

			/* Detect */
			flag = TRUE;
		}
	}

	/* Describe */
	if (flag)
	{
		/* Describe result */
		msg_print(_("�ٰ��ʤ���ʪ��¸�ߤ򴶤��Ȥä���", "You sense the presence of evil creatures!"));
	}

	/* Result */
	return (flag);
}

/*!
 * @brief ̵��̿�Υ�󥹥������Τ���(����ǥåɡ�����Ϥ�ޤ�) / Detect all "nonliving", "undead" or "demonic" monsters on current panel
 * @param range �����ϰ�
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool detect_monsters_nonliving(int range)
{
	int     i, y, x;
	bool    flag = FALSE;

	if (d_info[dungeon_type].flags1 & DF1_DARKNESS) range /= 3;

	/* Scan monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];
		monster_race *r_ptr = &r_info[m_ptr->r_idx];

		/* Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Only detect nearby monsters */
		if (distance(p_ptr->y, p_ptr->x, y, x) > range) continue;

		/* Detect non-living monsters */
		if (!monster_living(r_ptr))
		{
			/* Update monster recall window */
			if (p_ptr->monster_race_idx == m_ptr->r_idx)
			{
				/* Window stuff */
				p_ptr->window |= (PW_MONSTER);
			}

			/* Repair visibility later */
			repair_monsters = TRUE;

			/* Hack -- Detect monster */
			m_ptr->mflag2 |= (MFLAG2_MARK | MFLAG2_SHOW);

			/* Update the monster */
			update_mon(i, FALSE);

			/* Detect */
			flag = TRUE;
		}
	}

	/* Describe */
	if (flag)
	{
		/* Describe result */
		msg_print(_("�����Ǥʤ���󥹥�����¸�ߤ򴶤�����", "You sense the presence of unnatural beings!"));
	}

	/* Result */
	return (flag);
}

/*!
 * @brief �����Τ����󥹥������Τ��� / Detect all monsters it has mind on current panel
 * @param range �����ϰ�
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool detect_monsters_mind(int range)
{
	int     i, y, x;
	bool    flag = FALSE;

	if (d_info[dungeon_type].flags1 & DF1_DARKNESS) range /= 3;

	/* Scan monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];
		monster_race *r_ptr = &r_info[m_ptr->r_idx];

		/* Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Only detect nearby monsters */
		if (distance(p_ptr->y, p_ptr->x, y, x) > range) continue;

		/* Detect non-living monsters */
		if (!(r_ptr->flags2 & RF2_EMPTY_MIND))
		{
			/* Update monster recall window */
			if (p_ptr->monster_race_idx == m_ptr->r_idx)
			{
				/* Window stuff */
				p_ptr->window |= (PW_MONSTER);
			}

			/* Repair visibility later */
			repair_monsters = TRUE;

			/* Hack -- Detect monster */
			m_ptr->mflag2 |= (MFLAG2_MARK | MFLAG2_SHOW);

			/* Update the monster */
			update_mon(i, FALSE);

			/* Detect */
			flag = TRUE;
		}
	}

	/* Describe */
	if (flag)
	{
		/* Describe result */
		msg_print(_("�����򴶤��Ȥä���", "You sense the presence of someone's mind!"));
	}

	/* Result */
	return (flag);
}


/*!
 * @brief ��������ܥ�Υ�󥹥������Τ��� / Detect all (string) monsters on current panel
 * @param range �����ϰ�
 * @param Match �б�����ܥ�κ����ä���󥹥���ʸ����(ʣ�����경)
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool detect_monsters_string(int range, cptr Match)
{
	int i, y, x;
	bool flag = FALSE;

	if (d_info[dungeon_type].flags1 & DF1_DARKNESS) range /= 3;

	/* Scan monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];
		monster_race *r_ptr = &r_info[m_ptr->r_idx];

		/* Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Only detect nearby monsters */
		if (distance(p_ptr->y, p_ptr->x, y, x) > range) continue;

		/* Detect monsters with the same symbol */
		if (my_strchr(Match, r_ptr->d_char))
		{
			/* Update monster recall window */
			if (p_ptr->monster_race_idx == m_ptr->r_idx)
			{
				/* Window stuff */
				p_ptr->window |= (PW_MONSTER);
			}

			/* Repair visibility later */
			repair_monsters = TRUE;

			/* Hack -- Detect monster */
			m_ptr->mflag2 |= (MFLAG2_MARK | MFLAG2_SHOW);

			/* Update the monster */
			update_mon(i, FALSE);

			/* Detect */
			flag = TRUE;
		}
	}

	if (music_singing(MUSIC_DETECT) && p_ptr->magic_num1[2] > 3) flag = FALSE;

	/* Describe */
	if (flag)
	{
		/* Describe result */
		msg_print(_("��󥹥�����¸�ߤ򴶤��Ȥä���", "You sense the presence of monsters!"));
	}

	/* Result */
	return (flag);
}

/*!
 * @brief flags3���б������󥹥������Τ��� / A "generic" detect monsters routine, tagged to flags3
 * @param range �����ϰ�
 * @param match_flag ���Υե饰
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool detect_monsters_xxx(int range, u32b match_flag)
{
	int  i, y, x;
	bool flag = FALSE;
	cptr desc_monsters = _("�Ѥʥ�󥹥���", "weird monsters");

	if (d_info[dungeon_type].flags1 & DF1_DARKNESS) range /= 3;

	/* Scan monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];
		monster_race *r_ptr = &r_info[m_ptr->r_idx];

		/* Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Only detect nearby monsters */
		if (distance(p_ptr->y, p_ptr->x, y, x) > range) continue;

		/* Detect evil monsters */
		if (r_ptr->flags3 & (match_flag))
		{
			if (is_original_ap(m_ptr))
			{
				/* Take note that they are something */
				r_ptr->r_flags3 |= (match_flag);

				/* Update monster recall window */
				if (p_ptr->monster_race_idx == m_ptr->r_idx)
				{
					/* Window stuff */
					p_ptr->window |= (PW_MONSTER);
				}
			}

			/* Repair visibility later */
			repair_monsters = TRUE;

			/* Hack -- Detect monster */
			m_ptr->mflag2 |= (MFLAG2_MARK | MFLAG2_SHOW);

			/* Update the monster */
			update_mon(i, FALSE);

			/* Detect */
			flag = TRUE;
		}
	}

	/* Describe */
	if (flag)
	{
		switch (match_flag)
		{
			case RF3_DEMON:
			desc_monsters = _("�ǡ����", "demons");
				break;
			case RF3_UNDEAD:
			desc_monsters = _("����ǥå�", "the undead");
				break;
		}

		/* Describe result */
		msg_format(_("%s��¸�ߤ򴶤��Ȥä���", "You sense the presence of %s!"), desc_monsters);
		msg_print(NULL);
	}

	/* Result */
	return (flag);
}


/*!
 * @brief �����ν��� / Detect everything
 * @param range �����ϰ�
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool detect_all(int range)
{
	bool detect = FALSE;

	/* Detect everything */
	if (detect_traps(range, TRUE)) detect = TRUE;
	if (detect_doors(range)) detect = TRUE;
	if (detect_stairs(range)) detect = TRUE;

	/* There are too many hidden treasure.  So... */
	/* if (detect_treasure(range)) detect = TRUE; */

	if (detect_objects_gold(range)) detect = TRUE;
	if (detect_objects_normal(range)) detect = TRUE;
	if (detect_monsters_invis(range)) detect = TRUE;
	if (detect_monsters_normal(range)) detect = TRUE;

	/* Result */
	return (detect);
}


/*!
 * @brief �볦���󥹥�������ˡ���̤�Ϳ���� / Apply a "project()" directly to all viewable monsters
 * @param typ °������
 * @param dam ������
 * @return ���Ϥ����ä����TRUE���֤�
 * @details
 * <pre>
 * Note that affected monsters are NOT auto-tracked by this usage.
 *
 * To avoid misbehavior when monster deaths have side-effects,
 * this is done in two passes. -- JDL
 * </pre>
 */
bool project_hack(int typ, int dam)
{
	int     i, x, y;
	int     flg = PROJECT_JUMP | PROJECT_KILL | PROJECT_HIDE;
	bool    obvious = FALSE;


	/* Mark all (nearby) monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Require line of sight */
		if (!player_has_los_bold(y, x) || !projectable(p_ptr->y, p_ptr->x, y, x)) continue;

		/* Mark the monster */
		m_ptr->mflag |= (MFLAG_TEMP);
	}

	/* Affect all marked monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];

		/* Skip unmarked monsters */
		if (!(m_ptr->mflag & (MFLAG_TEMP))) continue;

		/* Remove mark */
		m_ptr->mflag &= ~(MFLAG_TEMP);

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Jump directly to the target monster */
		if (project(0, 0, y, x, dam, typ, flg, -1)) obvious = TRUE;
	}

	/* Result */
	return (obvious);
}


/*!
 * @brief �볦���󥹥������®������� / Speed monsters
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool speed_monsters(void)
{
	return (project_hack(GF_OLD_SPEED, p_ptr->lev));
}

/*!
 * @brief �볦���󥹥������®������� / Slow monsters
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool slow_monsters(int power)
{
	return (project_hack(GF_OLD_SLOW, power));
}

/*!
 * @brief �볦���󥹥�����̲�餻����� / Sleep monsters
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool sleep_monsters(int power)
{
	return (project_hack(GF_OLD_SLEEP, power));
}

/*!
 * @brief �볦��μٰ��ʥ�󥹥�����ƥ�ݡ��ȡ������������������ / Banish evil monsters
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool banish_evil(int dist)
{
	return (project_hack(GF_AWAY_EVIL, dist));
}

/*!
 * @brief �볦��Υ���ǥåɡ���󥹥������ݤ�������� / Turn undead
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool turn_undead(void)
{
	bool tester = (project_hack(GF_TURN_UNDEAD, p_ptr->lev));
	if (tester)
		chg_virtue(V_UNLIFE, -1);
	return tester;
}

/*!
 * @brief �볦��Υ���ǥåɡ���󥹥����˥��᡼����Ϳ������� / Dispel undead monsters
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool dispel_undead(int dam)
{
	bool tester = (project_hack(GF_DISP_UNDEAD, dam));
	if (tester)
		chg_virtue(V_UNLIFE, -2);
	return tester;
}

/*!
 * @brief �볦��μٰ��ʥ�󥹥����˥��᡼����Ϳ������� / Dispel evil monsters
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool dispel_evil(int dam)
{
	return (project_hack(GF_DISP_EVIL, dam));
}

/*!
 * @brief �볦������ɤʥ�󥹥����˥��᡼����Ϳ������� / Dispel good monsters
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool dispel_good(int dam)
{
	return (project_hack(GF_DISP_GOOD, dam));
}

/*!
 * @brief �볦��Τ������󥹥����˥��᡼����Ϳ������� / Dispel all monsters
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool dispel_monsters(int dam)
{
	return (project_hack(GF_DISP_ALL, dam));
}

/*!
 * @brief �볦�����̿�Τ����󥹥����˥��᡼����Ϳ������� / Dispel 'living' monsters
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool dispel_living(int dam)
{
	return (project_hack(GF_DISP_LIVING, dam));
}

/*!
 * @brief �볦��ΰ���ϥ�󥹥����˥��᡼����Ϳ������� / Dispel 'living' monsters
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool dispel_demons(int dam)
{
	return (project_hack(GF_DISP_DEMON, dam));
}

/*!
 * @brief �볦��Υ�󥹥����ˡ�����׸��̤�Ϳ�������
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool crusade(void)
{
	return (project_hack(GF_CRUSADE, p_ptr->lev*4));
}

/*!
 * @brief �볦���󥹥������ܤ餻����� / Wake up all monsters, and speed up "los" monsters.
 * @param who �ܤ餻�븶���򵯤�������󥹥���(0�ʤ�Хץ쥤�䡼)
 * @return �ʤ�
 */
void aggravate_monsters(int who)
{
	int     i;
	bool    sleep = FALSE;
	bool    speed = FALSE;


	/* Aggravate everyone nearby */
	for (i = 1; i < m_max; i++)
	{
		monster_type    *m_ptr = &m_list[i];
/*		monster_race    *r_ptr = &r_info[m_ptr->r_idx]; */

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Skip aggravating monster (or player) */
		if (i == who) continue;

		/* Wake up nearby sleeping monsters */
		if (m_ptr->cdis < MAX_SIGHT * 2)
		{
			/* Wake up */
			if (MON_CSLEEP(m_ptr))
			{
				(void)set_monster_csleep(i, 0);
				sleep = TRUE;
			}
			if (!is_pet(m_ptr)) m_ptr->mflag2 |= MFLAG2_NOPET;
		}

		/* Speed up monsters in line of sight */
		if (player_has_los_bold(m_ptr->fy, m_ptr->fx))
		{
			if (!is_pet(m_ptr))
			{
				(void)set_monster_fast(i, MON_FAST(m_ptr) + 100);
				speed = TRUE;
			}
		}
	}

	/* Messages */
#ifdef JP
	if (speed) msg_print("�ն�ǲ�������ǡ��ʳ�����褦�ʴ������������");
	else if (sleep) msg_print("��������ǡ��ʳ�����褦���������������󤯤�ʹ��������");
#else
	if (speed) msg_print("You feel a sudden stirring nearby!");
	else if (sleep) msg_print("You hear a sudden stirring in the distance!");
#endif
	if (p_ptr->riding) p_ptr->update |= PU_BONUS;
}


/*!
 * @brief ��󥹥����ؤ�ñ�������������֥롼���� / Delete a non-unique/non-quest monster
 * @param m_idx ���������󥹥���ID
 * @param power �����ΰ���
 * @param player_cast �ץ쥤�䡼����ˡ�ˤ���Τʤ�� TRUE
 * @param dam_side �ץ쥤�䡼�ؤ���ô���᡼����(1d(dam_side))
 * @param spell_name �������̤򵯤�������ˡ��̾��
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool genocide_aux(int m_idx, int power, bool player_cast, int dam_side, cptr spell_name)
{
	int          msec = delay_factor * delay_factor * delay_factor;
	monster_type *m_ptr = &m_list[m_idx];
	monster_race *r_ptr = &r_info[m_ptr->r_idx];
	bool         resist = FALSE;

	if (is_pet(m_ptr) && !player_cast) return FALSE;

	/* Hack -- Skip Unique Monsters or Quest Monsters */
	if (r_ptr->flags1 & (RF1_UNIQUE | RF1_QUESTOR)) resist = TRUE;

	else if (r_ptr->flags7 & RF7_UNIQUE2) resist = TRUE;

	else if (m_idx == p_ptr->riding) resist = TRUE;

	else if ((p_ptr->inside_quest && !random_quest_number(dun_level)) || p_ptr->inside_arena || p_ptr->inside_battle) resist = TRUE;

	else if (player_cast && (r_ptr->level > randint0(power))) resist = TRUE;

	else if (player_cast && (m_ptr->mflag2 & MFLAG2_NOGENO)) resist = TRUE;

	/* Delete the monster */
	else
	{
		if (record_named_pet && is_pet(m_ptr) && m_ptr->nickname)
		{
			char m_name[80];

			monster_desc(m_name, m_ptr, MD_INDEF_VISIBLE);
			do_cmd_write_nikki(NIKKI_NAMED_PET, RECORD_NAMED_PET_GENOCIDE, m_name);
		}

		delete_monster_idx(m_idx);
	}

	if (resist && player_cast)
	{
		bool see_m = is_seen(m_ptr);
		char m_name[80];

		monster_desc(m_name, m_ptr, 0);
		if (see_m)
		{
			msg_format(_("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected."), m_name);
		}
		if (MON_CSLEEP(m_ptr))
		{
			(void)set_monster_csleep(m_idx, 0);
			if (m_ptr->ml)
			{
				msg_format(_("%^s���ܤ�Фޤ�����", "%^s wakes up."), m_name);
			}
		}
		if (is_friendly(m_ptr) && !is_pet(m_ptr))
		{
			if (see_m)
			{
				msg_format(_("%s���ܤä���", "%^s gets angry!"), m_name);
			}
			set_hostile(m_ptr);
		}
		if (one_in_(13)) m_ptr->mflag2 |= MFLAG2_NOGENO;
	}

	if (player_cast)
	{
		/* Take damage */
		take_hit(DAMAGE_GENO, randint1(dam_side), format(_("%^s�μ�ʸ�򾧤�����ϫ", "the strain of casting %^s"), spell_name), -1);
	}

	/* Visual feedback */
	move_cursor_relative(p_ptr->y, p_ptr->x);

	/* Redraw */
	p_ptr->redraw |= (PR_HP);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);

	/* Handle */
	handle_stuff();

	/* Fresh */
	Term_fresh();

	/* Delay */
	Term_xtra(TERM_XTRA_DELAY, msec);

	return !resist;
}


/*!
 * @brief ��󥹥����ؤΥ���ܥ����������롼���� / Delete all non-unique/non-quest monsters of a given "type" from the level
 * @param power �����ΰ���
 * @param player_cast �ץ쥤�䡼����ˡ�ˤ���Τʤ�� TRUE
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool symbol_genocide(int power, bool player_cast)
{
	int  i;
	char typ;
	bool result = FALSE;

	/* Prevent genocide in quest levels */
	if ((p_ptr->inside_quest && !random_quest_number(dun_level)) || p_ptr->inside_arena || p_ptr->inside_battle)
	{
		return (FALSE);
	}

	/* Mega-Hack -- Get a monster symbol */
	while (!get_com(_("�ɤμ���(ʸ��)�Υ�󥹥������������ޤ���: ", "Choose a monster race (by symbol) to genocide: "), &typ, FALSE)) ;

	/* Delete the monsters of that "type" */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];
		monster_race *r_ptr = &r_info[m_ptr->r_idx];

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Skip "wrong" monsters */
		if (r_ptr->d_char != typ) continue;

		/* Take note */
		result |= genocide_aux(i, power, player_cast, 4, _("����", "Genocide"));
	}

	if (result)
	{
		chg_virtue(V_VITALITY, -2);
		chg_virtue(V_CHANCE, -1);
	}

	return result;
}


/*!
 * @brief ��󥹥����ؤμ������������롼���� / Delete all nearby (non-unique) monsters
 * @param power �����ΰ���
 * @param player_cast �ץ쥤�䡼����ˡ�ˤ���Τʤ�� TRUE
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool mass_genocide(int power, bool player_cast)
{
	int  i;
	bool result = FALSE;

	/* Prevent mass genocide in quest levels */
	if ((p_ptr->inside_quest && !random_quest_number(dun_level)) || p_ptr->inside_arena || p_ptr->inside_battle)
	{
		return (FALSE);
	}

	/* Delete the (nearby) monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Skip distant monsters */
		if (m_ptr->cdis > MAX_SIGHT) continue;

		/* Note effect */
		result |= genocide_aux(i, power, player_cast, 3, _("��������", "Mass Genocide"));
	}

	if (result)
	{
		chg_virtue(V_VITALITY, -2);
		chg_virtue(V_CHANCE, -1);
	}

	return result;
}


/*!
 * @brief ����ǥåɡ���󥹥����ؤμ������������롼���� / Delete all nearby (non-unique) undead
 * @param power �����ΰ���
 * @param player_cast �ץ쥤�䡼����ˡ�ˤ���Τʤ�� TRUE
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool mass_genocide_undead(int power, bool player_cast)
{
	int  i;
	bool result = FALSE;

	/* Prevent mass genocide in quest levels */
	if ((p_ptr->inside_quest && !random_quest_number(dun_level)) || p_ptr->inside_arena || p_ptr->inside_battle)
	{
		return (FALSE);
	}

	/* Delete the (nearby) monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];
		monster_race *r_ptr = &r_info[m_ptr->r_idx];

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		if (!(r_ptr->flags3 & RF3_UNDEAD)) continue;

		/* Skip distant monsters */
		if (m_ptr->cdis > MAX_SIGHT) continue;

		/* Note effect */
		result |= genocide_aux(i, power, player_cast, 3, _("����ǥåɾ���", "Annihilate Undead"));
	}

	if (result)
	{
		chg_virtue(V_UNLIFE, -2);
		chg_virtue(V_CHANCE, -1);
	}

	return result;
}


/*!
 * @brief ���ե�󥹥�����Ĵ������ / Probe nearby monsters
 * @return ���Ϥ����ä����TRUE���֤�
 */
bool probing(void)
{
	int     i, speed;
	int cu, cv;
	bool    probe = FALSE;
	char buf[256];
	cptr align;

	cu = Term->scr->cu;
	cv = Term->scr->cv;
	Term->scr->cu = 0;
	Term->scr->cv = 1;

	/* Probe all (nearby) monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];
		monster_race *r_ptr = &r_info[m_ptr->r_idx];

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Require line of sight */
		if (!player_has_los_bold(m_ptr->fy, m_ptr->fx)) continue;

		/* Probe visible monsters */
		if (m_ptr->ml)
		{
			char m_name[80];

			/* Start the message */
			if (!probe)
			{
				msg_print(_("Ĵ����...", "Probing..."));
			}

			msg_print(NULL);

			if (!is_original_ap(m_ptr))
			{
				if (m_ptr->mflag2 & MFLAG2_KAGE)
					m_ptr->mflag2 &= ~(MFLAG2_KAGE);

				m_ptr->ap_r_idx = m_ptr->r_idx;
				lite_spot(m_ptr->fy, m_ptr->fx);
			}
			/* Get "the monster" or "something" */
			monster_desc(m_name, m_ptr, MD_IGNORE_HALLU | MD_INDEF_HIDDEN);

			speed = m_ptr->mspeed - 110;
			if (MON_FAST(m_ptr)) speed += 10;
			if (MON_SLOW(m_ptr)) speed -= 10;

			/* Get the monster's alignment */
#ifdef JP
			if ((r_ptr->flags3 & (RF3_EVIL | RF3_GOOD)) == (RF3_EVIL | RF3_GOOD)) align = "����";
			else if (r_ptr->flags3 & RF3_EVIL) align = "�ٰ�";
			else if (r_ptr->flags3 & RF3_GOOD) align = "����";
			else if ((m_ptr->sub_align & (SUB_ALIGN_EVIL | SUB_ALIGN_GOOD)) == (SUB_ALIGN_EVIL | SUB_ALIGN_GOOD)) align = "��Ω(����)";
			else if (m_ptr->sub_align & SUB_ALIGN_EVIL) align = "��Ω(�ٰ�)";
			else if (m_ptr->sub_align & SUB_ALIGN_GOOD) align = "��Ω(����)";
			else align = "��Ω";
#else
			if ((r_ptr->flags3 & (RF3_EVIL | RF3_GOOD)) == (RF3_EVIL | RF3_GOOD)) align = "good&evil";
			else if (r_ptr->flags3 & RF3_EVIL) align = "evil";
			else if (r_ptr->flags3 & RF3_GOOD) align = "good";
			else if ((m_ptr->sub_align & (SUB_ALIGN_EVIL | SUB_ALIGN_GOOD)) == (SUB_ALIGN_EVIL | SUB_ALIGN_GOOD)) align = "neutral(good&evil)";
			else if (m_ptr->sub_align & SUB_ALIGN_EVIL) align = "neutral(evil)";
			else if (m_ptr->sub_align & SUB_ALIGN_GOOD) align = "neutral(good)";
			else align = "neutral";
#endif

			/* Describe the monster */
#ifdef JP
sprintf(buf,"%s ... °��:%s HP:%d/%d AC:%d ®��:%s%d �и�:", m_name, align, m_ptr->hp, m_ptr->maxhp, r_ptr->ac, (speed > 0) ? "+" : "", speed);
#else
sprintf(buf, "%s ... align:%s HP:%d/%d AC:%d speed:%s%d exp:", m_name, align, m_ptr->hp, m_ptr->maxhp, r_ptr->ac, (speed > 0) ? "+" : "", speed);
#endif
			if (r_ptr->next_r_idx)
			{
				strcat(buf, format("%d/%d ", m_ptr->exp, r_ptr->next_exp));
			}
			else
			{
				strcat(buf, "xxx ");
			}

#ifdef JP
			if (MON_CSLEEP(m_ptr)) strcat(buf,"��̲ ");
			if (MON_STUNNED(m_ptr)) strcat(buf,"ۯ۰ ");
			if (MON_MONFEAR(m_ptr)) strcat(buf,"���� ");
			if (MON_CONFUSED(m_ptr)) strcat(buf,"���� ");
			if (MON_INVULNER(m_ptr)) strcat(buf,"̵Ũ ");
#else
			if (MON_CSLEEP(m_ptr)) strcat(buf,"sleeping ");
			if (MON_STUNNED(m_ptr)) strcat(buf,"stunned ");
			if (MON_MONFEAR(m_ptr)) strcat(buf,"scared ");
			if (MON_CONFUSED(m_ptr)) strcat(buf,"confused ");
			if (MON_INVULNER(m_ptr)) strcat(buf,"invulnerable ");
#endif
			buf[strlen(buf)-1] = '\0';
			prt(buf,0,0);

			/* HACK : Add the line to message buffer */
			message_add(buf);
			p_ptr->window |= (PW_MESSAGE);
			window_stuff();

			if (m_ptr->ml) move_cursor_relative(m_ptr->fy, m_ptr->fx);
			inkey();

			Term_erase(0, 0, 255);

			/* Learn everything about this monster */
			if (lore_do_probe(m_ptr->r_idx))
			{
				char buf[80];

				/* Get base name of monster */
				strcpy(buf, (r_name + r_ptr->name));

#ifdef JP
				/* Note that we learnt some new flags  -Mogami- */
				msg_format("%s�ˤĤ��Ƥ���˾ܤ����ʤä��������롣", buf);
#else
				/* Pluralize it */
				plural_aux(buf);

				/* Note that we learnt some new flags  -Mogami- */
				msg_format("You now know more about %s.", buf);
#endif
				/* Clear -more- prompt */
				msg_print(NULL);
			}

			/* Probe worked */
			probe = TRUE;
		}
	}

	Term->scr->cu = cu;
	Term->scr->cv = cv;
	Term_fresh();

	/* Done */
	if (probe)
	{
		chg_virtue(V_KNOWLEDGE, 1);
		msg_print(_("����������Ǥ���", "That's all."));
	}

	/* Result */
	return (probe);
}



/*!
 * @brief *�˲�*������Ԥ� / The spell of destruction
 * @param y1 �˲����濴Y��ɸ
 * @param x1 �˲����濴X��ɸ 
 * @param r �˲���Ⱦ��
 * @param in_generate ���󥸥��ե�������ν����ʤ��TRUE
 * @return ���Ϥ����ä����TRUE���֤�
 * @details
 * <pre>
 * This spell "deletes" monsters (instead of "killing" them).
 *
 * Later we may use one function for both "destruction" and
 * "earthquake" by using the "full" to select "destruction".
 * </pre>
 */
bool destroy_area(int y1, int x1, int r, bool in_generate)
{
	int       y, x, k, t;
	cave_type *c_ptr;
	bool      flag = FALSE;

	/* Prevent destruction of quest levels and town */
	if ((p_ptr->inside_quest && is_fixed_quest_idx(p_ptr->inside_quest)) || !dun_level)
	{
		return (FALSE);
	}

	/* Lose monster light */
	if (!in_generate) clear_mon_lite();

	/* Big area of affect */
	for (y = (y1 - r); y <= (y1 + r); y++)
	{
		for (x = (x1 - r); x <= (x1 + r); x++)
		{
			/* Skip illegal grids */
			if (!in_bounds(y, x)) continue;

			/* Extract the distance */
			k = distance(y1, x1, y, x);

			/* Stay in the circle of death */
			if (k > r) continue;

			/* Access the grid */
			c_ptr = &cave[y][x];

			/* Lose room and vault */
			c_ptr->info &= ~(CAVE_ROOM | CAVE_ICKY);

			/* Lose light and knowledge */
			c_ptr->info &= ~(CAVE_MARK | CAVE_GLOW | CAVE_KNOWN);

			if (!in_generate) /* Normal */
			{
				/* Lose unsafety */
				c_ptr->info &= ~(CAVE_UNSAFE);

				/* Hack -- Notice player affect */
				if (player_bold(y, x))
				{
					/* Hurt the player later */
					flag = TRUE;

					/* Do not hurt this grid */
					continue;
				}
			}

			/* Hack -- Skip the epicenter */
			if ((y == y1) && (x == x1)) continue;

			if (c_ptr->m_idx)
			{
				monster_type *m_ptr = &m_list[c_ptr->m_idx];
				monster_race *r_ptr = &r_info[m_ptr->r_idx];

				if (in_generate) /* In generation */
				{
					/* Delete the monster (if any) */
					delete_monster(y, x);
				}
				else if (r_ptr->flags1 & RF1_QUESTOR)
				{
					/* Heal the monster */
					m_ptr->hp = m_ptr->maxhp;

					/* Try to teleport away quest monsters */
					if (!teleport_away(c_ptr->m_idx, (r * 2) + 1, TELEPORT_DEC_VALOUR)) continue;
				}
				else
				{
					if (record_named_pet && is_pet(m_ptr) && m_ptr->nickname)
					{
						char m_name[80];

						monster_desc(m_name, m_ptr, MD_INDEF_VISIBLE);
						do_cmd_write_nikki(NIKKI_NAMED_PET, RECORD_NAMED_PET_DESTROY, m_name);
					}

					/* Delete the monster (if any) */
					delete_monster(y, x);
				}
			}

			/* During generation, destroyed artifacts are "preserved" */
			if (preserve_mode || in_generate)
			{
				s16b this_o_idx, next_o_idx = 0;

				/* Scan all objects in the grid */
				for (this_o_idx = c_ptr->o_idx; this_o_idx; this_o_idx = next_o_idx)
				{
					object_type *o_ptr;

					/* Acquire object */
					o_ptr = &o_list[this_o_idx];

					/* Acquire next object */
					next_o_idx = o_ptr->next_o_idx;

					/* Hack -- Preserve unknown artifacts */
					if (object_is_fixed_artifact(o_ptr) && (!object_is_known(o_ptr) || in_generate))
					{
						/* Mega-Hack -- Preserve the artifact */
						a_info[o_ptr->name1].cur_num = 0;

						if (in_generate && cheat_peek)
						{
							char o_name[MAX_NLEN];
							object_desc(o_name, o_ptr, (OD_NAME_ONLY | OD_STORE));
							msg_format(_("����Υ����ƥ� (%s) ���������*�˲�*���줿��", "Artifact (%s) was *destroyed* during generation."), o_name);
						}
					}
					else if (in_generate && cheat_peek && o_ptr->art_name)
					{
						msg_print(_("�����ࡦ�����ƥ��ե����Ȥ�1�Ĥ��������*�˲�*���줿��", 
									"One of the random artifacts was *destroyed* during generation."));
					}
				}
			}

			/* Delete objects */
			delete_object(y, x);

			/* Destroy "non-permanent" grids */
			if (!cave_perma_grid(c_ptr))
			{
				/* Wall (or floor) type */
				t = randint0(200);

				if (!in_generate) /* Normal */
				{
					if (t < 20)
					{
						/* Create granite wall */
						cave_set_feat(y, x, feat_granite);
					}
					else if (t < 70)
					{
						/* Create quartz vein */
						cave_set_feat(y, x, feat_quartz_vein);
					}
					else if (t < 100)
					{
						/* Create magma vein */
						cave_set_feat(y, x, feat_magma_vein);
					}
					else
					{
						/* Create floor */
						cave_set_feat(y, x, floor_type[randint0(100)]);
					}
				}
				else /* In generation */
				{
					if (t < 20)
					{
						/* Create granite wall */
						place_extra_grid(c_ptr);
					}
					else if (t < 70)
					{
						/* Create quartz vein */
						c_ptr->feat = feat_quartz_vein;
					}
					else if (t < 100)
					{
						/* Create magma vein */
						c_ptr->feat = feat_magma_vein;
					}
					else
					{
						/* Create floor */
						place_floor_grid(c_ptr);
					}

					/* Clear garbage of hidden trap or door */
					c_ptr->mimic = 0;
				}
			}
		}
	}

	if (!in_generate)
	{
		/* Process "re-glowing" */
		for (y = (y1 - r); y <= (y1 + r); y++)
		{
			for (x = (x1 - r); x <= (x1 + r); x++)
			{
				/* Skip illegal grids */
				if (!in_bounds(y, x)) continue;

				/* Extract the distance */
				k = distance(y1, x1, y, x);

				/* Stay in the circle of death */
				if (k > r) continue;

				/* Access the grid */
				c_ptr = &cave[y][x];

				if (is_mirror_grid(c_ptr)) c_ptr->info |= CAVE_GLOW;
				else if (!(d_info[dungeon_type].flags1 & DF1_DARKNESS))
				{
					int i, yy, xx;
					cave_type *cc_ptr;

					for (i = 0; i < 9; i++)
					{
						yy = y + ddy_ddd[i];
						xx = x + ddx_ddd[i];
						if (!in_bounds2(yy, xx)) continue;
						cc_ptr = &cave[yy][xx];
						if (have_flag(f_info[get_feat_mimic(cc_ptr)].flags, FF_GLOW))
						{
							c_ptr->info |= CAVE_GLOW;
							break;
						}
					}
				}
			}
		}

		/* Hack -- Affect player */
		if (flag)
		{
			/* Message */
			msg_print(_("ǳ����褦��������ȯ��������", "There is a searing blast of light!"));

			/* Blind the player */
			if (!p_ptr->resist_blind && !p_ptr->resist_lite)
			{
				/* Become blind */
				(void)set_blind(p_ptr->blind + 10 + randint1(10));
			}
		}

		forget_flow();

		/* Mega-Hack -- Forget the view and lite */
		p_ptr->update |= (PU_UN_VIEW | PU_UN_LITE);

		/* Update stuff */
		p_ptr->update |= (PU_VIEW | PU_LITE | PU_FLOW | PU_MON_LITE | PU_MONSTERS);

		/* Redraw map */
		p_ptr->redraw |= (PR_MAP);

		/* Window stuff */
		p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);

		if (p_ptr->special_defense & NINJA_S_STEALTH)
		{
			if (cave[p_ptr->y][p_ptr->x].info & CAVE_GLOW) set_superstealth(FALSE);
		}
	}

	/* Success */
	return (TRUE);
}


/*!
 * @brief �Ͽ̽���(���֥롼����) /
 * Induce an "earthquake" of the given radius at the given location.
 * @return ���Ϥ����ä����TRUE���֤�
 * @param cy �濴Y��ɸ
 * @param cx �濴X��ɸ
 * @param r ����Ⱦ��
 * @param m_idx �Ͽ̤򵯤�������󥹥���ID(0�ʤ�Хץ쥤�䡼)
 * @details
 * <pre>
 *
 * This will turn some walls into floors and some floors into walls.
 *
 * The player will take damage and "jump" into a safe grid if possible,
 * otherwise, he will "tunnel" through the rubble instantaneously.
 *
 * Monsters will take damage, and "jump" into a safe grid if possible,
 * otherwise they will be "buried" in the rubble, disappearing from
 * the level in the same way that they do when genocided.
 *
 * Note that thus the player and monsters (except eaters of walls and
 * passers through walls) will never occupy the same grid as a wall.
 * Note that as of now (2.7.8) no monster may occupy a "wall" grid, even
 * for a single turn, unless that monster can pass_walls or kill_walls.
 * This has allowed massive simplification of the "monster" code.
 * </pre>
 */
bool earthquake_aux(int cy, int cx, int r, int m_idx)
{
	int             i, t, y, x, yy, xx, dy, dx;
	int             damage = 0;
	int             sn = 0, sy = 0, sx = 0;
	bool            hurt = FALSE;
	cave_type       *c_ptr;
	bool            map[32][32];


	/* Prevent destruction of quest levels and town */
	if ((p_ptr->inside_quest && is_fixed_quest_idx(p_ptr->inside_quest)) || !dun_level)
	{
		return (FALSE);
	}

	/* Paranoia -- Enforce maximum range */
	if (r > 12) r = 12;

	/* Clear the "maximal blast" area */
	for (y = 0; y < 32; y++)
	{
		for (x = 0; x < 32; x++)
		{
			map[y][x] = FALSE;
		}
	}

	/* Check around the epicenter */
	for (dy = -r; dy <= r; dy++)
	{
		for (dx = -r; dx <= r; dx++)
		{
			/* Extract the location */
			yy = cy + dy;
			xx = cx + dx;

			/* Skip illegal grids */
			if (!in_bounds(yy, xx)) continue;

			/* Skip distant grids */
			if (distance(cy, cx, yy, xx) > r) continue;

			/* Access the grid */
			c_ptr = &cave[yy][xx];

			/* Lose room and vault */
			c_ptr->info &= ~(CAVE_ROOM | CAVE_ICKY | CAVE_UNSAFE);

			/* Lose light and knowledge */
			c_ptr->info &= ~(CAVE_GLOW | CAVE_MARK | CAVE_KNOWN);

			/* Skip the epicenter */
			if (!dx && !dy) continue;

			/* Skip most grids */
			if (randint0(100) < 85) continue;

			/* Damage this grid */
			map[16+yy-cy][16+xx-cx] = TRUE;

			/* Hack -- Take note of player damage */
			if (player_bold(yy, xx)) hurt = TRUE;
		}
	}

	/* First, affect the player (if necessary) */
	if (hurt && !p_ptr->pass_wall && !p_ptr->kill_wall)
	{
		/* Check around the player */
		for (i = 0; i < 8; i++)
		{
			/* Access the location */
			y = p_ptr->y + ddy_ddd[i];
			x = p_ptr->x + ddx_ddd[i];

			/* Skip non-empty grids */
			if (!cave_empty_bold(y, x)) continue;

			/* Important -- Skip "quake" grids */
			if (map[16+y-cy][16+x-cx]) continue;

			if (cave[y][x].m_idx) continue;

			/* Count "safe" grids */
			sn++;

			/* Randomize choice */
			if (randint0(sn) > 0) continue;

			/* Save the safe location */
			sy = y; sx = x;
		}

		/* Random message */
		switch (randint1(3))
		{
			case 1:
			{
				msg_print(_("���󥸥����ɤ����줿��", "The cave ceiling collapses!"));
				break;
			}
			case 2:
			{
				msg_print(_("���󥸥��ξ����Լ����ˤͤ��ʤ��ä���", "The cave floor twists in an unnatural way!"));
				break;
			}
			default:
			{
				msg_print(_("���󥸥���ɤ줿�����줿�䤬Ƭ�˹ߤäƤ�����", "The cave quakes!  You are pummeled with debris!"));
				break;
			}
		}

		/* Hurt the player a lot */
		if (!sn)
		{
			/* Message and damage */
			msg_print(_("���ʤ��ϤҤɤ��������ä���", "You are severely crushed!"));
			damage = 200;
		}

		/* Destroy the grid, and push the player to safety */
		else
		{
			/* Calculate results */
			switch (randint1(3))
			{
				case 1:
				{
					msg_print(_("�ߤ�����򤦤ޤ��򤱤���", "You nimbly dodge the blast!"));
					damage = 0;
					break;
				}
				case 2:
				{
					msg_print(_("���Ф����ʤ���ľ�⤷��!", "You are bashed by rubble!"));
					damage = damroll(10, 4);
					(void)set_stun(p_ptr->stun + randint1(50));
					break;
				}
				case 3:
				{
					msg_print(_("���ʤ��Ͼ����ɤȤδ֤˶��ޤ�Ƥ��ޤä���", "You are crushed between the floor and ceiling!"));
					damage = damroll(10, 4);
					(void)set_stun(p_ptr->stun + randint1(50));
					break;
				}
			}

			/* Move the player to the safe location */
			(void)move_player_effect(sy, sx, MPE_DONT_PICKUP);
		}

		/* Important -- no wall on player */
		map[16+p_ptr->y-cy][16+p_ptr->x-cx] = FALSE;

		/* Take some damage */
		if (damage)
		{
			cptr killer;

			if (m_idx)
			{
				char m_name[80];
				monster_type *m_ptr = &m_list[m_idx];

				/* Get the monster's real name */
				monster_desc(m_name, m_ptr, MD_IGNORE_HALLU | MD_ASSUME_VISIBLE | MD_INDEF_VISIBLE);

				killer = format(_("%s�ε��������Ͽ�", "an earthquake caused by %s"), m_name);
			}
			else
			{
				killer = _("�Ͽ�", "an earthquake");
			}

			take_hit(DAMAGE_ATTACK, damage, killer, -1);
		}
	}

	/* Examine the quaked region */
	for (dy = -r; dy <= r; dy++)
	{
		for (dx = -r; dx <= r; dx++)
		{
			/* Extract the location */
			yy = cy + dy;
			xx = cx + dx;

			/* Skip unaffected grids */
			if (!map[16+yy-cy][16+xx-cx]) continue;

			/* Access the grid */
			c_ptr = &cave[yy][xx];

			if (c_ptr->m_idx == p_ptr->riding) continue;

			/* Process monsters */
			if (c_ptr->m_idx)
			{
				monster_type *m_ptr = &m_list[c_ptr->m_idx];
				monster_race *r_ptr = &r_info[m_ptr->r_idx];

				/* Quest monsters */
				if (r_ptr->flags1 & RF1_QUESTOR)
				{
					/* No wall on quest monsters */
					map[16+yy-cy][16+xx-cx] = FALSE;

					continue;
				}

				/* Most monsters cannot co-exist with rock */
				if (!(r_ptr->flags2 & (RF2_KILL_WALL)) &&
				    !(r_ptr->flags2 & (RF2_PASS_WALL)))
				{
					char m_name[80];

					/* Assume not safe */
					sn = 0;

					/* Monster can move to escape the wall */
					if (!(r_ptr->flags1 & (RF1_NEVER_MOVE)))
					{
						/* Look for safety */
						for (i = 0; i < 8; i++)
						{
							/* Access the grid */
							y = yy + ddy_ddd[i];
							x = xx + ddx_ddd[i];

							/* Skip non-empty grids */
							if (!cave_empty_bold(y, x)) continue;

							/* Hack -- no safety on glyph of warding */
							if (is_glyph_grid(&cave[y][x])) continue;
							if (is_explosive_rune_grid(&cave[y][x])) continue;

							/* ... nor on the Pattern */
							if (pattern_tile(y, x)) continue;

							/* Important -- Skip "quake" grids */
							if (map[16+y-cy][16+x-cx]) continue;

							if (cave[y][x].m_idx) continue;
							if (player_bold(y, x)) continue;

							/* Count "safe" grids */
							sn++;

							/* Randomize choice */
							if (randint0(sn) > 0) continue;

							/* Save the safe grid */
							sy = y; sx = x;
						}
					}

					/* Describe the monster */
					monster_desc(m_name, m_ptr, 0);

					/* Scream in pain */
					if (!ignore_unview || is_seen(m_ptr)) msg_format(_("%^s�϶��ˤǵ㤭��ᤤ����", "%^s wails out in pain!"), m_name);

					/* Take damage from the quake */
					damage = (sn ? damroll(4, 8) : (m_ptr->hp + 1));

					/* Monster is certainly awake */
					(void)set_monster_csleep(c_ptr->m_idx, 0);

					/* Apply damage directly */
					m_ptr->hp -= damage;

					/* Delete (not kill) "dead" monsters */
					if (m_ptr->hp < 0)
					{
						/* Message */
						if (!ignore_unview || is_seen(m_ptr)) 
							msg_format(_("%^s�ϴ��Ф�����Ƥ��ޤä���", "%^s is embedded in the rock!"), m_name);

						if (c_ptr->m_idx)
						{
							if (record_named_pet && is_pet(&m_list[c_ptr->m_idx]) && m_list[c_ptr->m_idx].nickname)
							{
								char m2_name[80];

								monster_desc(m2_name, m_ptr, MD_INDEF_VISIBLE);
								do_cmd_write_nikki(NIKKI_NAMED_PET, RECORD_NAMED_PET_EARTHQUAKE, m2_name);
							}
						}

						/* Delete the monster */
						delete_monster(yy, xx);

						/* No longer safe */
						sn = 0;
					}

					/* Hack -- Escape from the rock */
					if (sn)
					{
						int m_idx = cave[yy][xx].m_idx;

						/* Update the old location */
						cave[yy][xx].m_idx = 0;

						/* Update the new location */
						cave[sy][sx].m_idx = m_idx;

						/* Move the monster */
						m_ptr->fy = sy;
						m_ptr->fx = sx;

						/* Update the monster (new location) */
						update_mon(m_idx, TRUE);

						/* Redraw the old grid */
						lite_spot(yy, xx);

						/* Redraw the new grid */
						lite_spot(sy, sx);
					}
				}
			}
		}
	}

	/* Lose monster light */
	clear_mon_lite();

	/* Examine the quaked region */
	for (dy = -r; dy <= r; dy++)
	{
		for (dx = -r; dx <= r; dx++)
		{
			/* Extract the location */
			yy = cy + dy;
			xx = cx + dx;

			/* Skip unaffected grids */
			if (!map[16+yy-cy][16+xx-cx]) continue;

			/* Access the cave grid */
			c_ptr = &cave[yy][xx];

			/* Paranoia -- never affect player */
/*			if (player_bold(yy, xx)) continue; */

			/* Destroy location (if valid) */
			if (cave_valid_bold(yy, xx))
			{
				/* Delete objects */
				delete_object(yy, xx);

				/* Wall (or floor) type */
				t = cave_have_flag_bold(yy, xx, FF_PROJECT) ? randint0(100) : 200;

				/* Granite */
				if (t < 20)
				{
					/* Create granite wall */
					cave_set_feat(yy, xx, feat_granite);
				}

				/* Quartz */
				else if (t < 70)
				{
					/* Create quartz vein */
					cave_set_feat(yy, xx, feat_quartz_vein);
				}

				/* Magma */
				else if (t < 100)
				{
					/* Create magma vein */
					cave_set_feat(yy, xx, feat_magma_vein);
				}

				/* Floor */
				else
				{
					/* Create floor */
					cave_set_feat(yy, xx, floor_type[randint0(100)]);
				}
			}
		}
	}


	/* Process "re-glowing" */
	for (dy = -r; dy <= r; dy++)
	{
		for (dx = -r; dx <= r; dx++)
		{
			/* Extract the location */
			yy = cy + dy;
			xx = cx + dx;

			/* Skip illegal grids */
			if (!in_bounds(yy, xx)) continue;

			/* Skip distant grids */
			if (distance(cy, cx, yy, xx) > r) continue;

			/* Access the grid */
			c_ptr = &cave[yy][xx];

			if (is_mirror_grid(c_ptr)) c_ptr->info |= CAVE_GLOW;
			else if (!(d_info[dungeon_type].flags1 & DF1_DARKNESS))
			{
				int ii, yyy, xxx;
				cave_type *cc_ptr;

				for (ii = 0; ii < 9; ii++)
				{
					yyy = yy + ddy_ddd[ii];
					xxx = xx + ddx_ddd[ii];
					if (!in_bounds2(yyy, xxx)) continue;
					cc_ptr = &cave[yyy][xxx];
					if (have_flag(f_info[get_feat_mimic(cc_ptr)].flags, FF_GLOW))
					{
						c_ptr->info |= CAVE_GLOW;
						break;
					}
				}
			}
		}
	}


	/* Mega-Hack -- Forget the view and lite */
	p_ptr->update |= (PU_UN_VIEW | PU_UN_LITE);

	/* Update stuff */
	p_ptr->update |= (PU_VIEW | PU_LITE | PU_FLOW | PU_MON_LITE | PU_MONSTERS);

	/* Update the health bar */
	p_ptr->redraw |= (PR_HEALTH | PR_UHEALTH);

	/* Redraw map */
	p_ptr->redraw |= (PR_MAP);

	/* Window stuff */
	p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);

	if (p_ptr->special_defense & NINJA_S_STEALTH)
	{
		if (cave[p_ptr->y][p_ptr->x].info & CAVE_GLOW) set_superstealth(FALSE);
	}

	/* Success */
	return (TRUE);
}

/*!
 * @brief �Ͽ̽���(�ץ쥤�䡼���濴ȯư) /
 * Induce an "earthquake" of the given radius at the given location.
 * @return ���Ϥ����ä����TRUE���֤�
 * @param cy �濴Y��ɸ
 * @param cx �濴X��ɸ
 * @param r ����Ⱦ��
 */
bool earthquake(int cy, int cx, int r)
{
	return earthquake_aux(cy, cx, r, 0);
}

/*!
 * @brief �ڥå����˽��� /
 * @return �ʤ�
 */
void discharge_minion(void)
{
	int i;
	bool okay = TRUE;

	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];
		if (!m_ptr->r_idx || !is_pet(m_ptr)) continue;
		if (m_ptr->nickname) okay = FALSE;
	}
	if (!okay || p_ptr->riding)
	{
		if (!get_check(_("���������ڥåȤ����ˤ��ޤ�����", "You will blast all pets. Are you sure? ")))
			return;
	}
	for (i = 1; i < m_max; i++)
	{
		int dam;
		monster_type *m_ptr = &m_list[i];
		monster_race *r_ptr;

		if (!m_ptr->r_idx || !is_pet(m_ptr)) continue;
		r_ptr = &r_info[m_ptr->r_idx];

		/* Uniques resist discharging */
		if (r_ptr->flags1 & RF1_UNIQUE)
		{
			char m_name[80];
			monster_desc(m_name, m_ptr, 0x00);
			msg_format(_("%s�����ˤ����Τ�����ꡢ����˼�ʬ�������ؤȵ��ä���", "%^s resists to be blasted, and run away."), m_name);
			delete_monster_idx(i);
			continue;
		}
		dam = m_ptr->maxhp / 2;
		if (dam > 100) dam = (dam-100)/2 + 100;
		if (dam > 400) dam = (dam-400)/2 + 400;
		if (dam > 800) dam = 800;
		project(i, 2+(r_ptr->level/20), m_ptr->fy,
			m_ptr->fx, dam, GF_PLASMA, 
			PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL, -1);

		if (record_named_pet && m_ptr->nickname)
		{
			char m_name[80];

			monster_desc(m_name, m_ptr, MD_INDEF_VISIBLE);
			do_cmd_write_nikki(NIKKI_NAMED_PET, RECORD_NAMED_PET_BLAST, m_name);
		}

		delete_monster_idx(i);
	}
}


/*!
 * @brief �������Τ�Ȥ餹���֥롼����
 * @return �ʤ�
 * @details
 * <pre>
 * This routine clears the entire "temp" set.
 * This routine will Perma-Lite all "temp" grids.
 * This routine is used (only) by "lite_room()"
 * Dark grids are illuminated.
 * Also, process all affected monsters.
 *
 * SMART monsters always wake up when illuminated
 * NORMAL monsters wake up 1/4 the time when illuminated
 * STUPID monsters wake up 1/10 the time when illuminated
 * </pre>
 */
static void cave_temp_room_lite(void)
{
	int i;

	/* Clear them all */
	for (i = 0; i < temp_n; i++)
	{
		int y = temp_y[i];
		int x = temp_x[i];

		cave_type *c_ptr = &cave[y][x];

		/* No longer in the array */
		c_ptr->info &= ~(CAVE_TEMP);

		/* Update only non-CAVE_GLOW grids */
		/* if (c_ptr->info & (CAVE_GLOW)) continue; */

		/* Perma-Lite */
		c_ptr->info |= (CAVE_GLOW);

		/* Process affected monsters */
		if (c_ptr->m_idx)
		{
			int chance = 25;

			monster_type    *m_ptr = &m_list[c_ptr->m_idx];

			monster_race    *r_ptr = &r_info[m_ptr->r_idx];

			/* Update the monster */
			update_mon(c_ptr->m_idx, FALSE);

			/* Stupid monsters rarely wake up */
			if (r_ptr->flags2 & (RF2_STUPID)) chance = 10;

			/* Smart monsters always wake up */
			if (r_ptr->flags2 & (RF2_SMART)) chance = 100;

			/* Sometimes monsters wake up */
			if (MON_CSLEEP(m_ptr) && (randint0(100) < chance))
			{
				/* Wake up! */
				(void)set_monster_csleep(c_ptr->m_idx, 0);

				/* Notice the "waking up" */
				if (m_ptr->ml)
				{
					char m_name[80];

					/* Acquire the monster name */
					monster_desc(m_name, m_ptr, 0);

					/* Dump a message */
					msg_format(_("%^s���ܤ�Фޤ�����", "%^s wakes up."), m_name);
				}
			}
		}

		/* Note */
		note_spot(y, x);

		/* Redraw */
		lite_spot(y, x);

		update_local_illumination(y, x);
	}

	/* None left */
	temp_n = 0;
}



/*!
 * @brief �������Τ�Ť����륵�֥롼����
 * @return �ʤ�
 * @details
 * <pre>
 * This routine clears the entire "temp" set.
 * This routine will "darken" all "temp" grids.
 * In addition, some of these grids will be "unmarked".
 * This routine is used (only) by "unlite_room()"
 * Also, process all affected monsters
 * </pre>
 */
static void cave_temp_room_unlite(void)
{
	int i;

	/* Clear them all */
	for (i = 0; i < temp_n; i++)
	{
		int y = temp_y[i];
		int x = temp_x[i];
		int j;

		cave_type *c_ptr = &cave[y][x];
		bool do_dark = !is_mirror_grid(c_ptr);

		/* No longer in the array */
		c_ptr->info &= ~(CAVE_TEMP);

		/* Darken the grid */
		if (do_dark)
		{
			if (dun_level || !is_daytime())
			{
				for (j = 0; j < 9; j++)
				{
					int by = y + ddy_ddd[j];
					int bx = x + ddx_ddd[j];

					if (in_bounds2(by, bx))
					{
						cave_type *cc_ptr = &cave[by][bx];

						if (have_flag(f_info[get_feat_mimic(cc_ptr)].flags, FF_GLOW))
						{
							do_dark = FALSE;
							break;
						}
					}
				}

				if (!do_dark) continue;
			}

			c_ptr->info &= ~(CAVE_GLOW);

			/* Hack -- Forget "boring" grids */
			if (!have_flag(f_info[get_feat_mimic(c_ptr)].flags, FF_REMEMBER))
			{
				/* Forget the grid */
				if (!view_torch_grids) c_ptr->info &= ~(CAVE_MARK);

				/* Notice */
				note_spot(y, x);
			}

			/* Process affected monsters */
			if (c_ptr->m_idx)
			{
				/* Update the monster */
				update_mon(c_ptr->m_idx, FALSE);
			}

			/* Redraw */
			lite_spot(y, x);

			update_local_illumination(y, x);
		}
	}

	/* None left */
	temp_n = 0;
}


/*!
 * @brief ���դ˴ؿ��ݥ��󥿤ξ��˳��������Ϸ��������Ĥ��뤫��׻����� / Determine how much contiguous open space this grid is next to
 * @param cy Y��ɸ
 * @param cx X��ɸ
 * @param pass_bold �Ϸ������֤��ؿ��ݥ���
 * @return �����Ϸ��ο�
 */
static int next_to_open(int cy, int cx, bool (*pass_bold)(int, int))
{
	int i;

	int y, x;

	int len = 0;
	int blen = 0;

	for (i = 0; i < 16; i++)
	{
		y = cy + ddy_cdd[i % 8];
		x = cx + ddx_cdd[i % 8];

		/* Found a wall, break the length */
		if (!pass_bold(y, x))
		{
			/* Track best length */
			if (len > blen)
			{
				blen = len;
			}

			len = 0;
		}
		else
		{
			len++;
		}
	}

	return (MAX(len, blen));
}

/*!
 * @brief ���դ˴ؿ��ݥ��󥿤ξ��˳��������Ϸ��������Ĥ��뤫��׻����� / Determine how much contiguous open space this grid is next to
 * @param cy Y��ɸ
 * @param cx X��ɸ
 * @param pass_bold �Ϸ������֤��ؿ��ݥ���
 * @return �����Ϸ��ο�
 */
static int next_to_walls_adj(int cy, int cx, bool (*pass_bold)(int, int))
{
	int i;

	int y, x;

	int c = 0;

	for (i = 0; i < 8; i++)
	{
		y = cy + ddy_ddd[i];
		x = cx + ddx_ddd[i];

		if (!pass_bold(y, x)) c++;
	}

	return c;
}


/*!
 * @brief ������ˤ�������μ��Ϥ˳��������Ϸ����������Ĥ��뤫�򥰥��Х��ѿ�temp_n���֤� / Aux function -- see below
 * @param y �������y��ɸ1��
 * @param x �������x��ɸ1��
 * @param only_room �������Ϸ��Τߤ�����å��оݤˤ���ʤ�� TRUE
 * @param pass_bold �Ϸ������֤��ؿ��ݥ���
 * @return �����Ϸ��ο�
 */
static void cave_temp_room_aux(int y, int x, bool only_room, bool (*pass_bold)(int, int))
{
	cave_type *c_ptr;

	/* Get the grid */
	c_ptr = &cave[y][x];

	/* Avoid infinite recursion */
	if (c_ptr->info & (CAVE_TEMP)) return;

	/* Do not "leave" the current room */
	if (!(c_ptr->info & (CAVE_ROOM)))
	{
		if (only_room) return;

		/* Verify */
		if (!in_bounds2(y, x)) return;

		/* Do not exceed the maximum spell range */
		if (distance(p_ptr->y, p_ptr->x, y, x) > MAX_RANGE) return;

		/* Verify this grid */
		/*
		 * The reason why it is ==6 instead of >5 is that 8 is impossible
		 * due to the check for cave_bold above.
		 * 7 lights dead-end corridors (you need to do this for the
		 * checkboard interesting rooms, so that the boundary is lit
		 * properly.
		 * This leaves only a check for 6 bounding walls!
		 */
		if (in_bounds(y, x) && pass_bold(y, x) &&
		    (next_to_walls_adj(y, x, pass_bold) == 6) && (next_to_open(y, x, pass_bold) <= 1)) return;
	}

	/* Paranoia -- verify space */
	if (temp_n == TEMP_MAX) return;

	/* Mark the grid as "seen" */
	c_ptr->info |= (CAVE_TEMP);

	/* Add it to the "seen" set */
	temp_y[temp_n] = y;
	temp_x[temp_n] = x;
	temp_n++;
}

/*!
 * @brief ����Υޥ��������̤���(LOS�ե饰����Ĥ�)���֤��� / Aux function -- see below
 * @param y ����Y��ɸ
 * @param x ����X��ɸ
 * @return �����̤��ʤ��true���֤���
 */
static bool cave_pass_lite_bold(int y, int x)
{
	return cave_los_bold(y, x);
}

/*!
 * @brief ������ˤ�������μ��Ϥ������ĸ����̤����򥰥��Х��ѿ�temp_n���֤� / Aux function -- see below
 * @param y ����Y��ɸ
 * @param x ����X��ɸ
 * @return �ʤ�
 */
static void cave_temp_lite_room_aux(int y, int x)
{
	cave_temp_room_aux(y, x, FALSE, cave_pass_lite_bold);
}

/*!
 * @brief ����Υޥ��������̤��������Τߤ��̤������֤��� / Aux function -- see below
 * @param y ����Y��ɸ
 * @param x ����X��ɸ
 * @return �������̤��ʤ��true���֤���
 */
static bool cave_pass_dark_bold(int y, int x)
{
	return cave_have_flag_bold(y, x, FF_PROJECT);
}


/*!
 * @brief ������ˤ�������μ��Ϥ������ļ������̤����򥰥��Х��ѿ�temp_n���֤� / Aux function -- see below
 * @param y ����Y��ɸ
 * @param x ����X��ɸ
 * @return �ʤ�
 */
static void cave_temp_unlite_room_aux(int y, int x)
{
	cave_temp_room_aux(y, x, TRUE, cave_pass_dark_bold);
}


/*!
 * @brief ���ꤵ�줿�������Ȥ餹 / Illuminate any room containing the given location.
 * @param y1 ����Y��ɸ
 * @param x1 ����X��ɸ
 * @return �ʤ�
 */
void lite_room(int y1, int x1)
{
	int i, x, y;

	/* Add the initial grid */
	cave_temp_lite_room_aux(y1, x1);

	/* While grids are in the queue, add their neighbors */
	for (i = 0; i < temp_n; i++)
	{
		x = temp_x[i], y = temp_y[i];

		/* Walls get lit, but stop light */
		if (!cave_pass_lite_bold(y, x)) continue;

		/* Spread adjacent */
		cave_temp_lite_room_aux(y + 1, x);
		cave_temp_lite_room_aux(y - 1, x);
		cave_temp_lite_room_aux(y, x + 1);
		cave_temp_lite_room_aux(y, x - 1);

		/* Spread diagonal */
		cave_temp_lite_room_aux(y + 1, x + 1);
		cave_temp_lite_room_aux(y - 1, x - 1);
		cave_temp_lite_room_aux(y - 1, x + 1);
		cave_temp_lite_room_aux(y + 1, x - 1);
	}

	/* Now, lite them all up at once */
	cave_temp_room_lite();

	if (p_ptr->special_defense & NINJA_S_STEALTH)
	{
		if (cave[p_ptr->y][p_ptr->x].info & CAVE_GLOW) set_superstealth(FALSE);
	}
}


/*!
 * @brief ���ꤵ�줿�������Ť����� / Darken all rooms containing the given location
 * @param y1 ����Y��ɸ
 * @param x1 ����X��ɸ
 * @return �ʤ�
 */
void unlite_room(int y1, int x1)
{
	int i, x, y;

	/* Add the initial grid */
	cave_temp_unlite_room_aux(y1, x1);

	/* Spread, breadth first */
	for (i = 0; i < temp_n; i++)
	{
		x = temp_x[i], y = temp_y[i];

		/* Walls get dark, but stop darkness */
		if (!cave_pass_dark_bold(y, x)) continue;

		/* Spread adjacent */
		cave_temp_unlite_room_aux(y + 1, x);
		cave_temp_unlite_room_aux(y - 1, x);
		cave_temp_unlite_room_aux(y, x + 1);
		cave_temp_unlite_room_aux(y, x - 1);

		/* Spread diagonal */
		cave_temp_unlite_room_aux(y + 1, x + 1);
		cave_temp_unlite_room_aux(y - 1, x - 1);
		cave_temp_unlite_room_aux(y - 1, x + 1);
		cave_temp_unlite_room_aux(y + 1, x - 1);
	}

	/* Now, darken them all at once */
	cave_temp_room_unlite();
}



/*!
 * @brief �ץ쥤�䡼���֤��濴��LITE_WEAK°�����̤�������������Ԥ� / Hack -- call light around the player Affect all monsters in the projection radius
 * @param dam ����
 * @param rad ����Ⱦ��
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool lite_area(int dam, int rad)
{
	int flg = PROJECT_GRID | PROJECT_KILL;

	if (d_info[dungeon_type].flags1 & DF1_DARKNESS)
	{
		msg_print(_("���󥸥�󤬸���ۼ�������", "The darkness of this dungeon absorb your light."));
		return FALSE;
	}

	/* Hack -- Message */
	if (!p_ptr->blind)
	{
		msg_print(_("�򤤸����դ��ʤ�ä���", "You are surrounded by a white light."));
	}

	/* Hook into the "project()" function */
	(void)project(0, rad, p_ptr->y, p_ptr->x, dam, GF_LITE_WEAK, flg, -1);

	/* Lite up the room */
	lite_room(p_ptr->y, p_ptr->x);

	/* Assume seen */
	return (TRUE);
}


/*!
 * @brief �ץ쥤�䡼���֤��濴��LITE_DARK°�����̤�������������Ԥ� / Hack -- call light around the player Affect all monsters in the projection radius
 * @param dam ����
 * @param rad ����Ⱦ��
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool unlite_area(int dam, int rad)
{
	int flg = PROJECT_GRID | PROJECT_KILL;

	/* Hack -- Message */
	if (!p_ptr->blind)
	{
		msg_print(_("�ŰǤ��դ��ʤ�ä���", "Darkness surrounds you."));
	}

	/* Hook into the "project()" function */
	(void)project(0, rad, p_ptr->y, p_ptr->x, dam, GF_DARK_WEAK, flg, -1);

	/* Lite up the room */
	unlite_room(p_ptr->y, p_ptr->x);

	/* Assume seen */
	return (TRUE);
}



/*!
 * @brief �ܡ���ϥ��ڥ��ȯư / Cast a ball spell
 * @param typ ����°��
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param dam ����
 * @param rad Ⱦ��
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 * @details
 * <pre>
 * Stop if we hit a monster, act as a "ball"
 * Allow "target" mode to pass over monsters
 * Affect grids, objects, and monsters
 * </pre>
 */
bool fire_ball(int typ, int dir, int dam, int rad)
{
	int tx, ty;

	int flg = PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL;

	if (typ == GF_CONTROL_LIVING) flg|= PROJECT_HIDE;
	/* Use the given direction */
	tx = p_ptr->x + 99 * ddx[dir];
	ty = p_ptr->y + 99 * ddy[dir];

	/* Hack -- Use an actual "target" */
	if ((dir == 5) && target_okay())
	{
		flg &= ~(PROJECT_STOP);
		tx = target_col;
		ty = target_row;
	}

	/* Analyze the "dir" and the "target".  Hurt items on floor. */
	return (project(0, rad, ty, tx, dam, typ, flg, -1));
}



/*!
 * @brief ���åȷϥ��ڥ��ȯư(�ܺ٤ʺ��ϳ�ǧ��) / Cast a ball spell
 * @param typ ����°��
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param dam ����
 * @param rad Ⱦ��
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 * @details
 * <pre>
 * Stop if we hit a monster, act as a "ball"
 * Allow "target" mode to pass over monsters
 * Affect grids, objects, and monsters
 * </pre>
 */
bool fire_rocket(int typ, int dir, int dam, int rad)
{
	int tx, ty;

	int flg = PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL;

	/* Use the given direction */
	tx = p_ptr->x + 99 * ddx[dir];
	ty = p_ptr->y + 99 * ddy[dir];

	/* Hack -- Use an actual "target" */
	if ((dir == 5) && target_okay())
	{
		tx = target_col;
		ty = target_row;
	}

	/* Analyze the "dir" and the "target".  Hurt items on floor. */
	return (project(0, rad, ty, tx, dam, typ, flg, -1));
}


/*!
 * @brief �ܡ���(�ϥ���)�ϥ��ڥ��ȯư / Cast a ball spell
 * @param typ ����°��
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param dam ����
 * @param rad Ⱦ��
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 * @details
 * <pre>
 * Stop if we hit a monster, act as a "ball"
 * Allow "target" mode to pass over monsters
 * Affect grids, objects, and monsters
 * </pre>
 */
bool fire_ball_hide(int typ, int dir, int dam, int rad)
{
	int tx, ty;

	int flg = PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL | PROJECT_HIDE;

	/* Use the given direction */
	tx = p_ptr->x + 99 * ddx[dir];
	ty = p_ptr->y + 99 * ddy[dir];

	/* Hack -- Use an actual "target" */
	if ((dir == 5) && target_okay())
	{
		flg &= ~(PROJECT_STOP);
		tx = target_col;
		ty = target_row;
	}

	/* Analyze the "dir" and the "target".  Hurt items on floor. */
	return (project(0, rad, ty, tx, dam, typ, flg, -1));
}


/*!
 * @brief ��ƥ��ϥ��ڥ��ȯư / Cast a meteor spell
 * @param who ���ڥ�Ӿ��ԤΥ�󥹥���ID(0=�ץ쥤�䡼)
 * @param typ ����°��
 * @param dam ����
 * @param rad Ⱦ��
 * @param y �濴��Y��ɸ
 * @param x �濴��X��ɸ
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 * @details
 * <pre>
 * Cast a meteor spell, defined as a ball spell cast by an arbitary monster, 
 * player, or outside source, that starts out at an arbitrary location, and 
 * leaving no trail from the "caster" to the target.  This function is 
 * especially useful for bombardments and similar. -LM-
 * Option to hurt the player.
 * </pre>
 */
bool fire_meteor(int who, int typ, int y, int x, int dam, int rad)
{
	int flg = PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL;

	/* Analyze the "target" and the caster. */
	return (project(who, rad, y, x, dam, typ, flg, -1));
}


/*!
 * @brief �֥饹�ȷϥ��ڥ��ȯư / Cast a blast spell
 * @param typ ����°��
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param dd ���ϥ�������
 * @param ds ���ϥ�������
 * @param num ���ܲ��
 * @param dev ���ʬ��
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool fire_blast(int typ, int dir, int dd, int ds, int num, int dev)
{
	int ly, lx, ld;
	int ty, tx, y, x;
	int i;

	int flg = PROJECT_FAST | PROJECT_THRU | PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE | PROJECT_GRID;

	/* Assume okay */
	bool result = TRUE;

	/* Use the given direction */
	if (dir != 5)
	{
		ly = ty = p_ptr->y + 20 * ddy[dir];
		lx = tx = p_ptr->x + 20 * ddx[dir];
	}

	/* Use an actual "target" */
	else /* if (dir == 5) */
	{
		tx = target_col;
		ty = target_row;

		lx = 20 * (tx - p_ptr->x) + p_ptr->x;
		ly = 20 * (ty - p_ptr->y) + p_ptr->y;
	}

	ld = distance(p_ptr->y, p_ptr->x, ly, lx);

	/* Blast */
	for (i = 0; i < num; i++)
	{
		while (1)
		{
			/* Get targets for some bolts */
			y = rand_spread(ly, ld * dev / 20);
			x = rand_spread(lx, ld * dev / 20);

			if (distance(ly, lx, y, x) <= ld * dev / 20) break;
		}

		/* Analyze the "dir" and the "target". */
		if (!project(0, 0, y, x, damroll(dd, ds), typ, flg, -1))
		{
			result = FALSE;
		}
	}

	return (result);
}


/*!
 * @brief ��󥹥����Ȥΰ��ָ򴹽��� / Switch position with a monster.
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool teleport_swap(int dir)
{
	int tx, ty;
	cave_type * c_ptr;
	monster_type * m_ptr;
	monster_race * r_ptr;

	if ((dir == 5) && target_okay())
	{
		tx = target_col;
		ty = target_row;
	}
	else
	{
		tx = p_ptr->x + ddx[dir];
		ty = p_ptr->y + ddy[dir];
	}
	c_ptr = &cave[ty][tx];

	if (p_ptr->anti_tele)
	{
		msg_print(_("�Ի׵Ĥ��Ϥ��ƥ�ݡ��Ȥ��ɤ�����", "A mysterious force prevents you from teleporting!"));
		return FALSE;
	}

	if (!c_ptr->m_idx || (c_ptr->m_idx == p_ptr->riding))
	{
		msg_print(_("����ȤϾ���򴹤Ǥ��ޤ���", "You can't trade places with that!"));

		/* Failure */
		return FALSE;
	}

	if ((c_ptr->info & CAVE_ICKY) || (distance(ty, tx, p_ptr->y, p_ptr->x) > p_ptr->lev * 3 / 2 + 10))
	{
		msg_print(_("���Ԥ�����", "Failed to swap."));

		/* Failure */
		return FALSE;
	}

	m_ptr = &m_list[c_ptr->m_idx];
	r_ptr = &r_info[m_ptr->r_idx];

	(void)set_monster_csleep(c_ptr->m_idx, 0);

	if (r_ptr->flagsr & RFR_RES_TELE)
	{
		msg_print(_("�ƥ�ݡ��Ȥ���⤵�줿��", "Your teleportation is blocked!"));

		if (is_original_ap_and_seen(m_ptr)) r_ptr->r_flagsr |= RFR_RES_TELE;

		/* Failure */
		return FALSE;
	}

	sound(SOUND_TELEPORT);

	/* Swap the player and monster */
	(void)move_player_effect(ty, tx, MPE_FORGET_FLOW | MPE_HANDLE_STUFF | MPE_DONT_PICKUP);

	/* Success */
	return TRUE;
}


/*!
 * @brief ��������������ƻ������Ф��ʥե饰Ǥ�ջ���� / Hack -- apply a "projection()" in a direction (or at the target)
 * @param typ ����°��
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param dam ����
 * @param flg �ե饰
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool project_hook(int typ, int dir, int dam, int flg)
{
	int tx, ty;

	/* Pass through the target if needed */
	flg |= (PROJECT_THRU);

	/* Use the given direction */
	tx = p_ptr->x + ddx[dir];
	ty = p_ptr->y + ddy[dir];

	/* Hack -- Use an actual "target" */
	if ((dir == 5) && target_okay())
	{
		tx = target_col;
		ty = target_row;
	}

	/* Analyze the "dir" and the "target", do NOT explode */
	return (project(0, 0, ty, tx, dam, typ, flg, -1));
}


/*!
 * @brief �ܥ�ȷϥ��ڥ��ȯư / Cast a bolt spell.
 * @param typ ����°��
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param dam ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 * @details
 * <pre>
 * Stop if we hit a monster, as a "bolt".
 * Affect monsters and grids (not objects).
 * </pre>
 */
bool fire_bolt(int typ, int dir, int dam)
{
	int flg = PROJECT_STOP | PROJECT_KILL | PROJECT_GRID;
	if (typ != GF_ARROW) flg |= PROJECT_REFLECTABLE;
	return (project_hook(typ, dir, dam, flg));
}


/*!
 * @brief �ӡ���ϥ��ڥ��ȯư / Cast a beam spell.
 * @param typ ����°��
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param dam ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 * @details
 * <pre>
 * Pass through monsters, as a "beam".
 * Affect monsters, grids and objects.
 * </pre>
 */
bool fire_beam(int typ, int dir, int dam)
{
	int flg = PROJECT_BEAM | PROJECT_KILL | PROJECT_GRID | PROJECT_ITEM;
	return (project_hook(typ, dir, dam, flg));
}


/*!
 * @brief ��Ψ�˱������ܥ�ȷ�/�ӡ���ϥ��ڥ��ȯư / Cast a bolt spell, or rarely, a beam spell.
 * @param prob �ӡ��ಽ�����Ψ(%)
 * @param typ ����°��
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param dam ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 * @details
 * <pre>
 * Pass through monsters, as a "beam".
 * Affect monsters, grids and objects.
 * </pre>
 */
bool fire_bolt_or_beam(int prob, int typ, int dir, int dam)
{
	if (randint0(100) < prob)
	{
		return (fire_beam(typ, dir, dam));
	}
	else
	{
		return (fire_bolt(typ, dir, dam));
	}
}

/*!
 * @brief LITE_WEAK°���ˤ������ӡ������
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param dam ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool lite_line(int dir, int dam)
{
	int flg = PROJECT_BEAM | PROJECT_GRID | PROJECT_KILL;
	return (project_hook(GF_LITE_WEAK, dir, dam, flg));
}

/*!
 * @brief �۷�ܥ�Ƚ���
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param dam ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool drain_life(int dir, int dam)
{
	int flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_OLD_DRAIN, dir, dam, flg));
}

/*!
 * @brief �����ϲ����
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param dam ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool wall_to_mud(int dir, int dam)
{
	int flg = PROJECT_BEAM | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL;
	return (project_hook(GF_KILL_WALL, dir, dam, flg));
}

/*!
 * @brief ��ˡ�λܾ�����
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool wizard_lock(int dir)
{
	int flg = PROJECT_BEAM | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL;
	return (project_hook(GF_JAM_DOOR, dir, 20 + randint1(30), flg));
}

/*!
 * @brief �ɥ��˲�����
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool destroy_door(int dir)
{
	int flg = PROJECT_BEAM | PROJECT_GRID | PROJECT_ITEM;
	return (project_hook(GF_KILL_DOOR, dir, 0, flg));
}

/*!
 * @brief �ȥ�åײ������
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool disarm_trap(int dir)
{
	int flg = PROJECT_BEAM | PROJECT_GRID | PROJECT_ITEM;
	return (project_hook(GF_KILL_TRAP, dir, 0, flg));
}

/*!
 * @brief ��󥹥�����������
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param dam ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool heal_monster(int dir, int dam)
{
	int flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_OLD_HEAL, dir, dam, flg));
}

/*!
 * @brief ��󥹥�����®����
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param power ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool speed_monster(int dir, int power)
{
	int flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_OLD_SPEED, dir, power, flg));
}

/*!
 * @brief ��󥹥�����®����
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param power ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool slow_monster(int dir, int power)
{
	int flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_OLD_SLOW, dir, power, flg));
}

/*!
 * @brief ��󥹥�����̲����
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param power ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool sleep_monster(int dir, int power)
{
	int flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_OLD_SLEEP, dir, power, flg));
}

/*!
 * @brief ��󥹥�����«(STASIS)����
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 * @details ���Ϥϥץ쥤�䡼��٥�*2�˸���
 */
bool stasis_monster(int dir)
{
	return (fire_ball_hide(GF_STASIS, dir, p_ptr->lev*2, 0));
}

/*!
 * @brief �ٰ��ʥ�󥹥�����«(STASIS)����
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 * @details ���Ϥϥץ쥤�䡼��٥�*2�˸���
 */
bool stasis_evil(int dir)
{
	return (fire_ball_hide(GF_STASIS_EVIL, dir, p_ptr->lev*2, 0));
}

/*!
 * @brief ��󥹥����������
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param plev �ץ쥤�䡼��٥�(=����)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool confuse_monster(int dir, int plev)
{
	int flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_OLD_CONF, dir, plev, flg));
}

/*!
 * @brief ��󥹥���ۯ۰����
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param plev �ץ쥤�䡼��٥�(=����)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool stun_monster(int dir, int plev)
{
	int flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_STUN, dir, plev, flg));
}

/*!
 * @brief �����󥸥�󥹥�������
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param power ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool poly_monster(int dir, int power)
{
	int flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	bool tester = (project_hook(GF_OLD_POLY, dir, power, flg));
	if (tester)
		chg_virtue(V_CHANCE, 1);
	return(tester);
}

/*!
 * @brief �������󥹥�������
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool clone_monster(int dir)
{
	int flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_OLD_CLONE, dir, 0, flg));
}

/*!
 * @brief ��󥹥�����������
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param plev �ץ쥤�䡼��٥�(=����)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool fear_monster(int dir, int plev)
{
	int flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_TURN_ALL, dir, plev, flg));
}

/*!
 * @brief ��θ�������
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param plev �ץ쥤�䡼��٥�(���Ϥ�plev*200)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool death_ray(int dir, int plev)
{
	int flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_DEATH_RAY, dir, plev * 200, flg));
}

/*!
 * @brief ��󥹥����ѥƥ�ݡ��Ƚ���
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param distance ��ư��Υ
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool teleport_monster(int dir, int distance)
{
	int flg = PROJECT_BEAM | PROJECT_KILL;
	return (project_hook(GF_AWAY_ALL, dir, distance, flg));
}

/*!
 * @brief �ɥ���������(�ץ쥤�䡼�濴�˼���1�ޥ�) / Hooks -- affect adjacent grids (radius 1 ball attack)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool door_creation(void)
{
	int flg = PROJECT_GRID | PROJECT_ITEM | PROJECT_HIDE;
	return (project(0, 1, p_ptr->y, p_ptr->x, 0, GF_MAKE_DOOR, flg, -1));
}

/*!
 * @brief �ȥ�å���������(�����������1�ޥ�)
 * @param y ����Y��ɸ
 * @param x ����X��ɸ
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool trap_creation(int y, int x)
{
	int flg = PROJECT_GRID | PROJECT_ITEM | PROJECT_HIDE;
	return (project(0, 1, y, x, 0, GF_MAKE_TRAP, flg, -1));
}

/*!
 * @brief ������������(�ץ쥤�䡼�濴�˼���1�ޥ�)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool tree_creation(void)
{
	int flg = PROJECT_GRID | PROJECT_ITEM | PROJECT_HIDE;
	return (project(0, 1, p_ptr->y, p_ptr->x, 0, GF_MAKE_TREE, flg, -1));
}

/*!
 * @brief ��ˡ�Υ롼����������(�ץ쥤�䡼�濴�˼���1�ޥ�)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool glyph_creation(void)
{
	int flg = PROJECT_GRID | PROJECT_ITEM;
	return (project(0, 1, p_ptr->y, p_ptr->x, 0, GF_MAKE_GLYPH, flg, -1));
}

/*!
 * @brief ����������(�ץ쥤�䡼�濴�˼���1�ޥ�)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool wall_stone(void)
{
	int flg = PROJECT_GRID | PROJECT_ITEM | PROJECT_HIDE;

	bool dummy = (project(0, 1, p_ptr->y, p_ptr->x, 0, GF_STONE_WALL, flg, -1));

	/* Update stuff */
	p_ptr->update |= (PU_FLOW);

	/* Redraw map */
	p_ptr->redraw |= (PR_MAP);

	return dummy;
}

/*!
 * @brief �ɥ��˲�����(�ץ쥤�䡼�濴�˼���1�ޥ�)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool destroy_doors_touch(void)
{
	int flg = PROJECT_GRID | PROJECT_ITEM | PROJECT_HIDE;
	return (project(0, 1, p_ptr->y, p_ptr->x, 0, GF_KILL_DOOR, flg, -1));
}

/*!
 * @brief �ȥ�åײ������(�ץ쥤�䡼�濴�˼���1�ޥ�)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool disarm_traps_touch(void)
{
	int flg = PROJECT_GRID | PROJECT_ITEM | PROJECT_HIDE;
	return (project(0, 1, p_ptr->y, p_ptr->x, 0, GF_KILL_TRAP, flg, -1));
}

/*!
 * @brief ���꡼�ץ�󥹥�������(�ץ쥤�䡼�濴�˼���1�ޥ�)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool sleep_monsters_touch(void)
{
	int flg = PROJECT_KILL | PROJECT_HIDE;
	return (project(0, 1, p_ptr->y, p_ptr->x, p_ptr->lev, GF_OLD_SLEEP, flg, -1));
}


/*!
 * @brief ����������(����������5�ޥ�)
 * @param who �Ѽԥ�󥹥���ID(0�ʤ�Хץ쥤�䡼)
 * @param y ����Y��ɸ
 * @param x ����X��ɸ
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool animate_dead(int who, int y, int x)
{
	int flg = PROJECT_ITEM | PROJECT_HIDE;
	return (project(who, 5, y, x, 0, GF_ANIM_DEAD, flg, -1));
}

/*!
 * @brief ���پ������
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
void call_chaos(void)
{
	int Chaos_type, dummy, dir;
	int plev = p_ptr->lev;
	bool line_chaos = FALSE;

	int hurt_types[31] =
	{
		GF_ELEC,      GF_POIS,    GF_ACID,    GF_COLD,
		GF_FIRE,      GF_MISSILE, GF_ARROW,   GF_PLASMA,
		GF_HOLY_FIRE, GF_WATER,   GF_LITE,    GF_DARK,
		GF_FORCE,     GF_INERTIAL, GF_MANA,    GF_METEOR,
		GF_ICE,       GF_CHAOS,   GF_NETHER,  GF_DISENCHANT,
		GF_SHARDS,    GF_SOUND,   GF_NEXUS,   GF_CONFUSION,
		GF_TIME,      GF_GRAVITY, GF_ROCKET,  GF_NUKE,
		GF_HELL_FIRE, GF_DISINTEGRATE, GF_PSY_SPEAR
	};

	Chaos_type = hurt_types[randint0(31)];
	if (one_in_(4)) line_chaos = TRUE;

	if (one_in_(6))
	{
		for (dummy = 1; dummy < 10; dummy++)
		{
			if (dummy - 5)
			{
				if (line_chaos)
					fire_beam(Chaos_type, dummy, 150);
				else
					fire_ball(Chaos_type, dummy, 150, 2);
			}
		}
	}
	else if (one_in_(3))
	{
		fire_ball(Chaos_type, 0, 500, 8);
	}
	else
	{
		if (!get_aim_dir(&dir)) return;
		if (line_chaos)
			fire_beam(Chaos_type, dir, 250);
		else
			fire_ball(Chaos_type, dir, 250, 3 + (plev / 35));
	}
}

/*!
 * @brief TY_CURSE����ȯư / Activate the evil Topi Ylinen curse
 * @param stop_ty �Ƶ�������ߥե饰
 * @param count ȯư���
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 * @details
 * <pre>
 * rr9: Stop the nasty things when a Cyberdemon is summoned
 * or the player gets paralyzed.
 * </pre>
 */
bool activate_ty_curse(bool stop_ty, int *count)
{
	int     i = 0;

	int flg = (PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL | PROJECT_JUMP);

	do
	{
		switch (randint1(34))
		{
		case 28: case 29:
			if (!(*count))
			{
				msg_print(_("���̤��ɤ줿...", "The ground trembles..."));
				earthquake(p_ptr->y, p_ptr->x, 5 + randint0(10));
				if (!one_in_(6)) break;
			}
		case 30: case 31:
			if (!(*count))
			{
				int dam = damroll(10, 10);
				msg_print(_("�������Ϥμ����ؤ��⤬��������", "A portal opens to a plane of raw mana!"));
				project(0, 8, p_ptr->y, p_ptr->x, dam, GF_MANA, flg, -1);
				take_hit(DAMAGE_NOESCAPE, dam, _("�������Ϥβ���", "released pure mana"), -1);
				if (!one_in_(6)) break;
			}
		case 32: case 33:
			if (!(*count))
			{
				msg_print(_("���Ϥζ��֤��Ĥ����", "Space warps about you!"));
				teleport_player(damroll(10, 10), TELEPORT_PASSIVE);
				if (randint0(13)) (*count) += activate_hi_summon(p_ptr->y, p_ptr->x, FALSE);
				if (!one_in_(6)) break;
			}
		case 34:
			msg_print(_("���ͥ륮���Τ��ͤ�򴶤�����", "You feel a surge of energy!"));
			wall_breaker();
			if (!randint0(7))
			{
				project(0, 7, p_ptr->y, p_ptr->x, 50, GF_KILL_WALL, flg, -1);
				take_hit(DAMAGE_NOESCAPE, 50, _("���ͥ륮���Τ��ͤ�", "surge of energy"), -1);
			}
			if (!one_in_(6)) break;
		case 1: case 2: case 3: case 16: case 17:
			aggravate_monsters(0);
			if (!one_in_(6)) break;
		case 4: case 5: case 6:
			(*count) += activate_hi_summon(p_ptr->y, p_ptr->x, FALSE);
			if (!one_in_(6)) break;
		case 7: case 8: case 9: case 18:
			(*count) += summon_specific(0, p_ptr->y, p_ptr->x, dun_level, 0, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE | PM_NO_PET));
			if (!one_in_(6)) break;
		case 10: case 11: case 12:
			msg_print(_("�и��ͤ��Τ���ۤ����줿�������롪", "You feel your experience draining away..."));
			lose_exp(p_ptr->exp / 16);
			if (!one_in_(6)) break;
		case 13: case 14: case 15: case 19: case 20:
			if (stop_ty || (p_ptr->free_act && (randint1(125) < p_ptr->skill_sav)) || (p_ptr->pclass == CLASS_BERSERKER))
			{
				/* Do nothing */ ;
			}
			else
			{
				msg_print(_("Ħ���ˤʤä���ʬ����", "You feel like a statue!"));
				if (p_ptr->free_act)
					set_paralyzed(p_ptr->paralyzed + randint1(3));
				else
					set_paralyzed(p_ptr->paralyzed + randint1(13));
				stop_ty = TRUE;
			}
			if (!one_in_(6)) break;
		case 21: case 22: case 23:
			(void)do_dec_stat(randint0(6));
			if (!one_in_(6)) break;
		case 24:
			msg_print(_("�ۤ������ï�������ǲ����Ƥ롩", "Huh? Who am I? What am I doing here?"));
			lose_all_info();
			if (!one_in_(6)) break;
		case 25:
			/*
			 * Only summon Cyberdemons deep in the dungeon.
			 */
			if ((dun_level > 65) && !stop_ty)
			{
				(*count) += summon_cyber(-1, p_ptr->y, p_ptr->x);
				stop_ty = TRUE;
				break;
			}
			if (!one_in_(6)) break;
		default:
			while (i < 6)
			{
				do
				{
					(void)do_dec_stat(i);
				}
				while (one_in_(2));

				i++;
			}
		}
	}
	while (one_in_(3) && !stop_ty);

	return stop_ty;
}

/*!
 * @brief HI_SUMMON(��龤��)����ȯư
 * @param y ��������Y��ɸ
 * @param x ��������X��ɸ
 * @param can_pet �ץ쥤�䡼�ΥڥåȤȤʤ��ǽ��������ʤ��TRUE�ˤ���
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
int activate_hi_summon(int y, int x, bool can_pet)
{
	int i;
	int count = 0;
	int summon_lev;
	u32b mode = PM_ALLOW_GROUP;
	bool pet = FALSE;

	if (can_pet)
	{
		if (one_in_(4))
		{
			mode |= PM_FORCE_FRIENDLY;
		}
		else
		{
			mode |= PM_FORCE_PET;
			pet = TRUE;
		}
	}

	if (!pet) mode |= PM_NO_PET;

	summon_lev = (pet ? p_ptr->lev * 2 / 3 + randint1(p_ptr->lev / 2) : dun_level);

	for (i = 0; i < (randint1(7) + (dun_level / 40)); i++)
	{
		switch (randint1(25) + (dun_level / 20))
		{
			case 1: case 2:
				count += summon_specific((pet ? -1 : 0), y, x, summon_lev, SUMMON_ANT, mode);
				break;
			case 3: case 4:
				count += summon_specific((pet ? -1 : 0), y, x, summon_lev, SUMMON_SPIDER, mode);
				break;
			case 5: case 6:
				count += summon_specific((pet ? -1 : 0), y, x, summon_lev, SUMMON_HOUND, mode);
				break;
			case 7: case 8:
				count += summon_specific((pet ? -1 : 0), y, x, summon_lev, SUMMON_HYDRA, mode);
				break;
			case 9: case 10:
				count += summon_specific((pet ? -1 : 0), y, x, summon_lev, SUMMON_ANGEL, mode);
				break;
			case 11: case 12:
				count += summon_specific((pet ? -1 : 0), y, x, summon_lev, SUMMON_UNDEAD, mode);
				break;
			case 13: case 14:
				count += summon_specific((pet ? -1 : 0), y, x, summon_lev, SUMMON_DRAGON, mode);
				break;
			case 15: case 16:
				count += summon_specific((pet ? -1 : 0), y, x, summon_lev, SUMMON_DEMON, mode);
				break;
			case 17:
				if (can_pet) break;
				count += summon_specific((pet ? -1 : 0), y, x, summon_lev, SUMMON_AMBERITES, (mode | PM_ALLOW_UNIQUE));
				break;
			case 18: case 19:
				if (can_pet) break;
				count += summon_specific((pet ? -1 : 0), y, x, summon_lev, SUMMON_UNIQUE, (mode | PM_ALLOW_UNIQUE));
				break;
			case 20: case 21:
				if (!can_pet) mode |= PM_ALLOW_UNIQUE;
				count += summon_specific((pet ? -1 : 0), y, x, summon_lev, SUMMON_HI_UNDEAD, mode);
				break;
			case 22: case 23:
				if (!can_pet) mode |= PM_ALLOW_UNIQUE;
				count += summon_specific((pet ? -1 : 0), y, x, summon_lev, SUMMON_HI_DRAGON, mode);
				break;
			case 24:
				count += summon_specific((pet ? -1 : 0), y, x, 100, SUMMON_CYBER, mode);
				break;
			default:
				if (!can_pet) mode |= PM_ALLOW_UNIQUE;
				count += summon_specific((pet ? -1 : 0), y, x,pet ? summon_lev : (((summon_lev * 3) / 2) + 5), 0, mode);
		}
	}

	return count;
}


/*!
 * @brief �����С��ǡ����ξ���
 * @param who ������Υ�󥹥���ID(0�ʤ�Хץ쥤�䡼)
 * @param y ��������Y��ɸ
 * @param x ��������X��ɸ
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
int summon_cyber(int who, int y, int x)
{
	int i;
	int max_cyber = (easy_band ? 1 : (dun_level / 24) + randint1(2)); /* #tang 50 -> 24 */
	int count = 0;
	u32b mode = PM_ALLOW_GROUP;

	/* Summoned by a monster */
	if (who > 0)
	{
		monster_type *m_ptr = &m_list[who];
		if (is_pet(m_ptr)) mode |= PM_FORCE_PET;
	}

	if (max_cyber > 4) max_cyber = 4;

	for (i = 0; i < max_cyber; i++)
	{
		count += summon_specific(who, y, x, 100, SUMMON_CYBER, mode);
	}

	return count;
}

/*!
 * @brief �����˲�����(�ץ쥤�䡼�濴)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
void wall_breaker(void)
{
	int i;
	int y = 0, x = 0;
	int attempts = 1000;

	if (randint1(80 + p_ptr->lev) < 70)
	{
		while (attempts--)
		{
			scatter(&y, &x, p_ptr->y, p_ptr->x, 4, 0);

			if (!cave_have_flag_bold(y, x, FF_PROJECT)) continue;

			if (!player_bold(y, x)) break;
		}

		project(0, 0, y, x, 20 + randint1(30), GF_KILL_WALL,
				  (PROJECT_BEAM | PROJECT_THRU | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL), -1);
	}
	else if (randint1(100) > 30)
	{
		earthquake(p_ptr->y, p_ptr->x, 1);
	}
	else
	{
		int num = damroll(5, 3);

		for (i = 0; i < num; i++)
		{
			while (1)
			{
				scatter(&y, &x, p_ptr->y, p_ptr->x, 10, 0);

				if (!player_bold(y, x)) break;
			}

			project(0, 0, y, x, 20 + randint1(30), GF_KILL_WALL,
					  (PROJECT_BEAM | PROJECT_THRU | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL), -1);
		}
	}
}


/*!
 * @brief �ѥ˥å�����󥹥�������(�ץ쥤�䡼�볦�ϰ���) / Confuse monsters
 * @param dam ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool confuse_monsters(int dam)
{
	return (project_hack(GF_OLD_CONF, dam));
}


/*!
 * @brief ���㡼�ࡦ��󥹥�������(�ץ쥤�䡼�볦�ϰ���) / Charm monsters
 * @param dam ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool charm_monsters(int dam)
{
	return (project_hack(GF_CHARM, dam));
}


/*!
 * @brief ưʪ̥λ����(�ץ쥤�䡼�볦�ϰ���) / Charm Animals
 * @param dam ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool charm_animals(int dam)
{
	return (project_hack(GF_CONTROL_ANIMAL, dam));
}


/*!
 * @brief ��󥹥���ۯ۰����(�ץ쥤�䡼�볦�ϰ���) / Stun monsters
 * @param dam ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool stun_monsters(int dam)
{
	return (project_hack(GF_STUN, dam));
}


/*!
 * @brief ��󥹥�����߸���(�ץ쥤�䡼�볦�ϰ���) / Stasis monsters
 * @param dam ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool stasis_monsters(int dam)
{
	return (project_hack(GF_STASIS, dam));
}


/*!
 * @brief ��󥹥��������������(�ץ쥤�䡼�볦�ϰ���) / Mindblast monsters
 * @param dam ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool mindblast_monsters(int dam)
{
	return (project_hack(GF_PSI, dam));
}


/*!
 * @brief ��󥹥�����������(�ץ쥤�䡼�볦�ϰ���) / Banish all monsters
 * @param dist ���ϡʵ�Υ��
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool banish_monsters(int dist)
{
	return (project_hack(GF_AWAY_ALL, dist));
}


/*!
 * @brief �ٰ��໶����(�ץ쥤�䡼�볦�ϰ���) / Turn evil
 * @param dam ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool turn_evil(int dam)
{
	return (project_hack(GF_TURN_EVIL, dam));
}


/*!
 * @brief ����󥹥����໶����(�ץ쥤�䡼�볦�ϰ���) / Turn everyone
 * @param dam ����
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool turn_monsters(int dam)
{
	return (project_hack(GF_TURN_ALL, dam));
}


/*!
 * @brief ��θ���(�ץ쥤�䡼�볦�ϰ���) / Death-ray all monsters (note: OBSCENELY powerful)
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool deathray_monsters(void)
{
	return (project_hack(GF_DEATH_RAY, p_ptr->lev * 200));
}

/*!
 * @brief ���㡼�ࡦ��󥹥���(1��)
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param plev �ѥ
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool charm_monster(int dir, int plev)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
	return (project_hook(GF_CHARM, dir, plev, flg));
}

/*!
 * @brief ����ǥåɻ���(1��)
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param plev �ѥ
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool control_one_undead(int dir, int plev)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
	return (project_hook(GF_CONTROL_UNDEAD, dir, plev, flg));
}

/*!
 * @brief �������(1��)
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param plev �ѥ
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool control_one_demon(int dir, int plev)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
	return (project_hook(GF_CONTROL_DEMON, dir, plev, flg));
}

/*!
 * @brief ưʪ����(1��)
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param plev �ѥ
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool charm_animal(int dir, int plev)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
	return (project_hook(GF_CONTROL_ANIMAL, dir, plev, flg));
}

/*!
 * @brief ��ʪ����(1��)
 * @param dir ����(5�ʤ�Х����Х��ѿ� target_col/target_row �κ�ɸ����ɸ�ˤ���)
 * @param plev �ѥ
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool charm_living(int dir, int plev)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
	return (project_hook(GF_CONTROL_LIVING, dir, plev, flg));
}

/*!
 * @brief �Ѥ��Ƚ���
 * @param success Ƚ��������ν����ʤ��TRUE
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤�
 */
bool kawarimi(bool success)
{
	object_type forge;
	object_type *q_ptr = &forge;
	int y, x;

	if (p_ptr->is_dead) return FALSE;
	if (p_ptr->confused || p_ptr->blind || p_ptr->paralyzed || p_ptr->image) return FALSE;
	if (randint0(200) < p_ptr->stun) return FALSE;

	if (!success && one_in_(3))
	{
		msg_print(_("���ԡ�ƨ�����ʤ��ä���", "Failed! You couldn't run away."));
		p_ptr->special_defense &= ~(NINJA_KAWARIMI);
		p_ptr->redraw |= (PR_STATUS);
		return FALSE;
	}

	y = p_ptr->y;
	x = p_ptr->x;

	teleport_player(10 + randint1(90), 0L);

	object_wipe(q_ptr);

	object_prep(q_ptr, lookup_kind(TV_STATUE, SV_WOODEN_STATUE));

	q_ptr->pval = MON_NINJA;

	/* Drop it in the dungeon */
	(void)drop_near(q_ptr, -1, y, x);

#ifdef JP
	if (success) msg_print("�����������������᤯�Ȥ�Ҥ뤬��������");
	else msg_print("���ԡ����������Ƥ��ޤä���");
#else
	if (success) msg_print("You have turned around just before the attack hit you.");
	else msg_print("Failed! You are hit by the attack.");
#endif

	p_ptr->special_defense &= ~(NINJA_KAWARIMI);
	p_ptr->redraw |= (PR_STATUS);

	/* Teleported */
	return TRUE;
}


/*!
 * @brief ���Ƚ��� / "Rush Attack" routine for Samurai or Ninja
 * @param mdeath ��ɸ��󥹥�������˴���������֤�
 * @return ���Ѥ��ºݤˤ��ä����TRUE���֤� /  Return value is for checking "done"
 */
bool rush_attack(bool *mdeath)
{
	int dir;
	int tx, ty;
	int tm_idx = 0;
	u16b path_g[32];
	int path_n, i;
	bool tmp_mdeath = FALSE;
	bool moved = FALSE;

	if (mdeath) *mdeath = FALSE;

	project_length = 5;
	if (!get_aim_dir(&dir)) return FALSE;

	/* Use the given direction */
	tx = p_ptr->x + project_length * ddx[dir];
	ty = p_ptr->y + project_length * ddy[dir];

	/* Hack -- Use an actual "target" */
	if ((dir == 5) && target_okay())
	{
		tx = target_col;
		ty = target_row;
	}

	if (in_bounds(ty, tx)) tm_idx = cave[ty][tx].m_idx;

	path_n = project_path(path_g, project_length, p_ptr->y, p_ptr->x, ty, tx, PROJECT_STOP | PROJECT_KILL);
	project_length = 0;

	/* No need to move */
	if (!path_n) return TRUE;

	/* Use ty and tx as to-move point */
	ty = p_ptr->y;
	tx = p_ptr->x;

	/* Project along the path */
	for (i = 0; i < path_n; i++)
	{
		monster_type *m_ptr;

		int ny = GRID_Y(path_g[i]);
		int nx = GRID_X(path_g[i]);

		if (cave_empty_bold(ny, nx) && player_can_enter(cave[ny][nx].feat, 0))
		{
			ty = ny;
			tx = nx;

			/* Go to next grid */
			continue;
		}

		if (!cave[ny][nx].m_idx)
		{
			if (tm_idx)
			{
				msg_print(_("���ԡ�", "Failed!"));
			}
			else
			{
				msg_print(_("�����ˤ����ȤǤ�����ʤ���", "You can't move to that place."));
			}

			/* Exit loop */
			break;
		}

		/* Move player before updating the monster */
		if (!player_bold(ty, tx)) teleport_player_to(ty, tx, TELEPORT_NONMAGICAL);

		/* Update the monster */
		update_mon(cave[ny][nx].m_idx, TRUE);

		/* Found a monster */
		m_ptr = &m_list[cave[ny][nx].m_idx];

		if (tm_idx != cave[ny][nx].m_idx)
		{
#ifdef JP
			msg_format("%s%s��Ω���դ����äƤ��롪", tm_idx ? "�̤�" : "",
				   m_ptr->ml ? "��󥹥���" : "����");
#else
			msg_format("There is %s in the way!", m_ptr->ml ? (tm_idx ? "another monster" : "a monster") : "someone");
#endif
		}
		else if (!player_bold(ty, tx))
		{
			/* Hold the monster name */
			char m_name[80];

			/* Get the monster name (BEFORE polymorphing) */
			monster_desc(m_name, m_ptr, 0);
			msg_format(_("���᤯%s�β�������������", "You quickly jump in and attack %s!"), m_name);
		}

		if (!player_bold(ty, tx)) teleport_player_to(ty, tx, TELEPORT_NONMAGICAL);
		moved = TRUE;
		tmp_mdeath = py_attack(ny, nx, HISSATSU_NYUSIN);

		break;
	}

	if (!moved && !player_bold(ty, tx)) teleport_player_to(ty, tx, TELEPORT_NONMAGICAL);

	if (mdeath) *mdeath = tmp_mdeath;
	return TRUE;
}


/*!
 * @brief �����ξõ� / Remove all mirrors in this floor
 * @param explode ��ȯ������ȼ���ʤ��TRUE
 * @return �ʤ�
 */
void remove_all_mirrors(bool explode)
{
	int x, y;

	for (x = 0; x < cur_wid; x++)
	{
		for (y = 0; y < cur_hgt; y++)
		{
			if (is_mirror_grid(&cave[y][x]))
			{
				remove_mirror(y, x);
				if (explode)
					project(0, 2, y, x, p_ptr->lev / 2 + 5, GF_SHARDS,
						(PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL | PROJECT_JUMP | PROJECT_NO_HANGEKI), -1);
			}
		}
	}
}
