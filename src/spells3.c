/*!
 * @file spells3.c
 * @brief ��ˡ���̤μ���/ Spell code (part 3)
 * @date 2014/07/26
 * @author
 * <pre>
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.  Other copyrights may also apply.
 * </pre>
 */

#include "angband.h"

/*! �ƥ�ݡ�����õ���λ�Կ� / Maximum number of tries for teleporting */
#define MAX_TRIES 100

/*! ǽ���͸��ݤδ��ܳ�Ψ(1 / HURT_CHANCE) / 1/x chance of reducing stats (for elemental attacks) */
#define HURT_CHANCE 16

/*!
 * @brief ���ꤵ�줿�ޥ�����󥹥����Υƥ�ݡ��Ȳ�ǽ�褫�ɤ�����Ƚ�ꤹ�롣
 * @param m_idx ��󥹥���ID
 * @param y ��ư��Y��ɸ
 * @param x ��ư��X��ɸ
 * @param mode ���ץ����
 * @return �ƥ�ݡ�����Ȥ��������ʤ��true
 */
static bool cave_monster_teleportable_bold(MONSTER_IDX m_idx, int y, int x, BIT_FLAGS mode)
{
	monster_type *m_ptr = &m_list[m_idx];
	cave_type    *c_ptr = &cave[y][x];
	feature_type *f_ptr = &f_info[c_ptr->feat];

	/* Require "teleportable" space */
	if (!have_flag(f_ptr->flags, FF_TELEPORTABLE)) return FALSE;

	if (c_ptr->m_idx && (c_ptr->m_idx != m_idx)) return FALSE;
	if (player_bold(y, x)) return FALSE;

	/* Hack -- no teleport onto glyph of warding */
	if (is_glyph_grid(c_ptr)) return FALSE;
	if (is_explosive_rune_grid(c_ptr)) return FALSE;

	if (!(mode & TELEPORT_PASSIVE))
	{
		if (!monster_can_cross_terrain(c_ptr->feat, &r_info[m_ptr->r_idx], 0)) return FALSE;
	}

	return TRUE;
}


/*!
 * @brief ��󥹥����Υƥ�ݡ��ȥ����������� /
 * Teleport a monster, normally up to "dis" grids away.
 * @param m_idx ��󥹥���ID
 * @param dis �ƥ�ݡ��ȵ�Υ
 * @param mode ���ץ����
 * @return �ƥ�ݡ��Ȥ��ºݤ˹Ԥ�줿��true
 * @details
 * Attempt to move the monster at least "dis/2" grids away.
 * But allow variation to prevent infinite loops.
 */
bool teleport_away(MONSTER_IDX m_idx, int dis, BIT_FLAGS mode)
{
	int oy, ox, d, i, min;
	int tries = 0;
	POSITION ny = 0, nx = 0;

	bool look = TRUE;

	monster_type *m_ptr = &m_list[m_idx];

	/* Paranoia */
	if (!m_ptr->r_idx) return (FALSE);

	/* Save the old location */
	oy = m_ptr->fy;
	ox = m_ptr->fx;

	/* Minimum distance */
	min = dis / 2;

	if ((mode & TELEPORT_DEC_VALOUR) &&
	    (((p_ptr->chp * 10) / p_ptr->mhp) > 5) &&
		(4+randint1(5) < ((p_ptr->chp * 10) / p_ptr->mhp)))
	{
		chg_virtue(V_VALOUR, -1);
	}

	/* Look until done */
	while (look)
	{
		tries++;

		/* Verify max distance */
		if (dis > 200) dis = 200;

		/* Try several locations */
		for (i = 0; i < 500; i++)
		{
			/* Pick a (possibly illegal) location */
			while (1)
			{
				ny = rand_spread(oy, dis);
				nx = rand_spread(ox, dis);
				d = distance(oy, ox, ny, nx);
				if ((d >= min) && (d <= dis)) break;
			}

			/* Ignore illegal locations */
			if (!in_bounds(ny, nx)) continue;

			if (!cave_monster_teleportable_bold(m_idx, ny, nx, mode)) continue;

			/* No teleporting into vaults and such */
			if (!(p_ptr->inside_quest || p_ptr->inside_arena))
				if (cave[ny][nx].info & CAVE_ICKY) continue;

			/* This grid looks good */
			look = FALSE;

			/* Stop looking */
			break;
		}

		/* Increase the maximum distance */
		dis = dis * 2;

		/* Decrease the minimum distance */
		min = min / 2;

		/* Stop after MAX_TRIES tries */
		if (tries > MAX_TRIES) return (FALSE);
	}

	/* Sound */
	sound(SOUND_TPOTHER);

	/* Update the old location */
	cave[oy][ox].m_idx = 0;

	/* Update the new location */
	cave[ny][nx].m_idx = m_idx;

	/* Move the monster */
	m_ptr->fy = ny;
	m_ptr->fx = nx;

	/* Forget the counter target */
	reset_target(m_ptr);

	/* Update the monster (new location) */
	update_mon(m_idx, TRUE);

	/* Redraw the old grid */
	lite_spot(oy, ox);

	/* Redraw the new grid */
	lite_spot(ny, nx);

	if (r_info[m_ptr->r_idx].flags7 & (RF7_LITE_MASK | RF7_DARK_MASK))
		p_ptr->update |= (PU_MON_LITE);

	return (TRUE);
}


/*!
 * @brief ��󥹥�������ꤵ�줿��ɸ�ն�˥ƥ�ݡ��Ȥ��� /
 * Teleport monster next to a grid near the given location
 * @param m_idx ��󥹥���ID
 * @param ty �ܰ�Y��ɸ
 * @param tx �ܰ�X��ɸ
 * @param power �ƥ�ݡ���������Ψ
 * @param mode ���ץ����
 * @return �ʤ�
 */
void teleport_monster_to(MONSTER_IDX m_idx, POSITION ty, POSITION tx, int power, BIT_FLAGS mode)
{
	int ny, nx, oy, ox, d, i, min;
	int attempts = 500;
	int dis = 2;
	bool look = TRUE;
	monster_type *m_ptr = &m_list[m_idx];

	/* Paranoia */
	if (!m_ptr->r_idx) return;

	/* "Skill" test */
	if (randint1(100) > power) return;

	/* Initialize */
	ny = m_ptr->fy;
	nx = m_ptr->fx;

	/* Save the old location */
	oy = m_ptr->fy;
	ox = m_ptr->fx;

	/* Minimum distance */
	min = dis / 2;

	/* Look until done */
	while (look && --attempts)
	{
		/* Verify max distance */
		if (dis > 200) dis = 200;

		/* Try several locations */
		for (i = 0; i < 500; i++)
		{
			/* Pick a (possibly illegal) location */
			while (1)
			{
				ny = rand_spread(ty, dis);
				nx = rand_spread(tx, dis);
				d = distance(ty, tx, ny, nx);
				if ((d >= min) && (d <= dis)) break;
			}

			/* Ignore illegal locations */
			if (!in_bounds(ny, nx)) continue;

			if (!cave_monster_teleportable_bold(m_idx, ny, nx, mode)) continue;

			/* No teleporting into vaults and such */
			/* if (cave[ny][nx].info & (CAVE_ICKY)) continue; */

			/* This grid looks good */
			look = FALSE;

			/* Stop looking */
			break;
		}

		/* Increase the maximum distance */
		dis = dis * 2;

		/* Decrease the minimum distance */
		min = min / 2;
	}

	if (attempts < 1) return;

	/* Sound */
	sound(SOUND_TPOTHER);

	/* Update the old location */
	cave[oy][ox].m_idx = 0;

	/* Update the new location */
	cave[ny][nx].m_idx = m_idx;

	/* Move the monster */
	m_ptr->fy = ny;
	m_ptr->fx = nx;

	/* Update the monster (new location) */
	update_mon(m_idx, TRUE);

	/* Redraw the old grid */
	lite_spot(oy, ox);

	/* Redraw the new grid */
	lite_spot(ny, nx);

	if (r_info[m_ptr->r_idx].flags7 & (RF7_LITE_MASK | RF7_DARK_MASK))
		p_ptr->update |= (PU_MON_LITE);
}

/*!
 * @brief ���ꤵ�줿�ޥ��˥ץ쥤�䡼���ƥ�ݡ��Ȳ�ǽ���ɤ�����Ƚ�ꤹ�롣
 * @param y ��ư��Y��ɸ
 * @param x ��ư��X��ɸ
 * @param mode ���ץ����
 * @return �ƥ�ݡ�����Ȥ��������ʤ��true
 */
bool cave_player_teleportable_bold(int y, int x, BIT_FLAGS mode)
{
	cave_type    *c_ptr = &cave[y][x];
	feature_type *f_ptr = &f_info[c_ptr->feat];

	/* Require "teleportable" space */
	if (!have_flag(f_ptr->flags, FF_TELEPORTABLE)) return FALSE;

	/* No magical teleporting into vaults and such */
	if (!(mode & TELEPORT_NONMAGICAL) && (c_ptr->info & CAVE_ICKY)) return FALSE;

	if (c_ptr->m_idx && (c_ptr->m_idx != p_ptr->riding)) return FALSE;

	/* don't teleport on a trap. */
	if (have_flag(f_ptr->flags, FF_HIT_TRAP)) return FALSE;

	if (!(mode & TELEPORT_PASSIVE))
	{
		if (!player_can_enter(c_ptr->feat, 0)) return FALSE;

		if (have_flag(f_ptr->flags, FF_WATER) && have_flag(f_ptr->flags, FF_DEEP))
		{
			if (!p_ptr->levitation && !p_ptr->can_swim) return FALSE;
		}

		if (have_flag(f_ptr->flags, FF_LAVA) && !p_ptr->immune_fire && !IS_INVULN())
		{
			/* Always forbid deep lava */
			if (have_flag(f_ptr->flags, FF_DEEP)) return FALSE;

			/* Forbid shallow lava when the player don't have levitation */
			if (!p_ptr->levitation) return FALSE;
		}

	}

	return TRUE;
}


/*! �ƥ�ݡ��Ⱥ����Υ */
#define MAX_TELEPORT_DISTANCE 200

/*!
 * @brief �ץ쥤�䡼�Υƥ�ݡ���������Ȱ�ư���� /
 * Teleport the player to a location up to "dis" grids away.
 * @param dis ���ܰ�ư��Υ
 * @param mode ���ץ����
 * @return �ºݤ˥ƥ�ݡ��Ƚ������Ԥ�줿��true
 * @details
 * <pre>
 * If no such spaces are readily available, the distance may increase.
 * Try very hard to move the player at least a quarter that distance.
 *
 * There was a nasty tendency for a long time; which was causing the
 * player to "bounce" between two or three different spots because
 * these are the only spots that are "far enough" way to satisfy the
 * algorithm.
 *
 * But this tendency is now removed; in the new algorithm, a list of
 * candidates is selected first, which includes at least 50% of all
 * floor grids within the distance, and any single grid in this list
 * of candidates has equal possibility to be choosen as a destination.
 * </pre>
 */

bool teleport_player_aux(int dis, BIT_FLAGS mode)
{
	int candidates_at[MAX_TELEPORT_DISTANCE + 1];
	int total_candidates, cur_candidates;
	POSITION y = 0, x = 0;
	int min, pick, i;

	int left = MAX(1, p_ptr->x - dis);
	int right = MIN(cur_wid - 2, p_ptr->x + dis);
	int top = MAX(1, p_ptr->y - dis);
	int bottom = MIN(cur_hgt - 2, p_ptr->y + dis);

	if (p_ptr->wild_mode) return FALSE;

	if (p_ptr->anti_tele && !(mode & TELEPORT_NONMAGICAL))
	{
		msg_print(_("�Ի׵Ĥ��Ϥ��ƥ�ݡ��Ȥ��ɤ�����", "A mysterious force prevents you from teleporting!"));
		return FALSE;
	}

	/* Initialize counters */
	total_candidates = 0;
	for (i = 0; i <= MAX_TELEPORT_DISTANCE; i++)
		candidates_at[i] = 0;

	/* Limit the distance */
	if (dis > MAX_TELEPORT_DISTANCE) dis = MAX_TELEPORT_DISTANCE;

	/* Search valid locations */
	for (y = top; y <= bottom; y++)
	{
		for (x = left; x <= right; x++)
		{
			int d;

			/* Skip illegal locations */
			if (!cave_player_teleportable_bold(y, x, mode)) continue;

			/* Calculate distance */
			d = distance(p_ptr->y, p_ptr->x, y, x);

			/* Skip too far locations */
			if (d > dis) continue;

			/* Count the total number of candidates */
			total_candidates++;

			/* Count the number of candidates in this circumference */
			candidates_at[d]++;
		}
	}

	/* No valid location! */
	if (0 == total_candidates) return FALSE;

	/* Fix the minimum distance */
	for (cur_candidates = 0, min = dis; min >= 0; min--)
	{
		cur_candidates += candidates_at[min];

		/* 50% of all candidates will have an equal chance to be choosen. */
		if (cur_candidates && (cur_candidates >= total_candidates / 2)) break;
	}

	/* Pick up a single location randomly */
	pick = randint1(cur_candidates);

	/* Search again the choosen location */
	for (y = top; y <= bottom; y++)
	{
		for (x = left; x <= right; x++)
		{
			int d;

			/* Skip illegal locations */
			if (!cave_player_teleportable_bold(y, x, mode)) continue;

			/* Calculate distance */
			d = distance(p_ptr->y, p_ptr->x, y, x);

			/* Skip too far locations */
			if (d > dis) continue;

			/* Skip too close locations */
			if (d < min) continue;

			/* This grid was picked up? */
			pick--;
			if (!pick) break;
		}

		/* Exit the loop */
		if (!pick) break;
	}

	if (player_bold(y, x)) return FALSE;

	/* Sound */
	sound(SOUND_TELEPORT);

#ifdef JP
	if ((p_ptr->pseikaku == SEIKAKU_COMBAT) || (inventory[INVEN_BOW].name1 == ART_CRIMSON))
		msg_format("�ؤ��ä�������%s��", p_ptr->name);
#endif

	/* Move the player */
	(void)move_player_effect(y, x, MPE_FORGET_FLOW | MPE_HANDLE_STUFF | MPE_DONT_PICKUP);

	return TRUE;
}

/*!
 * @brief �ץ쥤�䡼�Υƥ�ݡ��Ƚ����ᥤ��롼����
 * @param dis ���ܰ�ư��Υ
 * @param mode ���ץ����
 * @return �ʤ�
 */
void teleport_player(int dis, BIT_FLAGS mode)
{
	int yy, xx;

	/* Save the old location */
	int oy = p_ptr->y;
	int ox = p_ptr->x;

	if (!teleport_player_aux(dis, mode)) return;

	/* Monsters with teleport ability may follow the player */
	for (xx = -1; xx < 2; xx++)
	{
		for (yy = -1; yy < 2; yy++)
		{
			MONSTER_IDX tmp_m_idx = cave[oy+yy][ox+xx].m_idx;

			/* A monster except your mount may follow */
			if (tmp_m_idx && (p_ptr->riding != tmp_m_idx))
			{
				monster_type *m_ptr = &m_list[tmp_m_idx];
				monster_race *r_ptr = &r_info[m_ptr->r_idx];

				/*
				 * The latter limitation is to avoid
				 * totally unkillable suckers...
				 */
				if ((r_ptr->a_ability_flags2 & RF6_TPORT) &&
				    !(r_ptr->flagsr & RFR_RES_TELE))
				{
					if (!MON_CSLEEP(m_ptr)) teleport_monster_to(tmp_m_idx, p_ptr->y, p_ptr->x, r_ptr->level, 0L);
				}
			}
		}
	}
}


/*!
 * @brief �ץ쥤�䡼�Υƥ�ݡ��ȥ����������� /
 * @param m_idx �����������ߤ��ץ쥤�䡼ID
 * @param dis �ƥ�ݡ��ȵ�Υ
 * @return �ʤ�
 */
void teleport_player_away(MONSTER_IDX m_idx, int dis)
{
	int yy, xx;

	/* Save the old location */
	int oy = p_ptr->y;
	int ox = p_ptr->x;

	if (!teleport_player_aux(dis, TELEPORT_PASSIVE)) return;

	/* Monsters with teleport ability may follow the player */
	for (xx = -1; xx < 2; xx++)
	{
		for (yy = -1; yy < 2; yy++)
		{
			IDX tmp_m_idx = cave[oy+yy][ox+xx].m_idx;

			/* A monster except your mount or caster may follow */
			if (tmp_m_idx && (p_ptr->riding != tmp_m_idx) && (m_idx != tmp_m_idx))
			{
				monster_type *m_ptr = &m_list[tmp_m_idx];
				monster_race *r_ptr = &r_info[m_ptr->r_idx];

				/*
				 * The latter limitation is to avoid
				 * totally unkillable suckers...
				 */
				if ((r_ptr->a_ability_flags2 & RF6_TPORT) &&
				    !(r_ptr->flagsr & RFR_RES_TELE))
				{
					if (!MON_CSLEEP(m_ptr)) teleport_monster_to(tmp_m_idx, p_ptr->y, p_ptr->x, r_ptr->level, 0L);
				}
			}
		}
	}
}


/*!
 * @brief �ץ쥤�䡼�������ֶ��դ˥ƥ�ݡ��Ȥ�����
 * Teleport player to a grid near the given location
 * @param ny ��ɸY��ɸ
 * @param nx ��ɸX��ɸ
 * @param mode ���ץ����ե饰
 * @return �ʤ�
 * @details
 * <pre>
 * This function is slightly obsessive about correctness.
 * This function allows teleporting into vaults (!)
 * </pre>
 */
void teleport_player_to(POSITION ny, POSITION nx, BIT_FLAGS mode)
{
	POSITION y, x;
	int dis = 0, ctr = 0;

	if (p_ptr->anti_tele && !(mode & TELEPORT_NONMAGICAL))
	{
		msg_print(_("�Ի׵Ĥ��Ϥ��ƥ�ݡ��Ȥ��ɤ�����", "A mysterious force prevents you from teleporting!"));
		return;
	}

	/* Find a usable location */
	while (1)
	{
		/* Pick a nearby legal location */
		while (1)
		{
			y = (POSITION)rand_spread(ny, dis);
			x = (POSITION)rand_spread(nx, dis);
			if (in_bounds(y, x)) break;
		}

		/* Accept any grid when wizard mode */
		if (p_ptr->wizard && !(mode & TELEPORT_PASSIVE) && (!cave[y][x].m_idx || (cave[y][x].m_idx == p_ptr->riding))) break;

		/* Accept teleportable floor grids */
		if (cave_player_teleportable_bold(y, x, mode)) break;

		/* Occasionally advance the distance */
		if (++ctr > (4 * dis * dis + 4 * dis + 1))
		{
			ctr = 0;
			dis++;
		}
	}

	/* Sound */
	sound(SOUND_TELEPORT);

	/* Move the player */
	(void)move_player_effect(y, x, MPE_FORGET_FLOW | MPE_HANDLE_STUFF | MPE_DONT_PICKUP);
}


void teleport_away_followable(MONSTER_IDX m_idx)
{
	monster_type *m_ptr = &m_list[m_idx];
	int          oldfy = m_ptr->fy;
	int          oldfx = m_ptr->fx;
	bool         old_ml = m_ptr->ml;
	int          old_cdis = m_ptr->cdis;

	teleport_away(m_idx, MAX_SIGHT * 2 + 5, 0L);

	if (old_ml && (old_cdis <= MAX_SIGHT) && !world_monster && !p_ptr->inside_battle && los(p_ptr->y, p_ptr->x, oldfy, oldfx))
	{
		bool follow = FALSE;

		if ((p_ptr->muta1 & MUT1_VTELEPORT) || (p_ptr->pclass == CLASS_IMITATOR)) follow = TRUE;
		else
		{
			u32b flgs[TR_FLAG_SIZE];
			object_type *o_ptr;
			int i;

			for (i = INVEN_RARM; i < INVEN_TOTAL; i++)
			{
				o_ptr = &inventory[i];
				if (o_ptr->k_idx && !object_is_cursed(o_ptr))
				{
					object_flags(o_ptr, flgs);
					if (have_flag(flgs, TR_TELEPORT))
					{
						follow = TRUE;
						break;
					}
				}
			}
		}

		if (follow)
		{
			if (get_check_strict(_("�Ĥ��Ƥ����ޤ�����", "Do you follow it? "), CHECK_OKAY_CANCEL))
			{
				if (one_in_(3))
				{
					teleport_player(200, TELEPORT_PASSIVE);
					msg_print(_("���ԡ�", "Failed!"));
				}
				else teleport_player_to(m_ptr->fy, m_ptr->fx, 0L);
				p_ptr->energy_need += ENERGY_NEED();
			}
		}
	}
}


/*!
 * @brief �ץ쥤�䡼�ڤӥ�󥹥������٥�ƥ�ݡ��Ȥ����� /
 * Teleport the player one level up or down (random when legal)
 * @param m_idx �ƥ�ݡ��Ȥ��оݤȤʤ��󥹥���ID(0�ʤ�Хץ쥤�䡼) / If m_idx <= 0, target is player.
 * @return �ʤ�
 */
