/*!
 * @file h-type.h
 * @brief ����������Ѥ����ѿ������ / Basic "types".
 * @date 2017/12/03
 * @author
 * ����(�Ѷ����ܥ����åա�)
 * @details
 * <pre>
 * Note the attempt to make all basic types have 4 letters.
 * This improves readibility and standardizes the code.
 * Likewise, all complex types are at least 4 letters.
 * Thus, almost every three letter word is a legal variable.
 * But beware of certain reserved words ('for' and 'if' and 'do').
 * Note that the type used in structures for bit flags should be uint.
 * As long as these bit flags are sequential, they will be space smart.
 * Note that on some machines, apparently "signed char" is illegal.
 * It must be true that char/byte takes exactly 1 byte
 * It must be true that sind/uind takes exactly 2 bytes
 * It must be true that sbig/ubig takes exactly 4 bytes
 * On Sparc's, a sint takes 4 bytes (2 is legal)
 * On Sparc's, a uint takes 4 bytes (2 is legal)
 * On Sparc's, a long takes 4 bytes (8 is legal)
 * On Sparc's, a huge takes 4 bytes (8 is legal)
 * On Sparc's, a vptr takes 4 bytes (8 is legal)
 * On Sparc's, a real takes 8 bytes (4 is legal)
 * Note that some files have already been included by "h-include.h"
 * These include <stdio.h> and <sys/types>, which define some types
 * In particular, uint is defined so we do not have to define it
 * Also, see <limits.h> for min/max values for sind, uind, long, huge
 * (SHRT_MIN, SHRT_MAX, USHRT_MAX, LONG_MIN, LONG_MAX, ULONG_MAX)
 * These limits should be verified and coded into "h-constant.h".
 * </pre>
 */

#ifndef INCLUDED_H_TYPE_H
#define INCLUDED_H_TYPE_H

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

/*** Special 4 letter names for some standard types ***/

typedef void *vptr;       /*!< void���ݥ������ / A standard pointer (to "void" because ANSI C says so) */
typedef const char *cptr; /*!< ʸ��������ѥݥ������ / A simple pointer (to unmodifiable strings) */
typedef double real;      /*!< double��real���Ȥ������ / Since float's are silly, hard code real numbers as doubles */


/*!
 * @brief ���顼�����ɤ���� / Error codes for function return values
 * @details
 * ���̤�������0�����Ի���������餫������������Ȥ��롣
 * Success = 0, Failure = -N, Problem = +N 
 */
typedef int errr;

#undef uint
#define uint uint_hack /*!< ��ޥå���ȥå���Ķ��ǽ�ʣ���򤱤뤿���uint_hack����� / Hack -- prevent problems with non-MACINTOSH */

#undef huge
#define huge huge_hack /*!< WINDOWS�Ķ��ǽ�ʣ���򤱤뤿���huge_hack��� / Hack -- prevent problems with WINDOWS */

#undef byte
#define byte byte_hack /*!< AMIGA�Ķ��ǽ�ʣ���򤱤뤿���byte_hack��� / Hack -- prevent problems with AMIGA */

#undef bool
#define bool bool_hack /*!< C++�Ķ��ǽ�ʣ���򤱤뤿���bool_hack��� Hack -- prevent problems with C++ */


/* Note that "signed char" is not always "defined" */
/* So always use "s16b" to hold small signed values */
/* A signed byte of memory */
/* typedef signed char syte; */

typedef unsigned char byte; /*!< byte����unsighned char�Ȥ������ / Note that unsigned values can cause math problems / An unsigned byte of memory */
typedef char bool; /*!< bool����char�Ȥ������ / Note that a bool is smaller than a full "int" / Simple True/False type */
typedef int sint; /*!< sint����int�Ȥ������ / A signed, standard integer (at least 2 bytes) */
typedef unsigned int uint; /* uint����int�Ȥ������ /  An unsigned, "standard" integer (often pre-defined) */


/* The largest possible signed integer (pre-defined) */
/* typedef long long; */

/* The largest possible unsigned integer */
typedef unsigned long huge;

/* Signed/Unsigned 16 bit value */
#ifdef HAVE_STDINT_H
typedef int16_t s16b;
typedef uint16_t u16b;
#else
typedef signed short s16b;
typedef unsigned short u16b;
#endif

/* Signed/Unsigned 32 bit value */
#ifdef HAVE_STDINT_H
typedef int32_t s32b;
typedef uint32_t u32b;
#else
typedef signed long s32b;
typedef unsigned long u32b;
#endif


typedef s16b IDX;				/*!< ���������ID������� */

typedef s16b FEAT_IDX;			/*!< ����������Ϸ�ID������� */
typedef s16b FLOOR_IDX;			/*!< ����������Ϸ�ID������� */

