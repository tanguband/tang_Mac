/*!
    @file do-spell.c
    @brief ��ˡ�Υ��󥿡��ե�������ȯư / Purpose: Do everything for each spell
    @date 2013/12/31
    @author
    2013 Deskull rearranged comment for Doxygen.
 */

#include "angband.h"


/*!
 * @brief
 * ��ˡ�θ��̤�֥���ץ����:������������͡פΥե����ޥåȤǽ��Ϥ��� / Generate dice info string such as "foo 2d10"
 * @param str ����ץ����
 * @param dice ��������
 * @param sides ��������
 * @param base ������
 * @return �ե����ޥåȤ˽����������줿ʸ����
 */
static cptr info_string_dice(cptr str, int dice, int sides, int base)
{
	/* Fix value */
	if (!dice)
		return format("%s%d", str, base);

	/* Dice only */
	else if (!base)
		return format("%s%dd%d", str, dice, sides);

	/* Dice plus base value */
	else
		return format("%s%dd%d%+d", str, dice, sides, base);
}


/*!
 * @brief ��ˡ�ˤ����᡼������Ϥ��� / Generate damage-dice info string such as "dam 2d10"
 * @param dice ��������
 * @param sides ��������
 * @param base ������
 * @return �ե����ޥåȤ˽����������줿ʸ����
 */
static cptr info_damage(int dice, int sides, int base)
{
	return info_string_dice(_("»��:", "dam "), dice, sides, base);
}

/*!
 * @brief ��ˡ�θ��̻��֤���Ϥ��� / Generate duration info string such as "dur 20+1d20"
 * @param base ������
 * @param sides ��������
 * @return �ե����ޥåȤ˽����������줿ʸ����
 */
static cptr info_duration(int base, int sides)
{
	return format(_("����:%d+1d%d", "dur %d+1d%d"), base, sides);
}

/*!
 * @brief ��ˡ�θ����ϰϤ���Ϥ��� / Generate range info string such as "range 5"
 * @param range �����ϰ�
 * @return �ե����ޥåȤ˽����������줿ʸ����
 */
static cptr info_range(POSITION range)
{
	return format(_("�ϰ�:%d", "range %d"), range);
}

/*!
 * @brief ��ˡ�ˤ������̤���Ϥ��� / Generate heal info string such as "heal 2d8"
 * @param dice ��������
 * @param sides ��������
 * @param base ������
 * @return �ե����ޥåȤ˽����������줿ʸ����
 */
static cptr info_heal(int dice, int sides, int base)
{
	return info_string_dice(_("����:", "heal "), dice, sides, base);
}

/*!
 * @brief ��ˡ����ȯư�ޤǤ��ٱ䥿�������Ϥ��� / Generate delay info string such as "delay 15+1d15"
 * @param base ������
 * @param sides ��������
 * @return �ե����ޥåȤ˽����������줿ʸ����
 */
static cptr info_delay(int base, int sides)
{
	return format(_("�ٱ�:%d+1d%d", "delay %d+1d%d"), base, sides);
}


/*!
 * @brief ��ˡ�ˤ����᡼������Ϥ���(�����͡�ʣ�������) / Generate multiple-damage info string such as "dam 25 each"
 * @param dam ������
 * @return �ե����ޥåȤ˽����������줿ʸ����
 */
static cptr info_multi_damage(HIT_POINT dam)
{
	return format(_("»��:��%d", "dam %d each"), dam);
}


/*!
 * @brief ��ˡ�ˤ����᡼������Ϥ���(�������Τߡ�ʣ�������) / Generate multiple-damage-dice info string such as "dam 5d2 each"
 * @param dice ��������
 * @param sides ��������
 * @return �ե����ޥåȤ˽����������줿ʸ����
 */
static cptr info_multi_damage_dice(int dice, int sides)
{
	return format(_("»��:��%dd%d", "dam %dd%d each"), dice, sides);
}

/*!
 * @brief ��ˡ�ˤ�����Ū�ʸ����ͤ���Ϥ���ʸ����͡� / Generate power info string such as "power 100"
 * @param power ������
 * @return �ե����ޥåȤ˽����������줿ʸ����
 */
static cptr info_power(int power)
{
	return format(_("����:%d", "power %d"), power);
}


/*!
 * @brief ��ˡ�ˤ�����Ū�ʸ����ͤ���Ϥ���ʥ������͡� / Generate power info string such as "power 100"
 * @param dice ��������
 * @param sides ��������
 * @return �ե����ޥåȤ˽����������줿ʸ����
 */
/*
 * Generate power info string such as "power 1d100"
 */
static cptr info_power_dice(int dice, int sides)
{
	return format(_("����:%dd%d", "power %dd%d"), dice, sides);
}


/*!
 * @brief ��ˡ�θ���Ⱦ�¤���Ϥ��� / Generate radius info string such as "rad 100"
 * @param rad ����Ⱦ��
 * @return �ե����ޥåȤ˽����������줿ʸ����
 */
static cptr info_radius(int rad)
{
	return format(_("Ⱦ��:%d", "rad %d"), rad);
}


/*!
 * @brief ��ˡ���̤θ³����̤���Ϥ��� / Generate weight info string such as "max wgt 15"
 * @param weight �������
 * @return �ե����ޥåȤ˽����������줿ʸ����
 */
static cptr info_weight(int weight)
{
#ifdef JP
	return format("�������:%d.%dkg", lbtokg1(weight), lbtokg2(weight));
#else
	return format("max wgt %d", weight/10);
#endif
}


/*!
 * @brief �����ܥ����ˡ�Υӡ��ಽ��Ψ�򻻽Ф��� / Prepare standard probability to become beam for fire_bolt_or_beam()
 * @return �ӡ��ಽ��Ψ(%)
 * @details
 * �ϡ��ɥ����ƥ��󥰤ˤ��������Ԥ��Ƥ��롣
 * �ᥤ����(��٥�)%���ϥ��ᥤ�������ڥ�ޥ�������(��٥�)%������ʳ��ο��Ȥ�(��٥�/2)%
 */
static int beam_chance(void)
{
	if (p_ptr->pclass == CLASS_MAGE)
		return p_ptr->lev;
	if (p_ptr->pclass == CLASS_HIGH_MAGE || p_ptr->pclass == CLASS_SORCERER)
		return p_ptr->lev + 10;

	return p_ptr->lev / 2;
}

/*!
 * @brief �ȥ�����ˡ�ȼ��ξ���������Ԥ� / Handle summoning and failure of trump spells
 * @param num summon_specific()�ؿ���ƤӽФ����
 * @param pet �ڥåȲ��Ȥ��ƾ�������뤫�ݤ�
 * @param y �������֤�y��ɸ
 * @param x �������֤�x��ɸ
 * @param lev ������٥�
 * @param type �������ID
 * @param mode ��󥹥����������ե饰
 * @return ��󥹥�������Ũ�Ф�ޤ�ơ˾��Ԥ��줿�ʤ��TRUE���֤���
 */
static bool trump_summoning(int num, bool pet, POSITION y, POSITION x, DEPTH lev, int type, BIT_FLAGS mode)
{
	PLAYER_LEVEL plev = p_ptr->lev;

	MONSTER_IDX who;
	int i;
	bool success = FALSE;

	/* Default level */
	if (!lev) lev = plev * 2 / 3 + randint1(plev / 2);

	if (pet)
	{
		/* Become pet */
		mode |= PM_FORCE_PET;

		/* Only sometimes allow unique monster */
		if (mode & PM_ALLOW_UNIQUE)
		{
			/* Forbid often */
			if (randint1(50 + plev) >= plev / 10)
				mode &= ~PM_ALLOW_UNIQUE;
		}

		/* Player is who summons */
		who = -1;
	}
	else
	{
		/* Prevent taming, allow unique monster */
		mode |= PM_NO_PET;

		/* Behave as if they appear by themselfs */
		who = 0;
	}

	for (i = 0; i < num; i++)
	{
		if (summon_specific(who, y, x, lev, type, mode))
			success = TRUE;
	}

	if (!success)
	{
		msg_print(_("ï�⤢�ʤ��Υ����ɤθƤ����������ʤ���", "Nobody answers to your Trump call."));
	}

	return success;
}


/*!
 * @brief �֥������פΥ�����ʸ��̤���ꤷ�ƽ������롣
 * @param dir ����ID
 * @return �ʤ�
 * @details
 * This spell should become more useful (more controlled) as the\n
 * player gains experience levels.  Thus, add 1/5 of the player's\n
 * level to the die roll.  This eliminates the worst effects later on,\n
 * while keeping the results quite random.  It also allows some potent\n
 * effects only at high level.
 */
static void cast_wonder(int dir)
{
	int plev = p_ptr->lev;
	int die = randint1(100) + plev / 5;
	int vir = virtue_number(V_CHANCE);

	if (vir)
	{
		if (p_ptr->virtues[vir - 1] > 0)
		{
			while (randint1(400) < p_ptr->virtues[vir - 1]) die++;
		}
		else
		{
			while (randint1(400) < (0-p_ptr->virtues[vir - 1])) die--;
		}
	}

	if (die < 26)
		chg_virtue(V_CHANCE, 1);

	if (die > 100)
	{
		msg_print(_("���ʤ����Ϥ��ߤʤ���Τ򴶤�����", "You feel a surge of power!"));
	}

	if (die < 8) clone_monster(dir);
	else if (die < 14) speed_monster(dir, plev);
	else if (die < 26) heal_monster(dir, damroll(4, 6));
	else if (die < 31) poly_monster(dir, plev);
	else if (die < 36)
		fire_bolt_or_beam(beam_chance() - 10, GF_MISSILE, dir,
				  damroll(3 + ((plev - 1) / 5), 4));
	else if (die < 41) confuse_monster(dir, plev);
	else if (die < 46) fire_ball(GF_POIS, dir, 20 + (plev / 2), 3);
	else if (die < 51) (void)lite_line(dir, damroll(6, 8));
	else if (die < 56)
		fire_bolt_or_beam(beam_chance() - 10, GF_ELEC, dir,
				  damroll(3 + ((plev - 5) / 4), 8));
	else if (die < 61)
		fire_bolt_or_beam(beam_chance() - 10, GF_COLD, dir,
				  damroll(5 + ((plev - 5) / 4), 8));
	else if (die < 66)
		fire_bolt_or_beam(beam_chance(), GF_ACID, dir,
				  damroll(6 + ((plev - 5) / 4), 8));
	else if (die < 71)
		fire_bolt_or_beam(beam_chance(), GF_FIRE, dir,
				  damroll(8 + ((plev - 5) / 4), 8));
	else if (die < 76) hypodynamic_bolt(dir, 75);
	else if (die < 81) fire_ball(GF_ELEC, dir, 30 + plev / 2, 2);
	else if (die < 86) fire_ball(GF_ACID, dir, 40 + plev, 2);
	else if (die < 91) fire_ball(GF_ICE, dir, 70 + plev, 3);
	else if (die < 96) fire_ball(GF_FIRE, dir, 80 + plev, 3);
	else if (die < 101) hypodynamic_bolt(dir, 100 + plev);
	else if (die < 104)
	{
		earthquake(p_ptr->y, p_ptr->x, 12);
	}
	else if (die < 106)
	{
		(void)destroy_area(p_ptr->y, p_ptr->x, 13 + randint0(5), FALSE);
	}
	else if (die < 108)
	{
		symbol_genocide(plev+50, TRUE);
	}
	else if (die < 110) dispel_monsters(120);
	else /* RARE */
	{
		dispel_monsters(150);
		slow_monsters(plev);
		sleep_monsters(plev);
		hp_player(300);
	}
}


/*!
 * @brief �ְ�����פΥ�����ʸ��̤���ꤷ�ƽ������롣
 * @param dir ����ID
 * @return �ʤ�
 */
static void cast_invoke_spirits(int dir)
{
	int plev = p_ptr->lev;
	int die = randint1(100) + plev / 5;
	int vir = virtue_number(V_CHANCE);

	if (vir)
	{
		if (p_ptr->virtues[vir - 1] > 0)
		{
			while (randint1(400) < p_ptr->virtues[vir - 1]) die++;
		}
		else
		{
			while (randint1(400) < (0-p_ptr->virtues[vir - 1])) die--;
		}
	}

	msg_print(_("���ʤ��ϻ�Ԥ������Ϥ򾷽�����...", "You call on the power of the dead..."));
	if (die < 26)
		chg_virtue(V_CHANCE, 1);

	if (die > 100)
	{
		msg_print(_("���ʤ��Ϥ��ɤ��ɤ����ϤΤ��ͤ�򴶤�����", "You feel a surge of eldritch force!"));
	}


	if (die < 8)
	{
		msg_print(_("�ʤ�Ƥ��ä������ʤ��μ�������̤��������ͱƤ�Ω���夬�äƤ�����", 
					"Oh no! Mouldering forms rise from the earth around you!"));

		(void)summon_specific(0, p_ptr->y, p_ptr->x, dun_level, SUMMON_UNDEAD, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE | PM_NO_PET));
		chg_virtue(V_UNLIFE, 1);
	}
	else if (die < 14)
	{
		msg_print(_("̾�����񤤼ٰ���¸�ߤ����ʤ��ο����̤�᤮�ƹԤä�...", "An unnamable evil brushes against your mind..."));

		set_afraid(p_ptr->afraid + randint1(4) + 4);
	}
	else if (die < 26)
	{
		msg_print(_("���ʤ���Ƭ�����̤�ͩ����������������������󤻤Ƥ���...",
					"Your head is invaded by a horde of gibbering spectral voices..."));

		set_confused(p_ptr->confused + randint1(4) + 4);
	}
	else if (die < 31)
	{
		poly_monster(dir, plev);
	}
	else if (die < 36)
	{
		fire_bolt_or_beam(beam_chance() - 10, GF_MISSILE, dir,
				  damroll(3 + ((plev - 1) / 5), 4));
	}
	else if (die < 41)
	{
		confuse_monster (dir, plev);
	}
	else if (die < 46)
	{
		fire_ball(GF_POIS, dir, 20 + (plev / 2), 3);
	}
	else if (die < 51)
	{
		(void)lite_line(dir, damroll(6, 8));
	}
	else if (die < 56)
	{
		fire_bolt_or_beam(beam_chance() - 10, GF_ELEC, dir,
				  damroll(3+((plev-5)/4),8));
	}
	else if (die < 61)
	{
		fire_bolt_or_beam(beam_chance() - 10, GF_COLD, dir,
				  damroll(5+((plev-5)/4),8));
	}
	else if (die < 66)
	{
		fire_bolt_or_beam(beam_chance(), GF_ACID, dir,
				  damroll(6+((plev-5)/4),8));
	}
	else if (die < 71)
	{
		fire_bolt_or_beam(beam_chance(), GF_FIRE, dir,
				  damroll(8+((plev-5)/4),8));
	}
	else if (die < 76)
	{
		hypodynamic_bolt(dir, 75);
	}
	else if (die < 81)
	{
		fire_ball(GF_ELEC, dir, 30 + plev / 2, 2);
	}
	else if (die < 86)
	{
		fire_ball(GF_ACID, dir, 40 + plev, 2);
	}
	else if (die < 91)
	{
		fire_ball(GF_ICE, dir, 70 + plev, 3);
	}
	else if (die < 96)
	{
		fire_ball(GF_FIRE, dir, 80 + plev, 3);
	}
	else if (die < 101)
	{
		hypodynamic_bolt(dir, 100 + plev);
	}
	else if (die < 104)
	{
		earthquake(p_ptr->y, p_ptr->x, 12);
	}
	else if (die < 106)
	{
		(void)destroy_area(p_ptr->y, p_ptr->x, 13 + randint0(5), FALSE);
	}
	else if (die < 108)
	{
		symbol_genocide(plev+50, TRUE);
	}
	else if (die < 110)
	{
		dispel_monsters(120);
	}
	else
	{ /* RARE */
		dispel_monsters(150);
		slow_monsters(plev);
		sleep_monsters(plev);
		hp_player(300);
	}

	if (die < 31)
	{
		msg_print(_("�������������������Ф����֤⤦�������ޤ��ϲ桹����֤ˤʤ�������夭�Ԥ衣��",
					"Sepulchral voices chuckle. 'Soon you will join us, mortal.'"));
	}
}

/*!
 * @brief ������Ū���̤��뤤�ϵڤӥ���åե�Ρֱ�̿���ء׸��̤�������˽������롣
 * @param spell ���Ȥʤ����ID
 * @return �ʤ�
 */
static void wild_magic(SPELL_IDX spell)
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
		break;
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
}

/*!
 * @brief �ȥ����ΰ�Ρ֥���åե�פθ��̤������˷��ƽ������롣
 * @return �ʤ�
 */
static void cast_shuffle(void)
{
	int plev = p_ptr->lev;
	int dir;
	int die;
	int vir = virtue_number(V_CHANCE);
	int i;

	/* Card sharks and high mages get a level bonus */
	if ((p_ptr->pclass == CLASS_ROGUE) ||
	    (p_ptr->pclass == CLASS_HIGH_MAGE) ||
	    (p_ptr->pclass == CLASS_SORCERER))
		die = (randint1(110)) + plev / 5;
	else
		die = randint1(120);


	if (vir)
	{
		if (p_ptr->virtues[vir - 1] > 0)
		{
			while (randint1(400) < p_ptr->virtues[vir - 1]) die++;
		}
		else
		{
			while (randint1(400) < (0-p_ptr->virtues[vir - 1])) die--;
		}
	}

	msg_print(_("���ʤ��ϥ����ɤ��ڤäư��������...", "You shuffle the deck and draw a card..."));

	if (die < 30)
		chg_virtue(V_CHANCE, 1);

	if (die < 7)
	{
		msg_print(_("�ʤ�Ƥ��ä����Ի�դ���", "Oh no! It's Death!"));

		for (i = 0; i < randint1(3); i++)
			activate_hi_summon(p_ptr->y, p_ptr->x, FALSE);
	}
	else if (die < 14)
	{
		msg_print(_("�ʤ�Ƥ��ä����԰���դ���", "Oh no! It's the Devil!"));
		summon_specific(0, p_ptr->y, p_ptr->x, dun_level, SUMMON_DEMON, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE | PM_NO_PET));
	}
	else if (die < 18)
	{
		int count = 0;
		msg_print(_("�ʤ�Ƥ��ä������ߤ�줿�ˡդ���", "Oh no! It's the Hanged Man."));
		activate_ty_curse(FALSE, &count);
	}
	else if (die < 22)
	{
		msg_print(_("����Ĵ�¤η��դ���", "It's the swords of discord."));
		aggravate_monsters(0);
	}
	else if (die < 26)
	{
		msg_print(_("�Զ�ԡդ���", "It's the Fool."));
		do_dec_stat(A_INT);
		do_dec_stat(A_WIS);
	}
	else if (die < 30)
	{
		msg_print(_("��̯�ʥ�󥹥����γ�����", "It's the picture of a strange monster."));
		trump_summoning(1, FALSE, p_ptr->y, p_ptr->x, (dun_level * 3 / 2), (32 + randint1(6)), PM_ALLOW_GROUP | PM_ALLOW_UNIQUE);
	}
	else if (die < 33)
	{
		msg_print(_("�Է�դ���", "It's the Moon."));
		unlite_area(10, 3);
	}
	else if (die < 38)
	{
		msg_print(_("�Ա�̿���ءդ���", "It's the Wheel of Fortune."));
		wild_magic(randint0(32));
	}
	else if (die < 40)
	{
		msg_print(_("�ƥ�ݡ��ȡ������ɤ���", "It's a teleport trump card."));
		teleport_player(10, TELEPORT_PASSIVE);
	}
	else if (die < 42)
	{
		msg_print(_("�������դ���", "It's Justice."));
		set_blessed(p_ptr->lev, FALSE);
	}
	else if (die < 47)
	{
		msg_print(_("�ƥ�ݡ��ȡ������ɤ���", "It's a teleport trump card."));
		teleport_player(100, TELEPORT_PASSIVE);
	}
	else if (die < 52)
	{
		msg_print(_("�ƥ�ݡ��ȡ������ɤ���", "It's a teleport trump card."));
		teleport_player(200, TELEPORT_PASSIVE);
	}
	else if (die < 60)
	{
		msg_print(_("����դ���", "It's the Tower."));
		wall_breaker();
	}
	else if (die < 72)
	{
		msg_print(_("�������դ���", "It's Temperance."));
		sleep_monsters_touch();
	}
	else if (die < 80)
	{
		msg_print(_("����դ���", "It's the Tower."));

		earthquake(p_ptr->y, p_ptr->x, 5);
	}
	else if (die < 82)
	{
		msg_print(_("ͧ��Ū�ʥ�󥹥����γ�����", "It's the picture of a friendly monster."));
		trump_summoning(1, TRUE, p_ptr->y, p_ptr->x, (dun_level * 3 / 2), SUMMON_MOLD, 0L);
	}
	else if (die < 84)
	{
		msg_print(_("ͧ��Ū�ʥ�󥹥����γ�����", "It's the picture of a friendly monster."));
		trump_summoning(1, TRUE, p_ptr->y, p_ptr->x, (dun_level * 3 / 2), SUMMON_BAT, 0L);
	}
	else if (die < 86)
	{
		msg_print(_("ͧ��Ū�ʥ�󥹥����γ�����", "It's the picture of a friendly monster."));
		trump_summoning(1, TRUE, p_ptr->y, p_ptr->x, (dun_level * 3 / 2), SUMMON_VORTEX, 0L);
	}
	else if (die < 88)
	{
		msg_print(_("ͧ��Ū�ʥ�󥹥����γ�����", "It's the picture of a friendly monster."));
		trump_summoning(1, TRUE, p_ptr->y, p_ptr->x, (dun_level * 3 / 2), SUMMON_COIN_MIMIC, 0L);
	}
	else if (die < 96)
	{
		msg_print(_("�����͡դ���", "It's the Lovers."));

		if (get_aim_dir(&dir))
			charm_monster(dir, MIN(p_ptr->lev, 20));
	}
	else if (die < 101)
	{
		msg_print(_("�Ա��ԡդ���", "It's the Hermit."));
		wall_stone();
	}
	else if (die < 111)
	{
		msg_print(_("�Կ�Ƚ�դ���", "It's the Judgement."));
		do_cmd_rerate(FALSE);
		lose_all_mutations();
	}
	else if (die < 120)
	{
		msg_print(_("�����ۡդ���", "It's the Sun."));
		chg_virtue(V_KNOWLEDGE, 1);
		chg_virtue(V_ENLIGHTEN, 1);
		wiz_lite(FALSE);
	}
	else
	{
		msg_print(_("�������դ���", "It's the World."));
		if (p_ptr->exp < PY_MAX_EXP)
		{
			s32b ee = (p_ptr->exp / 25) + 1;
			if (ee > 5000) ee = 5000;
			msg_print(_("���˷и����Ѥ���褦�ʵ������롣", "You feel more experienced."));
			gain_exp(ee);
		}
	}
}

/*!
 * @brief ��������ˡ��ή�����פν����Ȥ��ƥץ쥤�䡼���濴������������10+1d10�󷫤��֤���
 * / Drop 10+1d10 meteor ball at random places near the player
 * @param dam ���᡼��
 * @param rad ���Ϥ�Ⱦ��
 * @return �ʤ�
 */
static void cast_meteor(HIT_POINT dam, int rad)
{
	int i;
	int b = 10 + randint1(10);

	for (i = 0; i < b; i++)
	{
		POSITION y = 0, x = 0;
		int count;

		for (count = 0; count <= 20; count++)
		{
			int dy, dx, d;

			x = p_ptr->x - 8 + randint0(17);
			y = p_ptr->y - 8 + randint0(17);

			dx = (p_ptr->x > x) ? (p_ptr->x - x) : (x - p_ptr->x);
			dy = (p_ptr->y > y) ? (p_ptr->y - y) : (y - p_ptr->y);

			/* Approximate distance */
			d = (dy > dx) ? (dy + (dx >> 1)) : (dx + (dy >> 1));

			if (d >= 9) continue;

			if (!in_bounds(y, x) || !projectable(p_ptr->y, p_ptr->x, y, x)
			    || !cave_have_flag_bold(y, x, FF_PROJECT)) continue;

			/* Valid position */
			break;
		}

		if (count > 20) continue;

		project(0, rad, y, x, dam, GF_METEOR, PROJECT_KILL | PROJECT_JUMP | PROJECT_ITEM, -1);
	}
}


/*!
 * @brief �˼���ˡ�ֿ����ܤ�פν����Ȥ��ƥ������åȤ���ꤷ����ʬ��Υܡ�������20��ȯ�������롣
 * @param dam ���᡼��
 * @param rad ���Ϥ�Ⱦ��
 * @return �������åȤ���ꤷ���¹Ԥ����ʤ��TRUE���֤���
 */
static bool cast_wrath_of_the_god(HIT_POINT dam, int rad)
{
	int x, y, tx, ty;
	int nx, ny;
	int dir, i;
	int b = 10 + randint1(10);

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

	x = p_ptr->x;
	y = p_ptr->y;

	while (1)
	{
		/* Hack -- Stop at the target */
		if ((y == ty) && (x == tx)) break;

		ny = y;
		nx = x;
		mmove2(&ny, &nx, p_ptr->y, p_ptr->x, ty, tx);

		/* Stop at maximum range */
		if (MAX_RANGE <= distance(p_ptr->y, p_ptr->x, ny, nx)) break;

		/* Stopped by walls/doors */
		if (!cave_have_flag_bold(ny, nx, FF_PROJECT)) break;

		/* Stopped by monsters */
		if ((dir != 5) && cave[ny][nx].m_idx != 0) break;

		/* Save the new location */
		x = nx;
		y = ny;
	}
	tx = x;
	ty = y;

	for (i = 0; i < b; i++)
	{
		int count = 20, d = 0;

		while (count--)
		{
			int dx, dy;

			x = tx - 5 + randint0(11);
			y = ty - 5 + randint0(11);

			dx = (tx > x) ? (tx - x) : (x - tx);
			dy = (ty > y) ? (ty - y) : (y - ty);

			/* Approximate distance */
			d = (dy > dx) ? (dy + (dx >> 1)) : (dx + (dy >> 1));
			/* Within the radius */
			if (d < 5) break;
		}

		if (count < 0) continue;

		/* Cannot penetrate perm walls */
		if (!in_bounds(y,x) ||
		    cave_stop_disintegration(y,x) ||
		    !in_disintegration_range(ty, tx, y, x))
			continue;

		project(0, rad, y, x, dam, GF_DISINTEGRATE, PROJECT_JUMP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL, -1);
	}

	return TRUE;
}

/*!
 * @brief �����ΰ�Υ��졼�����ǡ���󾤴������Ѳ�ǽ�ʻ��Τ��ɤ������֤��� / An "item_tester_hook" for offer
 * @param o_ptr ���֥������ȹ�¤�Τλ��ȥݥ���
 * @return ���Ӥ˻��Ѳ�ǽ�ʻ��Τʤ��TRUE���֤���
 */
static bool item_tester_offer(object_type *o_ptr)
{
	/* Flasks of oil are okay */
	if (o_ptr->tval != TV_CORPSE) return (FALSE);

	if (o_ptr->sval != SV_CORPSE) return (FALSE);

	if (my_strchr("pht", r_info[o_ptr->pval].d_char)) return (TRUE);

	/* Assume not okay */
	return (FALSE);
}

/*!
 * @brief �����ΰ�Υ��졼�����ǡ���󾤴���������� / Daemon spell Summon Greater Demon
 * @return ������¹Ԥ����ʤ��TRUE���֤���
 */
static bool cast_summon_greater_demon(void)
{
	PLAYER_LEVEL plev = p_ptr->lev;
	OBJECT_IDX item;
	cptr q, s;
	int summon_lev;
	object_type *o_ptr;

	item_tester_hook = item_tester_offer;
	q = _("�ɤλ��Τ������ޤ���? ", "Sacrifice which corpse? ");
	s = _("����������Τ���äƤ��ʤ���", "You have nothing to scrifice.");
	if (!get_item(&item, q, s, (USE_INVEN | USE_FLOOR))) return FALSE;

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

	summon_lev = plev * 2 / 3 + r_info[o_ptr->pval].level;

	if (summon_specific(-1, p_ptr->y, p_ptr->x, summon_lev, SUMMON_HI_DEMON, (PM_ALLOW_GROUP | PM_FORCE_PET)))
	{
		msg_print(_("β���ΰ���������������", "The area fills with a stench of sulphur and brimstone."));
		msg_print(_("�֤��ѤǤ������ޤ�����������͡�", "'What is thy bidding... Master?'"));

		/* Decrease the item (from the pack) */
		if (item >= 0)
		{
			inven_item_increase(item, -1);
			inven_item_describe(item);
			inven_item_optimize(item);
		}

		/* Decrease the item (from the floor) */
		else
		{
			floor_item_increase(0 - item, -1);
			floor_item_describe(0 - item);
			floor_item_optimize(0 - item);
		}
	}
	else
	{
		msg_print(_("����ϸ���ʤ��ä���", "No Greater Demon arrive."));
	}

	return TRUE;
}

/*!
 * @brief �Τγ��Ϥ�������� / Start singing if the player is a Bard 
 * @param spell �ΰ���ˡ�Ȥ��Ƥ�ID
 * @param song ��ˡ���̤�ID
 * @return �ʤ�
 */
static void start_singing(SPELL_IDX spell, MAGIC_NUM1 song)
{
	/* Remember the song index */
	SINGING_SONG_EFFECT(p_ptr) = (MAGIC_NUM1)song;

	/* Remember the index of the spell which activated the song */
	SINGING_SONG_ID(p_ptr) = (MAGIC_NUM2)spell;


	/* Now the player is singing */
	set_action(ACTION_SING);


	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);
}

/*!
 * @brief �Τ���ߤ�������� / Stop singing if the player is a Bard 
 * @return �ʤ�
 */
void stop_singing(void)
{
	if (p_ptr->pclass != CLASS_BARD) return;

 	/* Are there interupted song? */
	if (INTERUPTING_SONG_EFFECT(p_ptr))
	{
		/* Forget interupted song */
		INTERUPTING_SONG_EFFECT(p_ptr) = MUSIC_NONE;
		return;
	}

	/* The player is singing? */
	if (!SINGING_SONG_EFFECT(p_ptr)) return;

	/* Hack -- if called from set_action(), avoid recursive loop */
	if (p_ptr->action == ACTION_SING) set_action(ACTION_NONE);

	/* Message text of each song or etc. */
	do_spell(REALM_MUSIC, SINGING_SONG_ID(p_ptr), SPELL_STOP);

	SINGING_SONG_EFFECT(p_ptr) = MUSIC_NONE;
	SINGING_SONG_ID(p_ptr) = 0;

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);
}


/*!
 * @brief ��̿�ΰ���ˡ�γƽ�����Ԥ�
 * @param spell ��ˡID
 * @param mode �������� (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
 * @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ˤ�ʸ����ݥ��󥿤��֤���SPELL_CAST����NULLʸ������֤���
 */