void teleport_level(MONSTER_IDX m_idx)
{
	bool         go_up;
	char         m_name[160];
	bool         see_m = TRUE;

	if (m_idx <= 0) /* To player */
	{
		strcpy(m_name, _("���ʤ�", "you"));
	}
	else /* To monster */
	{
		monster_type *m_ptr = &m_list[m_idx];

		/* Get the monster name (or "it") */
		monster_desc(m_name, m_ptr, 0);

		see_m = is_seen(m_ptr);
	}

	/* No effect in some case */
	if (TELE_LEVEL_IS_INEFF(m_idx))
	{
		if (see_m) msg_print(_("���̤��ʤ��ä���", "There is no effect."));
		return;
	}

	if ((m_idx <= 0) && p_ptr->anti_tele) /* To player */
	{
		msg_print(_("�Ի׵Ĥ��Ϥ��ƥ�ݡ��Ȥ��ɤ�����", "A mysterious force prevents you from teleporting!"));
		return;
	}

	/* Choose up or down */
	if (randint0(100) < 50) go_up = TRUE;
	else go_up = FALSE;

	if ((m_idx <= 0) && p_ptr->wizard)
	{
		if (get_check("Force to go up? ")) go_up = TRUE;
		else if (get_check("Force to go down? ")) go_up = FALSE;
	}

	/* Down only */ 
	if ((ironman_downward && (m_idx <= 0)) || (dun_level <= d_info[dungeon_type].mindepth))
	{
#ifdef JP
		if (see_m) msg_format("%^s�Ͼ����ͤ��ˤä�����Ǥ�����", m_name);
#else
		if (see_m) msg_format("%^s sink%s through the floor.", m_name, (m_idx <= 0) ? "" : "s");
#endif
		if (m_idx <= 0) /* To player */
		{
			if (!dun_level)
			{
				dungeon_type = ironman_downward ? DUNGEON_ANGBAND : p_ptr->recall_dungeon;
				p_ptr->oldpy = p_ptr->y;
				p_ptr->oldpx = p_ptr->x;
			}

			if (record_stair) do_cmd_write_nikki(NIKKI_TELE_LEV, 1, NULL);

			if (autosave_l) do_cmd_save_game(TRUE);

			if (!dun_level)
			{
				dun_level = d_info[dungeon_type].mindepth;
				prepare_change_floor_mode(CFM_RAND_PLACE);
			}
			else
			{
				prepare_change_floor_mode(CFM_SAVE_FLOORS | CFM_DOWN | CFM_RAND_PLACE | CFM_RAND_CONNECT);
			}

			/* Leaving */
			p_ptr->leaving = TRUE;
		}
	}

	/* Up only */
	else if (quest_number(dun_level) || (dun_level >= d_info[dungeon_type].maxdepth))
	{
#ifdef JP
		if (see_m) msg_format("%^s��ŷ����ͤ��ˤä�����⤤�Ƥ�����", m_name);
#else
		if (see_m) msg_format("%^s rise%s up through the ceiling.", m_name, (m_idx <= 0) ? "" : "s");
#endif


		if (m_idx <= 0) /* To player */
		{
			if (record_stair) do_cmd_write_nikki(NIKKI_TELE_LEV, -1, NULL);

			if (autosave_l) do_cmd_save_game(TRUE);

			prepare_change_floor_mode(CFM_SAVE_FLOORS | CFM_UP | CFM_RAND_PLACE | CFM_RAND_CONNECT);

			leave_quest_check();

			/* Leaving */
			p_ptr->inside_quest = 0;
			p_ptr->leaving = TRUE;
		}
	}
	else if (go_up)
	{
#ifdef JP
		if (see_m) msg_format("%^s��ŷ����ͤ��ˤä�����⤤�Ƥ�����", m_name);
#else
		if (see_m) msg_format("%^s rise%s up through the ceiling.", m_name, (m_idx <= 0) ? "" : "s");
#endif


		if (m_idx <= 0) /* To player */
		{
			if (record_stair) do_cmd_write_nikki(NIKKI_TELE_LEV, -1, NULL);

			if (autosave_l) do_cmd_save_game(TRUE);

			prepare_change_floor_mode(CFM_SAVE_FLOORS | CFM_UP | CFM_RAND_PLACE | CFM_RAND_CONNECT);

			/* Leaving */
			p_ptr->leaving = TRUE;
		}
	}
	else
	{
#ifdef JP
		if (see_m) msg_format("%^s�Ͼ����ͤ��ˤä�����Ǥ�����", m_name);
#else
		if (see_m) msg_format("%^s sink%s through the floor.", m_name, (m_idx <= 0) ? "" : "s");
#endif

		if (m_idx <= 0) /* To player */
		{
			/* Never reach this code on the surface */
			/* if (!dun_level) dungeon_type = p_ptr->recall_dungeon; */

			if (record_stair) do_cmd_write_nikki(NIKKI_TELE_LEV, 1, NULL);

			if (autosave_l) do_cmd_save_game(TRUE);

			prepare_change_floor_mode(CFM_SAVE_FLOORS | CFM_DOWN | CFM_RAND_PLACE | CFM_RAND_CONNECT);

			/* Leaving */
			p_ptr->leaving = TRUE;
		}
	}

	/* Monster level teleportation is simple deleting now */
	if (m_idx > 0)
	{
		monster_type *m_ptr = &m_list[m_idx];

		/* Check for quest completion */
		check_quest_completion(m_ptr);

		if (record_named_pet && is_pet(m_ptr) && m_ptr->nickname)
		{
			char m2_name[80];

			monster_desc(m2_name, m_ptr, MD_INDEF_VISIBLE);
			do_cmd_write_nikki(NIKKI_NAMED_PET, RECORD_NAMED_PET_TELE_LEVEL, m2_name);
		}

		delete_monster_idx(m_idx);
	}

	/* Sound */
	sound(SOUND_TPLEVEL);
}


/*!
 * @brief ����ޤǤ����ä����󥸥��ΰ�����ɽ���������򤵤��롣
 * @param note ���󥸥��˻ܤ���������
 * @param y ���󥽡���Y��ɸ
 * @param x ���󥽡���X��ɸ
 * @return ���򤵤줿���󥸥��ID
 */
DUNGEON_IDX choose_dungeon(cptr note, POSITION y, POSITION x)
{
	DUNGEON_IDX select_dungeon;
	DUNGEON_IDX i;
	int num = 0;
	DUNGEON_IDX *dun;

	/* Hack -- No need to choose dungeon in some case */
	if (lite_town || vanilla_town || ironman_downward)
	{
		if (max_dlv[DUNGEON_ANGBAND]) return DUNGEON_ANGBAND;
		else
		{
			msg_format(_("�ޤ�%s�����ä����ȤϤʤ���", "You haven't entered %s yet."), d_name + d_info[DUNGEON_ANGBAND].name);
			msg_print(NULL);
			return 0;
		}
	}

	/* Allocate the "dun" array */
	C_MAKE(dun, max_d_idx, s16b);

	screen_save();
	for(i = 1; i < max_d_idx; i++)
	{
		char buf[80];
		bool seiha = FALSE;

		if (!d_info[i].maxdepth) continue;
		if (!max_dlv[i]) continue;
		if (d_info[i].final_guardian)
		{
			if (!r_info[d_info[i].final_guardian].max_num) seiha = TRUE;
		}
		else if (max_dlv[i] == d_info[i].maxdepth) seiha = TRUE;

		sprintf(buf,_("      %c) %c%-12s : ���� %d ��", "      %c) %c%-16s : Max level %d"), 
					'a'+num, seiha ? '!' : ' ', d_name + d_info[i].name, (int)max_dlv[i]);
		prt(buf, y + num, x);
		dun[num++] = i;
	}

	if (!num)
	{
		prt(_("      ���٤���󥸥�󤬤ʤ���", "      No dungeon is available."), y, x);
	}

	prt(format(_("�ɤΥ��󥸥��%s���ޤ���:", "Which dungeon do you %s?: "), note), 0, 0);
	while(1)
	{
		i = inkey();
		if ((i == ESCAPE) || !num)
		{
			/* Free the "dun" array */
			C_KILL(dun, max_d_idx, s16b);

			screen_load();
			return 0;
		}
		if (i >= 'a' && i <('a'+num))
		{
			select_dungeon = dun[i-'a'];
			break;
		}
		else bell();
	}
	screen_load();

	/* Free the "dun" array */
	C_KILL(dun, max_d_idx, s16b);

	return select_dungeon;
}


/*!
 * @brief �ץ쥤�䡼�ε���ȯư�ڤ���߽��� /
 * Recall the player to town or dungeon
 * @param turns ȯư�ޤǤΥ������
 * @return ���TRUE���֤�
 */
bool recall_player(TIME_EFFECT turns)
{
	/*
	 * TODO: Recall the player to the last
	 * visited town when in the wilderness
	 */

	/* Ironman option */
	if (p_ptr->inside_arena || ironman_downward)
	{
		msg_print(_("���ⵯ����ʤ��ä���", "Nothing happens."));
		return TRUE;
	}

	if (dun_level && (max_dlv[dungeon_type] > dun_level) && !p_ptr->inside_quest && !p_ptr->word_recall)
	{
		if (get_check(_("�����Ϻǿ���ã������������Ǥ������γ�����ä���ޤ����� ", "Reset recall depth? ")))
		{
			max_dlv[dungeon_type] = dun_level;
			if (record_maxdepth)
				do_cmd_write_nikki(NIKKI_TRUMP, dungeon_type, _("���ԤΤȤ���", "when recall from dungeon"));
		}

	}
	if (!p_ptr->word_recall)
	{
		if (!dun_level)
		{
			DUNGEON_IDX select_dungeon;
			select_dungeon = choose_dungeon(_("�˵���", "recall"), 2, 14);
			if (!select_dungeon) return FALSE;
			p_ptr->recall_dungeon = select_dungeon;
		}
		p_ptr->word_recall = turns;
		msg_print(_("�����絤��ĥ��Ĥ�Ƥ���...", "The air about you becomes charged..."));
		p_ptr->redraw |= (PR_STATUS);
	}
	else
	{
		p_ptr->word_recall = 0;
		msg_print(_("ĥ��Ĥ᤿�絤��ή���ä�...", "A tension leaves the air around you..."));
		p_ptr->redraw |= (PR_STATUS);
	}
	return TRUE;
}

/*!
 * @brief �����ѥᥤ��롼����
 * @return ���TRUE���֤�
 */
bool word_of_recall(void)
{
	return(recall_player(randint0(21) + 15));
}

/*!
 * @brief �ե����ꥻ�åȽ���
 * @return �ꥻ�åȽ������ºݤ˹Ԥ�줿��TRUE���֤�
 */
bool reset_recall(void)
{
	int select_dungeon, dummy = 0;
	char ppp[80];
	char tmp_val[160];

	select_dungeon = choose_dungeon(_("�򥻥å�", "reset"), 2, 14);

	/* Ironman option */
	if (ironman_downward)
	{
		msg_print(_("���ⵯ����ʤ��ä���", "Nothing happens."));
		return TRUE;
	}

	if (!select_dungeon) return FALSE;
	/* Prompt */
	sprintf(ppp, _("�����˥��åȤ��ޤ��� (%d-%d):", "Reset to which level (%d-%d): "),
		(int)d_info[select_dungeon].mindepth, (int)max_dlv[select_dungeon]);

	/* Default */
	sprintf(tmp_val, "%d", (int)MAX(dun_level, 1));

	/* Ask for a level */
	if (get_string(ppp, tmp_val, 10))
	{
		/* Extract request */
		dummy = atoi(tmp_val);

		/* Paranoia */
		if (dummy < 1) dummy = 1;

		/* Paranoia */
		if (dummy > max_dlv[select_dungeon]) dummy = max_dlv[select_dungeon];
		if (dummy < d_info[select_dungeon].mindepth) dummy = d_info[select_dungeon].mindepth;

		max_dlv[select_dungeon] = dummy;

		if (record_maxdepth)
			do_cmd_write_nikki(NIKKI_TRUMP, select_dungeon, _("�ե����ꥻ�åȤ�", "using a scroll of reset recall"));
					/* Accept request */
#ifdef JP
msg_format("%s�ε��ԥ�٥�� %d ���˥��åȡ�", d_name+d_info[select_dungeon].name, dummy, dummy * 50);
#else
		msg_format("Recall depth set to level %d (%d').", dummy, dummy * 50);
#endif

	}
	else
	{
		return FALSE;
	}
	return TRUE;
}


/*!
 * @brief �ץ쥤�䡼�������������� /
 * Apply disenchantment to the player's stuff
 * @param mode �ǲ��̥ӥåȤ�1�ʤ�������������㴳�㸺�����
 * @return ���������˴ؤ����å�������ȯ����줿����TRUE���֤� /
 * Return "TRUE" if the player notices anything
 */
bool apply_disenchant(BIT_FLAGS mode)
{
	int             t = 0;
	object_type     *o_ptr;
	char            o_name[MAX_NLEN];
	int to_h, to_d, to_a, pval;

	/* Pick a random slot */
	switch (randint1(8))
	{
		case 1: t = INVEN_RARM; break;
		case 2: t = INVEN_LARM; break;
		case 3: t = INVEN_BOW; break;
		case 4: t = INVEN_BODY; break;
		case 5: t = INVEN_OUTER; break;
		case 6: t = INVEN_HEAD; break;
		case 7: t = INVEN_HANDS; break;
		case 8: t = INVEN_FEET; break;
	}

	/* Get the item */
	o_ptr = &inventory[t];

	/* No item, nothing happens */
	if (!o_ptr->k_idx) return (FALSE);

	/* Disenchant equipments only -- No disenchant on monster ball */
	if (!object_is_weapon_armour_ammo(o_ptr))
		return FALSE;

	/* Nothing to disenchant */
	if ((o_ptr->to_h <= 0) && (o_ptr->to_d <= 0) && (o_ptr->to_a <= 0) && (o_ptr->pval <= 1))
	{
		/* Nothing to notice */
		return (FALSE);
	}


	/* Describe the object */
	object_desc(o_name, o_ptr, (OD_OMIT_PREFIX | OD_NAME_ONLY));


	/* Artifacts have 71% chance to resist */
	if (object_is_artifact(o_ptr) && (randint0(100) < 71))
	{
		/* Message */
#ifdef JP
msg_format("%s(%c)��������ķ���֤�����",o_name, index_to_label(t) );
#else
		msg_format("Your %s (%c) resist%s disenchantment!",
			   o_name, index_to_label(t),
			   ((o_ptr->number != 1) ? "" : "s"));
#endif


		/* Notice */
		return (TRUE);
	}


	/* Memorize old value */
	to_h = o_ptr->to_h;
	to_d = o_ptr->to_d;
	to_a = o_ptr->to_a;
	pval = o_ptr->pval;

	/* Disenchant tohit */
	if (o_ptr->to_h > 0) o_ptr->to_h--;
	if ((o_ptr->to_h > 5) && (randint0(100) < 20)) o_ptr->to_h--;

	/* Disenchant todam */
	if (o_ptr->to_d > 0) o_ptr->to_d--;
	if ((o_ptr->to_d > 5) && (randint0(100) < 20)) o_ptr->to_d--;

	/* Disenchant toac */
	if (o_ptr->to_a > 0) o_ptr->to_a--;
	if ((o_ptr->to_a > 5) && (randint0(100) < 20)) o_ptr->to_a--;

	/* Disenchant pval (occasionally) */
	/* Unless called from wild_magic() */
	if ((o_ptr->pval > 1) && one_in_(13) && !(mode & 0x01)) o_ptr->pval--;

	if ((to_h != o_ptr->to_h) || (to_d != o_ptr->to_d) ||
	    (to_a != o_ptr->to_a) || (pval != o_ptr->pval))
	{
		/* Message */
#ifdef JP
		msg_format("%s(%c)���������Ƥ��ޤä���",
			   o_name, index_to_label(t) );
#else
		msg_format("Your %s (%c) %s disenchanted!",
			   o_name, index_to_label(t),
			   ((o_ptr->number != 1) ? "were" : "was"));
#endif

		chg_virtue(V_HARMONY, 1);
		chg_virtue(V_ENCHANT, -2);

		/* Recalculate bonuses */
		p_ptr->update |= (PU_BONUS);

		/* Window stuff */
		p_ptr->window |= (PW_EQUIP | PW_PLAYER);

		calc_android_exp();
	}

	/* Notice */
	return (TRUE);
}

/*!
 * @brief �ץ쥤�䡼�������Ѱ۽���
 * @return �ʤ�
 */
void mutate_player(void)
{
	BASE_STATUS max1, cur1, max2, cur2;
	int ii, jj, i;

	/* Pick a pair of stats */
	ii = randint0(6);
	for (jj = ii; jj == ii; jj = randint0(6)) /* loop */;

	max1 = p_ptr->stat_max[ii];
	cur1 = p_ptr->stat_cur[ii];
	max2 = p_ptr->stat_max[jj];
	cur2 = p_ptr->stat_cur[jj];

	p_ptr->stat_max[ii] = max2;
	p_ptr->stat_cur[ii] = cur2;
	p_ptr->stat_max[jj] = max1;
	p_ptr->stat_cur[jj] = cur1;

	for (i=0;i<6;i++)
	{
		if(p_ptr->stat_max[i] > p_ptr->stat_max_max[i]) p_ptr->stat_max[i] = p_ptr->stat_max_max[i];
		if(p_ptr->stat_cur[i] > p_ptr->stat_max_max[i]) p_ptr->stat_cur[i] = p_ptr->stat_max_max[i];
	}

	p_ptr->update |= (PU_BONUS);
}


/*!
 * @brief �ץ쥤�䡼�ΰ��̺������ / Apply Nexus
 * @param m_ptr ���̺����ץ쥤�䡼��Ϳ������󥹥����ξ��󻲾ȥݥ���
 * @return �ʤ�
 */
void apply_nexus(monster_type *m_ptr)
{
	switch (randint1(7))
	{
		case 1: case 2: case 3:
		{
			teleport_player(200, TELEPORT_PASSIVE);
			break;
		}

		case 4: case 5:
		{
			teleport_player_to(m_ptr->fy, m_ptr->fx, TELEPORT_PASSIVE);
			break;
		}

		case 6:
		{
			if (randint0(100) < p_ptr->skill_sav)
			{
				msg_print(_("���������Ϥ�ķ���֤�����", "You resist the effects!"));
				break;
			}

			/* Teleport Level */
			teleport_level(0);
			break;
		}

		case 7:
		{
			if (randint0(100) < p_ptr->skill_sav)
			{
				msg_print(_("���������Ϥ�ķ���֤�����", "You resist the effects!"));
				break;
			}

			msg_print(_("�Τ��ͤ���Ϥ᤿...", "Your body starts to scramble..."));
			mutate_player();
			break;
		}
	}
}


/*!
 * @brief ��̿�Ĥ�������ǳ���ɲý��� /
 * Charge a lite (torch or latern)
 * @return �ʤ�
 */
void phlogiston(void)
{
	GAME_TURN max_flog = 0;
	object_type * o_ptr = &inventory[INVEN_LITE];

	/* It's a lamp */
	if ((o_ptr->tval == TV_LITE) && (o_ptr->sval == SV_LITE_LANTERN))
	{
		max_flog = FUEL_LAMP;
	}

	/* It's a torch */
	else if ((o_ptr->tval == TV_LITE) && (o_ptr->sval == SV_LITE_TORCH))
	{
		max_flog = FUEL_TORCH;
	}

	/* No torch to refill */
	else
	{
		msg_print(_("ǳ�Ǥ���񤹤륢���ƥ���������Ƥ��ޤ���", "You are not wielding anything which uses phlogiston."));
		return;
	}

	if (o_ptr->xtra4 >= max_flog)
	{
		msg_print(_("���Υ����ƥ�ˤϤ���ʾ�ǳ�Ǥ��佼�Ǥ��ޤ���", "No more phlogiston can be put in this item."));
		return;
	}

	/* Refuel */
	o_ptr->xtra4 += (XTRA16)(max_flog / 2);

	/* Message */
	msg_print(_("�����ѥ����ƥ��ǳ�Ǥ��佼������", "You add phlogiston to your light item."));

	/* Comment */
	if (o_ptr->xtra4 >= max_flog)
	{
		o_ptr->xtra4 = (XTRA16)max_flog;
		msg_print(_("�����ѥ����ƥ��������ˤʤä���", "Your light item is full."));
	}

	/* Recalculate torch */
	p_ptr->update |= (PU_TORCH);
}


/*!
 * @brief ���ؤΥ����ղý��� /
 * Brand the current weapon
 * @param brand_type ������ID(e_info.txt�Ȥ�Ϣư���Ƥ��ʤ�)
 * @return �ʤ�
 */
void brand_weapon(int brand_type)
{
	OBJECT_IDX item;
	object_type *o_ptr;
	cptr        q, s;


	/* Assume enchant weapon */
	item_tester_hook = object_allow_enchant_melee_weapon;
	item_tester_no_ryoute = TRUE;

	/* Get an item */
	q = _("�ɤ����򶯲����ޤ���? ", "Enchant which weapon? ");
	s = _("�����Ǥ�����郎�ʤ���", "You have nothing to enchant.");

	if (!get_item(&item, q, s, (USE_EQUIP))) return;

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


	/* you can never modify artifacts / ego-items */
	/* you can never modify cursed items */
	/* TY: You _can_ modify broken items (if you're silly enough) */
	if (o_ptr->k_idx && !object_is_artifact(o_ptr) && !object_is_ego(o_ptr) &&
	    !object_is_cursed(o_ptr) &&
	    !((o_ptr->tval == TV_SWORD) && (o_ptr->sval == SV_DOKUBARI)) &&
	    !((o_ptr->tval == TV_POLEARM) && (o_ptr->sval == SV_DEATH_SCYTHE)) &&
	    !((o_ptr->tval == TV_SWORD) && (o_ptr->sval == SV_DIAMOND_EDGE)))
	{
		cptr act = NULL;

		/* Let's get the name before it is changed... */
		char o_name[MAX_NLEN];
		object_desc(o_name, o_ptr, (OD_OMIT_PREFIX | OD_NAME_ONLY));

		switch (brand_type)
		{
		case 17:
			if (o_ptr->tval == TV_SWORD)
			{
				act = _("�ϱԤ�����������", "becomes very sharp!");

				o_ptr->name2 = EGO_SHARPNESS;
				o_ptr->pval = (PARAMETER_VALUE)m_bonus(5, dun_level) + 1;

				if ((o_ptr->sval == SV_HAYABUSA) && (o_ptr->pval > 2))
					o_ptr->pval = 2;
			}
			else
			{
				act = _("���˲��Ϥ���������", "seems very powerful.");
				o_ptr->name2 = EGO_EARTHQUAKES;
				o_ptr->pval = (PARAMETER_VALUE)m_bonus(3, dun_level);
			}
			break;
		case 16:
			act = _("�Ͽʹ֤η����Ƥ��롪", "seems to be looking for humans!");
			o_ptr->name2 = EGO_KILL_HUMAN;
			break;
		case 15:
			act = _("���ŷ��ʤ��줿��", "covered with lightning!");
			o_ptr->name2 = EGO_BRAND_ELEC;
			break;
		case 14:
			act = _("�ϻ���ʤ��줿��", "coated with acid!");
			o_ptr->name2 = EGO_BRAND_ACID;
			break;
		case 13:
			act = _("�ϼٰ��ʤ��ʪ����Ƥ��롪", "seems to be looking for evil monsters!");
			o_ptr->name2 = EGO_KILL_EVIL;
			break;
		case 12:
			act = _("�ϰ������ν��ͤ����Τ���Ƥ��롪", "seems to be looking for demons!");
			o_ptr->name2 = EGO_KILL_DEMON;
			break;
		case 11:
			act = _("�ϻӤ���Ƥ��롪", "seems to be looking for undead!");
			o_ptr->name2 = EGO_KILL_UNDEAD;
			break;
		case 10:
			act = _("��ưʪ�η����Ƥ��롪", "seems to be looking for animals!");
			o_ptr->name2 = EGO_KILL_ANIMAL;
			break;
		case 9:
			act = _("�ϥɥ饴��η����Ƥ��롪", "seems to be looking for dragons!");
			o_ptr->name2 = EGO_KILL_DRAGON;
			break;
		case 8:
			act = _("�ϥȥ��η����Ƥ��롪", "seems to be looking for troll!s");
			o_ptr->name2 = EGO_KILL_TROLL;
			break;
		case 7:
			act = _("�ϥ������η����Ƥ��롪", "seems to be looking for orcs!");
			o_ptr->name2 = EGO_KILL_ORC;
			break;
		case 6:
			act = _("�ϵ�ͤη����Ƥ��롪", "seems to be looking for giants!");
			o_ptr->name2 = EGO_KILL_GIANT;
			break;
		case 5:
			act = _("�������԰���ˤʤä��褦����", "seems very unstable now.");
			o_ptr->name2 = EGO_TRUMP;
			o_ptr->pval = randint1(2);
			break;
		case 4:
			act = _("�Ϸ����Ƥ��롪", "thirsts for blood!");
			o_ptr->name2 = EGO_VAMPIRIC;
			break;
		case 3:
			act = _("���Ǥ�ʤ��줿��", "is coated with poison.");
			o_ptr->name2 = EGO_BRAND_POIS;
			break;
		case 2:
			act = _("�Ͻ���륹�˰��߹��ޤ줿��", "is engulfed in raw Logrus!");
			o_ptr->name2 = EGO_CHAOTIC;
			break;
		case 1:
			act = _("�ϱ�Υ�����ɤ�ʤ��줿��", "is covered in a fiery shield!");
			o_ptr->name2 = EGO_BRAND_FIRE;
			break;
		default:
			act = _("�Ͽ����䤿���֥롼�˵�������", "glows deep, icy blue!");
			o_ptr->name2 = EGO_BRAND_COLD;
			break;
		}

		msg_format(_("���ʤ���%s%s", "Your %s %s"), o_name, act);
		enchant(o_ptr, randint0(3) + 4, ENCH_TOHIT | ENCH_TODAM);

		o_ptr->discount = 99;
		chg_virtue(V_ENCHANT, 2);
	}
	else
	{
		if (flush_failure) flush();

		msg_print(_("°���ղä˼��Ԥ�����", "The Branding failed."));
		chg_virtue(V_ENCHANT, -2);
	}
	calc_android_exp();
}


