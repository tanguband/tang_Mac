/*!
 * @file monster1.c
 * @brief ��󥹥�������ε��� / describe monsters (using monster memory)
 * @date 2013/12/11
 * @author
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.  Other copyrights may also apply.
 * 2014 Deskull rearranged comment for Doxygen.
 */

#include "angband.h"


/*
 * Pronoun arrays, by gender.
 */
static cptr wd_he[3] =
#ifdef JP
{ "����", "��", "���" };
#else
{ "it", "he", "she" };
#endif

static cptr wd_his[3] =
#ifdef JP
{ "�����", "���", "�����" };
#else
{ "its", "his", "her" };
#endif



/*!
 * �Ѹ��ʣ���ϵ����ѥޥ��� / Pluralizer.  Args(count, singular, plural)
 */
#define plural(c,s,p) \
    (((c) == 1) ? (s) : (p))



/*!
 * @brief ��󥹥�����AC��������뤳�Ȥ��Ǥ��뤫���֤� / Determine if the "armor" is known
 * @param r_idx ��󥹥����μ�²ID
 * @return Ũ��AC���Τ��郎��������Ƥ���ʤ�TRUE���֤�
 * @details
 * The higher the level, the fewer kills needed.
 */
static bool know_armour(int r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	s32b level = r_ptr->level;

	s32b kills = r_ptr->r_tkills;

    bool known = (r_ptr->r_cast_spell == MAX_UCHAR)? TRUE: FALSE;

	if (cheat_know || known) return (TRUE);

	/* Normal monsters */
	if (kills > 304 / (4 + level)) return (TRUE);

	/* Skip non-uniques */
	if (!(r_ptr->flags1 & RF1_UNIQUE)) return (FALSE);

	/* Unique monsters */
	if (kills > 304 / (38 + (5 * level) / 4)) return (TRUE);

	/* Assume false */
	return (FALSE);
}


/*!
 * @brief ��󥹥������Ƿ���Ϥ��Τ뤳�Ȥ��Ǥ��뤫�ɤ������֤�
 * Determine if the "damage" of the given attack is known
 * @param r_idx ��󥹥����μ�²ID
 * @param i ��ǧ�������������
 * @return Ũ�Υ��᡼�����������Τ��郎��������Ƥ���ʤ�TRUE���֤�
 * @details
 * <pre>
 * the higher the level of the monster, the fewer the attacks you need,
 * the more damage an attack does, the more attacks you need
 * </pre>
 */
static bool know_damage(int r_idx, int i)
{
	monster_race *r_ptr = &r_info[r_idx];

	s32b level = r_ptr->level;

	s32b a = r_ptr->r_blows[i];

	s32b d1 = r_ptr->blow[i].d_dice;
	s32b d2 = r_ptr->blow[i].d_side;

	s32b d = d1 * d2;

	if (d >= ((4+level)*MAX_UCHAR)/80) d = ((4+level)*MAX_UCHAR-1)/80;

	/* Normal monsters */
	if ((4 + level) * a > 80 * d) return (TRUE);

	/* Skip non-uniques */
	if (!(r_ptr->flags1 & RF1_UNIQUE)) return (FALSE);

	/* Unique monsters */
	if ((4 + level) * (2 * a) > 80 * d) return (TRUE);

	/* Assume false */
	return (FALSE);
}


/*
 * Prepare hook for c_roff(). It will be changed for spoiler generation in wizard1.c.
 */
void (*hook_c_roff)(byte attr, cptr str) = c_roff;

/*!
 * @brief ��󥹥����λפ��Х�å������򤢤餫������ꤵ�줿�ؿ��ݥ��󥿤˴�Ť����Ϥ���
 * @param str ����ʸ����
 * @return �ʤ�
 */
static void hooked_roff(cptr str)
{
	/* Spawn */
	hook_c_roff(TERM_WHITE, str);
}

/*!
* @brief �������ܤ�ʸ������Ѵ�����
* @param base_damage ������
* @param dice_num ��������
* @param dice_side ��������
* @param dice_mult ��������Ψ
* @param dice_div ����������
* @param msg ʸ������Ǽ����ݥ���
* @return �ʤ�
*/
void dice_to_string(int base_damage, int dice_num, int dice_side, int dice_mult, int dice_div, char* msg)
{
    char base[80] = "", dice[80] = "", mult[80]="";

    if (dice_num == 0)
    {
        sprintf(msg, "%d", base_damage);
    }
    else
    {
        if (base_damage != 0)
            sprintf(base, "%d+", base_damage);

        if (dice_num == 1)
            sprintf(dice, "d%d", dice_side);
        else
            sprintf(dice, "%dd%d", dice_num, dice_side);

        if (dice_mult != 1 || dice_div != 1)
        {
            if (dice_div == 1)
                sprintf(mult, "*%d", dice_mult);
            else
                sprintf(mult, "*(%d/%d)", dice_mult, dice_div);
        }
        sprintf(msg, "%s%s%s", base, dice, mult);
    }
}

/*!
* @brief ʸ����˥�󥹥����ι����Ϥ�ä���
* @param r_idx ��󥹥����μ�²ID
* @param SPELL_NUM ��ʸ�ֹ�
* @param msg ɽ������ʸ����
* @param tmp �֤���å��������Ǽ��������
* @return �ʤ�
*/
void set_damage(int r_idx, int SPELL_NUM, char* msg, char* tmp)
{
    int base_damage = monspell_race_damage(SPELL_NUM, r_idx, BASE_DAM);
    int dice_num = monspell_race_damage(SPELL_NUM, r_idx, DICE_NUM);
    int dice_side = monspell_race_damage(SPELL_NUM, r_idx, DICE_SIDE);
    int dice_mult = monspell_race_damage(SPELL_NUM, r_idx, DICE_MULT);
    int dice_div = monspell_race_damage(SPELL_NUM, r_idx, DICE_DIV);
    char dmg_str[80], dice_str[80];
    dice_to_string(base_damage, dice_num, dice_side, dice_mult, dice_div, dmg_str);
    sprintf(dice_str, "(%s)", dmg_str);

    if (know_armour(r_idx))
        sprintf(tmp, msg, dice_str);
    else
        sprintf(tmp, msg, "");
}

/*!
 * @brief ��󥹥����λפ��о����ɽ������
 * Hack -- display monster information using "hooked_roff()"
 * @param r_idx ��󥹥����μ�²ID
 * @param mode ɽ�����ץ����
 * @return �ʤ�
 * @details
 * This function should only be called with the cursor placed at the
 * left edge of the screen, on a cleared line, in which the recall is
 * to take place.  One extra blank line is left after the recall.
 */