static cptr do_life_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("�ڽ��μ���", "Cure Light Wounds");
		if (desc) return _("��������Ϥ򾯤����������롣", "Heals cut and HP a little.");
		{
			int dice = 2;
			int sides = 10;

			if (info) return info_heal(dice, sides, 0);

			if (cast)
			{
				hp_player(damroll(dice, sides));
				set_cut(p_ptr->cut - 10);
			}
		}
		break;

	case 1:
		if (name) return _("��ʡ", "Bless");
		if (desc) return _("������֡�̿��Ψ��AC�˥ܡ��ʥ������롣", "Gives bonus to hit and AC for a few turns.");
		{
			int base = 12;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_blessed(randint1(base) + base, FALSE);
			}
		}
		break;

	case 2:
		if (name) return _("�ڽ�", "Cause Light Wounds");
		if (desc) return _("1�ΤΥ�󥹥����˾����᡼����Ϳ���롣�񹳤�����̵����", "Wounds a monster a little unless resisted.");
		{
			int dice = 3 + (plev - 1) / 5;
			int sides = 4;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_ball_hide(GF_WOUNDS, dir, damroll(dice, sides), 0);
			}
		}
		break;

	case 3:
		if (name) return _("���ξ���", "Call Light");
		if (desc) return _("�������Ȥ餷�Ƥ����ϰϤ��������Τ�ʵפ����뤯���롣", "Lights up nearby area and the inside of a room permanently.");
		{
			int dice = 2;
			int sides = plev / 2;
			int rad = plev / 10 + 1;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				lite_area(damroll(dice, sides), rad);
			}
		}
		break;

	case 4:
		if (name) return _("� & �����ⴶ��", "Detect Doors & Traps");
		if (desc) return _("�᤯�����Ƥ�櫤���ȳ��ʤ��Τ��롣", "Detects traps, doors, and stairs in your vicinity.");
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_traps(rad, TRUE);
				detect_doors(rad);
				detect_stairs(rad);
			}
		}
		break;

	case 5:
		if (name) return _("�Ž��μ���", "Cure Medium Wounds");
		if (desc) return _("��������Ϥ������ٲ��������롣", "Heals cut and HP more.");
		{
			int dice = 4;
			int sides = 10;

			if (info) return info_heal(dice, sides, 0);

			if (cast)
			{
				hp_player(damroll(dice, sides));
				set_cut((p_ptr->cut / 2) - 20);
			}
		}
		break;

	case 6:
		if (name) return _("����", "Cure Poison");
		if (desc) return _("������Ǥ��������", "Cure poison status.");
		{
			if (cast)
			{
				set_poisoned(0);
			}
		}
		break;

	case 7:
		if (name) return _("��ʢ��­", "Satisfy Hunger");
		if (desc) return _("��ʢ�ˤ��롣", "Satisfies hunger.");
		{
			if (cast)
			{
				set_food(PY_FOOD_MAX - 1);
			}
		}
		break;

	case 8:
		if (name) return _("���", "Remove Curse");
		if (desc) return _("�����ƥ�ˤ����ä��夤�����������롣", "Removes normal curses from equipped items.");
		{
			if (cast)
			{
				if (remove_curse())
				{
					msg_print(_("ï���˸�����Ƥ���褦�ʵ������롣", "You feel as if someone is watching over you."));
				}
			}
		}
		break;

	case 9:
		if (name) return _("�Ž�", "Cause Medium Wounds");
		if (desc) return _("1�ΤΥ�󥹥���������᡼����Ϳ���롣�񹳤�����̵����", "Wounds a monster unless resisted.");
		{
			int sides = 8 + (plev - 5) / 4;
			int dice = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_ball_hide(GF_WOUNDS, dir, damroll(dice, sides), 0);
			}
		}
		break;

	case 10:
		if (name) return _("��̿���μ���", "Cure Critical Wounds");
		if (desc) return _("���Ϥ������˲��������������ۯ۰���֤��������롣", "Heals cut, stun and HP greatly.");
		{
			int dice = 8;
			int sides = 10;

			if (info) return info_heal(dice, sides, 0);

			if (cast)
			{
				hp_player(damroll(dice, sides));
				set_stun(0);
				set_cut(0);
			}
		}
		break;

	case 11:
		if (name) return _("��Ǯ�Ѵ�", "Resist Heat and Cold");
		if (desc) return _("������֡��б���䵤���Ф������������롣�����ˤ�����������Ѥ��롣", 
			"Gives resistance to fire and cold. These resistances can be added to which from equipment for more powerful resistances.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_cold(randint1(base) + base, FALSE);
				set_oppose_fire(randint1(base) + base, FALSE);
			}
		}
		break;

	case 12:
		if (name) return _("���մ���", "Sense Surroundings");
		if (desc) return _("���դ��Ϸ����Τ��롣", "Maps nearby area.");
    
		{
			int rad = DETECT_RAD_MAP;

			if (info) return info_radius(rad);

			if (cast)
			{
				map_area(rad);
			}
		}
		break;

	case 13:
		if (name) return _("�ѥ˥å�������ǥå�", "Turn Undead");
		if (desc) return _("�볦��Υ���ǥåɤ��ݤ����롣�񹳤�����̵����", "Attempts to scare undead monsters in sight.");
    
		{
			if (cast)
			{
				turn_undead();
			}
		}
		break;

	case 14:
		if (name) return _("���ϲ���", "Healing");
		if (desc) return _("�ˤ�ƶ��Ϥʲ�����ʸ�ǡ������ۯ۰���֤��������롣", "Much powerful healing magic, and heals cut and stun completely.");
    
		{
			int heal = 300;

			if (info) return info_heal(0, 0, heal);

			if (cast)
			{
				hp_player(heal);
				set_stun(0);
				set_cut(0);
			}
		}
		break;

	case 15:
		if (name) return _("�볦�����", "Glyph of Warding");
		if (desc) return _("��ʬ�Τ��뾲�ξ�ˡ���󥹥������̤�ȴ�����꾤�����줿�ꤹ�뤳�Ȥ��Ǥ��ʤ��ʤ�롼���������",
			"Sets a glyph on the floor beneath you. Monsters cannot attack you if you are on a glyph, but can try to break glyph.");
    
		{
			if (cast)
			{
				warding_glyph();
			}
		}
		break;

	case 16:
		if (name) return _("*���*", "Dispel Curse");
		if (desc) return _("�����ƥ�ˤ����ä����Ϥʼ����������롣", "Removes normal and heavy curse from equipped items.");
    
		{
			if (cast)
			{
				if (remove_all_curse())
				{
					msg_print(_("ï���˸�����Ƥ���褦�ʵ������롣", "You feel as if someone is watching over you."));
				}
			}
		}
		break;

	case 17:
		if (name) return _("�ռ�", "Perception");
		if (desc) return _("�����ƥ���̤��롣", "Identifies an item.");
    
		{
			if (cast)
			{
				if (!ident_spell(FALSE)) return NULL;
			}
		}
		break;

	case 18:
		if (name) return _("����ǥå��໶", "Dispel Undead");
		if (desc) return _("�볦������ƤΥ���ǥåɤ˥��᡼����Ϳ���롣", "Damages all undead monsters in sight.");
    
		{
			int dice = 1;
			int sides = plev * 5;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				dispel_undead(damroll(dice, sides));
			}
		}
		break;

	case 19:
		if (name) return _("��ι�", "Day of the Dove");
		if (desc) return _("�볦������ƤΥ�󥹥�����̥λ���롣�񹳤�����̵����", "Attempts to charm all monsters in sight.");
    
		{
			int power = plev * 2;

			if (info) return info_power(power);

			if (cast)
			{
				charm_monsters(power);
			}
		}
		break;

	case 20:
		if (name) return _("��̿��", "Cause Critical Wounds");
		if (desc) return _("1�ΤΥ�󥹥���������᡼����Ϳ���롣�񹳤�����̵����", "Wounds a monster critically unless resisted.");
    
		{
			int dice = 5 + (plev - 5) / 3;
			int sides = 15;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_ball_hide(GF_WOUNDS, dir, damroll(dice, sides), 0);
			}
		}
		break;

	case 21:
		if (name) return _("���Ԥξ�", "Word of Recall");
		if (desc) return _("�Ͼ�ˤ���Ȥ��ϥ��󥸥��κǿ����ء����󥸥��ˤ���Ȥ����Ͼ�ؤȰ�ư���롣", "Recalls player from dungeon to town, or from town to the deepest level of dungeon.");
    
		{
			int base = 15;
			int sides = 20;

			if (info) return info_delay(base, sides);

			if (cast)
			{
				if (!word_of_recall()) return NULL;
			}
		}
		break;

	case 22:
		if (name) return _("���¤κ���", "Alter Reality");
		if (desc) return _("���ߤγ���ƹ������롣", "Recreates current dungeon level.");
    
		{
			int base = 15;
			int sides = 20;

			if (info) return info_delay(base, sides);

			if (cast)
			{
				alter_reality();
			}
		}
		break;

	case 23:
		if (name) return _("�����볦", "Warding True");
		if (desc) return _("��ʬ�Τ��뾲�ȼ���8�ޥ��ξ��ξ�ˡ���󥹥������̤�ȴ�����꾤�����줿�ꤹ�뤳�Ȥ��Ǥ��ʤ��ʤ�롼���������", "Creates glyphs in all adjacent squares and under you.");
    
		{
			int rad = 1;

			if (info) return info_radius(rad);

			if (cast)
			{
				warding_glyph();
				glyph_creation();
			}
		}
		break;

	case 24:
		if (name) return _("���Ӳ�", "Sterilization");
		if (desc) return _("���γ������������󥹥����������Ǥ��ʤ��ʤ롣", "Prevents any breeders on current level from breeding.");
    
		{
			if (cast)
			{
				num_repro += MAX_REPRO;
			}
		}
		break;

	case 25:
		if (name) return _("������", "Detection");
		if (desc) return _("�᤯�����ƤΥ�󥹥�����櫡��⡢���ʡ������������ƥ����ƥ���Τ��롣", "Detects all monsters, traps, doors, stairs, treasures and items in your vicinity.");

		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_all(rad);
			}
		}
		break;

	case 26:
		if (name) return _("����ǥåɾ���", "Annihilate Undead");
		if (desc) return _("��ʬ�μ��Ϥˤ��륢��ǥåɤ򸽺ߤγ�����ä���롣�񹳤�����̵����",
			"Eliminates all nearby undead monsters, exhausting you.  Powerful or unique monsters may be able to resist.");
    
		{
			int power = plev + 50;

			if (info) return info_power(power);

			if (cast)
			{
				mass_genocide_undead(power, TRUE);
			}
		}
		break;

	case 27:
		if (name) return _("��Τ��", "Clairvoyance");
		if (desc) return _("���γ����Τ�ʵפ˾Ȥ餷�����󥸥���⤹�٤ƤΥ����ƥ���Τ��롣", "Maps and lights whole dungeon level. Knows all objects location. And gives telepathy for a while.");
    
		{
			if (cast)
			{
				wiz_lite(FALSE);
			}
		}
		break;

	case 28:
		if (name) return _("������", "Restoration");
		if (desc) return _("���٤ƤΥ��ơ������ȷи��ͤ�������롣", "Restores all stats and experience.");
    
		{
			if (cast)
			{
				do_res_stat(A_STR);
				do_res_stat(A_INT);
				do_res_stat(A_WIS);
				do_res_stat(A_DEX);
				do_res_stat(A_CON);
				do_res_stat(A_CHR);
				restore_level();
			}
		}
		break;

	case 29:
		if (name) return _("*���ϲ���*", "Healing True");
		if (desc) return _("�Ƕ��μ�������ˡ�ǡ������ۯ۰���֤��������롣", "The greatest healing magic. Heals all HP, cut and stun.");
    
		{
			int heal = 2000;

			if (info) return info_heal(0, 0, heal);

			if (cast)
			{
				hp_player(heal);
				set_stun(0);
				set_cut(0);
			}
		}
		break;

	case 30:
		if (name) return _("���ʤ�ӥ����", "Holy Vision");
		if (desc) return _("�����ƥ�λ���ǽ�Ϥ������Τ롣", "*Identifies* an item.");
    
		{
			if (cast)
			{
				if (!identify_fully(FALSE)) return NULL;
			}
		}
		break;

	case 31:
		if (name) return _("��ˤ�����", "Ultimate Resistance");
		if (desc) return _("������֡��������������դ���AC����ˡ�ɸ�ǽ�Ϥ�徺�����롣", "Gives ultimate resistance, bonus to AC and speed.");
    
		{
			TIME_EFFECT base = (TIME_EFFECT)plev / 2;

			if (info) return info_duration(base, base);

			if (cast)
			{
				TIME_EFFECT v = randint1(base) + base;
				set_fast(v, FALSE);
				set_oppose_acid(v, FALSE);
				set_oppose_elec(v, FALSE);
				set_oppose_fire(v, FALSE);
				set_oppose_cold(v, FALSE);
				set_oppose_pois(v, FALSE);
				set_ultimate_res(v, FALSE);
			}
		}
		break;
	}

	return "";
}

/*!
 * @brief ����ΰ���ˡ�γƽ�����Ԥ�
 * @param spell ��ˡID
 * @param mode �������� (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
 * @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ˤ�ʸ����ݥ��󥿤��֤���SPELL_CAST����NULLʸ������֤���
 */
static cptr do_sorcery_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("��󥹥�������", "Detect Monsters");
		if (desc) return _("�᤯�����Ƥθ������󥹥������Τ��롣", "Detects all monsters in your vicinity unless invisible.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_monsters_normal(rad);
			}
		}
		break;

	case 1:
		if (name) return _("���硼�ȡ��ƥ�ݡ���", "Phase Door");
		if (desc) return _("���Υ�Υƥ�ݡ��Ȥ򤹤롣", "Teleport short distance.");
    
		{
			POSITION range = 10;

			if (info) return info_range(range);

			if (cast)
			{
				teleport_player(range, 0L);
			}
		}
		break;

	case 2:
		if (name) return _("櫤��ⴶ��", "Detect Doors and Traps");
		if (desc) return _("�᤯�����Ƥ����櫤��Τ��롣", "Detects traps, doors, and stairs in your vicinity.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_traps(rad, TRUE);
				detect_doors(rad);
				detect_stairs(rad);
			}
		}
		break;

	case 3:
		if (name) return _("�饤�ȡ����ꥢ", "Light Area");
		if (desc) return _("�������Ȥ餷�Ƥ����ϰϤ��������Τ�ʵפ����뤯���롣", "Lights up nearby area and the inside of a room permanently.");
    
		{
			int dice = 2;
			int sides = plev / 2;
			int rad = plev / 10 + 1;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				lite_area(damroll(dice, sides), rad);
			}
		}
		break;

	case 4:
		if (name) return _("�ѥ˥å�����󥹥���", "Confuse Monster");
		if (desc) return _("��󥹥���1�Τ��𤵤��롣�񹳤�����̵����", "Attempts to confuse a monster.");
    
		{
			int power = (plev * 3) / 2;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				confuse_monster(dir, power);
			}
		}
		break;

	case 5:
		if (name) return _("�ƥ�ݡ���", "Teleport");
		if (desc) return _("���Υ�Υƥ�ݡ��Ȥ򤹤롣", "Teleport long distance.");
    
		{
			POSITION range = plev * 5;

			if (info) return info_range(range);

			if (cast)
			{
				teleport_player(range, 0L);
			}
		}
		break;

	case 6:
		if (name) return _("���꡼�ס���󥹥���", "Sleep Monster");
		if (desc) return _("��󥹥���1�Τ�̲�餻�롣�񹳤�����̵����", "Attempts to sleep a monster.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				sleep_monster(dir, plev);
			}
		}
		break;

	case 7:
		if (name) return _("���Ͻ�Ŷ", "Recharging");
		if (desc) return _("��/��ˡ���ν�Ŷ��������䤹������Ŷ��Υ�åɤν�Ŷ���֤򸺤餹��", "Recharges staffs, wands or rods.");
    
		{
			int power = plev * 4;

			if (info) return info_power(power);

			if (cast)
			{
				if (!recharge(power)) return NULL;
			}
		}
		break;

	case 8:
		if (name) return _("��ˡ���Ͽ�", "Magic Mapping");
		if (desc) return _("���դ��Ϸ����Τ��롣", "Maps nearby area.");
    
		{
			int rad = DETECT_RAD_MAP;

			if (info) return info_radius(rad);

			if (cast)
			{
				map_area(rad);
			}
		}
		break;

	case 9:
		if (name) return _("����", "Identify");
		if (desc) return _("�����ƥ���̤��롣", "Identifies an item.");
    
		{
			if (cast)
			{
				if (!ident_spell(FALSE)) return NULL;
			}
		}
		break;

	case 10:
		if (name) return _("��������󥹥���", "Slow Monster");
		if (desc) return _("��󥹥���1�Τ�®���롣�񹳤�����̵����", "Attempts to slow a monster.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				slow_monster(dir, plev);
			}
		}
		break;

	case 11:
		if (name) return _("���ե��꡼��", "Mass Sleep");
		if (desc) return _("�볦������ƤΥ�󥹥�����̲�餻�롣�񹳤�����̵����", "Attempts to sleep all monsters in sight.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				sleep_monsters(plev);
			}
		}
		break;

	case 12:
		if (name) return _("�ƥ�ݡ��ȡ���󥹥���", "Teleport Away");
		if (desc) return _("��󥹥�����ƥ�ݡ��Ȥ�����ӡ�������ġ��񹳤�����̵����", "Teleports all monsters on the line away unless resisted.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_beam(GF_AWAY_ALL, dir, power);
			}
		}
		break;

	case 13:
		if (name) return _("���ԡ���", "Haste Self");
		if (desc) return _("������֡���®���롣", "Hastes you for a while.");
    
		{
			int base = plev;
			int sides = 20 + plev;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_fast(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 14:
		if (name) return _("��������", "Detection True");
		if (desc) return _("�᤯�����ƤΥ�󥹥�����櫡��⡢���ʡ������������ƥ����ƥ���Τ��롣",
			"Detects all monsters, traps, doors, stairs, treasures and items in your vicinity.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_all(rad);
			}
		}
		break;

	case 15:
		if (name) return _("��������", "Identify True");
		if (desc) return _("�����ƥ�λ���ǽ�Ϥ������Τ롣", "*Identifies* an item.");
    
		{
			if (cast)
			{
				if (!identify_fully(FALSE)) return NULL;
			}
		}
		break;

	case 16:
		if (name) return _("ʪ�ΤȺ�������", "Detect items and Treasure");
		if (desc) return _("�᤯�����ƤΥ����ƥ�Ⱥ������Τ��롣", "Detects all treasures and items in your vicinity.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_objects_normal(rad);
				detect_treasure(rad);
				detect_objects_gold(rad);
			}
		}
		break;

	case 17:
		if (name) return _("���㡼�ࡦ��󥹥���", "Charm Monster");
		if (desc) return _("��󥹥���1�Τ�̥λ���롣�񹳤�����̵����", "Attempts to charm a monster.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				charm_monster(dir, power);
			}
		}
		break;

	case 18:
		if (name) return _("��������", "Sense Minds");
		if (desc) return _("������֡��ƥ�ѥ���ǽ�Ϥ����롣", "Gives telepathy for a while.");
    
		{
			int base = 25;
			int sides = 30;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_tim_esp(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 19:
		if (name) return _("����ư", "Teleport to town");
		if (desc) return _("���ذ�ư���롣�Ͼ�ˤ���Ȥ������Ȥ��ʤ���", "Teleport to a town which you choose in a moment. Can only be used outdoors.");
    
		{
			if (cast)
			{
				if (!tele_town()) return NULL;
			}
		}
		break;

	case 20:
		if (name) return _("����ʬ��", "Self Knowledge");
		if (desc) return _("���ߤμ�ʬ�ξ��֤������Τ롣",
			"Gives you useful info regarding your current resistances, the powers of your weapon and maximum limits of your stats.");
    
		{
			if (cast)
			{
				self_knowledge();
			}
		}
		break;

	case 21:
		if (name) return _("�ƥ�ݡ��ȡ���٥�", "Teleport Level");
		if (desc) return _("�ֻ��˾夫���γ��˥ƥ�ݡ��Ȥ��롣", "Teleport to up or down stairs in a moment.");
    
		{
			if (cast)
			{
				if (!get_check(_("������¾�γ��˥ƥ�ݡ��Ȥ��ޤ�����", "Are you sure? (Teleport Level)"))) return NULL;
				teleport_level(0);
			}
		}
		break;

	case 22:
		if (name) return _("���Ԥμ�ʸ", "Word of Recall");
		if (desc) return _("�Ͼ�ˤ���Ȥ��ϥ��󥸥��κǿ����ء����󥸥��ˤ���Ȥ����Ͼ�ؤȰ�ư���롣", 
			"Recalls player from dungeon to town, or from town to the deepest level of dungeon.");
    
		{
			int base = 15;
			int sides = 20;

			if (info) return info_delay(base, sides);

			if (cast)
			{
				if (!word_of_recall()) return NULL;
			}
		}
		break;

	case 23:
		if (name) return _("��������", "Dimension Door");
		if (desc) return _("û��Υ��λ��ꤷ�����˥ƥ�ݡ��Ȥ��롣", "Teleport to given location.");
    
		{
			POSITION range = plev / 2 + 10;

			if (info) return info_range(range);

			if (cast)
			{
				msg_print(_("�������⤬����������Ū�Ϥ�����ǲ�������", "You open a dimensional gate. Choose a destination."));
				if (!dimension_door()) return NULL;
			}
		}
		break;

	case 24:
		if (name) return _("Ĵ��", "Probing");
		if (desc) return _("��󥹥�����°�����Ĥ����ϡ��������ϡ����ԡ��ɡ����Τ��Τ롣",
			"Proves all monsters' alignment, HP, speed and their true character.");
    
		{
			if (cast)
			{
				probing();
			}
		}
		break;

	case 25:
		if (name) return _("��ȯ�Υ롼��", "Explosive Rune");
		if (desc) return _("��ʬ�Τ��뾲�ξ�ˡ���󥹥������̤����ȯ���ƥ��᡼����Ϳ����롼���������", 
			"Sets a glyph under you. The glyph will explode when a monster moves on it.");
    
		{
			int dice = 7;
			int sides = 7;
			int base = plev;

			if (info) return info_damage(dice, sides, base);

			if (cast)
			{
				explosive_rune();
			}
		}
		break;

	case 26:
		if (name) return _("ǰư��", "Telekinesis");
		if (desc) return _("�����ƥ��ʬ��­���ذ�ư�����롣", "Pulls a distant item close to you.");
    
		{
			int weight = plev * 15;

			if (info) return info_weight(weight);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fetch(dir, weight, FALSE);
			}
		}
		break;

	case 27:
		if (name) return _("��Τ��", "Clairvoyance");
		if (desc) return _("���γ����Τ�ʵפ˾Ȥ餷�����󥸥���⤹�٤ƤΥ����ƥ���Τ��롣����ˡ�������֥ƥ�ѥ���ǽ�Ϥ����롣",
			"Maps and lights whole dungeon level. Knows all objects location. And gives telepathy for a while.");
    
		{
			int base = 25;
			int sides = 30;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				chg_virtue(V_KNOWLEDGE, 1);
				chg_virtue(V_ENLIGHTEN, 1);

				wiz_lite(FALSE);

				if (!p_ptr->telepathy)
				{
					set_tim_esp(randint1(sides) + base, FALSE);
				}
			}
		}
		break;

	case 28:
		if (name) return _("̥λ�λ���", "Charm monsters");
		if (desc) return _("�볦������ƤΥ�󥹥�����̥λ���롣�񹳤�����̵����", "Attempts to charm all monsters in sight.");
    
		{
			int power = plev * 2;

			if (info) return info_power(power);

			if (cast)
			{
				charm_monsters(power);
			}
		}
		break;

	case 29:
		if (name) return _("ϣ���", "Alchemy");
		if (desc) return _("�����ƥ�1�Ĥ򤪶���Ѥ��롣", "Turns an item into 1/3 of its value in gold.");
    
		{
			if (cast)
			{
				if (!alchemy()) return NULL;
			}
		}
		break;

	case 30:
		if (name) return _("��ʪ����", "Banishment");
		if (desc) return _("�볦������ƤΥ�󥹥�����ƥ�ݡ��Ȥ����롣�񹳤�����̵����", "Teleports all monsters in sight away unless resisted.");
    
		{
			int power = plev * 4;

			if (info) return info_power(power);

			if (cast)
			{
				banish_monsters(power);
			}
		}
		break;

	case 31:
		if (name) return _("̵���ε�", "Globe of Invulnerability");
		if (desc) return _("������֡����᡼��������ʤ��ʤ�Хꥢ��ĥ�롣�ڤ줿�ִ֤˾������������񤹤�Τ���ա�",
			"Generates barrier which completely protect you from almost all damages. Takes a few your turns when the barrier breaks or duration time is exceeded.");
    
		{
			int base = 4;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_invuln(randint1(base) + base, FALSE);
			}
		}
		break;
	}

	return "";
}


/*!
 * @brief �����ΰ���ˡ�γƽ�����Ԥ�
 * @param spell ��ˡID
 * @param mode �������� (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
 * @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ˤ�ʸ����ݥ��󥿤��֤���SPELL_CAST����NULLʸ������֤���
 */