/*!
 * @brief ��̵����ˤ��ե�������ɽ������ /
 * Vanish all walls in this floor
 * @return �ºݤ˽�����ȿ�Ǥ��줿���TRUE
 */
static bool vanish_dungeon(void)
{
	int          y, x;
	cave_type    *c_ptr;
	feature_type *f_ptr;
	monster_type *m_ptr;
	char         m_name[80];

	/* Prevent vasishing of quest levels and town */
	if ((p_ptr->inside_quest && is_fixed_quest_idx(p_ptr->inside_quest)) || !dun_level)
	{
		return FALSE;
	}

	/* Scan all normal grids */
	for (y = 1; y < cur_hgt - 1; y++)
	{
		for (x = 1; x < cur_wid - 1; x++)
		{
			c_ptr = &cave[y][x];

			/* Seeing true feature code (ignore mimic) */
			f_ptr = &f_info[c_ptr->feat];

			/* Lose room and vault */
			c_ptr->info &= ~(CAVE_ROOM | CAVE_ICKY);

			m_ptr = &m_list[c_ptr->m_idx];

			/* Awake monster */
			if (c_ptr->m_idx && MON_CSLEEP(m_ptr))
			{
				/* Reset sleep counter */
				(void)set_monster_csleep(c_ptr->m_idx, 0);

				/* Notice the "waking up" */
				if (m_ptr->ml)
				{
					/* Acquire the monster name */
					monster_desc(m_name, m_ptr, 0);

					/* Dump a message */
					msg_format(_("%^s���ܤ�Фޤ�����", "%^s wakes up."), m_name);
				}
			}

			/* Process all walls, doors and patterns */
			if (have_flag(f_ptr->flags, FF_HURT_DISI)) cave_alter_feat(y, x, FF_HURT_DISI);
		}
	}

	/* Special boundary walls -- Top and bottom */
	for (x = 0; x < cur_wid; x++)
	{
		c_ptr = &cave[0][x];
		f_ptr = &f_info[c_ptr->mimic];

		/* Lose room and vault */
		c_ptr->info &= ~(CAVE_ROOM | CAVE_ICKY);

		/* Set boundary mimic if needed */
		if (c_ptr->mimic && have_flag(f_ptr->flags, FF_HURT_DISI))
		{
			c_ptr->mimic = feat_state(c_ptr->mimic, FF_HURT_DISI);

			/* Check for change to boring grid */
			if (!have_flag(f_info[c_ptr->mimic].flags, FF_REMEMBER)) c_ptr->info &= ~(CAVE_MARK);
		}

		c_ptr = &cave[cur_hgt - 1][x];
		f_ptr = &f_info[c_ptr->mimic];

		/* Lose room and vault */
		c_ptr->info &= ~(CAVE_ROOM | CAVE_ICKY);

		/* Set boundary mimic if needed */
		if (c_ptr->mimic && have_flag(f_ptr->flags, FF_HURT_DISI))
		{
			c_ptr->mimic = feat_state(c_ptr->mimic, FF_HURT_DISI);

			/* Check for change to boring grid */
			if (!have_flag(f_info[c_ptr->mimic].flags, FF_REMEMBER)) c_ptr->info &= ~(CAVE_MARK);
		}
	}

	/* Special boundary walls -- Left and right */
	for (y = 1; y < (cur_hgt - 1); y++)
	{
		c_ptr = &cave[y][0];
		f_ptr = &f_info[c_ptr->mimic];

		/* Lose room and vault */
		c_ptr->info &= ~(CAVE_ROOM | CAVE_ICKY);

		/* Set boundary mimic if needed */
		if (c_ptr->mimic && have_flag(f_ptr->flags, FF_HURT_DISI))
		{
			c_ptr->mimic = feat_state(c_ptr->mimic, FF_HURT_DISI);

			/* Check for change to boring grid */
			if (!have_flag(f_info[c_ptr->mimic].flags, FF_REMEMBER)) c_ptr->info &= ~(CAVE_MARK);
		}

		c_ptr = &cave[y][cur_wid - 1];
		f_ptr = &f_info[c_ptr->mimic];

		/* Lose room and vault */
		c_ptr->info &= ~(CAVE_ROOM | CAVE_ICKY);

		/* Set boundary mimic if needed */
		if (c_ptr->mimic && have_flag(f_ptr->flags, FF_HURT_DISI))
		{
			c_ptr->mimic = feat_state(c_ptr->mimic, FF_HURT_DISI);

			/* Check for change to boring grid */
			if (!have_flag(f_info[c_ptr->mimic].flags, FF_REMEMBER)) c_ptr->info &= ~(CAVE_MARK);
		}
	}

	/* Mega-Hack -- Forget the view and lite */
	p_ptr->update |= (PU_UN_VIEW | PU_UN_LITE);

	/* Update stuff */
	p_ptr->update |= (PU_VIEW | PU_LITE | PU_FLOW | PU_MON_LITE);

	/* Update the monsters */
	p_ptr->update |= (PU_MONSTERS);

	/* Redraw map */
	p_ptr->redraw |= (PR_MAP);

	/* Window stuff */
	p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);

	return TRUE;
}

/*!
 * @brief ��̵������� /
 * @return �ʤ�
 */
void call_the_(void)
{
	int i;
	cave_type *c_ptr;
	bool do_call = TRUE;

	for (i = 0; i < 9; i++)
	{
		c_ptr = &cave[p_ptr->y + ddy_ddd[i]][p_ptr->x + ddx_ddd[i]];

		if (!cave_have_flag_grid(c_ptr, FF_PROJECT))
		{
			if (!c_ptr->mimic || !have_flag(f_info[c_ptr->mimic].flags, FF_PROJECT) ||
			    !permanent_wall(&f_info[c_ptr->feat]))
			{
				do_call = FALSE;
				break;
			}
		}
	}

	if (do_call)
	{
		for (i = 1; i < 10; i++)
		{
			if (i - 5) fire_ball(GF_ROCKET, i, 175, 2);
		}

		for (i = 1; i < 10; i++)
		{
			if (i - 5) fire_ball(GF_MANA, i, 175, 3);
		}

		for (i = 1; i < 10; i++)
		{
			if (i - 5) fire_ball(GF_NUKE, i, 175, 4);
		}
	}

	/* Prevent destruction of quest levels and town */
	else if ((p_ptr->inside_quest && is_fixed_quest_idx(p_ptr->inside_quest)) || !dun_level)
	{
		msg_print(_("���̤��ɤ줿��", "The ground trembles."));
	}

	else
	{
#ifdef JP
		msg_format("���ʤ���%s���ɤ˶᤹������Ǿ����Ƥ��ޤä���",
			((mp_ptr->spell_book == TV_LIFE_BOOK) ? "����" : "��ʸ"));
		msg_print("�礭����ȯ�������ä���");
#else
		msg_format("You %s the %s too close to a wall!",
			((mp_ptr->spell_book == TV_LIFE_BOOK) ? "recite" : "cast"),
			((mp_ptr->spell_book == TV_LIFE_BOOK) ? "prayer" : "spell"));
		msg_print("There is a loud explosion!");
#endif

		if (one_in_(666))
		{
			if (!vanish_dungeon()) msg_print(_("���󥸥��ϰ���Ťޤ��֤ä���", "The dungeon silences a moment."));
		}
		else
		{
			if (destroy_area(p_ptr->y, p_ptr->x, 15 + p_ptr->lev + randint0(11), FALSE))
				msg_print(_("���󥸥����������...", "The dungeon collapses..."));
			else
				msg_print(_("���󥸥����礭���ɤ줿��", "The dungeon trembles."));
		}

		take_hit(DAMAGE_NOESCAPE, 100 + randint1(150), _("����Ū�ʵ�̵����", "a suicidal Call the Void"), -1);
	}
}


/*!
 * @brief �����ƥ�����󤻽��� /
 * Fetch an item (teleport it right underneath the caster)
 * @param dir ��ˡ��ȯư����
 * @param wgt ���ƽ���
 * @param require_los �������̤���׵᤹��ʤ��TRUE
 * @return �ʤ�
 */
void fetch(DIRECTION dir, WEIGHT wgt, bool require_los)
{
	POSITION ty, tx;
	OBJECT_IDX i;
	cave_type *c_ptr;
	object_type *o_ptr;
	char o_name[MAX_NLEN];

	/* Check to see if an object is already there */
	if (cave[p_ptr->y][p_ptr->x].o_idx)
	{
		msg_print(_("��ʬ��­�β��ˤ���ʪ�ϼ��ޤ���", "You can't fetch when you're already standing on something."));
		return;
	}

	/* Use a target */
	if (dir == 5 && target_okay())
	{
		tx = target_col;
		ty = target_row;

		if (distance(p_ptr->y, p_ptr->x, ty, tx) > MAX_RANGE)
		{
			msg_print(_("����ʤ˱󤯤ˤ���ʪ�ϼ��ޤ���", "You can't fetch something that far away!"));
			return;
		}

		c_ptr = &cave[ty][tx];

		/* We need an item to fetch */
		if (!c_ptr->o_idx)
		{
			msg_print(_("�����ˤϲ��⤢��ޤ���", "There is no object at this place."));
			return;
		}

		/* No fetching from vault */
		if (c_ptr->info & CAVE_ICKY)
		{
			msg_print(_("�����ƥब����ȥ���򳰤���������", "The item slips from your control."));
			return;
		}

		/* We need to see the item */
		if (require_los)
		{
			if (!player_has_los_bold(ty, tx))
			{
				msg_print(_("�����Ϥ��ʤ��λ볦�����äƤ��ޤ���", "You have no direct line of sight to that location."));
				return;
			}
			else if (!projectable(p_ptr->y, p_ptr->x, ty, tx))
			{
				msg_print(_("�������ɤθ������Ǥ���", "You have no direct line of sight to that location."));
				return;
			}
		}
	}
	else
	{
		/* Use a direction */
		ty = p_ptr->y; /* Where to drop the item */
		tx = p_ptr->x;

		do
		{
			ty += ddy[dir];
			tx += ddx[dir];
			c_ptr = &cave[ty][tx];

			if ((distance(p_ptr->y, p_ptr->x, ty, tx) > MAX_RANGE) ||
				!cave_have_flag_bold(ty, tx, FF_PROJECT)) return;
		}
		while (!c_ptr->o_idx);
	}

	o_ptr = &o_list[c_ptr->o_idx];

	if (o_ptr->weight > wgt)
	{
		/* Too heavy to 'fetch' */
		msg_print(_("���Υ����ƥ�ϽŲ᤮�ޤ���", "The object is too heavy."));
		return;
	}

	i = c_ptr->o_idx;
	c_ptr->o_idx = o_ptr->next_o_idx;
	cave[p_ptr->y][p_ptr->x].o_idx = i; /* 'move' it */

	o_ptr->next_o_idx = 0;
	o_ptr->iy = (byte)p_ptr->y;
	o_ptr->ix = (byte)p_ptr->x;

	object_desc(o_name, o_ptr, OD_NAME_ONLY);
	msg_format(_("%^s�����ʤ���­��������Ǥ�����", "%^s flies through the air to your feet."), o_name);

	note_spot(p_ptr->y, p_ptr->x);
	p_ptr->redraw |= PR_MAP;
}

/*!
 * @brief �������ƽ���
 * @return �ʤ�
 */
void alter_reality(void)
{
	/* Ironman option */
	if (p_ptr->inside_arena || ironman_downward)
	{
		msg_print(_("���ⵯ����ʤ��ä���", "Nothing happens."));
		return;
	}

	if (!p_ptr->alter_reality)
	{
		TIME_EFFECT turns = randint0(21) + 15;

		p_ptr->alter_reality = turns;
		msg_print(_("���ηʿ����Ѥ��Ϥ᤿...", "The view around you begins to change..."));

		p_ptr->redraw |= (PR_STATUS);
	}
	else
	{
		p_ptr->alter_reality = 0;
		msg_print(_("�ʿ���������ä�...", "The view around you got back..."));
		p_ptr->redraw |= (PR_STATUS);
	}
	return;
}


/*!
 * @brief ���Υ롼�����ֽ��� /
 * Leave a "glyph of warding" which prevents monster movement
 * @return �ºݤ����֤��Ԥ�줿���TRUE���֤�
 */
bool warding_glyph(void)
{
	/* XXX XXX XXX */
	if (!cave_clean_bold(p_ptr->y, p_ptr->x))
	{
		msg_print(_("����Υ����ƥब��ʸ��ķ���֤�����", "The object resists the spell."));
		return FALSE;
	}

	/* Create a glyph */
	cave[p_ptr->y][p_ptr->x].info |= CAVE_OBJECT;
	cave[p_ptr->y][p_ptr->x].mimic = feat_glyph;

	/* Notice */
	note_spot(p_ptr->y, p_ptr->x);

	/* Redraw */
	lite_spot(p_ptr->y, p_ptr->x);

	return TRUE;
}

/*!
 * @brief �����ֽ���
 * @return �ºݤ����֤��Ԥ�줿���TRUE���֤�
 */
bool place_mirror(void)
{
	/* XXX XXX XXX */
	if (!cave_clean_bold(p_ptr->y, p_ptr->x))
	{
		msg_print(_("����Υ����ƥब��ʸ��ķ���֤�����", "The object resists the spell."));
		return FALSE;
	}

	/* Create a mirror */
	cave[p_ptr->y][p_ptr->x].info |= CAVE_OBJECT;
	cave[p_ptr->y][p_ptr->x].mimic = feat_mirror;

	/* Turn on the light */
	cave[p_ptr->y][p_ptr->x].info |= CAVE_GLOW;

	/* Notice */
	note_spot(p_ptr->y, p_ptr->x);

	/* Redraw */
	lite_spot(p_ptr->y, p_ptr->x);

	update_local_illumination(p_ptr->y, p_ptr->x);

	return TRUE;
}


/*!
 * @brief ��ȯ�Υ롼�����ֽ��� /
 * Leave an "explosive rune" which prevents monster movement
 * @return �ºݤ����֤��Ԥ�줿���TRUE���֤�
 */
bool explosive_rune(void)
{
	/* XXX XXX XXX */
	if (!cave_clean_bold(p_ptr->y, p_ptr->x))
	{
		msg_print(_("����Υ����ƥब��ʸ��ķ���֤�����", "The object resists the spell."));
		return FALSE;
	}

	/* Create a glyph */
	cave[p_ptr->y][p_ptr->x].info |= CAVE_OBJECT;
	cave[p_ptr->y][p_ptr->x].mimic = feat_explosive_rune;

	/* Notice */
	note_spot(p_ptr->y, p_ptr->x);
	
	/* Redraw */
	lite_spot(p_ptr->y, p_ptr->x);

	return TRUE;
}


/*!
 * @brief ����������ƥ������� /
 * Identify everything being carried.
 * Done by a potion of "self knowledge".
 * @return �ʤ�
 */
void identify_pack(void)
{
	int i;

	/* Simply identify and know every item */
	for (i = 0; i < INVEN_TOTAL; i++)
	{
		object_type *o_ptr = &inventory[i];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Identify it */
		identify_item(o_ptr);

		/* Auto-inscription */
		autopick_alter_item(i, FALSE);
	}
}


/*!
 * @brief �������������μ���Ψ�������ʬΨ�� /
 * Used by the "enchant" function (chance of failure)
 * (modified for Zangband, we need better stuff there...) -- TY
 * @return �ʤ�
 */
static int enchant_table[16] =
{
	0, 10,  50, 100, 200,
	300, 400, 500, 650, 800,
	950, 987, 993, 995, 998,
	1000
};


/*!
 * @brief �����β������ /
 * Removes curses from items in inventory
 * @param all �ڤ������ޤǤβ���ʤ��0
 * @return ������줿�����ƥ�ο�
 * @details
 * <pre>
 * Note that Items which are "Perma-Cursed" (The One Ring,
 * The Crown of Morgoth) can NEVER be uncursed.
 *
 * Note that if "all" is FALSE, then Items which are
 * "Heavy-Cursed" (Mormegil, Calris, and Weapons of Morgul)
 * will not be uncursed.
 * </pre>
 */
static int remove_curse_aux(int all)
{
	int i, cnt = 0;

	/* Attempt to uncurse items being worn */
	for (i = INVEN_RARM; i < INVEN_TOTAL; i++)
	{
		object_type *o_ptr = &inventory[i];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Uncursed already */
		if (!object_is_cursed(o_ptr)) continue;

		/* Heavily Cursed Items need a special spell */
		if (!all && (o_ptr->curse_flags & TRC_HEAVY_CURSE)) continue;

		/* Perma-Cursed Items can NEVER be uncursed */
		if (o_ptr->curse_flags & TRC_PERMA_CURSE)
		{
			/* Uncurse it */
			o_ptr->curse_flags &= (TRC_CURSED | TRC_HEAVY_CURSE | TRC_PERMA_CURSE);
			continue;
		}

		/* Uncurse it */
		o_ptr->curse_flags = 0L;

		/* Hack -- Assume felt */
		o_ptr->ident |= (IDENT_SENSE);

		/* Take note */
		o_ptr->feeling = FEEL_NONE;

		/* Recalculate the bonuses */
		p_ptr->update |= (PU_BONUS);

		/* Window stuff */
		p_ptr->window |= (PW_EQUIP);

		/* Count the uncursings */
		cnt++;
	}

	/* Return "something uncursed" */
	return (cnt);
}


/*!
 * @brief �����ηڤ������������ /
 * Remove most curses
 * @return �������������������
 */
int remove_curse(void)
{
	return (remove_curse_aux(FALSE));
}

/*!
 * @brief �����νŤ������������ /
 * Remove all curses
 * @return �������������������
 */
int remove_all_curse(void)
{
	return (remove_curse_aux(TRUE));
}


/*!
 * @brief �����ƥ�β��ͤ˱�����ϣ��ѽ��� /
 * Turns an object into gold, gain some of its value in a shop
 * @return �������ºݤ˹Ԥ�줿��TRUE���֤�
 */
bool alchemy(void)
{
	OBJECT_IDX item;
	int amt = 1;
	ITEM_NUMBER old_number;
	PRICE price;
	bool force = FALSE;
	object_type *o_ptr;
	char o_name[MAX_NLEN];
	char out_val[MAX_NLEN+40];

	cptr q, s;

	/* Hack -- force destruction */
	if (command_arg > 0) force = TRUE;

	/* Get an item */
	q = _("�ɤΥ����ƥ�����Ѥ��ޤ�����", "Turn which item to gold? ");
	s = _("����Ѥ�����ʪ������ޤ���", "You have nothing to turn to gold.");

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


	/* See how many items */
	if (o_ptr->number > 1)
	{
		/* Get a quantity */
		amt = get_quantity(NULL, o_ptr->number);

		/* Allow user abort */
		if (amt <= 0) return FALSE;
	}


	/* Describe the object */
	old_number = o_ptr->number;
	o_ptr->number = amt;
	object_desc(o_name, o_ptr, 0);
	o_ptr->number = old_number;

	/* Verify unless quantity given */
	if (!force)
	{
		if (confirm_destroy || (object_value(o_ptr) > 0))
		{
			/* Make a verification */
			sprintf(out_val, _("������%s�����Ѥ��ޤ�����", "Really turn %s to gold? "), o_name);
			if (!get_check(out_val)) return FALSE;
		}
	}

	/* Artifacts cannot be destroyed */
	if (!can_player_destroy_object(o_ptr))
	{
		/* Message */
		msg_format(_("%s�����Ѥ��뤳�Ȥ˼��Ԥ�����", "You fail to turn %s to gold!"), o_name);

		/* Done */
		return FALSE;
	}

	price = object_value_real(o_ptr);

	if (price <= 0)
	{
		/* Message */
		msg_format(_("%s��˥��ζ���Ѥ�����", "You turn %s to fool's gold."), o_name);
	}
	else
	{
		price /= 3;

		if (amt > 1) price *= amt;

		if (price > 30000) price = 30000;
		msg_format(_("%s���%d �ζ���Ѥ�����", "You turn %s to %ld coins worth of gold."), o_name, price);

		p_ptr->au += price;

		/* Redraw gold */
		p_ptr->redraw |= (PR_GOLD);

		/* Window stuff */
		p_ptr->window |= (PW_PLAYER);

	}

	/* Eliminate the item (from the pack) */
	if (item >= 0)
	{
		inven_item_increase(item, -amt);
		inven_item_describe(item);
		inven_item_optimize(item);
	}

	/* Eliminate the item (from the floor) */
	else
	{
		floor_item_increase(0 - item, -amt);
		floor_item_describe(0 - item);
		floor_item_optimize(0 - item);
	}

	return TRUE;
}


