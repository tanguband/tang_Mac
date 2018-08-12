/*!
    @file bldg.c
    @brief Į�λ��߽��� / Building commands
    @date 2013/12/23
    @author
    Created by Ken Wigle for Kangband - a variant of Angband 2.8.3\n
    -KMW-\n
    \n
    Rewritten for Kangband 2.8.3i using Kamband's version of\n
    bldg.c as written by Ivan Tkatchev\n
    \n
    Changed for ZAngband by Robert Ruehlmann\n
*/

#include "angband.h"


/*!
 * �롼����� / hack as in leave_store in store.c
 */
static bool leave_bldg = FALSE;

/*!
 * @brief ����������ꤵ�줿��²�����ȡ���ˡ�ΰ�ե饰���ץ쥤�䡼�Ȱ��פ��뤫��Ƚ�ꤹ�롣
 * @details �Ƽ葉��ɤ�����ʤɡ�����ο��Ȥʤ��ͥ�����֤���������ߡ�
 * ���뤤�Ͽ�Ʋ�ʤ�����μ�²�Ǥ����ѤǤ��ʤ����ߤ�Ƚ�������Ԥ���
 * @param bldg ���߹�¤�Τλ��ȥݥ���
 * @return ��²�����ȡ���ˡ�ΰ�Τ����줫�����פ��Ƥ��뤫������
 */
static bool is_owner(building_type *bldg)
{
	if (bldg->member_class[p_ptr->pclass] == BUILDING_OWNER)
	{
		return (TRUE);
	}

	if (bldg->member_race[p_ptr->prace] == BUILDING_OWNER)
	{
		return (TRUE);
	}

	if ((is_magic(p_ptr->realm1) && (bldg->member_realm[p_ptr->realm1] == BUILDING_OWNER)) ||
		(is_magic(p_ptr->realm2) && (bldg->member_realm[p_ptr->realm2] == BUILDING_OWNER)))
	{
		return (TRUE);
	}

	return (FALSE);
}

/*!
 * @brief ����������ꤵ�줿��²�����ȡ���ˡ�ΰ�ե饰���ץ쥤�䡼�Ȱ��פ��뤫��Ƚ�ꤹ�롣
 �ʥ��ڥ�ޥ�����������Ƚ��Ĥ���
 * @details �Ƽ葉��ɤ�����ʤɡ�����ο��Ȥʤ��ͥ�����֤���������ߡ�
 * ���뤤�Ͽ�Ʋ�ʤ�����μ�²�Ǥ����ѤǤ��ʤ����ߤ�Ƚ�������Ԥ���
 * @param bldg ���߹�¤�Τλ��ȥݥ���
 * @return ��²�����ȡ���ˡ�ΰ�Τ����줫�����פ��Ƥ��뤫������
 * @todo is_owner()�Ȥμ¼�Ū��¿�ż����ʤΤǡ���ե�������󥰤�Ԥ��٤��Ǥ��롣
 */
static bool is_member(building_type *bldg)
{
	if (bldg->member_class[p_ptr->pclass])
	{
		return (TRUE);
	}

	if (bldg->member_race[p_ptr->prace])
	{
		return (TRUE);
	}

	if ((is_magic(p_ptr->realm1) && bldg->member_realm[p_ptr->realm1]) ||
	    (is_magic(p_ptr->realm2) && bldg->member_realm[p_ptr->realm2]))
	{
		return (TRUE);
	}


	if (p_ptr->pclass == CLASS_SORCERER)
	{
		int i;
		bool OK = FALSE;
		for (i = 0; i < MAX_MAGIC; i++)
		{
			if (bldg->member_realm[i+1]) OK = TRUE;
		}
		return OK;
	}
	return (FALSE);
}

/*!
 * @brief ���󥽡����ɽ�����줿���ߤ˴ؤ�������õ�� / Clear the building information
 * @details �õ�Ϲ���˥̥�ʸ����ǹԤ��롣
 * @param min_row ���Ϲ��ֹ�
 * @param max_row �������ֹ�
 * @return �ʤ�
 */
static void clear_bldg(int min_row, int max_row)
{
	int   i;

	for (i = min_row; i <= max_row; i++)
		prt("", i, 0);
}

/*!
 * @brief ������ɽ�����롣
 * @return �ʤ�
 */
static void building_prt_gold(void)
{
	char tmp_str[80];
	prt(_("������Τ���: ", "Gold Remaining: "), 23,53);
	sprintf(tmp_str, "%9ld", (long)p_ptr->au);
	prt(tmp_str, 23, 68);
}

/*!
 * @brief ���ߤΥ����ӥ�������ɽ������ / Display a building.
 * @param bldg ���߹�¤�Τλ��ȥݥ���
 * @return �ʤ�
 */
static void show_building(building_type* bldg)
{
	char buff[20];
	int i;
	byte action_color;
	char tmp_str[80];

	Term_clear();
	sprintf(tmp_str, "%s (%s) %35s", bldg->owner_name, bldg->owner_race, bldg->name);
	prt(tmp_str, 2, 1);


	for (i = 0; i < 8; i++)
	{
		if (bldg->letters[i])
		{
			if (bldg->action_restr[i] == 0)
			{
				if ((is_owner(bldg) && (bldg->member_costs[i] == 0)) ||
					(!is_owner(bldg) && (bldg->other_costs[i] == 0)))
				{
					action_color = TERM_WHITE;
					buff[0] = '\0';
				}
				else if (is_owner(bldg))
				{
					action_color = TERM_YELLOW;
					sprintf(buff, _("($%ld)", "(%ldgp)"), (long int)bldg->member_costs[i]);
				}
				else
				{
					action_color = TERM_YELLOW;
					sprintf(buff, _("($%ld)", "(%ldgp)"), (long int)bldg->other_costs[i]);				}
			}
			else if (bldg->action_restr[i] == 1)
			{
				if (!is_member(bldg))
				{
					action_color = TERM_L_DARK;
					strcpy(buff, _("(��Ź)", "(closed)"));
				}
				else if ((is_owner(bldg) && (bldg->member_costs[i] == 0)) ||
					(is_member(bldg) && (bldg->other_costs[i] == 0)))
				{
					action_color = TERM_WHITE;
					buff[0] = '\0';
				}
				else if (is_owner(bldg))
				{
					action_color = TERM_YELLOW;
					sprintf(buff, _("($%ld)", "(%ldgp)"), (long int)bldg->member_costs[i]);
				}
				else
				{
					action_color = TERM_YELLOW;
					sprintf(buff, _("($%ld)", "(%ldgp)"), (long int)bldg->other_costs[i]);
				}
			}
			else
			{
				if (!is_owner(bldg))
				{
					action_color = TERM_L_DARK;
					strcpy(buff, _("(��Ź)", "(closed)"));
				}
				else if (bldg->member_costs[i] != 0)
				{
					action_color = TERM_YELLOW;
					sprintf(buff, _("($%ld)", "(%ldgp)"), (long int)bldg->member_costs[i]);
				}
				else
				{
					action_color = TERM_WHITE;
					buff[0] = '\0';
				}
			}

			sprintf(tmp_str," %c) %s %s", bldg->letters[i], bldg->act_names[i], buff);
			c_put_str(action_color, tmp_str, 19+(i/2), 35*(i%2));
		}
	}
	prt(_(" ESC) ��ʪ��Ф�", " ESC) Exit building"), 23, 0);
}

/*!
 * @brief Ʈ��������륳�ޥ�ɤν��� / arena commands
 * @param cmd Ʈ���������ID
 * @return �ʤ�
 */
static void arena_comm(int cmd)
{
	monster_race    *r_ptr;
	cptr            name;


	switch (cmd)
	{
		case BACT_ARENA:
			if (p_ptr->arena_number == MAX_ARENA_MONS)
			{
				clear_bldg(5, 19);
				prt(_("���꡼�ʤ�ͥ���ԡ�", "               Arena Victor!"), 5, 0);
				prt(_("����ǤȤ������ʤ������Ƥ�Ũ���ݤ��ޤ�����", "Congratulations!  You have defeated all before you."), 7, 0);
				prt(_("�޶�Ȥ��� $1,000,000 ��Ϳ�����ޤ���", "For that, receive the prize: 1,000,000 gold pieces"), 8, 0);

				prt("", 10, 0);
				prt("", 11, 0);
				p_ptr->au += 1000000L;
				msg_print(_("���ڡ���������³��", "Press the space bar to continue"));
				msg_print(NULL);
				p_ptr->arena_number++;
			}
			else if (p_ptr->arena_number > MAX_ARENA_MONS)
			{
				if (p_ptr->arena_number < MAX_ARENA_MONS+2)
				{
					msg_print(_("���Τ���˺Ƕ���ĩ��Ԥ��Ѱդ��Ƥ�������", "The strongest challenger is waiting for you."));
					msg_print(NULL);
					if (get_check(_("ĩ�魯�뤫�͡�", "Do you fight? ")))
					{	
						msg_print(_("��̤��褤��", "Die, maggots."));
						msg_print(NULL);
					
						p_ptr->exit_bldg = FALSE;
						reset_tim_flags();

						/* Save the surface floor as saved floor */
						prepare_change_floor_mode(CFM_SAVE_FLOORS);

						p_ptr->inside_arena = TRUE;
						p_ptr->leaving = TRUE;
						leave_bldg = TRUE;
					}
					else
					{
						msg_print(_("��ǰ����", "We are disappointed."));
					}
				}
				else
				{
					msg_print(_("���ʤ��ϥ��꡼�ʤ����ꡢ���Ф餯�δֱɸ��ˤҤ��ä���",
								"You enter the arena briefly and bask in your glory."));
					msg_print(NULL);
				}
			}
			else if (p_ptr->riding && (p_ptr->pclass != CLASS_BEASTMASTER) && (p_ptr->pclass != CLASS_CAVALRY))
			{
				msg_print(_("�ڥåȤ˾�ä��ޤޤǤϥ��꡼�ʤ����줵���Ƥ�館�ʤ��ä���",
							"You don't have permission to enter with pet."));
				msg_print(NULL);
			}
			else
			{
				p_ptr->exit_bldg = FALSE;
				reset_tim_flags();

				/* Save the surface floor as saved floor */
				prepare_change_floor_mode(CFM_SAVE_FLOORS);

				p_ptr->inside_arena = TRUE;
				p_ptr->leaving = TRUE;
				leave_bldg = TRUE;
			}
			break;
		case BACT_POSTER:
			if (p_ptr->arena_number == MAX_ARENA_MONS)
				msg_print(_("���ʤ��Ͼ����Ԥ��� ���꡼�ʤǤΥ����ˡ��˻��ä��ʤ�����",
							"You are victorious. Enter the arena for the ceremony."));

			else if (p_ptr->arena_number > MAX_ARENA_MONS)
			{
				msg_print(_("���ʤ��Ϥ��٤Ƥ�Ũ�˾���������", "You have won against all foes."));
			}
			else
			{
				r_ptr = &r_info[arena_info[p_ptr->arena_number].r_idx];
				name = (r_name + r_ptr->name);
				msg_format(_("%s ��ĩ�魯���ΤϤ��ʤ�����", "Do I hear any challenges against: %s"), name);

				p_ptr->monster_race_idx = arena_info[p_ptr->arena_number].r_idx;
				p_ptr->window |= (PW_MONSTER);
				window_stuff();

			}
			break;
		case BACT_ARENA_RULES:

			/* Save screen */
			screen_save();

			/* Peruse the arena help file */
			(void)show_file(TRUE, _("arena_j.txt", "arena.txt"), NULL, 0, 0);

			/* Load screen */
			screen_load();

			break;
	}
}

/*!
 * @brief �����ΤΥ���åȥ���ܥ��ɽ������ / display fruit for dice slots
 * @param row ����ܥ��ɽ������Ԥξ�ü
 * @param col ����ܥ��ɽ������Ԥκ�ü
 * @param fruit ɽ�����륷��ܥ�ID
 * @return �ʤ�
 */
static void display_fruit(int row, int col, int fruit)
{
	switch (fruit)
	{
		case 0: /* lemon */
			c_put_str(TERM_YELLOW, "   ####.", row, col);
			c_put_str(TERM_YELLOW, "  #    #", row + 1, col);
			c_put_str(TERM_YELLOW, " #     #", row + 2, col);
			c_put_str(TERM_YELLOW, "#      #", row + 3, col);
			c_put_str(TERM_YELLOW, "#      #", row + 4, col);
			c_put_str(TERM_YELLOW, "#     # ", row + 5, col);
			c_put_str(TERM_YELLOW, "#    #  ", row + 6, col);
			c_put_str(TERM_YELLOW, ".####   ", row + 7, col);
			prt(                 _(" ���� ",
						           " Lemon  "), row + 8, col);
			break;
		case 1: /* orange */
			c_put_str(TERM_ORANGE, "   ##   ", row, col);
			c_put_str(TERM_ORANGE, "  #..#  ", row + 1, col);
			c_put_str(TERM_ORANGE, " #....# ", row + 2, col);
			c_put_str(TERM_ORANGE, "#......#", row + 3, col);
			c_put_str(TERM_ORANGE, "#......#", row + 4, col);
			c_put_str(TERM_ORANGE, " #....# ", row + 5, col);
			c_put_str(TERM_ORANGE, "  #..#  ", row + 6, col);
			c_put_str(TERM_ORANGE, "   ##   ", row + 7, col);
			prt(                 _("�����",
								   " Orange "), row + 8, col);
			break;
		case 2: /* sword */
			c_put_str(TERM_SLATE, _("   ��   ",  "   /\\   ") , row, col);
			c_put_str(TERM_SLATE, _("   ||   ", "   ##   ") , row + 1, col);
			c_put_str(TERM_SLATE, _("   ||   ", "   ##   ") , row + 2, col);
			c_put_str(TERM_SLATE, _("   ||   ", "   ##   ") , row + 3, col);
			c_put_str(TERM_SLATE, _("   ||   ", "   ##   ") , row + 4, col);
			c_put_str(TERM_SLATE, _("   ||   ", "   ##   ") , row + 5, col);
			c_put_str(TERM_UMBER, _(" |=��=| ", " ###### ") , row + 6, col);
			c_put_str(TERM_UMBER, _("   ��   ", "   ##   ") , row + 7, col);
			prt(                  _("   ��   ", " Sword  ") , row + 8, col);
			break;
		case 3: /* shield */
			c_put_str(TERM_SLATE, " ###### ", row, col);
			c_put_str(TERM_SLATE, "#      #", row + 1, col);
			c_put_str(TERM_SLATE, "# ++++ #", row + 2, col);
			c_put_str(TERM_SLATE, "# +==+ #", row + 3, col);
			c_put_str(TERM_SLATE, "#  ++  #", row + 4, col);
			c_put_str(TERM_SLATE, " #    # ", row + 5, col);
			c_put_str(TERM_SLATE, "  #  #  ", row + 6, col);
			c_put_str(TERM_SLATE, "   ##   ", row + 7, col);
			prt(                _("   ��   ",
								  " Shield "), row + 8, col);
			break;
		case 4: /* plum */
			c_put_str(TERM_VIOLET, "   ##   ", row, col);
			c_put_str(TERM_VIOLET, " ###### ", row + 1, col);
			c_put_str(TERM_VIOLET, "########", row + 2, col);
			c_put_str(TERM_VIOLET, "########", row + 3, col);
			c_put_str(TERM_VIOLET, "########", row + 4, col);
			c_put_str(TERM_VIOLET, " ###### ", row + 5, col);
			c_put_str(TERM_VIOLET, "  ####  ", row + 6, col);
			c_put_str(TERM_VIOLET, "   ##   ", row + 7, col);
			prt(                 _(" �ץ�� ",
								   "  Plum  "), row + 8, col);
			break;
		case 5: /* cherry */
			c_put_str(TERM_RED, "      ##", row, col);
			c_put_str(TERM_RED, "   ###  ", row + 1, col);
			c_put_str(TERM_RED, "  #..#  ", row + 2, col);
			c_put_str(TERM_RED, "  #..#  ", row + 3, col);
			c_put_str(TERM_RED, " ###### ", row + 4, col);
			c_put_str(TERM_RED, "#..##..#", row + 5, col);
			c_put_str(TERM_RED, "#..##..#", row + 6, col);
			c_put_str(TERM_RED, " ##  ## ", row + 7, col);
			prt(              _("�����꡼",
								" Cherry "), row + 8, col);
			break;
	}
}

/*! @note
 * kpoker no (tyuto-hannpa na)pakuri desu...
 * joker ha shineru node haitte masen.
 *
 * TODO: donataka! tsukutte!
 *  - agatta yaku no kiroku (like DQ).
 *  - kakkoii card no e.
 *  - sousa-sei no koujyo.
 *  - code wo wakariyasuku.
 *  - double up.
 *  - Joker... -- done.
 *
 * 9/13/2000 --Koka
 * 9/15/2000 joker wo jissou. soreto, code wo sukosi kakikae. --Habu
 */

#define SUIT_OF(card)  ((card) / 13) /*!< �ȥ��ץ����ɤΥ����Ȥ��֤� */
#define NUM_OF(card)   ((card) % 13) /*!< �ȥ��ץ����ɤ��ֹ���֤� */
#define IS_JOKER(card) ((card) == 52) /*!< �ȥ��ץ����ɤ����硼�������ɤ������֤� */

static int cards[5]; /*!< �ݡ������θ��ߤμ껥ID */

/*!
 * @brief �ݡ������λ������ڤ롣
 * @param deck �ǥå�������
 * @return �ʤ�
 */
static void reset_deck(int deck[])
{
	int i;
	for (i = 0; i < 53; i++) deck[i] = i;

	/* shuffle cards */
	for (i = 0; i < 53; i++){
		int tmp1 = randint0(53 - i) + i;
		int tmp2 = deck[i];
		deck[i] = deck[tmp1];
		deck[tmp1] = tmp2;
	}
}

/*!
 * @brief �ݡ������ץ쥤��˥��硼��������äƤ��뤫��Ƚ����֤���
 * @return ���硼��������äƤ��뤫��
 */
static bool have_joker(void)
{
	int i;

	for (i = 0; i < 5; i++){
	  if(IS_JOKER(cards[i])) return TRUE;
	}
	return FALSE;
}

/*!
 * @brief �ݡ������μ껥�˳������ֹ�λ������뤫���֤���
 * @param num õ�����������ɤ��ֹ档
 * @return �������ֹ椬�껥�ˤ��뤫��
 */
static bool find_card_num(int num)
{
	int i;
	for (i = 0; i < 5; i++)
		if (NUM_OF(cards[i]) == num && !IS_JOKER(cards[i])) return TRUE;
	return FALSE;
}

/*!
 * @brief �ݡ������μ껥���ե�å���������Ƥ��뤫�򵢤���
 * @return ���Ƚ����
 */
static bool yaku_check_flush(void)
{
	int i, suit;
	bool joker_is_used = FALSE;

	suit = IS_JOKER(cards[0]) ? SUIT_OF(cards[1]) : SUIT_OF(cards[0]);
	for (i = 0; i < 5; i++){
		if (SUIT_OF(cards[i]) != suit){
		  if(have_joker() && !joker_is_used)
		    joker_is_used = TRUE;
		  else
		    return FALSE;
		}
	}

	return TRUE;
}

/*!
 * @brief �ݡ������μ껥�����ȥ졼�Ȥ�ޤ����̤�������Ƥ��뤫�򵢤���
 * @return ���Ƚ���� 0�ĥ��ȥ졼�ȡ��ե�å��夤�����ʤ�/1�ĥ��ȥ졼�ȤΤ�/2�ĥ��ȥ졼�ȥե�å���/3�ĥ���륹�ȥ졼�ȥե�å���
 */