static cptr do_nature_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;

	static const char s_dam[] = _("»��:", "dam ");
	static const char s_rng[] = _("����", "rng ");

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("��󥹥�������", "Detect Creatures");
		if (desc) return _("�᤯�����Ƥθ������󥹥������Τ��롣", "Detects all monsters in your vicinity unless invisible.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_monsters_normal(rad);
			}
		}
		break;

	case 1:
		if (name) return _("���", "Lightning");
		if (desc) return _("�ŷ��û���ӡ�������ġ�", "Fires a short beam of lightning.");
    
		{
			int dice = 3 + (plev - 1) / 5;
			int sides = 4;
			POSITION range = plev / 6 + 2;

			if (info) return format("%s%dd%d %s%d", s_dam, dice, sides, s_rng, range);

			if (cast)
			{
				project_length = range;

				if (!get_aim_dir(&dir)) return NULL;

				fire_beam(GF_ELEC, dir, damroll(dice, sides));
			}
		}
		break;

	case 2:
		if (name) return _("櫤��ⴶ��", "Detect Doors and Traps");
		if (desc) return _("�᤯�����Ƥ�櫤�����Τ��롣", "Detects traps, doors, and stairs in your vicinity.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_traps(rad, TRUE);
				detect_doors(rad);
				detect_stairs(rad);
			}
		}
		break;

	case 3:
		if (name) return _("��������", "Produce Food");
		if (desc) return _("�������ĺ��Ф���", "Produces a Ration of Food.");
    
		{
			if (cast)
			{
				object_type forge, *q_ptr = &forge;
				msg_print(_("����������������", "A food ration is produced."));

				/* Create the food ration */
				object_prep(q_ptr, lookup_kind(TV_FOOD, SV_FOOD_RATION));

				/* Drop the object from heaven */
				drop_near(q_ptr, -1, p_ptr->y, p_ptr->x);
			}
		}
		break;

	case 4:
		if (name) return _("���θ�", "Daylight");
		if (desc) return _("�������Ȥ餷�Ƥ����ϰϤ��������Τ�ʵפ����뤯���롣", "Lights up nearby area and the inside of a room permanently.");
    
		{
			int dice = 2;
			int sides = plev / 2;
			int rad = (plev / 10) + 1;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				lite_area(damroll(dice, sides), rad);

				if ((prace_is_(RACE_VAMPIRE) || (p_ptr->mimic_form == MIMIC_VAMPIRE)) && !p_ptr->resist_lite)
				{
					msg_print(_("���θ������ʤ������Τ�Ǥ�������", "The daylight scorches your flesh!"));
					take_hit(DAMAGE_NOESCAPE, damroll(2, 2), _("���θ�", "daylight"), -1);
				}
			}
		}
		break;

	case 5:
		if (name) return _("ưʪ����", "Animal Taming");
		if (desc) return _("ưʪ1�Τ�̥λ���롣�񹳤�����̵����", "Attempts to charm an animal.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				charm_animal(dir, power);
			}
		}
		break;

	case 6:
		if (name) return _("�Ķ��ؤ�����", "Resist Environment");
		if (desc) return _("������֡��䵤���ꡢ�ŷ���Ф������������롣�����ˤ�����������Ѥ��롣",
			"Gives resistance to fire, cold and electricity for a while. These resistances can be added to which from equipment for more powerful resistances.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_cold(randint1(base) + base, FALSE);
				set_oppose_fire(randint1(base) + base, FALSE);
				set_oppose_elec(randint1(base) + base, FALSE);
			}
		}
		break;

	case 7:
		if (name) return _("�����Ǽ���", "Cure Wounds & Poison");
		if (desc) return _("����������������Ǥ��Τ��鴰���˼����������Ϥ򾯤����������롣", "Heals all cut and poison status. Heals HP a little.");
    
		{
			int dice = 2;
			int sides = 8;

			if (info) return info_heal(dice, sides, 0);

			if (cast)
			{
				hp_player(damroll(dice, sides));
				set_cut(0);
				set_poisoned(0);
			}
		}
		break;

	case 8:
		if (name) return _("�����ϲ�", "Stone to Mud");
		if (desc) return _("�ɤ��Ϥ����ƾ��ˤ��롣", "Turns one rock square to mud.");
    
		{
			int dice = 1;
			int sides = 30;
			int base = 20;

			if (info) return info_damage(dice, sides, base);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				wall_to_mud(dir, 20 + randint1(30));
			}
		}
		break;

	case 9:
		if (name) return _("���������ܥ��", "Frost Bolt");
		if (desc) return _("�䵤�Υܥ�Ȥ⤷���ϥӡ�������ġ�", "Fires a bolt or beam of cold.");
    
		{
			int dice = 3 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_bolt_or_beam(beam_chance() - 10, GF_COLD, dir, damroll(dice, sides));
			}
		}
		break;

	case 10:
		if (name) return _("�����γ���", "Nature Awareness");
		if (desc) return _("���դ��Ϸ����Τ����᤯��櫡��⡢���ʡ����ƤΥ�󥹥������Τ��롣",
			"Maps nearby area. Detects all monsters, traps, doors and stairs.");
    
		{
			int rad1 = DETECT_RAD_MAP;
			int rad2 = DETECT_RAD_DEFAULT;

			if (info) return info_radius(MAX(rad1, rad2));

			if (cast)
			{
				map_area(rad1);
				detect_traps(rad2, TRUE);
				detect_doors(rad2);
				detect_stairs(rad2);
				detect_monsters_normal(rad2);
			}
		}
		break;

	case 11:
		if (name) return _("�ե��������ܥ��", "Fire Bolt");
		if (desc) return _("�б�Υܥ�Ȥ⤷���ϥӡ�������ġ�", "Fires a bolt or beam of fire.");
    
		{
			int dice = 5 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_bolt_or_beam(beam_chance() - 10, GF_FIRE, dir, damroll(dice, sides));
			}
		}
		break;

	case 12:
		if (name) return _("���۸���", "Ray of Sunlight");
		if (desc) return _("���������ġ�����������󥹥����˸��̤����롣", "Fires a beam of light which damages to light-sensitive monsters.");
    
		{
			int dice = 6;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				msg_print(_("���۸��������줿��", "A line of sunlight appears."));
				lite_line(dir, damroll(6, 8));
			}
		}
		break;

	case 13:
		if (name) return _("­����", "Entangle");
		if (desc) return _("�볦������ƤΥ�󥹥�����®�����롣�񹳤�����̵����", "Attempts to slow all monsters in sight.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				slow_monsters(plev);
			}
		}
		break;

	case 14:
		if (name) return _("ưʪ����", "Summon Animal");
		if (desc) return _("ưʪ��1�ξ������롣", "Summons an animal.");
    
		{
			if (cast)
			{
				if (!(summon_specific(-1, p_ptr->y, p_ptr->x, plev, SUMMON_ANIMAL_RANGER, (PM_ALLOW_GROUP | PM_FORCE_PET))))
				{
					msg_print(_("ưʪ�ϸ���ʤ��ä���", "No animals arrive."));
				}
				break;
			}
		}
		break;

	case 15:
		if (name) return _("������", "Herbal Healing");
		if (desc) return _("���Ϥ������˲��������������ۯ۰���֡��Ǥ����������롣", "Heals HP greatly. And heals cut, stun and poison completely.");
    
		{
			int heal = 500;

			if (info) return info_heal(0, 0, heal);

			if (cast)
			{
				hp_player(heal);
				set_stun(0);
				set_cut(0);
				set_poisoned(0);
			}
		}
		break;

	case 16:
		if (name) return _("��������", "Stair Building");
		if (desc) return _("��ʬ�Τ�����֤˳��ʤ��롣", "Creates a stair which goes down or up.");
    
		{
			if (cast)
			{
				stair_creation();
			}
		}
		break;

	case 17:
		if (name) return _("ȩ�в�", "Stone Skin");
		if (desc) return _("������֡�AC��徺�����롣", "Gives bonus to AC for a while.");
    
		{
			int base = 20;
			int sides = 30;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_shield(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 18:
		if (name) return _("��������", "Resistance True");
		if (desc) return _("������֡������ŷ⡢�ꡢ�䵤���Ǥ��Ф������������롣�����ˤ�����������Ѥ��롣", 
			"Gives resistance to fire, cold, electricity, acid and poison for a while. These resistances can be added to which from equipment for more powerful resistances.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_acid(randint1(base) + base, FALSE);
				set_oppose_elec(randint1(base) + base, FALSE);
				set_oppose_fire(randint1(base) + base, FALSE);
				set_oppose_cold(randint1(base) + base, FALSE);
				set_oppose_pois(randint1(base) + base, FALSE);
			}
		}
		break;

	case 19:
		if (name) return _("������¤", "Forest Creation");
		if (desc) return _("���Ϥ��ڤ���Ф���", "Creates trees in all adjacent squares.");
    
		{
			if (cast)
			{
				tree_creation();
			}
		}
		break;

	case 20:
		if (name) return _("ưʪͧ��", "Animal Friendship");
		if (desc) return _("�볦������Ƥ�ưʪ��̥λ���롣�񹳤�����̵����", "Attempts to charm all animals in sight.");
    
		{
			int power = plev * 2;

			if (info) return info_power(power);

			if (cast)
			{
				charm_animals(power);
			}
		}
		break;

	case 21:
		if (name) return _("�����", "Stone Tell");
		if (desc) return _("�����ƥ�λ���ǽ�Ϥ������Τ롣", "*Identifies* an item.");
    
		{
			if (cast)
			{
				if (!identify_fully(FALSE)) return NULL;
			}
		}
		break;

	case 22:
		if (name) return _("�Ф���", "Wall of Stone");
		if (desc) return _("��ʬ�μ��Ϥ˲�־����ɤ��롣", "Creates granite walls in all adjacent squares.");
    
		{
			if (cast)
			{
				wall_stone();
			}
		}
		break;

	case 23:
		if (name) return _("�忩�ɻ�", "Protect from Corrosion");
		if (desc) return _("�����ƥ����ǽ��Ĥ��ʤ��褦�ù����롣", "Makes an equipment acid-proof.");
    
		{
			if (cast)
			{
				if (!rustproof()) return NULL;
			}
		}
		break;

	case 24:
		if (name) return _("�Ͽ�", "Earthquake");
		if (desc) return _("���ϤΥ��󥸥����ɤ餷���ɤȾ��������������Ѥ��롣", 
			"Shakes dungeon structure, and results in random swapping of floors and walls.");
    
		{
			int rad = 10;

			if (info) return info_radius(rad);

			if (cast)
			{
				earthquake(p_ptr->y, p_ptr->x, rad);
			}
		}
		break;

	case 25:
		if (name) return _("���ޥ�����", "Cyclone");
		if (desc) return _("�������˸����äƹ��⤹�롣", "Attacks all adjacent monsters.");
    
		{
			if (cast)
			{
				int y = 0, x = 0;
				cave_type       *c_ptr;
				monster_type    *m_ptr;

				for (dir = 0; dir < 8; dir++)
				{
					y = p_ptr->y + ddy_ddd[dir];
					x = p_ptr->x + ddx_ddd[dir];
					c_ptr = &cave[y][x];

					/* Get the monster */
					m_ptr = &m_list[c_ptr->m_idx];

					/* Hack -- attack monsters */
					if (c_ptr->m_idx && (m_ptr->ml || cave_have_flag_bold(y, x, FF_PROJECT)))
						py_attack(y, x, 0);
				}
			}
		}
		break;

	case 26:
		if (name) return _("�֥ꥶ����", "Blizzard");
		if (desc) return _("������䵤�ε�����ġ�", "Fires a huge ball of cold.");
    
		{
			HIT_POINT dam = 70 + plev * 3 / 2;
			int rad = plev / 12 + 1;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_COLD, dir, dam, rad);
			}
		}
		break;

	case 27:
		if (name) return _("�����", "Lightning Storm");
		if (desc) return _("������ŷ�ε�����ġ�", "Fires a huge electric ball.");
    
		{
			HIT_POINT dam = 90 + plev * 3 / 2;
			int rad = plev / 12 + 1;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_ball(GF_ELEC, dir, dam, rad);
				break;
			}
		}
		break;

	case 28:
		if (name) return _("��Ĭ", "Whirlpool");
		if (desc) return _("����ʿ�ε�����ġ�", "Fires a huge ball of water.");
    
		{
			HIT_POINT dam = 100 + plev * 3 / 2;
			int rad = plev / 12 + 1;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_ball(GF_WATER, dir, dam, rad);
			}
		}
		break;

	case 29:
		if (name) return _("�۸�����", "Call Sunlight");
		if (desc) return _("��ʬ���濴�Ȥ������ε��ȯ�������롣����ˡ����γ����Τ�ʵפ˾Ȥ餷�����󥸥���⤹�٤ƤΥ����ƥ���Τ��롣",
			"Generates ball of light centered on you. Maps and lights whole dungeon level. Knows all objects location.");
    
		{
			HIT_POINT dam = 150;
			int rad = 8;

			if (info) return info_damage(0, 0, dam/2);

			if (cast)
			{
				fire_ball(GF_LITE, 0, dam, rad);
				chg_virtue(V_KNOWLEDGE, 1);
				chg_virtue(V_ENLIGHTEN, 1);
				wiz_lite(FALSE);

				if ((prace_is_(RACE_VAMPIRE) || (p_ptr->mimic_form == MIMIC_VAMPIRE)) && !p_ptr->resist_lite)
				{
					msg_print(_("���������ʤ������Τ�Ǥ�������", "The sunlight scorches your flesh!"));
					take_hit(DAMAGE_NOESCAPE, 50, _("����", "sunlight"), -1);
				}
			}
		}
		break;

	case 30:
		if (name) return _("����ο�", "Elemental Branding");
		if (desc) return _("���˱꤫�䵤��°����Ĥ��롣", "Makes current weapon fire or frost branded.");
    
		{
			if (cast)
			{
				brand_weapon(randint0(2));
			}
		}
		break;

	case 31:
		if (name) return _("�����ζ���", "Nature's Wrath");
		if (desc) return _("�᤯�����ƤΥ�󥹥����˥��᡼����Ϳ�����Ͽ̤򵯤�������ʬ���濴�Ȥ���ʬ��ε��ȯ�������롣", 
			"Damages all monsters in sight. Makes quake. Generates disintegration ball centered on you.");
    
		{
			int d_dam = 4 * plev;
			int b_dam = (100 + plev) * 2;
			int b_rad = 1 + plev / 12;
			int q_rad = 20 + plev / 2;

			if (info) return format("%s%d+%d", s_dam, d_dam, b_dam/2);

			if (cast)
			{
				dispel_monsters(d_dam);
				earthquake(p_ptr->y, p_ptr->x, q_rad);
				project(0, b_rad, p_ptr->y, p_ptr->x, b_dam, GF_DISINTEGRATE, PROJECT_KILL | PROJECT_ITEM, -1);
			}
		}
		break;
	}

	return "";
}


/*!
 * @brief �������ΰ���ˡ�γƽ�����Ԥ�
 * @param spell ��ˡID
 * @param mode �������� (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
 * @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ˤ�ʸ����ݥ��󥿤��֤���SPELL_CAST����NULLʸ������֤���
 */
static cptr do_chaos_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;

	static const char s_dam[] = _("»��:", "dam ");
	static const char s_random[] = _("������", "random");

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("�ޥ��å����ߥ�����", "Magic Missile");
		if (desc) return _("�夤��ˡ��������ġ�", "Fires a weak bolt of magic.");
    
		{
			int dice = 3 + ((plev - 1) / 5);
			int sides = 4;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance() - 10, GF_MISSILE, dir, damroll(dice, sides));
			}
		}
		break;

	case 1:
		if (name) return _("�ȥ�å�/�ɥ��˲�", "Trap / Door Destruction");
		if (desc) return _("���ܤ���櫤�����˲����롣", "Destroys all traps in adjacent squares.");
    
		{
			int rad = 1;

			if (info) return info_radius(rad);

			if (cast)
			{
				destroy_doors_touch();
			}
		}
		break;

	case 2:
		if (name) return _("����", "Flash of Light");
		if (desc) return _("�������Ȥ餷�Ƥ����ϰϤ��������Τ�ʵפ����뤯���롣", "Lights up nearby area and the inside of a room permanently.");
    
		{
			int dice = 2;
			int sides = plev / 2;
			int rad = (plev / 10) + 1;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				lite_area(damroll(dice, sides), rad);
			}
		}
		break;

	case 3:
		if (name) return _("����μ�", "Touch of Confusion");
		if (desc) return _("�����𤵤��빶���Ǥ���褦�ˤ��롣", "Attempts to confuse the next monster that you hit.");
    
		{
			if (cast)
			{
				if (!(p_ptr->special_attack & ATTACK_CONFUSE))
				{
					msg_print(_("���ʤ��μ�ϸ���Ϥ᤿��", "Your hands start glowing."));
					p_ptr->special_attack |= ATTACK_CONFUSE;
					p_ptr->redraw |= (PR_STATUS);
				}
			}
		}
		break;

	case 4:
		if (name) return _("��������", "Mana Burst");
		if (desc) return _("��ˡ�ε�����ġ�", "Fires a ball of magic.");
    
		{
			int dice = 3;
			int sides = 5;
			int rad = (plev < 30) ? 2 : 3;
			int base;

			if (p_ptr->pclass == CLASS_MAGE ||
			    p_ptr->pclass == CLASS_HIGH_MAGE ||
			    p_ptr->pclass == CLASS_SORCERER)
				base = plev + plev / 2;
			else
				base = plev + plev / 4;


			if (info) return info_damage(dice, sides, base);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_MISSILE, dir, damroll(dice, sides) + base, rad);

				/*
				 * Shouldn't actually use GF_MANA, as
				 * it will destroy all items on the
				 * floor
				 */
			}
		}
		break;

	case 5:
		if (name) return _("�ե��������ܥ��", "Fire Bolt");
		if (desc) return _("��Υܥ�Ȥ⤷���ϥӡ�������ġ�", "Fires a bolt or beam of fire.");
    
		{
			int dice = 8 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance(), GF_FIRE, dir, damroll(dice, sides));
			}
		}
		break;

	case 6:
		if (name) return _("�Ϥη�", "Fist of Force");
		if (desc) return _("����������ʬ��ε�����ġ�", "Fires a tiny ball of disintegration.");
    
		{
			int dice = 8 + ((plev - 5) / 4);
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_DISINTEGRATE, dir,
					damroll(dice, sides), 0);
			}
		}
		break;

	case 7:
		if (name) return _("�ƥ�ݡ���", "Teleport Self");
		if (desc) return _("���Υ�Υƥ�ݡ��Ȥ򤹤롣", "Teleport long distance.");
    
		{
			POSITION range = plev * 5;

			if (info) return info_range(range);

			if (cast)
			{
				teleport_player(range, 0L);
			}
		}
		break;

	case 8:
		if (name) return _("������", "Wonder");
		if (desc) return _("��󥹥����˥�����ʸ��̤�Ϳ���롣", "Fires something with random effects.");
    
		{
			if (info) return s_random;

			if (cast)
			{

				if (!get_aim_dir(&dir)) return NULL;

				cast_wonder(dir);
			}
		}
		break;

	case 9:
		if (name) return _("���������ܥ��", "Chaos Bolt");
		if (desc) return _("�������Υܥ�Ȥ⤷���ϥӡ�������ġ�", "Fires a bolt or ball of chaos.");
    
		{
			int dice = 10 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance(), GF_CHAOS, dir, damroll(dice, sides));
			}
		}
		break;

	case 10:
		if (name) return _("���˥å����֡���", "Sonic Boom");
		if (desc) return _("��ʬ���濴�Ȥ����첻�ε��ȯ�������롣", "Generates a ball of sound centered on you.");
    
		{
			HIT_POINT dam = 60 + plev;
			int rad = plev / 10 + 2;

			if (info) return info_damage(0, 0, dam/2);

			if (cast)
			{
				msg_print(_("�ɡ����������ɤ줿��", "BOOM! Shake the room!"));
				project(0, rad, p_ptr->y, p_ptr->x, dam, GF_SOUND, PROJECT_KILL | PROJECT_ITEM, -1);
			}
		}
		break;

	case 11:
		if (name) return _("���Ǥ���", "Doom Bolt");
		if (desc) return _("�������ϤΥӡ�������ġ�", "Fires a beam of pure mana.");
    
		{
			int dice = 11 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_beam(GF_MANA, dir, damroll(dice, sides));
			}
		}
		break;

	case 12:
		if (name) return _("�ե��������ܡ���", "Fire Ball");
		if (desc) return _("��ε�����ġ�", "Fires a ball of fire.");
    
		{
			HIT_POINT dam = plev + 55;
			int rad = 2;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_FIRE, dir, dam, rad);
			}
		}
		break;

	case 13:
		if (name) return _("�ƥ�ݡ��ȡ���������", "Teleport Other");
		if (desc) return _("��󥹥�����ƥ�ݡ��Ȥ�����ӡ�������ġ��񹳤�����̵����", "Teleports all monsters on the line away unless resisted.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_beam(GF_AWAY_ALL, dir, power);
			}
		}
		break;

	case 14:
		if (name) return _("�˲��θ���", "Word of Destruction");
		if (desc) return _("���դΥ����ƥࡢ��󥹥������Ϸ����˲����롣", "Destroy everything in nearby area.");
    
		{
			int base = 12;
			int sides = 4;

			if (cast)
			{
				destroy_area(p_ptr->y, p_ptr->x, base + randint1(sides), FALSE);
			}
		}
		break;

	case 15:
		if (name) return _("���륹ȯư", "Invoke Logrus");
		if (desc) return _("����ʥ������ε�����ġ�", "Fires a huge ball of chaos.");
    
		{
			HIT_POINT dam = plev * 2 + 99;
			int rad = plev / 5;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_CHAOS, dir, dam, rad);
			}
		}
		break;

	case 16:
		if (name) return _("¾������", "Polymorph Other");
		if (desc) return _("��󥹥���1�Τ��ѿȤ����롣�񹳤�����̵����", "Attempts to polymorph a monster.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				poly_monster(dir, plev);
			}
		}
		break;

	case 17:
		if (name) return _("Ϣ�����", "Chain Lightning");
		if (desc) return _("���������Ф����ŷ�Υӡ�������ġ�", "Fires lightning beams in all directions.");
    
		{
			int dice = 5 + plev / 10;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				for (dir = 0; dir <= 9; dir++)
					fire_beam(GF_ELEC, dir, damroll(dice, sides));
			}
		}
		break;

	case 18:
		if (name) return _("��������", "Arcane Binding");
		if (desc) return _("��/��ˡ���ν�Ŷ��������䤹������Ŷ��Υ�åɤν�Ŷ���֤򸺤餹��", "Recharges staffs, wands or rods.");
    
		{
			int power = 90;

			if (info) return info_power(power);

			if (cast)
			{
				if (!recharge(power)) return NULL;
			}
		}
		break;

	case 19:
		if (name) return _("����ʬ��", "Disintegrate");
		if (desc) return _("�����ʬ��ε�����ġ�", "Fires a huge ball of disintegration.");
    
		{
			HIT_POINT dam = plev + 70;
			int rad = 3 + plev / 40;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_DISINTEGRATE, dir, dam, rad);
			}
		}
		break;

	case 20:
		if (name) return _("��������", "Alter Reality");
		if (desc) return _("���ߤγ���ƹ������롣", "Recreates current dungeon level.");
    
		{
			int base = 15;
			int sides = 20;

			if (info) return info_delay(base, sides);

			if (cast)
			{
				alter_reality();
			}
		}
		break;

	case 21:
		if (name) return _("�ޥ��å������å�", "Magic Rocket");
		if (desc) return _("���åȤ�ȯ�ͤ��롣", "Fires a magic rocket.");
    
		{
			HIT_POINT dam = 120 + plev * 2;
			int rad = 2;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				msg_print(_("���å�ȯ�͡�", "You launch a rocket!"));
				fire_rocket(GF_ROCKET, dir, dam, rad);
			}
		}
		break;

	case 22:
		if (name) return _("���٤ο�", "Chaos Branding");
		if (desc) return _("���˥�������°����Ĥ��롣", "Makes current weapon a Chaotic weapon.");
    
		{
			if (cast)
			{
				brand_weapon(2);
			}
		}
		break;

	case 23:
		if (name) return _("���⾤��", "Summon Demon");
		if (desc) return _("�����1�ξ������롣", "Summons a demon.");
    
		{
			if (cast)
			{
				u32b flg = 0L;
				bool pet = !one_in_(3);

				if (pet) flg |= PM_FORCE_PET;
				else flg |= PM_NO_PET;
				if (!(pet && (plev < 50))) flg |= PM_ALLOW_GROUP;

				if (summon_specific((pet ? -1 : 0), p_ptr->y, p_ptr->x, (plev * 3) / 2, SUMMON_DEMON, flg))
				{
					msg_print(_("β���ΰ���������������", "The area fills with a stench of sulphur and brimstone."));
					if (pet)
					{
						msg_print(_("�֤��ѤǤ������ޤ�����������͡�", "'What is thy bidding... Master?'"));
					}
					else
					{
						msg_print(_("���ܤ����Ԥ衢�����β��ͤˤ��餺�� �����κ���ĺ��������",
									"'NON SERVIAM! Wretch! I shall feast on thy mortal soul!'"));
					}
				}
			}
		}
		break;

	case 24:
		if (name) return _("���ϸ���", "Beam of Gravity");
		if (desc) return _("���ϤΥӡ�������ġ�", "Fires a beam of gravity.");
    
		{
			int dice = 9 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_beam(GF_GRAVITY, dir, damroll(dice, sides));
			}
		}
		break;

	case 25:
		if (name) return _("ή����", "Meteor Swarm");
		if (desc) return _("��ʬ�μ��դ���Ф���Ȥ���", "Makes meteor balls fall down to nearby random locations.");
    
		{
			HIT_POINT dam = plev * 2;
			int rad = 2;

			if (info) return info_multi_damage(dam);

			if (cast)
			{
				cast_meteor(dam, rad);
			}
		}
		break;

	case 26:
		if (name) return _("��ΰ��", "Flame Strike");
		if (desc) return _("��ʬ���濴�Ȥ���Ķ����ʱ�ε��ȯ�������롣", "Generate a huge ball of fire centered on you.");
    
		{
			HIT_POINT dam = 300 + 3 * plev;
			int rad = 8;

			if (info) return info_damage(0, 0, dam/2);

			if (cast)
			{
				fire_ball(GF_FIRE, 0, dam, rad);
			}
		}
		break;

	case 27:
		if (name) return _("���پ���", "Call Chaos");
		if (desc) return _("�������°���ε��ӡ����ȯ�������롣", "Generate random kind of balls or beams.");
    
		{
			if (info) return format("%s150 / 250", s_dam);

			if (cast)
			{
				call_chaos();
			}
		}
		break;

	case 28:
		if (name) return _("��������", "Polymorph Self");
		if (desc) return _("��ʬ���ѿȤ����褦�Ȥ��롣", "Polymorphs yourself.");
    
		{
			if (cast)
			{
				if (!get_check(_("�ѿȤ��ޤ���������Ǥ�����", "You will polymorph yourself. Are you sure? "))) return NULL;
				do_poly_self();
			}
		}
		break;

	case 29:
		if (name) return _("���Ϥ���", "Mana Storm");
		if (desc) return _("���˶��Ϥǵ���ʽ������Ϥε�����ġ�", "Fires an extremely powerful huge ball of pure mana.");
    
		{
			HIT_POINT dam = 300 + plev * 4;
			int rad = 4;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_MANA, dir, dam, rad);
			}
		}
		break;

	case 30:
		if (name) return _("���륹�Υ֥쥹", "Breathe Logrus");
		if (desc) return _("���˶��Ϥʥ������ε�����ġ�", "Fires an extremely powerful ball of chaos.");
    
		{
			HIT_POINT dam = p_ptr->chp;
			int rad = 2;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_CHAOS, dir, dam, rad);
			}
		}
		break;

	case 31:
		if (name) return _("��̵����", "Call the Void");
		if (desc) return _("��ʬ�˼��Ϥ˸����äơ����åȡ��������Ϥε塢�������Ѵ�ʪ�ε�����ġ����������ɤ����ܤ��ƻ��Ѥ���ȹ��ϰϤ��˲����롣", 
			"Fires rockets, mana balls and nuclear waste balls in all directions each unless you are not adjacent to any walls. Otherwise *destroys* huge area.");
    
		{
			if (info) return format("%s3 * 175", s_dam);

			if (cast)
			{
				call_the_();
			}
		}
		break;
	}

	return "";
}

/*!
 * @brief �Ź��ΰ���ˡ�γƽ�����Ԥ�
 * @param spell ��ˡID
 * @param mode �������� (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
 * @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ˤ�ʸ����ݥ��󥿤��֤���SPELL_CAST����NULLʸ������֤���
 */
static cptr do_death_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;

	static const char s_dam[] = _("»��:", "dam ");
	static const char s_random[] = _("������", "random");

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("̵��̿����", "Detect Unlife");
		if (desc) return _("�᤯����̿�Τʤ���󥹥������Τ��롣", "Detects all nonliving monsters in your vicinity.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_monsters_nonliving(rad);
			}
		}
		break;

	case 1:
		if (name) return _("������", "Malediction");
		if (desc) return _("���������ʼٰ����Ϥ���ĥܡ�������ġ����ɤʥ�󥹥����ˤ��礭�ʥ��᡼����Ϳ���롣", 
			"Fires a tiny ball of evil power which hurts good monsters greatly.");
    
		{
			int dice = 3 + (plev - 1) / 5;
			int sides = 4;
			int rad = 0;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				/*
				 * A radius-0 ball may (1) be aimed at
				 * objects etc., and will affect them;
				 * (2) may be aimed at ANY visible
				 * monster, unlike a 'bolt' which must
				 * travel to the monster.
				 */

				fire_ball(GF_HELL_FIRE, dir, damroll(dice, sides), rad);

				if (one_in_(5))
				{
					/* Special effect first */
					int effect = randint1(1000);

					if (effect == 666)
						fire_ball_hide(GF_DEATH_RAY, dir, plev * 200, 0);
					else if (effect < 500)
						fire_ball_hide(GF_TURN_ALL, dir, plev, 0);
					else if (effect < 800)
						fire_ball_hide(GF_OLD_CONF, dir, plev, 0);
					else
						fire_ball_hide(GF_STUN, dir, plev, 0);
				}
			}
		}
		break;

	case 2:
		if (name) return _("�ٰ�����", "Detect Evil");
		if (desc) return _("�᤯�μٰ��ʥ�󥹥������Τ��롣", "Detects all evil monsters in your vicinity.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_monsters_evil(rad);
			}
		}
		break;

	case 3:
		if (name) return _("������", "Stinking Cloud");
		if (desc) return _("�Ǥε�����ġ�", "Fires a ball of poison.");
    
		{
			HIT_POINT dam = 10 + plev / 2;
			int rad = 2;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_POIS, dir, dam, rad);
			}
		}
		break;

	case 4:
		if (name) return _("����̲��", "Black Sleep");
		if (desc) return _("1�ΤΥ�󥹥�����̲�餻�롣�񹳤�����̵����", "Attempts to sleep a monster.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				sleep_monster(dir, plev);
			}
		}
		break;

	case 5:
		if (name) return _("����", "Resist Poison");
		if (desc) return _("������֡��Ǥؤ����������롣�����ˤ�����������Ѥ��롣", 
			"Gives resistance to poison. This resistance can be added to which from equipment for more powerful resistance.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_pois(randint1(base) + base, FALSE);
			}
		}
		break;

	case 6:
		if (name) return _("����", "Horrify");
		if (desc) return _("��󥹥���1�Τ��ݤ�����ۯ۰�����롣�񹳤�����̵����", "Attempts to scare and stun a monster.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fear_monster(dir, power);
				stun_monster(dir, power);
			}
		}
		break;

	case 7:
		if (name) return _("����ǥåɽ�°", "Enslave Undead");
		if (desc) return _("����ǥå�1�Τ�̥λ���롣�񹳤�����̵����", "Attempts to charm an undead monster.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				control_one_undead(dir, power);
			}
		}
		break;

	case 8:
		if (name) return _("����ȥ�ԡ��ε�", "Orb of Entropy");
		if (desc) return _("��̿�Τ���Ԥ�HP�Ⱥ���HP�����˥��᡼����Ϳ������̤Τ��������ġ�", "Fires a ball which damages to both HP and MaxHP of living monsters.");

		{
			int dice = 3;
			int sides = 6;
			int rad = (plev < 30) ? 2 : 3;
			int base;

			if (p_ptr->pclass == CLASS_MAGE ||
			    p_ptr->pclass == CLASS_HIGH_MAGE ||
			    p_ptr->pclass == CLASS_SORCERER)
				base = plev + plev / 2;
			else
				base = plev + plev / 4;


			if (info) return info_damage(dice, sides, base);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_HYPODYNAMIA, dir, damroll(dice, sides) + base, rad);
			}
		}
		break;

	case 9:
		if (name) return _("�Ϲ�����", "Nether Bolt");
		if (desc) return _("�Ϲ��Υܥ�Ȥ⤷���ϥӡ�������ġ�", "Fires a bolt or beam of nether.");
    
		{
			int dice = 8 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance(), GF_NETHER, dir, damroll(dice, sides));
			}
		}
		break;

	case 10:
		if (name) return _("��٤��", "Cloud kill");
		if (desc) return _("��ʬ���濴�Ȥ����Ǥε��ȯ�������롣", "Generate a ball of poison centered on you.");
    
		{
			HIT_POINT dam = (30 + plev) * 2;
			int rad = plev / 10 + 2;

			if (info) return info_damage(0, 0, dam/2);

			if (cast)
			{
				project(0, rad, p_ptr->y, p_ptr->x, dam, GF_POIS, PROJECT_KILL | PROJECT_ITEM, -1);
			}
		}
		break;

	case 11:
		if (name) return _("��󥹥�������", "Genocide One");
		if (desc) return _("��󥹥���1�Τ�ä���롣�и��ͤ䥢���ƥ�ϼ������ʤ����񹳤�����̵����", "Attempts to vanish a monster.");
    
		{
			int power = plev + 50;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball_hide(GF_GENOCIDE, dir, power, 0);
			}
		}
		break;

	case 12:
		if (name) return _("�Ǥο�", "Poison Branding");
		if (desc) return _("�����Ǥ�°����Ĥ��롣", "Makes current weapon poison branded.");
    
		{
			if (cast)
			{
				brand_weapon(3);
			}
		}
		break;

	case 13:
		if (name) return _("�۷����", "Vampiric Bolt");
		if (desc) return _("�ܥ�Ȥˤ���󥹥���1�Τ�����̿�Ϥ�ۤ��Ȥ롣�ۤ��Ȥä���̿�Ϥˤ�ä���ʢ�٤��夬�롣", 
			"Absorbs some HP from a monster and gives them to you by bolt. You will also gain nutritional sustenance from this.");
    
		{
			int dice = 1;
			int sides = plev * 2;
			int base = plev * 2;

			if (info) return info_damage(dice, sides, base);

			if (cast)
			{
				HIT_POINT dam = base + damroll(dice, sides);

				if (!get_aim_dir(&dir)) return NULL;

				if (hypodynamic_bolt(dir, dam))
				{
					chg_virtue(V_SACRIFICE, -1);
					chg_virtue(V_VITALITY, -1);

					hp_player(dam);

					/*
					 * Gain nutritional sustenance:
					 * 150/hp drained
					 *
					 * A Food ration gives 5000
					 * food points (by contrast)
					 * Don't ever get more than
					 * "Full" this way But if we
					 * ARE Gorged, it won't cure
					 * us
					 */
					dam = p_ptr->food + MIN(5000, 100 * dam);

					/* Not gorged already */
					if (p_ptr->food < PY_FOOD_MAX)
						set_food(dam >= PY_FOOD_MAX ? PY_FOOD_MAX - 1 : dam);
				}
			}
		}
		break;

	case 14:
		if (name) return _("ȿ���ν�", "Animate dead");
		if (desc) return _("���Ϥλ��Τ���������֤���", "Resurrects nearby corpse and skeletons. And makes these your pets.");
    
		{
			if (cast)
			{
				animate_dead(0, p_ptr->y, p_ptr->x);
			}
		}
		break;

	case 15:
		if (name) return _("����", "Genocide");
		if (desc) return _("���ꤷ��ʸ���Υ�󥹥����򸽺ߤγ�����ä���롣�񹳤�����̵����", 
			"Eliminates an entire class of monster, exhausting you.  Powerful or unique monsters may resist.");
    
		{
			int power = plev+50;

			if (info) return info_power(power);

			if (cast)
			{
				symbol_genocide(power, TRUE);
			}
		}
		break;

	case 16:
		if (name) return _("����β�", "Berserk");
		if (desc) return _("����β��������ݤ����롣", "Gives bonus to hit and HP, immunity to fear for a while. But decreases AC.");
    
		{
			int base = 25;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_shero(randint1(base) + base, FALSE);
				hp_player(30);
				set_afraid(0);
			}
		}
		break;

	case 17:
		if (name) return _("�����", "Invoke Spirits");
		if (desc) return _("��������͡��ʸ��̤������롣", "Causes random effects.");
    
		{
			if (info) return s_random;

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				cast_invoke_spirits(dir);
			}
		}
		break;

	case 18:
		if (name) return _("�Ź�����", "Dark Bolt");
		if (desc) return _("�Ź��Υܥ�Ȥ⤷���ϥӡ�������ġ�", "Fires a bolt or beam of darkness.");
    
		{
			int dice = 4 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance(), GF_DARK, dir, damroll(dice, sides));
			}
		}
		break;

	case 19:
		if (name) return _("�������", "Battle Frenzy");
		if (desc) return _("����β��������ݤ�������®���롣", 
			"Gives another bonus to hit and HP, immunity to fear for a while. Hastes you. But decreases AC.");
    
		{
			int b_base = 25;
			int sp_base = plev / 2;
			int sp_sides = 20 + plev / 2;

			if (info) return info_duration(b_base, b_base);

			if (cast)
			{
				set_shero(randint1(25) + 25, FALSE);
				hp_player(30);
				set_afraid(0);
				set_fast(randint1(sp_sides) + sp_base, FALSE);
			}
		}
		break;

	case 20:
		if (name) return _("�۷�ο�", "Vampiric Branding");
		if (desc) return _("���˵۷��°����Ĥ��롣", "Makes current weapon Vampiric.");
    
		{
			if (cast)
			{
				brand_weapon(4);
			}
		}
		break;

	case 21:
		if (name) return _("�۷��Ϣ��", "Vampiric Bolts");
		if (desc) return _("3Ϣ�ͤΥܥ�Ȥˤ���󥹥���1�Τ�����̿�Ϥ�ۤ��Ȥ롣�ۤ��Ȥä���̿�Ϥˤ�ä����Ϥ��������롣", 
			"Fires 3 bolts. Each of the bolts absorbs some HP from a monster and gives them to you.");
		{
			HIT_POINT dam = 100;

			if (info) return format("%s3*%d", s_dam, dam);

			if (cast)
			{
				int i;

				if (!get_aim_dir(&dir)) return NULL;

				chg_virtue(V_SACRIFICE, -1);
				chg_virtue(V_VITALITY, -1);

				for (i = 0; i < 3; i++)
				{
					if (hypodynamic_bolt(dir, dam))
						hp_player(dam);
				}
			}
		}
		break;

	case 22:
		if (name) return _("��θ���", "Nether Wave");
		if (desc) return _("�볦�����̿�Τ����󥹥����˥��᡼����Ϳ���롣", "Damages all living monsters in sight.");
    
		{
			int sides = plev * 3;

			if (info) return info_damage(1, sides, 0);

			if (cast)
			{
				dispel_living(randint1(sides));
			}
		}
		break;

	case 23:
		if (name) return _("�Ź�����", "Darkness Storm");
		if (desc) return _("����ʰŹ��ε�����ġ�", "Fires a huge ball of darkness.");
    
		{
			HIT_POINT dam = 100 + plev * 2;
			int rad = 4;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_DARK, dir, dam, rad);
			}
		}
		break;

	case 24:
		if (name) return _("��θ���", "Death Ray");
		if (desc) return _("��θ��������ġ�", "Fires a beam of death.");
    
		{
			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				death_ray(dir, plev);
			}
		}
		break;

	case 25:
		if (name) return _("��Ծ���", "Raise the Dead");
		if (desc) return _("1�ΤΥ���ǥåɤ򾤴����롣", "Summons an undead monster.");
    
		{
			if (cast)
			{
				int type;
				bool pet = one_in_(3);
				u32b flg = 0L;

				type = (plev > 47 ? SUMMON_HI_UNDEAD : SUMMON_UNDEAD);

				if (!pet || (pet && (plev > 24) && one_in_(3)))
					flg |= PM_ALLOW_GROUP;

				if (pet) flg |= PM_FORCE_PET;
				else flg |= (PM_ALLOW_UNIQUE | PM_NO_PET);

				if (summon_specific((pet ? -1 : 0), p_ptr->y, p_ptr->x, (plev * 3) / 2, type, flg))
				{
					msg_print(_("�䤿���������ʤ��μ���˿᤭�Ϥ᤿����������Խ��򱿤�Ǥ���...",
								"Cold winds begin to blow around you, carrying with them the stench of decay..."));


					if (pet)
					{
						msg_print(_("�Ť��λऻ��Զ������ʤ��˻Ť��뤿���ڤ���ᴤä���",
									"Ancient, long-dead forms arise from the ground to serve you!"));
					}
					else
					{
						msg_print(_("��Ԥ�ᴤä���̲���˸���뤢�ʤ���ȳ���뤿��ˡ�",
									"'The dead arise... to punish you for disturbing them!'"));
					}

					chg_virtue(V_UNLIFE, 1);
				}
			}
		}
		break;

	case 26:
		if (name) return _("��Ԥ�����", "Esoteria");
		if (desc) return _("�����ƥ��1�ļ��̤��롣��٥뤬�⤤�ȥ����ƥ��ǽ�Ϥ������Τ뤳�Ȥ��Ǥ��롣",
			"Identifies an item. Or *identifies* an item at higher level.");
    
		{
			if (cast)
			{
				if (randint1(50) > plev)
				{
					if (!ident_spell(FALSE)) return NULL;
				}
				else
				{
					if (!identify_fully(FALSE)) return NULL;
				}
			}
		}
		break;

	case 27:
		if (name) return _("�۷쵴�Ѳ�", "Polymorph Vampire");
		if (desc) return _("������֡��۷쵴���Ѳ����롣�Ѳ����Ƥ���֤�����μ�²��ǽ�Ϥ򼺤�������˵۷쵴�Ȥ��Ƥ�ǽ�Ϥ����롣", 
			"Mimic a vampire for a while. Loses abilities of original race and gets abilities as a vampire.");
    
		{
			int base = 10 + plev / 2;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_mimic(base + randint1(base), MIMIC_VAMPIRE, FALSE);
			}
		}
		break;

	case 28:
		if (name) return _("�и�������", "Restore Life");
		if (desc) return _("���ä��и��ͤ�������롣", "Restore lost experience.");
    
		{
			if (cast)
			{
				restore_level();
			}
		}
		break;

	case 29:
		if (name) return _("��������", "Mass Genocide");
		if (desc) return _("��ʬ�μ��Ϥˤ����󥹥����򸽺ߤγ�����ä���롣�񹳤�����̵����", 
			"Eliminates all nearby monsters, exhausting you.  Powerful or unique monsters may be able to resist.");
    
		{
			int power = plev + 50;

			if (info) return info_power(power);

			if (cast)
			{
				mass_genocide(power, TRUE);
			}
		}
		break;

	case 30:
		if (name) return _("�Ϲ��ι��", "Hellfire");
		if (desc) return _("�ٰ����Ϥ������������ġ����ɤʥ�󥹥����ˤ��礭�ʥ��᡼����Ϳ���롣", 
			"Fires a powerful ball of evil power. Hurts good monsters greatly.");
    
		{
			HIT_POINT dam = 666;
			int rad = 3;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_HELL_FIRE, dir, dam, rad);
				take_hit(DAMAGE_USELIFE, 20 + randint1(30), _("�Ϲ��ι�Фμ�ʸ�򾧤�����ϫ", "the strain of casting Hellfire"), -1);
			}
		}
		break;

	case 31:
		if (name) return _("ͩ�β�", "Wraithform");
		if (desc) return _("������֡��ɤ��̤�ȴ���뤳�Ȥ��Ǥ���������᡼�����ڸ������ͩ�Τξ��֤��ѿȤ��롣", 
			"Becomes wraith form which gives ability to pass walls and makes all damages half.");
    
		{
			int base = plev / 2;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_wraith_form(randint1(base) + base, FALSE);
			}
		}
		break;
	}

	return "";
}