/*!
 * @brief �������Ǥ��ˤ���� /
 * Break the curse of an item
 * @param o_ptr ������������λ��ȥݥ���
 * @return �ʤ�
 */
static void break_curse(object_type *o_ptr)
{
	if (object_is_cursed(o_ptr) && !(o_ptr->curse_flags & TRC_PERMA_CURSE) && !(o_ptr->curse_flags & TRC_HEAVY_CURSE) && (randint0(100) < 25))
	{
		msg_print(_("�������Ƥ����������Ǥ��ˤ�줿��", "The curse is broken!"));

		o_ptr->curse_flags = 0L;
		o_ptr->ident |= (IDENT_SENSE);
		o_ptr->feeling = FEEL_NONE;
	}
}


/*!
 * @brief ���������������� /
 * Enchants a plus onto an item. -RAK-
 * @param o_ptr �������륢���ƥ�λ��ȥݥ���
 * @param n ����������
 * @param eflag �������ץ����(̿��/���᡼��/AC)
 * @return �����������������TRUE���֤�
 * @details
 * <pre>
 * Revamped!  Now takes item pointer, number of times to try enchanting,
 * and a flag of what to try enchanting.  Artifacts resist enchantment
 * some of the time, and successful enchantment to at least +0 might
 * break a curse on the item. -CFT-
 *
 * Note that an item can technically be enchanted all the way to +15 if
 * you wait a very, very, long time.  Going from +9 to +10 only works
 * about 5% of the time, and from +10 to +11 only about 1% of the time.
 *
 * Note that this function can now be used on "piles" of items, and
 * the larger the pile, the lower the chance of success.
 * </pre>
 */
bool enchant(object_type *o_ptr, int n, int eflag)
{
	int     i, chance, prob;
	bool    res = FALSE;
	bool    a = object_is_artifact(o_ptr);
	bool    force = (eflag & ENCH_FORCE);


	/* Large piles resist enchantment */
	prob = o_ptr->number * 100;

	/* Missiles are easy to enchant */
	if ((o_ptr->tval == TV_BOLT) ||
	    (o_ptr->tval == TV_ARROW) ||
	    (o_ptr->tval == TV_SHOT))
	{
		prob = prob / 20;
	}

	/* Try "n" times */
	for (i = 0; i < n; i++)
	{
		/* Hack -- Roll for pile resistance */
		if (!force && randint0(prob) >= 100) continue;

		/* Enchant to hit */
		if (eflag & ENCH_TOHIT)
		{
			if (o_ptr->to_h < 0) chance = 0;
			else if (o_ptr->to_h > 15) chance = 1000;
			else chance = enchant_table[o_ptr->to_h];

			if (force || ((randint1(1000) > chance) && (!a || (randint0(100) < 50))))
			{
				o_ptr->to_h++;
				res = TRUE;

				/* only when you get it above -1 -CFT */
				if (o_ptr->to_h >= 0)
					break_curse(o_ptr);
			}
		}

		/* Enchant to damage */
		if (eflag & ENCH_TODAM)
		{
			if (o_ptr->to_d < 0) chance = 0;
			else if (o_ptr->to_d > 15) chance = 1000;
			else chance = enchant_table[o_ptr->to_d];

			if (force || ((randint1(1000) > chance) && (!a || (randint0(100) < 50))))
			{
				o_ptr->to_d++;
				res = TRUE;

				/* only when you get it above -1 -CFT */
				if (o_ptr->to_d >= 0)
					break_curse(o_ptr);
			}
		}

		/* Enchant to armor class */
		if (eflag & ENCH_TOAC)
		{
			if (o_ptr->to_a < 0) chance = 0;
			else if (o_ptr->to_a > 15) chance = 1000;
			else chance = enchant_table[o_ptr->to_a];

			if (force || ((randint1(1000) > chance) && (!a || (randint0(100) < 50))))
			{
				o_ptr->to_a++;
				res = TRUE;

				/* only when you get it above -1 -CFT */
				if (o_ptr->to_a >= 0)
					break_curse(o_ptr);
			}
		}
	}

	/* Failure */
	if (!res) return (FALSE);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);

	calc_android_exp();

	/* Success */
	return (TRUE);
}


/*!
 * @brief �����������������Υᥤ��롼���� /
 * Enchant an item (in the inventory or on the floor)
 * @param num_hit ̿�潤����
 * @param num_dam ���᡼��������
 * @param num_ac AC������
 * @return �����������������TRUE���֤�
 * @details
 * Note that "num_ac" requires armour, else weapon
 * Returns TRUE if attempted, FALSE if cancelled
 */
bool enchant_spell(HIT_PROB num_hit, HIT_POINT num_dam, ARMOUR_CLASS num_ac)
{
	OBJECT_IDX item;
	bool        okay = FALSE;
	object_type *o_ptr;
	char        o_name[MAX_NLEN];
	cptr        q, s;


	/* Assume enchant weapon */
	item_tester_hook = object_allow_enchant_weapon;
	item_tester_no_ryoute = TRUE;

	/* Enchant armor if requested */
	if (num_ac) item_tester_hook = object_is_armour;

	/* Get an item */
	q = _("�ɤΥ����ƥ�򶯲����ޤ���? ", "Enchant which item? ");
	s = _("�����Ǥ��륢���ƥब�ʤ���", "You have nothing to enchant.");

	if (!get_item(&item, q, s, (USE_EQUIP | USE_INVEN | USE_FLOOR))) return (FALSE);

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


	/* Description */
	object_desc(o_name, o_ptr, (OD_OMIT_PREFIX | OD_NAME_ONLY));

	/* Describe */
#ifdef JP
msg_format("%s �����뤯��������",
    o_name);
#else
	msg_format("%s %s glow%s brightly!",
		   ((item >= 0) ? "Your" : "The"), o_name,
		   ((o_ptr->number > 1) ? "" : "s"));
#endif


	/* Enchant */
	if (enchant(o_ptr, num_hit, ENCH_TOHIT)) okay = TRUE;
	if (enchant(o_ptr, num_dam, ENCH_TODAM)) okay = TRUE;
	if (enchant(o_ptr, num_ac, ENCH_TOAC)) okay = TRUE;

	/* Failure */
	if (!okay)
	{
		/* Flush */
		if (flush_failure) flush();

		/* Message */
		msg_print(_("�����˼��Ԥ�����", "The enchantment failed."));

		if (one_in_(3)) chg_virtue(V_ENCHANT, -1);
	}
	else
		chg_virtue(V_ENCHANT, 1);

	calc_android_exp();

	/* Something happened */
	return (TRUE);
}


/*!
 * @brief �����ƥब�¤β��ͤΥ����ƥफ�ɤ���Ƚ�ꤹ�� /
 * Check if an object is nameless weapon or armour
 * @param o_ptr Ƚ�ꤹ�륢���ƥ�ξ��󻲾ȥݥ���
 * @return �¤ʤ��TRUE���֤�
 */
static bool item_tester_hook_nameless_weapon_armour(object_type *o_ptr)
{
	/* Require weapon or armour */
	if (!object_is_weapon_armour_ammo(o_ptr)) return FALSE;
	
	/* Require nameless object if the object is well known */
	if (object_is_known(o_ptr) && !object_is_nameless(o_ptr))
		return FALSE;

	return TRUE;
}

/*!
 * @brief �����ƥ��ե����������δ�ʪ���� /
 * @return �������ºݤ˻�ߤ�줿��TRUE���֤�
 */
bool artifact_scroll(void)
{
	OBJECT_IDX item;
	bool            okay = FALSE;
	object_type     *o_ptr;
	char            o_name[MAX_NLEN];
	cptr            q, s;


	item_tester_no_ryoute = TRUE;

	/* Enchant weapon/armour */
	item_tester_hook = item_tester_hook_nameless_weapon_armour;

	/* Get an item */
	q = _("�ɤΥ����ƥ�򶯲����ޤ���? ", "Enchant which item? ");
	s = _("�����Ǥ��륢���ƥब�ʤ���", "You have nothing to enchant.");

	if (!get_item(&item, q, s, (USE_EQUIP | USE_INVEN | USE_FLOOR))) return (FALSE);

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


	/* Description */
	object_desc(o_name, o_ptr, (OD_OMIT_PREFIX | OD_NAME_ONLY));

	/* Describe */
#ifdef JP
	msg_format("%s ����������ȯ������",o_name);
#else
	msg_format("%s %s radiate%s a blinding light!",
		  ((item >= 0) ? "Your" : "The"), o_name,
		  ((o_ptr->number > 1) ? "" : "s"));
#endif

	if (object_is_artifact(o_ptr))
	{
#ifdef JP
		msg_format("%s�ϴ�������Υ����ƥ�Ǥ���", o_name  );
#else
		msg_format("The %s %s already %s!",
		    o_name, ((o_ptr->number > 1) ? "are" : "is"),
		    ((o_ptr->number > 1) ? "artifacts" : "an artifact"));
#endif

		okay = FALSE;
	}

	else if (object_is_ego(o_ptr))
	{
#ifdef JP
		msg_format("%s�ϴ���̾�Τ��륢���ƥ�Ǥ���", o_name );
#else
		msg_format("The %s %s already %s!",
		    o_name, ((o_ptr->number > 1) ? "are" : "is"),
		    ((o_ptr->number > 1) ? "ego items" : "an ego item"));
#endif

		okay = FALSE;
	}

	else if (o_ptr->xtra3)
	{
#ifdef JP
		msg_format("%s�ϴ��˶�������Ƥ��ޤ���", o_name );
#else
		msg_format("The %s %s already %s!",
		    o_name, ((o_ptr->number > 1) ? "are" : "is"),
		    ((o_ptr->number > 1) ? "customized items" : "a customized item"));
#endif
	}

	else
	{
		if (o_ptr->number > 1)
		{
#ifdef JP
			msg_print("ʣ���Υ����ƥ����ˡ�򤫤�������Υ��ͥ륮���Ϥ���ޤ���");
			msg_format("%d �Ĥ�%s�����줿��",(o_ptr->number)-1, o_name);
#else
			msg_print("Not enough enough energy to enchant more than one object!");
			msg_format("%d of your %s %s destroyed!",(o_ptr->number)-1, o_name, (o_ptr->number>2?"were":"was"));
#endif

			if (item >= 0)
			{
				inven_item_increase(item, 1-(o_ptr->number));
			}
			else
			{
				floor_item_increase(0-item, 1-(o_ptr->number));
			}
		}
		okay = create_artifact(o_ptr, TRUE);
	}

	/* Failure */
	if (!okay)
	{
		/* Flush */
		if (flush_failure) flush();

		/* Message */
		msg_print(_("�����˼��Ԥ�����", "The enchantment failed."));

		if (one_in_(3)) chg_virtue(V_ENCHANT, -1);
	}
	else
	{
		if (record_rand_art)
		{
			/* Description */
			object_desc(o_name, o_ptr, OD_NAME_ONLY);
			do_cmd_write_nikki(NIKKI_ART_SCROLL, 0, o_name);
		}
		chg_virtue(V_ENCHANT, 1);
	}

	calc_android_exp();

	/* Something happened */
	return (TRUE);
}


/*!
 * @brief �����ƥ������� /
 * Identify an object
 * @param o_ptr ���ꤵ��륢���ƥ�ξ��󻲾ȥݥ���
 * @return �ºݤ˴���Ǥ�����TRUE���֤�
 */
bool identify_item(object_type *o_ptr)
{
	bool old_known = FALSE;
	char o_name[MAX_NLEN];

	/* Description */
	object_desc(o_name, o_ptr, 0);

	if (o_ptr->ident & IDENT_KNOWN)
		old_known = TRUE;

	if (!(o_ptr->ident & (IDENT_MENTAL)))
	{
		if (object_is_artifact(o_ptr) || one_in_(5))
			chg_virtue(V_KNOWLEDGE, 1);
	}

	/* Identify it fully */
	object_aware(o_ptr);
	object_known(o_ptr);

	/* Player touches it */
	o_ptr->marked |= OM_TOUCHED;

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);

	strcpy(record_o_name, o_name);
	record_turn = turn;

	/* Description */
	object_desc(o_name, o_ptr, OD_NAME_ONLY);

	if(record_fix_art && !old_known && object_is_fixed_artifact(o_ptr))
		do_cmd_write_nikki(NIKKI_ART, 0, o_name);
	if(record_rand_art && !old_known && o_ptr->art_name)
		do_cmd_write_nikki(NIKKI_ART, 0, o_name);

	return old_known;
}

/*!
 * @brief �����ƥब����Ѥߤ���Ƚ�ꤹ�� /
 * @param o_ptr Ƚ�ꤹ�륢���ƥ�ξ��󻲾ȥݥ���
 * @return �ºݤ˴���Ѥߤʤ��TRUE���֤�
 */
static bool item_tester_hook_identify(object_type *o_ptr)
{
	return (bool)!object_is_known(o_ptr);
}

/*!
 * @brief �����ƥब����Ѥߤ�����ɶ񤫤�Ƚ�ꤹ�� /
 * @param o_ptr Ƚ�ꤹ�륢���ƥ�ξ��󻲾ȥݥ���
 * @return �ºݤ˴���Ѥߤʤ��TRUE���֤�
 */
static bool item_tester_hook_identify_weapon_armour(object_type *o_ptr)
{
	if (object_is_known(o_ptr))
		return FALSE;
	return object_is_weapon_armour_ammo(o_ptr);
}

/*!
 * @brief �����ƥ����Υᥤ��롼������� /
 * Identify an object in the inventory (or on the floor)
 * @param only_equip �����ʤΤߤ��оݤȤ���ʤ��TRUE���֤�
 * @return �ºݤ˴����Ԥä��ʤ��TRUE���֤�
 * @details
 * This routine does *not* automatically combine objects.
 * Returns TRUE if something was identified, else FALSE.
 */
bool ident_spell(bool only_equip)
{
	OBJECT_IDX item;
	object_type     *o_ptr;
	char            o_name[MAX_NLEN];
	cptr            q, s;
	bool old_known;

	item_tester_no_ryoute = TRUE;

	if (only_equip)
		item_tester_hook = item_tester_hook_identify_weapon_armour;
	else
		item_tester_hook = item_tester_hook_identify;

	if (can_get_item())
	{
		q = _("�ɤΥ����ƥ����ꤷ�ޤ���? ", "Identify which item? ");
	}
	else
	{
		if (only_equip)
			item_tester_hook = object_is_weapon_armour_ammo;
		else
			item_tester_hook = NULL;

		q = _("���٤ƴ���ѤߤǤ��� ", "All items are identified. ");
	}

	/* Get an item */
	s = _("���ꤹ��٤������ƥब�ʤ���", "You have nothing to identify.");

	if (!get_item(&item, q, s, (USE_EQUIP | USE_INVEN | USE_FLOOR))) return (FALSE);

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

	/* Identify it */
	old_known = identify_item(o_ptr);

	/* Description */
	object_desc(o_name, o_ptr, 0);

	/* Describe */
	if (item >= INVEN_RARM)
	{
		msg_format(_("%^s: %s(%c)��", "%^s: %s (%c)."), describe_use(item), o_name, index_to_label(item));
	}
	else if (item >= 0)
	{
		msg_format(_("���å���: %s(%c)��", "In your pack: %s (%c)."), o_name, index_to_label(item));
	}
	else
	{
		msg_format(_("����: %s��", "On the ground: %s."), o_name);
	}

	/* Auto-inscription/destroy */
	autopick_alter_item(item, (bool)(destroy_identify && !old_known));

	/* Something happened */
	return (TRUE);
}


/*!
 * @brief �����ƥ����ǲ��Υᥤ��롼������� /
 * Identify an object in the inventory (or on the floor)
 * @param only_equip �����ʤΤߤ��оݤȤ���ʤ��TRUE���֤�
 * @return �ºݤ����ǲ����Ԥä��ʤ��TRUE���֤�
 * @details
 * <pre>
 * Mundanify an object in the inventory (or on the floor)
 * This routine does *not* automatically combine objects.
 * Returns TRUE if something was mundanified, else FALSE.
 * </pre>
 */
bool mundane_spell(bool only_equip)
{
	OBJECT_IDX item;
	object_type     *o_ptr;
	cptr            q, s;

	if (only_equip) item_tester_hook = object_is_weapon_armour_ammo;
	item_tester_no_ryoute = TRUE;

	/* Get an item */
	q = _("�ɤ��Ȥ��ޤ�����", "Use which item? ");
	s = _("�Ȥ����Τ�����ޤ���", "You have nothing you can use.");

	if (!get_item(&item, q, s, (USE_EQUIP | USE_INVEN | USE_FLOOR))) return (FALSE);

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

	/* Oops */
	msg_print(_("�ޤФ椤���������ä���", "There is a bright flash of light!"));
	{
		POSITION iy = o_ptr->iy;                 /* Y-position on map, or zero */
		POSITION ix = o_ptr->ix;                 /* X-position on map, or zero */
		s16b next_o_idx = o_ptr->next_o_idx; /* Next object in stack (if any) */
		byte marked = o_ptr->marked;         /* Object is marked */
		s16b weight = o_ptr->number * o_ptr->weight;
		u16b inscription = o_ptr->inscription;

		/* Wipe it clean */
		object_prep(o_ptr, o_ptr->k_idx);

		o_ptr->iy = iy;
		o_ptr->ix = ix;
		o_ptr->next_o_idx = next_o_idx;
		o_ptr->marked = marked;
		o_ptr->inscription = inscription;
		if (item >= 0) p_ptr->total_weight += (o_ptr->weight - weight);
	}
	calc_android_exp();

	/* Something happened */
	return TRUE;
}

/*!
 * @brief �����ƥब*����*�Ѥߤ���Ƚ�ꤹ�� /
 * @param o_ptr Ƚ�ꤹ�륢���ƥ�ξ��󻲾ȥݥ���
 * @return �ºݤ˴���Ѥߤʤ��TRUE���֤�
 */
static bool item_tester_hook_identify_fully(object_type *o_ptr)
{
	return (bool)(!object_is_known(o_ptr) || !(o_ptr->ident & IDENT_MENTAL));
}

/*!
 * @brief �����ƥब*����*�Ѥߤ�����ɶ񤫤�Ƚ�ꤹ�� /
 * @param o_ptr Ƚ�ꤹ�륢���ƥ�ξ��󻲾ȥݥ���
 * @return �ºݤ˴���Ѥߤʤ��TRUE���֤�
 */
static bool item_tester_hook_identify_fully_weapon_armour(object_type *o_ptr)
{
	if (!item_tester_hook_identify_fully(o_ptr))
		return FALSE;
	return object_is_weapon_armour_ammo(o_ptr);
}

/*!
 * @brief �����ƥ�*����*�Υᥤ��롼������� /
 * Identify an object in the inventory (or on the floor)
 * @param only_equip �����ʤΤߤ��оݤȤ���ʤ��TRUE���֤�
 * @return �ºݤ˴����Ԥä��ʤ��TRUE���֤�
 * @details
 * Fully "identify" an object in the inventory  -BEN-
 * This routine returns TRUE if an item was identified.
 */
bool identify_fully(bool only_equip)
{
	OBJECT_IDX item;
	object_type     *o_ptr;
	char            o_name[MAX_NLEN];
	cptr            q, s;
	bool old_known;

	item_tester_no_ryoute = TRUE;
	if (only_equip)
		item_tester_hook = item_tester_hook_identify_fully_weapon_armour;
	else
		item_tester_hook = item_tester_hook_identify_fully;

	if (can_get_item())
	{
		q = _("�ɤΥ����ƥ��*����*���ޤ���? ", "*Identify* which item? ");
	}
	else
	{
		if (only_equip)
			item_tester_hook = object_is_weapon_armour_ammo;
		else
			item_tester_hook = NULL;

		q = _("���٤�*����*�ѤߤǤ��� ", "All items are *identified*. ");
	}

	/* Get an item */
	s = _("*����*����٤������ƥब�ʤ���", "You have nothing to *identify*.");

	if (!get_item(&item, q, s, (USE_EQUIP | USE_INVEN | USE_FLOOR))) return (FALSE);

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

	/* Identify it */
	old_known = identify_item(o_ptr);

	/* Mark the item as fully known */
	o_ptr->ident |= (IDENT_MENTAL);

	/* Handle stuff */
	handle_stuff();

	/* Description */
	object_desc(o_name, o_ptr, 0);

	/* Describe */
	if (item >= INVEN_RARM)
	{
		msg_format(_("%^s: %s(%c)��", "%^s: %s (%c)."), describe_use(item), o_name, index_to_label(item));
	}
	else if (item >= 0)
	{
		msg_format(_("���å���: %s(%c)��", "In your pack: %s (%c)."), o_name, index_to_label(item));
	}
	else
	{
		msg_format(_("����: %s��", "On the ground: %s."), o_name);
	}

	/* Describe it fully */
	(void)screen_object(o_ptr, 0L);

	/* Auto-inscription/destroy */
	autopick_alter_item(item, (bool)(destroy_identify && !old_known));

	/* Success */
	return (TRUE);
}


/*!
 * @brief ���Ͻ�Ŷ����ǽ�ʥ����ƥफ�ɤ���Ƚ�ꤹ�� /
 * Hook for "get_item()".  Determine if something is rechargable.
 * @param o_ptr Ƚ�ꤹ�륢���ƥ�ξ��󻲾ȥݥ���
 * @return ���Ͻ�Ŷ����ǽ�ʤ��TRUE���֤�
 */
bool item_tester_hook_recharge(object_type *o_ptr)
{
	/* Recharge staffs */
	if (o_ptr->tval == TV_STAFF) return (TRUE);

	/* Recharge wands */
	if (o_ptr->tval == TV_WAND) return (TRUE);

	/* Hack -- Recharge rods */
	if (o_ptr->tval == TV_ROD) return (TRUE);

	/* Nope */
	return (FALSE);
}


/*!
 * @brief ���Ͻ�Ŷ���� /
 * Recharge a wand/staff/rod from the pack or on the floor.
 * This function has been rewritten in Oangband and ZAngband.
 * @param power ��Ŷ�ѥ
 * @return �����������פ�������ޤǿʤ����TRUE���֤�
 *
 * Sorcery/Arcane -- Recharge  --> recharge(plev * 4)
 * Chaos -- Arcane Binding     --> recharge(90)
 *
 * Scroll of recharging        --> recharge(130)
 * Artifact activation/Thingol --> recharge(130)
 *
 * It is harder to recharge high level, and highly charged wands,
 * staffs, and rods.  The more wands in a stack, the more easily and
 * strongly they recharge.  Staffs, however, each get fewer charges if
 * stacked.
 *
 * XXX XXX XXX Beware of "sliding index errors".
 */