static int yaku_check_straight(void)
{
	int i, lowest = 99;
	bool joker_is_used = FALSE;
	bool straight = FALSE;

	/* get lowest */
	for (i = 0; i < 5; i++)
	{
		if (NUM_OF(cards[i]) < lowest && !IS_JOKER(cards[i]))
			lowest = NUM_OF(cards[i]);
	}
	
	/* Check Royal Straight Flush */
	if (yaku_check_flush())
	{
	  if( lowest == 0 ){
		for (i = 0; i < 4; i++)
		{
			if (!find_card_num(9 + i)){
				if( have_joker() && !joker_is_used )
				  joker_is_used = TRUE;
				else
				  break;
			}
		}
		if (i == 4) return 3; /* Wow! Royal Straight Flush!!! */
	  }
	  if(lowest == 9){
		for (i = 0; i < 3; i++)
		{
			if (!find_card_num(10 + i))
				break;
		}
		if (i == 3 && have_joker()) return 3; /* Wow! Royal Straight Flush!!! */
	  }
	}

	joker_is_used = FALSE;

	/* Straight Only Check */

	if (lowest == 0) { /* (10 - J - Q - K)[JOKER] - A */
		for (i = 0; i < 4; i++)
		{
			if (!find_card_num(9 + i)) {
				if (have_joker() && !joker_is_used)
					joker_is_used = TRUE;
				else
					break; /* None */
			}
		}
		if(i == 4) straight = TRUE;
	}

	joker_is_used = FALSE;

	for (i = 0; i < 5; i++)
	{
		if(!find_card_num(lowest + i)){
			if( have_joker() && !joker_is_used )
				joker_is_used = TRUE;
			else
				break; /* None */
		}
	}
	if(i == 5) straight = TRUE;
	
	if (straight && yaku_check_flush()) return 2; /* Straight Flush */
	else if(straight) return 1; /* Only Straight */
	else return 0;
}

/*!
 * @brief �ݡ������Υڥ���ξ��֤��֤���
 * @return 0:nopair 1:1 pair 2:2 pair 3:3 cards 4:full house 6:4cards
 */
static int yaku_check_pair(void)
{
	int i, i2, matching = 0;

	for (i = 0; i < 5; i++)
	{
		for (i2 = i+1; i2 < 5; i2++)
		{
			if (IS_JOKER(cards[i]) || IS_JOKER(cards[i2])) continue;
			if (NUM_OF(cards[i]) == NUM_OF(cards[i2]))
				matching++;
		}
	}

	if(have_joker()){
	  switch(matching){
	  case 0:
	    matching = 1;
	    break;
	  case 1:
	    matching = 3;
	    break;
	  case 2:
	    matching = 4;
	    break;
	  case 3:
	    matching = 6;
	    break;
	  case 6:
	    matching = 7;
	    break;
	  default:
	    /* don't reach */
	    break;
	  }
	}

	return matching;
}

#define ODDS_5A 3000 /*!< �ե����֥�����������Ψ */
#define ODDS_5C 400 /*!< �ե����֥����ɤ�����Ψ */
#define ODDS_RF 200 /*!< ����륹�ȥ졼�ȥե�å��������Ψ */
#define ODDS_SF 80 /*!< ���ȥ졼�ȥե�å��������Ψ */
#define ODDS_4C 16 /*!< �ե��������ɤ�����Ψ */
#define ODDS_FH 12 /*!< �ե�ϥ���������Ψ */
#define ODDS_FL 8 /*!< �ե�å��������Ψ */
#define ODDS_ST 4 /*!< ���ȥ졼�Ȥ�����Ψ */
#define ODDS_3C 1 /*!< ���꡼�����ɤ�����Ψ */
#define ODDS_2P 1 /*!< �ġ��ڥ�������Ψ */

/*!
 * @brief �ݡ��������������å��������η�̤���̤�ɽ�����Ĥķ�̤��֤���
 * @return ���ID
 */
static int yaku_check(void)
{
	prt("                            ", 4, 3);

	switch(yaku_check_straight()){
	case 3: /* RF! */
		c_put_str(TERM_YELLOW, _("����륹�ȥ졼�ȥե�å���", "Royal Flush"),  4,  3);
		return ODDS_RF;
	case 2: /* SF! */
		c_put_str(TERM_YELLOW, _("���ȥ졼�ȥե�å���", "Straight Flush"),  4,  3);
		return ODDS_SF;
	case 1:
		c_put_str(TERM_YELLOW, _("���ȥ졼��", "Straight"),  4,  3);
		return ODDS_ST;
	default:
		/* Not straight -- fall through */
		break;
	}

	if (yaku_check_flush())
	{
		c_put_str(TERM_YELLOW, _("�ե�å���", "Flush"),  4,  3);
		return ODDS_FL;
	}

	switch (yaku_check_pair())
	{
	case 1:
		c_put_str(TERM_YELLOW, _("���ڥ�", "One pair"),  4,  3);
		return 0;
	case 2:
		c_put_str(TERM_YELLOW, _("�ġ��ڥ�", "Two pair"),  4,  3);
		return ODDS_2P;
	case 3:
		c_put_str(TERM_YELLOW, _("���꡼������", "Three of a kind"),  4,  3);
		return ODDS_3C;
	case 4:
		c_put_str(TERM_YELLOW, _("�ե�ϥ���", "Full house"),  4,  3);
		return ODDS_FH;
	case 6:
		c_put_str(TERM_YELLOW, _("�ե���������", "Four of a kind"),  4,  3);
		return ODDS_4C;
	case 7:
		if (!NUM_OF(cards[0]) && !NUM_OF(cards[1]))
		{
			c_put_str(TERM_YELLOW, _("�ե����֥�����", "Five ace"),  4,  3);
			return ODDS_5A;
		}
		else
		{
			c_put_str(TERM_YELLOW, _("�ե����֥�����", "Five of a kind"),  4,  3);
			return ODDS_5C;
		}
	default:
		break;
	}
	return 0;
}

/*!
 * @brief �ݡ������μΤƤ�/�Ĥ����󥿡��ե�������ɽ���򹹿����롣
 * @param hoge ��������θ��߰���
 * @param kaeruka �����ɤμΤƤ�/�Ĥ��ե饰����
 * @return �ʤ�
 */
static void display_kaeruka(int hoge, int kaeruka[])
{
	int i;
	char col = TERM_WHITE;
	for (i = 0; i < 5; i++)
	{
		if (i == hoge) col = TERM_YELLOW;
		else if(kaeruka[i]) col = TERM_WHITE;
		else col = TERM_L_BLUE;
		
		if(kaeruka[i])
			c_put_str(col, _("������", "Change"), 14,  5+i*16);
		else
			c_put_str(col, _("�Τ���", " Stay "), 14,  5+i*16);
	}
	if (hoge > 4) col = TERM_YELLOW;
	else col = TERM_WHITE;
	c_put_str(col, _("����", "Sure"), 16,  38);

	/* Hilite current option */
	if (hoge < 5) move_cursor(14, 5+hoge*16);
	else move_cursor(16, 38);
}

/*!
 * @brief �ݡ������μ껥��ɽ�����롣
 * @return �ʤ�
 */
static void display_cards(void)
{
	int i, j;
	char suitcolor[4] = {TERM_YELLOW, TERM_L_RED, TERM_L_BLUE, TERM_L_GREEN};
#ifdef JP
	cptr suit[4] = {"��", "��", "��", "��"};
	cptr card_grph[13][7] = {{"��   %s     ",
				  "     ��     ",
				  "     ��     ",
				  "     ��     ",
				  "     ��     ",
				  "     %s     ",
				  "          ��"},
				 {"��          ",
				  "     %s     ",
				  "            ",
				  "            ",
				  "            ",
				  "     %s     ",
				  "          ��"},
				 {"��          ",
				  "     %s     ",
				  "            ",
				  "     %s     ",
				  "            ",
				  "     %s     ",
				  "          ��"},
				 {"��          ",
				  "   %s  %s   ",
				  "            ",
				  "            ",
				  "            ",
				  "   %s  %s   ",
				  "          ��"},
				 {"��          ",
				  "   %s  %s   ",
				  "            ",
				  "     %s     ",
				  "            ",
				  "   %s  %s   ",
				  "          ��"},
				 {"��          ",
				  "   %s  %s   ",
				  "            ",
				  "   %s  %s   ",
				  "            ",
				  "   %s  %s   ",
				  "          ��"},
				 {"��          ",
				  "   %s  %s   ",
				  "     %s     ",
				  "   %s  %s   ",
				  "            ",
				  "   %s  %s   ",
				  "          ��"},
				 {"��          ",
				  "   %s  %s   ",
				  "     %s     ",
				  "   %s  %s   ",
				  "     %s     ",
				  "   %s  %s   ",
				  "          ��"},
				 {"�� %s  %s   ",
				  "            ",
				  "   %s  %s   ",
				  "     %s     ",
				  "   %s  %s   ",
				  "            ",
				  "   %s  %s ��"},
				 {"10 %s  %s   ",
				  "     %s     ",
				  "   %s  %s   ",
				  "            ",
				  "   %s  %s   ",
				  "     %s     ",
				  "   %s  %s 10"},
				 {"��   ��     ",
				  "%s   ||     ",
				  "     ||     ",
				  "     ||     ",
				  "     ||     ",
				  "   |=��=| %s",
				  "     ��   ��"},
				 {"�� ######   ",
				  "%s#      #  ",
				  "  # ++++ #  ",
				  "  # +==+ #  ",
				  "   # ++ #   ",
				  "    #  #  %s",
				  "     ##   ��"},
				 {"��          ",
				  "%s ���ޡ�   ",
				  "  �æææ�  ",
				  "  �� �� ��  ",
				  "   ��    �� ",
				  "    �� �� %s",
				  "          ��"}};
	cptr joker_grph[7] = {    "            ",
				  "     ��     ",
				  "     ��     ",
				  "     ��     ",
				  "     ��     ",
				  "     ��     ",
				  "            "};

#else

	cptr suit[4] = {"[]", "qp", "<>", "db"};
	cptr card_grph[13][7] = {{"A    %s     ",
				  "     He     ",
				  "     ng     ",
				  "     ba     ",
				  "     nd     ",
				  "     %s     ",
				  "           A"},
				 {"2           ",
				  "     %s     ",
				  "            ",
				  "            ",
				  "            ",
				  "     %s     ",
				  "           2"},
				 {"3           ",
				  "     %s     ",
				  "            ",
				  "     %s     ",
				  "            ",
				  "     %s     ",
				  "           3"},
				 {"4           ",
				  "   %s  %s   ",
				  "            ",
				  "            ",
				  "            ",
				  "   %s  %s   ",
				  "           4"},
				 {"5           ",
				  "   %s  %s   ",
				  "            ",
				  "     %s     ",
				  "            ",
				  "   %s  %s   ",
				  "           5"},
				 {"6           ",
				  "   %s  %s   ",
				  "            ",
				  "   %s  %s   ",
				  "            ",
				  "   %s  %s   ",
				  "           6"},
				 {"7           ",
				  "   %s  %s   ",
				  "     %s     ",
				  "   %s  %s   ",
				  "            ",
				  "   %s  %s   ",
				  "           7"},
				 {"8           ",
				  "   %s  %s   ",
				  "     %s     ",
				  "   %s  %s   ",
				  "     %s     ",
				  "   %s  %s   ",
				  "           8"},
				 {"9  %s  %s   ",
				  "            ",
				  "   %s  %s   ",
				  "     %s     ",
				  "   %s  %s   ",
				  "            ",
				  "   %s  %s  9"},
				 {"10 %s  %s   ",
				  "     %s     ",
				  "   %s  %s   ",
				  "            ",
				  "   %s  %s   ",
				  "     %s     ",
				  "   %s  %s 10"},
				 {"J    /\\     ",
				  "%s   ||     ",
				  "     ||     ",
				  "     ||     ",
				  "     ||     ",
				  "   |=HH=| %s",
				  "     ][    J"},
				 {"Q  ######   ",
				  "%s#      #  ",
				  "  # ++++ #  ",
				  "  # +==+ #  ",
				  "   # ++ #   ",
				  "    #  #  %s",
				  "     ##    Q"},
				 {"K           ",
				  "%s _'~~`_   ",
				  "   jjjjj$&  ",
				  "   q q uu   ",
				  "   c    &   ",
				  "    v__/  %s",
				  "           K"}};
	cptr joker_grph[7] = {    "            ",
				  "     J      ",
				  "     O      ",
				  "     K      ",
				  "     E      ",
				  "     R      ",
				  "            "};
#endif

	for (i = 0; i < 5; i++)
	{
		prt(_("����������������", " +------------+ "),  5,  i*16);
	}

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 7; j++)
		{
			prt(_("��", " |"),  j+6,  i*16);
			if(IS_JOKER(cards[i]))
				c_put_str(TERM_VIOLET, joker_grph[j],  j+6,  2+i*16);
			else
				c_put_str(suitcolor[SUIT_OF(cards[i])], format(card_grph[NUM_OF(cards[i])][j], suit[SUIT_OF(cards[i])], suit[SUIT_OF(cards[i])]),  j+6,  2+i*16);
			prt(_("��", "| "),  j+6,  i*16+14);
		}
	}
	for (i = 0; i < 5; i++)
	{
		prt(_("����������������", " +------------+ "), 13,  i*16);
	}
}

/*!
 * @brief �ݡ������Σ��ץ쥤�롼����
 * @return ���ץ쥤����η��
 */
static int do_poker(void)
{
	int i, k = 2;
	char cmd;
	int deck[53]; /* yamafuda : 0...52 */
	int deck_ptr = 0;
	int kaeruka[5]; /* 0:kaenai 1:kaeru */

	bool done = FALSE;
	bool kettei = TRUE;
	bool kakikae = TRUE;

	reset_deck(deck);

	for (i = 0; i < 5; i++)
	{
		cards[i] = deck[deck_ptr++];
		kaeruka[i] = 0; /* default:nokosu */
	}
	
#if 0
	/* debug:RF */
	cards[0] = 12;
	cards[1] = 0;
	cards[2] = 9;
	cards[3] = 11;
	cards[4] = 10;
#endif
#if 0
	/* debug:SF */
	cards[0] = 3;
	cards[1] = 2;
	cards[2] = 4;
	cards[3] = 6;
	cards[4] = 5;
#endif
#if 0
	/* debug:Four Cards */
	cards[0] = 0;
	cards[1] = 0 + 13 * 1;
	cards[2] = 0 + 13 * 2;
	cards[3] = 0 + 13 * 3;
	cards[4] = 51;
#endif
#if 0
	/* debug:Straight1 */
	cards[0] = 1;
	cards[1] = 0 + 13;
	cards[2] = 3;
	cards[3] = 2 + 26;
	cards[4] = 4;
#endif
#if 0
	/* debug:Straight2 */
	cards[0] = 12;
	cards[1] = 0;
	cards[2] = 9;
	cards[3] = 11 + 13 * 2;
	cards[4] = 10;
#endif
#if 0
	/* debug:Straight3 */
	cards[0] = 52;
	cards[1] = 0;
	cards[2] = 9;
	cards[3] = 11 + 13 * 2;
	cards[4] = 10;
#endif
#if 0
	/* debug:Straight4 */
	cards[0] = 12;
	cards[1] = 52;
	cards[2] = 9;
	cards[3] = 11 + 13 * 2;
	cards[4] = 10;
#endif
#if 0
	/* debug:Straight5 */
	cards[0] = 4;
	cards[1] = 5 + 13;
	cards[2] = 6;
	cards[3] = 7 + 26;
	cards[4] = 3;
#endif
#if 0
	/* debug:Five Card1 */
	cards[0] = 4;
	cards[1] = 52;
	cards[2] = 4 + 13;
	cards[3] = 4 + 26;
	cards[4] = 4 + 39;
#endif
#if 0
	/* debug:Five Card2 */
	cards[1] = 52;
	cards[0] = 4;
	cards[2] = 4 + 13;
	cards[3] = 4 + 26;
	cards[4] = 4 + 39;
#endif
#if 0
	/* debug */
	cards[0] = 52;
	cards[1] = 0;
	cards[2] = 1;
	cards[3] = 2;
	cards[4] = 3;
#endif

	/* suteruno wo kimeru */
	prt(_("�Ĥ������ɤ���Ʋ�����(�����ǰ�ư, ���ڡ���������)��", "Stay witch? "), 0, 0);

	display_cards();
	yaku_check();

	while (!done)
	{
		if (kakikae) display_kaeruka(k+kettei*5, kaeruka);
		kakikae = FALSE;
		cmd = inkey();
		switch (cmd)
		{
		case '6': case 'l': case 'L': case KTRL('F'):
			if (!kettei) k = (k+1)%5;
			else {k = 0;kettei = FALSE;}
			kakikae = TRUE;
			break;
		case '4': case 'h': case 'H': case KTRL('B'):
			if (!kettei) k = (k+4)%5;
			else {k = 4;kettei = FALSE;}
			kakikae = TRUE;
			break;
		case '2': case 'j': case 'J': case KTRL('N'):
			if (!kettei) {kettei = TRUE;kakikae = TRUE;}
			break;
		case '8': case 'k': case 'K': case KTRL('P'):
			if (kettei) {kettei = FALSE;kakikae = TRUE;}
			break;
		case ' ': case '\r':
			if (kettei) done = TRUE;
			else {kaeruka[k] = !kaeruka[k];kakikae = TRUE;}
			break;
		default:
			break;
		}
	}
	
	prt("",0,0);

	for (i = 0; i < 5; i++)
		if (kaeruka[i] == 1) cards[i] = deck[deck_ptr++]; /* soshite toru */

	display_cards();
	
	return yaku_check();
}
#undef SUIT_OF
#undef NUM_OF
#undef IS_JOKER
/* end of poker codes --Koka */

/*!
 * @brief �����Σ��ץ쥤���ȤΥᥤ��롼���� / gamble_comm
 * @param cmd �ץ쥤���륲����ID
 * @return �ʤ�
 */