/*!
 * @brief �ȥ����ΰ���ˡ�γƽ�����Ԥ�
 * @param spell ��ˡID
 * @param mode �������� (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
 * @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ˤ�ʸ����ݥ��󥿤��֤���SPELL_CAST����NULLʸ������֤���
 */
static cptr do_trump_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;
	bool fail = (mode == SPELL_FAIL) ? TRUE : FALSE;
	static const char s_random[] = _("������", "random");

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("���硼�ȡ��ƥ�ݡ���", "Phase Door");
		if (desc) return _("���Υ�Υƥ�ݡ��Ȥ򤹤롣", "Teleport short distance.");
    
		{
			POSITION range = 10;

			if (info) return info_range(range);

			if (cast)
			{
				teleport_player(range, 0L);
			}
		}
		break;

	case 1:
		if (name) return _("����Υ�����", "Trump Spiders");
		if (desc) return _("����򾤴����롣", "Summons spiders.");
    
		{
			if (cast || fail)
			{
				msg_print(_("���ʤ�������Υ����ɤ˽��椹��...", "You concentrate on the trump of an spider..."));
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, SUMMON_SPIDER, PM_ALLOW_GROUP))
				{
					if (fail)
					{
						msg_print(_("�������줿������ܤäƤ��롪", "The summoned spiders get angry!"));
					}
				}
			}
		}
		break;

	case 2:
		if (name) return _("����åե�", "Shuffle");
		if (desc) return _("�����ɤ��ꤤ�򤹤롣", "Causes random effects.");
    
		{
			if (info) return s_random;

			if (cast)
			{
				cast_shuffle();
			}
		}
		break;

	case 3:
		if (name) return _("�ե����ꥻ�å�", "Reset Recall");
		if (desc) return _("�ǿ������ѹ����롣", "Resets the 'deepest' level for recall spell.");
    
		{
			if (cast)
			{
				if (!reset_recall()) return NULL;
			}
		}
		break;

	case 4:
		if (name) return _("�ƥ�ݡ���", "Teleport");
		if (desc) return _("���Υ�Υƥ�ݡ��Ȥ򤹤롣", "Teleport long distance.");
    
		{
			POSITION range = plev * 4;

			if (info) return info_range(range);

			if (cast)
			{
				teleport_player(range, 0L);
			}
		}
		break;

	case 5:
		if (name) return _("���ΤΥ�����", "Trump Spying");
		if (desc) return _("������֡��ƥ�ѥ���ǽ�Ϥ����롣", "Gives telepathy for a while.");
    
		{
			int base = 25;
			int sides = 30;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_tim_esp(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 6:
		if (name) return _("�ƥ�ݡ��ȡ���󥹥���", "Teleport Away");
		if (desc) return _("��󥹥�����ƥ�ݡ��Ȥ�����ӡ�������ġ��񹳤�����̵����", "Teleports all monsters on the line away unless resisted.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_beam(GF_AWAY_ALL, dir, power);
			}
		}
		break;

	case 7:
		if (name) return _("ưʪ�Υ�����", "Trump Animals");
		if (desc) return _("1�Τ�ưʪ�򾤴����롣", "Summons an animal.");
    
		{
			if (cast || fail)
			{
				int type = (!fail ? SUMMON_ANIMAL_RANGER : SUMMON_ANIMAL);
				msg_print(_("���ʤ���ưʪ�Υ����ɤ˽��椹��...", "You concentrate on the trump of an animal..."));
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, type, 0L))
				{
					if (fail)
					{
						msg_print(_("�������줿ưʪ���ܤäƤ��롪", "The summoned animal gets angry!"));
					}
				}
			}
		}
		break;

	case 8:
		if (name) return _("��ư�Υ�����", "Trump Reach");
		if (desc) return _("�����ƥ��ʬ��­���ذ�ư�����롣", "Pulls a distant item close to you.");
    
		{
			int weight = plev * 15;

			if (info) return info_weight(weight);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fetch(dir, weight, FALSE);
			}
		}
		break;

	case 9:
		if (name) return _("���ߥ����Υ�����", "Trump Kamikaze");
		if (desc) return _("ʣ������ȯ�����󥹥����򾤴����롣", "Summons monsters which explode by itself.");
    
		{
			if (cast || fail)
			{
				int x, y;
				int type;

				if (cast)
				{
					if (!target_set(TARGET_KILL)) return NULL;
					x = target_col;
					y = target_row;
				}
				else
				{
					/* Summons near player when failed */
					x = p_ptr->x;
					y = p_ptr->y;
				}

				if (p_ptr->pclass == CLASS_BEASTMASTER)
					type = SUMMON_KAMIKAZE_LIVING;
				else
					type = SUMMON_KAMIKAZE;

				msg_print(_("���ʤ��ϥ��ߥ����Υ����ɤ˽��椹��...", "You concentrate on several trumps at once..."));
				if (trump_summoning(2 + randint0(plev / 7), !fail, y, x, 0, type, 0L))
				{
					if (fail)
					{
						msg_print(_("�������줿��󥹥������ܤäƤ��롪", "The summoned creatures get angry!"));
					}
				}
			}
		}
		break;

	case 10:
		if (name) return _("�����", "Phantasmal Servant");
		if (desc) return _("1�Τ�ͩ��򾤴����롣", "Summons a ghost.");
    
		{
			/* Phantasmal Servant is not summoned as enemy when failed */
			if (cast)
			{
				int summon_lev = plev * 2 / 3 + randint1(plev / 2);

				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, (summon_lev * 3 / 2), SUMMON_PHANTOM, 0L))
				{
					msg_print(_("���ѤǤ������ޤ����������͡�", "'Your wish, master?'"));
				}
			}
		}
		break;

	case 11:
		if (name) return _("���ԡ��ɡ���󥹥���", "Haste Monster");
		if (desc) return _("��󥹥���1�Τ��®�����롣", "Hastes a monster.");
    
		{
			if (cast)
			{
				bool result;

				/* Temporary enable target_pet option */
				bool old_target_pet = target_pet;
				target_pet = TRUE;

				result = get_aim_dir(&dir);

				/* Restore target_pet option */
				target_pet = old_target_pet;

				if (!result) return NULL;

				speed_monster(dir, plev);
			}
		}
		break;

	case 12:
		if (name) return _("�ƥ�ݡ��ȡ���٥�", "Teleport Level");
		if (desc) return _("�ֻ��˾夫���γ��˥ƥ�ݡ��Ȥ��롣", "Teleport to up or down stairs in a moment.");
    
		{
			if (cast)
			{
				if (!get_check(_("������¾�γ��˥ƥ�ݡ��Ȥ��ޤ�����", "Are you sure? (Teleport Level)"))) return NULL;
				teleport_level(0);
			}
		}
		break;

	case 13:
		if (name) return _("��������", "Dimension Door");
		if (desc) return _("û��Υ��λ��ꤷ�����˥ƥ�ݡ��Ȥ��롣", "Teleport to given location.");
    
		{
			POSITION range = plev / 2 + 10;

			if (info) return info_range(range);

			if (cast)
			{
				msg_print(_("�������⤬����������Ū�Ϥ�����ǲ�������", "You open a dimensional gate. Choose a destination."));
				if (!dimension_door()) return NULL;
			}
		}
		break;

	case 14:
		if (name) return _("���Ԥμ�ʸ", "Word of Recall");
		if (desc) return _("�Ͼ�ˤ���Ȥ��ϥ��󥸥��κǿ����ء����󥸥��ˤ���Ȥ����Ͼ�ؤȰ�ư���롣",
			"Recalls player from dungeon to town, or from town to the deepest level of dungeon.");
    
		{
			int base = 15;
			int sides = 20;

			if (info) return info_delay(base, sides);

			if (cast)
			{
				if (!word_of_recall()) return NULL;
			}
		}
		break;

	case 15:
		if (name) return _("��ʪ����", "Banish");
		if (desc) return _("�볦������ƤΥ�󥹥�����ƥ�ݡ��Ȥ����롣�񹳤�����̵����", "Teleports all monsters in sight away unless resisted.");
    
		{
			int power = plev * 4;

			if (info) return info_power(power);

			if (cast)
			{
				banish_monsters(power);
			}
		}
		break;

	case 16:
		if (name) return _("���ָ򴹤Υ�����", "Swap Position");
		if (desc) return _("1�ΤΥ�󥹥����Ȱ��֤�򴹤��롣", "Swap positions of you and a monster.");
    
		{
			if (cast)
			{
				bool result;

				/* HACK -- No range limit */
				project_length = -1;

				result = get_aim_dir(&dir);

				/* Restore range to default */
				project_length = 0;

				if (!result) return NULL;

				teleport_swap(dir);
			}
		}
		break;

	case 17:
		if (name) return _("����ǥåɤΥ�����", "Trump Undead");
		if (desc) return _("1�ΤΥ���ǥåɤ򾤴����롣", "Summons an undead monster.");
    
		{
			if (cast || fail)
			{
				msg_print(_("���ʤ��ϥ���ǥåɤΥ����ɤ˽��椹��...", "You concentrate on the trump of an undead creature..."));
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, SUMMON_UNDEAD, 0L))
				{
					if (fail)
					{
						msg_print(_("�������줿����ǥåɤ��ܤäƤ��롪", "The summoned undead creature gets angry!"));
					}
				}
			}
		}
		break;

	case 18:
		if (name) return _("�����Υ�����", "Trump Reptiles");
		if (desc) return _("1�ΤΥҥɥ�򾤴����롣", "Summons a hydra.");
    
		{
			if (cast || fail)
			{
				msg_print(_("���ʤ��������Υ����ɤ˽��椹��...", "You concentrate on the trump of a reptile..."));
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, SUMMON_HYDRA, 0L))
				{
					if (fail)
					{
						msg_print(_("�������줿�������ܤäƤ��롪", "The summoned reptile gets angry!"));
					}
				}
			}
		}
		break;

	case 19:
		if (name) return _("��󥹥����Υ�����", "Trump Monsters");
		if (desc) return _("ʣ���Υ�󥹥����򾤴����롣", "Summons some monsters.");
    
		{
			if (cast || fail)
			{
				int type;
				msg_print(_("���ʤ��ϥ�󥹥����Υ����ɤ˽��椹��...", "You concentrate on several trumps at once..."));
				if (p_ptr->pclass == CLASS_BEASTMASTER)
					type = SUMMON_LIVING;
				else
					type = 0;

				if (trump_summoning((1 + (plev - 15)/ 10), !fail, p_ptr->y, p_ptr->x, 0, type, 0L))
				{
					if (fail)
					{
						msg_print(_("�������줿��󥹥������ܤäƤ��롪", "The summoned creatures get angry!"));
					}
				}

			}
		}
		break;

	case 20:
		if (name) return _("�ϥ���ɤΥ�����", "Trump Hounds");
		if (desc) return _("1���롼�פΥϥ���ɤ򾤴����롣", "Summons a group of hounds.");
    
		{
			if (cast || fail)
			{
				msg_print(_("���ʤ��ϥϥ���ɤΥ����ɤ˽��椹��...", "You concentrate on the trump of a hound..."));
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, SUMMON_HOUND, PM_ALLOW_GROUP))
				{
					if (fail)
					{
						msg_print(_("�������줿�ϥ���ɤ��ܤäƤ��롪", "The summoned hounds get angry!"));
					}
				}
			}
		}
		break;

	case 21:
		if (name) return _("�ȥ��פο�", "Trump Branding");
		if (desc) return _("���˥ȥ��פ�°����Ĥ��롣", "Makes current weapon a Trump weapon.");
    
		{
			if (cast)
			{
				brand_weapon(5);
			}
		}
		break;

	case 22:
		if (name) return _("�ʹ֥ȥ���", "Living Trump");
		if (desc) return _("������˥ƥ�ݡ��Ȥ��������Ѱۤ�����ʬ�ΰջפǥƥ�ݡ��Ȥ��������Ѱۤ��ȤˤĤ���", 
			"Gives mutation which makes you teleport randomly or makes you able to teleport at will.");
    
		{
			if (cast)
			{
				int mutation;

				if (one_in_(7))
					/* Teleport control */
					mutation = 12;
				else
					/* Random teleportation (uncontrolled) */
					mutation = 77;

				/* Gain the mutation */
				if (gain_random_mutation(mutation))
				{
					msg_print(_("���ʤ��������Ƥ��륫���ɤ��Ѥ�ä���", "You have turned into a Living Trump."));
				}
			}
		}
		break;

	case 23:
		if (name) return _("�����С��ǡ����Υ�����", "Trump Cyberdemon");
		if (desc) return _("1�ΤΥ����С��ǡ����򾤴����롣", "Summons a cyber demon.");
    
		{
			if (cast || fail)
			{
				msg_print(_("���ʤ��ϥ����С��ǡ����Υ����ɤ˽��椹��...", "You concentrate on the trump of a Cyberdemon..."));
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, SUMMON_CYBER, 0L))
				{
					if (fail)
					{
						msg_print(_("�������줿�����С��ǡ������ܤäƤ��롪", "The summoned Cyberdemon gets angry!"));
					}
				}
			}
		}
		break;

	case 24:
		if (name) return _("ͽ���Υ�����", "Trump Divination");
		if (desc) return _("�᤯�����ƤΥ�󥹥�����櫡��⡢���ʡ������������ƥ����ƥ���Τ��롣",
			"Detects all monsters, traps, doors, stairs, treasures and items in your vicinity.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_all(rad);
			}
		}
		break;

	case 25:
		if (name) return _("�μ��Υ�����", "Trump Lore");
		if (desc) return _("�����ƥ�λ���ǽ�Ϥ������Τ롣", "*Identifies* an item.");
    
		{
			if (cast)
			{
				if (!identify_fully(FALSE)) return NULL;
			}
		}
		break;

	case 26:
		if (name) return _("������󥹥���", "Heal Monster");
		if (desc) return _("��󥹥���1�Τ����Ϥ���������롣", "Heal a monster.");
    
		{
			int heal = plev * 10 + 200;

			if (info) return info_heal(0, 0, heal);

			if (cast)
			{
				bool result;

				/* Temporary enable target_pet option */
				bool old_target_pet = target_pet;
				target_pet = TRUE;

				result = get_aim_dir(&dir);

				/* Restore target_pet option */
				target_pet = old_target_pet;

				if (!result) return NULL;

				heal_monster(dir, heal);
			}
		}
		break;

	case 27:
		if (name) return _("�ɥ饴��Υ�����", "Trump Dragon");
		if (desc) return _("1�ΤΥɥ饴��򾤴����롣", "Summons a dragon.");
    
		{
			if (cast || fail)
			{
				msg_print(_("���ʤ��ϥɥ饴��Υ����ɤ˽��椹��...", "You concentrate on the trump of a dragon..."));
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, SUMMON_DRAGON, 0L))
				{
					if (fail)
					{
						msg_print(_("�������줿�ɥ饴����ܤäƤ��롪", "The summoned dragon gets angry!"));
					}
				}
			}
		}
		break;

	case 28:
		if (name) return _("��ФΥ�����", "Trump Meteor");
		if (desc) return _("��ʬ�μ��դ���Ф���Ȥ���", "Makes meteor balls fall down to nearby random locations.");
    
		{
			HIT_POINT dam = plev * 2;
			int rad = 2;

			if (info) return info_multi_damage(dam);

			if (cast)
			{
				cast_meteor(dam, rad);
			}
		}
		break;

	case 29:
		if (name) return _("�ǡ����Υ�����", "Trump Demon");
		if (desc) return _("1�Τΰ���򾤴����롣", "Summons a demon.");
    
		{
			if (cast || fail)
			{
				msg_print(_("���ʤ��ϥǡ����Υ����ɤ˽��椹��...", "You concentrate on the trump of a demon..."));
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, SUMMON_DEMON, 0L))
				{
					if (fail)
					{
						msg_print(_("�������줿�ǡ������ܤäƤ��롪", "The summoned demon gets angry!"));
					}
				}
			}
		}
		break;

	case 30:
		if (name) return _("�Ϲ��Υ�����", "Trump Greater Undead");
		if (desc) return _("1�Τξ�饢��ǥåɤ򾤴����롣", "Summons a greater undead.");
    
		{
			if (cast || fail)
			{
				msg_print(_("���ʤ��϶��Ϥʥ���ǥåɤΥ����ɤ˽��椹��...", "You concentrate on the trump of a greater undead being..."));
				/* May allow unique depend on level and dice roll */
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, SUMMON_HI_UNDEAD, PM_ALLOW_UNIQUE))
				{
					if (fail)
					{
						msg_print(_("�������줿��饢��ǥåɤ��ܤäƤ��롪", "The summoned greater undead creature gets angry!"));
					}
				}
			}
		}
		break;

	case 31:
		if (name) return _("����ɥ饴��Υ�����", "Trump Ancient Dragon");
		if (desc) return _("1�Τθ���ɥ饴��򾤴����롣", "Summons an ancient dragon.");
    
		{
			if (cast)
			{
				int type;

				if (p_ptr->pclass == CLASS_BEASTMASTER)
					type = SUMMON_HI_DRAGON_LIVING;
				else
					type = SUMMON_HI_DRAGON;

				msg_print(_("���ʤ��ϸ���ɥ饴��Υ����ɤ˽��椹��...", "You concentrate on the trump of an ancient dragon..."));
				/* May allow unique depend on level and dice roll */
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, type, PM_ALLOW_UNIQUE))
				{
					if (fail)
					{
						msg_print(_("�������줿����ɥ饴����ܤäƤ��롪", "The summoned ancient dragon gets angry!"));
					}
				}
			}
		}
		break;
	}

	return "";
}


/*!
 * @brief ����ΰ���ˡ�γƽ�����Ԥ�
 * @param spell ��ˡID
 * @param mode �������� (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
 * @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ˤ�ʸ����ݥ��󥿤��֤���SPELL_CAST����NULLʸ������֤���
 */
static cptr do_arcane_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("�ŷ�", "Zap");
		if (desc) return _("�ŷ�Υܥ�Ȥ⤷���ϥӡ�������ġ�", "Fires a bolt or beam of lightning.");
    
		{
			int dice = 3 + (plev - 1) / 5;
			int sides = 3;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance() - 10, GF_ELEC, dir, damroll(dice, sides));
			}
		}
		break;

	case 1:
		if (name) return _("��ˡ�λܾ�", "Wizard Lock");
		if (desc) return _("��˸��򤫤��롣", "Locks a door.");
    
		{
			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				wizard_lock(dir);
			}
		}
		break;

	case 2:
		if (name) return _("Ʃ���δ���", "Detect Invisibility");
		if (desc) return _("�᤯��Ʃ���ʥ�󥹥������Τ��롣", "Detects all invisible monsters in your vicinity.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_monsters_invis(rad);
			}
		}
		break;

	case 3:
		if (name) return _("��󥹥�������", "Detect Monsters");
		if (desc) return _("�᤯�����Ƥθ������󥹥������Τ��롣", "Detects all monsters in your vicinity unless invisible.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_monsters_normal(rad);
			}
		}
		break;

	case 4:
		if (name) return _("���硼�ȡ��ƥ�ݡ���", "Blink");
		if (desc) return _("���Υ�Υƥ�ݡ��Ȥ򤹤롣", "Teleport short distance.");
    
		{
			POSITION range = 10;

			if (info) return info_range(range);

			if (cast)
			{
				teleport_player(range, 0L);
			}
		}
		break;

	case 5:
		if (name) return _("�饤�ȡ����ꥢ", "Light Area");
		if (desc) return _("�������Ȥ餷�Ƥ����ϰϤ��������Τ�ʵפ����뤯���롣", "Lights up nearby area and the inside of a room permanently.");
    
		{
			int dice = 2;
			int sides = plev / 2;
			int rad = plev / 10 + 1;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				lite_area(damroll(dice, sides), rad);
			}
		}
		break;

	case 6:
		if (name) return _("櫤��� �˲�", "Trap & Door Destruction");
		if (desc) return _("��ľ��������Ƥ�櫤�����˲����롣", "Fires a beam which destroy traps and doors.");
    
		{
			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				destroy_door(dir);
			}
		}
		break;

	case 7:
		if (name) return _("�ڽ��μ���", "Cure Light Wounds");
		if (desc) return _("��������Ϥ򾯤����������롣", "Heals cut and HP a little.");
    
		{
			int dice = 2;
			int sides = 8;

			if (info) return info_heal(dice, sides, 0);

			if (cast)
			{
				hp_player(damroll(dice, sides));
				set_cut(p_ptr->cut - 10);
			}
		}
		break;

	case 8:
		if (name) return _("櫤��� ����", "Detect Doors & Traps");
		if (desc) return _("�᤯�����Ƥ�櫤���ȳ��ʤ��Τ��롣", "Detects traps, doors, and stairs in your vicinity.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_traps(rad, TRUE);
				detect_doors(rad);
				detect_stairs(rad);
			}
		}
		break;

	case 9:
		if (name) return _("ǳ��", "Phlogiston");
		if (desc) return _("������ǳ������뤹�롣", "Adds more turns of light to a lantern or torch.");
    
		{
			if (cast)
			{
				phlogiston();
			}
		}
		break;

	case 10:
		if (name) return _("��������", "Detect Treasure");
		if (desc) return _("�᤯�κ������Τ��롣", "Detects all treasures in your vicinity.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_treasure(rad);
				detect_objects_gold(rad);
			}
		}
		break;

	case 11:
		if (name) return _("��ˡ ����", "Detect Enchantment");
		if (desc) return _("�᤯����ˡ�������ä������ƥ���Τ��롣", "Detects all magical items in your vicinity.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_objects_magic(rad);
			}
		}
		break;

	case 12:
		if (name) return _("�����ƥശ��", "Detect Objects");
		if (desc) return _("�᤯�����ƤΥ����ƥ���Τ��롣", "Detects all items in your vicinity.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_objects_normal(rad);
			}
		}
		break;

	case 13:
		if (name) return _("����", "Cure Poison");
		if (desc) return _("�Ǥ����⤫�鴰���˼�������", "Cures poison status.");
    
		{
			if (cast)
			{
				set_poisoned(0);
			}
		}
		break;

	case 14:
		if (name) return _("����", "Resist Cold");
		if (desc) return _("������֡��䵤�ؤ����������롣�����ˤ�����������Ѥ��롣", "Gives resistance to cold. This resistance can be added to which from equipment for more powerful resistance.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_cold(randint1(base) + base, FALSE);
			}
		}
		break;

	case 15:
		if (name) return _("�Ѳ�", "Resist Fire");
		if (desc) return _("������֡���ؤ����������롣�����ˤ�����������Ѥ��롣", 
			"Gives resistance to fire. This resistance can be added to which from equipment for more powerful resistance.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_fire(randint1(base) + base, FALSE);
			}
		}
		break;

	case 16:
		if (name) return _("����", "Resist Lightning");
		if (desc) return _("������֡��ŷ�ؤ����������롣�����ˤ�����������Ѥ��롣", 
			"Gives resistance to electricity. This resistance can be added to which from equipment for more powerful resistance.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_elec(randint1(base) + base, FALSE);
			}
		}
		break;

	case 17:
		if (name) return _("�ѻ�", "Resist Acid");
		if (desc) return _("������֡����ؤ����������롣�����ˤ�����������Ѥ��롣", 
			"Gives resistance to acid. This resistance can be added to which from equipment for more powerful resistance.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_acid(randint1(base) + base, FALSE);
			}
		}
		break;

	case 18:
		if (name) return _("�Ž��μ���", "Cure Medium Wounds");
		if (desc) return _("��������Ϥ������ٲ��������롣", "Heals cut and HP more.");
    
		{
			int dice = 4;
			int sides = 8;

			if (info) return info_heal(dice, sides, 0);

			if (cast)
			{
				hp_player(damroll(dice, sides));
				set_cut((p_ptr->cut / 2) - 50);
			}
		}
		break;

	case 19:
		if (name) return _("�ƥ�ݡ���", "Teleport");
		if (desc) return _("���Υ�Υƥ�ݡ��Ȥ򤹤롣", "Teleport long distance.");
    
		{
			POSITION range = plev * 5;

			if (info) return info_range(range);

			if (cast)
			{
				teleport_player(range, 0L);
			}
		}
		break;

	case 20:
		if (name) return _("����", "Identify");
		if (desc) return _("�����ƥ���̤��롣", "Identifies an item.");
    
		{
			if (cast)
			{
				if (!ident_spell(FALSE)) return NULL;
			}
		}
		break;

	case 21:
		if (name) return _("�����ϲ�", "Stone to Mud");
		if (desc) return _("�ɤ��Ϥ����ƾ��ˤ��롣", "Turns one rock square to mud.");
    
		{
			int dice = 1;
			int sides = 30;
			int base = 20;

			if (info) return info_damage(dice, sides, base);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				wall_to_mud(dir, 20 + randint1(30));
			}
		}
		break;

	case 22:
		if (name) return _("����", "Ray of Light");
		if (desc) return _("���������ġ�����������󥹥����˸��̤����롣", "Fires a beam of light which damages to light-sensitive monsters.");
    
		{
			int dice = 6;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				msg_print(_("�����������줿��", "A line of light appears."));
				lite_line(dir, damroll(6, 8));
			}
		}
		break;

	case 23:
		if (name) return _("��ʢ��­", "Satisfy Hunger");
		if (desc) return _("��ʢ�ˤ��롣", "Satisfies hunger.");
    
		{
			if (cast)
			{
				set_food(PY_FOOD_MAX - 1);
			}
		}
		break;

	case 24:
		if (name) return _("Ʃ����ǧ", "See Invisible");
		if (desc) return _("������֡�Ʃ���ʤ�Τ�������褦�ˤʤ롣", "Gives see invisible for a while.");
    
		{
			int base = 24;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_tim_invis(randint1(base) + base, FALSE);
			}
		}
		break;

	case 25:
		if (name) return _("�����󥿥뾤��", "Conjure Elemental");
		if (desc) return _("1�ΤΥ����󥿥�򾤴����롣", "Summons an elemental.");
    
		{
			if (cast)
			{
				if (!summon_specific(-1, p_ptr->y, p_ptr->x, plev, SUMMON_ELEMENTAL, (PM_ALLOW_GROUP | PM_FORCE_PET)))
				{
					msg_print(_("�����󥿥�ϸ���ʤ��ä���", "No Elementals arrive."));
				}
			}
		}
		break;

	case 26:
		if (name) return _("�ƥ�ݡ��ȡ���٥�", "Teleport Level");
		if (desc) return _("�ֻ��˾夫���γ��˥ƥ�ݡ��Ȥ��롣", "Teleport to up or down stairs in a moment.");
    
		{
			if (cast)
			{
				if (!get_check(_("������¾�γ��˥ƥ�ݡ��Ȥ��ޤ�����", "Are you sure? (Teleport Level)"))) return NULL;
				teleport_level(0);
			}
		}
		break;

	case 27:
		if (name) return _("�ƥ�ݡ��ȡ���󥹥���", "Teleport Away");
		if (desc) return _("��󥹥�����ƥ�ݡ��Ȥ�����ӡ�������ġ��񹳤�����̵����", "Teleports all monsters on the line away unless resisted.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_beam(GF_AWAY_ALL, dir, power);
			}
		}
		break;

	case 28:
		if (name) return _("���Ǥε�", "Elemental Ball");
		if (desc) return _("�ꡢ�ŷ⡢�䵤�����Τɤ줫�ε�����ġ�", "Fires a ball of some elements.");
    
		{
			HIT_POINT dam = 75 + plev;
			int rad = 2;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				int type;

				if (!get_aim_dir(&dir)) return NULL;

				switch (randint1(4))
				{
					case 1:  type = GF_FIRE;break;
					case 2:  type = GF_ELEC;break;
					case 3:  type = GF_COLD;break;
					default: type = GF_ACID;break;
				}

				fire_ball(type, dir, dam, rad);
			}
		}
		break;

	case 29:
		if (name) return _("������", "Detection");
		if (desc) return _("�᤯�����ƤΥ�󥹥�����櫡��⡢���ʡ������������ƥ����ƥ���Τ��롣", 
			"Detects all monsters, traps, doors, stairs, treasures and items in your vicinity.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_all(rad);
			}
		}
		break;

	case 30:
		if (name) return _("���Ԥμ�ʸ", "Word of Recall");
		if (desc) return _("�Ͼ�ˤ���Ȥ��ϥ��󥸥��κǿ����ء����󥸥��ˤ���Ȥ����Ͼ�ؤȰ�ư���롣", 
			"Recalls player from dungeon to town, or from town to the deepest level of dungeon.");
    
		{
			int base = 15;
			int sides = 20;

			if (info) return info_delay(base, sides);

			if (cast)
			{
				if (!word_of_recall()) return NULL;
			}
		}
		break;

	case 31:
		if (name) return _("��Τ��", "Clairvoyance");
		if (desc) return _("���γ����Τ�ʵפ˾Ȥ餷�����󥸥���⤹�٤ƤΥ����ƥ���Τ��롣����ˡ�������֥ƥ�ѥ���ǽ�Ϥ����롣", 
			"Maps and lights whole dungeon level. Knows all objects location. And gives telepathy for a while.");
    
		{
			int base = 25;
			int sides = 30;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				chg_virtue(V_KNOWLEDGE, 1);
				chg_virtue(V_ENLIGHTEN, 1);

				wiz_lite(FALSE);

				if (!p_ptr->telepathy)
				{
					set_tim_esp(randint1(sides) + base, FALSE);
				}
			}
		}
		break;
	}

	return "";
}