bool recharge(int power)
{
	OBJECT_IDX item;
	int lev;
	int recharge_strength;
	TIME_EFFECT recharge_amount;

	object_type *o_ptr;
	object_kind *k_ptr;

	bool fail = FALSE;
	byte fail_type = 1;

	cptr q, s;
	char o_name[MAX_NLEN];

	/* Only accept legal items */
	item_tester_hook = item_tester_hook_recharge;

	/* Get an item */
	q = _("�ɤΥ����ƥ�����Ϥ�Ŷ���ޤ���? ", "Recharge which item? ");
	s = _("���Ϥ�Ŷ���٤������ƥब�ʤ���", "You have nothing to recharge.");

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

	/* Get the object kind. */
	k_ptr = &k_info[o_ptr->k_idx];

	/* Extract the object "level" */
	lev = k_info[o_ptr->k_idx].level;


	/* Recharge a rod */
	if (o_ptr->tval == TV_ROD)
	{
		/* Extract a recharge strength by comparing object level to power. */
		recharge_strength = ((power > lev/2) ? (power - lev/2) : 0) / 5;


		/* Back-fire */
		if (one_in_(recharge_strength))
		{
			/* Activate the failure code. */
			fail = TRUE;
		}

		/* Recharge */
		else
		{
			/* Recharge amount */
			recharge_amount = (power * damroll(3, 2));

			/* Recharge by that amount */
			if (o_ptr->timeout > recharge_amount)
				o_ptr->timeout -= recharge_amount;
			else
				o_ptr->timeout = 0;
		}
	}


	/* Recharge wand/staff */
	else
	{
		/* Extract a recharge strength by comparing object level to power.
		 * Divide up a stack of wands' charges to calculate charge penalty.
		 */
		if ((o_ptr->tval == TV_WAND) && (o_ptr->number > 1))
			recharge_strength = (100 + power - lev -
			(8 * o_ptr->pval / o_ptr->number)) / 15;

		/* All staffs, unstacked wands. */
		else recharge_strength = (100 + power - lev -
			(8 * o_ptr->pval)) / 15;

		/* Paranoia */
		if (recharge_strength < 0) recharge_strength = 0;

		/* Back-fire */
		if (one_in_(recharge_strength))
		{
			/* Activate the failure code. */
			fail = TRUE;
		}

		/* If the spell didn't backfire, recharge the wand or staff. */
		else
		{
			/* Recharge based on the standard number of charges. */
			recharge_amount = randint1(1 + k_ptr->pval / 2);

			/* Multiple wands in a stack increase recharging somewhat. */
			if ((o_ptr->tval == TV_WAND) && (o_ptr->number > 1))
			{
				recharge_amount +=
					(randint1(recharge_amount * (o_ptr->number - 1))) / 2;
				if (recharge_amount < 1) recharge_amount = 1;
				if (recharge_amount > 12) recharge_amount = 12;
			}

			/* But each staff in a stack gets fewer additional charges,
			 * although always at least one.
			 */
			if ((o_ptr->tval == TV_STAFF) && (o_ptr->number > 1))
			{
				recharge_amount /= (TIME_EFFECT)o_ptr->number;
				if (recharge_amount < 1) recharge_amount = 1;
			}

			/* Recharge the wand or staff. */
			o_ptr->pval += recharge_amount;


			/* Hack -- we no longer "know" the item */
			o_ptr->ident &= ~(IDENT_KNOWN);

			/* Hack -- we no longer think the item is empty */
			o_ptr->ident &= ~(IDENT_EMPTY);
		}
	}


	/* Inflict the penalties for failing a recharge. */
	if (fail)
	{
		/* Artifacts are never destroyed. */
		if (object_is_fixed_artifact(o_ptr))
		{
			object_desc(o_name, o_ptr, OD_NAME_ONLY);
			msg_format(_("���Ϥ���ή������%s�ϴ��������Ϥ򼺤ä���", "The recharging backfires - %s is completely drained!"), o_name);

			/* Artifact rods. */
			if ((o_ptr->tval == TV_ROD) && (o_ptr->timeout < 10000))
				o_ptr->timeout = (o_ptr->timeout + 100) * 2;

			/* Artifact wands and staffs. */
			else if ((o_ptr->tval == TV_WAND) || (o_ptr->tval == TV_STAFF))
				o_ptr->pval = 0;
		}
		else
		{
			/* Get the object description */
			object_desc(o_name, o_ptr, (OD_OMIT_PREFIX | OD_NAME_ONLY));

			/*** Determine Seriousness of Failure ***/

			/* Mages recharge objects more safely. */
			if (p_ptr->pclass == CLASS_MAGE || p_ptr->pclass == CLASS_HIGH_MAGE || p_ptr->pclass == CLASS_SORCERER || p_ptr->pclass == CLASS_MAGIC_EATER || p_ptr->pclass == CLASS_BLUE_MAGE)
			{
				/* 10% chance to blow up one rod, otherwise draining. */
				if (o_ptr->tval == TV_ROD)
				{
					if (one_in_(10)) fail_type = 2;
					else fail_type = 1;
				}
				/* 75% chance to blow up one wand, otherwise draining. */
				else if (o_ptr->tval == TV_WAND)
				{
					if (!one_in_(3)) fail_type = 2;
					else fail_type = 1;
				}
				/* 50% chance to blow up one staff, otherwise no effect. */
				else if (o_ptr->tval == TV_STAFF)
				{
					if (one_in_(2)) fail_type = 2;
					else fail_type = 0;
				}
			}

			/* All other classes get no special favors. */
			else
			{
				/* 33% chance to blow up one rod, otherwise draining. */
				if (o_ptr->tval == TV_ROD)
				{
					if (one_in_(3)) fail_type = 2;
					else fail_type = 1;
				}
				/* 20% chance of the entire stack, else destroy one wand. */
				else if (o_ptr->tval == TV_WAND)
				{
					if (one_in_(5)) fail_type = 3;
					else fail_type = 2;
				}
				/* Blow up one staff. */
				else if (o_ptr->tval == TV_STAFF)
				{
					fail_type = 2;
				}
			}

			/*** Apply draining and destruction. ***/

			/* Drain object or stack of objects. */
			if (fail_type == 1)
			{
				if (o_ptr->tval == TV_ROD)
				{
					msg_print(_("���Ϥ���ʮ�ͤ��ơ���åɤ��餵������Ϥ�ۤ���äƤ��ޤä���", "The recharge backfires, draining the rod further!"));

					if (o_ptr->timeout < 10000)
						o_ptr->timeout = (o_ptr->timeout + 100) * 2;
				}
				else if (o_ptr->tval == TV_WAND)
				{
					msg_format(_("%s����»���Ȥ줿�������Ϥ����Ƽ���줿��", "You save your %s from destruction, but all charges are lost."), o_name);
					o_ptr->pval = 0;
				}
				/* Staffs aren't drained. */
			}

			/* Destroy an object or one in a stack of objects. */
			if (fail_type == 2)
			{
				if (o_ptr->number > 1)
					msg_format(_("��˽����ˡ�Τ����%s�����ܲ��줿��", "Wild magic consumes one of your %s!"), o_name);
				else
					msg_format(_("��˽����ˡ�Τ����%s�����줿��", "Wild magic consumes your %s!"), o_name);

				/* Reduce rod stack maximum timeout, drain wands. */
				if (o_ptr->tval == TV_ROD) o_ptr->timeout = (o_ptr->number - 1) * k_ptr->pval;
				if (o_ptr->tval == TV_WAND) o_ptr->pval = 0;

				/* Reduce and describe inventory */
				if (item >= 0)
				{
					inven_item_increase(item, -1);
					inven_item_describe(item);
					inven_item_optimize(item);
				}

				/* Reduce and describe floor item */
				else
				{
					floor_item_increase(0 - item, -1);
					floor_item_describe(0 - item);
					floor_item_optimize(0 - item);
				}
			}

			/* Destroy all members of a stack of objects. */
			if (fail_type == 3)
			{
				if (o_ptr->number > 1)
					msg_format(_("��˽����ˡ�Τ����%s�����Ʋ��줿��", "Wild magic consumes all your %s!"), o_name);
				else
					msg_format(_("��˽����ˡ�Τ����%s�����줿��", "Wild magic consumes your %s!"), o_name);

				/* Reduce and describe inventory */
				if (item >= 0)
				{
					inven_item_increase(item, -999);
					inven_item_describe(item);
					inven_item_optimize(item);
				}

				/* Reduce and describe floor item */
				else
				{
					floor_item_increase(0 - item, -999);
					floor_item_describe(0 - item);
					floor_item_optimize(0 - item);
				}
			}
		}
	}

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Window stuff */
	p_ptr->window |= (PW_INVEN);

	/* Something was done */
	return (TRUE);
}


/*!
 * @brief ���ν�ʡ���� /
 * Bless a weapon
 * @return �����������פ��������Ԥä��ʤ��TRUE���֤�
 */
bool bless_weapon(void)
{
	OBJECT_IDX item;
	object_type     *o_ptr;
	BIT_FLAGS flgs[TR_FLAG_SIZE];
	char            o_name[MAX_NLEN];
	cptr            q, s;

	item_tester_no_ryoute = TRUE;

	/* Bless only weapons */
	item_tester_hook = object_is_weapon;

	/* Get an item */
	q = _("�ɤΥ����ƥ���ʡ���ޤ�����", "Bless which weapon? ");
	s = _("��ʡ�Ǥ�����郎����ޤ���", "You have weapon to bless.");

	if (!get_item(&item, q, s, (USE_EQUIP | USE_INVEN | USE_FLOOR)))
		return FALSE;

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


	/* Description */
	object_desc(o_name, o_ptr, (OD_OMIT_PREFIX | OD_NAME_ONLY));

	/* Extract the flags */
	object_flags(o_ptr, flgs);

	if (object_is_cursed(o_ptr))
	{
		if (((o_ptr->curse_flags & TRC_HEAVY_CURSE) && (randint1(100) < 33)) ||
			have_flag(flgs, TR_ADD_L_CURSE) ||
			have_flag(flgs, TR_ADD_H_CURSE) ||
		    (o_ptr->curse_flags & TRC_PERMA_CURSE))
		{
#ifdef JP
msg_format("%s��ʤ������������Ͻ�ʡ��ķ���֤�����",
    o_name);
#else
			msg_format("The black aura on %s %s disrupts the blessing!",
			    ((item >= 0) ? "your" : "the"), o_name);
#endif

			return TRUE;
		}

#ifdef JP
msg_format("%s ����ٰ��ʥ����餬�ä�����",
    o_name);
#else
		msg_format("A malignant aura leaves %s %s.",
		    ((item >= 0) ? "your" : "the"), o_name);
#endif


		/* Uncurse it */
		o_ptr->curse_flags = 0L;

		/* Hack -- Assume felt */
		o_ptr->ident |= (IDENT_SENSE);

		/* Take note */
		o_ptr->feeling = FEEL_NONE;

		/* Recalculate the bonuses */
		p_ptr->update |= (PU_BONUS);

		/* Window stuff */
		p_ptr->window |= (PW_EQUIP);
	}

	/*
	 * Next, we try to bless it. Artifacts have a 1/3 chance of
	 * being blessed, otherwise, the operation simply disenchants
	 * them, godly power negating the magic. Ok, the explanation
	 * is silly, but otherwise priests would always bless every
	 * artifact weapon they find. Ego weapons and normal weapons
	 * can be blessed automatically.
	 */
	if (have_flag(flgs, TR_BLESSED))
	{
#ifdef JP
msg_format("%s �ϴ��˽�ʡ����Ƥ��롣",
    o_name    );
#else
		msg_format("%s %s %s blessed already.",
		    ((item >= 0) ? "Your" : "The"), o_name,
		    ((o_ptr->number > 1) ? "were" : "was"));
#endif

		return TRUE;
	}

	if (!(object_is_artifact(o_ptr) || object_is_ego(o_ptr)) || one_in_(3))
	{
		/* Describe */
#ifdef JP
msg_format("%s�ϵ�������",
     o_name);
#else
		msg_format("%s %s shine%s!",
		    ((item >= 0) ? "Your" : "The"), o_name,
		    ((o_ptr->number > 1) ? "" : "s"));
#endif

		add_flag(o_ptr->art_flags, TR_BLESSED);
		o_ptr->discount = 99;
	}
	else
	{
		bool dis_happened = FALSE;
		msg_print(_("�������Ͻ�ʡ����äƤ��롪", "The weapon resists your blessing!"));

		/* Disenchant tohit */
		if (o_ptr->to_h > 0)
		{
			o_ptr->to_h--;
			dis_happened = TRUE;
		}

		if ((o_ptr->to_h > 5) && (randint0(100) < 33)) o_ptr->to_h--;

		/* Disenchant todam */
		if (o_ptr->to_d > 0)
		{
			o_ptr->to_d--;
			dis_happened = TRUE;
		}

		if ((o_ptr->to_d > 5) && (randint0(100) < 33)) o_ptr->to_d--;

		/* Disenchant toac */
		if (o_ptr->to_a > 0)
		{
			o_ptr->to_a--;
			dis_happened = TRUE;
		}

		if ((o_ptr->to_a > 5) && (randint0(100) < 33)) o_ptr->to_a--;

		if (dis_happened)
		{
			msg_print(_("���Ϥ����Ǥ�ʷ�ϵ���������...", "There is a static feeling in the air..."));

#ifdef JP
msg_format("%s ������������",
     o_name    );
#else
			msg_format("%s %s %s disenchanted!",
			    ((item >= 0) ? "Your" : "The"), o_name,
			    ((o_ptr->number > 1) ? "were" : "was"));
#endif

		}
	}

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Window stuff */
	p_ptr->window |= (PW_EQUIP | PW_PLAYER);

	calc_android_exp();

	return TRUE;
}


/*!
 * @brief ���᤭���� /
 * pulish shield
 * @return �����������פ��������Ԥä��ʤ��TRUE���֤�
 */
bool pulish_shield(void)
{
	OBJECT_IDX item;
	object_type     *o_ptr;
	u32b flgs[TR_FLAG_SIZE];
	char            o_name[MAX_NLEN];
	cptr            q, s;

	item_tester_no_ryoute = TRUE;
	/* Assume enchant weapon */
	item_tester_tval = TV_SHIELD;

	/* Get an item */
	q = _("�ɤν���᤭�ޤ�����", "Pulish which weapon? ");
	s = _("�᤯�⤬����ޤ���", "You have weapon to pulish.");

	if (!get_item(&item, q, s, (USE_EQUIP | USE_INVEN | USE_FLOOR)))
		return FALSE;

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


	/* Description */
	object_desc(o_name, o_ptr, (OD_OMIT_PREFIX | OD_NAME_ONLY));

	/* Extract the flags */
	object_flags(o_ptr, flgs);

	if (o_ptr->k_idx && !object_is_artifact(o_ptr) && !object_is_ego(o_ptr) &&
	    !object_is_cursed(o_ptr) && (o_ptr->sval != SV_MIRROR_SHIELD))
	{
#ifdef JP
msg_format("%s�ϵ�������", o_name);
#else
		msg_format("%s %s shine%s!",
		    ((item >= 0) ? "Your" : "The"), o_name,
		    ((o_ptr->number > 1) ? "" : "s"));
#endif
		o_ptr->name2 = EGO_REFLECTION;
		enchant(o_ptr, randint0(3) + 4, ENCH_TOAC);

		o_ptr->discount = 99;
		chg_virtue(V_ENCHANT, 2);

		return TRUE;
	}
	else
	{
		if (flush_failure) flush();

		msg_print(_("���Ԥ�����", "Failed."));
		chg_virtue(V_ENCHANT, -2);
	}
	calc_android_exp();

	return FALSE;
}


/*!
 * @brief ������»���̽��� /
 * Potions "smash open" and cause an area effect when
 * @param who ����»�μ���ID(�ץ쥤�䡼��������ƥब���줿���0������Υ����ƥ�ξ���󥹥���ID)
 * @param y �˲�����Y��ɸ
 * @param x �˲�����X��ɸ
 * @param k_idx ��»�������Υ����ƥ�ID
 * @return ������Ӥ���󥹥�����������ʤ��TRUE���֤�
 * @details
 * <pre>
 * (1) they are shattered while in the player's inventory,
 * due to cold (etc) attacks;
 * (2) they are thrown at a monster, or obstacle;
 * (3) they are shattered by a "cold ball" or other such spell
 * while lying on the floor.
 *
 * Arguments:
 *    who   ---  who caused the potion to shatter (0=player)
 *          potions that smash on the floor are assumed to
 *          be caused by no-one (who = 1), as are those that
 *          shatter inside the player inventory.
 *          (Not anymore -- I changed this; TY)
 *    y, x  --- coordinates of the potion (or player if
 *          the potion was in her inventory);
 *    o_ptr --- pointer to the potion object.
 * </pre>
 */
bool potion_smash_effect(MONSTER_IDX who, POSITION y, POSITION x, KIND_OBJECT_IDX k_idx)
{
	int     radius = 2;
	int     dt = 0;
	int     dam = 0;
	bool    angry = FALSE;

	object_kind *k_ptr = &k_info[k_idx];

	switch (k_ptr->sval)
	{
		case SV_POTION_SALT_WATER:
		case SV_POTION_SLIME_MOLD:
		case SV_POTION_LOSE_MEMORIES:
		case SV_POTION_DEC_STR:
		case SV_POTION_DEC_INT:
		case SV_POTION_DEC_WIS:
		case SV_POTION_DEC_DEX:
		case SV_POTION_DEC_CON:
		case SV_POTION_DEC_CHR:
		case SV_POTION_WATER:   /* perhaps a 'water' attack? */
		case SV_POTION_APPLE_JUICE:
			return TRUE;

		case SV_POTION_INFRAVISION:
		case SV_POTION_DETECT_INVIS:
		case SV_POTION_SLOW_POISON:
		case SV_POTION_CURE_POISON:
		case SV_POTION_BOLDNESS:
		case SV_POTION_RESIST_HEAT:
		case SV_POTION_RESIST_COLD:
		case SV_POTION_HEROISM:
		case SV_POTION_BESERK_STRENGTH:
		case SV_POTION_RES_STR:
		case SV_POTION_RES_INT:
		case SV_POTION_RES_WIS:
		case SV_POTION_RES_DEX:
		case SV_POTION_RES_CON:
		case SV_POTION_RES_CHR:
		case SV_POTION_INC_STR:
		case SV_POTION_INC_INT:
		case SV_POTION_INC_WIS:
		case SV_POTION_INC_DEX:
		case SV_POTION_INC_CON:
		case SV_POTION_INC_CHR:
		case SV_POTION_AUGMENTATION:
		case SV_POTION_ENLIGHTENMENT:
		case SV_POTION_STAR_ENLIGHTENMENT:
		case SV_POTION_SELF_KNOWLEDGE:
		case SV_POTION_EXPERIENCE:
		case SV_POTION_RESISTANCE:
		case SV_POTION_INVULNERABILITY:
		case SV_POTION_NEW_LIFE:
			/* All of the above potions have no effect when shattered */
			return FALSE;
		case SV_POTION_SLOWNESS:
			dt = GF_OLD_SLOW;
			dam = 5;
			angry = TRUE;
			break;
		case SV_POTION_POISON:
			dt = GF_POIS;
			dam = 3;
			angry = TRUE;
			break;
		case SV_POTION_BLINDNESS:
			dt = GF_DARK;
			angry = TRUE;
			break;
		case SV_POTION_CONFUSION: /* Booze */
			dt = GF_OLD_CONF;
			angry = TRUE;
			break;
		case SV_POTION_SLEEP:
			dt = GF_OLD_SLEEP;
			angry = TRUE;
			break;
		case SV_POTION_RUINATION:
		case SV_POTION_DETONATIONS:
			dt = GF_SHARDS;
			dam = damroll(25, 25);
			angry = TRUE;
			break;
		case SV_POTION_DEATH:
			dt = GF_DEATH_RAY;    /* !! */
			dam = k_ptr->level * 10;
			angry = TRUE;
			radius = 1;
			break;
		case SV_POTION_SPEED:
			dt = GF_OLD_SPEED;
			break;
		case SV_POTION_CURE_LIGHT:
			dt = GF_OLD_HEAL;
			dam = damroll(2, 3);
			break;
		case SV_POTION_CURE_SERIOUS:
			dt = GF_OLD_HEAL;
			dam = damroll(4, 3);
			break;
		case SV_POTION_CURE_CRITICAL:
		case SV_POTION_CURING:
			dt = GF_OLD_HEAL;
			dam = damroll(6, 3);
			break;
		case SV_POTION_HEALING:
			dt = GF_OLD_HEAL;
			dam = damroll(10, 10);
			break;
		case SV_POTION_RESTORE_EXP:
			dt = GF_STAR_HEAL;
			dam = 0;
			radius = 1;
			break;
		case SV_POTION_LIFE:
			dt = GF_STAR_HEAL;
			dam = damroll(50, 50);
			radius = 1;
			break;
		case SV_POTION_STAR_HEALING:
			dt = GF_OLD_HEAL;
			dam = damroll(50, 50);
			radius = 1;
			break;
		case SV_POTION_RESTORE_MANA:   /* MANA */
			dt = GF_MANA;
			dam = damroll(10, 10);
			radius = 1;
			break;
		default:
			/* Do nothing */  ;
	}

	(void)project(who, radius, y, x, dam, dt,
	    (PROJECT_JUMP | PROJECT_ITEM | PROJECT_KILL), -1);

	/* XXX  those potions that explode need to become "known" */
	return angry;
}


/*!
 * @brief �ץ쥤�䡼�������μ�ʸ��ɽ������ /
 * Hack -- Display all known spells in a window
 * return �ʤ�
 * @details
 * XXX XXX XXX Need to analyze size of the window.
 * XXX XXX XXX Need more color coding.
 */