static bool gamble_comm(int cmd)
{
	int i;
	int roll1, roll2, roll3, choice, odds, win;
	s32b wager;
	s32b maxbet;
	s32b oldgold;

	char out_val[160], tmp_str[80], again;
	cptr p;

	screen_save();

	if (cmd == BACT_GAMBLE_RULES)
	{
		/* Peruse the gambling help file */
		(void)show_file(TRUE, _("jgambling.txt", "gambling.txt"), NULL, 0, 0);
	}
	else
	{
		/* No money */
		if (p_ptr->au < 1)
		{
			msg_print(_("���������ޤ���ʸ�ʤ�����ʤ��������ä���ФƤ�����", 
						"Hey! You don't have gold - get out of here!"));
			msg_print(NULL);
			screen_load();
			return FALSE;
		}

		clear_bldg(5, 23);

		maxbet = p_ptr->lev * 200;

		/* We can't bet more than we have */
		maxbet = MIN(maxbet, p_ptr->au);

		/* Get the wager */
		strcpy(out_val, "");
		sprintf(tmp_str,_("�Ҥ��� (1-%ld)��", "Your wager (1-%ld) ? "), (long int)maxbet);


		/*
		 * Use get_string() because we may need more than
		 * the s16b value returned by get_quantity().
		 */
		if (get_string(tmp_str, out_val, 32))
		{
			/* Strip spaces */
			for (p = out_val; *p == ' '; p++);

			/* Get the wager */
			wager = atol(p);

			if (wager > p_ptr->au)
			{
				msg_print(_("�������⤬­��ʤ�����ʤ������ФƤ�����", "Hey! You don't have the gold - get out of here!"));
				msg_print(NULL);
				screen_load();
				return (FALSE);
			}
			else if (wager > maxbet)
			{
				msg_format(_("%ld������ɤ��������褦���Ĥ�ϼ�äȤ��ʡ�",
							 "I'll take %ld gold of that. Keep the rest."), (long int)maxbet);
				wager = maxbet;
			}
			else if (wager < 1)
			{
				msg_print(_("�ϣˡ���������ɤ���Ϥ���褦��", "Ok, we'll start with 1 gold."));
				wager = 1;
			}
			msg_print(NULL);
			win = FALSE;
			odds = 0;
			oldgold = p_ptr->au;

			sprintf(tmp_str, _("���������ν����: %9ld", "Gold before game: %9ld"), (long int)oldgold);
			prt(tmp_str, 20, 2);
			sprintf(tmp_str, _("���ߤγݤ���:     %9ld", "Current Wager:    %9ld"), (long int)wager);
			prt(tmp_str, 21, 2);

			do
			{
				p_ptr->au -= wager;
				switch (cmd)
				{
				 case BACT_IN_BETWEEN: /* Game of In-Between */
					c_put_str(TERM_GREEN, _("���󡦥ӥȥ�����", "In Between"),5,2);

					odds = 4;
					win = FALSE;
					roll1 = randint1(10);
					roll2 = randint1(10);
					choice = randint1(10);
					sprintf(tmp_str, _("��������: %d        ��������: %d", "Black die: %d       Black Die: %d"), roll1, roll2);

					prt(tmp_str, 8, 3);
					sprintf(tmp_str, _("�֥�����: %d", "Red die: %d"), choice);

					prt(tmp_str, 11, 14);
					if (((choice > roll1) && (choice < roll2)) ||
						((choice < roll1) && (choice > roll2)))
						win = TRUE;
					break;
				case BACT_CRAPS:  /* Game of Craps */
					c_put_str(TERM_GREEN, _("����åץ�", "Craps"), 5, 2);

					win = 3;
					odds = 2;
					roll1 = randint1(6);
					roll2 = randint1(6);
					roll3 = roll1 +  roll2;
					choice = roll3;
					sprintf(tmp_str, _("�������: %d %d      Total: %d", 
									   "First roll: %d %d    Total: %d"), roll1, roll2, roll3);
					prt(tmp_str, 7, 5);
					if ((roll3 == 7) || (roll3 == 11))
						win = TRUE;
					else if ((roll3 == 2) || (roll3 == 3) || (roll3 == 12))
						win = FALSE;
					else
						do
						{
							msg_print(_("�ʤˤ������򲡤��Ȥ⤦��󿶤�ޤ���", "Hit any key to roll again"));

							msg_print(NULL);
							roll1 = randint1(6);
							roll2 = randint1(6);
							roll3 = roll1 +  roll2;
							sprintf(tmp_str, _("����: %d %d          ���:      %d", 
										   "Roll result: %d %d   Total:     %d"), roll1, roll2, roll3);
							prt(tmp_str, 8, 5);
							if (roll3 == choice)
								win = TRUE;
							else if (roll3 == 7)
								win = FALSE;
						} while ((win != TRUE) && (win != FALSE));
					break;

				case BACT_SPIN_WHEEL:  /* Spin the Wheel Game */
					win = FALSE;
					odds = 9;
					c_put_str(TERM_GREEN, _("�롼��å�", "Wheel"), 5, 2);

					prt("0  1  2  3  4  5  6  7  8  9", 7, 5);
					prt("--------------------------------", 8, 3);
					strcpy(out_val, "");
					get_string(_("���֡� (0-9): ", "Pick a number (0-9): "), out_val, 32);

					for (p = out_val; iswspace(*p); p++);
					choice = atol(p);
					if (choice < 0)
					{
						msg_print(_("0�֤ˤ��Ȥ�����", "I'll put you down for 0."));
						choice = 0;
					}
					else if (choice > 9)
					{
						msg_print(_("�ϣˡ�9�֤ˤ��Ȥ�����", "Ok, I'll put you down for 9."));
						choice = 9;
					}
					msg_print(NULL);
					roll1 = randint0(10);
					sprintf(tmp_str, _("�롼��åȤϲ�ꡢ�ߤޤä������Ԥ� %d�֤���", 
									   "The wheel spins to a stop and the winner is %d"), roll1);
					prt(tmp_str, 13, 3);
					prt("", 9, 0);
					prt("*", 9, (3 * roll1 + 5));
					if (roll1 == choice)
						win = TRUE;
					break;

				case BACT_DICE_SLOTS: /* The Dice Slots */
					c_put_str(TERM_GREEN,  _("������������å�", "Dice Slots"), 5, 2);
					c_put_str(TERM_YELLOW, _("����   ����            2", ""), 6, 37);
					c_put_str(TERM_YELLOW, _("����   ����   ����   5", ""), 7, 37);
					c_put_str(TERM_ORANGE, _("����� ����� ����� 10", ""), 8, 37);
					c_put_str(TERM_UMBER, _("��       ��       ��       20", ""), 9, 37);
					c_put_str(TERM_SLATE, _("��       ��       ��       50", ""), 10, 37);
					c_put_str(TERM_VIOLET, _("�ץ��   �ץ��   �ץ��   200", ""), 11, 37);
					c_put_str(TERM_RED, _("�����꡼ �����꡼ �����꡼ 1000", ""), 12, 37);
					
					win = FALSE;
					roll1 = randint1(21);
					for (i=6;i>0;i--)
					{
						if ((roll1-i) < 1)
						{
							roll1 = 7-i;
							break;
						}
						roll1 -= i;
					}
					roll2 = randint1(21);
					for (i=6;i>0;i--)
					{
						if ((roll2-i) < 1)
						{
							roll2 = 7-i;
							break;
						}
						roll2 -= i;
					}
					choice = randint1(21);
					for (i=6;i>0;i--)
					{
						if ((choice-i) < 1)
						{
							choice = 7-i;
							break;
						}
						choice -= i;
					}
					put_str("/--------------------------\\", 7, 2);
					prt("\\--------------------------/", 17, 2);
					display_fruit(8,  3, roll1 - 1);
					display_fruit(8, 12, roll2 - 1);
					display_fruit(8, 21, choice - 1);
					if ((roll1 == roll2) && (roll2 == choice))
					{
						win = TRUE;
						switch(roll1)
						{
						case 1:
							odds = 5;break;
						case 2:
							odds = 10;break;
						case 3:
							odds = 20;break;
						case 4:
							odds = 50;break;
						case 5:
							odds = 200;break;
						case 6:
							odds = 1000;break;
						}
					}
					else if ((roll1 == 1) && (roll2 == 1))
					{
						win = TRUE;
						odds = 2;
					}
					break;
				case BACT_POKER:
					win = FALSE;
					odds = do_poker();
					if (odds) win = TRUE;
					break;
				}

				if (win)
				{
					prt(_("���ʤ��ξ���", "YOU WON"), 16, 37);

					p_ptr->au += odds * wager;
					sprintf(tmp_str, _("��Ψ: %d", "Payoff: %d"), odds);

					prt(tmp_str, 17, 37);
				}
				else
				{
					prt(_("���ʤ����餱", "You Lost"), 16, 37);
					prt("", 17, 37);
				}
				sprintf(tmp_str, _("���ߤν����:     %9ld", "Current Gold:     %9ld"), (long int)p_ptr->au);

				prt(tmp_str, 22, 2);
				prt(_("�⤦����(Y/N)��", "Again(Y/N)?"), 18, 37);

				move_cursor(18, 52);
				again = inkey();
				prt("", 16, 37);
				prt("", 17, 37);
				prt("", 18, 37);
				if (wager > p_ptr->au)
				{
					msg_print(_("�������⤬­��ʤ�����ʤ�������������ФƹԤ���", 
								"Hey! You don't have the gold - get out of here!"));
					msg_print(NULL);

					/* Get out here */
					break;
				}
			} while ((again == 'y') || (again == 'Y'));

			prt("", 18, 37);
			if (p_ptr->au >= oldgold)
			{
				msg_print(_("�ֺ�����٤����ʡ��Ǥ⼡�Ϥ��ä������äƤ�뤫��ʡ����Фˡ���",
							"You came out a winner! We'll win next time, I'm sure."));
				chg_virtue(V_CHANCE, 3);
			}
			else
			{
				msg_print(_("�ֶ�򥹥äƤ��ޤä��ʡ���Ϥϡ������˵��ä���������������", "You lost gold! Haha, better head home."));
				chg_virtue(V_CHANCE, -3);
			}
		}
		msg_print(NULL);
	}
	screen_load();
	return (TRUE);
}

/*!
 * @brief ��󥹥���Ʈ����˻��äǤ����󥹥�����Ƚ��
 * @param r_idx ��󥹥����ɣ�
 * @details ����NEVER_MOVE MULTIPLY QUANTUM RF7_AQUATIC RF7_CHAMELEON�Τ�������������
 * �����ʳ��Τʤ�餫��HP������ʤ���äƤ��뤳�ȡ�
 * @return ���äǤ��뤫�ݤ�
 */
static bool vault_aux_battle(MONRACE_IDX r_idx)
{
	int i;
	HIT_POINT dam = 0;

	monster_race *r_ptr = &r_info[r_idx];

	/* Decline town monsters */
/*	if (!mon_hook_dungeon(r_idx)) return FALSE; */

	/* Decline unique monsters */
/*	if (r_ptr->flags1 & (RF1_UNIQUE)) return (FALSE); */
/*	if (r_ptr->flags7 & (RF7_NAZGUL)) return (FALSE); */

	if (r_ptr->flags1 & (RF1_NEVER_MOVE)) return (FALSE);
	if (r_ptr->flags2 & (RF2_MULTIPLY)) return (FALSE);
	if (r_ptr->flags2 & (RF2_QUANTUM)) return (FALSE);
	if (r_ptr->flags7 & (RF7_AQUATIC)) return (FALSE);
	if (r_ptr->flags7 & (RF7_CHAMELEON)) return (FALSE);

	for (i = 0; i < 4; i++)
	{
		if (r_ptr->blow[i].method == RBM_EXPLODE) return (FALSE);
		if (r_ptr->blow[i].effect != RBE_DR_MANA) dam += r_ptr->blow[i].d_dice;
	}
	if (!dam && !(r_ptr->flags4 & (RF4_BOLT_MASK | RF4_BEAM_MASK | RF4_BALL_MASK | RF4_BREATH_MASK)) && !(r_ptr->a_ability_flags1 & (RF5_BOLT_MASK | RF5_BEAM_MASK | RF5_BALL_MASK | RF5_BREATH_MASK)) && !(r_ptr->a_ability_flags2 & (RF6_BOLT_MASK | RF6_BEAM_MASK | RF6_BALL_MASK | RF6_BREATH_MASK))) return (FALSE);

	/* Okay */
	return (TRUE);
}

/*!
 * @brief ��󥹥���Ʈ����˻��ä����󥹥�����ꥻ�åȤ��롣
 * @return �ʤ�
 */
void battle_monsters(void)
{
	int total, i;
	int max_dl = 0;
	int mon_level;
	int power[4];
	bool tekitou;
	bool old_inside_battle = p_ptr->inside_battle;

	for (i = 0; i < max_d_idx; i++)
		if (max_dl < max_dlv[i]) max_dl = max_dlv[i];

	mon_level = randint1(MIN(max_dl, 122))+5;
	if (randint0(100) < 60)
	{
		i = randint1(MIN(max_dl, 122))+5;
		mon_level = MAX(i, mon_level);
	}
	if (randint0(100) < 30)
	{
		i = randint1(MIN(max_dl, 122))+5;
		mon_level = MAX(i, mon_level);
	}

	while (1)
	{
		total = 0;
		tekitou = FALSE;
		for(i = 0; i < 4; i++)
		{
			MONRACE_IDX r_idx;
			int j;
			while (1)
			{
				get_mon_num_prep(vault_aux_battle, NULL);
				p_ptr->inside_battle = TRUE;
				r_idx = get_mon_num(mon_level);
				p_ptr->inside_battle = old_inside_battle;
				if (!r_idx) continue;

				if ((r_info[r_idx].flags1 & RF1_UNIQUE) || (r_info[r_idx].flags7 & RF7_UNIQUE2))
				{
					if ((r_info[r_idx].level + 10) > mon_level) continue;
				}

				for (j = 0; j < i; j++)
					if(r_idx == battle_mon[j]) break;
				if (j<i) continue;

				break;
			}
			battle_mon[i] = r_idx;
			if (r_info[r_idx].level < 45) tekitou = TRUE;
		}

		for (i=0;i<4;i++)
		{
			monster_race *r_ptr = &r_info[battle_mon[i]];
			int num_taisei = count_bits(r_ptr->flagsr & (RFR_IM_ACID | RFR_IM_ELEC | RFR_IM_FIRE | RFR_IM_COLD | RFR_IM_POIS));

			if (r_ptr->flags1 & RF1_FORCE_MAXHP)
				power[i] = r_ptr->hdice * r_ptr->hside * 2;
			else
				power[i] = r_ptr->hdice * (r_ptr->hside + 1);
			power[i] = power[i] * (100 + r_ptr->level) / 100;
			if (r_ptr->speed > 110)
				power[i] = power[i] * (r_ptr->speed * 2 - 110) / 100;
			if (r_ptr->speed < 110)
				power[i] = power[i] * (r_ptr->speed - 20) / 100;
			if (num_taisei > 2)
				power[i] = power[i] * (num_taisei*2+5) / 10;
			else if (r_ptr->a_ability_flags2 & RF6_INVULNER)
				power[i] = power[i] * 4 / 3;
			else if (r_ptr->a_ability_flags2 & RF6_HEAL)
				power[i] = power[i] * 4 / 3;
			else if (r_ptr->a_ability_flags1 & RF5_DRAIN_MANA)
				power[i] = power[i] * 11 / 10;
			if (r_ptr->flags1 & RF1_RAND_25)
				power[i] = power[i] * 9 / 10;
			if (r_ptr->flags1 & RF1_RAND_50)
				power[i] = power[i] * 9 / 10;
			if (r_ptr->flagsr & RFR_RES_ALL) power[i] *= 100000;
			if (r_ptr->arena_ratio) power[i] = power[i] * r_ptr->arena_ratio / 100;


			total += power[i];
		}
		for (i=0;i<4;i++)
		{
			power[i] = total*60/power[i];
			if (tekitou && ((power[i] < 160) || power[i] > 1500)) break;
			if ((power[i] < 160) && randint0(20)) break;
			if (power[i] < 101) power[i] = 100 + randint1(5);
			mon_odds[i] = power[i];
		}
		if (i == 4) break;
	}
}

/*!
 * @brief ��󥹥���Ʈ����Υᥤ��롼����
 * @return �Ҥ��򳫻Ϥ������ݤ�
 */
static bool kakutoujou(void)
{
	s32b maxbet;
	s32b wager;
	char out_val[160], tmp_str[80];
	cptr p;

	if ((turn - old_battle) > TURNS_PER_TICK*250)
	{
		battle_monsters();
		old_battle = turn;
	}

	screen_save();

	/* No money */
	if (p_ptr->au < 1)
	{
		msg_print(_("���������ޤ���ʸ�ʤ�����ʤ��������ä���ФƤ�����", "Hey! You don't have gold - get out of here!"));
		msg_print(NULL);
		screen_load();
		return FALSE;
	}
	else
	{
		int i;

		clear_bldg(4, 10);

		prt(_("��󥹥���                                                     ��Ψ",
			  "Monsters                                                       Odds"), 4, 4);
		for (i=0;i<4;i++)
		{
			char buf[80];
			monster_race *r_ptr = &r_info[battle_mon[i]];

			sprintf(buf, _("%d) %-58s  %4ld.%02ld��", "%d) %-58s  %4ld.%02ld"), i+1, 
						 _(format("%s%s",r_name + r_ptr->name, (r_ptr->flags1 & RF1_UNIQUE) ? "��ɤ�" : "      "),
						   format("%s%s", (r_ptr->flags1 & RF1_UNIQUE) ? "Fake " : "", r_name + r_ptr->name)),
						(long int)mon_odds[i]/100, (long int)mon_odds[i]%100);
			prt(buf, 5+i, 1);
		}
		prt(_("�ɤ���Ҥ��ޤ���:", "Which monster: "), 0, 0);
		while(1)
		{
			i = inkey();

			if (i == ESCAPE)
			{
				screen_load();
				return FALSE;
			}
			if (i >= '1' && i <= '4')
			{
				sel_monster = i-'1';
				battle_odds = mon_odds[sel_monster];
				break;
			}
			else bell();
		}

		clear_bldg(4,4);
		for (i=0;i<4;i++)
			if (i !=sel_monster) clear_bldg(i+5,i+5);

		maxbet = p_ptr->lev * 200;

		/* We can't bet more than we have */
		maxbet = MIN(maxbet, p_ptr->au);

		/* Get the wager */
		strcpy(out_val, "");
		sprintf(tmp_str,_("�Ҥ��� (1-%ld)��", "Your wager (1-%ld) ? "), (long int)maxbet);
		/*
		 * Use get_string() because we may need more than
		 * the s16b value returned by get_quantity().
		 */
		if (get_string(tmp_str, out_val, 32))
		{
			/* Strip spaces */
			for (p = out_val; *p == ' '; p++);

			/* Get the wager */
			wager = atol(p);

			if (wager > p_ptr->au)
			{
				msg_print(_("�������⤬­��ʤ�����ʤ������ФƤ�����", "Hey! You don't have the gold - get out of here!"));

				msg_print(NULL);
				screen_load();
				return (FALSE);
			}
			else if (wager > maxbet)
			{
				msg_format(_("%ld������ɤ��������褦���Ĥ�ϼ�äȤ��ʡ�", "I'll take %ld gold of that. Keep the rest."), (long int)maxbet);

				wager = maxbet;
			}
			else if (wager < 1)
			{
				msg_print(_("�ϣˡ���������ɤǤ�������", "Ok, we'll start with 1 gold."));
				wager = 1;
			}
			msg_print(NULL);
			battle_odds = MAX(wager+1, wager * battle_odds / 100);
			kakekin = wager;
			p_ptr->au -= wager;
			reset_tim_flags();

			/* Save the surface floor as saved floor */
			prepare_change_floor_mode(CFM_SAVE_FLOORS);

			p_ptr->inside_battle = TRUE;
			p_ptr->leaving = TRUE;

			leave_bldg = TRUE;
			screen_load();

			return (TRUE);
		}
	}
	screen_load();

	return (FALSE);
}

/*!
 * @brief �����ξ޶������ɽ�����롣
 * @return �ʤ�
 */
static void today_target(void)
{
	char buf[160];
	monster_race *r_ptr = &r_info[today_mon];

	clear_bldg(4,18);
	c_put_str(TERM_YELLOW, _("�����ξ޶��", "Wanted monster that changes from day to day"), 5, 10);
	sprintf(buf,_("�������åȡ� %s", "target: %s"),r_name + r_ptr->name);
	c_put_str(TERM_YELLOW, buf, 6, 10);
	sprintf(buf,_("���� ---- $%d", "corpse   ---- $%d"), (int)r_ptr->level * 50 + 100);
	prt(buf, 8, 10);
	sprintf(buf,_("��   ---- $%d", "skeleton ---- $%d"), (int)r_ptr->level * 30 + 60);
	prt(buf, 9, 10);
	p_ptr->today_mon = today_mon;
}

/*!
 * @brief �ĥ��Υ��ξ޶������ɽ�����롣
 * @return �ʤ�
 */
static void tsuchinoko(void)
{
	clear_bldg(4,18);
	c_put_str(TERM_YELLOW, _("������������󥹡�����", "Big chance to quick money!!!"), 5, 10);
	c_put_str(TERM_YELLOW, _("�������åȡ��������á֥ĥ��Υ���", "target: the rarest animal 'Tsuchinoko'"), 6, 10);
	c_put_str(TERM_WHITE, _("������� ---- $1,000,000", "catch alive ---- $1,000,000"), 8, 10);
	c_put_str(TERM_WHITE, _("����     ----   $200,000", "corpse      ----   $200,000"), 9, 10);
	c_put_str(TERM_WHITE, _("��       ----   $100,000", "bones       ----   $100,000"), 10, 10);
}

/*!
 * @brief �̾�ξ޶������ɽ�����롣
 * @return �ʤ�
 */
static void shoukinkubi(void)
{
	int i;
	int y = 0;

	clear_bldg(4,18);
	prt(_("���Τ����������󽷤򺹤��夲�ޤ���", "Offer a prize when you bring a wanted monster's corpse"),4 ,10);
	c_put_str(TERM_YELLOW, _("���ߤξ޶��", "Wanted monsters"), 6, 10);

	for (i = 0; i < MAX_KUBI; i++)
	{
		byte color;
		cptr done_mark;
		monster_race *r_ptr = &r_info[(kubi_r_idx[i] > 10000 ? kubi_r_idx[i] - 10000 : kubi_r_idx[i])];

		if (kubi_r_idx[i] > 10000)
		{
			color = TERM_RED;
			done_mark = _("(��)", "(done)");
		}
		else
		{
			color = TERM_WHITE;
			done_mark = "";
		}

		c_prt(color, format("%s %s", r_name + r_ptr->name, done_mark), y+7, 10);

		y = (y+1) % 10;
		if (!y && (i < MAX_KUBI -1))
		{
			prt(_("���������򲡤��Ƥ�������", "Hit any key."), 0, 0);
			(void)inkey();
			prt("", 0, 0);
			clear_bldg(7,18);
		}
	}
}



/*!
 * �޶����󽷥ơ��֥� / List of prize object
 */