/*!
 * @brief ���ΰ���ˡ�γƽ�����Ԥ�
 * @param spell ��ˡID
 * @param mode �������� (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
 * @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ˤ�ʸ����ݥ��󥿤��֤���SPELL_CAST����NULLʸ������֤���
 */
static cptr do_craft_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;

	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("�ֳ�������", "Infravision");
		if (desc) return _("������֡��ֳ������Ϥ���������롣", "Gives infravision for a while.");
    
		{
			int base = 100;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_tim_infra(base + randint1(base), FALSE);
			}
		}
		break;

	case 1:
		if (name) return _("�����϶���", "Regeneration");
		if (desc) return _("������֡������Ϥ���������롣", "Gives regeneration ability for a while.");
    
		{
			int base = 80;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_tim_regen(base + randint1(base), FALSE);
			}
		}
		break;

	case 2:
		if (name) return _("��ʢ��­", "Satisfy Hunger");
		if (desc) return _("��ʢ�ˤʤ롣", "Satisfies hunger.");
    
		{
			if (cast)
			{
				set_food(PY_FOOD_MAX - 1);
			}
		}
		break;

	case 3:
		if (name) return _("���䵤", "Resist Cold");
		if (desc) return _("������֡��䵤�ؤ����������롣�����ˤ�����������Ѥ��롣", 
			"Gives resistance to cold. This resistance can be added to which from equipment for more powerful resistance.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_cold(randint1(base) + base, FALSE);
			}
		}
		break;

	case 4:
		if (name) return _("�Ѳб�", "Resist Fire");
		if (desc) return _("������֡���ؤ����������롣�����ˤ�����������Ѥ��롣", 
			"Gives resistance to fire. This resistance can be added to which from equipment for more powerful resistance.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_fire(randint1(base) + base, FALSE);
			}
		}
		break;

	case 5:
		if (name) return _("�ε�����", "Heroism");
		if (desc) return _("������֡��ҡ�����ʬ�ˤʤ롣", "Removes fear, and gives bonus to hit and 10 more HP for a while.");
    
		{
			int base = 25;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_hero(randint1(base) + base, FALSE);
				hp_player(10);
				set_afraid(0);
			}
		}
		break;

	case 6:
		if (name) return _("���ŷ�", "Resist Lightning");
		if (desc) return _("������֡��ŷ�ؤ����������롣�����ˤ�����������Ѥ��롣",
			"Gives resistance to electricity. This resistance can be added to which from equipment for more powerful resistance.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_elec(randint1(base) + base, FALSE);
			}
		}
		break;

	case 7:
		if (name) return _("�ѻ�", "Resist Acid");
		if (desc) return _("������֡����ؤ����������롣�����ˤ�����������Ѥ��롣",
			"Gives resistance to acid. This resistance can be added to which from equipment for more powerful resistance.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_acid(randint1(base) + base, FALSE);
			}
		}
		break;

	case 8:
		if (name) return _("Ʃ����ǧ", "See Invisibility");
		if (desc) return _("������֡�Ʃ���ʤ�Τ�������褦�ˤʤ롣", "Gives see invisible for a while.");
    
		{
			int base = 24;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_tim_invis(randint1(base) + base, FALSE);
			}
		}
		break;

	case 9:
		if (name) return _("���", "Remove Curse");
		if (desc) return _("�����ƥ�ˤ����ä��夤�����������롣", "Removes normal curses from equipped items.");
    
		{
			if (cast)
			{
				if (remove_curse())
				{
					msg_print(_("ï���˸�����Ƥ���褦�ʵ������롣", "You feel as if someone is watching over you."));
				}
			}
		}
		break;

	case 10:
		if (name) return _("����", "Resist Poison");
		if (desc) return _("������֡��Ǥؤ����������롣�����ˤ�����������Ѥ��롣",
			"Gives resistance to poison. This resistance can be added to which from equipment for more powerful resistance.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_pois(randint1(base) + base, FALSE);
			}
		}
		break;

	case 11:
		if (name) return _("����β�", "Berserk");
		if (desc) return _("����β��������ݤ����롣", "Gives bonus to hit and HP, immunity to fear for a while. But decreases AC.");
    
		{
			int base = 25;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_shero(randint1(base) + base, FALSE);
				hp_player(30);
				set_afraid(0);
			}
		}
		break;

	case 12:
		if (name) return _("����ʬ��", "Self Knowledge");
		if (desc) return _("���ߤμ�ʬ�ξ��֤������Τ롣",
			"Gives you useful info regarding your current resistances, the powers of your weapon and maximum limits of your stats.");
    
		{
			if (cast)
			{
				self_knowledge();
			}
		}
		break;

	case 13:
		if (name) return _("�мٰ��볦", "Protection from Evil");
		if (desc) return _("�ٰ��ʥ�󥹥����ι�����ɤ��Хꥢ��ĥ�롣", "Gives aura which protect you from evil monster's physical attack.");
    
		{
			int base = 3 * plev;
			int sides = 25;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_protevil(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 14:
		if (name) return _("����", "Cure");
		if (desc) return _("�ǡ�ۯ۰���֡�������������������Ф�ľ����", "Heals poison, stun, cut and hallucination completely.");
    
		{
			if (cast)
			{
				set_poisoned(0);
				set_stun(0);
				set_cut(0);
				set_image(0);
			}
		}
		break;

	case 15:
		if (name) return _("��ˡ��", "Mana Branding");
		if (desc) return _("������֡������䵤���ꡢ�ŷ⡢�����ǤΤ����줫��°����Ĥ��롣��������ʤ��ȻȤ��ʤ���",
			"Makes current weapon some elemental branded. You must wield weapons.");
    
		{
			int base = plev / 2;

			if (info) return info_duration(base, base);

			if (cast)
			{
				if (!choose_ele_attack()) return NULL;
			}
		}
		break;

	case 16:
		if (name) return _("�ƥ�ѥ���", "Telepathy");
		if (desc) return _("������֡��ƥ�ѥ���ǽ�Ϥ����롣", "Gives telepathy for a while.");
    
		{
			int base = 25;
			int sides = 30;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_tim_esp(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 17:
		if (name) return _("ȩ�в�", "Stone Skin");
		if (desc) return _("������֡�AC��徺�����롣", "Gives bonus to AC for a while.");
    
		{
			int base = 30;
			int sides = 20;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_shield(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 18:
		if (name) return _("������", "Resistance");
		if (desc) return _("������֡������ŷ⡢�ꡢ�䵤���Ǥ��Ф������������롣�����ˤ�����������Ѥ��롣", 
			"Gives resistance to fire, cold, electricity, acid and poison for a while. These resistances can be added to which from equipment for more powerful resistances.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_acid(randint1(base) + base, FALSE);
				set_oppose_elec(randint1(base) + base, FALSE);
				set_oppose_fire(randint1(base) + base, FALSE);
				set_oppose_cold(randint1(base) + base, FALSE);
				set_oppose_pois(randint1(base) + base, FALSE);
			}
		}
		break;

	case 19:
		if (name) return _("���ԡ���", "Haste Self");
		if (desc) return _("������֡���®���롣", "Hastes you for a while.");
    
		{
			int base = plev;
			int sides = 20 + plev;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_fast(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 20:
		if (name) return _("��ȴ��", "Walk through Wall");
		if (desc) return _("������֡�Ⱦʪ�������ɤ��̤�ȴ������褦�ˤʤ롣", "Gives ability to pass walls for a while.");
    
		{
			int base = plev / 2;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_kabenuke(randint1(base) + base, FALSE);
			}
		}
		break;

	case 21:
		if (name) return _("���᤭", "Polish Shield");
		if (desc) return _("���ȿ�ͤ�°����Ĥ��롣", "Makes a shield a shield of reflection.");
    
		{
			if (cast)
			{
				pulish_shield();
			}
		}
		break;

	case 22:
		if (name) return _("���������¤", "Create Golem");
		if (desc) return _("1�ΤΥ���������¤���롣", "Creates a golem.");
    
		{
			if (cast)
			{
				if (summon_specific(-1, p_ptr->y, p_ptr->x, plev, SUMMON_GOLEM, PM_FORCE_PET))
				{
					msg_print(_("���������ä���", "You make a golem."));
				}
				else
				{
					msg_print(_("���ޤ�����������ʤ��ä���", "No Golems arrive."));
				}
			}
		}
		break;

	case 23:
		if (name) return _("��ˡ�γ�", "Magical armor");
		if (desc) return _("������֡���ˡ�ɸ��Ϥ�AC���夬�ꡢ��������ܤ�������ȿ��ǽ�ϡ������Τ餺����ͷ�����롣",
			"Gives resistance to magic, bonus to AC, resistance to confusion, blindness, reflection, free action and levitation for a while.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_magicdef(randint1(base) + base, FALSE);
			}
		}
		break;

	case 24:
		if (name) return _("����̵�ϲ�", "Remove Enchantment");
		if (desc) return _("���ɶ�ˤ�����줿���������Ϥ����˲�����롣", "Removes all magics completely from any weapon or armor.");
    
		{
			if (cast)
			{
				if (!mundane_spell(TRUE)) return NULL;
			}
		}
		break;

	case 25:
		if (name) return _("����ʴ��", "Remove All Curse");
		if (desc) return _("�����ƥ�ˤ����ä����Ϥʼ����������롣", "Removes normal and heavy curse from equipped items.");
    
		{
			if (cast)
			{
				if (remove_all_curse())
				{
					msg_print(_("ï���˸�����Ƥ���褦�ʵ������롣", "You feel as if someone is watching over you."));
				}
			}
		}
		break;

	case 26:
		if (name) return _("�����ʤ��μ�", "Knowledge True");
		if (desc) return _("�����ƥ�λ���ǽ�Ϥ������Τ롣", "*Identifies* an item.");
    
		{
			if (cast)
			{
				if (!identify_fully(FALSE)) return NULL;
			}
		}
		break;

	case 27:
		if (name) return _("��ﶯ��", "Enchant Weapon");
		if (desc) return _("����̿��Ψ�����ȥ��᡼�������򶯲����롣", "Attempts to increase +to-hit, +to-dam of a weapon.");
    
		{
			if (cast)
			{
				if (!enchant_spell(randint0(4) + 1, randint0(4) + 1, 0)) return NULL;
			}
		}
		break;

	case 28:
		if (name) return _("�ɶ񶯲�", "Enchant Armor");
		if (desc) return _("�����ɸ潤���򶯲����롣", "Attempts to increase +AC of an armor.");
    
		{
			if (cast)
			{
				if (!enchant_spell(0, 0, randint0(3) + 2)) return NULL;
			}
		}
		break;

	case 29:
		if (name) return _("���°����Ϳ", "Brand Weapon");
		if (desc) return _("���˥������°����Ĥ��롣", "Makes current weapon a random ego weapon.");
    
		{
			if (cast)
			{
				brand_weapon(randint0(18));
			}
		}
		break;

	case 30:
		if (name) return _("�ʹ֥ȥ���", "Living Trump");
		if (desc) return _("������˥ƥ�ݡ��Ȥ��������Ѱۤ�����ʬ�ΰջפǥƥ�ݡ��Ȥ��������Ѱۤ��ȤˤĤ���",
			"Gives mutation which makes you teleport randomly or makes you able to teleport at will.");
    
		{
			if (cast)
			{
				int mutation;

				if (one_in_(7))
					/* Teleport control */
					mutation = 12;
				else
					/* Random teleportation (uncontrolled) */
					mutation = 77;

				/* Gain the mutation */
				if (gain_random_mutation(mutation))
				{
					msg_print(_("���ʤ��������Ƥ��륫���ɤ��Ѥ�ä���", "You have turned into a Living Trump."));
				}
			}
		}
		break;

	case 31:
		if (name) return _("°���ؤ��ȱ�", "Immunity");
		if (desc) return _("������֡��䵤���ꡢ�ŷ⡢���Τ����줫���Ф����ȱ֤����롣",
			"Gives an immunity to fire, cold, electricity or acid for a while.");
    
		{
			int base = 13;

			if (info) return info_duration(base, base);

			if (cast)
			{
				if (!choose_ele_immune(base + randint1(base))) return NULL;
			}
		}
		break;
	}

	return "";
}

/*!
 * @brief �����ΰ���ˡ�γƽ�����Ԥ�
 * @param spell ��ˡID
 * @param mode �������� (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
 * @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ˤ�ʸ����ݥ��󥿤��֤���SPELL_CAST����NULLʸ������֤���
 */
static cptr do_daemon_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;
	static const char s_dam[] = _("»��:", "dam ");

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("�ޥ��å����ߥ�����", "Magic Missile");
		if (desc) return _("�夤��ˡ��������ġ�", "Fires a weak bolt of magic.");
    
		{
			int dice = 3 + (plev - 1) / 5;
			int sides = 4;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance() - 10, GF_MISSILE, dir, damroll(dice, sides));
			}
		}
		break;

	case 1:
		if (name) return _("̵��̿����", "Detect Unlife");
		if (desc) return _("�᤯����̿�Τʤ���󥹥������Τ��롣", "Detects all nonliving monsters in your vicinity.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_monsters_nonliving(rad);
			}
		}
		break;

	case 2:
		if (name) return _("�٤ʤ��ʡ", "Evil Bless");
		if (desc) return _("������֡�̿��Ψ��AC�˥ܡ��ʥ������롣", "Gives bonus to hit and AC for a few turns.");
    
		{
			int base = 12;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_blessed(randint1(base) + base, FALSE);
			}
		}
		break;

	case 3:
		if (name) return _("�Ѳб�", "Resist Fire");
		if (desc) return _("������֡���ؤ����������롣�����ˤ�����������Ѥ��롣", 
			"Gives resistance to fire, cold and electricity for a while. These resistances can be added to which from equipment for more powerful resistances.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_fire(randint1(base) + base, FALSE);
			}
		}
		break;

	case 4:
		if (name) return _("����", "Horrify");
		if (desc) return _("��󥹥���1�Τ��ݤ�����ۯ۰�����롣�񹳤�����̵����", "Attempts to scare and stun a monster.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fear_monster(dir, power);
				stun_monster(dir, power);
			}
		}
		break;

	case 5:
		if (name) return _("�Ϲ�����", "Nether Bolt");
		if (desc) return _("�Ϲ��Υܥ�Ȥ⤷���ϥӡ�������ġ�", "Fires a bolt or beam of nether.");
    
		{
			int dice = 6 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance(), GF_NETHER, dir, damroll(dice, sides));
			}
		}
		break;

	case 6:
		if (name) return _("����λ����", "Summon Manes");
		if (desc) return _("����λ���򾤴����롣", "Summons a manes.");
    
		{
			if (cast)
			{
				if (!summon_specific(-1, p_ptr->y, p_ptr->x, (plev * 3) / 2, SUMMON_MANES, (PM_ALLOW_GROUP | PM_FORCE_PET)))
				{
					msg_print(_("����λ���ϸ���ʤ��ä���", "No Manes arrive."));
				}
			}
		}
		break;

	case 7:
		if (name) return _("�Ϲ��α�", "Hellish Flame");
		if (desc) return _("�ٰ����Ϥ���ĥܡ�������ġ����ɤʥ�󥹥����ˤ��礭�ʥ��᡼����Ϳ���롣",
			"Fires a ball of evil power. Hurts good monsters greatly.");
    
		{
			int dice = 3;
			int sides = 6;
			int rad = (plev < 30) ? 2 : 3;
			int base;

			if (p_ptr->pclass == CLASS_MAGE ||
			    p_ptr->pclass == CLASS_HIGH_MAGE ||
			    p_ptr->pclass == CLASS_SORCERER)
				base = plev + plev / 2;
			else
				base = plev + plev / 4;


			if (info) return info_damage(dice, sides, base);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_HELL_FIRE, dir, damroll(dice, sides) + base, rad);
			}
		}
		break;

	case 8:
		if (name) return _("�ǡ�������", "Dominate Demon");
		if (desc) return _("����1�Τ�̥λ���롣�񹳤�����̵��", "Attempts to charm a demon.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				control_one_demon(dir, power);
			}
		}
		break;

	case 9:
		if (name) return _("�ӥ����", "Vision");
		if (desc) return _("���դ��Ϸ����Τ��롣", "Maps nearby area.");
    
		{
			int rad = DETECT_RAD_MAP;

			if (info) return info_radius(rad);

			if (cast)
			{
				map_area(rad);
			}
		}
		break;

	case 10:
		if (name) return _("���Ϲ�", "Resist Nether");
		if (desc) return _("������֡��Ϲ��ؤ����������롣", "Gives resistance to nether for a while.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_tim_res_nether(randint1(base) + base, FALSE);
			}
		}
		break;

	case 11:
		if (name) return _("�ץ饺�ޡ��ܥ��", "Plasma bolt");
		if (desc) return _("�ץ饺�ޤΥܥ�Ȥ⤷���ϥӡ�������ġ�", "Fires a bolt or beam of plasma.");
    
		{
			int dice = 11 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance(), GF_PLASMA, dir, damroll(dice, sides));
			}
		}
		break;

	case 12:
		if (name) return _("�ե��������ܡ���", "Fire Ball");
		if (desc) return _("��ε�����ġ�", "Fires a ball of fire.");
    
		{
			HIT_POINT dam = plev + 55;
			int rad = 2;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_FIRE, dir, dam, rad);
			}
		}
		break;

	case 13:
		if (name) return _("��ο�", "Fire Branding");
		if (desc) return _("���˱��°����Ĥ��롣", "Makes current weapon fire branded.");
    
		{
			if (cast)
			{
				brand_weapon(1);
			}
		}
		break;

	case 14:
		if (name) return _("�Ϲ���", "Nether Ball");
		if (desc) return _("�礭���Ϲ��ε�����ġ�", "Fires a huge ball of nether.");
    
		{
			HIT_POINT dam = plev * 3 / 2 + 100;
			int rad = plev / 20 + 2;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_NETHER, dir, dam, rad);
			}
		}
		break;

	case 15:
		if (name) return _("�ǡ���󾤴�", "Summon Demon");
		if (desc) return _("����1�Τ򾤴����롣", "Summons a demon.");
    
		{
			if (cast)
			{
				bool pet = !one_in_(3);
				u32b flg = 0L;

				if (pet) flg |= PM_FORCE_PET;
				else flg |= PM_NO_PET;
				if (!(pet && (plev < 50))) flg |= PM_ALLOW_GROUP;

				if (summon_specific((pet ? -1 : 0), p_ptr->y, p_ptr->x, plev*2/3+randint1(plev/2), SUMMON_DEMON, flg))
				{
					msg_print(_("β���ΰ���������������", "The area fills with a stench of sulphur and brimstone."));

					if (pet)
					{
						msg_print(_("�֤��ѤǤ������ޤ�����������͡�", "'What is thy bidding... Master?'"));
					}
					else
					{
						msg_print(_("���ܤ����Ԥ衢�����β��ͤˤ��餺�� �����κ���ĺ��������",
									"'NON SERVIAM! Wretch! I shall feast on thy mortal soul!'"));
					}
				}
				else
				{
					msg_print(_("����ϸ���ʤ��ä���", "No demons arrive."));
				}
				break;
			}
		}
		break;

	case 16:
		if (name) return _("�������", "Devilish Eye");
		if (desc) return _("������֡��ƥ�ѥ���ǽ�Ϥ����롣", "Gives telepathy for a while.");
    
		{
			int base = 30;
			int sides = 25;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_tim_esp(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 17:
		if (name) return _("����Υ�����", "Devil Cloak");
		if (desc) return _("���ݤ��������������֡�����䵤����������Υ���������롣�����������ˤ�����������Ѥ��롣", 
			"Removes fear. Gives resistance to fire and cold, and aura of fire. These resistances can be added to which from equipment for more powerful resistances.");
    
		{
			TIME_EFFECT base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				TIME_EFFECT dur = randint1(base) + base;
					
				set_oppose_fire(dur, FALSE);
				set_oppose_cold(dur, FALSE);
				set_tim_sh_fire(dur, FALSE);
				set_afraid(0);
				break;
			}
		}
		break;

	case 18:
		if (name) return _("�ϴ�ή", "The Flow of Lava");
		if (desc) return _("��ʬ���濴�Ȥ�����ε����Ф��������ϴ���Ѥ��롣", 
			"Generates a ball of fire centered on you which transforms floors to magma.");
    
		{
			HIT_POINT dam = (55 + plev) * 2;
			int rad = 3;

			if (info) return info_damage(0, 0, dam/2);

			if (cast)
			{
				fire_ball(GF_FIRE, 0, dam, rad);
				fire_ball_hide(GF_LAVA_FLOW, 0, 2 + randint1(2), rad);
			}
		}
		break;

	case 19:
		if (name) return _("�ץ饺�޵�", "Plasma Ball");
		if (desc) return _("�ץ饺�ޤε�����ġ�", "Fires a ball of plasma.");
    
		{
			HIT_POINT dam = plev * 3 / 2 + 80;
			int rad = 2 + plev / 40;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_PLASMA, dir, dam, rad);
			}
		}
		break;

	case 20:
		if (name) return _("�����Ѳ�", "Polymorph Demon");
		if (desc) return _("������֡�������Ѳ����롣�Ѳ����Ƥ���֤�����μ�²��ǽ�Ϥ򼺤�������˰���Ȥ��Ƥ�ǽ�Ϥ����롣", 
			"Mimic a demon for a while. Loses abilities of original race and gets abilities as a demon.");
    
		{
			int base = 10 + plev / 2;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_mimic(base + randint1(base), MIMIC_DEMON, FALSE);
			}
		}
		break;

	case 21:
		if (name) return _("�Ϲ�����ư", "Nather Wave");
		if (desc) return _("�볦������ƤΥ�󥹥����˥��᡼����Ϳ���롣���ɤʥ�󥹥������ä��礭�ʥ��᡼����Ϳ���롣", 
			"Damages all monsters in sight. Hurts good monsters greatly.");
    
		{
			int sides1 = plev * 2;
			int sides2 = plev * 2;

			if (info) return format("%sd%d+d%d", s_dam, sides1, sides2);

			if (cast)
			{
				dispel_monsters(randint1(sides1));
				dispel_good(randint1(sides2));
			}
		}
		break;

	case 22:
		if (name) return _("������Х�����ʭ", "Kiss of Succubus");
		if (desc) return _("���̺���ε�����ġ�", "Fires a ball of nexus.");
    
		{
			HIT_POINT dam = 100 + plev * 2;
			int rad = 4;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_ball(GF_NEXUS, dir, dam, rad);
			}
		}
		break;

	case 23:
		if (name) return _("���Ǥμ�", "Doom Hand");
		if (desc) return _("���Ǥμ�����ġ�����ä���󥹥����Ϥ��ΤȤ���HP��Ⱦʬ����Υ��᡼��������롣", "Attempts to make a monster's HP almost half.");
    
		{
			if (cast)
			{
				if (!get_aim_dir(&dir))
					return NULL;
				else 
					msg_print(_("<���Ǥμ�>�����ä���", "You invoke the Hand of Doom!"));
				
				fire_ball_hide(GF_HAND_DOOM, dir, plev * 2, 0);
			}
		}
		break;

	case 24:
		if (name) return _("�ε�����", "Raise the Morale");
		if (desc) return _("������֡��ҡ�����ʬ�ˤʤ롣", "Removes fear, and gives bonus to hit and 10 more HP for a while.");
    
		{
			int base = 25;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_hero(randint1(base) + base, FALSE);
				hp_player(10);
				set_afraid(0);
			}
		}
		break;

	case 25:
		if (name) return _("���Ǥ�����", "Immortal Body");
		if (desc) return _("������֡����ֵ�ž�ؤ����������롣", "Gives resistance to time for a while.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_tim_res_time(randint1(base)+base, FALSE);
			}
		}
		break;

	case 26:
		if (name) return _("�����αߴ�", "Insanity Circle");
		if (desc) return _("��ʬ���濴�Ȥ����������ε塢����ε��ȯ���������᤯�Υ�󥹥�����̥λ���롣", 
			"Generate balls of chaos, confusion and charm centered on you.");
    
		{
			HIT_POINT dam = 50 + plev;
			int power = 20 + plev;
			int rad = 3 + plev / 20;

			if (info) return format("%s%d+%d", s_dam, dam/2, dam/2);

			if (cast)
			{
				fire_ball(GF_CHAOS, 0, dam, rad);
				fire_ball(GF_CONFUSION, 0, dam, rad);
				fire_ball(GF_CHARM, 0, power, rad);
			}
		}
		break;

	case 27:
		if (name) return _("�ڥå�����", "Explode Pets");
		if (desc) return _("���ƤΥڥåȤ���Ū�����ˤ����롣", "Makes all pets explode.");
    
		{
			if (cast)
			{
				discharge_minion();
			}
		}
		break;

	case 28:
		if (name) return _("���졼�����ǡ���󾤴�", "Summon Greater Demon");
		if (desc) return _("���ǡ����򾤴����롣��������ˤϿʹ�('p','h','t'��ɽ������󥹥���)�λ��Τ������ʤ���Фʤ�ʤ���", 
			"Summons greater demon. It need to sacrifice a corpse of human ('p','h' or 't').");
    
		{
			if (cast)
			{
				if (!cast_summon_greater_demon()) return NULL;
			}
		}
		break;

	case 29:
		if (name) return _("�Ϲ���", "Nether Storm");
		if (desc) return _("Ķ������Ϲ��ε�����ġ�", "Generate a huge ball of nether.");
    
		{
			HIT_POINT dam = plev * 15;
			int rad = plev / 5;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_NETHER, dir, dam, rad);
			}
		}
		break;

	case 30:
		if (name) return _("��μ���", "Bloody Curse");
		if (desc) return _("��ʬ�����᡼��������뤳�Ȥˤ�ä��оݤ˼����򤫤������᡼����Ϳ���͡��ʸ��̤������������",
			"Puts blood curse which damages and causes various effects on a monster. You also take damage.");
    
		{
			HIT_POINT dam = 600;
			int rad = 0;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball_hide(GF_BLOOD_CURSE, dir, dam, rad);
				take_hit(DAMAGE_USELIFE, 20 + randint1(30), _("��μ���", "Blood curse"), -1);
			}
		}
		break;

	case 31:
		if (name) return _("�Ⲧ�Ѳ�", "Polymorph Demonlord");
		if (desc) return _("����β����Ѳ����롣�Ѳ����Ƥ���֤�����μ�²��ǽ�Ϥ򼺤�������˰���β��Ȥ��Ƥ�ǽ�Ϥ������ɤ��˲����ʤ����⤯��",
			"Mimic a demon lord for a while. Loses abilities of original race and gets great abilities as a demon lord. Even hard walls can't stop your walking.");
    
		{
			int base = 15;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_mimic(base + randint1(base), MIMIC_DEMON_LORD, FALSE);
			}
		}
		break;
	}

	return "";
}

/*!
 * @brief �˼��ΰ���ˡ�γƽ�����Ԥ�
 * @param spell ��ˡID
 * @param mode �������� (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
 * @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ˤ�ʸ����ݥ��󥿤��֤���SPELL_CAST����NULLʸ������֤���
 */