void display_spell_list(void)
{
	int             i, j;
	int             y, x;
	int             m[9];
	const magic_type *s_ptr;
	char            name[80];
	char            out_val[160];


	/* Erase window */
	clear_from(0);

	/* They have too many spells to list */
	if (p_ptr->pclass == CLASS_SORCERER) return;
	if (p_ptr->pclass == CLASS_RED_MAGE) return;

	/* Snipers */
	if (p_ptr->pclass == CLASS_SNIPER)
	{
		display_snipe_list();
		return;
	}

	/* mind.c type classes */
	if ((p_ptr->pclass == CLASS_MINDCRAFTER) ||
	    (p_ptr->pclass == CLASS_BERSERKER) ||
	    (p_ptr->pclass == CLASS_NINJA) ||
	    (p_ptr->pclass == CLASS_MIRROR_MASTER) ||
	    (p_ptr->pclass == CLASS_FORCETRAINER))
	{
		int             minfail = 0;
		int             plev = p_ptr->lev;
		int             chance = 0;
		mind_type       spell;
		char            comment[80];
		char            psi_desc[80];
		int             use_mind;
		bool use_hp = FALSE;

		y = 1;
		x = 1;

		/* Display a list of spells */
		prt("", y, x);
		put_str(_("̾��", "Name"), y, x + 5);
		put_str(_("Lv   MP ��Ψ ����", "Lv Mana Fail Info"), y, x + 35);

		switch(p_ptr->pclass)
		{
		case CLASS_MINDCRAFTER: use_mind = MIND_MINDCRAFTER;break;
		case CLASS_FORCETRAINER:          use_mind = MIND_KI;break;
		case CLASS_BERSERKER: use_mind = MIND_BERSERKER; use_hp = TRUE; break;
		case CLASS_MIRROR_MASTER: use_mind = MIND_MIRROR_MASTER; break;
		case CLASS_NINJA: use_mind = MIND_NINJUTSU; use_hp = TRUE; break;
		default:                use_mind = 0;break;
		}

		/* Dump the spells */
		for (i = 0; i < MAX_MIND_POWERS; i++)
		{
			byte a = TERM_WHITE;

			/* Access the available spell */
			spell = mind_powers[use_mind].info[i];
			if (spell.min_lev > plev) break;

			/* Get the failure rate */
			chance = spell.fail;

			/* Reduce failure rate by "effective" level adjustment */
			chance -= 3 * (p_ptr->lev - spell.min_lev);

			/* Reduce failure rate by INT/WIS adjustment */
			chance -= 3 * (adj_mag_stat[p_ptr->stat_ind[mp_ptr->spell_stat]] - 1);

			if (!use_hp)
			{
				/* Not enough mana to cast */
				if (spell.mana_cost > p_ptr->csp)
				{
					chance += 5 * (spell.mana_cost - p_ptr->csp);
					a = TERM_ORANGE;
				}
			}
			else
			{
				/* Not enough hp to cast */
				if (spell.mana_cost > p_ptr->chp)
				{
					chance += 100;
					a = TERM_RED;
				}
			}

			/* Extract the minimum failure rate */
			minfail = adj_mag_fail[p_ptr->stat_ind[mp_ptr->spell_stat]];

			/* Minimum failure rate */
			if (chance < minfail) chance = minfail;

			/* Stunning makes spells harder */
			if (p_ptr->stun > 50) chance += 25;
			else if (p_ptr->stun) chance += 15;

			/* Always a 5 percent chance of working */
			if (chance > 95) chance = 95;

			/* Get info */
			mindcraft_info(comment, use_mind, i);

			/* Dump the spell */
			sprintf(psi_desc, "  %c) %-30s%2d %4d %3d%%%s",
			    I2A(i), spell.name,
			    spell.min_lev, spell.mana_cost, chance, comment);

			Term_putstr(x, y + i + 1, -1, a, psi_desc);
		}
		return;
	}

	/* Cannot read spellbooks */
	if (REALM_NONE == p_ptr->realm1) return;

	/* Normal spellcaster with books */

	/* Scan books */
	for (j = 0; j < ((p_ptr->realm2 > REALM_NONE) ? 2 : 1); j++)
	{
		int n = 0;

		/* Reset vertical */
		m[j] = 0;

		/* Vertical location */
		y = (j < 3) ? 0 : (m[j - 3] + 2);

		/* Horizontal location */
		x = 27 * (j % 3);

		/* Scan spells */
		for (i = 0; i < 32; i++)
		{
			byte a = TERM_WHITE;

			/* Access the spell */
			if (!is_magic((j < 1) ? p_ptr->realm1 : p_ptr->realm2))
			{
				s_ptr = &technic_info[((j < 1) ? p_ptr->realm1 : p_ptr->realm2) - MIN_TECHNIC][i % 32];
			}
			else
			{
				s_ptr = &mp_ptr->info[((j < 1) ? p_ptr->realm1 : p_ptr->realm2) - 1][i % 32];
			}

			strcpy(name, do_spell((j < 1) ? p_ptr->realm1 : p_ptr->realm2, i % 32, SPELL_NAME));

			/* Illegible */
			if (s_ptr->slevel >= 99)
			{
				/* Illegible */
				strcpy(name, _("(Ƚ����ǽ)", "(illegible)"));

				/* Unusable */
				a = TERM_L_DARK;
			}

			/* Forgotten */
			else if ((j < 1) ?
				((p_ptr->spell_forgotten1 & (1L << i))) :
				((p_ptr->spell_forgotten2 & (1L << (i % 32)))))
			{
				/* Forgotten */
				a = TERM_ORANGE;
			}

			/* Unknown */
			else if (!((j < 1) ?
				(p_ptr->spell_learned1 & (1L << i)) :
				(p_ptr->spell_learned2 & (1L << (i % 32)))))
			{
				/* Unknown */
				a = TERM_RED;
			}

			/* Untried */
			else if (!((j < 1) ?
				(p_ptr->spell_worked1 & (1L << i)) :
				(p_ptr->spell_worked2 & (1L << (i % 32)))))
			{
				/* Untried */
				a = TERM_YELLOW;
			}

			/* Dump the spell --(-- */
			sprintf(out_val, "%c/%c) %-20.20s",
				I2A(n / 8), I2A(n % 8), name);

			/* Track maximum */
			m[j] = y + n;

			/* Dump onto the window */
			Term_putstr(x, m[j], -1, a, out_val);

			/* Next */
			n++;
		}
	}
}


/*!
 * @brief ��ʸ�ηи��ͤ��֤� /
 * Returns experience of a spell
 * @param spell ��ʸID
 * @param use_realm ��ˡ�ΰ�
 * @return �и���
 */
s16b experience_of_spell(int spell, int use_realm)
{
	if (p_ptr->pclass == CLASS_SORCERER) return SPELL_EXP_MASTER;
	else if (p_ptr->pclass == CLASS_RED_MAGE) return SPELL_EXP_SKILLED;
	else if (use_realm == p_ptr->realm1) return p_ptr->spell_exp[spell];
	else if (use_realm == p_ptr->realm2) return p_ptr->spell_exp[spell + 32];
	else return 0;
}


/*!
 * @brief ��ʸ�ξ���MP���֤� /
 * Modify mana consumption rate using spell exp and p_ptr->dec_mana
 * @param need_mana ���ܾ���MP
 * @param spell ��ʸID
 * @param realm ��ˡ�ΰ�
 * @return ����MP
 */
int mod_need_mana(int need_mana, int spell, int realm)
{
#define MANA_CONST   2400
#define MANA_DIV        4
#define DEC_MANA_DIV    3

	/* Realm magic */
	if ((realm > REALM_NONE) && (realm <= MAX_REALM))
	{
		/*
		 * need_mana defaults if spell exp equals SPELL_EXP_EXPERT and !p_ptr->dec_mana.
		 * MANA_CONST is used to calculate need_mana effected from spell proficiency.
		 */
		need_mana = need_mana * (MANA_CONST + SPELL_EXP_EXPERT - experience_of_spell(spell, realm)) + (MANA_CONST - 1);
		need_mana *= p_ptr->dec_mana ? DEC_MANA_DIV : MANA_DIV;
		need_mana /= MANA_CONST * MANA_DIV;
		if (need_mana < 1) need_mana = 1;
	}

	/* Non-realm magic */
	else
	{
		if (p_ptr->dec_mana) need_mana = (need_mana + 1) * DEC_MANA_DIV / MANA_DIV;
	}

#undef DEC_MANA_DIV
#undef MANA_DIV
#undef MANA_CONST

	return need_mana;
}


/*!
 * @brief ��ʸ�μ���Ψ��������1(�������������ϸ�������ʸ�ʰײ�) /
 * Modify spell fail rate
 * Using p_ptr->to_m_chance, p_ptr->dec_mana, p_ptr->easy_spell and p_ptr->heavy_spell
 * @param chance ����������Ψ
 * @return ����Ψ(%)
 * @todo �����Ƥ
 */
PERCENTAGE mod_spell_chance_1(PERCENTAGE chance)
{
	chance += p_ptr->to_m_chance;

	if (p_ptr->heavy_spell) chance += 20;

	if (p_ptr->dec_mana && p_ptr->easy_spell) chance -= 4;
	else if (p_ptr->easy_spell) chance -= 3;
	else if (p_ptr->dec_mana) chance -= 2;

	return chance;
}


/*!
 * @brief ��ʸ�μ���Ψ��������2(�������ϸ��������������ͽ���) /
 * Modify spell fail rate
 * Using p_ptr->to_m_chance, p_ptr->dec_mana, p_ptr->easy_spell and p_ptr->heavy_spell
 * @param chance ����������Ψ
 * @return ����Ψ(%)
 * Modify spell fail rate (as "suffix" process)
 * Using p_ptr->dec_mana, p_ptr->easy_spell and p_ptr->heavy_spell
 * Note: variable "chance" cannot be negative.
 * @todo �����Ƥ
 */
PERCENTAGE mod_spell_chance_2(PERCENTAGE chance)
{
	if (p_ptr->dec_mana) chance--;

	if (p_ptr->heavy_spell) chance += 5;

	return MAX(chance, 0);
}


/*!
 * @brief ��ʸ�μ���Ψ�׻��ᥤ��롼���� /
 * Returns spell chance of failure for spell -RAK-
 * @param spell ��ʸID
 * @param use_realm ��ˡ�ΰ�ID
 * @return ����Ψ(%)
 */
PERCENTAGE spell_chance(SPELL_IDX spell, REALM_IDX use_realm)
{
	PERCENTAGE chance, minfail;
	const magic_type *s_ptr;
	MANA_POINT need_mana;
	PERCENTAGE penalty = (mp_ptr->spell_stat == A_WIS) ? 10 : 4;


	/* Paranoia -- must be literate */
	if (!mp_ptr->spell_book) return (100);

	if (use_realm == REALM_HISSATSU) return 0;

	/* Access the spell */
	if (!is_magic(use_realm))
	{
		s_ptr = &technic_info[use_realm - MIN_TECHNIC][spell];
	}
	else
	{
		s_ptr = &mp_ptr->info[use_realm - 1][spell];
	}

	/* Extract the base spell failure rate */
	chance = s_ptr->sfail;

	/* Reduce failure rate by "effective" level adjustment */
	chance -= 3 * (p_ptr->lev - s_ptr->slevel);

	/* Reduce failure rate by INT/WIS adjustment */
	chance -= 3 * (adj_mag_stat[p_ptr->stat_ind[mp_ptr->spell_stat]] - 1);

	if (p_ptr->riding)
		chance += (MAX(r_info[m_list[p_ptr->riding].r_idx].level - p_ptr->skill_exp[GINOU_RIDING] / 100 - 10, 0));

	/* Extract mana consumption rate */
	need_mana = mod_need_mana(s_ptr->smana, spell, use_realm);

	/* Not enough mana to cast */
	if (need_mana > p_ptr->csp)
	{
		chance += 5 * (need_mana - p_ptr->csp);
	}

	if ((use_realm != p_ptr->realm1) && ((p_ptr->pclass == CLASS_MAGE) || (p_ptr->pclass == CLASS_PRIEST))) chance += 5;

	/* Extract the minimum failure rate */
	minfail = adj_mag_fail[p_ptr->stat_ind[mp_ptr->spell_stat]];

	/*
	 * Non mage/priest characters never get too good
	 * (added high mage, mindcrafter)
	 */
	if (mp_ptr->spell_xtra & MAGIC_FAIL_5PERCENT)
	{
		if (minfail < 5) minfail = 5;
	}

	/* Hack -- Priest prayer penalty for "edged" weapons  -DGK */
	if (((p_ptr->pclass == CLASS_PRIEST) || (p_ptr->pclass == CLASS_SORCERER)) && p_ptr->icky_wield[0]) chance += 25;
	if (((p_ptr->pclass == CLASS_PRIEST) || (p_ptr->pclass == CLASS_SORCERER)) && p_ptr->icky_wield[1]) chance += 25;

	chance = mod_spell_chance_1(chance);

	/* Goodness or evilness gives a penalty to failure rate */
	switch (use_realm)
	{
	case REALM_NATURE:
		if ((p_ptr->align > 50) || (p_ptr->align < -50)) chance += penalty;
		break;
	case REALM_LIFE: case REALM_CRUSADE:
		if (p_ptr->align < -20) chance += penalty;
		break;
	case REALM_DEATH: case REALM_DAEMON: case REALM_HEX:
		if (p_ptr->align > 20) chance += penalty;
		break;
	}

	/* Minimum failure rate */
	if (chance < minfail) chance = minfail;

	/* Stunning makes spells harder */
	if (p_ptr->stun > 50) chance += 25;
	else if (p_ptr->stun) chance += 15;

	/* Always a 5 percent chance of working */
	if (chance > 95) chance = 95;

	if ((use_realm == p_ptr->realm1) || (use_realm == p_ptr->realm2)
	    || (p_ptr->pclass == CLASS_SORCERER) || (p_ptr->pclass == CLASS_RED_MAGE))
	{
		s16b exp = experience_of_spell(spell, use_realm);
		if (exp >= SPELL_EXP_EXPERT) chance--;
		if (exp >= SPELL_EXP_MASTER) chance--;
	}

	/* Return the chance */
	return mod_spell_chance_2(chance);
}


/*!
 * @brief ��ˡ�����Ѳ�ǽ���ɤ������֤� /
 * Determine if a spell is "okay" for the player to cast or study
 * The spell must be legible, not forgotten, and also, to cast,
 * it must be known, and to study, it must not be known.
 * @param spell ��ʸID
 * @param learned ���Ѳ�ǽ��Ƚ��ʤ��TRUE���ؽ���ǽ���ɤ�����Ƚ��ʤ��FALSE
 * @param study_pray ����γؽ�Ƚ����Ū�ʤ��TRUE
 * @param use_realm ��ˡ�ΰ�ID
 * @return ����Ψ(%)
 */
bool spell_okay(int spell, bool learned, bool study_pray, int use_realm)
{
	const magic_type *s_ptr;

	/* Access the spell */
	if (!is_magic(use_realm))
	{
		s_ptr = &technic_info[use_realm - MIN_TECHNIC][spell];
	}
	else
	{
		s_ptr = &mp_ptr->info[use_realm - 1][spell];
	}

	/* Spell is illegal */
	if (s_ptr->slevel > p_ptr->lev) return (FALSE);

	/* Spell is forgotten */
	if ((use_realm == p_ptr->realm2) ?
	    (p_ptr->spell_forgotten2 & (1L << spell)) :
	    (p_ptr->spell_forgotten1 & (1L << spell)))
	{
		/* Never okay */
		return (FALSE);
	}

	if (p_ptr->pclass == CLASS_SORCERER) return (TRUE);
	if (p_ptr->pclass == CLASS_RED_MAGE) return (TRUE);

	/* Spell is learned */
	if ((use_realm == p_ptr->realm2) ?
	    (p_ptr->spell_learned2 & (1L << spell)) :
	    (p_ptr->spell_learned1 & (1L << spell)))
	{
		/* Always true */
		return (!study_pray);
	}

	/* Okay to study, not to cast */
	return (!learned);
}



/*!
 * @brief ��ʸ�����ɽ������ /
 * Print a list of spells (for browsing or casting or viewing)
 * @param target_spell ��ʸID		    
 * @param spells ɽ�����륹�ڥ�ID����λ��ȥݥ���
 * @param num ɽ�����륹�ڥ�ο�(spells�����ǿ�)
 * @param y ɽ����å���������Y��ɸ
 * @param x ɽ����å���������X��ɸ
 * @param use_realm ��ˡ�ΰ�ID
 * @return �ʤ�
 */
void print_spells(SPELL_IDX target_spell, SPELL_IDX *spells, int num, TERM_POSITION y, TERM_POSITION x, REALM_IDX use_realm)
{
	int             i, spell, exp_level, increment = 64;
	const magic_type *s_ptr;
	cptr            comment;
	char            info[80];
	char            out_val[160];
	byte            line_attr;
	int             need_mana;
	char            ryakuji[5];
	char            buf[256];
	bool max = FALSE;


	if (((use_realm <= REALM_NONE) || (use_realm > MAX_REALM)) && p_ptr->wizard)
	msg_print(_("�ٹ� print_spell ���ΰ�ʤ��˸ƤФ줿", "Warning! print_spells called with null realm"));

	/* Title the list */
	prt("", y, x);
	if (use_realm == REALM_HISSATSU)
		strcpy(buf,_("  Lv   MP", "  Lv   SP"));
	else
		strcpy(buf,_("������ Lv   MP ��Ψ ����", "Profic Lv   SP Fail Effect"));

	put_str(_("̾��", "Name"), y, x + 5);
	put_str(buf, y, x + 29);

	if ((p_ptr->pclass == CLASS_SORCERER) || (p_ptr->pclass == CLASS_RED_MAGE)) increment = 0;
	else if (use_realm == p_ptr->realm1) increment = 0;
	else if (use_realm == p_ptr->realm2) increment = 32;

	/* Dump the spells */
	for (i = 0; i < num; i++)
	{
		/* Access the spell */
		spell = spells[i];

		/* Access the spell */
		if (!is_magic(use_realm))
		{
			s_ptr = &technic_info[use_realm - MIN_TECHNIC][spell];
		}
		else
		{
			s_ptr = &mp_ptr->info[use_realm - 1][spell];
		}

		if (use_realm == REALM_HISSATSU)
			need_mana = s_ptr->smana;
		else
		{
			s16b exp = experience_of_spell(spell, use_realm);

			/* Extract mana consumption rate */
			need_mana = mod_need_mana(s_ptr->smana, spell, use_realm);

			if ((increment == 64) || (s_ptr->slevel >= 99)) exp_level = EXP_LEVEL_UNSKILLED;
			else exp_level = spell_exp_level(exp);

			max = FALSE;
			if (!increment && (exp_level == EXP_LEVEL_MASTER)) max = TRUE;
			else if ((increment == 32) && (exp_level >= EXP_LEVEL_EXPERT)) max = TRUE;
			else if (s_ptr->slevel >= 99) max = TRUE;
			else if ((p_ptr->pclass == CLASS_RED_MAGE) && (exp_level >= EXP_LEVEL_SKILLED)) max = TRUE;

			strncpy(ryakuji, exp_level_str[exp_level], 4);
			ryakuji[3] = ']';
			ryakuji[4] = '\0';
		}

		if (use_menu && target_spell)
		{
			if (i == (target_spell-1))
				strcpy(out_val, _("  �� ", "  >  "));
			else
				strcpy(out_val, "     ");
		}
		else sprintf(out_val, "  %c) ", I2A(i));
		/* Skip illegible spells */
		if (s_ptr->slevel >= 99)
		{
			strcat(out_val, format("%-30s", _("(Ƚ����ǽ)", "(illegible)")));
			c_prt(TERM_L_DARK, out_val, y + i + 1, x);
			continue;
		}

		/* XXX XXX Could label spells above the players level */

		/* Get extra info */
		strcpy(info, do_spell(use_realm, spell, SPELL_INFO));

		/* Use that info */
		comment = info;

		/* Assume spell is known and tried */
		line_attr = TERM_WHITE;

		/* Analyze the spell */
		if ((p_ptr->pclass == CLASS_SORCERER) || (p_ptr->pclass == CLASS_RED_MAGE))
		{
			if (s_ptr->slevel > p_ptr->max_plv)
			{
				comment = _("̤��", "unknown");
				line_attr = TERM_L_BLUE;
			}
			else if (s_ptr->slevel > p_ptr->lev)
			{
				comment = _("˺��", "forgotten");
				line_attr = TERM_YELLOW;
			}
		}
		else if ((use_realm != p_ptr->realm1) && (use_realm != p_ptr->realm2))
		{
			comment = _("̤��", "unknown");
			line_attr = TERM_L_BLUE;
		}
		else if ((use_realm == p_ptr->realm1) ?
		    ((p_ptr->spell_forgotten1 & (1L << spell))) :
		    ((p_ptr->spell_forgotten2 & (1L << spell))))
		{
			comment = _("˺��", "forgotten");
			line_attr = TERM_YELLOW;
		}
		else if (!((use_realm == p_ptr->realm1) ?
		    (p_ptr->spell_learned1 & (1L << spell)) :
		    (p_ptr->spell_learned2 & (1L << spell))))
		{
			comment = _("̤��", "unknown");
			line_attr = TERM_L_BLUE;
		}
		else if (!((use_realm == p_ptr->realm1) ?
		    (p_ptr->spell_worked1 & (1L << spell)) :
		    (p_ptr->spell_worked2 & (1L << spell))))
		{
			comment = _("̤�и�", "untried");
			line_attr = TERM_L_GREEN;
		}

		/* Dump the spell --(-- */
		if (use_realm == REALM_HISSATSU)
		{
			strcat(out_val, format("%-25s %2d %4d",
			    do_spell(use_realm, spell, SPELL_NAME), /* realm, spell */
			    s_ptr->slevel, need_mana));
		}
		else
		{
			strcat(out_val, format("%-25s%c%-4s %2d %4d %3d%% %s",
			    do_spell(use_realm, spell, SPELL_NAME), /* realm, spell */
			    (max ? '!' : ' '), ryakuji,
			    s_ptr->slevel, need_mana, spell_chance(spell, use_realm), comment));
		}
		c_prt(line_attr, out_val, y + i + 1, x);
	}

	/* Clear the bottom line */
	prt("", y + i + 1, x);
}


/*!
 * @brief �����ƥब������»���뤫�ɤ�����Ƚ�ꤹ��
 * @param o_ptr �����ƥ�ξ��󻲾ȥݥ���
 * @return ��»����ʤ��TRUE���֤�
 * Note that amulets, rods, and high-level spell books are immune
 * to "inventory damage" of any kind.  Also sling ammo and shovels.
 * Does a given class of objects (usually) hate acid?
 * Note that acid can either melt or corrode something.
 */
bool hates_acid(object_type *o_ptr)
{
	/* Analyze the type */
	switch (o_ptr->tval)
	{
		/* Wearable items */
		case TV_ARROW:
		case TV_BOLT:
		case TV_BOW:
		case TV_SWORD:
		case TV_HAFTED:
		case TV_POLEARM:
		case TV_HELM:
		case TV_CROWN:
		case TV_SHIELD:
		case TV_BOOTS:
		case TV_GLOVES:
		case TV_CLOAK:
		case TV_SOFT_ARMOR:
		case TV_HARD_ARMOR:
		case TV_DRAG_ARMOR:
		{
			return (TRUE);
		}

		/* Staffs/Scrolls are wood/paper */
		case TV_STAFF:
		case TV_SCROLL:
		{
			return (TRUE);
		}

		/* Ouch */
		case TV_CHEST:
		{
			return (TRUE);
		}

		/* Junk is useless */
		case TV_SKELETON:
		case TV_BOTTLE:
		case TV_JUNK:
		{
			return (TRUE);
		}
	}

	return (FALSE);
}