static void roff_aux(int r_idx, int mode)
{
	monster_race    *r_ptr = &r_info[r_idx];

	bool            old = FALSE;

	int             m, n, r;

	cptr            p, q;

#ifdef JP
	char            jverb_buf[64];
#else
	bool            sin = FALSE;
#endif
	int             msex = 0;

	bool nightmare = ironman_nightmare && !(mode & 0x02);
	int speed = nightmare ? r_ptr->speed + 5 : r_ptr->speed;

	bool            breath = FALSE;
	bool            magic = FALSE;
	bool            reinforce = FALSE;

	u32b		flags1;
	u32b		flags2;
	u32b		flags3;
	u32b		flags4;
	u32b		flags5;
	u32b		flags6;
	u32b		flags7;
	u32b		flagsr;

	byte drop_gold, drop_item;

	int		vn = 0;
	byte		color[96];
	cptr		vp[96];
	char tmp_msg[96][96];

	bool know_everything = FALSE;

	/* Obtain a copy of the "known" number of drops */
	drop_gold = r_ptr->r_drop_gold;
	drop_item = r_ptr->r_drop_item;

	/* Obtain a copy of the "known" flags */
	flags1 = (r_ptr->flags1 & r_ptr->r_flags1);
	flags2 = (r_ptr->flags2 & r_ptr->r_flags2);
	flags3 = (r_ptr->flags3 & r_ptr->r_flags3);
	flags4 = (r_ptr->flags4 & r_ptr->r_flags4);
	flags5 = (r_ptr->flags5 & r_ptr->r_flags5);
	flags6 = (r_ptr->flags6 & r_ptr->r_flags6);
	flags7 = (r_ptr->flags7 & r_ptr->flags7);
	flagsr = (r_ptr->flagsr & r_ptr->r_flagsr);

	for(n = 0; n < 6; n++)
	{
		if(r_ptr->reinforce_id[n] > 0) reinforce = TRUE;
	}

	/* cheat_know or research_mon() */
	if (cheat_know || (mode & 0x01))
		know_everything = TRUE;

	/* Cheat -- Know everything */
	if (know_everything)
	{
		/* Hack -- maximal drops */
		drop_gold = drop_item =
		(((r_ptr->flags1 & RF1_DROP_4D2) ? 8 : 0) +
		 ((r_ptr->flags1 & RF1_DROP_3D2) ? 6 : 0) +
		 ((r_ptr->flags1 & RF1_DROP_2D2) ? 4 : 0) +
		 ((r_ptr->flags1 & RF1_DROP_1D2) ? 2 : 0) +
		 ((r_ptr->flags1 & RF1_DROP_90)  ? 1 : 0) +
		 ((r_ptr->flags1 & RF1_DROP_60)  ? 1 : 0));

		/* Hack -- but only "valid" drops */
		if (r_ptr->flags1 & RF1_ONLY_GOLD) drop_item = 0;
		if (r_ptr->flags1 & RF1_ONLY_ITEM) drop_gold = 0;

		/* Hack -- know all the flags */
		flags1 = r_ptr->flags1;
		flags2 = r_ptr->flags2;
		flags3 = r_ptr->flags3;
		flags4 = r_ptr->flags4;
		flags5 = r_ptr->flags5;
		flags6 = r_ptr->flags6;
		flagsr = r_ptr->flagsr;
	}


	/* Extract a gender (if applicable) */
	if (r_ptr->flags1 & RF1_FEMALE) msex = 2;
	else if (r_ptr->flags1 & RF1_MALE) msex = 1;

	/* Assume some "obvious" flags */
	if (r_ptr->flags1 & RF1_UNIQUE)  flags1 |= (RF1_UNIQUE);
	if (r_ptr->flags1 & RF1_QUESTOR) flags1 |= (RF1_QUESTOR);
	if (r_ptr->flags1 & RF1_MALE)    flags1 |= (RF1_MALE);
	if (r_ptr->flags1 & RF1_FEMALE)  flags1 |= (RF1_FEMALE);

	/* Assume some "creation" flags */
	if (r_ptr->flags1 & RF1_FRIENDS) flags1 |= (RF1_FRIENDS);
	if (r_ptr->flags1 & RF1_ESCORT)  flags1 |= (RF1_ESCORT);
	if (r_ptr->flags1 & RF1_ESCORTS) flags1 |= (RF1_ESCORTS);

	/* Killing a monster reveals some properties */
	if (r_ptr->r_tkills || know_everything)
	{
		/* Know "race" flags */
		if (r_ptr->flags3 & RF3_ORC)      flags3 |= (RF3_ORC);
		if (r_ptr->flags3 & RF3_TROLL)    flags3 |= (RF3_TROLL);
		if (r_ptr->flags3 & RF3_GIANT)    flags3 |= (RF3_GIANT);
		if (r_ptr->flags3 & RF3_DRAGON)   flags3 |= (RF3_DRAGON);
		if (r_ptr->flags3 & RF3_DEMON)    flags3 |= (RF3_DEMON);
		if (r_ptr->flags3 & RF3_UNDEAD)   flags3 |= (RF3_UNDEAD);
		if (r_ptr->flags3 & RF3_EVIL)     flags3 |= (RF3_EVIL);
		if (r_ptr->flags3 & RF3_GOOD)     flags3 |= (RF3_GOOD);
		if (r_ptr->flags3 & RF3_ANIMAL)   flags3 |= (RF3_ANIMAL);
		if (r_ptr->flags3 & RF3_AMBERITE) flags3 |= (RF3_AMBERITE);
		if (r_ptr->flags2 & RF2_HUMAN)    flags2 |= (RF2_HUMAN);

		/* Know 'quantum' flag */
		if (r_ptr->flags2 & RF2_QUANTUM)  flags2 |= (RF2_QUANTUM);

		/* Know "forced" flags */
		if (r_ptr->flags1 & RF1_FORCE_DEPTH) flags1 |= (RF1_FORCE_DEPTH);
		if (r_ptr->flags1 & RF1_FORCE_MAXHP) flags1 |= (RF1_FORCE_MAXHP);
	}

	/* For output_monster_spoiler() */
	if (mode & 0x02)
	{
		/* Nothing to do */
	}
	else

	/* Treat uniques differently */
	if (flags1 & RF1_UNIQUE)
	{
		/* Hack -- Determine if the unique is "dead" */
		bool dead = (r_ptr->max_num == 0) ? TRUE : FALSE;

		/* We've been killed... */
		if (r_ptr->r_deaths)
		{
			/* Killed ancestors */
			hooked_roff(format(_("%^s�Ϥ��ʤ������Ĥ� %d ����äƤ���", "%^s has slain %d of your ancestors"),
					   wd_he[msex], r_ptr->r_deaths));

			/* But we've also killed it */
			if (dead)
			{
				hooked_roff(format(
					_("�������Ǥ˵�Ƥ���ϲ̤����Ƥ��롪", 
					 (", but you have avenged %s!  ", plural(r_ptr->r_deaths, "him", "them")))));
			}

			/* Unavenged (ever) */
			else
			{
				hooked_roff(format(
					_("�Τˡ��ޤ���Ƥ����̤����Ƥ��ʤ���", 
					 (", who %s unavenged.  ", plural(r_ptr->r_deaths, "remains", "remain")))));
			}

			/* Start a new line */
			hooked_roff("\n");
		}

		/* Dead unique who never hurt us */
		else if (dead)
		{
			hooked_roff(_("���ʤ��Ϥ��ε�Ũ�򤹤Ǥ�����äƤ��롣", "You have slain this foe.  "));

			/* Start a new line */
			hooked_roff("\n");
		}
	}

	/* Not unique, but killed us */
	else if (r_ptr->r_deaths)
	{
		/* Dead ancestors */
		hooked_roff(
			_(format("���Υ�󥹥����Ϥ��ʤ������Ĥ� %d ����äƤ���", r_ptr->r_deaths),
			  format("%d of your ancestors %s been killed by this creature, ", r_ptr->r_deaths, plural(r_ptr->r_deaths, "has", "have"))));

		/* Some kills this life */
		if (r_ptr->r_pkills)
		{
			hooked_roff(format(
				_("�������ʤ��Ϥ��Υ�󥹥����򾯤ʤ��Ȥ� %d �Τ��ݤ��Ƥ��롣", 
				 "and you have exterminated at least %d of the creatures.  "), r_ptr->r_pkills));
		}

		/* Some kills past lives */
		else if (r_ptr->r_tkills)
		{
			hooked_roff(format(
				_("�������ʤ������ĤϤ��Υ�󥹥����򾯤ʤ��Ȥ� %d �Τ��ݤ��Ƥ��롣", 
				  "and your ancestors have exterminated at least %d of the creatures.  "), r_ptr->r_tkills));
		}

		/* No kills */
		else
		{
			hooked_roff(format(
				_("�����ޤ�%s���ݤ������ȤϤʤ���", 
				  "and %s is not ever known to have been defeated.  "), wd_he[msex]));
		}

		/* Start a new line */
		hooked_roff("\n");
	}

	/* Normal monsters */
	else
	{
		/* Killed some this life */
		if (r_ptr->r_pkills)
		{
			hooked_roff(format(
				_("���ʤ��Ϥ��Υ�󥹥����򾯤ʤ��Ȥ� %d �Τϻ����Ƥ��롣",
				  "You have killed at least %d of these creatures.  "), r_ptr->r_pkills));
		}

		/* Killed some last life */
		else if (r_ptr->r_tkills)
		{
			hooked_roff(format(
				_("���ʤ������ĤϤ��Υ�󥹥����򾯤ʤ��Ȥ� %d �Τϻ����Ƥ��롣", 
				  "Your ancestors have killed at least %d of these creatures.  "), r_ptr->r_tkills));
		}

		/* Killed none */
		else
		{
			hooked_roff(_("���Υ�󥹥������ݤ������ȤϤʤ���", "No battles to the death are recalled.  "));
		}

		/* Start a new line */
		hooked_roff("\n");
	}

	/* Descriptions */
	{
		cptr tmp = r_text + r_ptr->text;

		if (tmp[0])
		{
			/* Dump it */
			hooked_roff(tmp);

			/* Start a new line */
			hooked_roff("\n");
		}
	}

	if (r_idx == MON_KAGE)
	{
		/* All done */
		hooked_roff("\n");

		return;
	}

	/* Nothing yet */
	old = FALSE;

	/* Describe location */
	if (r_ptr->level == 0)
	{
		hooked_roff(format(_("%^s��Į�˽���", "%^s lives in the town"), wd_he[msex]));
		old = TRUE;
	}
	else if (r_ptr->r_tkills || know_everything)
	{
		if (depth_in_feet)
		{
			hooked_roff(format(
				_("%^s���̾��ϲ� %d �ե����Ȥǽи���", "%^s is normally found at depths of %d feet"),
				  wd_he[msex], r_ptr->level * 50));
		}
		else
		{
			hooked_roff(format(
				_("%^s���̾��ϲ� %d ���ǽи���", "%^s is normally found on dungeon level %d"),
				  wd_he[msex], r_ptr->level));
		}
		old = TRUE;
	}


	/* Describe movement */
	if (r_idx == MON_CHAMELEON)
	{
		hooked_roff(_("��¾�Υ�󥹥����˲����롣", "and can take the shape of other monster."));
		return;
	}
	else
	{
		/* Introduction */
		if (old)
		{
			hooked_roff(_("��", ", and "));
		}
		else
		{
			hooked_roff(format(_("%^s��", "%^s "), wd_he[msex]));
			old = TRUE;
		}
#ifndef JP
		hooked_roff("moves");
#endif

		/* Random-ness */
		if ((flags1 & RF1_RAND_50) || (flags1 & RF1_RAND_25))
		{
			/* Adverb */
			if ((flags1 & RF1_RAND_50) && (flags1 & RF1_RAND_25))
			{
				hooked_roff(_("���ʤ�", " extremely"));
			}
			else if (flags1 & RF1_RAND_50)
			{
				hooked_roff(_("��ʬ", " somewhat"));
			}
			else if (flags1 & RF1_RAND_25)
			{
				hooked_roff(_("����", " a bit"));
			}

			/* Adjective */
			hooked_roff(_("�Ե�§��", " erratically"));

			/* Hack -- Occasional conjunction */
			if (speed != 110) hooked_roff(_("������", ", and"));
		}

		/* Speed */
		if (speed > 110)
		{
			if (speed > 139) hook_c_roff(TERM_RED, _("�����񤤤ۤ�", " incredibly"));
			else if (speed > 134) hook_c_roff(TERM_ORANGE, _("������", " extremely"));
			else if (speed > 129) hook_c_roff(TERM_ORANGE, _("����", " very"));
			else if (speed > 124) hook_c_roff(TERM_UMBER, _("���ʤ�", " fairly"));
			else if (speed < 120) hook_c_roff(TERM_L_UMBER, _("���", " somewhat"));
			hook_c_roff(TERM_L_RED, _("���᤯", " quickly"));
		}
		else if (speed < 110)
		{
			if (speed < 90) hook_c_roff(TERM_L_GREEN, _("�����񤤤ۤ�", " incredibly"));
			else if (speed < 95) hook_c_roff(TERM_BLUE, _("����", " very"));
			else if (speed < 100) hook_c_roff(TERM_BLUE, _("���ʤ�", " fairly"));
			else if (speed > 104) hook_c_roff(TERM_GREEN, _("���", " somewhat"));
			hook_c_roff(TERM_L_BLUE, _("��ä����", " slowly"));
		}
		else
		{
			hooked_roff(_("���̤�®����", " at normal speed"));
		}
#ifdef JP
		hooked_roff("ư���Ƥ���");
#endif
	}

	/* The code above includes "attack speed" */
	if (flags1 & RF1_NEVER_MOVE)
	{
		/* Introduce */
		if (old)
		{
			hooked_roff(_("��������", ", but "));
		}
		else
		{
			hooked_roff(format(_("%^s��", "%^s "), wd_he[msex]));
			old = TRUE;
		}

		/* Describe */
		hooked_roff(_("�����Ԥ����פ��ʤ�", "does not deign to chase intruders"));
	}

	/* End this sentence */
	if (old)
	{
		hooked_roff(_("��", ".  "));
		old = FALSE;
	}


	/* Describe experience if known */
	if (r_ptr->r_tkills || know_everything)
	{
		/* Introduction */
#ifdef JP
		hooked_roff("����");
#else
		if (flags1 & RF1_UNIQUE)
		{
			hooked_roff("Killing this");
		}
		else
		{
			hooked_roff("A kill of this");
		}
#endif


		/* Describe the "quality" */
		if (flags2 & RF2_ELDRITCH_HORROR) hook_c_roff(TERM_VIOLET, _("������Ͷ��", " sanity-blasting"));/*nuke me*/
		if (flags3 & RF3_ANIMAL)          hook_c_roff(TERM_L_GREEN, _("��������", " natural"));
		if (flags3 & RF3_EVIL)            hook_c_roff(TERM_L_DARK, _("�ٰ��ʤ�", " evil"));
		if (flags3 & RF3_GOOD)            hook_c_roff(TERM_YELLOW, _("���ɤ�", " good"));
		if (flags3 & RF3_UNDEAD)          hook_c_roff(TERM_VIOLET, _("����ǥåɤ�", " undead"));
		if (flags3 & RF3_AMBERITE)        hook_c_roff(TERM_VIOLET, _("����С��β�²��", " Amberite"));

		if ((flags3 & (RF3_DRAGON | RF3_DEMON | RF3_GIANT | RF3_TROLL | RF3_ORC)) || (flags2 & (RF2_QUANTUM | RF2_HUMAN)))
		{
		/* Describe the "race" */
			if (flags3 & RF3_DRAGON)   hook_c_roff(TERM_ORANGE, _("�ɥ饴��", " dragon"));
			if (flags3 & RF3_DEMON)    hook_c_roff(TERM_VIOLET, _("�ǡ����", " demon"));
			if (flags3 & RF3_GIANT)    hook_c_roff(TERM_L_UMBER, _("���㥤�����", " giant"));
			if (flags3 & RF3_TROLL)    hook_c_roff(TERM_BLUE, _("�ȥ��", " troll"));
			if (flags3 & RF3_ORC)      hook_c_roff(TERM_UMBER, _("������", " orc"));
			if (flags2 & RF2_HUMAN)    hook_c_roff(TERM_L_WHITE, _("�ʹ�", " human"));
			if (flags2 & RF2_QUANTUM)  hook_c_roff(TERM_VIOLET, _("�̻���ʪ", " quantum creature"));
		}
		else
		{
			hooked_roff(_("��󥹥���", " creature"));
		}

#ifdef JP
		hooked_roff("���ݤ����Ȥ�");
#endif
		/* Group some variables */
		{
			long i, j;

#ifdef JP
			i = p_ptr->lev;
			hooked_roff(format(" %lu ��٥�Υ���饯���ˤȤä�", (long)i));

			i = (long)r_ptr->mexp * r_ptr->level / (p_ptr->max_plv+2);
			j = ((((long)r_ptr->mexp * r_ptr->level % (p_ptr->max_plv+2)) *
			       (long)1000 / (p_ptr->max_plv+2) + 5) / 10);

			hooked_roff(format(" ��%ld.%02ld �ݥ���Ȥηи��Ȥʤ롣",
				(long)i, (long)j ));
#else
			/* calculate the integer exp part */
			i = (long)r_ptr->mexp * r_ptr->level / (p_ptr->max_plv+2);

			/* calculate the fractional exp part scaled by 100, */
			/* must use long arithmetic to avoid overflow  */
			j = ((((long)r_ptr->mexp * r_ptr->level % (p_ptr->max_plv+2)) *
			       (long)1000 / (p_ptr->max_plv+2) + 5) / 10);

			/* Mention the experience */
			hooked_roff(format(" is worth about %ld.%02ld point%s",
				    (long)i, (long)j,
				    (((i == 1) && (j == 0)) ? "" : "s")));

			/* Take account of annoying English */
			p = "th";
			i = p_ptr->lev % 10;
			if ((p_ptr->lev / 10) == 1) /* nothing */;
			else if (i == 1) p = "st";
			else if (i == 2) p = "nd";
			else if (i == 3) p = "rd";

			/* Take account of "leading vowels" in numbers */
			q = "";
			i = p_ptr->lev;
			if ((i == 8) || (i == 11) || (i == 18)) q = "n";

			/* Mention the dependance on the player's level */
			hooked_roff(format(" for a%s %lu%s level character.  ",
				    q, (long)i, p));
#endif

		}
	}

	if ((flags2 & RF2_AURA_FIRE) && (flags2 & RF2_AURA_ELEC) && (flags3 & RF3_AURA_COLD))
	{
		hook_c_roff(TERM_VIOLET, format(
			_("%^s�ϱ��ɹ�ȥ��ѡ�������ޤ�Ƥ��롣", "%^s is surrounded by flames, ice and electricity.  "), wd_he[msex]));
	}
	else if ((flags2 & RF2_AURA_FIRE) && (flags2 & RF2_AURA_ELEC))
	{
		hook_c_roff(TERM_L_RED, format(
			_("%^s�ϱ�ȥ��ѡ�������ޤ�Ƥ��롣", "%^s is surrounded by flames and electricity.  "), wd_he[msex]));
	}
	else if ((flags2 & RF2_AURA_FIRE) && (flags3 & RF3_AURA_COLD))
	{
		hook_c_roff(TERM_BLUE, format(
			_("%^s�ϱ��ɹ����ޤ�Ƥ��롣", "%^s is surrounded by flames and ice.  "), wd_he[msex]));
	}
	else if ((flags3 & RF3_AURA_COLD) && (flags2 & RF2_AURA_ELEC))
	{
		hook_c_roff(TERM_L_GREEN, format(
			_("%^s��ɹ�ȥ��ѡ�������ޤ�Ƥ��롣", "%^s is surrounded by ice and electricity.  "), wd_he[msex]));
	}
	else if (flags2 & RF2_AURA_FIRE)
	{
		hook_c_roff(TERM_RED, format(
			_("%^s�ϱ����ޤ�Ƥ��롣", "%^s is surrounded by flames.  "), wd_he[msex]));
	}
	else if (flags3 & RF3_AURA_COLD)
	{
		hook_c_roff(TERM_BLUE, format(
			_("%^s��ɹ����ޤ�Ƥ��롣", "%^s is surrounded by ice.  "), wd_he[msex]));
	}
	else if (flags2 & RF2_AURA_ELEC)
	{
		hook_c_roff(TERM_L_BLUE, format(
			_("%^s�ϥ��ѡ�������ޤ�Ƥ��롣", "%^s is surrounded by electricity.  "), wd_he[msex]));
	}

	if (flags2 & RF2_REFLECTING)
		hooked_roff(format(_("%^s����μ�ʸ��ķ���֤���", "%^s reflects bolt spells.  "), wd_he[msex]));

	/* Describe escorts */
	if ((flags1 & RF1_ESCORT) || (flags1 & RF1_ESCORTS) || reinforce)
	{
		hooked_roff(format(
			_("%^s���̾��Ҥ�ȼ�äƸ���롣", "%^s usually appears with escorts.  "), wd_he[msex]));

		if(reinforce)
		{
			hooked_roff(_("��Ҥι�����", "These escorts"));
			if((flags1 & RF1_ESCORT) || (flags1 & RF1_ESCORTS))
			{
				hooked_roff(_("���ʤ��Ȥ�", " at the least"));
			}
#ifndef JP
			hooked_roff(" contain ");
#endif			
			for(n = 0; n < 6; n++)
			{
				if(r_ptr->reinforce_id[n] && r_ptr->reinforce_dd[n] && r_ptr->reinforce_ds[n])
				{
					monster_race *rf_ptr = &r_info[r_ptr->reinforce_id[n]];
					if(rf_ptr->flags1 & RF1_UNIQUE)
					{
						hooked_roff(format(_("��%s", ", %s"), r_name + rf_ptr->name));
					}
					else
					{
#ifdef JP
						hooked_roff(format("�� %dd%d �Τ�%s", r_ptr->reinforce_dd[n], r_ptr->reinforce_ds[n],
							r_name + rf_ptr->name));
#else
						bool plural = (r_ptr->reinforce_dd[n] * r_ptr->reinforce_ds[n] > 1);
						char name[80];
						strcpy(name, r_name + rf_ptr->name);
						if(plural) plural_aux(name);
						hooked_roff(format(",%dd%d %s", r_ptr->reinforce_dd[n], r_ptr->reinforce_ds[n], name));
#endif
					}
				}
			}
			hooked_roff(_("������Ω�äƤ��롣", "."));
		}
	}

	/* Describe friends */
	else if (flags1 & RF1_FRIENDS)
	{
		hooked_roff(format(_("%^s���̾ｸ�ĤǸ���롣", "%^s usually appears in groups.  "), wd_he[msex]));
	}


	/* Collect inate attacks */
	vn = 0;
	if (flags4 & RF4_SHRIEK)  { vp[vn] = _("���Ĥǽ��������", "shriek for help"); color[vn++] = TERM_L_WHITE; }
	if (flags4 & RF4_ROCKET)  
    {
		set_damage(r_idx, (MS_ROCKET), _("���å�%s��ȯ�ͤ���", "shoot a rocket%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
        color[vn++] = TERM_UMBER; 
    }
    
	if (flags4 & RF4_SHOOT)
	{ 
		for (r = 0, m = 0; m < 4; m++)
		{
			if (r_ptr->blow[m].method == RBM_SHOOT)
            {
                if (know_armour(r_idx))
				    sprintf(tmp_msg[vn], _("���� %dd%d �μͷ�򤹤�","fire an arrow (Power:%dd%d)"), r_ptr->blow[m].d_side, r_ptr->blow[m].d_dice);
                else
                    sprintf(tmp_msg[vn], _("�ͷ�򤹤�", "fire an arrow"));
                vp[vn] = tmp_msg[vn]; color[vn++] = TERM_UMBER;
				break;
			}
		}		
	}
	if (flags6 & (RF6_SPECIAL)) { vp[vn] = _("���̤ʹ�ư�򤹤�", "do something"); color[vn++] = TERM_VIOLET; }

	/* Describe inate attacks */
	if (vn)
	{
		/* Intro */
		hooked_roff(format(_("%^s��", "%^s"), wd_he[msex]));


		/* Scan */
		for (n = 0; n < vn; n++)
		{
#ifdef JP
			if (n != vn - 1)
			{
				jverb(vp[n], jverb_buf, JVERB_OR);
				hook_c_roff(color[n], jverb_buf);
				hook_c_roff(color[n], "��");
				hooked_roff("��");
			}
			else hook_c_roff(color[n], vp[n]);
#else
			/* Intro */
			if (n == 0) hooked_roff(" may ");
			else if (n < vn - 1) hooked_roff(", ");
			else hooked_roff(" or ");

			/* Dump */
			hook_c_roff(color[n], vp[n]);
#endif

		}

		/* End */
		hooked_roff(_("���Ȥ����롣", ".  "));
	}


	/* Collect breaths */
	vn = 0;
	if (flags4 & (RF4_BR_ACID))		
	{ 
		set_damage(r_idx, (MS_BR_ACID), _("��%s", "acid%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_GREEN; 
	}
	if (flags4 & (RF4_BR_ELEC))		
	{ 
		set_damage(r_idx, (MS_BR_ELEC), _("���%s", "lightning%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_BLUE; 
	}
	if (flags4 & (RF4_BR_FIRE))		
	{ 
		set_damage(r_idx, (MS_BR_FIRE), _("�б�%s", "fire%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_RED; 
	}
	if (flags4 & (RF4_BR_COLD))		
	{ 
		set_damage(r_idx, (MS_BR_COLD), _("�䵤%s", "frost%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_WHITE; 
	}
	if (flags4 & (RF4_BR_POIS))		
	{ 
		set_damage(r_idx, (MS_BR_POIS), _("��%s", "poison%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_GREEN; 
	}
	if (flags4 & (RF4_BR_NETH))
	{ 
		set_damage(r_idx, (MS_BR_NETHER), _("�Ϲ�%s", "nether%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_DARK; 
	}
	if (flags4 & (RF4_BR_LITE))		
	{ 
		set_damage(r_idx, (MS_BR_LITE), _("����%s", "light%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_YELLOW; 
	}
	if (flags4 & (RF4_BR_DARK))		
	{ 
		set_damage(r_idx, (MS_BR_DARK), _("�Ź�%s", "darkness%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_DARK; 
	}
	if (flags4 & (RF4_BR_CONF))
	{ 
		set_damage(r_idx, (MS_BR_CONF), _("����%s", "confusion%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_UMBER; 
	}
	if (flags4 & (RF4_BR_SOUN))		
	{
		set_damage(r_idx, (MS_BR_SOUND), _("�첻%s", "sound%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_ORANGE; 
	}
	if (flags4 & (RF4_BR_CHAO))		
	{ 
		set_damage(r_idx, (MS_BR_CHAOS), _("������%s", "chaos%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_VIOLET; 
	}
	if (flags4 & (RF4_BR_DISE))		
	{ 
		set_damage(r_idx, (MS_BR_DISEN), _("����%s", "disenchantment%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_VIOLET; 
	}
	if (flags4 & (RF4_BR_NEXU))		
	{ 
		set_damage(r_idx, (MS_BR_NEXUS), _("���̺���%s", "nexus%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_VIOLET; 
	}
	if (flags4 & (RF4_BR_TIME))		
	{ 
		set_damage(r_idx, (MS_BR_TIME), _("���ֵ�ž%s", "time%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_BLUE; 
	}
	if (flags4 & (RF4_BR_INER))		
	{ 
		set_damage(r_idx, (MS_BR_INERTIA), _("����%s", "inertia%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_SLATE; 
	}
	if (flags4 & (RF4_BR_GRAV))		
	{ 
		set_damage(r_idx, (MS_BR_GRAVITY), _("����%s", "gravity%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_SLATE; 
	}
	if (flags4 & (RF4_BR_SHAR))		
	{ 
		set_damage(r_idx, (MS_BR_SHARDS), _("����%s", "shards%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_UMBER; 
	}
	if (flags4 & (RF4_BR_PLAS))		
	{ 
		set_damage(r_idx, (MS_BR_PLASMA), _("�ץ饺��%s", "plasma%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_RED; 
	}
	if (flags4 & (RF4_BR_WALL))		
	{ 
		set_damage(r_idx, (MS_BR_FORCE), _("�ե�����%s", "force%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_UMBER; 
	}
	if (flags4 & (RF4_BR_MANA))		
	{ 
		set_damage(r_idx, (MS_BR_MANA), _("����%s", "mana%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_BLUE; 
	}
	if (flags4 & (RF4_BR_NUKE))		
	{ 
		set_damage(r_idx, (MS_BR_NUKE), _("�������Ѵ�ʪ%s", "toxic waste%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_GREEN; 
	}
	if (flags4 & (RF4_BR_DISI))		
	{ 
		set_damage(r_idx, (MS_BR_DISI), _("ʬ��%s", "disintegration%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_SLATE; 
	}

	/* Describe breaths */
	if (vn)
	{
		/* Note breath */
		breath = TRUE;

		/* Intro */
		hooked_roff(format(_("%^s��", "%^s"), wd_he[msex]));

		/* Scan */
		for (n = 0; n < vn; n++)
		{
			/* Intro */
#ifdef JP
			if ( n != 0 ) hooked_roff("��");
#else
			if (n == 0) hooked_roff(" may breathe ");
			else if (n < vn-1) hooked_roff(", ");
			else hooked_roff(" or ");
#endif


			/* Dump */
			hook_c_roff(color[n], vp[n]);
		}
#ifdef JP
		hooked_roff("�Υ֥쥹���Ǥ����Ȥ�����");
#endif
	}


	/* Collect spells */
	vn = 0;
	if (flags5 & (RF5_BA_ACID))         
	{
		set_damage(r_idx, (MS_BALL_ACID), _("�����åɡ��ܡ���%s", "produce acid balls%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_GREEN;
	}
	if (flags5 & (RF5_BA_ELEC))         
	{
		set_damage(r_idx, (MS_BALL_ELEC), _("����������ܡ���%s", "produce lightning balls%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_BLUE;
	}
	if (flags5 & (RF5_BA_FIRE))         
	{
		set_damage(r_idx, (MS_BALL_FIRE), _("�ե��������ܡ���%s", "produce fire balls%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_RED;
	}
	if (flags5 & (RF5_BA_COLD))         
	{
		set_damage(r_idx, (MS_BALL_COLD), _("���������ܡ���%s", "produce frost balls%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_WHITE;
	}
	if (flags5 & (RF5_BA_POIS))         
	{
		set_damage(r_idx, (MS_BALL_POIS), _("������%s", "produce poison balls%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_GREEN;
	}
	if (flags5 & (RF5_BA_NETH))         
	{
		set_damage(r_idx, (MS_BALL_NETHER), _("�Ϲ���%s", "produce nether balls%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_DARK;
	}
	if (flags5 & (RF5_BA_WATE))         
	{
		set_damage(r_idx, (MS_BALL_WATER), _("�������������ܡ���%s", "produce water balls%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_BLUE;
	}
	if (flags4 & (RF4_BA_NUKE))         
	{
		set_damage(r_idx, (MS_BALL_NUKE), _("����ǽ��%s", "produce balls of radiation%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_GREEN;
	}
	if (flags5 & (RF5_BA_MANA))         
	{
		set_damage(r_idx, (MS_BALL_MANA), _("���Ϥ���%s", "invoke mana storms%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_BLUE;
	}
	if (flags5 & (RF5_BA_DARK))         
	{
		set_damage(r_idx, (MS_BALL_DARK), _("�Ź�����%s", "invoke darkness storms%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_DARK;
	}
	if (flags5 & (RF5_BA_LITE))         
	{
		set_damage(r_idx, (MS_STARBURST), _("�������С�����%s", "invoke starburst%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_YELLOW;
	}
	if (flags4 & (RF4_BA_CHAO))         
	{
		set_damage(r_idx, (MS_BALL_CHAOS), _("����륹%s", "invoke raw Logrus%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_VIOLET;
	}
	if (flags6 & (RF6_HAND_DOOM)){ vp[vn] = _("���Ǥμ�(40%-60%)", "invoke the Hand of Doom(40%-60%)"); color[vn++] = TERM_VIOLET; }
	if (flags6 & (RF6_PSY_SPEAR))
	{
		set_damage(r_idx, (MS_PSY_SPEAR), _("���η�%s", "psycho-spear%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_YELLOW;
	}
	if (flags5 & (RF5_DRAIN_MANA))
	{
		set_damage(r_idx, (MS_DRAIN_MANA), _("���ϵۼ�%s", "drain mana%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_SLATE;
	}
	if (flags5 & (RF5_MIND_BLAST))         
	{
		set_damage(r_idx, (MS_MIND_BLAST), _("��������%s", "cause mind blasting%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_RED;
	}
	if (flags5 & (RF5_BRAIN_SMASH))         
	{
		set_damage(r_idx, (MS_BRAIN_SMASH), _("Ǿ����%s", "cause brain smashing%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_RED;
	}
	if (flags5 & (RF5_CAUSE_1))         
	{
		set_damage(r_idx, (MS_CAUSE_1), 
			_("�ڽ��ܼ���%s", "cause light wounds and cursing%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_WHITE;
	}
	if (flags5 & (RF5_CAUSE_2))         
	{
		set_damage(r_idx, (MS_CAUSE_2), 
			_("�Ž��ܼ���%s", "cause serious wounds and cursing%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_WHITE;
	}
	if (flags5 & (RF5_CAUSE_3))         
	{
		set_damage(r_idx, (MS_CAUSE_3), 
			_("��̿���ܼ���%s", "cause critical wounds and cursing%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_WHITE;
	}
	if (flags5 & (RF5_CAUSE_4))         
	{
		set_damage(r_idx, (MS_CAUSE_4), 
			_("�빦���ͤ�%s", "cause mortal wounds%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_WHITE;
	}
	if (flags5 & (RF5_BO_ACID))         
	{
		set_damage(r_idx, (MS_BOLT_ACID), _("�����åɡ��ܥ��%s", "produce acid bolts%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_GREEN;
	}
	if (flags5 & (RF5_BO_ELEC))         
	{
		set_damage(r_idx, (MS_BOLT_ELEC), _("����������ܥ��%s", "produce lightning bolts%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_BLUE;
	}
	if (flags5 & (RF5_BO_FIRE))         
	{
		set_damage(r_idx, (MS_BOLT_FIRE), _("�ե��������ܥ��%s", "produce fire bolts%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_RED;
	}
	if (flags5 & (RF5_BO_COLD))         
	{
		set_damage(r_idx, (MS_BOLT_COLD), _("���������ܥ��%s", "produce frost bolts%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_WHITE;
	}
	if (flags5 & (RF5_BO_NETH))         
	{
		set_damage(r_idx, (MS_BOLT_NETHER), _("�Ϲ�����%s", "produce nether bolts%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_DARK;
	}
	if (flags5 & (RF5_BO_WATE))         
	{
		set_damage(r_idx, (MS_BOLT_WATER), _("�������������ܥ��%s", "produce water bolts%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_BLUE;
	}
	if (flags5 & (RF5_BO_MANA))         
	{
		set_damage(r_idx, (MS_BOLT_MANA), _("���Ϥ���%s", "produce mana bolts%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_BLUE;
	}
	if (flags5 & (RF5_BO_PLAS))         
	{
		set_damage(r_idx, (MS_BOLT_PLASMA), _("�ץ饺�ޡ��ܥ��%s", "produce plasma bolts%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_L_RED;
	}
	if (flags5 & (RF5_BO_ICEE))         
	{
		set_damage(r_idx, (MS_BOLT_ICE), _("�˴�����%s", "produce ice bolts%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_WHITE;
	}
	if (flags5 & (RF5_MISSILE))         
	{
		set_damage(r_idx, (MS_MAGIC_MISSILE), _("�ޥ��å��ߥ�����%s", "produce magic missiles%s"), tmp_msg[vn]);
        vp[vn] = tmp_msg[vn];
		color[vn++] = TERM_SLATE;
	}
	if (flags5 & (RF5_SCARE))           { vp[vn] = _("����", "terrify"); color[vn++] = TERM_SLATE; }
	if (flags5 & (RF5_BLIND))           { vp[vn] = _("�ܤ���ޤ�", "blind"); color[vn++] = TERM_L_DARK; }
	if (flags5 & (RF5_CONF))            { vp[vn] = _("����", "confuse"); color[vn++] = TERM_L_UMBER; }
	if (flags5 & (RF5_SLOW))            { vp[vn] = _("��®", "slow"); color[vn++] = TERM_UMBER; }
	if (flags5 & (RF5_HOLD))            { vp[vn] = _("����", "paralyze"); color[vn++] = TERM_RED; }
	if (flags6 & (RF6_HASTE))           { vp[vn] = _("��®", "haste-self"); color[vn++] = TERM_L_GREEN; }
	if (flags6 & (RF6_HEAL))            { vp[vn] = _("����", "heal-self"); color[vn++] = TERM_WHITE; }
	if (flags6 & (RF6_INVULNER))        { vp[vn] = _("̵Ũ��", "make invulnerable"); color[vn++] = TERM_WHITE; }
	if (flags4 & RF4_DISPEL)            { vp[vn] = _("���Ͼõ�", "dispel-magic"); color[vn++] = TERM_L_WHITE; }
	if (flags6 & (RF6_BLINK))           { vp[vn] = _("���硼�ȥƥ�ݡ���", "blink-self"); color[vn++] = TERM_UMBER; }
	if (flags6 & (RF6_TPORT))           { vp[vn] = _("�ƥ�ݡ���", "teleport-self"); color[vn++] = TERM_ORANGE; }
	if (flags6 & (RF6_WORLD))           { vp[vn] = _("����ߤ��", "stop the time"); color[vn++] = TERM_L_BLUE; }
	if (flags6 & (RF6_TELE_TO))         { vp[vn] = _("�ƥ�ݡ��ȥХå�", "teleport to"); color[vn++] = TERM_L_UMBER; }
	if (flags6 & (RF6_TELE_AWAY))       { vp[vn] = _("�ƥ�ݡ��ȥ�������", "teleport away"); color[vn++] = TERM_UMBER; }
	if (flags6 & (RF6_TELE_LEVEL))      { vp[vn] = _("�ƥ�ݡ��ȡ���٥�", "teleport level"); color[vn++] = TERM_ORANGE; }

	if (flags6 & (RF6_DARKNESS))
	{
		if ((p_ptr->pclass != CLASS_NINJA) || (r_ptr->flags3 & (RF3_UNDEAD | RF3_HURT_LITE)) || (r_ptr->flags7 & RF7_DARK_MASK))
		{
			vp[vn] = _("�Ű�", "create darkness"); color[vn++] = TERM_L_DARK;
		}
		else
		{
			vp[vn] = _("����", "create light"); color[vn++] = TERM_YELLOW;
		}
	}

	if (flags6 & (RF6_TRAPS))           { vp[vn] = _("�ȥ�å�", "create traps"); color[vn++] = TERM_BLUE; }
	if (flags6 & (RF6_FORGET))          { vp[vn] = _("�����õ�", "cause amnesia"); color[vn++] = TERM_BLUE; }
	if (flags6 & (RF6_RAISE_DEAD))      { vp[vn] = _("�������", "raise dead"); color[vn++] = TERM_RED; }
	if (flags6 & (RF6_S_MONSTER))       { vp[vn] = _("��󥹥������ξ���", "summon a monster"); color[vn++] = TERM_SLATE; }
	if (flags6 & (RF6_S_MONSTERS))      { vp[vn] = _("��󥹥���ʣ������", "summon monsters"); color[vn++] = TERM_L_WHITE; }
	if (flags6 & (RF6_S_KIN))           { vp[vn] = _("�߱群��", "summon aid"); color[vn++] = TERM_ORANGE; }
	if (flags6 & (RF6_S_ANT))           { vp[vn] = _("���꾤��", "summon ants"); color[vn++] = TERM_RED; }
	if (flags6 & (RF6_S_SPIDER))        { vp[vn] = _("���⾤��", "summon spiders"); color[vn++] = TERM_L_DARK; }
	if (flags6 & (RF6_S_HOUND))         { vp[vn] = _("�ϥ���ɾ���", "summon hounds"); color[vn++] = TERM_L_UMBER; }
	if (flags6 & (RF6_S_HYDRA))         { vp[vn] = _("�ҥɥ龤��", "summon hydras"); color[vn++] = TERM_L_GREEN; }
	if (flags6 & (RF6_S_ANGEL))         { vp[vn] = _("ŷ�Ȱ��ξ���", "summon an angel"); color[vn++] = TERM_YELLOW; }
	if (flags6 & (RF6_S_DEMON))         { vp[vn] = _("�ǡ������ξ���", "summon a demon"); color[vn++] = TERM_L_RED; }
	if (flags6 & (RF6_S_UNDEAD))        { vp[vn] = _("����ǥåɰ��ξ���", "summon an undead"); color[vn++] = TERM_L_DARK; }
	if (flags6 & (RF6_S_DRAGON))        { vp[vn] = _("�ɥ饴����ξ���", "summon a dragon"); color[vn++] = TERM_ORANGE; }
	if (flags6 & (RF6_S_HI_UNDEAD))     { vp[vn] = _("���Ϥʥ���ǥåɾ���", "summon Greater Undead"); color[vn++] = TERM_L_DARK; }
	if (flags6 & (RF6_S_HI_DRAGON))     { vp[vn] = _("����ɥ饴�󾤴�", "summon Ancient Dragons"); color[vn++] = TERM_ORANGE; }	
	if (flags6 & (RF6_S_CYBER))         { vp[vn] = _("�����С��ǡ���󾤴�", "summon Cyberdemons"); color[vn++] = TERM_UMBER; }
	if (flags6 & (RF6_S_AMBERITES))     { vp[vn] = _("����С��β�²����", "summon Lords of Amber"); color[vn++] = TERM_VIOLET; }
	if (flags6 & (RF6_S_UNIQUE))        { vp[vn] = _("��ˡ�������󥹥�������", "summon Unique Monsters"); color[vn++] = TERM_VIOLET; }


	/* Describe spells */
	if (vn)
	{
		/* Note magic */
		magic = TRUE;

		/* Intro */
		if (breath)
		{
			hooked_roff(_("���ʤ�����", ", and is also"));
		}
		else
		{
			hooked_roff(format(_("%^s��", "%^s is"), wd_he[msex]));
		}

#ifdef JP
		/* Adverb */
		if (flags2 & (RF2_SMART)) hook_c_roff(TERM_YELLOW, "Ū�Τ�");

		/* Verb Phrase */
		hooked_roff("��ˡ��Ȥ����Ȥ��Ǥ���");
#else
		/* Verb Phrase */
		hooked_roff(" magical, casting spells");

		/* Adverb */
		if (flags2 & RF2_SMART) hook_c_roff(TERM_YELLOW, " intelligently");
#endif


		/* Scan */
		for (n = 0; n < vn; n++)
		{
			/* Intro */
#ifdef JP
			if ( n != 0 ) hooked_roff("��");
#else
			if (n == 0) hooked_roff(" which ");
			else if (n < vn-1) hooked_roff(", ");
			else hooked_roff(" or ");
#endif


			/* Dump */
			hook_c_roff(color[n], vp[n]);
		}
#ifdef JP
		hooked_roff("�μ�ʸ�򾧤��뤳�Ȥ�����");
#endif
	}


	/* End the sentence about inate/other spells */
	if (breath || magic)
	{
		/* Total casting */
		m = r_ptr->r_cast_spell;

		/* Average frequency */
		n = r_ptr->freq_spell;

		/* Describe the spell frequency */
		if (m > 100 || know_everything)
		{
			hooked_roff(format(
				_("(��Ψ:1/%d)", "; 1 time in %d"), 100 / n));
		}

		/* Guess at the frequency */
		else if (m)
		{
			n = ((n + 9) / 10) * 10;
			hooked_roff(format(
				_("(��Ψ:��1/%d)", "; about 1 time in %d"), 100 / n));
		}

		/* End this sentence */
		hooked_roff(_("��", ".  "));
	}

	/* Describe monster "toughness" */
    if (know_everything || know_armour(r_idx))
	{
		/* Armor */
		hooked_roff(format(
			_("%^s�� AC%d ���ɸ��Ϥ�", "%^s has an armor rating of %d"),
			    wd_he[msex], r_ptr->ac));

		/* Maximized hitpoints */
		if ((flags1 & RF1_FORCE_MAXHP) || (r_ptr->hside == 1))
		{
			u32b hp = r_ptr->hdice * (nightmare ? 2 : 1) * r_ptr->hside;
			hooked_roff(format(
				_(" %d �����Ϥ����롣", " and a life rating of %d.  "),
				    (s16b)MIN(30000, hp)));
		}

		/* Variable hitpoints */
		else
		{
			hooked_roff(format(
				_(" %dd%d �����Ϥ����롣", " and a life rating of %dd%d.  "),
				    r_ptr->hdice * (nightmare ? 2 : 1), r_ptr->hside));
		}
	}



	/* Collect special abilities. */
	vn = 0;
	if (flags7 & (RF7_HAS_LITE_1 | RF7_HAS_LITE_2)) { vp[vn] = _("���󥸥���Ȥ餹", "illuminate the dungeon");     color[vn++] = TERM_WHITE; }
	if (flags7 & (RF7_HAS_DARK_1 | RF7_HAS_DARK_2)) { vp[vn] = _("���󥸥���Ť�����", "darken the dungeon");   color[vn++] = TERM_L_DARK; }
	if (flags2 & RF2_OPEN_DOOR) { vp[vn] = _("�ɥ��򳫤���", "open doors"); color[vn++] = TERM_WHITE; }
	if (flags2 & RF2_BASH_DOOR) { vp[vn] = _("�ɥ����Ǥ��ˤ�", "bash down doors"); color[vn++] = TERM_WHITE; }
	if (flags7 & RF7_CAN_FLY)  { vp[vn] = _("��������", "fly"); color[vn++] = TERM_WHITE; }
	if (flags7 & RF7_CAN_SWIM)   { vp[vn] = _("����Ϥ�", "swim"); color[vn++] = TERM_WHITE; }
	if (flags2 & RF2_PASS_WALL) { vp[vn] = _("�ɤ򤹤�ȴ����", "pass through walls"); color[vn++] = TERM_WHITE; }
	if (flags2 & RF2_KILL_WALL) { vp[vn] = _("�ɤ򷡤�ʤ�", "bore through walls"); color[vn++] = TERM_WHITE; }
	if (flags2 & RF2_MOVE_BODY) { vp[vn] = _("�夤��󥹥����򲡤��Τ���", "push past weaker monsters"); color[vn++] = TERM_WHITE; }
	if (flags2 & RF2_KILL_BODY) { vp[vn] = _("�夤��󥹥������ݤ�", "destroy weaker monsters"); color[vn++] = TERM_WHITE; }
	if (flags2 & RF2_TAKE_ITEM) { vp[vn] = _("�����ƥ�򽦤�", "pick up objects"); color[vn++] = TERM_WHITE; }
	if (flags2 & RF2_KILL_ITEM) { vp[vn] = _("�����ƥ�����", "destroy objects"); color[vn++] = TERM_WHITE; }


	/* Describe special abilities. */
	if (vn)
	{
		/* Intro */
		hooked_roff(format(_("%^s��", "%^s"), wd_he[msex]));

		/* Scan */
		for (n = 0; n < vn; n++)
		{
			/* Intro */
#ifdef JP
			if (n != vn - 1)
			{
				jverb(vp[n], jverb_buf, JVERB_AND);
				hook_c_roff(color[n], jverb_buf);
				hooked_roff("��");
			}
			else hook_c_roff(color[n], vp[n]);
#else
			if (n == 0) hooked_roff(" can ");
			else if (n < vn - 1) hooked_roff(", ");
			else hooked_roff(" and ");

			/* Dump */
			hook_c_roff(color[n], vp[n]);
#endif

		}

		/* End */
		hooked_roff(_("���Ȥ��Ǥ��롣", ".  "));

	}
	
	/* Aquatic */
	if (flags7 & RF7_AQUATIC)
	{
		hooked_roff(format(_("%^s�Ͽ��������Ǥ��롣", "%^s lives in water.  "), wd_he[msex]));
	}

	/* Describe special abilities. */
	if (flags7 & (RF7_SELF_LITE_1 | RF7_SELF_LITE_2))
	{
		hooked_roff(format(_("%^s�ϸ��äƤ��롣", "%^s is shining.  "), wd_he[msex]));
	}
	if (flags7 & (RF7_SELF_DARK_1 | RF7_SELF_DARK_2))
	{
		hook_c_roff(TERM_L_DARK, format(_("%^s�ϰŹ�����ޤ�Ƥ��롣", "%^s is surrounded by darkness.  "), wd_he[msex]));
	}
	if (flags2 & RF2_INVISIBLE)
	{
		hooked_roff(format(_("%^s��Ʃ�����ܤ˸����ʤ���", "%^s is invisible.  "), wd_he[msex]));
	}
	if (flags2 & RF2_COLD_BLOOD)
	{
		hooked_roff(format(_("%^s�����ưʪ�Ǥ��롣", "%^s is cold blooded.  "), wd_he[msex]));
	}
	if (flags2 & RF2_EMPTY_MIND)
	{
		hooked_roff(format(_("%^s�ϥƥ�ѥ����Ǥϴ��ΤǤ��ʤ���", "%^s is not detected by telepathy.  "), wd_he[msex]));
	}
	else if (flags2 & RF2_WEIRD_MIND)
	{
		hooked_roff(format(_("%^s�Ϥޤ�˥ƥ�ѥ����Ǵ��ΤǤ��롣", "%^s is rarely detected by telepathy.  "), wd_he[msex]));
	}
	if (flags2 & RF2_MULTIPLY)
	{
		hook_c_roff(TERM_L_UMBER, format(_("%^s����ȯŪ���������롣", "%^s breeds explosively.  "), wd_he[msex]));
	}
	if (flags2 & RF2_REGENERATE)
	{
		hook_c_roff(TERM_L_WHITE, format(_("%^s�����᤯���Ϥ�������롣", "%^s regenerates quickly.  "), wd_he[msex]));
	}
	if (flags7 & RF7_RIDING)
	{
		hook_c_roff(TERM_SLATE, format(_("%^s�˾�뤳�Ȥ��Ǥ��롣", "%^s is suitable for riding.  "), wd_he[msex]));
	}


	/* Collect susceptibilities */
	vn = 0;
	if (flags3 & RF3_HURT_ROCK) { vp[vn] = _("���������", "rock remover"); color[vn++] = TERM_UMBER; }
	if (flags3 & RF3_HURT_LITE) { vp[vn] = _("���뤤��", "bright light"); color[vn++] = TERM_YELLOW; }
	if (flags3 & RF3_HURT_FIRE) { vp[vn] = _("��", "fire"); color[vn++] = TERM_RED; }
	if (flags3 & RF3_HURT_COLD) { vp[vn] = _("�䵤", "cold"); color[vn++] = TERM_L_WHITE; }


	/* Describe susceptibilities */
	if (vn)
	{
		/* Intro */
		hooked_roff(format(_("%^s�ˤ�", "%^s"), wd_he[msex]));

		/* Scan */
		for (n = 0; n < vn; n++)
		{
			/* Intro */
#ifdef JP
			if ( n != 0 ) hooked_roff("��");
#else
			if (n == 0) hooked_roff(" is hurt by ");
			else if (n < vn-1) hooked_roff(", ");
			else hooked_roff(" and ");
#endif


			/* Dump */
			hook_c_roff(color[n], vp[n]);
		}

		/* End */
		hooked_roff(_("�ǥ��᡼����Ϳ�����롣", ".  "));
	}


	/* Collect immunities */
	vn = 0;
	if (flagsr & RFR_IM_ACID) { vp[vn] = _("��", "acid"); color[vn++] = TERM_GREEN; }
	if (flagsr & RFR_IM_ELEC) { vp[vn] = _("���", "lightning"); color[vn++] = TERM_BLUE; }
	if (flagsr & RFR_IM_FIRE) { vp[vn] = _("��", "fire"); color[vn++] = TERM_RED; }
	if (flagsr & RFR_IM_COLD) { vp[vn] = _("�䵤", "cold"); color[vn++] = TERM_L_WHITE; }
	if (flagsr & RFR_IM_POIS) { vp[vn] = _("��", "poison"); color[vn++] = TERM_L_GREEN; }


	/* Collect resistances */
	if (flagsr & RFR_RES_LITE) { vp[vn] = _("����", "light"); color[vn++] = TERM_YELLOW; }
	if (flagsr & RFR_RES_DARK) { vp[vn] = _("�Ź�", "dark"); color[vn++] = TERM_L_DARK; }
	if (flagsr & RFR_RES_NETH) { vp[vn] = _("�Ϲ�", "nether"); color[vn++] = TERM_L_DARK; }
	if (flagsr & RFR_RES_WATE) { vp[vn] = _("��", "water"); color[vn++] = TERM_BLUE; }
	if (flagsr & RFR_RES_PLAS) { vp[vn] = _("�ץ饺��", "plasma"); color[vn++] = TERM_L_RED; }
	if (flagsr & RFR_RES_SHAR) { vp[vn] = _("����", "shards"); color[vn++] = TERM_L_UMBER; }
	if (flagsr & RFR_RES_SOUN) { vp[vn] = _("�첻", "sound"); color[vn++] = TERM_ORANGE; }
	if (flagsr & RFR_RES_CHAO) { vp[vn] = _("������", "chaos"); color[vn++] = TERM_VIOLET; }
	if (flagsr & RFR_RES_NEXU) { vp[vn] = _("���̺���", "nexus"); color[vn++] = TERM_VIOLET; }
	if (flagsr & RFR_RES_DISE) { vp[vn] = _("����", "disenchantment"); color[vn++] = TERM_VIOLET; }
	if (flagsr & RFR_RES_WALL) { vp[vn] = _("�ե�����", "force"); color[vn++] = TERM_UMBER; }
	if (flagsr & RFR_RES_INER) { vp[vn] = _("����", "inertia"); color[vn++] = TERM_SLATE; }
	if (flagsr & RFR_RES_TIME) { vp[vn] = _("���ֵ�ž", "time"); color[vn++] = TERM_L_BLUE; }
	if (flagsr & RFR_RES_GRAV) { vp[vn] = _("����", "gravity"); color[vn++] = TERM_SLATE; }
	if (flagsr & RFR_RES_ALL) { vp[vn] = _("�����빶��", "all"); color[vn++] = TERM_YELLOW; }
	if ((flagsr & RFR_RES_TELE) && !(r_ptr->flags1 & RF1_UNIQUE)) { vp[vn] = _("�ƥ�ݡ���", "teleportation"); color[vn++] = TERM_ORANGE; }

	/* Describe immunities and resistances */
	if (vn)
	{
		/* Intro */
		hooked_roff(format(_("%^s��", "%^s"), wd_he[msex]));

		/* Scan */
		for (n = 0; n < vn; n++)
		{
			/* Intro */
#ifdef JP
			if ( n != 0 ) hooked_roff("��");
#else
			if (n == 0) hooked_roff(" resists ");
			else if (n < vn-1) hooked_roff(", ");
			else hooked_roff(" and ");
#endif


			/* Dump */
			hook_c_roff(color[n], vp[n]);
		}

		/* End */
		hooked_roff(_("����������äƤ��롣", ".  "));
	}


	if ((r_ptr->r_xtra1 & MR1_SINKA) || know_everything)
	{
		if (r_ptr->next_r_idx)
		{
			hooked_roff(format(_("%^s�Ϸи����Ѥ�ȡ�", "%^s will evolve into "), wd_he[msex]));
			hook_c_roff(TERM_YELLOW, format("%s", r_name+r_info[r_ptr->next_r_idx].name));
			hooked_roff(format(
				_(("�˿ʲ����롣"), 
				  (" when %s gets enugh experience.  ", wd_he[msex]))));
		}
		else if (!(r_ptr->flags1 & RF1_UNIQUE))
		{
			hooked_roff(format(_("%s�Ͽʲ����ʤ���", "%s won't evolve.  "), wd_he[msex]));
		}
	}

	/* Collect non-effects */
	vn = 0;
	if (flags3 & RF3_NO_STUN)  { vp[vn] = _("ۯ۰�Ȥ��ʤ�", "stunned"); color[vn++] = TERM_ORANGE; }
	if (flags3 & RF3_NO_FEAR)  { vp[vn] = _("���ݤ򴶤��ʤ�", "frightened"); color[vn++] = TERM_SLATE; }
	if (flags3 & RF3_NO_CONF)  { vp[vn] = _("���𤷤ʤ�", "confused"); color[vn++] = TERM_L_UMBER; }
	if (flags3 & RF3_NO_SLEEP) { vp[vn] = _("̲�餵��ʤ�", "slept"); color[vn++] = TERM_BLUE; }
	if ((flagsr & RFR_RES_TELE) && (r_ptr->flags1 & RF1_UNIQUE)) { vp[vn] = _("�ƥ�ݡ��Ȥ���ʤ�", "teleported"); color[vn++] = TERM_ORANGE; }

	/* Describe non-effects */
	if (vn)
	{
		/* Intro */
		hooked_roff(format(
			_("%^s��", "%^s"), wd_he[msex]));

		/* Scan */
		for (n = 0; n < vn; n++)
		{
			/* Intro */
#ifdef JP
			if ( n != 0 ) hooked_roff("����");
#else
			if (n == 0) hooked_roff(" cannot be ");
			else if (n < vn - 1) hooked_roff(", ");
			else hooked_roff(" or ");
#endif


			/* Dump */
			hook_c_roff(color[n], vp[n]);
		}

		/* End */
		hooked_roff(_("��", ".  "));
	}


	/* Do we know how aware it is? */
	if ((((int)r_ptr->r_wake * (int)r_ptr->r_wake) > r_ptr->sleep) ||
		  (r_ptr->r_ignore == MAX_UCHAR) ||
	    (r_ptr->sleep == 0 && r_ptr->r_tkills >= 10) || know_everything)
	{
		cptr act;

		if (r_ptr->sleep > 200)
		{
			act = _("��̵�뤷�����Ǥ��뤬", "prefers to ignore");
		}
		else if (r_ptr->sleep > 95)
		{
			act = _("���Ф��ƤۤȤ����դ�ʧ��ʤ���", "pays very little attention to");
		}
		else if (r_ptr->sleep > 75)
		{
			act = _("���Ф��Ƥ��ޤ���դ�ʧ��ʤ���", "pays little attention to");
		}
		else if (r_ptr->sleep > 45)
		{
			act = _("�򸫲ᤴ�������Ǥ��뤬", "tends to overlook");
		}
		else if (r_ptr->sleep > 25)
		{
			act = _("��ۤ�ξ����ϸ��Ƥ���", "takes quite a while to see");
		}
		else if (r_ptr->sleep > 10)
		{
			act = _("�򤷤Ф餯�ϸ��Ƥ���", "takes a while to see");
		}
		else if (r_ptr->sleep > 5)
		{
			act = _("���ʬ��տ������Ƥ���", "is fairly observant of");
		}
		else if (r_ptr->sleep > 3)
		{
			act = _("����տ������Ƥ���", "is observant of");
		}
		else if (r_ptr->sleep > 1)
		{
			act = _("�򤫤ʤ���տ������Ƥ���", "is very observant of");
		}
		else if (r_ptr->sleep > 0)
		{
			act = _("��ٲ����Ƥ���", "is vigilant for");
		}
		else
		{
			act = _("�򤫤ʤ�ٲ����Ƥ���", "is ever vigilant for");
		}

		hooked_roff(
			_(format("%^s�Ͽ�����%s�� %d �ե������褫�鿯���Ԥ˵��դ����Ȥ����롣", wd_he[msex], act, 10 * r_ptr->aaf),
			  format("%^s %s intruders, which %s may notice from %d feet.  ", wd_he[msex], act, wd_he[msex], 10 * r_ptr->aaf)));
	}


	/* Drops gold and/or items */
	if (drop_gold || drop_item)
	{
		/* Intro */
		hooked_roff(format(
			_("%^s��", "%^s may carry"), wd_he[msex]));
#ifndef JP
		/* No "n" needed */
		sin = FALSE;
#endif


		/* Count maximum drop */
		n = MAX(drop_gold, drop_item);

		/* One drop (may need an "n") */
		if (n == 1)
		{
			hooked_roff(_("��Ĥ�", " a"));
#ifndef JP
			sin = TRUE;
#endif
		}

		/* Two drops */
		else if (n == 2)
		{
			hooked_roff(
				_("��Ĥ���Ĥ�", " one or two"));
		}

		/* Many drops */
		else
		{
			hooked_roff(format(
				_(" %d �ĤޤǤ�", " up to %d"), n));
		}


		/* Great */
		if (flags1 & RF1_DROP_GREAT)
		{
			p = _("���̤�", " exceptional");
		}

		/* Good (no "n" needed) */
		else if (flags1 & RF1_DROP_GOOD)
		{
			p = _("�����", " good");
#ifndef JP
			sin = FALSE;
#endif
		}

		/* Okay */
		else
		{
			p = NULL;
		}


		/* Objects */
		if (drop_item)
		{
			/* Handle singular "an" */
#ifndef JP
			if (sin) hooked_roff("n");
			sin = FALSE;
#endif

			/* Dump "object(s)" */
			if (p) hooked_roff(p);
			hooked_roff(
				_("�����ƥ�", " object"));

#ifndef JP
			if (n != 1) hooked_roff("s");
#endif

			/* Conjunction replaces variety, if needed for "gold" below */
			p = _("��", " or");
		}

		/* Treasures */
		if (drop_gold)
		{
#ifndef JP
			/* Cancel prefix */
			if (!p) sin = FALSE;

			/* Handle singular "an" */
			if (sin) hooked_roff("n");
			sin = FALSE;
#endif

			/* Dump "treasure(s)" */
			if (p) hooked_roff(p);
			hooked_roff(_("����", " treasure"));
#ifndef JP
			if (n != 1) hooked_roff("s");
#endif

		}

		/* End this sentence */
		hooked_roff(_("����äƤ��뤳�Ȥ����롣", ".  "));
	}


	/* Count the number of "known" attacks */
	for (n = 0, m = 0; m < 4; m++)
	{
		/* Skip non-attacks */
		if (!r_ptr->blow[m].method) continue;
		if (r_ptr->blow[m].method == RBM_SHOOT) continue;

		/* Count known attacks */
		if (r_ptr->r_blows[m] || know_everything) n++;
	}

	/* Examine (and count) the actual attacks */
	for (r = 0, m = 0; m < 4; m++)
	{
		int method, effect, d1, d2;

		/* Skip non-attacks */
		if (!r_ptr->blow[m].method) continue;
		if (r_ptr->blow[m].method == RBM_SHOOT) continue;

		/* Skip unknown attacks */
		if (!r_ptr->r_blows[m] && !know_everything) continue;

		/* Extract the attack info */
		method = r_ptr->blow[m].method;
		effect = r_ptr->blow[m].effect;
		d1 = r_ptr->blow[m].d_dice;
		d2 = r_ptr->blow[m].d_side;

		/* No method yet */
		p = NULL;

		/* Acquire the method */
		switch (method)
		{
			case RBM_HIT:		p = _("����", "hit"); break;
			case RBM_TOUCH:		p = _("����", "touch"); break;
			case RBM_PUNCH:		p = _("�ѥ������", "punch"); break;
			case RBM_KICK:		p = _("����", "kick"); break;
			case RBM_CLAW:		p = _("�Ҥä���", "claw"); break;
			case RBM_BITE:		p = _("����", "bite"); break;
			case RBM_STING:		p = _("�ɤ�", "sting"); break;
			case RBM_SLASH:		p = _("�¤�", "slash"); break;
			case RBM_BUTT:		p = _("�Ѥ��ͤ�", "butt"); break;
			case RBM_CRUSH:		p = _("�������ꤹ��", "crush"); break;
			case RBM_ENGULF:	p = _("���߹���", "engulf"); break;
			case RBM_CHARGE: 	p = _("������褳��", "charge"); break;
			case RBM_CRAWL:		p = _("�Τξ���礤���", "crawl on you"); break;
			case RBM_DROOL:		p = _("�����򤿤餹", "drool on you"); break;
			case RBM_SPIT:		p = _("�ĤФ��Ǥ�", "spit"); break;
			case RBM_EXPLODE:	p = _("��ȯ����", "explode"); break;
			case RBM_GAZE:		p = _("�ˤ��", "gaze"); break;
			case RBM_WAIL:		p = _("�㤭����", "wail"); break;
			case RBM_SPORE:		p = _("˦�Ҥ����Ф�", "release spores"); break;
			case RBM_XXX4:		break;
			case RBM_BEG:		p = _("��򤻤���", "beg"); break;
			case RBM_INSULT:	p = _("�����", "insult"); break;
			case RBM_MOAN:		p = _("���᤯", "moan"); break;
			case RBM_SHOW:  	p = _("�Τ�", "sing"); break;
		}


		/* Default effect */
		q = NULL;

		/* Acquire the effect */
		switch (effect)
		{
			case RBE_SUPERHURT: q = _("���Ϥ˹��⤹��", "slaughter"); break;
			case RBE_HURT:    	q = _("���⤹��", "attack"); break;
			case RBE_POISON:  	q = _("�Ǥ򤯤�魯", "poison"); break;
			case RBE_UN_BONUS:	q = _("����������", "disenchant"); break;
			case RBE_UN_POWER:	q = _("��Ŷ���Ϥ�ۼ�����", "drain charges"); break;
			case RBE_EAT_GOLD:	q = _("������", "steal gold"); break;
			case RBE_EAT_ITEM:	q = _("�����ƥ�����", "steal items"); break;
			case RBE_EAT_FOOD:	q = _("���ʤ��ο����򿩤٤�", "eat your food"); break;
			case RBE_EAT_LITE:	q = _("�������ۼ�����", "absorb light"); break;
			case RBE_ACID:    	q = _("�������Ф�", "shoot acid"); break;
			case RBE_ELEC:    	q = _("���Ť�����", "electrocute"); break;
			case RBE_FIRE:    	q = _("ǳ�䤹", "burn"); break;
			case RBE_COLD:    	q = _("��餻��", "freeze"); break;
			case RBE_BLIND:   	q = _("���ܤˤ���", "blind"); break;
			case RBE_CONFUSE: 	q = _("���𤵤���", "confuse"); break;
			case RBE_TERRIFY: 	q = _("���ݤ�����", "terrify"); break;
			case RBE_PARALYZE:	q = _("���㤵����", "paralyze"); break;
			case RBE_LOSE_STR:	q = _("���Ϥ򸺾�������", "reduce strength"); break;
			case RBE_LOSE_INT:	q = _("��ǽ�򸺾�������", "reduce intelligence"); break;
			case RBE_LOSE_WIS:	q = _("�����򸺾�������", "reduce wisdom"); break;
			case RBE_LOSE_DEX:	q = _("���Ѥ��򸺾�������", "reduce dexterity"); break;
			case RBE_LOSE_CON:	q = _("�ѵ��Ϥ򸺾�������", "reduce constitution"); break;
			case RBE_LOSE_CHR:	q = _("̥�Ϥ򸺾�������", "reduce charisma"); break;
			case RBE_LOSE_ALL:	q = _("�����ơ������򸺾�������", "reduce all stats"); break;
			case RBE_SHATTER:	q = _("ʴ�դ���", "shatter"); break;
			case RBE_EXP_10:	q = _("�и��ͤ򸺾�(10d6+)������", "lower experience (by 10d6+)"); break;
			case RBE_EXP_20:	q = _("�и��ͤ򸺾�(20d6+)������", "lower experience (by 20d6+)"); break;
			case RBE_EXP_40:	q = _("�и��ͤ򸺾�(40d6+)������", "lower experience (by 40d6+)"); break;
			case RBE_EXP_80:	q = _("�и��ͤ򸺾�(80d6+)������", "lower experience (by 80d6+)"); break;
			case RBE_DISEASE:	q = _("�µ��ˤ���", "disease"); break;
			case RBE_TIME:      q = _("���֤����ꤵ����", "time"); break;
			case RBE_DR_LIFE:   q = _("��̿�Ϥ�ۼ�����", "drain life"); break;
			case RBE_DR_MANA:   q = _("���Ϥ�å��", "drain mana force"); break;
			case RBE_INERTIA:   q = _("��®������", "slow"); break;
			case RBE_STUN:      q = _("ۯ۰�Ȥ�����", "stun"); break;
		}


#ifdef JP
		if ( r == 0 ) hooked_roff( format("%^s��", wd_he[msex]) );

		/***�㴳ɽ�����ѹ� ita ***/

			/* Describe damage (if known) */
		if (d1 && d2 && (know_everything || know_damage(r_idx, m)))
		  {
		    
		    /* Display the damage */
		    hooked_roff(format(" %dd%d ", d1, d2));
		    hooked_roff("�Υ��᡼����");
		  }
		/* Hack -- force a method */
		if (!p) p = "������̯�ʤ��Ȥ򤹤�";

		/* Describe the method */
		/* XX����YY��/XX����YY����/XX��/XX���� */
		if(q) jverb( p ,jverb_buf, JVERB_TO);
		else if(r!=n-1) jverb( p ,jverb_buf, JVERB_AND);
		else strcpy(jverb_buf, p);

		hooked_roff(jverb_buf);

		/* Describe the effect (if any) */
		if (q)
		{
		  if(r!=n-1) jverb( q,jverb_buf, JVERB_AND);
		  else strcpy(jverb_buf,q); 
		  hooked_roff(jverb_buf);
		}
		if(r!=n-1) hooked_roff("��");
#else
		/* Introduce the attack description */
		if (!r)
		{
			hooked_roff(format("%^s can ", wd_he[msex]));
		}
		else if (r < n-1)
		{
			hooked_roff(", ");
		}
		else
		{
			hooked_roff(", and ");
		}


		/* Hack -- force a method */
		if (!p) p = "do something weird";

		/* Describe the method */
		hooked_roff(p);


		/* Describe the effect (if any) */
		if (q)
		{
			/* Describe the attack type */
			hooked_roff(" to ");
			hooked_roff(q);

			/* Describe damage (if known) */
			if (d1 && d2 && (know_everything || know_damage(r_idx, m)))
			{
				/* Display the damage */
				hooked_roff(" with damage");
				hooked_roff(format(" %dd%d", d1, d2));
			}
		}
#endif



		/* Count the attacks as printed */
		r++;
	}

	/* Finish sentence above */
	if (r)
	{
		hooked_roff(_("��", ".  "));
	}

	/* Notice lack of attacks */
	else if (flags1 & RF1_NEVER_BLOW)
	{
		hooked_roff(format(
			_("%^s��ʪ��Ū�ʹ�����ˡ������ʤ���",
			  "%^s has no physical attacks.  "), wd_he[msex]));
	}

	/* Or describe the lack of knowledge */
	else
	{
		hooked_roff(format(
			_("%s����ˤĤ��Ƥϲ����Τ�ʤ���",
			  "Nothing is known about %s attack.  "), wd_his[msex]));
	}


	/*
	 * Notice "Quest" monsters, but only if you
	 * already encountered the monster.
	 */
	if ((flags1 & RF1_QUESTOR) && ((r_ptr->r_sights) && (r_ptr->max_num) && ((r_idx == MON_OBERON) || (r_idx == MON_SERPENT))))
	{
		hook_c_roff(TERM_VIOLET, 
			_("���ʤ��Ϥ��Υ�󥹥����򻦤������Ȥ���������˾�򴶤��Ƥ���...",
			  "You feel an intense desire to kill this monster...  "));
	}

	else if (flags7 & RF7_GUARDIAN)
	{
		hook_c_roff(TERM_L_RED, 
			_("���Υ�󥹥����ϥ��󥸥��μ�Ǥ��롣",
			  "This monster is the master of a dungeon."));
	}


	/* All done */
	hooked_roff("\n");

}


/*!
 * @brief ��󥹥�������Υإå��򵭽Ҥ���
 * Hack -- Display the "name" and "attr/chars" of a monster race
 * @param r_idx ��󥹥����μ�²ID
 * @return �ʤ�
 */
void roff_top(int r_idx)
{
	monster_race	*r_ptr = &r_info[r_idx];

	byte		a1, a2;
	char		c1, c2;


	/* Access the chars */
	c1 = r_ptr->d_char;
	c2 = r_ptr->x_char;

	/* Access the attrs */
	a1 = r_ptr->d_attr;
	a2 = r_ptr->x_attr;


	/* Clear the top line */
	Term_erase(0, 0, 255);

	/* Reset the cursor */
	Term_gotoxy(0, 0);

#ifndef JP
	/* A title (use "The" for non-uniques) */
	if (!(r_ptr->flags1 & RF1_UNIQUE))
	{
		Term_addstr(-1, TERM_WHITE, "The ");
	}
#endif

	/* Dump the name */
	Term_addstr(-1, TERM_WHITE, (r_name + r_ptr->name));

	/* Append the "standard" attr/char info */
	Term_addstr(-1, TERM_WHITE, " ('");
	Term_add_bigch(a1, c1);
	Term_addstr(-1, TERM_WHITE, "')");

	/* Append the "optional" attr/char info */
	Term_addstr(-1, TERM_WHITE, "/('");
	Term_add_bigch(a2, c2);
	Term_addstr(-1, TERM_WHITE, "'):");

	/* Wizards get extra info */
	if (p_ptr->wizard)
	{
		char buf[6];

		sprintf(buf, "%d", r_idx);

		Term_addstr(-1, TERM_WHITE, " (");
		Term_addstr(-1, TERM_L_BLUE, buf);
		Term_addch(TERM_WHITE, ')');
	}
}



/*!
 * @brief  ��󥹥��������ɽ���ȶ��˲��̤����õ�륵�֥롼���� /
 * Hack -- describe the given monster race at the top of the screen
 * @param r_idx ��󥹥����μ�²ID
 * @param mode ɽ�����ץ����
 * @return �ʤ�
 */
void screen_roff(int r_idx, int mode)
{
	/* Flush messages */
	msg_print(NULL);

	/* Begin recall */
	Term_erase(0, 1, 255);

	hook_c_roff = c_roff;

	/* Recall monster */
	roff_aux(r_idx, mode);

	/* Describe monster */
	roff_top(r_idx);
}




/*!
 * @brief ��󥹥�������θ��ߤΥ�����ɥ���ɽ������ /
 * Hack -- describe the given monster race in the current "term" window
 * @param r_idx ��󥹥����μ�²ID
 * @return �ʤ�
 */
void display_roff(int r_idx)
{
	int y;

	/* Erase the window */
	for (y = 0; y < Term->hgt; y++)
	{
		/* Erase the line */
		Term_erase(0, y, 255);
	}

	/* Begin recall */
	Term_gotoxy(0, 1);

	hook_c_roff = c_roff;

	/* Recall monster */
	roff_aux(r_idx, 0);

	/* Describe monster */
	roff_top(r_idx);
}


/*!
 * @brief ��󥹥����ܺپ����ư���ݥ��顼�����˽��Ϥ��� /
 * Hack -- output description of the given monster race
 * @param r_idx ��󥹥����μ�²ID
 * @param roff_func ���Ͻ�����Ԥ��ؿ��ݥ���
 * @return �ʤ�
 */
void output_monster_spoiler(int r_idx, void (*roff_func)(byte attr, cptr str))
{
	hook_c_roff = roff_func;

	/* Recall monster */
	roff_aux(r_idx, 0x03);
}


/*!
 * @brief ��󥹥��������󥸥��˽и����뤫�ɤ������֤�
 * @param r_idx Ƚ�ꤹ���󥹥����μ�²ID
 * @return ���󥸥��˽и�����ʤ��TRUE���֤�
 */
bool mon_hook_dungeon(int r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (!(r_ptr->flags8 & RF8_WILD_ONLY))
		return TRUE;
	else
	{
		dungeon_info_type *d_ptr = &d_info[dungeon_type];
		if ((d_ptr->mflags8 & RF8_WILD_MOUNTAIN) &&
		    (r_ptr->flags8 & RF8_WILD_MOUNTAIN)) return TRUE;
		return FALSE;
	}
}


/*!
 * @brief ��󥹥��������Τ˽и����뤫�ɤ������֤�
 * @param r_idx Ƚ�ꤹ���󥹥����μ�²ID
 * @return ���Τ˽и�����ʤ��TRUE���֤�
 */
static bool mon_hook_ocean(int r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & RF8_WILD_OCEAN)
		return TRUE;
	else
		return FALSE;
}


/*!
 * @brief ��󥹥��������ߤ˽и����뤫�ɤ������֤�
 * @param r_idx Ƚ�ꤹ���󥹥����μ�²ID
 * @return ���ߤ˽и�����ʤ��TRUE���֤�
 */
static bool mon_hook_shore(int r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & RF8_WILD_SHORE)
		return TRUE;
	else
		return FALSE;
}


/*!
 * @brief ��󥹥��������Ϥ˽и����뤫�ɤ������֤�
 * @param r_idx Ƚ�ꤹ���󥹥����μ�²ID
 * @return ���Ϥ˽и�����ʤ��TRUE���֤�
 */
static bool mon_hook_waste(int r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & (RF8_WILD_WASTE | RF8_WILD_ALL))
		return TRUE;
	else
		return FALSE;
}


/*!
 * @brief ��󥹥�����Į�˽и����뤫�ɤ������֤�
 * @param r_idx Ƚ�ꤹ���󥹥����μ�²ID
 * @return ���Ϥ˽и�����ʤ��TRUE���֤�
 */
static bool mon_hook_town(int r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & (RF8_WILD_TOWN | RF8_WILD_ALL))
		return TRUE;
	else
		return FALSE;
}


/*!
 * @brief ��󥹥��������Ӥ˽и����뤫�ɤ������֤�
 * @param r_idx Ƚ�ꤹ���󥹥����μ�²ID
 * @return ���Ӥ˽и�����ʤ��TRUE���֤�
 */
static bool mon_hook_wood(int r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & (RF8_WILD_WOOD | RF8_WILD_ALL))
		return TRUE;
	else
		return FALSE;
}


/*!
 * @brief ��󥹥������л��˽и����뤫�ɤ������֤�
 * @param r_idx Ƚ�ꤹ���󥹥����μ�²ID
 * @return �л��˽и�����ʤ��TRUE���֤�
 */
static bool mon_hook_volcano(int r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & RF8_WILD_VOLCANO)
		return TRUE;
	else
		return FALSE;
}

/*!
 * @brief ��󥹥��������Ϥ˽и����뤫�ɤ������֤�
 * @param r_idx Ƚ�ꤹ���󥹥����μ�²ID
 * @return ���Ϥ˽и�����ʤ��TRUE���֤�
 */
static bool mon_hook_mountain(int r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & RF8_WILD_MOUNTAIN)
		return TRUE;
	else
		return FALSE;
}


/*!
 * @brief ��󥹥������𸶤˽и����뤫�ɤ������֤�
 * @param r_idx Ƚ�ꤹ���󥹥����μ�²ID
 * @return ���Ӥ˽и�����ʤ��TRUE���֤�
 */
static bool mon_hook_grass(int r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & (RF8_WILD_GRASS | RF8_WILD_ALL))
		return TRUE;
	else
		return FALSE;
}

/*!
 * @brief ��󥹥������������Ϸ��˽и����뤫�ɤ������֤�
 * @param r_idx Ƚ�ꤹ���󥹥����μ�²ID
 * @return �������Ϸ��˽и�����ʤ��TRUE���֤�
 */
static bool mon_hook_deep_water(int r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (!mon_hook_dungeon(r_idx)) return FALSE;

	if (r_ptr->flags7 & RF7_AQUATIC)
		return TRUE;
	else
		return FALSE;
}


/*!
 * @brief ��󥹥������������Ϸ��˽и����뤫�ɤ������֤�
 * @param r_idx Ƚ�ꤹ���󥹥����μ�²ID
 * @return �������Ϸ��˽и�����ʤ��TRUE���֤�
 */
static bool mon_hook_shallow_water(int r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (!mon_hook_dungeon(r_idx)) return FALSE;

	if (r_ptr->flags2 & RF2_AURA_FIRE)
		return FALSE;
	else
		return TRUE;
}


/*!
 * @brief ��󥹥������ϴ��Ϸ��˽и����뤫�ɤ������֤�
 * @param r_idx Ƚ�ꤹ���󥹥����μ�²ID
 * @return �ϴ��Ϸ��˽и�����ʤ��TRUE���֤�
 */
static bool mon_hook_lava(int r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (!mon_hook_dungeon(r_idx)) return FALSE;

	if (((r_ptr->flagsr & RFR_EFF_IM_FIRE_MASK) ||
	     (r_ptr->flags7 & RF7_CAN_FLY)) &&
	    !(r_ptr->flags3 & RF3_AURA_COLD))
		return TRUE;
	else
		return FALSE;
}


/*!
 * @brief ��󥹥������̾�ξ��Ϸ��˽и����뤫�ɤ������֤�
 * @param r_idx Ƚ�ꤹ���󥹥����μ�²ID
 * @return �̾�ξ��Ϸ��˽и�����ʤ��TRUE���֤�
 */
static bool mon_hook_floor(int r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (!(r_ptr->flags7 & RF7_AQUATIC) ||
	    (r_ptr->flags7 & RF7_CAN_FLY))
		return TRUE;
	else
		return FALSE;
}


/*!
 * @brief �ץ쥤�䡼�θ��ߤι���ޥå׺�ɸ�������������򸵤˥�󥹥������������ؿ����֤�
 * @return �����ˤ��ä���󥹥������������ؿ�
 */
monster_hook_type get_monster_hook(void)
{
	if (!dun_level && !p_ptr->inside_quest)
	{
		switch (wilderness[p_ptr->wilderness_y][p_ptr->wilderness_x].terrain)
		{
		case TERRAIN_TOWN:
			return (monster_hook_type)mon_hook_town;
		case TERRAIN_DEEP_WATER:
			return (monster_hook_type)mon_hook_ocean;
		case TERRAIN_SHALLOW_WATER:
		case TERRAIN_SWAMP:
			return (monster_hook_type)mon_hook_shore;
		case TERRAIN_DIRT:
		case TERRAIN_DESERT:
			return (monster_hook_type)mon_hook_waste;
		case TERRAIN_GRASS:
			return (monster_hook_type)mon_hook_grass;
		case TERRAIN_TREES:
			return (monster_hook_type)mon_hook_wood;
		case TERRAIN_SHALLOW_LAVA:
		case TERRAIN_DEEP_LAVA:
			return (monster_hook_type)mon_hook_volcano;
		case TERRAIN_MOUNTAIN:
			return (monster_hook_type)mon_hook_mountain;
		default:
			return (monster_hook_type)mon_hook_dungeon;
		}
	}
	else
	{
		return (monster_hook_type)mon_hook_dungeon;
	}
}

/*!
 * @brief ���ꤵ�줿����ޥå׺�ɸ�������򸵤˥�󥹥������������ؿ����֤�
 * @return �����ˤ��ä���󥹥������������ؿ�
 */
monster_hook_type get_monster_hook2(int y, int x)
{
	feature_type *f_ptr = &f_info[cave[y][x].feat];

	/* Set the monster list */

	/* Water */
	if (have_flag(f_ptr->flags, FF_WATER))
	{
		/* Deep water */
		if (have_flag(f_ptr->flags, FF_DEEP))
		{
			return (monster_hook_type)mon_hook_deep_water;
		}

		/* Shallow water */
		else
		{
			return (monster_hook_type)mon_hook_shallow_water;
		}
	}

	/* Lava */
	else if (have_flag(f_ptr->flags, FF_LAVA))
	{
		return (monster_hook_type)mon_hook_lava;
	}

	else return (monster_hook_type)mon_hook_floor;
}

/*!
 * @brief ��󥹥�����ͧ��Ū�ˤ���
 * @param m_ptr ��󥹥�������¤�Τλ��ȥݥ���
 * @return �ʤ�
 */
void set_friendly(monster_type *m_ptr)
{
	m_ptr->smart |= SM_FRIENDLY;
}

/*!
 * @brief ��󥹥�����ڥåȤˤ���
 * @param m_ptr ��󥹥�������¤�Τλ��ȥݥ���
 * @return �ʤ�
 */
void set_pet(monster_type *m_ptr)
{
	if (!is_pet(m_ptr)) check_pets_num_and_align(m_ptr, TRUE);

	/* Check for quest completion */
	check_quest_completion(m_ptr);

	m_ptr->smart |= SM_PET;
	if (!(r_info[m_ptr->r_idx].flags3 & (RF3_EVIL | RF3_GOOD)))
		m_ptr->sub_align = SUB_ALIGN_NEUTRAL;
}

/*!
 * @brief ��󥹥�����Ũ�˲�
 * Makes the monster hostile towards the player
 * @param m_ptr ��󥹥�������¤�Τλ��ȥݥ���
 * @return �ʤ�
 */
void set_hostile(monster_type *m_ptr)
{
	if (p_ptr->inside_battle) return;

	if (is_pet(m_ptr)) check_pets_num_and_align(m_ptr, FALSE);

	m_ptr->smart &= ~SM_PET;
	m_ptr->smart &= ~SM_FRIENDLY;
}


/*!
 * @brief ��󥹥������ܤ餻��
 * Anger the monster
 * @param m_ptr ��󥹥�������¤�Τλ��ȥݥ���
 * @return �ʤ�
 */
void anger_monster(monster_type *m_ptr)
{
	if (p_ptr->inside_battle) return;
	if (is_friendly(m_ptr))
	{
		char m_name[80];

		monster_desc(m_name, m_ptr, 0);
		msg_format(_("%^s���ܤä���", "%^s gets angry!"), m_name);

		set_hostile(m_ptr);

		chg_virtue(V_INDIVIDUALISM, 1);
		chg_virtue(V_HONOUR, -1);
		chg_virtue(V_JUSTICE, -1);
		chg_virtue(V_COMPASSION, -1);
	}
}


/*!
 * @brief ��󥹥������Ϸ���Ƨ�ˤǤ��뤫�ɤ������֤�
 * Check if monster can cross terrain
 * @param feat �Ϸ�ID
 * @param r_ptr ��󥹥�����²��¤�Τλ��ȥݥ���
 * @param mode ���ץ����
 * @return Ƨ�˲�ǽ�ʤ��TRUE���֤�
 */
bool monster_can_cross_terrain(s16b feat, monster_race *r_ptr, u16b mode)
{
	feature_type *f_ptr = &f_info[feat];

	/* Pattern */
	if (have_flag(f_ptr->flags, FF_PATTERN))
	{
		if (!(mode & CEM_RIDING))
		{
			if (!(r_ptr->flags7 & RF7_CAN_FLY)) return FALSE;
		}
		else
		{
			if (!(mode & CEM_P_CAN_ENTER_PATTERN)) return FALSE;
		}
	}

	/* "CAN" flags */
	if (have_flag(f_ptr->flags, FF_CAN_FLY) && (r_ptr->flags7 & RF7_CAN_FLY)) return TRUE;
	if (have_flag(f_ptr->flags, FF_CAN_SWIM) && (r_ptr->flags7 & RF7_CAN_SWIM)) return TRUE;
	if (have_flag(f_ptr->flags, FF_CAN_PASS))
	{
		if ((r_ptr->flags2 & RF2_PASS_WALL) && (!(mode & CEM_RIDING) || p_ptr->pass_wall)) return TRUE;
	}

	if (!have_flag(f_ptr->flags, FF_MOVE)) return FALSE;

	/* Some monsters can walk on mountains */
	if (have_flag(f_ptr->flags, FF_MOUNTAIN) && (r_ptr->flags8 & RF8_WILD_MOUNTAIN)) return TRUE;

	/* Water */
	if (have_flag(f_ptr->flags, FF_WATER))
	{
		if (!(r_ptr->flags7 & RF7_AQUATIC))
		{
			/* Deep water */
			if (have_flag(f_ptr->flags, FF_DEEP)) return FALSE;

			/* Shallow water */
			else if (r_ptr->flags2 & RF2_AURA_FIRE) return FALSE;
		}
	}

	/* Aquatic monster into non-water? */
	else if (r_ptr->flags7 & RF7_AQUATIC) return FALSE;

	/* Lava */
	if (have_flag(f_ptr->flags, FF_LAVA))
	{
		if (!(r_ptr->flagsr & RFR_EFF_IM_FIRE_MASK)) return FALSE;
	}

	return TRUE;
}


/*!
 * @brief ���ꤵ�줿��ɸ���Ϸ����󥹥�����Ƨ�ˤǤ��뤫�ɤ������֤�
 * Strictly check if monster can enter the grid
 * @param y �Ϸ���Y��ɸ
 * @param x �Ϸ���X��ɸ
 * @param r_ptr ��󥹥�����²��¤�Τλ��ȥݥ���
 * @param mode ���ץ����
 * @return Ƨ�˲�ǽ�ʤ��TRUE���֤�
 */
bool monster_can_enter(int y, int x, monster_race *r_ptr, u16b mode)
{
	cave_type *c_ptr = &cave[y][x];

	/* Player or other monster */
	if (player_bold(y, x)) return FALSE;
	if (c_ptr->m_idx) return FALSE;

	return monster_can_cross_terrain(c_ptr->feat, r_ptr, mode);
}


/*!
 * @brief ��󥹥�����°���δ�Ť���Ũ�дط���̵ͭ���֤��ʥ��֥롼�����
 * Check if this monster has "hostile" alignment (aux)
 * @param sub_align1 ��󥹥���1�Υ��֥ե饰
 * @param sub_align2 ��󥹥���2�Υ��֥ե饰
 * @return Ũ�дط��ˤ���ʤ��TRUE���֤�
 */
static bool check_hostile_align(byte sub_align1, byte sub_align2)
{
	if (sub_align1 != sub_align2)
	{
		if (((sub_align1 & SUB_ALIGN_EVIL) && (sub_align2 & SUB_ALIGN_GOOD)) ||
			((sub_align1 & SUB_ALIGN_GOOD) && (sub_align2 & SUB_ALIGN_EVIL)))
			return TRUE;
	}

	/* Non-hostile alignment */
	return FALSE;
}


/*!
 * @brief ��󥹥�����°���δ�Ť���Ũ�дط���̵ͭ���֤�
 * Check if two monsters are enemies
 * @param m_ptr ��󥹥���1�ι�¤�λ��ȥݥ���
 * @param n_ptr ��󥹥���2�ι�¤�λ��ȥݥ���
 * @return Ũ�дط��ˤ���ʤ��TRUE���֤�
 */
bool are_enemies(monster_type *m_ptr, monster_type *n_ptr)
{
	monster_race *r_ptr = &r_info[m_ptr->r_idx];
	monster_race *s_ptr = &r_info[n_ptr->r_idx];

	if (p_ptr->inside_battle)
	{
		if (is_pet(m_ptr) || is_pet(n_ptr)) return FALSE;
		return TRUE;
	}

	if ((r_ptr->flags8 & (RF8_WILD_TOWN | RF8_WILD_ALL))
	    && (s_ptr->flags8 & (RF8_WILD_TOWN | RF8_WILD_ALL)))
	{
		if (!is_pet(m_ptr) && !is_pet(n_ptr)) return FALSE;
	}

	/* Friendly vs. opposite aligned normal or pet */
	if (check_hostile_align(m_ptr->sub_align, n_ptr->sub_align))
	{
		if (!(m_ptr->mflag2 & MFLAG2_CHAMELEON) || !(n_ptr->mflag2 & MFLAG2_CHAMELEON)) return TRUE;
	}

	/* Hostile vs. non-hostile */
	if (is_hostile(m_ptr) != is_hostile(n_ptr))
	{
		return TRUE;
	}

	/* Default */
	return FALSE;
}


/*!
 * @brief ��󥹥������ץ쥤�䡼���Ф���Ũ�դ��������ɤ������֤�
 * Check if this monster race has "hostile" alignment
 * @param m_ptr ��󥹥�������¤�Τλ��ȥݥ���
 * @param pa_good �ץ쥤�䡼����������
 * @param pa_evil �ץ쥤�䡼�ΰ�������
 * @param r_ptr ��󥹥�����²����ι�¤�λ��ȥݥ���
 * @return �ץ쥤�䡼��Ũ�դ���Ĥʤ��TRUE���֤�
 * @details
 * If user is player, m_ptr == NULL.
 */
bool monster_has_hostile_align(monster_type *m_ptr, int pa_good, int pa_evil, monster_race *r_ptr)
{
	byte sub_align1 = SUB_ALIGN_NEUTRAL;
	byte sub_align2 = SUB_ALIGN_NEUTRAL;

	if (m_ptr) /* For a monster */
	{
		sub_align1 = m_ptr->sub_align;
	}
	else /* For player */
	{
		if (p_ptr->align >= pa_good) sub_align1 |= SUB_ALIGN_GOOD;
		if (p_ptr->align <= pa_evil) sub_align1 |= SUB_ALIGN_EVIL;
	}

	/* Racial alignment flags */
	if (r_ptr->flags3 & RF3_EVIL) sub_align2 |= SUB_ALIGN_EVIL;
	if (r_ptr->flags3 & RF3_GOOD) sub_align2 |= SUB_ALIGN_GOOD;

	if (check_hostile_align(sub_align1, sub_align2)) return TRUE;

	/* Non-hostile alignment */
	return FALSE;
}


/*!
 * @brief ��󥹥�������̿�Τ��ɤ������֤�
 * Is the monster "alive"?
 * @param r_ptr Ƚ�ꤹ���󥹥����μ�²����¤�λ��ȥݥ���
 * @return ��̿�Τʤ��TRUE���֤�
 * @details
 * Used to determine the message to print for a killed monster.
 * ("dies", "destroyed")
 */
bool monster_living(monster_race *r_ptr)
{
	/* Non-living, undead, or demon */
	if (r_ptr->flags3 & (RF3_DEMON | RF3_UNDEAD | RF3_NONLIVING))
		return FALSE;
	else
		return TRUE;
}


/*!
 * @brief ��󥹥������ü�ǽ�Ͼ塢�޶�󤫤��ӽ�����ɬ�פ����뤫�ɤ������֤���
 * Is the monster "alive"? / Is this monster declined to be questor or bounty?
 * @param r_idx ��󥹥����μ�²ID
 * @return �޶��˲ä����ʤ��ʤ��TRUE���֤�
 * @details
 * �¼��С��Ρ�����ѡ����ѡ�
 */
bool no_questor_or_bounty_uniques(int r_idx)
{
	switch (r_idx)
	{
	/*
	 * Decline them to be questor or bounty because they use
	 * special motion "split and combine"
	 */
	case MON_BANORLUPART:
	case MON_BANOR:
	case MON_LUPART:
		return TRUE;
	default:
		return FALSE;
	}
}