static cptr do_crusade_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("Ĩȳ", "Punishment");
		if (desc) return _("�ŷ�Υܥ�Ȥ⤷���ϥӡ�������ġ�", "Fires a bolt or beam of lightning.");
    
		{
			int dice = 3 + (plev - 1) / 5;
			int sides = 4;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance() - 10, GF_ELEC, dir, damroll(dice, sides));
			}
		}
		break;

	case 1:
		if (name) return _("�ٰ�¸�ߴ���", "Detect Evil");
		if (desc) return _("�᤯�μٰ��ʥ�󥹥������Τ��롣", "Detects all evil monsters in your vicinity.");
    
		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_monsters_evil(rad);
			}
		}
		break;

	case 2:
		if (name) return _("���ݽ���", "Remove Fear");
		if (desc) return _("���ݤ��������", "Removes fear.");
    
		{
			if (cast)
			{
				set_afraid(0);
			}
		}
		break;

	case 3:
		if (name) return _("�Ұ�", "Scare Monster");
		if (desc) return _("��󥹥���1�Τ��ݤ����롣�񹳤�����̵����", "Attempts to scare a monster.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fear_monster(dir, power);
			}
		}
		break;

	case 4:
		if (name) return _("����", "Sanctuary");
		if (desc) return _("���ܤ������ƤΥ�󥹥�����̲�餻�롣�񹳤�����̵����", "Attempts to sleep monsters in the adjacent squares.");
    
		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				sleep_monsters_touch();
			}
		}
		break;

	case 5:
		if (name) return _("����", "Portal");
		if (desc) return _("���Υ�Υƥ�ݡ��Ȥ򤹤롣", "Teleport medium distance.");
    
		{
			POSITION range = 25 + plev / 2;

			if (info) return info_range(range);

			if (cast)
			{
				teleport_player(range, 0L);
			}
		}
		break;

	case 6:
		if (name) return _("������������", "Star Dust");
		if (desc) return _("�������å��ն�������Υܥ�Ȥ�Ϣ�ͤ��롣", "Fires many bolts of light near the target.");
    
		{
			int dice = 3 + (plev - 1) / 9;
			int sides = 2;

			if (info) return info_multi_damage_dice(dice, sides);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_blast(GF_LITE, dir, dice, sides, 10, 3);
			}
		}
		break;

	case 7:
		if (name) return _("���ξ���", "Purify");
		if (desc) return _("�����ǡ�ۯ۰�����������롣", "Heals all cut, stun and poison status.");
    
		{
			if (cast)
			{
				set_cut(0);
				set_poisoned(0);
				set_stun(0);
			}
		}
		break;

	case 8:
		if (name) return _("�ٰ����Ф�", "Scatter Evil");
		if (desc) return _("�ٰ��ʥ�󥹥���1�Τ�ƥ�ݡ��Ȥ����롣�񹳤�����̵����", "Attempts to teleport an evil monster away.");
    
		{
			int power = MAX_SIGHT * 5;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_ball(GF_AWAY_EVIL, dir, power, 0);
			}
		}
		break;

	case 9:
		if (name) return _("���ʤ����", "Holy Orb");
		if (desc) return _("���ʤ��Ϥ�����������ġ��ٰ��ʥ�󥹥������Ф����礭�ʥ��᡼����Ϳ���뤬�����ɤʥ�󥹥����ˤϸ��̤��ʤ���", 
			"Fires a ball with holy power. Hurts evil monsters greatly, but don't effect good monsters.");
    
		{
			int dice = 3;
			int sides = 6;
			int rad = (plev < 30) ? 2 : 3;
			int base;

			if (p_ptr->pclass == CLASS_PRIEST ||
			    p_ptr->pclass == CLASS_HIGH_MAGE ||
			    p_ptr->pclass == CLASS_SORCERER)
				base = plev + plev / 2;
			else
				base = plev + plev / 4;


			if (info) return info_damage(dice, sides, base);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_HOLY_FIRE, dir, damroll(dice, sides) + base, rad);
			}
		}
		break;

	case 10:
		if (name) return _("����ʧ��", "Exorcism");
		if (desc) return _("�볦������ƤΥ���ǥåɵڤӰ���˥��᡼����Ϳ�����ٰ��ʥ�󥹥������ݤ����롣", 
			"Damages all undead and demons in sight, and scares all evil monsters in sight.");
    
		{
			int sides = plev;
			int power = plev;

			if (info) return info_damage(1, sides, 0);

			if (cast)
			{
				dispel_undead(randint1(sides));
				dispel_demons(randint1(sides));
				turn_evil(power);
			}
		}
		break;

	case 11:
		if (name) return _("���", "Remove Curse");
		if (desc) return _("�����ƥ�ˤ����ä��夤�����������롣", "Removes normal curses from equipped items.");
    
		{
			if (cast)
			{
				if (remove_curse())
				{
					msg_print(_("ï���˸�����Ƥ���褦�ʵ������롣", "You feel as if someone is watching over you."));
				}
			}
		}
		break;

	case 12:
		if (name) return _("Ʃ����ǧ", "Sense Unseen");
		if (desc) return _("������֡�Ʃ���ʤ�Τ�������褦�ˤʤ롣", "Gives see invisible for a while.");
    
		{
			int base = 24;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_tim_invis(randint1(base) + base, FALSE);
			}
		}
		break;

	case 13:
		if (name) return _("�мٰ��볦", "Protection from Evil");
		if (desc) return _("�ٰ��ʥ�󥹥����ι�����ɤ��Хꥢ��ĥ�롣", "Gives aura which protect you from evil monster's physical attack.");
    
		{
			int base = 25;
			int sides = 3 * plev;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_protevil(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 14:
		if (name) return _("�ۤ�����", "Judgment Thunder");
		if (desc) return _("���Ϥ��ŷ�Υܥ�Ȥ����ġ�", "Fires a powerful bolt of lightning.");
    
		{
			HIT_POINT dam = plev * 5;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_bolt(GF_ELEC, dir, dam);
			}
		}
		break;

	case 15:
		if (name) return _("���ʤ�����", "Holy Word");
		if (desc) return _("�볦��μٰ���¸�ߤ��礭�ʥ��᡼����Ϳ�������Ϥ���������ǡ����ݡ�ۯ۰���֡���������������롣",
			"Damages all evil monsters in sight, heals HP somewhat, and completely heals poison, fear, stun and cut status.");
    
		{
			int dam_sides = plev * 6;
			int heal = 100;

			if (info) return format(_("»:1d%d/��%d", "dam:d%d/h%d"), dam_sides, heal);
			if (cast)
			{
				dispel_evil(randint1(dam_sides));
				hp_player(heal);
				set_afraid(0);
				set_poisoned(0);
				set_stun(0);
				set_cut(0);
			}
		}
		break;

	case 16:
		if (name) return _("�����줿ƻ", "Unbarring Ways");
		if (desc) return _("��ľ��������Ƥ�櫤�����˲����롣", "Fires a beam which destroy traps and doors.");
    
		{
			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				destroy_door(dir);
			}
		}
		break;

	case 17:
		if (name) return _("����", "Arrest");
		if (desc) return _("�ٰ��ʥ�󥹥�����ư����ߤ�롣", "Attempts to paralyze an evil monster.");
    
		{
			int power = plev * 2;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				stasis_evil(dir);
			}
		}
		break;

	case 18:
		if (name) return _("���ʤ륪����", "Holy Aura");
		if (desc) return _("������֡��ٰ��ʥ�󥹥�������Ĥ������ʤ륪��������롣",
			"Gives aura of holy power which injures evil monsters which attacked you for a while.");
    
		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_tim_sh_holy(randint1(base) + base, FALSE);
			}
		}
		break;

	case 19:
		if (name) return _("����ǥå�&�����໶", "Dispel Undead & Demons");
		if (desc) return _("�볦������ƤΥ���ǥåɵڤӰ���˥��᡼����Ϳ���롣", "Damages all undead and demons in sight.");
    
		{
			int sides = plev * 4;

			if (info) return info_damage(1, sides, 0);

			if (cast)
			{
				dispel_undead(randint1(sides));
				dispel_demons(randint1(sides));
			}
		}
		break;

	case 20:
		if (name) return _("�ٰ��໶", "Dispel Evil");
		if (desc) return _("�볦������Ƥμٰ��ʥ�󥹥����˥��᡼����Ϳ���롣", "Damages all evil monsters in sight.");
    
		{
			int sides = plev * 4;

			if (info) return info_damage(1, sides, 0);

			if (cast)
			{
				dispel_evil(randint1(sides));
			}
		}
		break;

	case 21:
		if (name) return _("���ʤ��", "Holy Blade");
		if (desc) return _("�̾�������Ǽ٤�°����Ĥ��롣", "Makes current weapon especially deadly against evil monsters.");
    
		{
			if (cast)
			{
				brand_weapon(13);
			}
		}
		break;

	case 22:
		if (name) return _("�������С�����", "Star Burst");
		if (desc) return _("����������ε�����ġ�", "Fires a huge ball of powerful light.");
    
		{
			HIT_POINT dam = 100 + plev * 2;
			int rad = 4;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_LITE, dir, dam, rad);
			}
		}
		break;

	case 23:
		if (name) return _("ŷ�Ⱦ���", "Summon Angel");
		if (desc) return _("ŷ�Ȥ�1�ξ������롣", "Summons an angel.");
    
		{
			if (cast)
			{
				bool pet = !one_in_(3);
				u32b flg = 0L;

				if (pet) flg |= PM_FORCE_PET;
				else flg |= PM_NO_PET;
				if (!(pet && (plev < 50))) flg |= PM_ALLOW_GROUP;

				if (summon_specific((pet ? -1 : 0), p_ptr->y, p_ptr->x, (plev * 3) / 2, SUMMON_ANGEL, flg))
				{
					if (pet)
					{
						msg_print(_("�֤��ѤǤ������ޤ�����������͡�", "'What is thy bidding... Master?'"));
					}
					else
					{
						msg_print(_("�ֲ����β��ͤˤ��餺�� ���ԼԤ衢��������衪��", "Mortal! Repent of thy impiousness."));
					}
				}
			}
		}
		break;

	case 24:
		if (name) return _("�ε�����", "Heroism");
		if (desc) return _("������֡��ҡ�����ʬ�ˤʤ롣", "Removes fear, and gives bonus to hit and 10 more HP for a while.");
    
		{
			int base = 25;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_hero(randint1(base) + base, FALSE);
				hp_player(10);
				set_afraid(0);
			}
		}
		break;

	case 25:
		if (name) return _("�����໶", "Dispel Curse");
		if (desc) return _("�����ƥ�ˤ����ä����Ϥʼ����������롣", "Removes normal and heavy curse from equipped items.");
    
		{
			if (cast)
			{
				if (remove_all_curse())
				{
					msg_print(_("ï���˸�����Ƥ���褦�ʵ������롣", "You feel as if someone is watching over you."));
				}
			}
		}
		break;

	case 26:
		if (name) return _("�ٰ�����", "Banish Evil");
		if (desc) return _("�볦������Ƥμٰ��ʥ�󥹥�����ƥ�ݡ��Ȥ����롣�񹳤�����̵����", 
			"Teleports all evil monsters in sight away unless resisted.");
    
		{
			int power = 100;

			if (info) return info_power(power);

			if (cast)
			{
				if (banish_evil(power))
				{
					msg_print(_("�������Ϥ��ٰ����Ǥ�ʧ�ä���", "The holy power banishes evil!"));
				}
			}
		}
		break;

	case 27:
		if (name) return _("�ϥ�ޥ��ɥ�", "Armageddon");
		if (desc) return _("���դΥ����ƥࡢ��󥹥������Ϸ����˲����롣", "Destroy everything in nearby area.");
    
		{
			int base = 12;
			int sides = 4;

			if (cast)
			{
				destroy_area(p_ptr->y, p_ptr->x, base + randint1(sides), FALSE);
			}
		}
		break;

	case 28:
		if (name) return _("�ܤˤ��ܤ�", "An Eye for an Eye");
		if (desc) return _("������֡���ʬ�����᡼����������Ȥ��˹����Ԥä���󥹥������Ф���Ʊ���Υ��᡼����Ϳ���롣", 
			"Gives special aura for a while. When you are attacked by a monster, the monster are injured with same amount of damage as you take.");
    
		{
			int base = 10;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_tim_eyeeye(randint1(base) + base, FALSE);
			}
		}
		break;

	case 29:
		if (name) return _("�����ܤ�", "Wrath of the God");
		if (desc) return _("�������åȤμ��Ϥ�ʬ��ε��¿����Ȥ���", "Drops many balls of disintegration near the target.");
    
		{
			HIT_POINT dam = plev * 3 + 25;
			int rad = 2;

			if (info) return info_multi_damage(dam);

			if (cast)
			{
				if (!cast_wrath_of_the_god(dam, rad)) return NULL;
			}
		}
		break;

	case 30:
		if (name) return _("����", "Divine Intervention");
		if (desc) return _("���ܤ����󥹥��������ʤ���᡼����Ϳ�����볦��Υ�󥹥����˥��᡼������®��ۯ۰�����𡢶��ݡ�̲���Ϳ���롣��������Ϥ�������롣", 
			"Damages all adjacent monsters with holy power. Damages and attempt to slow, stun, confuse, scare and freeze all monsters in sight. And heals HP.");
    
		{
			int b_dam = plev * 11;
			int d_dam = plev * 4;
			int heal = 100;
			int power = plev * 4;

			if (info) return format(_("��%d/»%d+%d", "h%d/dm%d+%d"), heal, d_dam, b_dam/2);
			if (cast)
			{
				project(0, 1, p_ptr->y, p_ptr->x, b_dam, GF_HOLY_FIRE, PROJECT_KILL, -1);
				dispel_monsters(d_dam);
				slow_monsters(plev);
				stun_monsters(power);
				confuse_monsters(power);
				turn_monsters(power);
				stasis_monsters(power);
				hp_player(heal);
			}
		}
		break;

	case 31:
		if (name) return _("����", "Crusade");
		if (desc) return _("�볦������ɤʥ�󥹥�����ڥåȤˤ��褦�Ȥ����ʤ�ʤ��ä����ڤ����ɤǤʤ���󥹥������ݤ����롣�����¿���β�®���줿���Τ򾤴������ҡ�������ʡ����®���мٰ��볦�����롣", 
			"Attempts to charm all good monsters in sight, and scare all non-charmed monsters, and summons great number of knights, and gives heroism, bless, speed and protection from evil.");
    
		{
			if (cast)
			{
				int base = 25;
				int sp_sides = 20 + plev;
				int sp_base = plev;

				int i;
				crusade();
				for (i = 0; i < 12; i++)
				{
					int attempt = 10;
					POSITION my = 0, mx = 0;

					while (attempt--)
					{
						scatter(&my, &mx, p_ptr->y, p_ptr->x, 4, 0);

						/* Require empty grids */
						if (cave_empty_bold2(my, mx)) break;
					}
					if (attempt < 0) continue;
					summon_specific(-1, my, mx, plev, SUMMON_KNIGHTS, (PM_ALLOW_GROUP | PM_FORCE_PET | PM_HASTE));
				}
				set_hero(randint1(base) + base, FALSE);
				set_blessed(randint1(base) + base, FALSE);
				set_fast(randint1(sp_sides) + sp_base, FALSE);
				set_protevil(randint1(base) + base, FALSE);
				set_afraid(0);
			}
		}
		break;
	}

	return "";
}


/*!
 * @brief �Τγƽ�����Ԥ�
 * @param spell ��ID
 * @param mode �������� (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST / SPELL_FAIL / SPELL_CONT / SPELL_STOP)
 * @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ˤ�ʸ����ݥ��󥿤��֤���SPELL_CAST / SPELL_FAIL / SPELL_CONT / SPELL_STOP ����NULLʸ������֤���
 */
static cptr do_music_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;
	bool fail = (mode == SPELL_FAIL) ? TRUE : FALSE;
	bool cont = (mode == SPELL_CONT) ? TRUE : FALSE;
	bool stop = (mode == SPELL_STOP) ? TRUE : FALSE;
	static const char s_dam[] = _("»��:", "dam ");

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("���ߤβ�", "Song of Holding");
		if (desc) return _("�볦������ƤΥ�󥹥�����®�����롣�񹳤�����̵����", "Attempts to slow all monsters in sight.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("��ä���Ȥ������ǥ���������߻Ϥ᤿������", "You start humming a slow, steady melody..."));
			start_singing(spell, MUSIC_SLOW);
		}

		{
			int power = plev;

			if (info) return info_power(power);

			if (cont)
			{
				slow_monsters(plev);
			}
		}
		break;

	case 1:
		if (name) return _("��ʡ�β�", "Song of Blessing");
		if (desc) return _("̿��Ψ��AC�Υܡ��ʥ������롣", "Gives bonus to hit and AC for a few turns.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�����ʥ��ǥ����դǻϤ᤿������", "The holy power of the Music of the Ainur enters you..."));
			start_singing(spell, MUSIC_BLESS);
		}

		if (stop)
		{
			if (!p_ptr->blessed)
			{
				msg_print(_("���ʵ�ʬ���ä���������", "The prayer has expired."));
			}
		}

		break;

	case 2:
		if (name) return _("�����β���", "Wrecking Note");
		if (desc) return _("�첻�Υܥ�Ȥ����ġ�", "Fires a bolt of sound.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		{
			int dice = 4 + (plev - 1) / 5;
			int sides = 4;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt(GF_SOUND, dir, damroll(dice, sides));
			}
		}
		break;

	case 3:
		if (name) return _("ۯ۰����Χ", "Stun Pattern");
		if (desc) return _("�볦������ƤΥ�󥹥�����ۯ۰�����롣�񹳤�����̵����", "Attempts to stun all monsters in sight.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("���Ǥ�������ǥ����դǻϤ᤿������", "You weave a pattern of sounds to bewilder and daze..."));
			start_singing(spell, MUSIC_STUN);
		}

		{
			int dice = plev / 10;
			int sides = 2;

			if (info) return info_power_dice(dice, sides);

			if (cont)
			{
				stun_monsters(damroll(dice, sides));
			}
		}

		break;

	case 4:
		if (name) return _("��̿��ή��", "Flow of Life");
		if (desc) return _("���Ϥ򾯤����������롣", "Heals HP a little.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�Τ��̤����Τ˳赤����äƤ���������", "Life flows through you as you sing a song of healing..."));
			start_singing(spell, MUSIC_L_LIFE);
		}

		{
			int dice = 2;
			int sides = 6;

			if (info) return info_heal(dice, sides, 0);

			if (cont)
			{
				hp_player(damroll(dice, sides));
			}
		}

		break;

	case 5:
		if (name) return _("���ۤβ�", "Song of the Sun");
		if (desc) return _("�������Ȥ餷�Ƥ����ϰϤ��������Τ�ʵפ����뤯���롣", "Lights up nearby area and the inside of a room permanently.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		{
			int dice = 2;
			int sides = plev / 2;
			int rad = plev / 10 + 1;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				msg_print(_("���굱���Τ��դ��Ȥ餷����", "Your uplifting song brings brightness to dark places..."));
				lite_area(damroll(dice, sides), rad);
			}
		}
		break;

	case 6:
		if (name) return _("���ݤβ�", "Song of Fear");
		if (desc) return _("�볦������ƤΥ�󥹥������ݤ����롣�񹳤�����̵����", "Attempts to scare all monsters in sight.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("���ɤ��ɤ������ǥ����դǻϤ᤿������", "You start weaving a fearful pattern..."));
			start_singing(spell, MUSIC_FEAR);			
		}

		{
			int power = plev;

			if (info) return info_power(power);

			if (cont)
			{
				project_hack(GF_TURN_ALL, power);
			}
		}

		break;

	case 7:
		if (name) return _("�襤�β�", "Heroic Ballad");
		if (desc) return _("�ҡ�����ʬ�ˤʤ롣", "Removes fear, and gives bonus to hit and 10 more HP for a while.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�㤷���襤�βΤ�Τä�������", "You start singing a song of intense fighting..."));

			(void)hp_player(10);
			(void)set_afraid(0);

			/* Recalculate hitpoints */
			p_ptr->update |= (PU_HP);

			start_singing(spell, MUSIC_HERO);
		}

		if (stop)
		{
			if (!p_ptr->hero)
			{
				msg_print(_("�ҡ����ε�ʬ���ä���������", "The heroism wears off."));
				/* Recalculate hitpoints */
				p_ptr->update |= (PU_HP);
			}
		}

		break;

	case 8:
		if (name) return _("��Ū�γ�", "Clairaudience");
		if (desc) return _("�᤯���/��/���ʤ��Τ��롣��٥�15�����ƤΥ�󥹥�����20�Ǻ����ȥ����ƥ���ΤǤ���褦�ˤʤ롣��٥�25�Ǽ��դ��Ϸ����Τ���40�Ǥ��γ����Τ�ʵפ˾Ȥ餷�����󥸥����Τ��٤ƤΥ����ƥ���Τ��롣���θ��̤ϲΤ�³���뤳�Ȥǽ�˵����롣", 
			"Detects traps, doors and stairs in your vicinity. And detects all monsters at level 15, treasures and items at level 20. Maps nearby area at level 25. Lights and know the whole level at level 40. These effects occurs by turns while this song continues.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�Ť��ʲ��ڤ����Ф򸦤����ޤ�����������", "Your quiet music sharpens your sense of hearing..."));
			/* Hack -- Initialize the turn count */
			SINGING_COUNT(p_ptr) = 0;
			start_singing(spell, MUSIC_DETECT);
		}

		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cont)
			{
				int count = SINGING_COUNT(p_ptr);

				if (count >= 19) wiz_lite(FALSE);
				if (count >= 11)
				{
					map_area(rad);
					if (plev > 39 && count < 19)
						SINGING_COUNT(p_ptr) = count + 1;
				}
				if (count >= 6)
				{
					/* There are too many hidden treasure.  So... */
					/* detect_treasure(rad); */
					detect_objects_gold(rad);
					detect_objects_normal(rad);

					if (plev > 24 && count < 11)
						SINGING_COUNT(p_ptr) = count + 1;
				}
				if (count >= 3)
				{
					detect_monsters_invis(rad);
					detect_monsters_normal(rad);

					if (plev > 19 && count < 6)
						SINGING_COUNT(p_ptr) = count + 1;
				}
				detect_traps(rad, TRUE);
				detect_doors(rad);
				detect_stairs(rad);

				if (plev > 14 && count < 3)
					SINGING_COUNT(p_ptr) = count + 1;
			}
		}

		break;

	case 9:
		if (name) return _("���β�", "Soul Shriek");
		if (desc) return _("�볦������ƤΥ�󥹥������Ф������������Ԥ���", "Damages all monsters in sight with PSI damages.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("������Ǳ���ʤ���Τ�Τä�������", "You start singing a song of soul in pain..."));
			start_singing(spell, MUSIC_PSI);
		}

		{
			int dice = 1;
			int sides = plev * 3 / 2;

			if (info) return info_damage(dice, sides, 0);

			if (cont)
			{
				project_hack(GF_PSI, damroll(dice, sides));
			}
		}

		break;

	case 10:
		if (name) return _("�μ��β�", "Song of Lore");
		if (desc) return _("��ʬ�Τ���ޥ����٤�Υޥ�������Ƥ��륢���ƥ����ꤹ�롣", "Identifies all items which are in the adjacent squares.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�����������μ���ή�����Ǥ���������", "You recall the rich lore of the world..."));
			start_singing(spell, MUSIC_ID);
		}

		{
			int rad = 1;

			if (info) return info_radius(rad);

			/*
			 * �Τγ��ϻ��ˤ����ȯư��
			 * MP��­�Ǵ��꤬ȯư��������˲Τ����Ǥ��Ƥ��ޤ��Τ��ɻߡ�
			 */
			if (cont || cast)
			{
				project(0, rad, p_ptr->y, p_ptr->x, 0, GF_IDENTIFY, PROJECT_ITEM, -1);
			}
		}

		break;

	case 11:
		if (name) return _("���ۤβ�", "Hiding Tune");
		if (desc) return _("��̩��ưǽ�Ϥ�徺�����롣", "Gives improved stealth.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("���ʤ��λѤ��ʿ��ˤȤ�����Ǥ��ä�������", "Your song carries you beyond the sight of mortal eyes..."));
			start_singing(spell, MUSIC_STEALTH);
		}

		if (stop)
		{
			if (!p_ptr->tim_stealth)
			{
				msg_print(_("�Ѥ��Ϥä���ȸ�����褦�ˤʤä���", "You are no longer hided."));
			}
		}

		break;

	case 12:
		if (name) return _("���Ƥ���Χ", "Illusion Pattern");
		if (desc) return _("�볦������ƤΥ�󥹥������𤵤��롣�񹳤�����̵����", "Attempts to confuse all monsters in sight.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�դ���̤˸��Ƥ����줿������", "You weave a pattern of sounds to beguile and confuse..."));
			start_singing(spell, MUSIC_CONF);
		}

		{
			int power = plev * 2;

			if (info) return info_power(power);

			if (cont)
			{
				confuse_monsters(power);
			}
		}

		break;

	case 13:
		if (name) return _("���Ǥζ���", "Doomcall");
		if (desc) return _("�볦������ƤΥ�󥹥������Ф��ƹ첻�����Ԥ���", "Damages all monsters in sight with booming sound.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�첻��������������", "The fury of the Downfall of Numenor lashes out..."));
			start_singing(spell, MUSIC_SOUND);
		}

		{
			int dice = 10 + plev / 5;
			int sides = 7;

			if (info) return info_damage(dice, sides, 0);

			if (cont)
			{
				project_hack(GF_SOUND, damroll(dice, sides));
			}
		}

		break;

	case 14:
		if (name) return _("�ե��ꥨ��β�", "Firiel's Song");
		if (desc) return _("���Ϥλ��Τ���������֤���", "Resurrects nearby corpse and skeletons. And makes these your pets.");
    
		{
			/* Stop singing before start another */
			if (cast || fail) stop_singing();

			if (cast)
			{
				msg_print(_("��̿������Υơ��ޤ��դǻϤ᤿������", "The themes of life and revival are woven into your song..."));
				animate_dead(0, p_ptr->y, p_ptr->x);
			}
		}
		break;

	case 15:
		if (name) return _("ι�����", "Fellowship Chant");
		if (desc) return _("�볦������ƤΥ�󥹥�����̥λ���롣�񹳤�����̵����", "Attempts to charm all monsters in sight.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�¤餫�ʥ��ǥ����դǻϤ᤿������", "You weave a slow, soothing melody of imploration..."));
			start_singing(spell, MUSIC_CHARM);
		}

		{
			int dice = 10 + plev / 15;
			int sides = 6;

			if (info) return info_power_dice(dice, sides);

			if (cont)
			{
				charm_monsters(damroll(dice, sides));
			}
		}

		break;

	case 16:
		if (name) return _("ʬ����", "Sound of disintegration");
		if (desc) return _("�ɤ򷡤�ʤࡣ��ʬ��­���Υ����ƥ�Ͼ�ȯ���롣", "Makes you be able to burrow into walls. Objects under your feet evaporate.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("ʴ�դ�����ǥ����դǻϤ᤿������", "You weave a violent pattern of sounds to break wall."));
			start_singing(spell, MUSIC_WALL);
		}

		{
			/*
			 * �Τγ��ϻ��ˤ����ȯư��
			 * MP��­�Ǹ��̤�ȯư��������˲Τ����Ǥ��Ƥ��ޤ��Τ��ɻߡ�
			 */
			if (cont || cast)
			{
				project(0, 0, p_ptr->y, p_ptr->x,
					0, GF_DISINTEGRATE, PROJECT_KILL | PROJECT_ITEM | PROJECT_HIDE, -1);
			}
		}
		break;

	case 17:
		if (name) return _("��������", "Finrod's Resistance");
		if (desc) return _("�����ŷ⡢�ꡢ�䵤���Ǥ��Ф������������롣�����ˤ�����������Ѥ��롣", 
			"Gives resistance to fire, cold, electricity, acid and poison. These resistances can be added to which from equipment for more powerful resistances.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("���Ǥ��Ϥ��Ф���Ǧ�ѤβΤ�Τä���", "You sing a song of perseverance against powers..."));
			start_singing(spell, MUSIC_RESIST);
		}

		if (stop)
		{
			if (!p_ptr->oppose_acid)
			{
				msg_print(_("���ؤ����������줿�������롣", "You feel less resistant to acid."));
			}

			if (!p_ptr->oppose_elec)
			{
				msg_print(_("�ŷ�ؤ����������줿�������롣", "You feel less resistant to elec."));
			}

			if (!p_ptr->oppose_fire)
			{
				msg_print(_("�Фؤ����������줿�������롣", "You feel less resistant to fire."));
			}

			if (!p_ptr->oppose_cold)
			{
				msg_print(_("�䵤�ؤ����������줿�������롣", "You feel less resistant to cold."));
			}

			if (!p_ptr->oppose_pois)
			{
				msg_print(_("�Ǥؤ����������줿�������롣", "You feel less resistant to pois."));
			}
		}

		break;

	case 18:
		if (name) return _("�ۥӥåȤΥ��ǥ�", "Hobbit Melodies");
		if (desc) return _("��®���롣", "Hastes you.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�ڲ��ʲΤ�������߻Ϥ᤿������", "You start singing joyful pop song..."));
			start_singing(spell, MUSIC_SPEED);
		}

		if (stop)
		{
			if (!p_ptr->fast)
			{
				msg_print(_("ư�������ᤵ���ʤ��ʤä��褦����", "You feel yourself slow down."));
			}
		}

		break;

	case 19:
		if (name) return _("�Ĥ������", "World Contortion");
		if (desc) return _("�᤯�Υ�󥹥�����ƥ�ݡ��Ȥ����롣�񹳤�����̵����", "Teleports all nearby monsters away unless resisted.");
    
		{
			int rad = plev / 15 + 1;
			int power = plev * 3 + 1;

			if (info) return info_radius(rad);

			/* Stop singing before start another */
			if (cast || fail) stop_singing();

			if (cast)
			{
				msg_print(_("�Τ����֤��Ĥ᤿������", "Reality whirls wildly as you sing a dizzying melody..."));
				project(0, rad, p_ptr->y, p_ptr->x, power, GF_AWAY_ALL, PROJECT_KILL, -1);
			}
		}
		break;

	case 20:
		if (name) return _("�໶�β�", "Dispelling chant");
		if (desc) return _("�볦������ƤΥ�󥹥����˥��᡼����Ϳ���롣�ٰ��ʥ�󥹥������ä��礭�ʥ��᡼����Ϳ���롣", 
			"Damages all monsters in sight. Hurts evil monsters greatly.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�Ѥ����ʤ��Զ��²���Ũ���դ�Ω�Ƥ�������", "You cry out in an ear-wracking voice..."));
			start_singing(spell, MUSIC_DISPEL);
		}

		{
			int m_sides = plev * 3;
			int e_sides = plev * 3;

			if (info) return format("%s1d%d+1d%d", s_dam, m_sides, e_sides);

			if (cont)
			{
				dispel_monsters(randint1(m_sides));
				dispel_evil(randint1(e_sides));
			}
		}
		break;

	case 21:
		if (name) return _("����ޥ�δŸ�", "The Voice of Saruman");
		if (desc) return _("�볦������ƤΥ�󥹥�����®������̲�餻�褦�Ȥ��롣�񹳤�����̵����", "Attempts to slow and sleep all monsters in sight.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("ͥ������̥��Ū�ʲΤ�������߻Ϥ᤿������", "You start humming a gentle and attractive song..."));
			start_singing(spell, MUSIC_SARUMAN);
		}

		{
			int power = plev;

			if (info) return info_power(power);

			if (cont)
			{
				slow_monsters(plev);
				sleep_monsters(plev);
			}
		}

		break;

	case 22:
		if (name) return _("��β���", "Song of the Tempest");
		if (desc) return _("�첻�Υӡ�������ġ�", "Fires a beam of sound.");
    
		{
			int dice = 15 + (plev - 1) / 2;
			int sides = 10;

			if (info) return info_damage(dice, sides, 0);

			/* Stop singing before start another */
			if (cast || fail) stop_singing();

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_beam(GF_SOUND, dir, damroll(dice, sides));
			}
		}
		break;

	case 23:
		if (name) return _("�⤦��Ĥ�����", "Ambarkanta");
		if (desc) return _("���ߤγ���ƹ������롣", "Recreates current dungeon level.");
    
		{
			int base = 15;
			int sides = 20;

			if (info) return info_delay(base, sides);

			/* Stop singing before start another */
			if (cast || fail) stop_singing();

			if (cast)
			{
				msg_print(_("���Ϥ��Ѳ����Ϥ᤿������", "You sing of the primeval shaping of Middle-earth..."));
				alter_reality();
			}
		}
		break;

	case 24:
		if (name) return _("�˲�����Χ", "Wrecking Pattern");
		if (desc) return _("���ϤΥ��󥸥����ɤ餷���ɤȾ��������������Ѥ��롣", 
			"Shakes dungeon structure, and results in random swapping of floors and walls.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�˲�Ū�ʲΤ������錄�ä�������", "You weave a pattern of sounds to contort and shatter..."));
			start_singing(spell, MUSIC_QUAKE);
		}

		{
			int rad = 10;

			if (info) return info_radius(rad);

			if (cont)
			{
				earthquake(p_ptr->y, p_ptr->x, 10);
			}
		}

		break;


	case 25:
		if (name) return _("���ڤβ�", "Stationary Shriek");
		if (desc) return _("�볦������ƤΥ�󥹥��������㤵���褦�Ȥ��롣�񹳤�����̵����", "Attempts to freeze all monsters in sight.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("��ä���Ȥ������ǥ����դǻϤ᤿������", "You weave a very slow pattern which is almost likely to stop..."));
			start_singing(spell, MUSIC_STASIS);
		}

		{
			int power = plev * 4;

			if (info) return info_power(power);

			if (cont)
			{
				stasis_monsters(power);
			}
		}

		break;

	case 26:
		if (name) return _("���β�", "Endurance");
		if (desc) return _("��ʬ�Τ��뾲�ξ�ˡ���󥹥������̤�ȴ�����꾤�����줿�ꤹ�뤳�Ȥ��Ǥ��ʤ��ʤ�롼���������", 
			"Sets a glyph on the floor beneath you. Monsters cannot attack you if you are on a glyph, but can try to break glyph.");
    
		{
			/* Stop singing before start another */
			if (cast || fail) stop_singing();

			if (cast)
			{
				msg_print(_("�Τ������ʾ����Ф���������", "The holy power of the Music is creating sacred field..."));
				warding_glyph();
			}
		}
		break;

	case 27:
		if (name) return _("��ͺ�λ�", "The Hero's Poem");
		if (desc) return _("��®�����ҡ�����ʬ�ˤʤꡢ�볦������ƤΥ�󥹥����˥��᡼����Ϳ���롣", 
			"Hastes you. Gives heroism. Damages all monsters in sight.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("��ͺ�βΤ���������������", "You chant a powerful, heroic call to arms..."));
			(void)hp_player(10);
			(void)set_afraid(0);

			/* Recalculate hitpoints */
			p_ptr->update |= (PU_HP);

			start_singing(spell, MUSIC_SHERO);
		}

		if (stop)
		{
			if (!p_ptr->hero)
			{
				msg_print(_("�ҡ����ε�ʬ���ä���������", "The heroism wears off."));
				/* Recalculate hitpoints */
				p_ptr->update |= (PU_HP);
			}

			if (!p_ptr->fast)
			{
				msg_print(_("ư�������ᤵ���ʤ��ʤä��褦����", "You feel yourself slow down."));
			}
		}

		{
			int dice = 1;
			int sides = plev * 3;

			if (info) return info_damage(dice, sides, 0);

			if (cont)
			{
				dispel_monsters(damroll(dice, sides));
			}
		}
		break;

	case 28:
		if (name) return _("�������ʤν���", "Relief of Yavanna");
		if (desc) return _("���Ϥʲ����βΤǡ������ۯ۰���֤��������롣", "Powerful healing song. Also heals cut and stun completely.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�Τ��̤����Τ˳赤����äƤ���������", "Life flows through you as you sing the song..."));
			start_singing(spell, MUSIC_H_LIFE);
		}

		{
			int dice = 15;
			int sides = 10;

			if (info) return info_heal(dice, sides, 0);

			if (cont)
			{
				hp_player(damroll(dice, sides));
				set_stun(0);
				set_cut(0);
			}
		}

		break;

	case 29:
		if (name) return _("�����β�", "Goddess' rebirth");
		if (desc) return _("���٤ƤΥ��ơ������ȷи��ͤ�������롣", "Restores all stats and experience.");
    
		{
			/* Stop singing before start another */
			if (cast || fail) stop_singing();

			if (cast)
			{
				msg_print(_("�Ź�����˸�������դ�ޤ������Τ����γ��Ϥ����ᤷ����",
							"You strewed light and beauty in the dark as you sing. You feel refreshed."));
				(void)do_res_stat(A_STR);
				(void)do_res_stat(A_INT);
				(void)do_res_stat(A_WIS);
				(void)do_res_stat(A_DEX);
				(void)do_res_stat(A_CON);
				(void)do_res_stat(A_CHR);
				(void)restore_level();
			}
		}
		break;

	case 30:
		if (name) return _("�����������", "Wizardry of Sauron");
		if (desc) return _("���˶��ϤǤ����������첻�ε�����ġ�", "Fires an extremely powerful tiny ball of sound.");
    
		{
			int dice = 50 + plev;
			int sides = 10;
			int rad = 0;

			if (info) return info_damage(dice, sides, 0);

			/* Stop singing before start another */
			if (cast || fail) stop_singing();

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_SOUND, dir, damroll(dice, sides), rad);
			}
		}
		break;

	case 31:
		if (name) return _("�ե��󥴥�ե����ĩ��", "Fingolfin's Challenge");
		if (desc) return _("���᡼��������ʤ��ʤ�Хꥢ��ĥ�롣", 
			"Generates barrier which completely protect you from almost all damages. Takes a few your turns when the barrier breaks.");
    
		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�ե��󥴥�ե����̽���ؤ�ĩ���Τä�������",
						"You recall the valor of Fingolfin's challenge to the Dark Lord..."));

			/* Redraw map */
			p_ptr->redraw |= (PR_MAP);
		
			/* Update monsters */
			p_ptr->update |= (PU_MONSTERS);
		
			/* Window stuff */
			p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);

			start_singing(spell, MUSIC_INVULN);
		}

		if (stop)
		{
			if (!p_ptr->invuln)
			{
				msg_print(_("̵Ũ�ǤϤʤ��ʤä���", "The invulnerability wears off."));
				/* Redraw map */
				p_ptr->redraw |= (PR_MAP);

				/* Update monsters */
				p_ptr->update |= (PU_MONSTERS);

				/* Window stuff */
				p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);
			}
		}

		break;
	}

	return "";
}