/*!
 * @brief �����ƥब�ŷ����»���뤫�ɤ�����Ƚ�ꤹ�� /
 * Does a given object (usually) hate electricity?
 * @param o_ptr �����ƥ�ξ��󻲾ȥݥ���
 * @return ��»����ʤ��TRUE���֤�
 */
bool hates_elec(object_type *o_ptr)
{
	switch (o_ptr->tval)
	{
		case TV_RING:
		case TV_WAND:
		{
			return (TRUE);
		}
	}

	return (FALSE);
}


/*!
 * @brief �����ƥब�б����»���뤫�ɤ�����Ƚ�ꤹ�� /
 * Does a given object (usually) hate fire?
 * @param o_ptr �����ƥ�ξ��󻲾ȥݥ���
 * @return ��»����ʤ��TRUE���֤�
 * @details
 * Hafted/Polearm weapons have wooden shafts.
 * Arrows/Bows are mostly wooden.
 */
bool hates_fire(object_type *o_ptr)
{
	/* Analyze the type */
	switch (o_ptr->tval)
	{
		/* Wearable */
		case TV_LITE:
		case TV_ARROW:
		case TV_BOW:
		case TV_HAFTED:
		case TV_POLEARM:
		case TV_BOOTS:
		case TV_GLOVES:
		case TV_CLOAK:
		case TV_SOFT_ARMOR:
		{
			return (TRUE);
		}

		/* Books */
		case TV_LIFE_BOOK:
		case TV_SORCERY_BOOK:
		case TV_NATURE_BOOK:
		case TV_CHAOS_BOOK:
		case TV_DEATH_BOOK:
		case TV_TRUMP_BOOK:
		case TV_ARCANE_BOOK:
		case TV_CRAFT_BOOK:
		case TV_DAEMON_BOOK:
		case TV_CRUSADE_BOOK:
		case TV_MUSIC_BOOK:
		case TV_HISSATSU_BOOK:
		case TV_HEX_BOOK:
		{
			return (TRUE);
		}

		/* Chests */
		case TV_CHEST:
		{
			return (TRUE);
		}

		/* Staffs/Scrolls burn */
		case TV_STAFF:
		case TV_SCROLL:
		{
			return (TRUE);
		}
	}

	return (FALSE);
}


/*!
 * @brief �����ƥब�䵤����»���뤫�ɤ�����Ƚ�ꤹ�� /
 * Does a given object (usually) hate cold?
 * @param o_ptr �����ƥ�ξ��󻲾ȥݥ���
 * @return ��»����ʤ��TRUE���֤�
 */
bool hates_cold(object_type *o_ptr)
{
	switch (o_ptr->tval)
	{
		case TV_POTION:
		case TV_FLASK:
		case TV_BOTTLE:
		{
			return (TRUE);
		}
	}

	return (FALSE);
}


/*!
 * @brief �����ƥब������»���뤫�ɤ�����Ƚ�ꤹ��(�ᥤ��롼����) /
 * Melt something
 * @param o_ptr �����ƥ�ξ��󻲾ȥݥ���
 * @return ��»����ʤ��TRUE���֤�
 * @todo �����Ƥ
 */
int set_acid_destroy(object_type *o_ptr)
{
	u32b flgs[TR_FLAG_SIZE];
	if (!hates_acid(o_ptr)) return (FALSE);
	object_flags(o_ptr, flgs);
	if (have_flag(flgs, TR_IGNORE_ACID)) return (FALSE);
	return (TRUE);
}


/*!
 * @brief �����ƥब�ŷ����»���뤫�ɤ�����Ƚ�ꤹ��(�ᥤ��롼����) /
 * Electrical damage
 * @param o_ptr �����ƥ�ξ��󻲾ȥݥ���
 * @return ��»����ʤ��TRUE���֤�
 * @todo �����Ƥ
 */
int set_elec_destroy(object_type *o_ptr)
{
	u32b flgs[TR_FLAG_SIZE];
	if (!hates_elec(o_ptr)) return (FALSE);
	object_flags(o_ptr, flgs);
	if (have_flag(flgs, TR_IGNORE_ELEC)) return (FALSE);
	return (TRUE);
}


/*!
 * @brief �����ƥब�б����»���뤫�ɤ�����Ƚ�ꤹ��(�ᥤ��롼����) /
 * Burn something
 * @param o_ptr �����ƥ�ξ��󻲾ȥݥ���
 * @return ��»����ʤ��TRUE���֤�
 * @todo �����Ƥ
 */
int set_fire_destroy(object_type *o_ptr)
{
	u32b flgs[TR_FLAG_SIZE];
	if (!hates_fire(o_ptr)) return (FALSE);
	object_flags(o_ptr, flgs);
	if (have_flag(flgs, TR_IGNORE_FIRE)) return (FALSE);
	return (TRUE);
}


/*!
 * @brief �����ƥब�䵤����»���뤫�ɤ�����Ƚ�ꤹ��(�ᥤ��롼����) /
 * Freeze things
 * @param o_ptr �����ƥ�ξ��󻲾ȥݥ���
 * @return ��»����ʤ��TRUE���֤�
 * @todo �����Ƥ
 */
int set_cold_destroy(object_type *o_ptr)
{
	u32b flgs[TR_FLAG_SIZE];
	if (!hates_cold(o_ptr)) return (FALSE);
	object_flags(o_ptr, flgs);
	if (have_flag(flgs, TR_IGNORE_COLD)) return (FALSE);
	return (TRUE);
}


/*!
 * @brief �����ƥब�����Ψ����»���뤫�ɤ�����Ƚ�ꤹ�� /
 * Destroys a type of item on a given percent chance
 * @param typ ��»Ƚ��ؿ��ݥ���
 * @param perc ���ܳ�Ψ
 * @return ��»���������ƥ�ο�
 * @details
 * Note that missiles are no longer necessarily all destroyed
 * Destruction taken from "melee.c" code for "stealing".
 * New-style wands and rods handled correctly. -LM-
 * Returns number of items destroyed.
 */
int inven_damage(inven_func typ, int perc)
{
	int         i, j, k, amt;
	object_type *o_ptr;
	char        o_name[MAX_NLEN];

	if (CHECK_MULTISHADOW()) return 0;

	if (p_ptr->inside_arena) return 0;

	/* Count the casualties */
	k = 0;

	/* Scan through the slots backwards */
	for (i = 0; i < INVEN_PACK; i++)
	{
		o_ptr = &inventory[i];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Hack -- for now, skip artifacts */
		if (object_is_artifact(o_ptr)) continue;

		/* Give this item slot a shot at death */
		if ((*typ)(o_ptr))
		{
			/* Count the casualties */
			for (amt = j = 0; j < o_ptr->number; ++j)
			{
				if (randint0(100) < perc) amt++;
			}

			/* Some casualities */
			if (amt)
			{
				/* Get a description */
				object_desc(o_name, o_ptr, OD_OMIT_PREFIX);

				/* Message */
				msg_format(_("%s(%c)��%s����Ƥ��ޤä���", "%sour %s (%c) %s destroyed!"),

#ifdef JP
o_name, index_to_label(i),
    ((o_ptr->number > 1) ?
    ((amt == o_ptr->number) ? "����" :
    (amt > 1 ? "���Ĥ�" : "���")) : "")    );
#else
				    ((o_ptr->number > 1) ?
				    ((amt == o_ptr->number) ? "All of y" :
				    (amt > 1 ? "Some of y" : "One of y")) : "Y"),
				    o_name, index_to_label(i),
				    ((amt > 1) ? "were" : "was"));
#endif

#ifdef JP
				if ((p_ptr->pseikaku == SEIKAKU_COMBAT) || (inventory[INVEN_BOW].name1 == ART_CRIMSON))
					msg_print("���䤬�ä��ʡ�");
#endif

				/* Potions smash open */
				if (object_is_potion(o_ptr))
				{
					(void)potion_smash_effect(0, p_ptr->y, p_ptr->x, o_ptr->k_idx);
				}

				/* Reduce the charges of rods/wands */
				reduce_charges(o_ptr, amt);

				/* Destroy "amt" items */
				inven_item_increase(i, -amt);
				inven_item_optimize(i);

				/* Count the casualties */
				k += amt;
			}
		}
	}

	/* Return the casualty count */
	return (k);
}


/*!
 * @brief ������ˤ��������AC�������� /
 * Acid has hit the player, attempt to affect some armor.
 * @return AC���ºݤ�����������TRUE���֤�
 * @details
 * Note that the "base armor" of an object never changes.
 * If any armor is damaged (or resists), the player takes less damage.
 */
static int minus_ac(void)
{
	object_type *o_ptr = NULL;
	u32b flgs[TR_FLAG_SIZE];
	char        o_name[MAX_NLEN];


	/* Pick a (possibly empty) inventory slot */
	switch (randint1(7))
	{
		case 1: o_ptr = &inventory[INVEN_RARM]; break;
		case 2: o_ptr = &inventory[INVEN_LARM]; break;
		case 3: o_ptr = &inventory[INVEN_BODY]; break;
		case 4: o_ptr = &inventory[INVEN_OUTER]; break;
		case 5: o_ptr = &inventory[INVEN_HANDS]; break;
		case 6: o_ptr = &inventory[INVEN_HEAD]; break;
		case 7: o_ptr = &inventory[INVEN_FEET]; break;
	}

	/* Nothing to damage */
	if (!o_ptr->k_idx) return (FALSE);

	if (!object_is_armour(o_ptr)) return (FALSE);

	/* No damage left to be done */
	if (o_ptr->ac + o_ptr->to_a <= 0) return (FALSE);


	/* Describe */
	object_desc(o_name, o_ptr, (OD_OMIT_PREFIX | OD_NAME_ONLY));

	/* Extract the flags */
	object_flags(o_ptr, flgs);

	/* Object resists */
	if (have_flag(flgs, TR_IGNORE_ACID))
	{
		msg_format(_("������%s�ˤϸ��̤��ʤ��ä���", "Your %s is unaffected!"), o_name);
		return (TRUE);
	}

	/* Message */
	msg_format(_("%s�����᡼�����������", "Your %s is damaged!"), o_name);

	/* Damage the item */
	o_ptr->to_a--;

	/* Calculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Window stuff */
	p_ptr->window |= (PW_EQUIP | PW_PLAYER);

	calc_android_exp();

	/* Item was damaged */
	return (TRUE);
}


/*!
 * @brief ��°���ˤ��ץ쥤�䡼»������ /
 * Hurt the player with Acid
 * @param dam ���ܥ��᡼����
 * @param kb_str ���᡼����������
 * @param monspell �����Ȥʤä���󥹥����ü칶��ID
 * @param aura ����������᡼���������ʤ��TRUE
 * @return ����HP���᡼����
 */
int acid_dam(HIT_POINT dam, cptr kb_str, int monspell, bool aura)
{
	int get_damage;  
	int inv = (dam < 30) ? 1 : (dam < 60) ? 2 : 3;
	bool double_resist = IS_OPPOSE_ACID();

	/* Total Immunity */
	if (p_ptr->immune_acid || (dam <= 0))
	{
		learn_spell(monspell);
		return 0;
	}

	/* Vulnerability (Ouch!) */
	if (p_ptr->muta3 & MUT3_VULN_ELEM) dam *= 2;
	if (p_ptr->special_defense & KATA_KOUKIJIN) dam += dam / 3;

	/* Resist the damage */
	if (p_ptr->resist_acid) dam = (dam + 2) / 3;
	if (double_resist) dam = (dam + 2) / 3;

	if (aura || !CHECK_MULTISHADOW())
	{
		if ((!(double_resist || p_ptr->resist_acid)) &&
		    one_in_(HURT_CHANCE))
			(void)do_dec_stat(A_CHR);

		/* If any armor gets hit, defend the player */
		if (minus_ac()) dam = (dam + 1) / 2;
	}

	/* Take damage */
	get_damage = take_hit(aura ? DAMAGE_NOESCAPE : DAMAGE_ATTACK, dam, kb_str, monspell);

	/* Inventory damage */
	if (!aura && !(double_resist && p_ptr->resist_acid))
		inven_damage(set_acid_destroy, inv);
	return get_damage;
}


/*!
 * @brief �ŷ�°���ˤ��ץ쥤�䡼»������ /
 * Hurt the player with electricity
 * @param dam ���ܥ��᡼����
 * @param kb_str ���᡼����������
 * @param monspell �����Ȥʤä���󥹥����ü칶��ID
 * @param aura ����������᡼���������ʤ��TRUE
 * @return ����HP���᡼����
 */
int elec_dam(HIT_POINT dam, cptr kb_str, int monspell, bool aura)
{
	int get_damage;  
	int inv = (dam < 30) ? 1 : (dam < 60) ? 2 : 3;
	bool double_resist = IS_OPPOSE_ELEC();

	/* Total immunity */
	if (p_ptr->immune_elec || (dam <= 0))
	{
		learn_spell(monspell);
		return 0;
	}

	/* Vulnerability (Ouch!) */
	if (p_ptr->muta3 & MUT3_VULN_ELEM) dam *= 2;
	if (p_ptr->special_defense & KATA_KOUKIJIN) dam += dam / 3;
	if (prace_is_(RACE_ANDROID)) dam += dam / 3;

	/* Resist the damage */
	if (p_ptr->resist_elec) dam = (dam + 2) / 3;
	if (double_resist) dam = (dam + 2) / 3;

	if (aura || !CHECK_MULTISHADOW())
	{
		if ((!(double_resist || p_ptr->resist_elec)) &&
		    one_in_(HURT_CHANCE))
			(void)do_dec_stat(A_DEX);
	}

	/* Take damage */
	get_damage = take_hit(aura ? DAMAGE_NOESCAPE : DAMAGE_ATTACK, dam, kb_str, monspell);

	/* Inventory damage */
	if (!aura && !(double_resist && p_ptr->resist_elec))
		inven_damage(set_elec_destroy, inv);

	return get_damage;
}


/*!
 * @brief �б�°���ˤ��ץ쥤�䡼»������ /
 * Hurt the player with Fire
 * @param dam ���ܥ��᡼����
 * @param kb_str ���᡼����������
 * @param monspell �����Ȥʤä���󥹥����ü칶��ID
 * @param aura ����������᡼���������ʤ��TRUE
 * @return ����HP���᡼����
 */
int fire_dam(HIT_POINT dam, cptr kb_str, int monspell, bool aura)
{
	int get_damage;  
	int inv = (dam < 30) ? 1 : (dam < 60) ? 2 : 3;
	bool double_resist = IS_OPPOSE_FIRE();

	/* Totally immune */
	if (p_ptr->immune_fire || (dam <= 0))
	{
		learn_spell(monspell);
		return 0;
	}

	/* Vulnerability (Ouch!) */
	if (p_ptr->muta3 & MUT3_VULN_ELEM) dam *= 2;
	if (prace_is_(RACE_ENT)) dam += dam / 3;
	if (p_ptr->special_defense & KATA_KOUKIJIN) dam += dam / 3;

	/* Resist the damage */
	if (p_ptr->resist_fire) dam = (dam + 2) / 3;
	if (double_resist) dam = (dam + 2) / 3;

	if (aura || !CHECK_MULTISHADOW())
	{
		if ((!(double_resist || p_ptr->resist_fire)) &&
		    one_in_(HURT_CHANCE))
			(void)do_dec_stat(A_STR);
	}

	/* Take damage */
	get_damage = take_hit(aura ? DAMAGE_NOESCAPE : DAMAGE_ATTACK, dam, kb_str, monspell);

	/* Inventory damage */
	if (!aura && !(double_resist && p_ptr->resist_fire))
		inven_damage(set_fire_destroy, inv);

	return get_damage;
}


/*!
 * @brief �䵤°���ˤ��ץ쥤�䡼»������ /
 * Hurt the player with Cold
 * @param dam ���ܥ��᡼����
 * @param kb_str ���᡼����������
 * @param monspell �����Ȥʤä���󥹥����ü칶��ID
 * @param aura ����������᡼���������ʤ��TRUE
 * @return ����HP���᡼����
 */
int cold_dam(HIT_POINT dam, cptr kb_str, int monspell, bool aura)
{
	int get_damage;  
	int inv = (dam < 30) ? 1 : (dam < 60) ? 2 : 3;
	bool double_resist = IS_OPPOSE_COLD();

	/* Total immunity */
	if (p_ptr->immune_cold || (dam <= 0))
	{
		learn_spell(monspell);
		return 0;
	}

	/* Vulnerability (Ouch!) */
	if (p_ptr->muta3 & MUT3_VULN_ELEM) dam *= 2;
	if (p_ptr->special_defense & KATA_KOUKIJIN) dam += dam / 3;

	/* Resist the damage */
	if (p_ptr->resist_cold) dam = (dam + 2) / 3;
	if (double_resist) dam = (dam + 2) / 3;

	if (aura || !CHECK_MULTISHADOW())
	{
		if ((!(double_resist || p_ptr->resist_cold)) &&
		    one_in_(HURT_CHANCE))
			(void)do_dec_stat(A_STR);
	}

	/* Take damage */
	get_damage = take_hit(aura ? DAMAGE_NOESCAPE : DAMAGE_ATTACK, dam, kb_str, monspell);

	/* Inventory damage */
	if (!aura && !(double_resist && p_ptr->resist_cold))
		inven_damage(set_cold_destroy, inv);

	return get_damage;
}

/*!
 * @brief �ɶ�λ��ߤ��ɻ߽���
 * @return �����������פ��������Ԥä��ʤ��TRUE���֤�
 */
bool rustproof(void)
{
	OBJECT_IDX item;
	object_type *o_ptr;
	char        o_name[MAX_NLEN];
	cptr        q, s;

	item_tester_no_ryoute = TRUE;
	/* Select a piece of armour */
	item_tester_hook = object_is_armour;

	/* Get an item */
	q = _("�ɤ��ɶ�˻��ߤ�򤷤ޤ�����", "Rustproof which piece of armour? ");
	s = _("���ߤ�Ǥ����Τ�����ޤ���", "You have nothing to rustproof.");

	if (!get_item(&item, q, s, (USE_EQUIP | USE_INVEN | USE_FLOOR))) return FALSE;

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


	/* Description */
	object_desc(o_name, o_ptr, (OD_OMIT_PREFIX | OD_NAME_ONLY));

	add_flag(o_ptr->art_flags, TR_IGNORE_ACID);

	if ((o_ptr->to_a < 0) && !object_is_cursed(o_ptr))
	{
#ifdef JP
msg_format("%s�Ͽ���Ʊ�ͤˤʤä���",o_name);
#else
		msg_format("%s %s look%s as good as new!",
			((item >= 0) ? "Your" : "The"), o_name,
			((o_ptr->number > 1) ? "" : "s"));
#endif

		o_ptr->to_a = 0;
	}

#ifdef JP
msg_format("%s���忩���ʤ��ʤä���", o_name);
#else
	msg_format("%s %s %s now protected against corrosion.",
		((item >= 0) ? "Your" : "The"), o_name,
		((o_ptr->number > 1) ? "are" : "is"));
#endif


	calc_android_exp();

	return TRUE;
}


/*!
 * @brief �ɶ�������� /
 * Curse the players armor
 * @return �ºݤ˼������줿��TRUE���֤�
 */
bool curse_armor(void)
{
	int i;
	object_type *o_ptr;

	char o_name[MAX_NLEN];


	/* Curse the body armor */
	o_ptr = &inventory[INVEN_BODY];

	/* Nothing to curse */
	if (!o_ptr->k_idx) return (FALSE);


	/* Describe */
	object_desc(o_name, o_ptr, OD_OMIT_PREFIX);

	/* Attempt a saving throw for artifacts */
	if (object_is_artifact(o_ptr) && (randint0(100) < 50))
	{
		/* Cool */
#ifdef JP
msg_format("%s��%s����߹��⤦�Ȥ�������%s�Ϥ����ķ���֤�����",
"���ݤΰŹ�������", "�ɶ�", o_name);
#else
		msg_format("A %s tries to %s, but your %s resists the effects!",
			   "terrible black aura", "surround your armor", o_name);
#endif

	}

	/* not artifact or failed save... */
	else
	{
		/* Oops */
		msg_format(_("���ݤΰŹ������餬���ʤ���%s����߹������", "A terrible black aura blasts your %s!"), o_name);
		chg_virtue(V_ENCHANT, -5);

		/* Blast the armor */
		o_ptr->name1 = 0;
		o_ptr->name2 = EGO_BLASTED;
		o_ptr->to_a = 0 - randint1(5) - randint1(5);
		o_ptr->to_h = 0;
		o_ptr->to_d = 0;
		o_ptr->ac = 0;
		o_ptr->dd = 0;
		o_ptr->ds = 0;

		for (i = 0; i < TR_FLAG_SIZE; i++)
			o_ptr->art_flags[i] = 0;

		/* Curse it */
		o_ptr->curse_flags = TRC_CURSED;

		/* Break it */
		o_ptr->ident |= (IDENT_BROKEN);

		/* Recalculate bonuses */
		p_ptr->update |= (PU_BONUS);

		/* Recalculate mana */
		p_ptr->update |= (PU_MANA);

		/* Window stuff */
		p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);
	}

	return (TRUE);
}

/*!
 * @brief ���������� /
 * Curse the players weapon
 * @param force ̵���˼�����Ԥ��ʤ��TRUE
 * @param o_ptr �����������Υ����ƥ���󻲾ȥݥ���
 * @return �ºݤ˼������줿��TRUE���֤�
 */