static struct {
	OBJECT_TYPE_VALUE tval; /*!< �١��������ƥ�Υᥤ�����ID */
	OBJECT_SUBTYPE_VALUE sval; /*!< �١��������ƥ�Υ��ּ���ID */
} prize_list[MAX_KUBI] = 
{
	{TV_POTION, SV_POTION_CURING},
	{TV_POTION, SV_POTION_SPEED},
	{TV_POTION, SV_POTION_SPEED},
	{TV_POTION, SV_POTION_RESISTANCE},
	{TV_POTION, SV_POTION_ENLIGHTENMENT},

	{TV_POTION, SV_POTION_HEALING},
	{TV_POTION, SV_POTION_RESTORE_MANA},
	{TV_SCROLL, SV_SCROLL_STAR_DESTRUCTION},
	{TV_POTION, SV_POTION_STAR_ENLIGHTENMENT},
	{TV_SCROLL, SV_SCROLL_SUMMON_PET},

	{TV_SCROLL, SV_SCROLL_GENOCIDE},
	{TV_POTION, SV_POTION_STAR_HEALING},
	{TV_POTION, SV_POTION_STAR_HEALING},
	{TV_POTION, SV_POTION_NEW_LIFE},
	{TV_SCROLL, SV_SCROLL_MASS_GENOCIDE},

	{TV_POTION, SV_POTION_LIFE},
	{TV_POTION, SV_POTION_LIFE},
	{TV_POTION, SV_POTION_AUGMENTATION},
	{TV_POTION, SV_POTION_INVULNERABILITY},
	{TV_SCROLL, SV_SCROLL_ARTIFACT},
};

/*!
 * @brief �޶��ΰ����������� / Get prize
 * @return �Ƽ�޶��Τ����줫�Ǥⴹ�⤬�Ԥ�줿���ݤ���
 */
static bool kankin(void)
{
	int i, j;
	bool change = FALSE;
	char o_name[MAX_NLEN];
	object_type *o_ptr;

	/* Loop for inventory and right/left arm */
	for (i = 0; i <= INVEN_LARM; i++)
	{
		o_ptr = &inventory[i];

		/* Living Tsuchinoko worthes $1000000 */
		if ((o_ptr->tval == TV_CAPTURE) && (o_ptr->pval == MON_TSUCHINOKO))
		{
			char buf[MAX_NLEN+20];
			object_desc(o_name, o_ptr, 0);
			sprintf(buf, _("%s �򴹶⤷�ޤ�����", "Convert %s into money? "),o_name);
			if (get_check(buf))
			{
				msg_format(_("�޶� %ld��������줿��", "You get %ldgp."), (long int)(1000000L * o_ptr->number));
				p_ptr->au += 1000000L * o_ptr->number;
				p_ptr->redraw |= (PR_GOLD);
				inven_item_increase(i, -o_ptr->number);
				inven_item_describe(i);
				inven_item_optimize(i);
			}
			change = TRUE;
		}
	}

	for (i = 0; i < INVEN_PACK; i++)
	{
		o_ptr = &inventory[i];

		/* Corpse of Tsuchinoko worthes $200000 */
		if ((o_ptr->tval == TV_CORPSE) && (o_ptr->sval == SV_CORPSE) && (o_ptr->pval == MON_TSUCHINOKO))
		{
			char buf[MAX_NLEN+20];
			object_desc(o_name, o_ptr, 0);
			sprintf(buf, _("%s �򴹶⤷�ޤ�����", "Convert %s into money? "),o_name);
			if (get_check(buf))
			{
				msg_format(_("�޶� %ld��������줿��", "You get %ldgp."), (long int)(200000L * o_ptr->number));
				p_ptr->au += 200000L * o_ptr->number;
				p_ptr->redraw |= (PR_GOLD);
				inven_item_increase(i, -o_ptr->number);
				inven_item_describe(i);
				inven_item_optimize(i);
			}
			change = TRUE;
		}
	}

	for (i = 0; i < INVEN_PACK; i++)
	{
		o_ptr = &inventory[i];

		/* Bones of Tsuchinoko worthes $100000 */
		if ((o_ptr->tval == TV_CORPSE) && (o_ptr->sval == SV_SKELETON) && (o_ptr->pval == MON_TSUCHINOKO))
		{
			char buf[MAX_NLEN+20];
			object_desc(o_name, o_ptr, 0);
			sprintf(buf, _("%s �򴹶⤷�ޤ�����", "Convert %s into money? "),o_name);
			if (get_check(buf))
			{
				msg_format(_("�޶� %ld��������줿��", "You get %ldgp."), (long int)(100000L * o_ptr->number));
				p_ptr->au += 100000L * o_ptr->number;
				p_ptr->redraw |= (PR_GOLD);
				inven_item_increase(i, -o_ptr->number);
				inven_item_describe(i);
				inven_item_optimize(i);
			}
			change = TRUE;
		}
	}

	for (i = 0; i < INVEN_PACK; i++)
	{
		o_ptr = &inventory[i];
		if ((o_ptr->tval == TV_CORPSE) && (o_ptr->sval == SV_CORPSE) && (streq(r_name + r_info[o_ptr->pval].name, r_name + r_info[today_mon].name)))
		{
			char buf[MAX_NLEN+20];
			object_desc(o_name, o_ptr, 0);
			sprintf(buf, _("%s �򴹶⤷�ޤ�����", "Convert %s into money? "),o_name);
			if (get_check(buf))
			{
				msg_format(_("�޶� %ld��������줿��", "You get %ldgp."), (long int)((r_info[today_mon].level * 50 + 100) * o_ptr->number));
				p_ptr->au += (r_info[today_mon].level * 50 + 100) * o_ptr->number;
				p_ptr->redraw |= (PR_GOLD);
				inven_item_increase(i, -o_ptr->number);
				inven_item_describe(i);
				inven_item_optimize(i);
			}
			change = TRUE;
		}
	}

	for (i = 0; i < INVEN_PACK; i++)
	{
		o_ptr = &inventory[i];

		if ((o_ptr->tval == TV_CORPSE) && (o_ptr->sval == SV_SKELETON) && (streq(r_name + r_info[o_ptr->pval].name, r_name + r_info[today_mon].name)))
		{
			char buf[MAX_NLEN+20];
			object_desc(o_name, o_ptr, 0);
			sprintf(buf, _("%s �򴹶⤷�ޤ�����", "Convert %s into money? "),o_name);
			if (get_check(buf))
			{
				msg_format(_("�޶� %ld��������줿��", "You get %ldgp."), (long int)((r_info[today_mon].level * 30 + 60) * o_ptr->number));
				p_ptr->au += (r_info[today_mon].level * 30 + 60) * o_ptr->number;
				p_ptr->redraw |= (PR_GOLD);
				inven_item_increase(i, -o_ptr->number);
				inven_item_describe(i);
				inven_item_optimize(i);
			}
			change = TRUE;
		}
	}

	for (j = 0; j < MAX_KUBI; j++)
	{
		/* Need reverse order --- Positions will be changed in the loop */
		for (i = INVEN_PACK-1; i >= 0; i--)
		{
			o_ptr = &inventory[i];
			if ((o_ptr->tval == TV_CORPSE) && (o_ptr->pval == kubi_r_idx[j]))
			{
				char buf[MAX_NLEN+20];
				int num, k, item_new;
				object_type forge;

				object_desc(o_name, o_ptr, 0);
				sprintf(buf, _("%s���Ϥ��ޤ�����", "Hand %s over? "),o_name);
				if (!get_check(buf)) continue;

#if 0 /* Obsoleted */
				msg_format(_("�޶� %ld��������줿��", "You get %ldgp."), (r_info[kubi_r_idx[j]].level + 1) * 300 * o_ptr->number);
				p_ptr->au += (r_info[kubi_r_idx[j]].level+1) * 300 * o_ptr->number;
				p_ptr->redraw |= (PR_GOLD);
				inven_item_increase(i, -o_ptr->number);
				inven_item_describe(i);
				inven_item_optimize(i);
				chg_virtue(V_JUSTICE, 5);
				kubi_r_idx[j] += 10000;

				change = TRUE;
#endif /* Obsoleted */

				/* Hand it first */
				inven_item_increase(i, -o_ptr->number);
				inven_item_describe(i);
				inven_item_optimize(i);

				chg_virtue(V_JUSTICE, 5);
				kubi_r_idx[j] += 10000;

				/* Count number of unique corpses already handed */
				for (num = 0, k = 0; k < MAX_KUBI; k++)
				{
					if (kubi_r_idx[k] >= 10000) num++;
				}
				msg_format(_("����ǹ�� %d �ݥ���ȳ������ޤ�����" ,"You earned %d point%s total."), num, (num > 1 ? "s" : ""));

				/* Prepare to make a prize */
				object_prep(&forge, lookup_kind(prize_list[num-1].tval, prize_list[num-1].sval));
				apply_magic(&forge, object_level, AM_NO_FIXED_ART);

				/* Identify it fully */
				object_aware(&forge);
				object_known(&forge);

				/*
				 * Hand it --- Assume there is an empty slot.
				 * Since a corpse is handed at first,
				 * there is at least one empty slot.
				 */
				item_new = inven_carry(&forge);

				/* Describe the object */
				object_desc(o_name, &forge, 0);
				msg_format(_("%s(%c)����ä���", "You get %s (%c). "), o_name, index_to_label(item_new));

				/* Auto-inscription */
				autopick_alter_item(item_new, FALSE);

				/* Handle stuff */
				handle_stuff();

				change = TRUE;
			}
		}
	}

	if (!change)
	{
		msg_print(_("�޶������줽���ʤ�Τϻ��äƤ��ʤ��ä���", "You have nothing."));
		msg_print(NULL);
		return FALSE;
	}
	return TRUE;
}

/*!
 * @brief ��̴�θ����Ȥʤ��󥹥������ɤ������֤���
 * @param r_idx Ƚ���оݤȤʤ��󥹥����Σɣ�
 * @return ��̴�θ����Ȥʤ����뤫�ݤ���
 */
bool get_nightmare(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	/* Require eldritch horrors */
	if (!(r_ptr->flags2 & (RF2_ELDRITCH_HORROR))) return (FALSE);

	/* Require high level */
	if (r_ptr->level <= p_ptr->lev) return (FALSE);

	/* Accept this monster */
	return (TRUE);
}

/*!
 * @brief �ɲ������ѥ��֥롼����
 * @details inn commands\n
 * Note that resting for the night was a perfect way to avoid player\n
 * ghosts in the town *if* you could only make it to the inn in time (-:\n
 * Now that the ghosts are temporarily disabled in 2.8.X, this function\n
 * will not be that useful.  I will keep it in the hopes the player\n
 * ghost code does become a reality again. Does help to avoid filthy urchins.\n
 * Resting at night is also a quick way to restock stores -KMW-\n
 * @param cmd �ɲ������ѻ���ID
 * @return ���ߤ����Ѥ��ºݤ˹Ԥ�줿���ݤ���
 */
static bool inn_comm(int cmd)
{
	switch (cmd)
	{
		case BACT_FOOD: /* Buy food & drink */
			if (p_ptr->food >= PY_FOOD_FULL)
			{
				msg_print(_("������ʢ����", "You are full now."));
				return FALSE;
			}
			msg_print(_("�С��ƥ�Ϥ����餫�ο���ʪ�ȥӡ���򤯤줿��", "The barkeep gives you some gruel and a beer."));
			(void)set_food(PY_FOOD_MAX - 1);
			break;

		case BACT_REST: /* Rest for the night */
			if ((p_ptr->poisoned) || (p_ptr->cut))
			{
				msg_print(_("���ʤ���ɬ�פʤΤ������ǤϤʤ������żԤǤ���", "You need a healer, not a room."));
				msg_print(NULL);
				msg_print(_("���ߤޤ��󡢤Ǥ⤦����ï���˻�ʤ���㺤��ޤ���ǡ�", "Sorry, but don't want anyone dying in here."));
			}
			else
			{
				s32b oldturn = turn;
				int prev_day, prev_hour, prev_min;

				extract_day_hour_min(&prev_day, &prev_hour, &prev_min);
				if ((prev_hour >= 6) && (prev_hour <= 17)) 
					do_cmd_write_nikki(NIKKI_BUNSHOU, 0, _("�ɲ�����ޤä���", "stay over daytime at the inn."));
				else
					do_cmd_write_nikki(NIKKI_BUNSHOU, 0, _("�ɲ�����ޤä���", "stay over night at the inn."));
				
				turn = (turn / (TURNS_PER_TICK * TOWN_DAWN / 2) + 1) * (TURNS_PER_TICK * TOWN_DAWN / 2);
				if (dungeon_turn < dungeon_turn_limit)
				{
					dungeon_turn += MIN((turn - oldturn), TURNS_PER_TICK * 250) * INN_DUNGEON_TURN_ADJ;
					if (dungeon_turn > dungeon_turn_limit) dungeon_turn = dungeon_turn_limit;
				}

				prevent_turn_overflow();

				if ((prev_hour >= 18) && (prev_hour <= 23)) do_cmd_write_nikki(NIKKI_HIGAWARI, 0, NULL);
				p_ptr->chp = p_ptr->mhp;

				if (ironman_nightmare)
				{
					msg_print(_("̲��˽����ȶ��������ʤ�����褮�ä���", "Horrible visions flit through your mind as you sleep."));

					/* Have some nightmares */
					while(1)
					{
						sanity_blast(NULL, FALSE);
						if (!one_in_(3)) break;
					}

					msg_print(_("���ʤ����䶫�����ܤ�Фޤ�����", "You awake screaming."));
					do_cmd_write_nikki(NIKKI_BUNSHOU, 0, _("��̴�ˤ��ʤ���Ƥ褯̲��ʤ��ä���", "be troubled by a nightmare."));
				}
				else
				{
					set_blind(0);
					set_confused(0);
					p_ptr->stun = 0;
					p_ptr->chp = p_ptr->mhp;
					p_ptr->csp = p_ptr->msp;
					if (p_ptr->pclass == CLASS_MAGIC_EATER)
					{
						int i;
						for (i = 0; i < 72; i++)
						{
							p_ptr->magic_num1[i] = p_ptr->magic_num2[i] * EATER_CHARGE;
						}
						for (; i < 108; i++)
						{
							p_ptr->magic_num1[i] = 0;
						}
					}

					if ((prev_hour >= 6) && (prev_hour <= 17))
					{
						msg_print(_("���ʤ��ϥ�ե�å��夷���ܳФᡢͼ����ޤ�����", "You awake refreshed for the evening."));
						do_cmd_write_nikki(NIKKI_BUNSHOU, 0, _("ͼ����ޤ�����", "awake refreshed."));
					}
					else
					{
						msg_print(_("���ʤ��ϥ�ե�å��夷���ܳФᡢ����������ޤ�����", "You awake refreshed for the new day."));
						do_cmd_write_nikki(NIKKI_BUNSHOU, 0, _("������������ī��ޤ�����", "awake refreshed."));
					}
				}
			}
			break;

		case BACT_RUMORS: /* Listen for rumors */
			{
				display_rumor(TRUE);
				break;
			}
	}

	return (TRUE);
}


/*!
 * @brief �������Ⱦ����ɽ�����ĤĽ������롣/ Display quest information
 * @param questnum �������Ȥ�ID
 * @param do_init �������Ȥγ��Ͻ���(TRUE)����̽�����(FALSE)
 * @return �ʤ�
 */
static void get_questinfo(IDX questnum, bool do_init)
{
	int     i;
	IDX     old_quest;
	char    tmp_str[80];

	/* Clear the text */
	for (i = 0; i < 10; i++)
	{
		quest_text[i][0] = '\0';
	}

	quest_text_line = 0;

	/* Set the quest number temporary */
	old_quest = p_ptr->inside_quest;
	p_ptr->inside_quest = questnum;

	/* Get the quest text */
	init_flags = INIT_SHOW_TEXT;
	if (do_init) init_flags |= INIT_ASSIGN;

	process_dungeon_file("q_info.txt", 0, 0, 0, 0);

	/* Reset the old quest number */
	p_ptr->inside_quest = old_quest;

	/* Print the quest info */
	sprintf(tmp_str, _("�������Ⱦ��� (����: %d ������)", "Quest Information (Danger level: %d)"), (int)quest[questnum].level);

	prt(tmp_str, 5, 0);

	prt(quest[questnum].name, 7, 0);

	for (i = 0; i < 10; i++)
	{
		c_put_str(TERM_YELLOW, quest_text[i], i + 8, 0);
	}
}

/*!
 * @brief �������Ƚ����Υᥤ��롼���� / Request a quest from the Lord.
 * @return �ʤ�
 */
static void castle_quest(void)
{
	IDX q_index = 0;
	monster_race    *r_ptr;
	quest_type      *q_ptr;
	cptr            name;


	clear_bldg(4, 18);

	/* Current quest of the building */
	q_index = cave[p_ptr->y][p_ptr->x].special;

	/* Is there a quest available at the building? */
	if (!q_index)
	{
		put_str(_("���ΤȤ��������ȤϤ���ޤ���", "I don't have a quest for you at the moment."), 8, 0);
		return;
	}

	q_ptr = &quest[q_index];

	/* Quest is completed */
	if (q_ptr->status == QUEST_STATUS_COMPLETED)
	{
		/* Rewarded quest */
		q_ptr->status = QUEST_STATUS_REWARDED;

		get_questinfo(q_index, FALSE);

		reinit_wilderness = TRUE;
	}
	/* Failed quest */
	else if (q_ptr->status == QUEST_STATUS_FAILED)
	{
		get_questinfo(q_index, FALSE);

		/* Mark quest as done (but failed) */
		q_ptr->status = QUEST_STATUS_FAILED_DONE;

		reinit_wilderness = TRUE;
	}
	/* Quest is still unfinished */
	else if (q_ptr->status == QUEST_STATUS_TAKEN)
	{
		put_str(_("���ʤ��ϸ��ߤΥ������Ȥ�λ�����Ƥ��ޤ���", "You have not completed your current quest yet!"), 8, 0);
		put_str(_("CTRL-Q��Ȥ��Х������Ȥξ��֤������å��Ǥ��ޤ���", "Use CTRL-Q to check the status of your quest."), 9, 0);
		put_str(_("�������Ȥ򽪤�餻������ä���Ʋ�������", "Return when you have completed your quest."), 12, 0);
	}
	/* No quest yet */
	else if (q_ptr->status == QUEST_STATUS_UNTAKEN)
	{
		q_ptr->status = QUEST_STATUS_TAKEN;

		reinit_wilderness = TRUE;

		/* Assign a new quest */
		if (q_ptr->type == QUEST_TYPE_KILL_ANY_LEVEL)
		{
			if (q_ptr->r_idx == 0)
			{
				/* Random monster at least 5 - 10 levels out of deep */
				q_ptr->r_idx = get_mon_num(q_ptr->level + 4 + randint1(6));
			}

			r_ptr = &r_info[q_ptr->r_idx];

			while ((r_ptr->flags1 & RF1_UNIQUE) || (r_ptr->rarity != 1))
			{
				q_ptr->r_idx = get_mon_num(q_ptr->level) + 4 + randint1(6);
				r_ptr = &r_info[q_ptr->r_idx];
			}

			if (q_ptr->max_num == 0)
			{
				/* Random monster number */
				if (randint1(10) > 7)
					q_ptr->max_num = 1;
				else
					q_ptr->max_num = randint1(3) + 1;
			}

			q_ptr->cur_num = 0;
			name = (r_name + r_ptr->name);
			msg_format(_("��������: %s�� %d���ݤ�", "Your quest: kill %d %s"), name,q_ptr->max_num);
		}
		else
		{
			get_questinfo(q_index, TRUE);
		}
	}
}


/*!
 * @brief Į�˴ؤ���إ�פ�ɽ������ / Display town history
 * @return �ʤ�
 */
static void town_history(void)
{
	/* Save screen */
	screen_save();

	/* Peruse the building help file */
	(void)show_file(TRUE, _("jbldg.txt", "bldg.txt"), NULL, 0, 0);

	/* Load screen */
	screen_load();
}

/*!
 * @brief �ͷ������ƥ�����ˤ����᡼�������ͽ����׻��ʥ��ʥ��ѡ��ν�����������и��͡� / critical happens at i / 10000
 * @param plus_ammo ���ƤΥ��᡼������
 * @param plus_bow �ݤΥ��᡼������
 * @return ���᡼��������
 * @note ���ܥ��᡼���̤Ƚ��̤Ϥ������̤ǤϷ׻��˲ä��ʤ���
 */
HIT_POINT calc_crit_ratio_shot(HIT_POINT plus_ammo, HIT_POINT plus_bow)
{
	HIT_POINT i;
	object_type *j_ptr =  &inventory[INVEN_BOW];
	
	/* Extract "shot" power */
	i = p_ptr->to_h_b + plus_ammo;
	
	if (p_ptr->tval_ammo == TV_BOLT)
		i = (p_ptr->skill_thb + (p_ptr->weapon_exp[0][j_ptr->sval] / 400 + i) * BTH_PLUS_ADJ);
	else
		i = (p_ptr->skill_thb + ((p_ptr->weapon_exp[0][j_ptr->sval] - (WEAPON_EXP_MASTER / 2)) / 200 + i) * BTH_PLUS_ADJ);

	/* Snipers can shot more critically with crossbows */
	if (p_ptr->concent) i += ((i * p_ptr->concent) / 5);
	if ((p_ptr->pclass == CLASS_SNIPER) && (p_ptr->tval_ammo == TV_BOLT)) i *= 2;
	
	/* Good bow makes more critical */
	i += plus_bow * 8 * (p_ptr->concent ? p_ptr->concent + 5 : 5);
	
	if (i < 0) i = 0;
	
	return i;
}