/*!
 * @brief ���Ѥγƽ�����Ԥ�
 * @param spell ����ID
 * @param mode �������� (SPELL_NAME / SPELL_DESC / SPELL_CAST)
 * @return SPELL_NAME / SPELL_DESC ���ˤ�ʸ����ݥ��󥿤��֤���SPELL_CAST����NULLʸ������֤���
 */
static cptr do_hissatsu_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("���ӹ�", "Tobi-Izuna");
		if (desc) return _("2�ޥ�Υ�줿�Ȥ���ˤ����󥹥����򹶷⤹�롣", "Attacks a two squares distant monster.");
    
		if (cast)
		{
			project_length = 2;
			if (!get_aim_dir(&dir)) return NULL;

			project_hook(GF_ATTACK, dir, HISSATSU_2, PROJECT_STOP | PROJECT_KILL);
		}
		break;

	case 1:
		if (name) return _("�޷�¤�", "3-Way Attack");
		if (desc) return _("3�������Ф��ƹ��⤹�롣", "Attacks in 3 directions in one time.");
    
		if (cast)
		{
			int cdir;
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			for (cdir = 0;cdir < 8; cdir++)
			{
				if (cdd[cdir] == dir) break;
			}

			if (cdir == 8) return NULL;

			y = p_ptr->y + ddy_cdd[cdir];
			x = p_ptr->x + ddx_cdd[cdir];
			if (cave[y][x].m_idx)
				py_attack(y, x, 0);
			else
				msg_print(_("����϶����ڤä���", "You attack the empty air."));
			
			y = p_ptr->y + ddy_cdd[(cdir + 7) % 8];
			x = p_ptr->x + ddx_cdd[(cdir + 7) % 8];
			if (cave[y][x].m_idx)
				py_attack(y, x, 0);
			else
				msg_print(_("����϶����ڤä���", "You attack the empty air."));
			
			y = p_ptr->y + ddy_cdd[(cdir + 1) % 8];
			x = p_ptr->x + ddx_cdd[(cdir + 1) % 8];
			if (cave[y][x].m_idx)
				py_attack(y, x, 0);
			else
				msg_print(_("����϶����ڤä���", "You attack the empty air."));
		}
		break;

	case 2:
		if (name) return _("�֡�����", "Boomerang");
		if (desc) return _("����긵����äƤ���褦���ꤲ�롣��äƤ��ʤ����Ȥ⤢�롣", 
			"Throws current weapon. And it'll return to your hand unless failed.");
    
		if (cast)
		{
			if (!do_cmd_throw_aux(1, TRUE, -1)) return NULL;
		}
		break;

	case 3:
		if (name) return _("����", "Burning Strike");
		if (desc) return _("�б������Τʤ���󥹥���������᡼����Ϳ���롣", "Attacks a monster with more damage unless it has resistance to fire.");
    
		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_FIRE);
			else
			{
				msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 4:
		if (name) return _("��������", "Detect Ferocity");
		if (desc) return _("�᤯�λ׹ͤ��뤳�Ȥ��Ǥ����󥹥������Τ��롣", "Detects all monsters except mindless in your vicinity.");
    
		if (cast)
		{
			detect_monsters_mind(DETECT_RAD_DEFAULT);
		}
		break;

	case 5:
		if (name) return _("�ߤ��Ǥ�", "Strike to Stun");
		if (desc) return _("���˥��᡼����Ϳ���ʤ�����ۯ۰�Ȥ����롣", "Attempts to stun a monster in the adjacent.");
    
		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_MINEUCHI);
			else
			{
				msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 6:
		if (name) return _("�����󥿡�", "Counter");
		if (desc) return _("���˹��⤵�줿�Ȥ���ȿ�⤹�롣ȿ�⤹�뤿�Ӥ�MP�����", 
			"Prepares to counterattack. When attack by a monster, strikes back using SP each time.");
    
		if (cast)
		{
			if (p_ptr->riding)
			{
				msg_print(_("������ˤ�̵������", "You cannot do it when riding."));
				return NULL;
			}
			msg_print(_("���ι�����Ф��ƿȹ�������", "You prepare to counter blow."));
			p_ptr->counter = TRUE;
		}
		break;

	case 7:
		if (name) return _("ʧ��ȴ��", "Harainuke");
		if (desc) return _("���⤷���塢ȿ��¦��ȴ���롣", 
			"Attacks monster with your weapons normally, then move through counter side of the monster.");
    
		if (cast)
		{
			POSITION y, x;

			if (p_ptr->riding)
			{
				msg_print(_("������ˤ�̵������", "You cannot do it when riding."));
				return NULL;
			}
	
			if (!get_rep_dir2(&dir)) return NULL;
	
			if (dir == 5) return NULL;
			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];
	
			if (!cave[y][x].m_idx)
			{
				msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "There is no monster."));
				return NULL;
			}
	
			py_attack(y, x, 0);
	
			if (!player_can_enter(cave[y][x].feat, 0) || is_trap(cave[y][x].feat))
				break;
	
			y += ddy[dir];
			x += ddx[dir];
	
			if (player_can_enter(cave[y][x].feat, 0) && !is_trap(cave[y][x].feat) && !cave[y][x].m_idx)
			{
				msg_print(NULL);
	
				/* Move the player */
				(void)move_player_effect(y, x, MPE_FORGET_FLOW | MPE_HANDLE_STUFF | MPE_DONT_PICKUP);
			}
		}
		break;

	case 8:
		if (name) return _("�����ڥ�ĥ���", "Serpent's Tongue");
		if (desc) return _("�������Τʤ���󥹥���������᡼����Ϳ���롣", "Attacks a monster with more damage unless it has resistance to poison.");
    
		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_POISON);
			else
			{
				msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 9:
		if (name) return _("�������������", "Zammaken");
		if (desc) return _("��̿�Τʤ��ٰ��ʥ�󥹥���������᡼����Ϳ���뤬��¾�Υ�󥹥����ˤ��������̤��ʤ���", 
			"Attacks an evil unliving monster with great damage. No effect to other  monsters.");
    
		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_ZANMA);
			else
			{
				msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 10:
		if (name) return _("������", "Wind Blast");
		if (desc) return _("���⤷����������ؿ᤭���Ф���", "Attacks an adjacent monster, and blow it away.");
    
		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, 0);
			else
			{
				msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "There is no monster."));
				return NULL;
			}
			if (d_info[dungeon_type].flags1 & DF1_NO_MELEE)
			{
				return "";
			}
			if (cave[y][x].m_idx)
			{
				int i;
				POSITION ty = y, tx = x;
				POSITION oy = y, ox = x;
				MONSTER_IDX m_idx = cave[y][x].m_idx;
				monster_type *m_ptr = &m_list[m_idx];
				char m_name[80];
	
				monster_desc(m_name, m_ptr, 0);
	
				for (i = 0; i < 5; i++)
				{
					y += ddy[dir];
					x += ddx[dir];
					if (cave_empty_bold(y, x))
					{
						ty = y;
						tx = x;
					}
					else break;
				}
				if ((ty != oy) || (tx != ox))
				{
					msg_format(_("%s��᤭���Ф�����", "You blow %s away!"), m_name);
					cave[oy][ox].m_idx = 0;
					cave[ty][tx].m_idx = m_idx;
					m_ptr->fy = ty;
					m_ptr->fx = tx;
	
					update_mon(m_idx, TRUE);
					lite_spot(oy, ox);
					lite_spot(ty, tx);
	
					if (r_info[m_ptr->r_idx].flags7 & (RF7_LITE_MASK | RF7_DARK_MASK))
						p_ptr->update |= (PU_MON_LITE);
				}
			}
		}
		break;

	case 11:
		if (name) return _("�ᾢ��������", "Judge");
		if (desc) return _("���ɶ��1�ļ��̤��롣��٥�45�ʾ�����ɶ��ǽ�Ϥ������Τ뤳�Ȥ��Ǥ��롣", 
			"Identifies a weapon or armor. Or *identifies* these at level 45.");
    
		if (cast)
		{
			if (plev > 44)
			{
				if (!identify_fully(TRUE)) return NULL;
			}
			else
			{
				if (!ident_spell(TRUE)) return NULL;
			}
		}
		break;

	case 12:
		if (name) return _("�˴��", "Rock Smash");
		if (desc) return _("�����������зϤΥ�󥹥���������᡼����Ϳ���롣", "Breaks rock. Or greatly damage a monster made by rocks.");
    
		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_HAGAN);
	
			if (!cave_have_flag_bold(y, x, FF_HURT_ROCK)) break;
	
			/* Destroy the feature */
			cave_alter_feat(y, x, FF_HURT_ROCK);
	
			/* Update some things */
			p_ptr->update |= (PU_FLOW);
		}
		break;

	case 13:
		if (name) return _("�������", "Midare-Setsugekka");
		if (desc) return _("���������������䵤�����Τʤ���󥹥���������᡼����Ϳ���롣", 
			"Attacks a monster with increased number of attacks and more damage unless it has resistance to cold.");
    
		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_COLD);
			else
			{
				msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 14:
		if (name) return _("�޽��ͤ�", "Spot Aiming");
		if (desc) return _("��󥹥���������ݤ�����򷫤�Ф������Ԥ����1���������᡼����Ϳ�����ʤ���", 
			"Attempts to kill a monster instantly. If failed cause only 1HP of damage.");
    
		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_KYUSHO);
			else
			{
				msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 15:
		if (name) return _("����¤�", "Majingiri");
		if (desc) return _("�񿴤ΰ��ǹ��⤹�롣���⤬���蘆��䤹����", 
			"Attempts to attack with critical hit. But this attack is easy to evade for a monster.");
    
		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_MAJIN);
			else
			{
				msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 16:
		if (name) return _("�Τƿ�", "Desperate Attack");
		if (desc) return _("���Ϥʹ���򷫤�Ф������Υ�����ޤǤδ֡����餦���᡼���������롣", 
			"Attacks with all of your power. But all damages you take will be doubled for one turn.");
    
		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_SUTEMI);
			else
			{
				msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "There is no monster."));
				return NULL;
			}
			p_ptr->sutemi = TRUE;
		}
		break;

	case 17:
		if (name) return _("������޻�", "Lightning Eagle");
		if (desc) return _("�ŷ������Τʤ���󥹥����������礭�����᡼����Ϳ���롣", 
			"Attacks a monster with more damage unless it has resistance to electricity.");
    
		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_ELEC);
			else
			{
				msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 18:
		if (name) return _("����", "Rush Attack");
		if (desc) return _("���᤯���˶��깶�⤹�롣", "Steps close to a monster and attacks at a time.");
    
		if (cast)
		{
			if (!rush_attack(NULL)) return NULL;
		}
		break;

	case 19:
		if (name) return _("��ή��", "Bloody Maelstrom");
		if (desc) return _("��ʬ���Ȥ������Ĥġ����ν��������ۤ��礭�����Ϥ���������Ũ�򹶷�Ǥ��롣�����Ƥ��ʤ���󥹥����ˤϸ��̤��ʤ���", 
			"Attacks all adjacent monsters with power corresponding to your cut status. Then increases your cut status. No effect to unliving monsters.");
    
		if (cast)
		{
			int y = 0, x = 0;

			cave_type       *c_ptr;
			monster_type    *m_ptr;
	
			if (p_ptr->cut < 300)
				set_cut(p_ptr->cut + 300);
			else
				set_cut(p_ptr->cut * 2);
	
			for (dir = 0; dir < 8; dir++)
			{
				y = p_ptr->y + ddy_ddd[dir];
				x = p_ptr->x + ddx_ddd[dir];
				c_ptr = &cave[y][x];
	
				/* Get the monster */
				m_ptr = &m_list[c_ptr->m_idx];
	
				/* Hack -- attack monsters */
				if (c_ptr->m_idx && (m_ptr->ml || cave_have_flag_bold(y, x, FF_PROJECT)))
				{
					if (!monster_living(&r_info[m_ptr->r_idx]))
					{
						char m_name[80];
	
						monster_desc(m_name, m_ptr, 0);
						msg_format(_("%s�ˤϸ��̤��ʤ���", "%s is unharmed!"), m_name);
					}
					else py_attack(y, x, HISSATSU_SEKIRYUKA);
				}
			}
		}
		break;

	case 20:
		if (name) return _("��̷�", "Earthquake Blow");
		if (desc) return _("�Ͽ̤򵯤�����", "Shakes dungeon structure, and results in random swapping of floors and walls.");
    
		if (cast)
		{
			int y,x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_QUAKE);
			else
				earthquake(p_ptr->y, p_ptr->x, 10);
		}
		break;

	case 21:
		if (name) return _("������", "Crack");
		if (desc) return _("�׷��ȤΥӡ�������ġ�", "Fires a beam of shock wave.");
    
		if (cast)
		{
			int total_damage = 0, basedam, i;
			u32b flgs[TR_FLAG_SIZE];
			object_type *o_ptr;
			if (!get_aim_dir(&dir)) return NULL;
			msg_print(_("�����礭�����겼������", "You swing your weapon downward."));
			for (i = 0; i < 2; i++)
			{
				int damage;
	
				if (!buki_motteruka(INVEN_RARM+i)) break;
				o_ptr = &inventory[INVEN_RARM+i];
				basedam = (o_ptr->dd * (o_ptr->ds + 1)) * 50;
				damage = o_ptr->to_d * 100;
				object_flags(o_ptr, flgs);
				if ((o_ptr->name1 == ART_VORPAL_BLADE) || (o_ptr->name1 == ART_CHAINSWORD))
				{
					/* vorpal blade */
					basedam *= 5;
					basedam /= 3;
				}
				else if (have_flag(flgs, TR_VORPAL))
				{
					/* vorpal flag only */
					basedam *= 11;
					basedam /= 9;
				}
				damage += basedam;
				damage *= p_ptr->num_blow[i];
				total_damage += damage / 200;
				if (i) total_damage = total_damage*7/10;
			}
			fire_beam(GF_FORCE, dir, total_damage);
		}
		break;

	case 22:
		if (name) return _("������ͺ����", "War Cry");
		if (desc) return _("�볦�������󥹥������Ф��ƹ첻�ι����Ԥ�������ˡ��᤯�ˤ����󥹥������ܤ餻�롣", 
			"Damages all monsters in sight with sound. Aggravate nearby monsters.");
    
		if (cast)
		{
			msg_print(_("ͺ���Ӥ򤢤�����", "You roar out!"));
			project_hack(GF_SOUND, randint1(plev * 3));
			aggravate_monsters(0);
		}
		break;

	case 23:
		if (name) return _("̵�л���", "Musou-Sandan");
		if (desc) return _("���Ϥ�3�ʹ���򷫤�Ф���", "Attacks with powerful 3 strikes.");
    
		if (cast)
		{
			int i;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			for (i = 0; i < 3; i++)
			{
				POSITION y, x;
				POSITION ny, nx;
				MONSTER_IDX m_idx;
				cave_type *c_ptr;
				monster_type *m_ptr;
	
				y = p_ptr->y + ddy[dir];
				x = p_ptr->x + ddx[dir];
				c_ptr = &cave[y][x];
	
				if (c_ptr->m_idx)
					py_attack(y, x, HISSATSU_3DAN);
				else
				{
					msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "There is no monster."));
					return NULL;
				}
	
				if (d_info[dungeon_type].flags1 & DF1_NO_MELEE)
				{
					return "";
				}
	
				/* Monster is dead? */
				if (!c_ptr->m_idx) break;
	
				ny = y + ddy[dir];
				nx = x + ddx[dir];
				m_idx = c_ptr->m_idx;
				m_ptr = &m_list[m_idx];
	
				/* Monster cannot move back? */
				if (!monster_can_enter(ny, nx, &r_info[m_ptr->r_idx], 0))
				{
					/* -more- */
					if (i < 2) msg_print(NULL);
					continue;
				}
	
				c_ptr->m_idx = 0;
				cave[ny][nx].m_idx = m_idx;
				m_ptr->fy = ny;
				m_ptr->fx = nx;
	
				update_mon(m_idx, TRUE);
	
				/* Redraw the old spot */
				lite_spot(y, x);
	
				/* Redraw the new spot */
				lite_spot(ny, nx);
	
				/* Player can move forward? */
				if (player_can_enter(c_ptr->feat, 0))
				{
					/* Move the player */
					if (!move_player_effect(y, x, MPE_FORGET_FLOW | MPE_HANDLE_STUFF | MPE_DONT_PICKUP)) break;
				}
				else
				{
					break;
				}

				/* -more- */
				if (i < 2) msg_print(NULL);
			}
		}
		break;

	case 24:
		if (name) return _("�۷쵴�β�", "Vampire's Fang");
		if (desc) return _("���⤷���������Ϥ�ۤ��Ȥꡢ��ʬ�����Ϥ���������롣��̿������ʤ���󥹥����ˤ��̤��ʤ���", 
			"Attacks with vampiric strikes which absorbs HP from a monster and gives them to you. No effect to unliving monsters.");
    
		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_DRAIN);
			else
			{
					msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 25:
		if (name) return _("����", "Moon Dazzling");
		if (desc) return _("�볦��ε����Ƥ�������󥹥�����ۯ۰������̲���Ϳ���褦�Ȥ��롣", "Attempts to stun, confuse and sleep all waking monsters.");
    
		if (cast)
		{
			msg_print(_("�����Ե�§���ɤ餷��������", "You irregularly wave your weapon..."));
			project_hack(GF_ENGETSU, plev * 4);
			project_hack(GF_ENGETSU, plev * 4);
			project_hack(GF_ENGETSU, plev * 4);
		}
		break;

	case 26:
		if (name) return _("ɴ�ͻ¤�", "Hundred Slaughter");
		if (desc) return _("Ϣ³�������Ȥǥ�󥹥����򹶷⤹�롣���⤹�뤿�Ӥ�MP�����MP���ʤ��ʤ뤫����󥹥������ݤ��ʤ��ä���ɴ�ͻ¤�Ͻ�λ���롣", 
			"Performs a series of rush attacks. The series continues while killing each monster in a time and SP remains.");
    
		if (cast)
		{
			const int mana_cost_per_monster = 8;
			bool is_new = TRUE;
			bool mdeath;

			do
			{
				if (!rush_attack(&mdeath)) break;
				if (is_new)
				{
					/* Reserve needed mana point */
					p_ptr->csp -= technic_info[REALM_HISSATSU - MIN_TECHNIC][26].smana;
					is_new = FALSE;
				}
				else
					p_ptr->csp -= mana_cost_per_monster;

				if (!mdeath) break;
				command_dir = 0;

				p_ptr->redraw |= PR_MANA;
				handle_stuff();
			}
			while (p_ptr->csp > mana_cost_per_monster);

			if (is_new) return NULL;
	
			/* Restore reserved mana */
			p_ptr->csp += technic_info[REALM_HISSATSU - MIN_TECHNIC][26].smana;
		}
		break;

	case 27:
		if (name) return _("ŷ��ζ��", "Dragonic Flash");
		if (desc) return _("�볦��ξ�����ꤷ�ơ����ξ��ȼ�ʬ�δ֤ˤ�������󥹥����򹶷⤷�����ξ��˰�ư���롣", 
			"Runs toward given location while attacking all monsters on the path.");
    
		if (cast)
		{
			POSITION y, x;

			if (!tgt_pt(&x, &y)) return NULL;

			if (!cave_player_teleportable_bold(y, x, 0L) ||
			    (distance(y, x, p_ptr->y, p_ptr->x) > MAX_SIGHT / 2) ||
			    !projectable(p_ptr->y, p_ptr->x, y, x))
			{
				msg_print(_("���ԡ�", "You cannot move to that place!"));
				break;
			}
			if (p_ptr->anti_tele)
			{
				msg_print(_("�Ի׵Ĥ��Ϥ��ƥ�ݡ��Ȥ��ɤ�����", "A mysterious force prevents you from teleporting!"));
				break;
			}
			project(0, 0, y, x, HISSATSU_ISSEN, GF_ATTACK, PROJECT_BEAM | PROJECT_KILL, -1);
			teleport_player_to(y, x, 0L);
		}
		break;

	case 28:
		if (name) return _("��Ťη���", "Twin Slash");
		if (desc) return _("1�������2�ٹ����Ԥ���", "double attacks at a time.");
    
		if (cast)
		{
			int x, y;
	
			if (!get_rep_dir(&dir, FALSE)) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
			{
				py_attack(y, x, 0);
				if (cave[y][x].m_idx)
				{
					handle_stuff();
					py_attack(y, x, 0);
				}
			}
			else
			{
				msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "You don't see any monster in this direction"));
				return NULL;
			}
		}
		break;

	case 29:
		if (name) return _("����������", "Kofuku-Zettousei");
		if (desc) return _("���Ϥʹ����Ԥ����᤯�ξ��ˤ���̤��ڤ֡�", "Performs a powerful attack which even effect nearby monsters.");
    
		if (cast)
		{
			int total_damage = 0, basedam, i;
			int y, x;
			u32b flgs[TR_FLAG_SIZE];
			object_type *o_ptr;
	
			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (d_info[dungeon_type].flags1 & DF1_NO_MELEE)
			{
				msg_print(_("�ʤ������⤹�뤳�Ȥ��Ǥ��ʤ���", "Something prevent you from attacking."));
				return "";
			}
			msg_print(_("�����礭�����겼������", "You swing your weapon downward."));
			for (i = 0; i < 2; i++)
			{
				int damage;
				if (!buki_motteruka(INVEN_RARM+i)) break;
				o_ptr = &inventory[INVEN_RARM+i];
				basedam = (o_ptr->dd * (o_ptr->ds + 1)) * 50;
				damage = o_ptr->to_d * 100;
				object_flags(o_ptr, flgs);
				if ((o_ptr->name1 == ART_VORPAL_BLADE) || (o_ptr->name1 == ART_CHAINSWORD))
				{
					/* vorpal blade */
					basedam *= 5;
					basedam /= 3;
				}
				else if (have_flag(flgs, TR_VORPAL))
				{
					/* vorpal flag only */
					basedam *= 11;
					basedam /= 9;
				}
				damage += basedam;
				damage += p_ptr->to_d[i] * 100;
				damage *= p_ptr->num_blow[i];
				total_damage += (damage / 100);
			}
			project(0, (cave_have_flag_bold(y, x, FF_PROJECT) ? 5 : 0), y, x, total_damage * 3 / 2, GF_METEOR, PROJECT_KILL | PROJECT_JUMP | PROJECT_ITEM, -1);
		}
		break;

	case 30:
		if (name) return _("�ı���Ǧ��", "Keiun-Kininken");
		if (desc) return _("��ʬ����᡼���򤯤餦�������������礭�ʥ��᡼����Ϳ���롣����ǥåɤˤ��ä˸��̤����롣", 
			"Attacks a monster with extremely powerful damage. But you also takes some damages. Hurts a undead monster greatly.");
    
		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_UNDEAD);
			else
			{
				msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "There is no monster."));
				return NULL;
			}
			take_hit(DAMAGE_NOESCAPE, 100 + randint1(100), _("�ı���Ǧ����Ȥä��׷�", "exhaustion on using Keiun-Kininken"), -1);
		}
		break;

	case 31:
		if (name) return _("��ʢ", "Harakiri");
		if (desc) return _("�����ƻ�Ȥϡ���̤��Ȥȸ��Ĥ����ꡣ��", "'Busido is found in death'");

		if (cast)
		{
			int i;
			if (!get_check(_("�����˼������ޤ�����", "Do you really want to commit suicide? "))) return NULL;
				/* Special Verification for suicide */
			prt(_("��ǧ�Τ��� '@' �򲡤��Ʋ�������", "Please verify SUICIDE by typing the '@' sign: "), 0, 0);
	
			flush();
			i = inkey();
			prt("", 0, 0);
			if (i != '@') return NULL;
			if (p_ptr->total_winner)
			{
				take_hit(DAMAGE_FORCE, 9999, "Seppuku", -1);
				p_ptr->total_winner = TRUE;
			}
			else
			{
				msg_print(_("���ƻ�Ȥϡ���̤��Ȥȸ��Ĥ����ꡣ", "Meaning of Bushi-do is found in the death."));
				take_hit(DAMAGE_FORCE, 9999, "Seppuku", -1);
			}
		}
		break;
	}

	return "";
}

/*!
 * @brief �����ΰ�����������оݤˤǤ�����狼�ɤ������֤��� / An "item_tester_hook" for offer
 * @param o_ptr ���֥������ȹ�¤�Τλ��ȥݥ���
 * @return ������ǽ�����ʤ��TRUE���֤�
 */