typedef s16b RACE_IDX;			/*!< ��������Υץ쥤�䡼��²ID������� */
typedef s16b CLASS_IDX;			/*!< ��������Υץ쥤�䡼����ID������� */
typedef s16b MIMIC_RACE_IDX;	/*!< ��������Υץ쥤�䡼�ѿȼ�²ID������� */
typedef s16b PATRON_IDX;		/*!< ��������Υץ쥤�䡼�ѥȥ��ID������� */
typedef s32b ACTION_IDX;		/*!< �ץ쥤�䡼�����߼�äƤ�������ư��ID��� */
typedef s16b BACT_IDX;			/*!< Į�λ��߽�����ID��� */
typedef s16b BACT_RESTRICT_IDX;	/*!< Į�λ��߽����ε�������ID��� */

typedef s16b MONRACE_IDX;		/*!< ��������Υ�󥹥�����²ID������� */
typedef s16b MONSTER_IDX;		/*!< ��������Υ�󥹥�������ID������� */
typedef s16b DUNGEON_IDX;		/*!< ��������Υ��󥸥��ID������� */
typedef s16b REALM_IDX;			/*!< �����������ˡ�ΰ�ID������� */
typedef s16b ARTIFACT_IDX;		/*!< ��������Υ����ƥ��ե�����ID������� */
typedef s16b EGO_IDX;			/*!< �����ƥ२����ID������� */
typedef s16b ACTIVATION_IDX;	/*!< �����ƥ��ȯư����ID������� */
typedef s16b VIRTUES_IDX;		/*!< �����������ID������� */
typedef s16b QUEST_IDX;			/*!< ��������Υ�������ID������� */
typedef byte ROOM_IDX;			/*!< ������ID������� */

typedef s16b INVENTORY_IDX;		/*!< ��������ν����ID������� */
typedef s16b OBJECT_IDX;		/*!< ��������Υ����ƥ�ID������� */
typedef s32b ESSENCE_IDX;		/*!< ���ꥨ�å��󥹤�ID�� */
typedef s16b KIND_OBJECT_IDX;	/*!< ��������Υ١��������ƥ�ID������� */

typedef s32b POSITION;			/*!< ��������κ�ɸ������� */
typedef s16b POSITION_IDX;		/*!< ��������κ�ɸ�ꥹ��ID�� */

typedef s32b ALIGNMENT;		/*!< ����°���η���� */
typedef byte FEAT_SUBTYPE;	/*!< �Ϸ���������� (�ȥ�å׼���/�ѥ��������/Ź�޼���)*/

/*!
 * @var typedef s32b HIT_POINT
 * @brief HP�Ȥ��������̤η����
 * @details 
 * HIT_POINT�ϥץ쥤�䡼�ڤӥ�󥹥�����HP�γ��ͤȤ��������̤η��Ǥ��롣
 */
typedef s32b HIT_POINT;			

/*!
 * @var typedef s32b MANA_POINT
 * @brief MP�Ȥ��������̤η����
 * @details
 * MANA_POINT�ϥץ쥤�䡼��MP�γ��ϤȤ��������̤η��Ǥ��롣
 */
typedef s32b MANA_POINT;		/*!< ���������MP������� */

typedef int ARTIFACT_BIAS_IDX;  /*!< �����ॢ���ƥ��ե����ȤΥХ������� */

typedef s16b HIT_PROB;			/*!< ���������̿�潤���ͤ���� */
typedef s16b BASE_STATUS;		/*!< ��������δ���ǽ���ͷ������ */

typedef s32b MONSTER_NUMBER;	/*!< ��������Υ�󥹥������������ */
typedef s32b ITEM_NUMBER;		/*!< ��������Υ����ƥ��������� */

typedef s16b ACTION_ENERGY;		/*!< ��������ι�ư���ͥ륮��������� */
typedef s16b ARMOUR_CLASS;		/*!< ��������ι�ư�����ޡ����饹������� */
typedef s16b TIME_EFFECT;		/*!< ��������λ��´��֤η������ */
typedef byte CHARACTER_IDX;		/*!< ��������Υ���饯���������Ƽ�ID�η������ */
typedef byte DISCOUNT_RATE;		/*!< ����������Ͱ���Ψ�η������ */
typedef byte SPEED;				/*!< ��������β�®�ͤη���� */

/*!
 * @var typedef s16b ENEGRY
 * @brief ��ư���ͥ륮���η����
 * @details
 * ENERGY�ϥץ쥤�䡼�ȥ�󥹥����ι�ư��������ư���ͥ륮���򼨤�������Ǥ��롣
 */
typedef s16b ENERGY;			/*!< ��������ι�ư���ͥ륮���η���� */


typedef s16b SLEEP_DEGREE;		/*!< ��󥹥����ο�̲�٤η���� */

typedef s16b PLAYER_LEVEL;		/*!< ��������Υץ쥤�䡼��٥�η������ */
typedef int DIRECTION;			/*!< ������������Ѥη���� */
typedef s32b EXP;				/*!< ��������μ�и��ͤη���� */
typedef s16b SUB_EXP;			/*!< ������������и��ͤη���� */
typedef s16b TERM_COLOR;		/*!< �ƥ�����ɽ�����η���� */