/*!
 * @brief �ͷ������ƥ�����ˤ����᡼�������ͽ����׻��ʽ��̰�¸��ʬ�� / critical happens at i / 10000
 * @param weight ���ν���
 * @param plus_ammo ���ƤΥ��᡼������
 * @param plus_bow �ݤΥ��᡼������
 * @param dam ���ܥ��᡼����
 * @return ���᡼��������
 */
HIT_POINT calc_expect_crit_shot(int weight, int plus_ammo, int plus_bow,  HIT_POINT dam)
{
	u32b num;
	int i, k, crit;
	i = calc_crit_ratio_shot(plus_ammo, plus_bow);
	
	k = 0;
	num = 0;
	
	crit = MIN(500, 900/weight);
	num += dam * 3 /2 * crit;
	k = crit;
	
	crit = MIN(500, 1350/weight);
	crit -= k;
	num += dam * 2 * crit;
	k += crit;
	
	if(k < 500)
	{
		crit = 500 - k;
		num += dam * 3 * crit;
	}
	
	num /= 500;
	
	num *= i;
	num += (10000 - i) * dam;
	num /= 10000;
	
	return num;
}

/*!
 * @brief ���������ƥ�����ˤ����᡼�������ͽ����׻��ʽ��̤��ǿ˽����� / critical happens at i / 10000
 * @param weight ���ν���
 * @param plus ���Υ��᡼������
 * @param dam ���ܥ��᡼��
 * @param meichuu ̿����
 * @param dokubari �ǿ˽������ݤ�
 * @return ���᡼��������
 */
HIT_POINT calc_expect_crit(int weight, int plus, HIT_POINT dam, s16b meichuu, bool dokubari)
{
	u32b k, num;
	int i;
	
	if(dokubari) return dam;
	
	i = (weight + (meichuu * 3 + plus * 5) + p_ptr->skill_thn);
	if (i < 0) i = 0;
	
	k = weight;
	num = 0;
	
	if (k < 400)						num += (2 * dam + 5) * (400 - k);
	if (k < 700)						num += (2 * dam + 10) * (MIN(700, k + 650) - MAX(400, k));
	if (k > (700 - 650) && k < 900)		num += (3 * dam + 15) * (MIN(900, k + 650) - MAX(700, k));
	if (k > (900 - 650) && k < 1300)		num += (3 * dam + 20) * (MIN(1300, k + 650) - MAX(900, k));
	if (k > (1300 - 650))					num += (7 * dam / 2 + 25) * MIN(650, k - (1300 - 650));
	
	num /= 650;
	if(p_ptr->pclass == CLASS_NINJA)
	{
		num *= i;
		num += (4444 - i) * dam;
		num /= 4444;
	}
	else
	{
		num *= i;
		num += (5000 - i) * dam;
		num /= 5000;
	}
	
	return num;
}

/*!
 * @brief ��������쥤�ˤ����᡼�������ͽ����׻� / critical happens at i / 10000
 * @param dam ���ܥ��᡼��
 * @param mult ���쥤��Ψ�ʳݤ�����ʬ��
 * @param div ���쥤��Ψ�ʳ�껻��ʬ��
 * @param force �������̷׻��ե饰
 * @return ���᡼��������
 */
static HIT_POINT calc_slaydam(HIT_POINT dam, int mult, int div, bool force)
{
	int tmp;
	if(force)
	{
		tmp = dam * 60;
		tmp *= mult * 3;
		tmp /= div * 2;
		tmp += dam * 60 * 2;
		tmp /= 60;
	}
	else
	{
		tmp = dam * 60;
		tmp *= mult; 
		tmp /= div;
		tmp /= 60;
	}
	return tmp;
}

/*!
 * @brief ������δ����ͷ׻��ʥ��쥤�����̥���ƥ����뢪�ڤ�̣���̡�
 * @param dam ���ܥ��᡼��
 * @param mult ���쥤��Ψ�ʳݤ�����ʬ��
 * @param div ���쥤��Ψ�ʳ�껻��ʬ��
 * @param force �������̷׻��ե饰
 * @param weight ����
 * @param plus �����᡼������
 * @param meichuu ̿����
 * @param dokubari �ǿ˽������ݤ�
 * @param vorpal_mult �ڤ�̣��Ψ�ʳݤ�����ʬ��
 * @param vorpal_div �ڤ�̣��Ψ�ʳ�껻��ʬ��
 * @return ���᡼��������
 */
static u32b calc_expect_dice(u32b dam, int mult, int div, bool force, int weight, int plus, s16b meichuu, bool dokubari, int vorpal_mult, int vorpal_div)
{
	dam = calc_slaydam(dam, mult, div, force);
	dam = calc_expect_crit(weight, plus, dam, meichuu, dokubari);
	dam = calc_slaydam(dam, vorpal_mult, vorpal_div, FALSE);
	return dam;
}


/*!
 * @brief ���γƾ����Υ��᡼�������ͤ�ɽ�����롣
 * @param r ɽ����
 * @param c ɽ����
 * @param mindice ��������ʬ�Ǿ���
 * @param maxdice ��������ʬ������
 * @param blows ������
 * @param dam_bonus ���᡼��������
 * @param attr �������
 * @param color ������Ƥ�ɽ����
 * @details
 * Display the damage figure of an object\n
 * (used by compare_weapon_aux)\n
 * \n
 * Only accurate for the current weapon, because it includes\n
 * the current +dam of the player.\n
 * @return �ʤ�
 */
static void show_weapon_dmg(int r, int c, int mindice, int maxdice, int blows, int dam_bonus, cptr attr, byte color)
{
	char tmp_str[80];
	int mindam, maxdam;
	
	mindam = blows * (mindice + dam_bonus);
	maxdam = blows * (maxdice + dam_bonus);

	/* Print the intro text */
	c_put_str(color, attr, r, c);

	/* Calculate the min and max damage figures */
	sprintf(tmp_str, _("��������: %d-%d ���᡼��", "Attack: %d-%d damage"), mindam, maxdam);
	
	/* Print the damage */
	put_str(tmp_str, r, c + 8);
}


/*!
 * @brief �������Υ��᡼�������ɽ�����롣
 * @param o_ptr ���֥������Ȥι�¤�Τλ��ȥݥ��󥿡�
 * @param col ɽ������Ԥξ�ü
 * @param r ɽ��������κ�ü
 * @details
 * Show the damage figures for the various monster types\n
 * \n
 * Only accurate for the current weapon, because it includes\n
 * the current number of blows for the player.\n
 * @return �ʤ�
 */
static void compare_weapon_aux(object_type *o_ptr, int col, int r)
{
	u32b flgs[TR_FLAG_SIZE];
	int blow = p_ptr->num_blow[0];
	bool force = FALSE;
	bool dokubari = FALSE;
	
	/* Effective dices */
	int eff_dd = o_ptr->dd + p_ptr->to_dd[0];
	int eff_ds = o_ptr->ds + p_ptr->to_ds[0];
	
	int mindice = eff_dd;
	int maxdice = eff_ds * eff_dd;
	int mindam = 0;
	int maxdam = 0;
	int vorpal_mult = 1;
	int vorpal_div = 1;
	int dmg_bonus = o_ptr->to_d + p_ptr->to_d[0];
	

	/* Get the flags of the weapon */
	object_flags(o_ptr, flgs);
	
	if((o_ptr->tval == TV_SWORD) && (o_ptr->sval == SV_DOKUBARI)) dokubari = TRUE;
	
	
	/* Show Critical Damage*/
	mindam = calc_expect_crit(o_ptr->weight, o_ptr->to_h, mindice, p_ptr->to_h[0], dokubari);
	maxdam = calc_expect_crit(o_ptr->weight, o_ptr->to_h, maxdice, p_ptr->to_h[0], dokubari);
	
	show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus, _("��:", "Critical:"), TERM_L_RED);

	
	/* Vorpal Hit*/
	if ((have_flag(flgs, TR_VORPAL) || hex_spelling(HEX_RUNESWORD)))
	{
		if((o_ptr->name1 == ART_VORPAL_BLADE) || (o_ptr->name1 == ART_CHAINSWORD))
		{
			vorpal_mult = 5;
			vorpal_div = 3;
		}
		else
		{
			vorpal_mult = 11;
			vorpal_div = 9;
		}
		
		mindam = calc_expect_dice(mindice, 1, 1, FALSE, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 1, 1, FALSE, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);		
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus, _("�ڤ�̣:", "Vorpal:") , TERM_L_RED);
	}	
	
	if ((p_ptr->pclass != CLASS_SAMURAI) && have_flag(flgs, TR_FORCE_WEAPON) && (p_ptr->csp > (o_ptr->dd * o_ptr->ds / 5)))
	{
		force = TRUE;
		
		mindam = calc_expect_dice(mindice, 1, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 1, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus, _("����:", "Force  :"), TERM_L_BLUE);
	}
		
	/* Print the relevant lines */
	if (have_flag(flgs, TR_KILL_ANIMAL))
	{
		mindam = calc_expect_dice(mindice, 4, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 4, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);		
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("ưʪ:", "Animals:"), TERM_YELLOW);
	}
	else if (have_flag(flgs, TR_SLAY_ANIMAL)) 
	{
		mindam = calc_expect_dice(mindice, 5, 2, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 5, 2, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("ưʪ:", "Animals:"), TERM_YELLOW);
	}
	if (have_flag(flgs, TR_KILL_EVIL))
	{	
		mindam = calc_expect_dice(mindice, 7, 2, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 7, 2, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("�ٰ�:", "Evil:"), TERM_YELLOW);
	}
	else if (have_flag(flgs, TR_SLAY_EVIL))
	{	
		mindam = calc_expect_dice(mindice, 2, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 2, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);		
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("�ٰ�:", "Evil:"), TERM_YELLOW);
	}
	if (have_flag(flgs, TR_KILL_HUMAN))
	{	
		mindam = calc_expect_dice(mindice, 4, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 4, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("�ʹ�:", "Human:"), TERM_YELLOW);
	}
	else if (have_flag(flgs, TR_SLAY_HUMAN))
	{	
		mindam = calc_expect_dice(mindice, 5, 2, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 5, 2, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("�ʹ�:", "Human:"), TERM_YELLOW);
	}
	if (have_flag(flgs, TR_KILL_UNDEAD))
	{
		mindam = calc_expect_dice(mindice, 5, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 5, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("�Ի�:", "Undead:"), TERM_YELLOW);
	}
	else if (have_flag(flgs, TR_SLAY_UNDEAD)) 
	{
		mindam = calc_expect_dice(mindice, 3, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 3, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("�Ի�:", "Undead:"), TERM_YELLOW);
	}
	if (have_flag(flgs, TR_KILL_DEMON))
	{	
		mindam = calc_expect_dice(mindice, 5, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 5, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("����:", "Demons:") , TERM_YELLOW);
	}
	else if (have_flag(flgs, TR_SLAY_DEMON))
	{	
		mindam = calc_expect_dice(mindice, 3, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 3, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus, _("����:", "Demons:") , TERM_YELLOW);
	}
	if (have_flag(flgs, TR_KILL_ORC))
	{
		mindam = calc_expect_dice(mindice, 5, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 5, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("������:", "Orcs:"), TERM_YELLOW);
	}
	else if (have_flag(flgs, TR_SLAY_ORC))
	{
		mindam = calc_expect_dice(mindice, 3, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 3, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("������:", "Orcs:"), TERM_YELLOW);
	}
	if (have_flag(flgs, TR_KILL_TROLL))
	{
		mindam = calc_expect_dice(mindice, 5, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 5, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("�ȥ��:", "Trolls:") , TERM_YELLOW);
	}
	else if (have_flag(flgs, TR_SLAY_TROLL))
	{
		mindam = calc_expect_dice(mindice, 3, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 3, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,   _("�ȥ��:", "Trolls:") , TERM_YELLOW);
	}
	if (have_flag(flgs, TR_KILL_GIANT))
	{
		mindam = calc_expect_dice(mindice, 5, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 5, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("���:", "Giants:"), TERM_YELLOW);
	}
	else if (have_flag(flgs, TR_SLAY_GIANT))
	{
		mindam = calc_expect_dice(mindice, 3, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 3, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("���:", "Giants:"), TERM_YELLOW);
	}
	if (have_flag(flgs, TR_KILL_DRAGON))
	{
		mindam = calc_expect_dice(mindice, 5, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 5, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("ε:", "Dragons:"), TERM_YELLOW);
	}
	else if (have_flag(flgs, TR_SLAY_DRAGON))
	{		
		mindam = calc_expect_dice(mindice, 3, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 3, 1, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,   _("ε:", "Dragons:"), TERM_YELLOW);
	}
	if (have_flag(flgs, TR_BRAND_ACID))
	{
		mindam = calc_expect_dice(mindice, 5, 2, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 5, 2, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("��°��:", "Acid:"), TERM_RED);
	}
	if (have_flag(flgs, TR_BRAND_ELEC))
	{
		mindam = calc_expect_dice(mindice, 5, 2, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 5, 2, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("��°��:", "Elec:"), TERM_RED);
	}
	if (have_flag(flgs, TR_BRAND_FIRE))
	{
		mindam = calc_expect_dice(mindice, 5, 2, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 5, 2, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("��°��:", "Fire:"), TERM_RED);
	}
	if (have_flag(flgs, TR_BRAND_COLD))
	{
		mindam = calc_expect_dice(mindice, 5, 2, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 5, 2, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus,  _("��°��:", "Cold:"), TERM_RED);
	}
	if (have_flag(flgs, TR_BRAND_POIS))
	{
		mindam = calc_expect_dice(mindice, 5, 2, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		maxdam = calc_expect_dice(maxdice, 5, 2, force, o_ptr->weight, o_ptr->to_h, p_ptr->to_h[0], dokubari, vorpal_mult, vorpal_div);
		show_weapon_dmg(r++, col, mindam, maxdam, blow, dmg_bonus, _("��°��:", "Poison:"), TERM_RED);
	}
}

/*!
 * @brief ��󥹥����ؤ�̿��Ψ�η׻�
 * @param to_h ̿����
 * @param ac ŨAC
 * @return �ʤ�
 */
static int hit_chance(int to_h, int ac)
{
	int chance = 0;
	int meichuu = p_ptr->skill_thn + (p_ptr->to_h[0] + to_h) * BTH_PLUS_ADJ;

	if (meichuu <= 0) return 5;

	chance = 100 - ((ac * 75) / meichuu);

	if (chance > 95) chance = 95;
	if (chance < 5) chance = 5;
	if (p_ptr->pseikaku == SEIKAKU_NAMAKE)
		chance = (chance * 19 + 9) / 20;
	return chance;
}

/*!
 * @brief ��ﾢ�ˤ������������δ��������ɽ�����롣
 * @param o_ptr ���֥������Ȥι�¤�Τλ��ȥݥ��󥿡�
 * @param row ɽ��������κ�ü
 * @param col ɽ������Ԥξ�ü
 * @details
 * Displays all info about a weapon
 *
 * Only accurate for the current weapon, because it includes
 * various info about the player's +to_dam and number of blows.
 * @return �ʤ�
 */
static void list_weapon(object_type *o_ptr, int row, int col)
{
	char o_name[MAX_NLEN];
	char tmp_str[80];

	/* Effective dices */
	int eff_dd = o_ptr->dd + p_ptr->to_dd[0];
	int eff_ds = o_ptr->ds + p_ptr->to_ds[0];

	/* Print the weapon name */
	object_desc(o_name, o_ptr, OD_NAME_ONLY);
	c_put_str(TERM_YELLOW, o_name, row, col);

	/* Print the player's number of blows */
	sprintf(tmp_str, _("������: %d", "Number of Blows: %d"), p_ptr->num_blow[0]);
	put_str(tmp_str, row+1, col);

	/* Print to_hit and to_dam of the weapon */
	sprintf(tmp_str, _("̿��Ψ:  0  50 100 150 200 (Ũ��AC)", "To Hit:  0  50 100 150 200 (AC)"));
	put_str(tmp_str, row+2, col);

	/* Print the weapons base damage dice */
	sprintf(tmp_str, "        %2d  %2d  %2d  %2d  %2d (%%)",
				hit_chance(o_ptr->to_h, 0), hit_chance(o_ptr->to_h, 50), hit_chance(o_ptr->to_h, 100),
				hit_chance(o_ptr->to_h, 150), hit_chance(o_ptr->to_h, 200));
	put_str(tmp_str, row+3, col);
	c_put_str(TERM_YELLOW, _("��ǽ�ʥ��᡼��:", "Possible Damage:"), row+5, col);

	/* Damage for one blow (if it hits) */
	sprintf(tmp_str, _("������ˤĤ� %d-%d", "One Strike: %d-%d damage"),
	    (int)(eff_dd + o_ptr->to_d + p_ptr->to_d[0]),
		(int)(eff_ds * eff_dd + o_ptr->to_d + p_ptr->to_d[0]));
	put_str(tmp_str, row+6, col+1);

	/* Damage for the complete attack (if all blows hit) */
	sprintf(tmp_str, _("��������ˤĤ� %d-%d", "One Attack: %d-%d damage"),
		(int)(p_ptr->num_blow[0] * (eff_dd + o_ptr->to_d + p_ptr->to_d[0])),
			(int)(p_ptr->num_blow[0] * (eff_ds * eff_dd + o_ptr->to_d + p_ptr->to_d[0])));
	put_str(tmp_str, row+7, col+1);
}


/*!
 * @brief ��ﾢ�Ρ����״����оݤˤʤ뤫��Ƚ�ꤹ�롣/ Hook to specify "weapon"
 * @param o_ptr ���֥������Ȥι�¤�Τλ��ȥݥ��󥿡�
 * @return �оݤˤʤ�ʤ�TRUE���֤���
 */
static bool item_tester_hook_melee_weapon(object_type *o_ptr)
{
	switch (o_ptr->tval)
	{
		case TV_HAFTED:
		case TV_POLEARM:
		case TV_DIGGING:
		{
			return (TRUE);
		}
		case TV_SWORD:
		{
			if (o_ptr->sval != SV_DOKUBARI) return (TRUE);
		}
	}

	return (FALSE);
}


/*!
 * @brief ��ﾢ�Ρ����ơ״����оݤˤʤ뤫��Ƚ�ꤹ�롣/ Hook to specify "weapon"
 * @param o_ptr ���֥������Ȥι�¤�Τλ��ȥݥ��󥿡�
 * @return �оݤˤʤ�ʤ�TRUE���֤���
 */
static bool item_tester_hook_ammo(object_type *o_ptr)
{
	switch (o_ptr->tval)
	{
		case TV_SHOT:
		case TV_ARROW:
		case TV_BOLT:
		{
			return (TRUE);
		}
	}

	return (FALSE);
}


/*!
 * @brief ��ﾢ���꣱��ʬ�ʥ��֥������ȣ���ˤν�����/ Compare weapons
 * @details 
 * Copies the weapons to compare into the weapon-slot and\n
 * compares the values for both weapons.\n
 * ���Ĥ�������Ӥ򤷤ʤ��ʤ����Ѥ�Ⱦ�ۤˤʤ롣
 * @param bcost ���ܴ�������
 * @return �ǽ�Ū�ˤ����ä�����
 */