bool curse_weapon_object(bool force, object_type *o_ptr)
{
	int i;
	char o_name[MAX_NLEN];

	/* Nothing to curse */
	if (!o_ptr->k_idx) return (FALSE);

	/* Describe */
	object_desc(o_name, o_ptr, OD_OMIT_PREFIX);

	/* Attempt a saving throw */
	if (object_is_artifact(o_ptr) && (randint0(100) < 50) && !force)
	{
		/* Cool */
#ifdef JP
		msg_format("%s��%s����߹��⤦�Ȥ�������%s�Ϥ����ķ���֤�����",
				"���ݤΰŹ�������", "���", o_name);
#else
		msg_format("A %s tries to %s, but your %s resists the effects!",
				"terrible black aura", "surround your weapon", o_name);
#endif
	}

	/* not artifact or failed save... */
	else
	{
		/* Oops */
		if (!force) msg_format(_("���ݤΰŹ������餬���ʤ���%s����߹������", "A terrible black aura blasts your %s!"), o_name);
		chg_virtue(V_ENCHANT, -5);

		/* Shatter the weapon */
		o_ptr->name1 = 0;
		o_ptr->name2 = EGO_SHATTERED;
		o_ptr->to_h = 0 - randint1(5) - randint1(5);
		o_ptr->to_d = 0 - randint1(5) - randint1(5);
		o_ptr->to_a = 0;
		o_ptr->ac = 0;
		o_ptr->dd = 0;
		o_ptr->ds = 0;

		for (i = 0; i < TR_FLAG_SIZE; i++)
			o_ptr->art_flags[i] = 0;

		/* Curse it */
		o_ptr->curse_flags = TRC_CURSED;

		/* Break it */
		o_ptr->ident |= (IDENT_BROKEN);

		/* Recalculate bonuses */
		p_ptr->update |= (PU_BONUS);

		/* Recalculate mana */
		p_ptr->update |= (PU_MANA);

		/* Window stuff */
		p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);
	}

	/* Notice */
	return (TRUE);
}

/*!
 * @brief �����������Υᥤ��롼���� /
 * Curse the players weapon
 * @param force ̵���˼�����Ԥ��ʤ��TRUE
 * @param slot ��������������������å�
 * @return �ºݤ˼������줿��TRUE���֤�
 */
bool curse_weapon(bool force, int slot)
{
	/* Curse the weapon */
	return curse_weapon_object(force, &inventory[slot]);
}


/*!
 * @brief �ܥ�ȤΥ���������(�бꥨ���Τ�) /
 * Enchant some bolts
 * @return ���TRUE���֤�
 */
bool brand_bolts(void)
{
	int i;

	/* Use the first acceptable bolts */
	for (i = 0; i < INVEN_PACK; i++)
	{
		object_type *o_ptr = &inventory[i];

		/* Skip non-bolts */
		if (o_ptr->tval != TV_BOLT) continue;

		/* Skip artifacts and ego-items */
		if (object_is_artifact(o_ptr) || object_is_ego(o_ptr))
			continue;

		/* Skip cursed/broken items */
		if (object_is_cursed(o_ptr) || object_is_broken(o_ptr)) continue;

		/* Randomize */
		if (randint0(100) < 75) continue;

		/* Message */
		msg_print(_("�����ܥ����𤬱�Υ��������ޤ줿��", "Your bolts are covered in a fiery aura!"));

		/* Ego-item */
		o_ptr->name2 = EGO_FLAME;

		/* Enchant */
		enchant(o_ptr, randint0(3) + 4, ENCH_TOHIT | ENCH_TODAM);

		/* Notice */
		return (TRUE);
	}

	/* Flush */
	if (flush_failure) flush();

	/* Fail */
	msg_print(_("��Ƕ�������Τ˼��Ԥ�����", "The fiery enchantment failed."));

	/* Notice */
	return (TRUE);
}


/*!
 * @brief �ѿȽ��������˥�󥹥����ζ��٥�٥��ӥ�󥹥������֤� /
 * Helper function -- return a "nearby" race for polymorphing
 * @param r_idx ���Ȥʤ��󥹥�����²ID
 * @return �ѹ���Υ�󥹥�����²ID
 * @details
 * Note that this function is one of the more "dangerous" ones...
 */
static IDX poly_r_idx(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	int i;
	MONRACE_IDX r;
	DEPTH lev1, lev2;

	/* Hack -- Uniques/Questors never polymorph */
	if ((r_ptr->flags1 & RF1_UNIQUE) ||
	    (r_ptr->flags1 & RF1_QUESTOR))
		return (r_idx);

	/* Allowable range of "levels" for resulting monster */
	lev1 = r_ptr->level - ((randint1(20) / randint1(9)) + 1);
	lev2 = r_ptr->level + ((randint1(20) / randint1(9)) + 1);

	/* Pick a (possibly new) non-unique race */
	for (i = 0; i < 1000; i++)
	{
		/* Pick a new race, using a level calculation */
		r = get_mon_num((dun_level + r_ptr->level) / 2 + 5);

		/* Handle failure */
		if (!r) break;

		/* Obtain race */
		r_ptr = &r_info[r];

		/* Ignore unique monsters */
		if (r_ptr->flags1 & RF1_UNIQUE) continue;

		/* Ignore monsters with incompatible levels */
		if ((r_ptr->level < lev1) || (r_ptr->level > lev2)) continue;

		/* Use that index */
		r_idx = r;

		/* Done */
		break;
	}

	/* Result */
	return (r_idx);
}

/*!
 * @brief �����ɸ�ˤ����󥹥������ѿȤ����� /
 * Helper function -- return a "nearby" race for polymorphing
 * @param y �����Y��ɸ
 * @param x �����X��ɸ
 * @return �ºݤ��ѿȤ�����TRUE���֤�
 */
bool polymorph_monster(POSITION y, POSITION x)
{
	cave_type *c_ptr = &cave[y][x];
	monster_type *m_ptr = &m_list[c_ptr->m_idx];
	bool polymorphed = FALSE;
	MONRACE_IDX new_r_idx;
	MONRACE_IDX old_r_idx = m_ptr->r_idx;
	bool targeted = (target_who == c_ptr->m_idx) ? TRUE : FALSE;
	bool health_tracked = (p_ptr->health_who == c_ptr->m_idx) ? TRUE : FALSE;
	monster_type back_m;

	if (p_ptr->inside_arena || p_ptr->inside_battle) return (FALSE);

	if ((p_ptr->riding == c_ptr->m_idx) || (m_ptr->mflag2 & MFLAG2_KAGE)) return (FALSE);

	/* Memorize the monster before polymorphing */
	back_m = *m_ptr;

	/* Pick a "new" monster race */
	new_r_idx = poly_r_idx(old_r_idx);

	/* Handle polymorph */
	if (new_r_idx != old_r_idx)
	{
		BIT_FLAGS mode = 0L;
		bool preserve_hold_objects = back_m.hold_o_idx ? TRUE : FALSE;
		s16b this_o_idx, next_o_idx = 0;

		/* Get the monsters attitude */
		if (is_friendly(m_ptr)) mode |= PM_FORCE_FRIENDLY;
		if (is_pet(m_ptr)) mode |= PM_FORCE_PET;
		if (m_ptr->mflag2 & MFLAG2_NOPET) mode |= PM_NO_PET;

		/* Mega-hack -- ignore held objects */
		m_ptr->hold_o_idx = 0;

		/* "Kill" the "old" monster */
		delete_monster_idx(c_ptr->m_idx);

		/* Create a new monster (no groups) */
		if (place_monster_aux(0, y, x, new_r_idx, mode))
		{
			m_list[hack_m_idx_ii].nickname = back_m.nickname;
			m_list[hack_m_idx_ii].parent_m_idx = back_m.parent_m_idx;
			m_list[hack_m_idx_ii].hold_o_idx = back_m.hold_o_idx;

			/* Success */
			polymorphed = TRUE;
		}
		else
		{
			/* Placing the new monster failed */
			if (place_monster_aux(0, y, x, old_r_idx, (mode | PM_NO_KAGE | PM_IGNORE_TERRAIN)))
			{
				m_list[hack_m_idx_ii] = back_m;

				/* Re-initialize monster process */
				mproc_init();
			}
			else preserve_hold_objects = FALSE;
		}

		/* Mega-hack -- preserve held objects */
		if (preserve_hold_objects)
		{
			for (this_o_idx = back_m.hold_o_idx; this_o_idx; this_o_idx = next_o_idx)
			{
				/* Acquire object */
				object_type *o_ptr = &o_list[this_o_idx];

				/* Acquire next object */
				next_o_idx = o_ptr->next_o_idx;

				/* Held by new monster */
				o_ptr->held_m_idx = hack_m_idx_ii;
			}
		}
		else if (back_m.hold_o_idx) /* Failed (paranoia) */
		{
			/* Delete objects */
			for (this_o_idx = back_m.hold_o_idx; this_o_idx; this_o_idx = next_o_idx)
			{
				/* Acquire next object */
				next_o_idx = o_list[this_o_idx].next_o_idx;

				/* Delete the object */
				delete_object_idx(this_o_idx);
			}
		}

		if (targeted) target_who = hack_m_idx_ii;
		if (health_tracked) health_track(hack_m_idx_ii);
	}

	return polymorphed;
}

/*!
 * @brief ����������� /
 * Dimension Door
 * @param x �ƥ�ݡ������X��ɸ
 * @param y �ƥ�ݡ������Y��ɸ
 * @return ��ɸ�˻����̤�ƥ�ݡ��ȤǤ����ʤ��TRUE���֤�
 */
static bool dimension_door_aux(DEPTH x, DEPTH y)
{
	PLAYER_LEVEL plev = p_ptr->lev;

	p_ptr->energy_need += (s16b)((s32b)(60 - plev) * ENERGY_NEED() / 100L);

	if (!cave_player_teleportable_bold(y, x, 0L) ||
	    (distance(y, x, p_ptr->y, p_ptr->x) > plev / 2 + 10) ||
	    (!randint0(plev / 10 + 10)))
	{
		p_ptr->energy_need += (s16b)((s32b)(60 - plev) * ENERGY_NEED() / 100L);
		teleport_player((plev + 2) * 2, TELEPORT_PASSIVE);

		/* Failed */
		return FALSE;
	}
	else
	{
		teleport_player_to(y, x, 0L);

		/* Success */
		return TRUE;
	}
}


/*!
 * @brief ������������Υᥤ��롼���� /
 * Dimension Door
 * @return ���������񤷤����TRUE���֤�
 */
bool dimension_door(void)
{
	DEPTH x = 0, y = 0;

	/* Rerutn FALSE if cancelled */
	if (!tgt_pt(&x, &y)) return FALSE;

	if (dimension_door_aux(x, y)) return TRUE;

	msg_print(_("�������ʪ�������������ޤ������ʤ��ä���", "You fail to exit the astral plane correctly!"));

	return TRUE;
}


/*!
 * @brief ��ȴ�������Υᥤ��롼���� /
 * Mirror Master's Dimension Door
 * @return ���������񤷤����TRUE���֤�
 */
bool mirror_tunnel(void)
{
	POSITION x = 0, y = 0;

	/* Rerutn FALSE if cancelled */
	if (!tgt_pt(&x, &y)) return FALSE;

	if (dimension_door_aux(x, y)) return TRUE;

	msg_print(_("���������򤦤ޤ��̤�ʤ��ä���", "You fail to pass the mirror plane correctly!"));

	return TRUE;
}

/*!
 * @brief ���Ͽ�������
 * @param power ���ܸ���
 * @return ���������񤷤����TRUE���֤�
 */
bool eat_magic(int power)
{
	object_type * o_ptr;
	object_kind *k_ptr;
	int lev;
	OBJECT_IDX item;
	int recharge_strength = 0;

	bool fail = FALSE;
	byte fail_type = 1;

	cptr q, s;
	char o_name[MAX_NLEN];

	item_tester_hook = item_tester_hook_recharge;

	/* Get an item */
	q = _("�ɤΥ����ƥफ�����Ϥ�ۼ����ޤ�����", "Drain which item? ");
	s = _("���Ϥ�ۼ��Ǥ��륢���ƥब����ޤ���", "You have nothing to drain.");

	if (!get_item(&item, q, s, (USE_INVEN | USE_FLOOR))) return FALSE;

	if (item >= 0)
	{
		o_ptr = &inventory[item];
	}
	else
	{
		o_ptr = &o_list[0 - item];
	}

	k_ptr = &k_info[o_ptr->k_idx];
	lev = k_info[o_ptr->k_idx].level;

	if (o_ptr->tval == TV_ROD)
	{
		recharge_strength = ((power > lev/2) ? (power - lev/2) : 0) / 5;

		/* Back-fire */
		if (one_in_(recharge_strength))
		{
			/* Activate the failure code. */
			fail = TRUE;
		}
		else
		{
			if (o_ptr->timeout > (o_ptr->number - 1) * k_ptr->pval)
			{
				msg_print(_("��Ŷ��Υ�åɤ������Ϥ�ۼ����뤳�ȤϤǤ��ޤ���", "You can't absorb energy from a discharged rod."));
			}
			else
			{
				p_ptr->csp += lev;
				o_ptr->timeout += k_ptr->pval;
			}
		}
	}
	else
	{
		/* All staffs, wands. */
		recharge_strength = (100 + power - lev) / 15;

		/* Paranoia */
		if (recharge_strength < 0) recharge_strength = 0;

		/* Back-fire */
		if (one_in_(recharge_strength))
		{
			/* Activate the failure code. */
			fail = TRUE;
		}
		else
		{
			if (o_ptr->pval > 0)
			{
				p_ptr->csp += lev / 2;
				o_ptr->pval --;

				/* XXX Hack -- unstack if necessary */
				if ((o_ptr->tval == TV_STAFF) && (item >= 0) && (o_ptr->number > 1))
				{
					object_type forge;
					object_type *q_ptr;

					/* Get local object */
					q_ptr = &forge;

					/* Obtain a local object */
					object_copy(q_ptr, o_ptr);

					/* Modify quantity */
					q_ptr->number = 1;

					/* Restore the charges */
					o_ptr->pval++;

					/* Unstack the used item */
					o_ptr->number--;
					p_ptr->total_weight -= q_ptr->weight;
					item = inven_carry(q_ptr);

					/* Message */
					msg_print(_("���ޤȤ�ʤ�������", "You unstack your staff."));
				}
			}
			else
			{
				msg_print(_("�ۼ��Ǥ������Ϥ�����ޤ���", "There's no energy there to absorb!"));
			}
			if (!o_ptr->pval) o_ptr->ident |= IDENT_EMPTY;
		}
	}

	/* Inflict the penalties for failing a recharge. */
	if (fail)
	{
		/* Artifacts are never destroyed. */
		if (object_is_fixed_artifact(o_ptr))
		{
			object_desc(o_name, o_ptr, OD_NAME_ONLY);
			msg_format(_("���Ϥ���ή������%s�ϴ��������Ϥ򼺤ä���", "The recharging backfires - %s is completely drained!"), o_name);

			/* Artifact rods. */
			if (o_ptr->tval == TV_ROD)
				o_ptr->timeout = k_ptr->pval * o_ptr->number;

			/* Artifact wands and staffs. */
			else if ((o_ptr->tval == TV_WAND) || (o_ptr->tval == TV_STAFF))
				o_ptr->pval = 0;
		}
		else
		{
			/* Get the object description */
			object_desc(o_name, o_ptr, (OD_OMIT_PREFIX | OD_NAME_ONLY));

			/*** Determine Seriousness of Failure ***/

			/* Mages recharge objects more safely. */
			if (p_ptr->pclass == CLASS_MAGE || p_ptr->pclass == CLASS_HIGH_MAGE || p_ptr->pclass == CLASS_SORCERER || p_ptr->pclass == CLASS_MAGIC_EATER || p_ptr->pclass == CLASS_BLUE_MAGE)
			{
				/* 10% chance to blow up one rod, otherwise draining. */
				if (o_ptr->tval == TV_ROD)
				{
					if (one_in_(10)) fail_type = 2;
					else fail_type = 1;
				}
				/* 75% chance to blow up one wand, otherwise draining. */
				else if (o_ptr->tval == TV_WAND)
				{
					if (!one_in_(3)) fail_type = 2;
					else fail_type = 1;
				}
				/* 50% chance to blow up one staff, otherwise no effect. */
				else if (o_ptr->tval == TV_STAFF)
				{
					if (one_in_(2)) fail_type = 2;
					else fail_type = 0;
				}
			}

			/* All other classes get no special favors. */
			else
			{
				/* 33% chance to blow up one rod, otherwise draining. */
				if (o_ptr->tval == TV_ROD)
				{
					if (one_in_(3)) fail_type = 2;
					else fail_type = 1;
				}
				/* 20% chance of the entire stack, else destroy one wand. */
				else if (o_ptr->tval == TV_WAND)
				{
					if (one_in_(5)) fail_type = 3;
					else fail_type = 2;
				}
				/* Blow up one staff. */
				else if (o_ptr->tval == TV_STAFF)
				{
					fail_type = 2;
				}
			}

			/*** Apply draining and destruction. ***/

			/* Drain object or stack of objects. */
			if (fail_type == 1)
			{
				if (o_ptr->tval == TV_ROD)
				{
					msg_format(_("��åɤ���»���Ȥ줿�������Ϥ����Ƽ��ʤ�줿��",
								 "You save your rod from destruction, but all charges are lost."), o_name);
					o_ptr->timeout = k_ptr->pval * o_ptr->number;
				}
				else if (o_ptr->tval == TV_WAND)
				{
					msg_format(_("%s����»���Ȥ줿�������Ϥ����Ƽ���줿��", "You save your %s from destruction, but all charges are lost."), o_name);
					o_ptr->pval = 0;
				}
				/* Staffs aren't drained. */
			}

			/* Destroy an object or one in a stack of objects. */
			if (fail_type == 2)
			{
				if (o_ptr->number > 1)
				{
					msg_format(_("��˽����ˡ�Τ����%s�����ܲ��줿��", "Wild magic consumes one of your %s!"), o_name);
					/* Reduce rod stack maximum timeout, drain wands. */
					if (o_ptr->tval == TV_ROD) o_ptr->timeout = MIN(o_ptr->timeout, k_ptr->pval * (o_ptr->number - 1));
					else if (o_ptr->tval == TV_WAND) o_ptr->pval = o_ptr->pval * (o_ptr->number - 1) / o_ptr->number;
				}
				else
				{
					msg_format(_("��˽����ˡ�Τ����%s�����ܤ����줿��", "Wild magic consumes your %s!"), o_name);
				}
				
				/* Reduce and describe inventory */
				if (item >= 0)
				{
					inven_item_increase(item, -1);
					inven_item_describe(item);
					inven_item_optimize(item);
				}

				/* Reduce and describe floor item */
				else
				{
					floor_item_increase(0 - item, -1);
					floor_item_describe(0 - item);
					floor_item_optimize(0 - item);
				}
			}

			/* Destroy all members of a stack of objects. */
			if (fail_type == 3)
			{
				if (o_ptr->number > 1)
					msg_format(_("��˽����ˡ�Τ����%s�����Ʋ��줿��", "Wild magic consumes all your %s!"), o_name);
				else
					msg_format(_("��˽����ˡ�Τ����%s�����줿��", "Wild magic consumes your %s!"), o_name);

				/* Reduce and describe inventory */
				if (item >= 0)
				{
					inven_item_increase(item, -999);
					inven_item_describe(item);
					inven_item_optimize(item);
				}

				/* Reduce and describe floor item */
				else
				{
					floor_item_increase(0 - item, -999);
					floor_item_describe(0 - item);
					floor_item_optimize(0 - item);
				}
			}
		}
	}

	if (p_ptr->csp > p_ptr->msp)
	{
		p_ptr->csp = p_ptr->msp;
	}

	/* Redraw mana and hp */
	p_ptr->redraw |= (PR_MANA);

	p_ptr->notice |= (PN_COMBINE | PN_REORDER);
	p_ptr->window |= (PW_INVEN);

	return TRUE;
}

/*!
 * @brief Ʊ²����(�緳)����
 * @param level ��������٥�
 * @param y ������Y��ɸ
 * @param x ������X��ɸ
 * @param mode �������ץ����
 * @return ���������񤷤����TRUE���֤�
 */
bool summon_kin_player(int level, int y, int x, BIT_FLAGS mode)
{
	bool pet = (bool)(mode & PM_FORCE_PET);
	if (!pet) mode |= PM_NO_PET;

	switch (p_ptr->mimic_form)
	{
	case MIMIC_NONE:
		switch (p_ptr->prace)
		{
			case RACE_HUMAN:
			case RACE_AMBERITE:
			case RACE_BARBARIAN:
			case RACE_BEASTMAN:
			case RACE_DUNADAN:
				summon_kin_type = 'p';
				break;
			case RACE_HALF_ELF:
			case RACE_ELF:
			case RACE_HOBBIT:
			case RACE_GNOME:
			case RACE_DWARF:
			case RACE_HIGH_ELF:
			case RACE_NIBELUNG:
			case RACE_DARK_ELF:
			case RACE_MIND_FLAYER:
			case RACE_KUTAR:
			case RACE_S_FAIRY:
				summon_kin_type = 'h';
				break;
			case RACE_HALF_ORC:
				summon_kin_type = 'o';
				break;
			case RACE_HALF_TROLL:
				summon_kin_type = 'T';
				break;
			case RACE_HALF_OGRE:
				summon_kin_type = 'O';
				break;
			case RACE_HALF_GIANT:
			case RACE_HALF_TITAN:
			case RACE_CYCLOPS:
				summon_kin_type = 'P';
				break;
			case RACE_YEEK:
				summon_kin_type = 'y';
				break;
			case RACE_KLACKON:
				summon_kin_type = 'K';
				break;
			case RACE_KOBOLD:
				summon_kin_type = 'k';
				break;
			case RACE_IMP:
				if (one_in_(13)) summon_kin_type = 'U';
				else summon_kin_type = 'u';
				break;
			case RACE_DRACONIAN:
				summon_kin_type = 'd';
				break;
			case RACE_GOLEM:
			case RACE_ANDROID:
				summon_kin_type = 'g';
				break;
			case RACE_SKELETON:
				if (one_in_(13)) summon_kin_type = 'L';
				else summon_kin_type = 's';
				break;
			case RACE_ZOMBIE:
				summon_kin_type = 'z';
				break;
			case RACE_VAMPIRE:
				summon_kin_type = 'V';
				break;
			case RACE_SPECTRE:
				summon_kin_type = 'G';
				break;
			case RACE_SPRITE:
				summon_kin_type = 'I';
				break;
			case RACE_ENT:
				summon_kin_type = '#';
				break;
			case RACE_ANGEL:
				summon_kin_type = 'A';
				break;
			case RACE_DEMON:
				summon_kin_type = 'U';
				break;
			default:
				summon_kin_type = 'p';
				break;
		}
		break;
	case MIMIC_DEMON:
		if (one_in_(13)) summon_kin_type = 'U';
		else summon_kin_type = 'u';
		break;
	case MIMIC_DEMON_LORD:
		summon_kin_type = 'U';
		break;
	case MIMIC_VAMPIRE:
		summon_kin_type = 'V';
		break;
	}	
	return summon_specific((pet ? -1 : 0), y, x, level, SUMMON_KIN, mode);
}

/*!
 * @brief ������(����������)����
 * @param py �ץ쥤�䡼Y��ɸ
 * @param px �ץ쥤�䡼X��ɸ
 * @return �ʤ�
 */
void massacre(void)
{
	int x, y;
	cave_type       *c_ptr;
	monster_type    *m_ptr;
	int dir;

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