typedef s32b OBJECT_TYPE_VALUE;    /*!< ��������Υ����ƥ��ʬ��η���� */
typedef s32b OBJECT_SUBTYPE_VALUE; /*!< ��������Υ����ƥ���ʬ��η���� */
typedef s16b PARAMETER_VALUE;      /*!< ��������Υ����ƥ�ǽ���ͤη���� */
typedef s32b WEIGHT;               /*!< ��������ν��̤η����(�ݥ��) */

typedef int DICE_NUMBER; /*!< ��������Υ��������η���� */
typedef int DICE_SID;    /*!< ��������Υ������̤η���� */
typedef s32b PRICE;      /*!< ��������ζ�۲��ͤη���� */
typedef s16b FEED;       /*!< ��������μ����٤η���� */

typedef u32b STR_OFFSET;      /*!< �ƥ����ȥ��ե��åȤη���� */

typedef s32b DEPTH;     /*!< ��������γ��إ�٥�η���� */
typedef byte RARITY;    /*!< ��������δ����٤η���� */

typedef s32b GAME_TURN;     /*!< ��������Υ�����η���� */
typedef u32b REAL_TIME;     /*!< �»���η���� */

typedef s16b PERCENTAGE;    /*!< ��������Υѡ������ɽ���η����(/100��) */
typedef s16b MULTIPLY;      /*!< �����������Ψ�η����(/10��) */

typedef u32b BIT_FLAGS;     /*!< 32�ӥåȤΥե饰����η���� */
typedef u16b BIT_FLAGS16;   /*!< 16�ӥåȤΥե饰����η���� */
typedef byte BIT_FLAGS8;    /*!< 8�ӥåȤΥե饰����η���� */

typedef s16b XTRA16;     /*!< �����ѿ�16�ӥåȤη���� */
typedef byte XTRA8;      /*!< �����ѿ�8�ӥåȤη���� */

typedef s16b COMMAND_CODE;   /*!< ���ޥ�����Ƥη���� */
typedef s16b COMMAND_ARG;	 /*!< ���ޥ�ɰ����η���� */
typedef s16b COMMAND_NUM;	 /*!< ���ޥ�ɿ��η���� */

typedef int TERM_POSITION;   /*!< ���󥽡���ɽ����ɸ�η���� */
typedef byte SYMBOL_COLOR;    /*!< �����ο��η���� */
typedef byte SYMBOL_CODE;     /*!< ������ʸ���η���� */

typedef s32b MAGIC_NUM1;   /*!< �ץ쥤�䡼��������ˡ��������1�η����*/
typedef byte MAGIC_NUM2;   /*!< �ץ쥤�䡼��������ˡ��������2�η����*/

typedef s32b SPELL_IDX;    /*!< ����ˡ�ΰ�/����ǽ�Ϥ��Ȥμ�ʸID����� */
typedef s16b PROB;         /*!< ��Ψ�νŤߤη���� */
typedef byte FEAT_POWER;   /*!< �Ϸ����٤η���� */

typedef int QUANTITY; /*!< ���󥿡��ե�������λ���Ŀ� */

typedef byte BLOW_METHOD; /*!< ��󥹥������Ƿ����ID */
typedef byte BLOW_EFFECT; /*!< ��󥹥������Ƿ����ID */

typedef int EFFECT_ID; /*!< ����°��ID */

typedef s16b QUEST_TYPE;   /*!< �������Ȥμ���ID */
typedef s16b QUEST_STATUS; /*!< �������Ȥξ���ID */

typedef s16b ACTION_SKILL_POWER; /*!< ��ư��ǽ�� */

typedef int PET_COMMAND_IDX; /*!< �ڥåȤؤλؼ�ID */
typedef byte FF_FLAGS_IDX;   /*!< �Ϸ�����ID */

typedef s16b FEAT_PRIORITY; /*!< �Ϸ��ν̾�ɽ��ͥ���� */

/*** Pointers to all the basic types defined above ***/

typedef real *real_ptr;
typedef errr *errr_ptr;
typedef char *char_ptr;
typedef byte *byte_ptr;
typedef bool *bool_ptr;
typedef sint *sint_ptr;
typedef uint *uint_ptr;
typedef long *long_ptr;
typedef huge *huge_ptr;
typedef s16b *s16b_ptr;
typedef u16b *u16b_ptr;
typedef s32b *s32b_ptr;
typedef u32b *u32b_ptr;
typedef vptr *vptr_ptr;
typedef cptr *cptr_ptr;



/*** Pointers to Functions of special types (for various purposes) ***/

/* A generic function takes a user data and a special data */
typedef errr	(*func_gen)(vptr, vptr);

/* An equality testing function takes two things to compare (bool) */
typedef bool	(*func_eql)(vptr, vptr);

/* A comparison function takes two things and to compare (-1,0,+1) */
typedef sint	(*func_cmp)(vptr, vptr);

/* A hasher takes a thing (and a max hash size) to hash (0 to siz - 1) */
typedef uint	(*func_hsh)(vptr, uint);

/* A key extractor takes a thing and returns (a pointer to) some key */
typedef vptr	(*func_key)(vptr);



#endif