static int compare_weapons(int bcost)
{
	int i, n;
	OBJECT_IDX item, item2;
	object_type *o_ptr[2];
	object_type orig_weapon;
	object_type *i_ptr;
	cptr q, s;
	int row = 2;
	int wid = 38, mgn = 2;
	bool old_character_xtra = character_xtra;
	char ch;
	int total = 0;
	int cost = 0; /* First time no price */

	/* Save the screen */
	screen_save();

	/* Clear the screen */
	clear_bldg(0, 22);

	/* Store copy of original wielded weapon */
	i_ptr = &inventory[INVEN_RARM];
	object_copy(&orig_weapon, i_ptr);

	/* Only compare melee weapons */
	item_tester_no_ryoute = TRUE;
	item_tester_hook = item_tester_hook_melee_weapon;

	/* Get the first weapon */
	q = _("�������ϡ�", "What is your first weapon? ");
	s = _("��٤��Τ�����ޤ���", "You have nothing to compare.");

	if (!get_item(&item, q, s, (USE_EQUIP | USE_INVEN)))
	{
		screen_load();
		return (0);
	}

	/* Get the item (in the pack) */
	o_ptr[0] = &inventory[item];
	n = 1;
	total = bcost;

	while (TRUE)
	{
		/* Clear the screen */
		clear_bldg(0, 22);

		/* Only compare melee weapons */
		item_tester_no_ryoute = TRUE;
		item_tester_hook = item_tester_hook_melee_weapon;

		/* Hack -- prevent "icky" message */
		character_xtra = TRUE;

		/* Diaplay selected weapon's infomation */
		for (i = 0; i < n; i++)
		{
			int col = (wid * i + mgn);

			/* Copy i-th weapon into the weapon slot (if it's not already there) */
			if (o_ptr[i] != i_ptr) object_copy(i_ptr, o_ptr[i]);

			/* Get the new values */
			calc_bonuses();

			/* List the new values */
			list_weapon(o_ptr[i], row, col);
			compare_weapon_aux(o_ptr[i], col, row + 8);

			/* Copy back the original weapon into the weapon slot */
			object_copy(i_ptr, &orig_weapon);
		}

		/* Reset the values for the old weapon */
		calc_bonuses();

		character_xtra = old_character_xtra;

#ifdef JP
		put_str(format("[ ����о�: 's'���ѹ� ($%d) ]", cost), 1, (wid + mgn));
		put_str("(���ֹ⤤���᡼����Ŭ�Ѥ���ޤ���ʣ�������Ǹ��̤�­��������ޤ���)", row + 4, 0);
		prt("���ߤε��̤���Ƚ�Ǥ���ȡ����ʤ������ϰʲ��Τ褦�ʰ��Ϥ�ȯ�����ޤ�:", 0, 0);
#else
		put_str(format("[ 's' Select secondary weapon($%d) ]", cost), row + 1, (wid * i + mgn));
		put_str("(Only highest damage applies per monster. Special damage not cumulative.)", row + 4, 0);
		prt("Based on your current abilities, here is what your weapons will do", 0, 0);
#endif

		flush();
		ch = inkey();

		if (ch == 's')
		{
			if (total + cost > p_ptr->au)
			{
				msg_print(_("���⤬­��ޤ���", "You don't have enough money!"));
				msg_print(NULL);
				continue;
			}

			q = _("��������ϡ�", "What is your second weapon? ");
			s = _("��٤��Τ�����ޤ���", "You have nothing to compare.");

			/* Get the second weapon */
			if (!get_item(&item2, q, s, (USE_EQUIP | USE_INVEN))) continue;

			total += cost;
			cost = bcost / 2;

			/* Get the item (in the pack) */
			o_ptr[1] = &inventory[item2];
			n = 2;
		}
		else
		{
			break;
		}
	}

	/* Restore the screen */
	screen_load();

	/* Done */
	return (total);
}


/*!
 * @brief AC�������Ψ�����᡼������Ψ��׻���ɽ�����롣 / Evaluate AC
 * @details 
 * Calculate and display the dodge-rate and the protection-rate
 * based on AC
 * @param iAC �ץ쥤�䡼��AC��
 * @return ���TRUE���֤���
 */
static bool eval_ac(int iAC)
{
#ifdef JP
	const char memo[] =
		"���᡼���ڸ�Ψ�Ȥϡ�Ũ�ι��⤬�����ä������Υ��᡼����\n"
		"���ѡ�����ȷڸ����뤫�򼨤��ޤ���\n"
		"���᡼���ڸ����̾��ľ�ܹ���(���ब�ֹ��⤹��פȡ�ʴ�դ���פ�ʪ)\n"
		"���Ф��ƤΤ߸��̤�����ޤ���\n \n"
		"Ũ�Υ�٥�Ȥϡ�����Ũ���̾ﲿ���˸���뤫�򼨤��ޤ���\n \n"
		"����Ψ��Ũ��ľ�ܹ���򲿥ѡ�����Ȥγ�Ψ���򤱤뤫�򼨤���\n"
		"Ũ�Υ�٥�Ȥ��ʤ���AC�ˤ�äƷ��ꤵ��ޤ���\n \n"
		"���᡼�������ͤȤϡ�Ũ�Σ������ݥ���Ȥ��̾ﹶ����Ф���\n"
		"����Ψ�ȥ��᡼���ڸ�Ψ���θ�������᡼���δ����ͤ򼨤��ޤ���\n";
#else
	const char memo[] =
		"'Protection Rate' means how much damage is reduced by your armor.\n"
		"Note that the Protection rate is effective only against normal "
		"'attack' and 'shatter' type melee attacks, "
		"and has no effect against any other types such as 'poison'.\n \n"
		"'Dodge Rate' indicates the success rate on dodging the "
		"monster's melee attacks.  "
		"It is depend on the level of the monster and your AC.\n \n"
		"'Average Damage' indicates the expected amount of damage "
		"when you are attacked by normal melee attacks with power=100.";
#endif

	int protection;
	int col, row = 2;
	int lvl;
	char buf[80*20], *t;

	/* AC lower than zero has no effect */
	if (iAC < 0) iAC = 0;

	/* ���᡼���ڸ�Ψ��׻� */
	protection = 100 * MIN(iAC, 150) / 250;

	screen_save();
	clear_bldg(0, 22);

#ifdef JP
	put_str(format("���ʤ��θ��ߤ�AC: %3d", iAC), row++, 0);
	put_str(format("���᡼���ڸ�Ψ  : %3d%%", protection), row++, 0);
	row++;

	put_str("Ũ�Υ�٥�      :", row + 0, 0);
	put_str("����Ψ          :", row + 1, 0);
	put_str("���᡼��������  :", row + 2, 0);
#else
	put_str(format("Your current AC : %3d", iAC), row++, 0);
	put_str(format("Protection rate : %3d%%", protection), row++, 0);
	row++;

	put_str("Level of Monster:", row + 0, 0);
	put_str("Dodge Rate      :", row + 1, 0);
	put_str("Average Damage  :", row + 2, 0);
#endif
    
	for (col = 17 + 1, lvl = 0; lvl <= 100; lvl += 10, col += 5)
	{
		int quality = 60 + lvl * 3; /* attack quality with power 60 */
		int dodge;   /* ����Ψ(%) */
		int average; /* ���᡼�������� */

		put_str(format("%3d", lvl), row + 0, col);

		/* ����Ψ��׻� */
		dodge = 5 + (MIN(100, 100 * (iAC * 3 / 4) / quality) * 9 + 5) / 10;
		put_str(format("%3d%%", dodge), row + 1, col);

		/* 100���ι�����Ф��ƤΥ��᡼�������ͤ�׻� */
		average = (100 - dodge) * (100 - protection) / 100;
		put_str(format("%3d", average), row + 2, col);
	}

	/* Display note */
	roff_to_buf(memo, 70, buf, sizeof(buf));
	for (t = buf; t[0]; t += strlen(t) + 1)
		put_str(t, (row++) + 4, 4);

#ifdef JP
	prt("���ߤΤ��ʤ����������餹��ȡ����ʤ����ɸ��Ϥ�"
		   "���줯�餤�Ǥ�:", 0, 0);
#else
	prt("Defense abilities from your current Armor Class are evaluated below.", 0, 0);
#endif
  
	flush();
	(void)inkey();
	screen_load();

	/* Done */
	return (TRUE);
}


/*!
 * @brief �����оݤȤʤ���줿��狼��Ƚ�ꤹ�롣 / Hook to specify "broken weapon"
 * @param o_ptr ���֥������Ȥι�¤�Τλ��ȥݥ��󥿡�
 * @return �����оݤˤʤ�ʤ�TRUE���֤���
 */
static bool item_tester_hook_broken_weapon(object_type *o_ptr)
{
 	if (o_ptr->tval != TV_SWORD) return FALSE;

	switch (o_ptr->sval)
	{
	case SV_BROKEN_DAGGER:
	case SV_BROKEN_SWORD:
		return TRUE;
	}

	return FALSE;
}

/*!
 * @brief ���������Υ��֥������Ȥ��齤���оݤ�������ܿ����롣
 * @param to_ptr �����оݥ��֥������Ȥι�¤�Τλ��ȥݥ��󥿡�
 * @param from_ptr �����������֥������Ȥι�¤�Τλ��ȥݥ��󥿡�
 * @return �����оݤˤʤ�ʤ�TRUE���֤���
 */
static void give_one_ability_of_object(object_type *to_ptr, object_type *from_ptr)
{
	int i, n = 0;
	int cand[TR_FLAG_MAX];
	u32b to_flgs[TR_FLAG_SIZE];
	u32b from_flgs[TR_FLAG_SIZE];

	object_flags(to_ptr, to_flgs);
	object_flags(from_ptr, from_flgs);

	for (i = 0; i < TR_FLAG_MAX; i++)
	{
		switch (i)
		{
		case TR_IGNORE_ACID:
		case TR_IGNORE_ELEC:
		case TR_IGNORE_FIRE:
		case TR_IGNORE_COLD:
		case TR_ACTIVATE:
		case TR_RIDING:
		case TR_THROW:
		case TR_SHOW_MODS:
		case TR_HIDE_TYPE:
		case TR_ES_ATTACK:
		case TR_ES_AC:
		case TR_FULL_NAME:
		case TR_FIXED_FLAVOR:
			break;
		default:
			if (have_flag(from_flgs, i) && !have_flag(to_flgs, i))
			{
				if (!(is_pval_flag(i) && (from_ptr->pval < 1))) cand[n++] = i;
			}
		}
	}

	if (n > 0)
	{
		int bmax;
		int tr_idx = cand[randint0(n)];
		add_flag(to_ptr->art_flags, tr_idx);
		if (is_pval_flag(tr_idx)) to_ptr->pval = MAX(to_ptr->pval, 1);
		bmax = MIN(3, MAX(1, 40 / (to_ptr->dd * to_ptr->ds)));
		if (tr_idx == TR_BLOWS) to_ptr->pval = MIN(to_ptr->pval, bmax);
		if (tr_idx == TR_SPEED) to_ptr->pval = MIN(to_ptr->pval, 4);
	}

	return;
}

/*!
 * @brief �����ƥཤ�������Υᥤ��롼���� / Repair broken weapon
 * @param bcost ���ܴ�������
 * @return �ºݤˤ����ä�����
 */
static int repair_broken_weapon_aux(int bcost)
{
	s32b cost;
	OBJECT_IDX item, mater;
	object_type *o_ptr, *mo_ptr; /* broken weapon and material weapon */
	object_kind *k_ptr;
	int i, dd_bonus, ds_bonus;
	IDX k_idx;
	char basenm[MAX_NLEN];
	cptr q, s; /* For get_item prompt */
	int row = 7;

	/* Clear screen */
	clear_bldg(0, 22);

	/* Notice */
	prt(_("�����ˤϺ����Ȥʤ�⤦1�Ĥ���郎ɬ�פǤ���", "Hand one material weapon to repair a broken weapon."), row, 2);
	prt(_("�����˻��Ѥ������Ϥʤ��ʤ�ޤ���", "The material weapon will disappear after repairing!!"), row+1, 2);

	/* Get an item */
	q = _("�ɤ��ޤ줿���������ޤ�����", "Repair which broken weapon? ");
	s = _("�����Ǥ����ޤ줿��郎����ޤ���", "You have no broken weapon to repair.");

	/* Only forge broken weapons */
	item_tester_hook = item_tester_hook_broken_weapon;

	if (!get_item(&item, q, s, (USE_INVEN | USE_EQUIP))) return (0);

	/* Get the item (in the pack) */
	o_ptr = &inventory[item];

	/* It is worthless */
	if (!object_is_ego(o_ptr) && !object_is_artifact(o_ptr))
	{
		msg_format(_("�����ľ���Ƥ⤷�礦���ʤ�����", "It is worthless to repair."));
		return (0);
	}

	/* They are too many */
	if (o_ptr->number > 1)
	{
		msg_format(_("���٤�ʣ���������뤳�ȤϤǤ��ޤ���", "They are too many to repair at once!"));
		return (0);
	}

	/* Display item name */
	object_desc(basenm, o_ptr, OD_NAME_ONLY);
	prt(format(_("������������ %s", "Repairing: %s"), basenm), row+3, 2);

	/* Get an item */
	q = _("�����Ȥʤ����ϡ�", "Which weapon for material? ");
	s = _("�����Ȥʤ���郎����ޤ���", "You have no material to repair.");

	/* Only forge broken weapons */
	item_tester_hook = item_tester_hook_melee_weapon;

	if (!get_item(&mater, q, s, (USE_INVEN | USE_EQUIP))) return (0);
	if (mater == item)
	{
		msg_print(_("���饤����ۤ���ʤ���", "This is not a klein bottle!"));
		return (0);
	}

	/* Get the item (in the pack) */
	mo_ptr = &inventory[mater];

	/* Display item name */
	object_desc(basenm, mo_ptr, OD_NAME_ONLY);
	prt(format(_("�����Ȥ����� %s", "Material : %s"), basenm), row+4, 2);

	/* Get the value of one of the items (except curses) */
	cost = bcost + object_value_real(o_ptr) * 2;

	if (!get_check(format(_("��%d������ޤ���������Ǥ����� ", "Costs %d gold, okay? "), cost))) return (0);

	/* Check if the player has enough money */
	if (p_ptr->au < cost)
	{
		object_desc(basenm, o_ptr, OD_NAME_ONLY);
		msg_format(_("%s������������Υ�����ɤ�����ޤ���",
			"You do not have the gold to repair %s!"), basenm);
		msg_print(NULL);
		return (0);
	}

	if (o_ptr->sval == SV_BROKEN_DAGGER)
	{
		IDX j;
		int n = 1;

		/* Suppress compiler warning */
		k_idx = 0;

		for (j = 1; j < max_k_idx; j++)
		{
			object_kind *k_aux_ptr = &k_info[j];

			if (k_aux_ptr->tval != TV_SWORD) continue;
			if ((k_aux_ptr->sval == SV_BROKEN_DAGGER) ||
				(k_aux_ptr->sval == SV_BROKEN_SWORD) ||
				(k_aux_ptr->sval == SV_DOKUBARI)) continue;
			if (k_aux_ptr->weight > 99) continue;

			if (one_in_(n)) 
			{
				k_idx = j;
				n++;
			}
		}
	}
	else /* TV_BROKEN_SWORD */
	{
		/* Repair to a sword or sometimes material's type weapon */
		OBJECT_TYPE_VALUE tval = (one_in_(5) ? mo_ptr->tval : TV_SWORD);

		while(1)
		{
			object_kind *ck_ptr;

			k_idx = lookup_kind(tval, SV_ANY);
			ck_ptr = &k_info[k_idx];

			if (tval == TV_SWORD)
			{
				if ((ck_ptr->sval == SV_BROKEN_DAGGER) ||
					(ck_ptr->sval == SV_BROKEN_SWORD) ||
					(ck_ptr->sval == SV_DIAMOND_EDGE) ||
					(ck_ptr->sval == SV_DOKUBARI)) continue;
			}
			if (tval == TV_POLEARM)
			{
				if ((ck_ptr->sval == SV_DEATH_SCYTHE) ||
					(ck_ptr->sval == SV_TSURIZAO)) continue;
			}
			if (tval == TV_HAFTED)
			{
				if ((ck_ptr->sval == SV_GROND) ||
					(ck_ptr->sval == SV_WIZSTAFF) ||
					(ck_ptr->sval == SV_NAMAKE_HAMMER)) continue;
			}

			break;
		}
	}

	/* Calculate dice bonuses */
	dd_bonus = o_ptr->dd - k_info[o_ptr->k_idx].dd;
	ds_bonus = o_ptr->ds - k_info[o_ptr->k_idx].ds;
	dd_bonus += mo_ptr->dd - k_info[mo_ptr->k_idx].dd;
	ds_bonus += mo_ptr->ds - k_info[mo_ptr->k_idx].ds;

	/* Change base object */
	k_ptr = &k_info[k_idx];
	o_ptr->k_idx = k_idx;
	o_ptr->weight = k_ptr->weight;
	o_ptr->tval = k_ptr->tval;
	o_ptr->sval = k_ptr->sval;
	o_ptr->dd = k_ptr->dd;
	o_ptr->ds = k_ptr->ds;

	/* Copy base object's ability */
	for (i = 0; i < TR_FLAG_SIZE; i++) o_ptr->art_flags[i] |= k_ptr->flags[i];
	if (k_ptr->pval) o_ptr->pval = MAX(o_ptr->pval, randint1(k_ptr->pval));
	if (have_flag(k_ptr->flags, TR_ACTIVATE)) o_ptr->xtra2 = (byte_hack)k_ptr->act_idx;

	/* Dice up */
	if (dd_bonus > 0)
	{
		o_ptr->dd++;
		for (i = 1; i < dd_bonus; i++)
		{
			if (one_in_(o_ptr->dd + i)) o_ptr->dd++;
		}
	}
	if (ds_bonus > 0)
	{
		o_ptr->ds++;
		for (i = 1; i < ds_bonus; i++)
		{
			if (one_in_(o_ptr->ds + i)) o_ptr->ds++;
		}
	}

	/* */
	if (have_flag(k_ptr->flags, TR_BLOWS))
	{
		int bmax = MIN(3, MAX(1, 40 / (o_ptr->dd * o_ptr->ds)));
		o_ptr->pval = MIN(o_ptr->pval, bmax);
	}

	/* Add one random ability from material weapon */
	give_one_ability_of_object(o_ptr, mo_ptr);

	/* Add to-dam, to-hit and to-ac from material weapon */
	o_ptr->to_d += MAX(0, (mo_ptr->to_d / 3));
	o_ptr->to_h += MAX(0, (mo_ptr->to_h / 3));
	o_ptr->to_a += MAX(0, (mo_ptr->to_a));

	if ((o_ptr->name1 == ART_NARSIL) ||
		(object_is_random_artifact(o_ptr) && one_in_(1)) ||
		(object_is_ego(o_ptr) && one_in_(7)))
	{
		/* Forge it */
		if (object_is_ego(o_ptr))
		{
			add_flag(o_ptr->art_flags, TR_IGNORE_FIRE);
			add_flag(o_ptr->art_flags, TR_IGNORE_ACID);
		}

		/* Add one random ability from material weapon */
		give_one_ability_of_object(o_ptr, mo_ptr);

		/* Add one random activation */
		if (!activation_index(o_ptr)) one_activation(o_ptr);

		/* Narsil */
		if (o_ptr->name1 == ART_NARSIL)
		{
			one_high_resistance(o_ptr);
			one_ability(o_ptr);
		}

		msg_print(_("����Ϥ��ʤ�ζ�ʪ���ä��褦����", "This blade seems to be exceptionally."));
	}

	object_desc(basenm, o_ptr, OD_NAME_ONLY);
#ifdef JP
	msg_format("��%d��%s�˽������ޤ�����", cost, basenm);
#else
	msg_format("Repaired into %s for %d gold.", basenm, cost);
#endif
	msg_print(NULL);

	/* Remove BROKEN flag */
	o_ptr->ident &= ~(IDENT_BROKEN);

	/* Add repaired flag */
	o_ptr->discount = 99;

	/* Decrease material object */
	inven_item_increase(mater, -1);
	inven_item_optimize(mater);

	/* Copyback */
	p_ptr->update |= PU_BONUS;
	handle_stuff();

	/* Something happened */
	return (cost);
}

/*!
 * @brief �����ƥཤ�������β��ϥ롼���� / Repair broken weapon
 * @param bcost ���ܴ�������
 * @return �ºݤˤ����ä�����
 */
static int repair_broken_weapon(int bcost)
{
	int cost;

	screen_save();
	cost = repair_broken_weapon_aux(bcost);
	screen_load();
	return cost;
}


/*!
 * @brief �����ƥ�ζ�����Ԥ��� / Enchant item
 * @param cost �����������
 * @param to_hit ̿��򥢥åפ�������
 * @param to_dam ���᡼���򥢥åפ�������
 * @param to_ac ���ä򥢥åפ�������
 * @return �ºݤˤ����ä�����
 */