static bool item_tester_hook_weapon_except_bow(object_type *o_ptr)
{
	switch (o_ptr->tval)
	{
		case TV_SWORD:
		case TV_HAFTED:
		case TV_POLEARM:
		case TV_DIGGING:
		{
			return (TRUE);
		}
	}

	return (FALSE);
}

/*!
 * @brief �����ΰ�γƽ����˻Ȥ������줿�������ɤ������֤��� / An "item_tester_hook" for offer
 * @param o_ptr ���֥������ȹ�¤�Τλ��ȥݥ���
 * @return �Ȥ��������ʤ��TRUE���֤�
 */
static bool item_tester_hook_cursed(object_type *o_ptr)
{
	return (bool)(object_is_cursed(o_ptr));
}

/*!
 * @brief �����ΰ���ˡ�γƽ�����Ԥ�
 * @param spell ��ˡID
 * @param mode �������� (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST / SPELL_CONT / SPELL_STOP)
 * @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ˤ�ʸ����ݥ��󥿤��֤���SPELL_CAST / SPELL_CONT / SPELL_STOP ����NULLʸ������֤���
 */
static cptr do_hex_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;
	bool cont = (mode == SPELL_CONT) ? TRUE : FALSE;
	bool stop = (mode == SPELL_STOP) ? TRUE : FALSE;

	bool add = TRUE;

	PLAYER_LEVEL plev = p_ptr->lev;
	HIT_POINT power;

	switch (spell)
	{
	/*** 1st book (0-7) ***/
	case 0:
		if (name) return _("�٤ʤ��ʡ", "Evily blessing");
		if (desc) return _("��ʡ�ˤ�깶�����٤��ɸ��Ϥ��夬�롣", "Attempts to increase +to_hit of a weapon and AC");
		if (cast)
		{
			if (!p_ptr->blessed)
			{
				msg_print(_("���ʵ�ʬ�ˤʤä���", "You feel righteous!"));
			}
		}
		if (stop)
		{
			if (!p_ptr->blessed)
			{
				msg_print(_("���ʵ�ʬ���ä���������", "The prayer has expired."));
			}
		}
		break;

	case 1:
		if (name) return _("�ڽ��μ���", "Cure light wounds");
		if (desc) return _("HP����򾯤����������롣", "Heals cut and HP a little.");
		if (info) return info_heal(1, 10, 0);
		if (cast)
		{
			msg_print(_("��ʬ���ɤ��ʤäƤ��롣", "You feel better and better."));
		}
		if (cast || cont)
		{
			hp_player(damroll(1, 10));
			set_cut(p_ptr->cut - 10);
		}
		break;

	case 2:
		if (name) return _("����Υ�����", "Demonic aura");
		if (desc) return _("��Υ������ȤˤޤȤ�������®�٤�®���ʤ롣", "Gives fire aura and regeneration.");
		if (cast)
		{
			msg_print(_("�Τ���Υ������ʤ��줿��", "You have enveloped by fiery aura!"));
		}
		if (stop)
		{
			msg_print(_("��Υ����餬�ä���ä���", "Fiery aura disappeared."));
		}
		break;

	case 3:
		if (name) return _("����̸", "Stinking mist");
		if (desc) return _("�볦��Υ�󥹥����������̤��ǤΥ��᡼����Ϳ���롣", "Deals few damages of poison to all monsters in your sight.");
		power = plev / 2 + 5;
		if (info) return info_damage(1, power, 0);
		if (cast || cont)
		{
			project_hack(GF_POIS, randint1(power));
		}
		break;

	case 4:
		if (name) return _("���϶���", "Extra might");
		if (desc) return _("�ѼԤ����Ϥ�徺�����롣", "Attempts to increase your strength.");
		if (cast)
		{
			msg_print(_("�������Ϥ�ͯ������롣", "You feel you get stronger."));
		}
		break;

	case 5:
		if (name) return _("������", "Curse weapon");
		if (desc) return _("�������Ƥ������������", "Curses your weapon.");
		if (cast)
		{
			OBJECT_IDX item;
			cptr q, s;
			char o_name[MAX_NLEN];
			object_type *o_ptr;
			u32b f[TR_FLAG_SIZE];

			item_tester_hook = item_tester_hook_weapon_except_bow;
			q = _("�ɤ������ޤ�����", "Which weapon do you curse?");
			s = _("�����������Ƥ��ʤ���", "You wield no weapons.");

			if (!get_item(&item, q, s, (USE_EQUIP))) return FALSE;

			o_ptr = &inventory[item];
			object_desc(o_name, o_ptr, OD_NAME_ONLY);
			object_flags(o_ptr, f);

			if (!get_check(format(_("������ %s ������ޤ�����", "Do you curse %s, really��"), o_name))) return FALSE;

			if (!one_in_(3) &&
				(object_is_artifact(o_ptr) || have_flag(f, TR_BLESSED)))
			{
				msg_format(_("%s �ϼ�����ķ���֤�����", "%s resists the effect."), o_name);
				if (one_in_(3))
				{
					if (o_ptr->to_d > 0)
					{
						o_ptr->to_d -= randint1(3) % 2;
						if (o_ptr->to_d < 0) o_ptr->to_d = 0;
					}
					if (o_ptr->to_h > 0)
					{
						o_ptr->to_h -= randint1(3) % 2;
						if (o_ptr->to_h < 0) o_ptr->to_h = 0;
					}
					if (o_ptr->to_a > 0)
					{
						o_ptr->to_a -= randint1(3) % 2;
						if (o_ptr->to_a < 0) o_ptr->to_a = 0;
					}
					msg_format(_("%s ���������Ƥ��ޤä���", "Your %s was disenchanted!"), o_name);
				}
			}
			else
			{
				int curse_rank = 0;
				msg_format(_("���ݤΰŹ������餬���ʤ���%s����߹������", "A terrible black aura blasts your %s!"), o_name);
				o_ptr->curse_flags |= (TRC_CURSED);

				if (object_is_artifact(o_ptr) || object_is_ego(o_ptr))
				{

					if (one_in_(3)) o_ptr->curse_flags |= (TRC_HEAVY_CURSE);
					if (one_in_(666))
					{
						o_ptr->curse_flags |= (TRC_TY_CURSE);
						if (one_in_(666)) o_ptr->curse_flags |= (TRC_PERMA_CURSE);

						add_flag(o_ptr->art_flags, TR_AGGRAVATE);
						add_flag(o_ptr->art_flags, TR_VORPAL);
						add_flag(o_ptr->art_flags, TR_VAMPIRIC);
						msg_print(_("���������������", "Blood, Blood, Blood!"));
						curse_rank = 2;
					}
				}

				o_ptr->curse_flags |= get_curse(curse_rank, o_ptr);
			}

			p_ptr->update |= (PU_BONUS);
			add = FALSE;
		}
		break;

	case 6:
		if (name) return _("�ٰ�����", "Evil detection");
		if (desc) return _("���Ϥμٰ��ʥ�󥹥������Τ��롣", "Detects evil monsters.");
		if (info) return info_range(MAX_SIGHT);
		if (cast)
		{
			msg_print(_("�ٰ�����ʪ��¸�ߤ򴶤�����Ȥ�����", "You attend to the presence of evil creatures."));
		}
		break;

	case 7:
		if (name) return _("����", "Patience");
		if (desc) return _("�������󹶷���Ѥ����塢���������᡼�����Ϲ��ζȲФȤ��Ƽ��Ϥ����Ф��롣", 
			"Bursts hell fire strongly after patients any damage while few turns.");
		power = MIN(200, (HEX_REVENGE_POWER(p_ptr) * 2));
		if (info) return info_damage(0, 0, power);
		if (cast)
		{
			int a = 3 - (p_ptr->pspeed - 100) / 10;
			MAGIC_NUM2 r = 3 + randint1(3) + MAX(0, MIN(3, a));

			if (HEX_REVENGE_TURN(p_ptr) > 0)
			{
				msg_print(_("���Ǥ˲����򤷤Ƥ��롣", "You are already patienting."));
				return NULL;
			}

			HEX_REVENGE_TYPE(p_ptr) = 1;
			HEX_REVENGE_TURN(p_ptr) = r;
			HEX_REVENGE_POWER(p_ptr) = 0;
			msg_print(_("���ä��Ѥ��뤳�Ȥˤ�����", "You decide to patient all damages."));
			add = FALSE;
		}
		if (cont)
		{
			int rad = 2 + (power / 50);

			HEX_REVENGE_TURN(p_ptr)--;

			if ((HEX_REVENGE_TURN(p_ptr) <= 0) || (power >= 200))
			{
				msg_print(_("�������򤫤줿��", "Time for end of patioence!"));
				if (power)
				{
					project(0, rad, p_ptr->y, p_ptr->x, power, GF_HELL_FIRE,
						(PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL), -1);
				}
				if (p_ptr->wizard)
				{
					msg_format(_("%d���Υ��᡼�����֤�����", "You return %d damages."), power);
				}

				/* Reset */
				HEX_REVENGE_TYPE(p_ptr) = 0;
				HEX_REVENGE_TURN(p_ptr) = 0;
				HEX_REVENGE_POWER(p_ptr) = 0;
			}
		}
		break;

	/*** 2nd book (8-15) ***/
	case 8:
		if (name) return _("ɹ�γ�", "Ice armor");
		if (desc) return _("ɹ�Υ������ȤˤޤȤ����ɸ��Ϥ��徺���롣", "Gives fire aura and bonus to AC.");
		if (cast)
		{
			msg_print(_("�Τ�ɹ�γ���ʤ��줿��", "You have enveloped by ice armor!"));
		}
		if (stop)
		{
			msg_print(_("ɹ�γ����ä���ä���", "Ice armor disappeared."));
		}
		break;

	case 9:
		if (name) return _("�Ž��μ���", "Cure serious wounds");
		if (desc) return _("���Ϥ����¿�����������롣", "Heals cut and HP more.");
		if (info) return info_heal(2, 10, 0);
		if (cast)
		{
			msg_print(_("��ʬ���ɤ��ʤäƤ��롣", "You feel better and better."));
		}
		if (cast || cont)
		{
			hp_player(damroll(2, 10));
			set_cut((p_ptr->cut / 2) - 10);
		}
		break;

	case 10:
		if (name) return _("���ʵ���", "Inhail potion");
		if (desc) return _("��ʸ�Ӿ�����ߤ��뤳�Ȥʤ������θ��̤����뤳�Ȥ��Ǥ��롣", "Quaffs a potion without canceling of casting a spell.");
		if (cast)
		{
			CASTING_HEX_FLAGS(p_ptr) |= (1L << HEX_INHAIL);
			do_cmd_quaff_potion();
			CASTING_HEX_FLAGS(p_ptr) &= ~(1L << HEX_INHAIL);
			add = FALSE;
		}
		break;

	case 11:
		if (name) return _("����̸", "Hypodynamic mist");
		if (desc) return _("�볦��Υ�󥹥����������̤ο��°���Υ��᡼����Ϳ���롣", 
			"Deals few damages of hypodynamia to all monsters in your sight.");
		power = (plev / 2) + 5;
		if (info) return info_damage(1, power, 0);
		if (cast || cont)
		{
			project_hack(GF_HYPODYNAMIA, randint1(power));
		}
		break;

	case 12:
		if (name) return _("�����", "Swords to runeswords");
		if (desc) return _("���ι����Ϥ�夲�롣�ڤ�̣�����������˱�����Ϳ������᡼�����徺�������ɤʥ�󥹥������Ф�����᡼����2�ܤˤʤ롣", 
			"Gives vorpal ability to your weapon. Increases damages by your weapon acccording to curse of your weapon.");
		if (cast)
		{
#ifdef JP
			msg_print("���ʤ�����郎������������");
#else
			if (!empty_hands(FALSE))
				msg_print("Your weapons glow bright black.");
			else
				msg_print("Your weapon glows bright black.");
#endif
		}
		if (stop)
		{
#ifdef JP
			msg_print("���ε������ä���ä���");
#else
			msg_format("Brightness of weapon%s disappeared.", (empty_hands(FALSE)) ? "" : "s");
#endif
		}
		break;

	case 13:
		if (name) return _("����μ�", "Touch of confusion");
		if (desc) return _("���⤷���ݥ�󥹥������𤵤��롣", "Confuses a monster when you attack.");
		if (cast)
		{
			msg_print(_("���ʤ��μ꤬�֤������Ϥ᤿��", "Your hands glow bright red."));
		}
		if (stop)
		{
			msg_print(_("��ε������ʤ��ʤä���", "Brightness on your hands disappeard."));
		}
		break;

	case 14:
		if (name) return _("���ζ���", "Building up");
		if (desc) return _("�ѼԤ����ϡ����Ѥ����ѵ��Ϥ�徺�����롣�������ξ�¤� 1 ���ä����롣", 
			"Attempts to increases your strength, dexterity and constitusion.");
		if (cast)
		{
			msg_print(_("���Τ������ʤä�����������", "You feel your body is developed more now."));
		}
		break;

	case 15:
		if (name) return _("ȿ�ƥ�ݡ��ȷ볦", "Anti teleport barrier");
		if (desc) return _("�볦��Υ�󥹥����Υƥ�ݡ��Ȥ��˳�����Хꥢ��ĥ�롣", "Obstructs all teleportations by monsters in your sight.");
		power = plev * 3 / 2;
		if (info) return info_power(power);
		if (cast)
		{
			msg_print(_("�ƥ�ݡ��Ȥ��ɤ������򤫤�����", "You feel anyone can not teleport except you."));
		}
		break;

	/*** 3rd book (16-23) ***/
	case 16:
		if (name) return _("�׷�Υ�����", "Cloak of shock");
		if (desc) return _("�ŵ��Υ������ȤˤޤȤ���ư����®���ʤ롣", "Gives lightning aura and a bonus to speed.");
		if (cast)
		{
			msg_print(_("�Τ���ʤΥ������ʤ��줿��", "You have enveloped by electrical aura!"));
		}
		if (stop)
		{
			msg_print(_("��ʤΥ����餬�ä���ä���", "Electrical aura disappeared."));
		}
		break;

	case 17:
		if (name) return _("��̿���μ���", "Cure critical wounds");
		if (desc) return _("���Ϥ������������롣", "Heals cut and HP greatry.");
		if (info) return info_heal(4, 10, 0);
		if (cast)
		{
			msg_print(_("��ʬ���ɤ��ʤäƤ��롣", "You feel better and better."));
		}
		if (cast || cont)
		{
			hp_player(damroll(4, 10));
			set_stun(0);
			set_cut(0);
			set_poisoned(0);
		}
		break;

	case 18:
		if (name) return _("��������", "Recharging");
		if (desc) return _("��ˡ��ƻ������Ϥ�ƽ�Ŷ���롣", "Recharges a magic device.");
		power = plev * 2;
		if (info) return info_power(power);
		if (cast)
		{
			if (!recharge(power)) return NULL;
			add = FALSE;
		}
		break;

	case 19:
		if (name) return _("�������", "Animate Dead");
		if (desc) return _("���Τ��ɤ餻�ƥڥåȤˤ��롣", "Raises corpses and skeletons from dead.");
		if (cast)
		{
			msg_print(_("��ԤؤθƤӤ�����Ϥ᤿��", "You start to call deads.!"));
		}
		if (cast || cont)
		{
			animate_dead(0, p_ptr->y, p_ptr->x);
		}
		break;

	case 20:
		if (name) return _("�ɶ����", "Curse armor");
		if (desc) return _("�������Ƥ����ɶ�˼����򤫤��롣", "Curse a piece of armour that you wielding.");
		if (cast)
		{
			OBJECT_IDX item;
			cptr q, s;
			char o_name[MAX_NLEN];
			object_type *o_ptr;
			u32b f[TR_FLAG_SIZE];

			item_tester_hook = object_is_armour;
			q = _("�ɤ������ޤ�����", "Which piece of armour do you curse?");
			s = _("�ɶ���������Ƥ��ʤ���", "You wield no piece of armours.");

			if (!get_item(&item, q, s, (USE_EQUIP))) return FALSE;

			o_ptr = &inventory[item];
			object_desc(o_name, o_ptr, OD_NAME_ONLY);
			object_flags(o_ptr, f);

			if (!get_check(format(_("������ %s ������ޤ�����", "Do you curse %s, really��"), o_name))) return FALSE;

			if (!one_in_(3) &&
				(object_is_artifact(o_ptr) || have_flag(f, TR_BLESSED)))
			{
				msg_format(_("%s �ϼ�����ķ���֤�����", "%s resists the effect."), o_name);
				if (one_in_(3))
				{
					if (o_ptr->to_d > 0)
					{
						o_ptr->to_d -= randint1(3) % 2;
						if (o_ptr->to_d < 0) o_ptr->to_d = 0;
					}
					if (o_ptr->to_h > 0)
					{
						o_ptr->to_h -= randint1(3) % 2;
						if (o_ptr->to_h < 0) o_ptr->to_h = 0;
					}
					if (o_ptr->to_a > 0)
					{
						o_ptr->to_a -= randint1(3) % 2;
						if (o_ptr->to_a < 0) o_ptr->to_a = 0;
					}
					msg_format(_("%s ���������Ƥ��ޤä���", "Your %s was disenchanted!"), o_name);
				}
			}
			else
			{
				int curse_rank = 0;
				msg_format(_("���ݤΰŹ������餬���ʤ���%s����߹������", "A terrible black aura blasts your %s!"), o_name);
				o_ptr->curse_flags |= (TRC_CURSED);

				if (object_is_artifact(o_ptr) || object_is_ego(o_ptr))
				{

					if (one_in_(3)) o_ptr->curse_flags |= (TRC_HEAVY_CURSE);
					if (one_in_(666))
					{
						o_ptr->curse_flags |= (TRC_TY_CURSE);
						if (one_in_(666)) o_ptr->curse_flags |= (TRC_PERMA_CURSE);

						add_flag(o_ptr->art_flags, TR_AGGRAVATE);
						add_flag(o_ptr->art_flags, TR_RES_POIS);
						add_flag(o_ptr->art_flags, TR_RES_DARK);
						add_flag(o_ptr->art_flags, TR_RES_NETHER);
						msg_print(_("���������������", "Blood, Blood, Blood!"));
						curse_rank = 2;
					}
				}

				o_ptr->curse_flags |= get_curse(curse_rank, o_ptr);
			}

			p_ptr->update |= (PU_BONUS);
			add = FALSE;
		}
		break;

	case 21:
		if (name) return _("�ƤΥ�����", "Cloak of shadow");
		if (desc) return _("�ƤΥ������ȤˤޤȤ���Ũ�˱ƤΥ��᡼����Ϳ���롣", "Gives aura of shadow.");
		if (cast)
		{
			object_type *o_ptr = &inventory[INVEN_OUTER];

			if (!o_ptr->k_idx)
			{
				msg_print(_("��������ȤˤĤ��Ƥ��ʤ���", "You don't ware any cloak."));
				return NULL;
			}
			else if (!object_is_cursed(o_ptr))
			{
				msg_print(_("�������ϼ����Ƥ��ʤ���", "Your cloak is not cursed."));
				return NULL;
			}
			else
			{
				msg_print(_("�ƤΥ������ȤˤޤȤä���", "You have enveloped by shadow aura!"));
			}
		}
		if (cont)
		{
			object_type *o_ptr = &inventory[INVEN_OUTER];

			if ((!o_ptr->k_idx) || (!object_is_cursed(o_ptr)))
			{
				do_spell(REALM_HEX, spell, SPELL_STOP);
				CASTING_HEX_FLAGS(p_ptr) &= ~(1L << spell);
				CASTING_HEX_NUM(p_ptr)--;
				if (!SINGING_SONG_ID(p_ptr)) set_action(ACTION_NONE);
			}
		}
		if (stop)
		{
			msg_print(_("�ƤΥ����餬�ä���ä���", "Shadow aura disappeared."));
		}
		break;

	case 22:
		if (name) return _("���ˤ����Ϥ�", "Pains to mana");
		if (desc) return _("�볦��Υ�󥹥������������᡼��Ϳ�������Ϥ�ۤ���롣", "Deals psychic damages to all monsters in sight, and drains some mana.");
		power = plev * 3 / 2;
		if (info) return info_damage(1, power, 0);
		if (cast || cont)
		{
			project_hack(GF_PSI_DRAIN, randint1(power));
		}
		break;

	case 23:
		if (name) return _("�ܤˤ��ܤ�", "Eye for an eye");
		if (desc) return _("�Ƿ����ˡ�Ǽ��������᡼���򡢹��⸵�Υ�󥹥����ˤ�Ϳ���롣", "Returns same damage which you got to the monster which damaged you.");
		if (cast)
		{
			msg_print(_("������������˾�ˤ���줿��", "You wish strongly you want to revenge anything."));
		}
		break;

	/*** 4th book (24-31) ***/
	case 24:
		if (name) return _("ȿ�����볦", "Anti multiply barrier");
		if (desc) return _("���γ������������󥹥������������˻ߤ��롣", "Obstructs all multiplying by monsters in entire floor.");
		if (cast)
		{
			msg_print(_("�������˻ߤ�������򤫤�����", "You feel anyone can not already multiply."));
		}
		break;

	case 25:
		if (name) return _("������", "Restoration");
		if (desc) return _("�и��ͤ���������褷����������ǽ���ͤ���������롣", "Restores experience and status.");
		if (cast)
		{
			msg_print(_("�Τ����γ��Ϥ����ᤷ�Ϥ᤿��", "You feel your lost status starting to return."));
		}
		if (cast || cont)
		{
			bool flag = FALSE;
			int d = (p_ptr->max_exp - p_ptr->exp);
			int r = (p_ptr->exp / 20);
			int i;

			if (d > 0)
			{
				if (d < r)
					p_ptr->exp = p_ptr->max_exp;
				else
					p_ptr->exp += r;

				/* Check the experience */
				check_experience();

				flag = TRUE;
			}
			for (i = A_STR; i < 6; i ++)
			{
				if (p_ptr->stat_cur[i] < p_ptr->stat_max[i])
				{
					if (p_ptr->stat_cur[i] < 18)
						p_ptr->stat_cur[i]++;
					else
						p_ptr->stat_cur[i] += 10;

					if (p_ptr->stat_cur[i] > p_ptr->stat_max[i])
						p_ptr->stat_cur[i] = p_ptr->stat_max[i];

					/* Recalculate bonuses */
					p_ptr->update |= (PU_BONUS);

					flag = TRUE;
				}
			}

			if (!flag)
			{
				msg_format(_("%s�μ�ʸ�αӾ����᤿��", "Finish casting '%^s'."), do_spell(REALM_HEX, HEX_RESTORE, SPELL_NAME));
				CASTING_HEX_FLAGS(p_ptr) &= ~(1L << HEX_RESTORE);
				if (cont) CASTING_HEX_NUM(p_ptr)--;
				if (CASTING_HEX_NUM(p_ptr)) p_ptr->action = ACTION_NONE;

				/* Redraw status */
				p_ptr->update |= (PU_BONUS | PU_HP | PU_MANA | PU_SPELLS);
				p_ptr->redraw |= (PR_EXTRA);

				return "";
			}
		}
		break;

	case 26:
		if (name) return _("���ϵۼ�", "Drain curse power");
		if (desc) return _("����줿���μ�����ۼ��������Ϥ�������롣", "Drains curse on your weapon and heals SP a little.");
		if (cast)
		{
			OBJECT_IDX item;
			cptr s, q;
			u32b f[TR_FLAG_SIZE];
			object_type *o_ptr;

			item_tester_hook = item_tester_hook_cursed;
			q = _("�ɤ������ʤ���ۼ����ޤ�����", "Which cursed equipment do you drain mana from?");
			s = _("����줿�����ƥ���������Ƥ��ʤ���", "You have no cursed equipment.");

			if (!get_item(&item, q, s, (USE_EQUIP))) return FALSE;

			o_ptr = &inventory[item];
			object_flags(o_ptr, f);

			p_ptr->csp += (p_ptr->lev / 5) + randint1(p_ptr->lev / 5);
			if (have_flag(f, TR_TY_CURSE) || (o_ptr->curse_flags & TRC_TY_CURSE)) p_ptr->csp += randint1(5);
			if (p_ptr->csp > p_ptr->msp) p_ptr->csp = p_ptr->msp;

			if (o_ptr->curse_flags & TRC_PERMA_CURSE)
			{
				/* Nothing */
			}
			else if (o_ptr->curse_flags & TRC_HEAVY_CURSE)
			{
				if (one_in_(7))
				{
					msg_print(_("���������Ƶۤ���ä���", "Heavy curse vanished away."));
					o_ptr->curse_flags = 0L;
				}
			}
			else if ((o_ptr->curse_flags & (TRC_CURSED)) && one_in_(3))
			{
				msg_print(_("���������Ƶۤ���ä���", "Curse vanished away."));
				o_ptr->curse_flags = 0L;
			}

			add = FALSE;
		}
		break;

	case 27:
		if (name) return _("�۷�ο�", "Swords to vampires");
		if (desc) return _("�۷�°���ǹ��⤹�롣", "Gives vampiric ability to your weapon.");
		if (cast)
		{
#ifdef JP
			msg_print("���ʤ�����郎����ߤ��Ƥ��롣");
#else
			if (!empty_hands(FALSE))
				msg_print("Your weapons want more blood now.");
			else
				msg_print("Your weapon wants more blood now.");
#endif
		}
		if (stop)
		{
#ifdef JP
			msg_print("���γ�˾���ä���ä���");
#else
			msg_format("Thirsty of weapon%s disappeared.", (empty_hands(FALSE)) ? "" : "s");
#endif
		}
		break;

	case 28:
		if (name) return _("ۯ۰�θ���", "Word of stun");
		if (desc) return _("�볦��Υ�󥹥�����ۯ۰�Ȥ����롣", "Stuns all monsters in your sight.");
		power = plev * 4;
		if (info) return info_power(power);
		if (cast || cont)
		{
			stun_monsters(power);
		}
		break;

	case 29:
		if (name) return _("�ư�ư", "Moving into shadow");
		if (desc) return _("��󥹥������٤Υޥ��˽ְִ�ư���롣", "Teleports you close to a monster.");
		if (cast)
		{
			int i, dir;
			POSITION y, x;
			bool flag;

			for (i = 0; i < 3; i++)
			{
				if (!tgt_pt(&x, &y)) return FALSE;

				flag = FALSE;

				for (dir = 0; dir < 8; dir++)
				{
					int dy = y + ddy_ddd[dir];
					int dx = x + ddx_ddd[dir];
					if (dir == 5) continue;
					if(cave[dy][dx].m_idx) flag = TRUE;
				}

				if (!cave_empty_bold(y, x) || (cave[y][x].info & CAVE_ICKY) ||
					(distance(y, x, p_ptr->y, p_ptr->x) > plev + 2))
				{
					msg_print(_("�����ˤϰ�ư�Ǥ��ʤ���", "Can not teleport to there."));
					continue;
				}
				break;
			}

			if (flag && randint0(plev * plev / 2))
			{
				teleport_player_to(y, x, 0L);
			}
			else
			{
				msg_print(_("���äȡ�", "Oops!"));
				teleport_player(30, 0L);
			}

			add = FALSE;
		}
		break;

	case 30:
		if (name) return _("ȿ��ˡ�볦", "Anti magic barrier");
		if (desc) return _("�볦��Υ�󥹥�������ˡ���˳�����Хꥢ��ĥ�롣", "Obstructs all magic spell of monsters in your sight.");
		power = plev * 3 / 2;
		if (info) return info_power(power);
		if (cast)
		{
			msg_print(_("��ˡ���ɤ������򤫤�����", "You feel anyone can not cast spells except you."));
		}
		break;

	case 31:
		if (name) return _("���������", "Revenge sentence");
		if (desc) return _("���������ˤ���ޤǼ��������᡼���˱��������Ϥ��Ϲ��ι�Ф��Ƥ����ġ�", 
			"Fires  a ball of hell fire to try revenging after few turns.");
		power = HEX_REVENGE_POWER(p_ptr);
		if (info) return info_damage(0, 0, power);
		if (cast)
		{
			MAGIC_NUM2 r;
			int a = 3 - (p_ptr->pspeed - 100) / 10;
			r = 1 + randint1(2) + MAX(0, MIN(3, a));

			if (HEX_REVENGE_TURN(p_ptr) > 0)
			{
				msg_print(_("���Ǥ����������Ѥߤ���", "You already pronounced your revenge."));
				return NULL;
			}

			HEX_REVENGE_TYPE(p_ptr) = 2;
			HEX_REVENGE_TURN(p_ptr) = r;
			msg_format(_("���ʤ�����������𤷤������� %d ������", "You pronounce your revenge. %d turns left."), r);
			add = FALSE;
		}
		if (cont)
		{
			HEX_REVENGE_TURN(p_ptr)--;

			if (HEX_REVENGE_TURN(p_ptr) <= 0)
			{
				int dir;

				if (power)
				{
					command_dir = 0;

					do
					{
						msg_print(_("�����λ�����", "Time to revenge!"));
					}
					while (!get_aim_dir(&dir));

					fire_ball(GF_HELL_FIRE, dir, power, 1);

					if (p_ptr->wizard)
					{
						msg_format(_("%d���Υ��᡼�����֤�����", "You return %d damages."), power);
					}
				}
				else
				{
					msg_print(_("�������뵤����������", "You are not a mood to revenge."));
				}
				HEX_REVENGE_POWER(p_ptr) = 0;
			}
		}
		break;
	}

	/* start casting */
	if ((cast) && (add))
	{
		/* add spell */
		CASTING_HEX_FLAGS(p_ptr) |= 1L << (spell);
		CASTING_HEX_NUM(p_ptr)++;

		if (p_ptr->action != ACTION_SPELL) set_action(ACTION_SPELL);
	}

	/* Redraw status */
	if (!info)
	{
		p_ptr->update |= (PU_BONUS | PU_HP | PU_MANA | PU_SPELLS);
		p_ptr->redraw |= (PR_EXTRA | PR_HP | PR_MANA);
	}

	return "";
}


/*!
 * @brief ��ˡ�����Υᥤ��롼����
 * @param realm ��ˡ�ΰ��ID
 * @param spell ���ΰ����ˡID
 * @param mode �������
 * @return ���ΰ���ˡ�˳Ƽ�ƥ����Ȥ��᤿����ʸ���󻲾ȥݥ��󥿡������Ǥʤ�����NULL�ݥ��󥿤��֤���
 */
cptr do_spell(REALM_IDX realm, SPELL_IDX spell, BIT_FLAGS mode)
{
	switch (realm)
	{
	case REALM_LIFE:     return do_life_spell(spell, mode);
	case REALM_SORCERY:  return do_sorcery_spell(spell, mode);
	case REALM_NATURE:   return do_nature_spell(spell, mode);
	case REALM_CHAOS:    return do_chaos_spell(spell, mode);
	case REALM_DEATH:    return do_death_spell(spell, mode);
	case REALM_TRUMP:    return do_trump_spell(spell, mode);
	case REALM_ARCANE:   return do_arcane_spell(spell, mode);
	case REALM_CRAFT:    return do_craft_spell(spell, mode);
	case REALM_DAEMON:   return do_daemon_spell(spell, mode);
	case REALM_CRUSADE:  return do_crusade_spell(spell, mode);
	case REALM_MUSIC:    return do_music_spell(spell, mode);
	case REALM_HISSATSU: return do_hissatsu_spell(spell, mode);
	case REALM_HEX:      return do_hex_spell(spell, mode);
	}

	return NULL;
}
