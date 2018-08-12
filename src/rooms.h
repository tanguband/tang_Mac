/*!
 * @file rooms.h
 * @brief ������������������إå��� / Header file for rooms.c, used only in generate.c
 * @date 2014/09/07
 * @author
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke<br>
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.  Other copyrights may also apply.<br
 */

#define DUN_ROOMS_MAX	40 /*!< �������������κ����Կ� / Number of rooms to attempt (was 50) */


/* �ӷ��Ϸ�������ID / Room types for generate_lake() */
#define LAKE_T_LAVA        1 /*!< �ӷ��Ϸ�ID: �ϴ� */
#define LAKE_T_WATER       2 /*!< �ӷ��Ϸ�ID: �� */
#define LAKE_T_CAVE        3 /*!< �ӷ��Ϸ�ID: ��ƶ */
#define LAKE_T_EARTH_VAULT 4 /*!< �ӷ��Ϸ�ID: ��°��VAULT */
#define LAKE_T_AIR_VAULT   5 /*!< �ӷ��Ϸ�ID: ��°��VAULT */
#define LAKE_T_WATER_VAULT 6 /*!< �ӷ��Ϸ�ID: ��°��VAULT */
#define LAKE_T_FIRE_VAULT  7 /*!< �ӷ��Ϸ�ID: ��°��VAULT */


/* ������ID / Room types for room_build() */
#define ROOM_T_NORMAL         0	 /*!<������ID:����Ĺ���� / Simple (33x11) */
#define ROOM_T_OVERLAP        1	 /*!<������ID:Ĺ������ĽŤ� / Overlapping (33x11) */
#define ROOM_T_CROSS          2	 /*!<������ID:���� / Crossed (33x11) */
#define ROOM_T_INNER_FEAT     3	 /*!<������ID:����� / Large (33x11) */
#define ROOM_T_NEST           4	 /*!<������ID:��󥹥���NEST / Monster nest (33x11) */
#define ROOM_T_PIT            5	 /*!<������ID:��󥹥���PIT / Monster pit (33x11) */
#define ROOM_T_LESSER_VAULT   6	 /*!<������ID:����VAULT / Lesser vault (33x22) */
#define ROOM_T_GREATER_VAULT  7	 /*!<������ID:�緿VAULT / Greater vault (66x44) */
#define ROOM_T_FRACAVE        8	 /*!<������ID:�ե饯�����Ϸ� / Fractal cave (42x24) */
#define ROOM_T_RANDOM_VAULT   9	 /*!<������ID:������VAULT / Random vault (44x22) */
#define ROOM_T_OVAL          10	 /*!<������ID:�߷����� / Circular rooms (22x22) */
#define ROOM_T_CRYPT         11	 /*!<������ID:��Ʋ / Crypts (22x22) */
#define ROOM_T_TRAP_PIT      12	 /*!<������ID:�ȥ�åפĤ���󥹥���PIT / Trapped monster pit */
#define ROOM_T_TRAP          13	 /*!<������ID:�ȥ�å����� / Piranha/Armageddon trap room */
#define ROOM_T_GLASS         14	 /*!<������ID:���饹���� / Glass room */
#define ROOM_T_ARCADE        15  /*!<������ID:��Ź / Arcade */

#define ROOM_T_MAX 16 /*!<������ID����� */


/*
 * Room type information
 */
typedef struct room_info_type room_info_type;

struct room_info_type
{
        /* Allocation information. */
        s16b prob[ROOM_T_MAX];

        /* Minimum level on which room can appear. */
        byte min_level;
};


/* Externs */
#ifdef ALLOW_CAVERNS_AND_LAKES
extern void build_lake(int type);
extern void build_cavern(void);
#endif /* ALLOW_CAVERNS_AND_LAKES */

extern bool generate_rooms(void);
extern void build_maze_vault(int x0, int y0, int xsize, int ysize, bool is_vault);