static bool enchant_item(int cost, int to_hit, int to_dam, int to_ac)
{
	int         i;
	OBJECT_IDX  item;
	bool        okay = FALSE;
	object_type *o_ptr;
	cptr        q, s;
	int         maxenchant = (p_ptr->lev / 5);
	char        tmp_str[MAX_NLEN];

	clear_bldg(4, 18);
#ifdef JP
	prt(format("���ߤΤ��ʤ��ε��̤��ȡ�+%d �ޤǲ��ɤǤ��ޤ���", maxenchant), 5, 0);
	prt(format(" ���ɤ�����ϰ�ĤˤĤ���%d �Ǥ���", cost), 7, 0);
#else
	prt(format("  Based on your skill, we can improve up to +%d.", maxenchant), 5, 0);
	prt(format("  The price for the service is %d gold per item.", cost), 7, 0);
#endif

	item_tester_no_ryoute = TRUE;

	/* Get an item */
	q = _("�ɤΥ����ƥ����ɤ��ޤ�����", "Improve which item? ");
	s = _("���ɤǤ����Τ�����ޤ���", "You have nothing to improve.");

	if (!get_item(&item, q, s, (USE_INVEN | USE_EQUIP))) return (FALSE);

	/* Get the item (in the pack) */
	o_ptr = &inventory[item];

	/* Check if the player has enough money */
	if (p_ptr->au < (cost * o_ptr->number))
	{
		object_desc(tmp_str, o_ptr, OD_NAME_ONLY);
		msg_format(_("%s����ɤ�������Υ�����ɤ�����ޤ���", "You do not have the gold to improve %s!"), tmp_str);
		return (FALSE);
	}

	/* Enchant to hit */
	for (i = 0; i < to_hit; i++)
	{
		if (o_ptr->to_h < maxenchant)
		{
			if (enchant(o_ptr, 1, (ENCH_TOHIT | ENCH_FORCE)))
			{
				okay = TRUE;
				break;
			}
		}
	}

	/* Enchant to damage */
	for (i = 0; i < to_dam; i++)
	{
		if (o_ptr->to_d < maxenchant)
		{
			if (enchant(o_ptr, 1, (ENCH_TODAM | ENCH_FORCE)))
			{
				okay = TRUE;
				break;
			}
		}
	}

	/* Enchant to AC */
	for (i = 0; i < to_ac; i++)
	{
		if (o_ptr->to_a < maxenchant)
		{
			if (enchant(o_ptr, 1, (ENCH_TOAC | ENCH_FORCE)))
			{
				okay = TRUE;
				break;
			}
		}
	}

	/* Failure */
	if (!okay)
	{
		/* Flush */
		if (flush_failure) flush();

		/* Message */
		msg_print(_("���ɤ˼��Ԥ�����", "The improvement failed."));

		return (FALSE);
	}
	else
	{
		object_desc(tmp_str, o_ptr, OD_NAME_AND_ENCHANT);
#ifdef JP
		msg_format("��%d��%s�˲��ɤ��ޤ�����", cost * o_ptr->number, tmp_str);
#else
		msg_format("Improved into %s for %d gold.", tmp_str, cost * o_ptr->number);
#endif

		/* Charge the money */
		p_ptr->au -= (cost * o_ptr->number);

		if (item >= INVEN_RARM) calc_android_exp();

		/* Something happened */
		return (TRUE);
	}
}


/*!
 * @brief ��ƻ��λ��Ѳ���������������ߤΥᥤ��롼���� / Recharge rods, wands and staves
 * @details
 * The player can select the number of charges to add\n
 * (up to a limit), and the recharge never fails.\n
 *\n
 * The cost for rods depends on the level of the rod. The prices\n
 * for recharging wands and staves are dependent on the cost of\n
 * the base-item.\n
 * @return �ʤ�
 */
static void building_recharge(void)
{
	OBJECT_IDX  item;
	int         lev;
	object_type *o_ptr;
	object_kind *k_ptr;
	cptr        q, s;
	int         price;
	PARAMETER_VALUE charges;
	int         max_charges;
	char        tmp_str[MAX_NLEN];

	msg_flag = FALSE;

	/* Display some info */
	clear_bldg(4, 18);
	prt(_("  �ƽ�Ŷ�����Ѥϥ����ƥ�μ���ˤ��ޤ���", "  The prices of recharge depend on the type."), 6, 0);


	/* Only accept legal items */
	item_tester_hook = item_tester_hook_recharge;

	/* Get an item */
	q = _("�ɤΥ����ƥ�����Ϥ�Ŷ���ޤ���? ", "Recharge which item? ");
	s = _("���Ϥ�Ŷ���٤������ƥब�ʤ���", "You have nothing to recharge.");
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

	k_ptr = &k_info[o_ptr->k_idx];

	/*
	 * We don't want to give the player free info about
	 * the level of the item or the number of charges.
	 */
	/* The item must be "known" */
	if (!object_is_known(o_ptr))
	{
		msg_format(_("��Ŷ�������˴��ꤵ��Ƥ���ɬ�פ�����ޤ���", "The item must be identified first!"));
		msg_print(NULL);

		if ((p_ptr->au >= 50) &&
			get_check(_("��50�Ǵ��ꤷ�ޤ����� ", "Identify for 50 gold? ")))

		{
			/* Pay the price */
			p_ptr->au -= 50;

			/* Identify it */
			identify_item(o_ptr);

			/* Description */
			object_desc(tmp_str, o_ptr, 0);
			msg_format(_("%s �Ǥ���", "You have: %s."), tmp_str);

			/* Auto-inscription */
			autopick_alter_item(item, FALSE);

			/* Update the gold display */
			building_prt_gold();
		}
		else
		{
			return;
		}
	}

	/* Extract the object "level" */
	lev = k_info[o_ptr->k_idx].level;

	/* Price for a rod */
	if (o_ptr->tval == TV_ROD)
	{
		if (o_ptr->timeout > 0)
		{
			/* Fully recharge */
			price = (lev * 50 * o_ptr->timeout) / k_ptr->pval;
		}
		else
		{
			/* No recharge necessary */
			price = 0;
			msg_format(_("����Ϻƽ�Ŷ����ɬ�פϤ���ޤ���", "That doesn't need to be recharged."));
			return;
		}
	}
	else if (o_ptr->tval == TV_STAFF)
	{
		/* Price per charge ( = double the price paid by shopkeepers for the charge) */
		price = (k_info[o_ptr->k_idx].cost / 10) * o_ptr->number;

		/* Pay at least 10 gold per charge */
		price = MAX(10, price);
	}
	else
	{
		/* Price per charge ( = double the price paid by shopkeepers for the charge) */
		price = (k_info[o_ptr->k_idx].cost / 10);

		/* Pay at least 10 gold per charge */
		price = MAX(10, price);
	}

	/* Limit the number of charges for wands and staffs */
	if (o_ptr->tval == TV_WAND
		&& (o_ptr->pval / o_ptr->number >= k_ptr->pval))
	{
		if (o_ptr->number > 1)
		{
			msg_print(_("������ˡ���Ϥ⤦��ʬ�˽�Ŷ����Ƥ��ޤ���", "These wands are already fully charged."));
		}
		else
		{
			msg_print(_("������ˡ���Ϥ⤦��ʬ�˽�Ŷ����Ƥ��ޤ���", "This wand is already fully charged."));
		}
		return;
	}
	else if (o_ptr->tval == TV_STAFF && o_ptr->pval >= k_ptr->pval)
	{
		if (o_ptr->number > 1)
		{
			msg_print(_("���ξ�Ϥ⤦��ʬ�˽�Ŷ����Ƥ��ޤ���", "These staffs are already fully charged."));
		}
		else
		{
			msg_print(_("���ξ�Ϥ⤦��ʬ�˽�Ŷ����Ƥ��ޤ���", "This staff is already fully charged."));
		}
		return;
	}

	/* Check if the player has enough money */
	if (p_ptr->au < price)
	{
		object_desc(tmp_str, o_ptr, OD_NAME_ONLY);
#ifdef JP
		msg_format("%s��ƽ�Ŷ����ˤϡ�%d ɬ�פǤ���", tmp_str,price );
#else
		msg_format("You need %d gold to recharge %s!", price, tmp_str);
#endif

		return;
	}

	if (o_ptr->tval == TV_ROD)
	{
#ifdef JP
if (get_check(format("���Υ�åɤ��%d �Ǻƽ�Ŷ���ޤ�����",
 price)))
#else
		if (get_check(format("Recharge the %s for %d gold? ",
			((o_ptr->number > 1) ? "rods" : "rod"), price)))
#endif

		{
			/* Recharge fully */
			o_ptr->timeout = 0;
		}
		else
		{
			return;
		}
	}
	else
	{
		if (o_ptr->tval == TV_STAFF)
			max_charges = k_ptr->pval - o_ptr->pval;
		else
			max_charges = o_ptr->number * k_ptr->pval - o_ptr->pval;

		/* Get the quantity for staves and wands */
		charges = (PARAMETER_VALUE)get_quantity(format(_("���ʬ��%d �ǲ���ʬ��Ŷ���ޤ�����", "Add how many charges for %d gold? "), price), 
					MIN(p_ptr->au / price, max_charges));

		/* Do nothing */
		if (charges < 1) return;

		/* Get the new price */
		price *= charges;

		/* Recharge */
		o_ptr->pval += charges;

		/* We no longer think the item is empty */
		o_ptr->ident &= ~(IDENT_EMPTY);
	}

	/* Give feedback */
	object_desc(tmp_str, o_ptr, 0);
#ifdef JP
	msg_format("%s���%d �Ǻƽ�Ŷ���ޤ�����", tmp_str, price);
#else
	msg_format("%^s %s recharged for %d gold.", tmp_str, ((o_ptr->number > 1) ? "were" : "was"), price);
#endif

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Window stuff */
	p_ptr->window |= (PW_INVEN);

	/* Pay the price */
	p_ptr->au -= price;

	/* Finished */
	return;
}


/*!
 * @brief ��ƻ��λ��Ѳ���������������ߤΰ������������֥롼���� / Recharge rods, wands and staves
 * @details
 * The player can select the number of charges to add\n
 * (up to a limit), and the recharge never fails.\n
 *\n
 * The cost for rods depends on the level of the rod. The prices\n
 * for recharging wands and staves are dependent on the cost of\n
 * the base-item.\n
 * @return �ʤ�
 */
static void building_recharge_all(void)
{
	int         i;
	int         lev;
	object_type *o_ptr;
	object_kind *k_ptr;
	int         price = 0;
	int         total_cost = 0;


	/* Display some info */
	msg_flag = FALSE;
	clear_bldg(4, 18);
	prt(_("  �ƽ�Ŷ�����Ѥϥ����ƥ�μ���ˤ��ޤ���", "  The prices of recharge depend on the type."), 6, 0);

	/* Calculate cost */
	for ( i = 0; i < INVEN_PACK; i++)
	{
		o_ptr = &inventory[i];
				
		/* skip non magic device */
		if (o_ptr->tval < TV_STAFF || o_ptr->tval > TV_ROD) continue;

		/* need identified */
		if (!object_is_known(o_ptr)) total_cost += 50;

		/* Extract the object "level" */
		lev = k_info[o_ptr->k_idx].level;

		k_ptr = &k_info[o_ptr->k_idx];

		switch (o_ptr->tval)
		{
		case TV_ROD:
			price = (lev * 50 * o_ptr->timeout) / k_ptr->pval;
			break;

		case TV_STAFF:
			/* Price per charge ( = double the price paid by shopkeepers for the charge) */
			price = (k_info[o_ptr->k_idx].cost / 10) * o_ptr->number;

			/* Pay at least 10 gold per charge */
			price = MAX(10, price);

			/* Fully charge */
			price = (k_ptr->pval - o_ptr->pval) * price;
			break;

		case TV_WAND:
			/* Price per charge ( = double the price paid by shopkeepers for the charge) */
			price = (k_info[o_ptr->k_idx].cost / 10);

			/* Pay at least 10 gold per charge */
			price = MAX(10, price);

			/* Fully charge */
			price = (o_ptr->number * k_ptr->pval - o_ptr->pval) * price;
			break;
		}

		/* if price <= 0 then item have enough charge */
		if (price > 0) total_cost += price;
	}

	if (!total_cost)
	{
		msg_print(_("��Ŷ����ɬ�פϤ���ޤ���", "No need to recharge."));
		msg_print(NULL);
		return;
	}

	/* Check if the player has enough money */
	if (p_ptr->au < total_cost)
	{
		msg_format(_("���٤ƤΥ����ƥ��ƽ�Ŷ����ˤϡ�%d ɬ�פǤ���", "You need %d gold to recharge all items!"), total_cost );
		msg_print(NULL);
		return;
	}
	if (!get_check(format(_("���٤ƤΥ����ƥ�� ��%d �Ǻƽ�Ŷ���ޤ�����", "Recharge all items for %d gold? "),  total_cost))) return;
	
	for (i = 0; i < INVEN_PACK; i++)
	{
		o_ptr = &inventory[i];
		k_ptr = &k_info[o_ptr->k_idx];

		/* skip non magic device */
		if (o_ptr->tval < TV_STAFF || o_ptr->tval > TV_ROD) continue;

		/* Identify it */
		if (!object_is_known(o_ptr))
		{
			identify_item(o_ptr);

			/* Auto-inscription */
			autopick_alter_item(i, FALSE);
		}

		/* Recharge */
		switch (o_ptr->tval)
		{
		case TV_ROD:
			o_ptr->timeout = 0;
			break;
		case TV_STAFF:
			if (o_ptr->pval < k_ptr->pval) o_ptr->pval = k_ptr->pval;
			/* We no longer think the item is empty */
			o_ptr->ident &= ~(IDENT_EMPTY);
			break;
		case TV_WAND:
			if (o_ptr->pval < o_ptr->number * k_ptr->pval)
				o_ptr->pval = o_ptr->number * k_ptr->pval;
			/* We no longer think the item is empty */
			o_ptr->ident &= ~(IDENT_EMPTY);
			break;
		}
	}

	/* Give feedback */
	msg_format(_("��%d �Ǻƽ�Ŷ���ޤ�����", "You pay %d gold."), total_cost);
	msg_print(NULL);

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Window stuff */
	p_ptr->window |= (PW_INVEN);

	/* Pay the price */
	p_ptr->au -= total_cost;

	/* Finished */
	return;
}

/*!
 * @brief Į�֤Υƥ�ݡ��Ȥ�Ԥ��ᥤ��롼����
 * @return �ƥ�ݡ��Ƚ�������ꤷ�����ݤ�
 */
bool tele_town(void)
{
	int i, x, y;
	int num = 0;

	if (dun_level)
	{
		msg_print(_("������ˡ���Ͼ�Ǥ����Ȥ��ʤ���", "This spell can only be used on the surface!"));
		return FALSE;
	}

	if (p_ptr->inside_arena || p_ptr->inside_battle)
	{
		msg_print(_("������ˡ�ϳ��Ǥ����Ȥ��ʤ���", "This spell can only be used outside!"));
		return FALSE;
	}

	screen_save();
	clear_bldg(4, 10);

	for (i=1;i<max_towns;i++)
	{
		char buf[80];

		if ((i == NO_TOWN) || (i == SECRET_TOWN) || (i == p_ptr->town_num) || !(p_ptr->visit & (1L << (i-1)))) continue;

		sprintf(buf,"%c) %-20s", I2A(i-1), town[i].name);
		prt(buf, 5+i, 5);
		num++;
	}

	if (!num)
	{
		msg_print(_("�ޤ��Ԥ���Ȥ����ʤ���", "You have not yet visited any town."));
		msg_print(NULL);
		screen_load();
		return FALSE;
	}

	prt(_("�ɤ��˹Ԥ��ޤ���:", "Which town you go: "), 0, 0);
	while(1)
	{
		i = inkey();

		if (i == ESCAPE)
		{
			screen_load();
			return FALSE;
		}
		else if ((i < 'a') || (i > ('a'+max_towns-2))) continue;
		else if (((i-'a'+1) == p_ptr->town_num) || ((i-'a'+1) == NO_TOWN) || ((i-'a'+1) == SECRET_TOWN) || !(p_ptr->visit & (1L << (i-'a')))) continue;
		break;
	}

	for (y = 0; y < max_wild_y; y++)
	{
		for (x = 0; x < max_wild_x; x++)
		{
			if(wilderness[y][x].town == (i-'a'+1))
			{
				p_ptr->wilderness_y = y;
				p_ptr->wilderness_x = x;
			}
		}
	}

	p_ptr->leaving = TRUE;
	leave_bldg = TRUE;
	p_ptr->teleport_town = TRUE;
	screen_load();
	return TRUE;
}

/*!
 * @brief ���ߤǥ�󥹥����ξ�����Τ�ᥤ��롼���� / research_mon -KMW-
 * @return ���TRUE���֤���
 * @todo �֤��ͤ���̣�����ʤΤ�ľ���������ɤ����⤷��ʤ���
 */
static bool research_mon(void)
{
	IDX i;
	int n;
	MONRACE_IDX r_idx;
	char sym, query;
	char buf[128];

	bool notpicked;

	bool recall = FALSE;

	u16b why = 0;

	IDX *who;

	/* XTRA HACK WHATSEARCH */
	bool    all = FALSE;
	bool    uniq = FALSE;
	bool    norm = FALSE;
	char temp[80] = "";

	/* XTRA HACK REMEMBER_IDX */
	static int old_sym = '\0';
	static IDX old_i = 0;


	/* Save the screen */
	screen_save();

	/* Get a character, or abort */
	if (!get_com(_("��󥹥�����ʸ�������Ϥ��Ʋ�����(���� or ^A��,^U��,^N���,^M̾��):",
				   "Enter character to be identified(^A:All,^U:Uniqs,^N:Non uniqs,^M:Name): "), &sym, FALSE)) 

	{
		/* Restore */
		screen_load();

		return (FALSE);
	}

	/* Find that character info, and describe it */
	for (i = 0; ident_info[i]; ++i)
	{
		if (sym == ident_info[i][0]) break;
	}

		/* XTRA HACK WHATSEARCH */
	if (sym == KTRL('A'))
	{
		all = TRUE;
		strcpy(buf, _("����󥹥����Υꥹ��", "Full monster list."));
	}
	else if (sym == KTRL('U'))
	{
		all = uniq = TRUE;
		strcpy(buf, _("��ˡ�������󥹥����Υꥹ��", "Unique monster list."));
	}
	else if (sym == KTRL('N'))
	{
		all = norm = TRUE;
		strcpy(buf, _("��ˡ�������󥹥����Υꥹ��", "Non-unique monster list."));
	}
	else if (sym == KTRL('M'))
	{
		all = TRUE;
		if (!get_string(_("̾��(�Ѹ�ξ�羮ʸ���ǲ�)", "Enter name:"),temp, 70))
		{
			temp[0]=0;

			/* Restore */
			screen_load();

			return FALSE;
		}
		sprintf(buf, _("̾��:%s�˥ޥå�", "Monsters with a name \"%s\""),temp);
	}
	else if (ident_info[i])
	{
		sprintf(buf, "%c - %s.", sym, ident_info[i] + 2);
	}
	else
	{
		sprintf(buf, "%c - %s", sym, _("̵����ʸ��", "Unknown Symbol"));
	}

	/* Display the result */
	prt(buf, 16, 10);


	/* Allocate the "who" array */
	C_MAKE(who, max_r_idx, IDX);

	/* Collect matching monsters */
	for (n = 0, i = 1; i < max_r_idx; i++)
	{
		monster_race *r_ptr = &r_info[i];

		/* Empty monster */
		if (!r_ptr->name) continue;

		/* XTRA HACK WHATSEARCH */
		/* Require non-unique monsters if needed */
		if (norm && (r_ptr->flags1 & (RF1_UNIQUE))) continue;

		/* Require unique monsters if needed */
		if (uniq && !(r_ptr->flags1 & (RF1_UNIQUE))) continue;

		/* ̾������ */
		if (temp[0])
		{
			int xx;
			char temp2[80];

			for (xx = 0; temp[xx] && xx < 80; xx++)
			{
#ifdef JP
				if (iskanji(temp[xx]))
				{
					xx++;
					continue;
				}
#endif
				if (isupper(temp[xx])) temp[xx] = (char)tolower(temp[xx]);
			}
  
#ifdef JP
			strcpy(temp2, r_name + r_ptr->E_name);
#else
			strcpy(temp2, r_name + r_ptr->name);
#endif
			for (xx = 0; temp2[xx] && xx < 80; xx++)
				if (isupper(temp2[xx])) temp2[xx] = (char)tolower(temp2[xx]);

#ifdef JP
			if (my_strstr(temp2, temp) || my_strstr(r_name + r_ptr->name, temp))
#else
			if (my_strstr(temp2, temp))
#endif
				who[n++] = i;
		}
		else if (all || (r_ptr->d_char == sym)) who[n++] = i;
	}

	/* Nothing to recall */
	if (!n)
	{
		/* Free the "who" array */
		C_KILL(who, max_r_idx, IDX);

		/* Restore */
		screen_load();

		return (FALSE);
	}

	/* Sort by level */
	why = 2;
	query = 'y';

	/* Sort if needed */
	if (why)
	{
		/* Select the sort method */
		ang_sort_comp = ang_sort_comp_hook;
		ang_sort_swap = ang_sort_swap_hook;

		/* Sort the array */
		ang_sort(who, &why, n);
	}


	/* Start at the end */
	/* XTRA HACK REMEMBER_IDX */
	if (old_sym == sym && old_i < n) i = old_i;
	else i = n - 1;

	notpicked = TRUE;

	/* Scan the monster memory */
	while (notpicked)
	{
		/* Extract a race */
		r_idx = who[i];

		/* Hack -- Begin the prompt */
		roff_top(r_idx);

		/* Hack -- Complete the prompt */
		Term_addstr(-1, TERM_WHITE, _(" ['r'�פ���, ' '��³��, ESC]", " [(r)ecall, ESC, space to continue]"));

		/* Interact */
		while (1)
		{
			/* Recall */
			if (recall)
			{
				/*** Recall on screen ***/

				/* Get maximal info about this monster */
				lore_do_probe(r_idx);

				/* Save this monster ID */
				monster_race_track(r_idx);

				/* Hack -- Handle stuff */
				handle_stuff();

				/* know every thing mode */
				screen_roff(r_idx, 0x01);
				notpicked = FALSE;

				/* XTRA HACK REMEMBER_IDX */
				old_sym = sym;
				old_i = i;
			}

			/* Command */
			query = inkey();

			/* Normal commands */
			if (query != 'r') break;

			/* Toggle recall */
			recall = !recall;
		}

		/* Stop scanning */
		if (query == ESCAPE) break;

		/* Move to "prev" monster */
		if (query == '-')
		{
			if (++i == n)
			{
				i = 0;
				if (!expand_list) break;
			}
		}

		/* Move to "next" monster */
		else
		{
			if (i-- == 0)
			{
				i = n - 1;
				if (!expand_list) break;
			}
		}
	}


	/* Re-display the identity */
	/* prt(buf, 5, 5);*/

	/* Free the "who" array */
	C_KILL(who, max_r_idx, IDX);

	/* Restore */
	screen_load();

	return (!notpicked);
}


/*!
 * @brief ���ߤν����¹ԥᥤ��롼���� / Execute a building command
 * @param bldg ���߹�¤�Τλ��ȥݥ���
 * @param i �¹Ԥ��������ߤΥ����ӥ��ơ��֥��ź��
 * @return �ʤ�
 */
static void bldg_process_command(building_type *bldg, int i)
{
	BACT_IDX bact = bldg->actions[i];
	PRICE bcost;
	bool paid = FALSE;
	int amt;

	/* Flush messages XXX XXX XXX */
	msg_flag = FALSE;
	msg_print(NULL);

	if (is_owner(bldg))
		bcost = bldg->member_costs[i];
	else
		bcost = bldg->other_costs[i];

	/* action restrictions */
	if (((bldg->action_restr[i] == 1) && !is_member(bldg)) ||
	    ((bldg->action_restr[i] == 2) && !is_owner(bldg)))
	{
		msg_print(_("��������򤹤븢���Ϥ���ޤ���", "You have no right to choose that!"));
		return;
	}

	/* check gold (HACK - Recharge uses variable costs) */
	if ((bact != BACT_RECHARGE) &&
	    (((bldg->member_costs[i] > p_ptr->au) && is_owner(bldg)) ||
	     ((bldg->other_costs[i] > p_ptr->au) && !is_owner(bldg))))
	{
		msg_print(_("���⤬­��ޤ���", "You do not have the gold!"));
		return;
	}

	switch (bact)
	{
	case BACT_NOTHING:
		/* Do nothing */
		break;
	case BACT_RESEARCH_ITEM:
		paid = identify_fully(FALSE);
		break;
	case BACT_TOWN_HISTORY:
		town_history();
		break;
	case BACT_RACE_LEGENDS:
		race_legends();
		break;
	case BACT_QUEST:
		castle_quest();
		break;
	case BACT_KING_LEGENDS:
	case BACT_ARENA_LEGENDS:
	case BACT_LEGENDS:
		show_highclass();
		break;
	case BACT_POSTER:
	case BACT_ARENA_RULES:
	case BACT_ARENA:
		arena_comm(bact);
		break;
	case BACT_IN_BETWEEN:
	case BACT_CRAPS:
	case BACT_SPIN_WHEEL:
	case BACT_DICE_SLOTS:
	case BACT_GAMBLE_RULES:
	case BACT_POKER:
		gamble_comm(bact);
		break;
	case BACT_REST:
	case BACT_RUMORS:
	case BACT_FOOD:
		paid = inn_comm(bact);
		break;
	case BACT_RESEARCH_MONSTER:
		paid = research_mon();
		break;
	case BACT_COMPARE_WEAPONS:
		paid = TRUE;
		bcost = compare_weapons(bcost);
		break;
	case BACT_ENCHANT_WEAPON:
		item_tester_hook = object_allow_enchant_melee_weapon;
		enchant_item(bcost, 1, 1, 0);
		break;
	case BACT_ENCHANT_ARMOR:
		item_tester_hook = object_is_armour;
		enchant_item(bcost, 0, 0, 1);
		break;
	case BACT_RECHARGE:
		building_recharge();
		break;
	case BACT_RECHARGE_ALL:
		building_recharge_all();
		break;
	case BACT_IDENTS: /* needs work */
#ifdef JP
		if (!get_check("����ʪ�����ƴ��ꤷ�Ƥ�����Ǥ�����")) break;
		identify_pack();
		msg_print(" ����ʪ���Ƥ����ꤵ��ޤ�����");
#else
		if (!get_check("Do you pay for identify all your possession? ")) break;
		identify_pack();
		msg_print("Your possessions have been identified.");
#endif

		paid = TRUE;
		break;
	case BACT_IDENT_ONE: /* needs work */
		paid = ident_spell(FALSE);
		break;
	case BACT_LEARN:
		do_cmd_study();
		break;
	case BACT_HEALING: /* needs work */
		hp_player(200);
		set_poisoned(0);
		set_blind(0);
		set_confused(0);
		set_cut(0);
		set_stun(0);
		paid = TRUE;
		break;
	case BACT_RESTORE: /* needs work */
		if (do_res_stat(A_STR)) paid = TRUE;
		if (do_res_stat(A_INT)) paid = TRUE;
		if (do_res_stat(A_WIS)) paid = TRUE;
		if (do_res_stat(A_DEX)) paid = TRUE;
		if (do_res_stat(A_CON)) paid = TRUE;
		if (do_res_stat(A_CHR)) paid = TRUE;
		break;
	case BACT_ENCHANT_ARROWS:
		item_tester_hook = item_tester_hook_ammo;
		enchant_item(bcost, 1, 1, 0);
		break;
	case BACT_ENCHANT_BOW:
		item_tester_tval = TV_BOW;
		enchant_item(bcost, 1, 1, 0);
		break;
	case BACT_RECALL:
		if (recall_player(1)) paid = TRUE;
		break;
	case BACT_TELEPORT_LEVEL:
	{
		IDX select_dungeon;
		DEPTH max_depth;

		clear_bldg(4, 20);
		select_dungeon = choose_dungeon(_("�˥ƥ�ݡ���", "teleport"), 4, 0);
		show_building(bldg);
		if (!select_dungeon) return;

		max_depth = d_info[select_dungeon].maxdepth;

		/* Limit depth in Angband */
		if (select_dungeon == DUNGEON_ANGBAND)
		{
			if (quest[QUEST_OBERON].status != QUEST_STATUS_FINISHED) max_depth = 98;
			else if(quest[QUEST_SERPENT].status != QUEST_STATUS_FINISHED) max_depth = 99;
		}
		amt = get_quantity(format(_("%s�β����˥ƥ�ݡ��Ȥ��ޤ�����", "Teleport to which level of %s? "), 
							d_name + d_info[select_dungeon].name), (QUANTITY)max_depth);

		if (amt > 0)
		{
			p_ptr->word_recall = 1;
			p_ptr->recall_dungeon = select_dungeon;
			max_dlv[p_ptr->recall_dungeon] = ((amt > d_info[select_dungeon].maxdepth) ? d_info[select_dungeon].maxdepth : ((amt < d_info[select_dungeon].mindepth) ? d_info[select_dungeon].mindepth : amt));
			if (record_maxdepth)
				do_cmd_write_nikki(NIKKI_TRUMP, select_dungeon, _("�ȥ��ץ����", "at Trump Tower"));
				
			msg_print(_("�����絤��ĥ��Ĥ�Ƥ���...", "The air about you becomes charged..."));

			paid = TRUE;
			p_ptr->redraw |= (PR_STATUS);
		}
		break;
	}
	case BACT_LOSE_MUTATION:
		if (p_ptr->muta1 || p_ptr->muta2 ||
		    (p_ptr->muta3 & ~MUT3_GOOD_LUCK) ||
		    (p_ptr->pseikaku != SEIKAKU_LUCKY &&
		     (p_ptr->muta3 & MUT3_GOOD_LUCK)))
		{
			while(!lose_mutation(0));
			paid = TRUE;
		}
		else
		{
			msg_print(_("�����٤������Ѱۤ�̵����", "You have no mutations."));
			msg_print(NULL);
		}
		break;
	case BACT_BATTLE:
		kakutoujou();
		break;
	case BACT_TSUCHINOKO:
		tsuchinoko();
		break;
	case BACT_KUBI:
		shoukinkubi();
		break;
	case BACT_TARGET:
		today_target();
		break;
	case BACT_KANKIN:
		kankin();
		break;
	case BACT_HEIKOUKA:
		msg_print(_("ʿ�ղ��ε�����Ԥʤä���", "You received an equalization ritual."));
		set_virtue(V_COMPASSION, 0);
		set_virtue(V_HONOUR, 0);
		set_virtue(V_JUSTICE, 0);
		set_virtue(V_SACRIFICE, 0);
		set_virtue(V_KNOWLEDGE, 0);
		set_virtue(V_FAITH, 0);
		set_virtue(V_ENLIGHTEN, 0);
		set_virtue(V_ENCHANT, 0);
		set_virtue(V_CHANCE, 0);
		set_virtue(V_NATURE, 0);
		set_virtue(V_HARMONY, 0);
		set_virtue(V_VITALITY, 0);
		set_virtue(V_UNLIFE, 0);
		set_virtue(V_PATIENCE, 0);
		set_virtue(V_TEMPERANCE, 0);
		set_virtue(V_DILIGENCE, 0);
		set_virtue(V_VALOUR, 0);
		set_virtue(V_INDIVIDUALISM, 0);
		get_virtues();
		paid = TRUE;
		break;
	case BACT_TELE_TOWN:
		paid = tele_town();
		break;
	case BACT_EVAL_AC:
		paid = eval_ac(p_ptr->dis_ac + p_ptr->dis_to_a);
		break;
	case BACT_BROKEN_WEAPON:
		paid = TRUE;
		bcost = repair_broken_weapon(bcost);
		break;
	}

	if (paid)
	{
		p_ptr->au -= bcost;
	}
}

/*!
 * @brief ��������������˥ץ쥤�䡼����ä��ݤν��� / Do building commands
 * @return �ʤ�
 */
void do_cmd_quest(void)
{
	p_ptr->energy_use = 100;

	if (!cave_have_flag_bold(p_ptr->y, p_ptr->x, FF_QUEST_ENTER))
	{
		msg_print(_("�����ˤϥ������Ȥ������Ϥʤ���", "You see no quest level here."));
		return;
	}
	else
	{
		msg_print(_("�����ˤϥ������Ȥؤ�����������ޤ���", "There is an entry of a quest."));
		if (!get_check(_("�������Ȥ�����ޤ�����", "Do you enter? "))) return;
		if ((p_ptr->pseikaku == SEIKAKU_COMBAT) || (inventory[INVEN_BOW].name1 == ART_CRIMSON))
			msg_print(_("�ؤȤˤ������äƤߤ褦��������", ""));

		/* Player enters a new quest */
		p_ptr->oldpy = 0;
		p_ptr->oldpx = 0;

		leave_quest_check();

		if (quest[p_ptr->inside_quest].type != QUEST_TYPE_RANDOM) dun_level = 1;
		p_ptr->inside_quest = cave[p_ptr->y][p_ptr->x].special;

		p_ptr->leaving = TRUE;
	}
}


/*!
 * @brief ����������˥ץ쥤�䡼����ä��ݤν��� / Do building commands
 * @return �ʤ�
 */
void do_cmd_bldg(void)
{
	int             i, which;
	char            command;
	bool            validcmd;
	building_type   *bldg;


	p_ptr->energy_use = 100;

	if (!cave_have_flag_bold(p_ptr->y, p_ptr->x, FF_BLDG))
	{
		msg_print(_("�����ˤϷ�ʪ�Ϥʤ���", "You see no building here."));
		return;
	}

	which = f_info[cave[p_ptr->y][p_ptr->x].feat].subtype;

	bldg = &building[which];

	/* Don't re-init the wilderness */
	reinit_wilderness = FALSE;

	if ((which == 2) && (p_ptr->arena_number < 0))
	{
		msg_print(_("���ԼԤ��ѤϤʤ�����", "'There's no place here for a LOSER like you!'"));
		return;
	}
	else if ((which == 2) && p_ptr->inside_arena)
	{
		if (!p_ptr->exit_bldg && m_cnt > 0)
		{
			prt(_("�����Ȥ��ĤޤäƤ��롣��󥹥��������ʤ����ԤäƤ��롪", "The gates are closed.  The monster awaits!"), 0, 0);
		}
		else
		{
			/* Don't save the arena as saved floor */
			prepare_change_floor_mode(CFM_SAVE_FLOORS | CFM_NO_RETURN);

			p_ptr->inside_arena = FALSE;
			p_ptr->leaving = TRUE;

			/* Re-enter the arena */
			command_new = SPECIAL_KEY_BUILDING;

			/* No energy needed to re-enter the arena */
			p_ptr->energy_use = 0;
		}

		return;
	}
	else if (p_ptr->inside_battle)
	{
		/* Don't save the arena as saved floor */
		prepare_change_floor_mode(CFM_SAVE_FLOORS | CFM_NO_RETURN);

		p_ptr->leaving = TRUE;
		p_ptr->inside_battle = FALSE;

		/* Re-enter the monster arena */
		command_new = SPECIAL_KEY_BUILDING;

		/* No energy needed to re-enter the arena */
		p_ptr->energy_use = 0;

		return;
	}
	else
	{
		p_ptr->oldpy = p_ptr->y;
		p_ptr->oldpx = p_ptr->x;
	}

	/* Forget the lite */
	forget_lite();

	/* Forget the view */
	forget_view();

	/* Hack -- Increase "icky" depth */
	character_icky++;

	command_arg = 0;
	command_rep = 0;
	command_new = 0;

	show_building(bldg);
	leave_bldg = FALSE;

	play_music(TERM_XTRA_MUSIC_BASIC, MUSIC_BASIC_BUILD);

	while (!leave_bldg)
	{
		validcmd = FALSE;
		prt("", 1, 0);

		building_prt_gold();

		command = inkey();

		if (command == ESCAPE)
		{
			leave_bldg = TRUE;
			p_ptr->inside_arena = FALSE;
			p_ptr->inside_battle = FALSE;
			break;
		}

		for (i = 0; i < 8; i++)
		{
			if (bldg->letters[i])
			{
				if (bldg->letters[i] == command)
				{
					validcmd = TRUE;
					break;
				}
			}
		}

		if (validcmd)
			bldg_process_command(bldg, i);

		/* Notice stuff */
		notice_stuff();

		/* Handle stuff */
		handle_stuff();
	}

	select_floor_music();

	/* Flush messages XXX XXX XXX */
	msg_flag = FALSE;
	msg_print(NULL);

	/* Reinit wilderness to activate quests ... */
	if (reinit_wilderness)
	{
		p_ptr->leaving = TRUE;
	}

	/* Hack -- Decrease "icky" depth */
	character_icky--;

	/* Clear the screen */
	Term_clear();

	/* Update the visuals */
	p_ptr->update |= (PU_VIEW | PU_MONSTERS | PU_BONUS | PU_LITE | PU_MON_LITE);

	/* Redraw entire screen */
	p_ptr->redraw |= (PR_BASIC | PR_EXTRA | PR_EQUIPPY | PR_MAP);

	/* Window stuff */
	p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);
}


/*!
 * @brief ���������������Υ�å������ơ��֥� / Array of places to find an inscription
 */
static cptr find_quest[] =
{
	_("���˥�å���������ޤ�Ƥ���:", "You find the following inscription in the floor"),
	_("�ɤ˥�å���������ޤ�Ƥ���:", "You see a message inscribed in the wall"),
	_("��å������򸫤Ĥ���:", "There is a sign saying"),
	_("���������ʤξ�˽񤤤Ƥ���:", "Something is written on the staircase"),
	_("��ʪ�򸫤Ĥ�������å��������񤤤Ƥ���:", "You find a scroll with the following message"),
};


/*!
 * @brief �������Ȥ�Ƴ����å�������ɽ������ / Discover quest
 * @param q_idx ���Ϥ��줿�������Ȥ�ID
 */
void quest_discovery(int q_idx)
{
	quest_type      *q_ptr = &quest[q_idx];
	monster_race    *r_ptr = &r_info[q_ptr->r_idx];
	int             q_num = q_ptr->max_num;
	char            name[80];

	/* No quest index */
	if (!q_idx) return;

	strcpy(name, (r_name + r_ptr->name));

	msg_print(find_quest[rand_range(0, 4)]);
	msg_print(NULL);

	if (q_num == 1)
	{
		/* Unique */

		/* Hack -- "unique" monsters must be "unique" */
		if ((r_ptr->flags1 & RF1_UNIQUE) &&
		    (0 == r_ptr->max_num))
		{
			msg_print(_("���γ��ϰ�����ï���ˤ�äƼ���Ƥ����褦���ġ�", "It seems that this level was protected by someone before..."));
			/* The unique is already dead */
			quest[q_idx].status = QUEST_STATUS_FINISHED;
			q_ptr->complev = 0;
			update_playtime();
			q_ptr->comptime = playtime;
		}
		else
		{
			msg_format(_("��դ��衪���γ���%s�ˤ�äƼ���Ƥ��롪", "Beware, this level is protected by %s!"), name);
		}
	}
	else
	{
		/* Normal monsters */
#ifndef JP
		plural_aux(name);
#endif
		msg_format(_("��դ������γ���%d�Τ�%s�ˤ�äƼ���Ƥ��롪", "Be warned, this level is guarded by %d %s!"), q_num, name);

	}
}


/*!
 * @brief ���������ä����󥸥��γ��ؤ˸��ꤵ��Ƥ�����̤Υ������Ȥ�õ���Ф�ID���֤���
 * / Hack -- Check if a level is a "quest" level
 * @param level �����оݤˤʤ볬
 * @return ��������ID���֤����������ʤ����0���֤���
 */
IDX quest_number(DEPTH level)
{
	IDX i;

	/* Check quests */
	if (p_ptr->inside_quest)
		return (p_ptr->inside_quest);

	for (i = 0; i < max_q_idx; i++)
	{
		if (quest[i].status != QUEST_STATUS_TAKEN) continue;

		if ((quest[i].type == QUEST_TYPE_KILL_LEVEL) &&
			!(quest[i].flags & QUEST_FLAG_PRESET) &&
		    (quest[i].level == level) &&
		    (quest[i].dungeon == dungeon_type))
			return (i);
	}

	/* Check for random quest */
	return (random_quest_number(level));
}

/*!
 * @brief ���������ä����󥸥��γ��ؤ˸��ꤵ��Ƥ�������९�����Ȥ�õ���Ф�ID���֤���
 * @param level �����оݤˤʤ볬
 * @return ��������ID���֤����������ʤ����0���֤���
 */
IDX random_quest_number(DEPTH level)
{
	IDX i;

	if (dungeon_type != DUNGEON_ANGBAND) return 0;

	for (i = MIN_RANDOM_QUEST; i < MAX_RANDOM_QUEST + 1; i++)
	{
		if ((quest[i].type == QUEST_TYPE_RANDOM) &&
			(quest[i].status == QUEST_STATUS_TAKEN) &&
			(quest[i].level == level) &&
			(quest[i].dungeon == DUNGEON_ANGBAND))
		{
			return i;
		}
	}

	/* Nope */
	return 0;
}
