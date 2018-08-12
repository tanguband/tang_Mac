/*!
 * @file mind.c
 * @brief �ƿ��Ȥ��ü쵻ǽ���� / Special magics
 * @date 2014/01/15
 * @author
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke\n
 * This software may be copied and distributed for educational, research,\n
 * and not for profit purposes provided that this copyright and statement\n
 * are included in all such copies.  Other copyrights may also apply.\n
 * 2005 henkma \n
 * 2014 Deskull rearranged comment for Doxygen.\n
 * @details
 * mind.c�Ȥ��뤬�ºݤˤ�Ķǽ�ϼԡ������ѻա�����Ρ����Ȥ���Ǧ�ԤޤǤ�
 * �ü쵻ǽ��·���Ƽ������Ƥ��롣
 */



#include "angband.h"

/*! �ü쵻ǽ�ΰ����ơ��֥� */
mind_power const mind_powers[5] =
{
  {
    {
      /* Level gained,  cost,  %fail,  name */
#ifdef JP
      { 1,   1,  15, "���"},
      { 2,   1,  20, "���й���"},
      { 3,   2,  25, "�����ν֤�"},
      { 7,   6,  35, "�����θ���"},
      { 9,   7,  50, "��������"},
      { 11,  7,  30, "ǰư�׷���"},
      { 13, 12,  50, "����"},
      { 15, 12,  60, "��������ȥ꡼"},
      { 18, 10,  45, "������ư"},
      { 23, 15,  50, "���ɥ�ʥ�󡦥ɡ��ԥ�"},
      { 26, 28,  60, "�ƥ쥭�ͥ���"},
      { 28, 10,  40, "�������å����ɥ쥤��"},
      { 35, 35,  75, "���η�"},
      { 45,150,  85, "����������"},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
#else
      { 1,   1,  15, "Precognition"},
      { 2,   1,  20, "Neural Blast"},
      { 3,   2,  25, "Minor Displacement"},
      { 7,   6,  35, "Major Displacement"},
      { 9,   7,  50, "Domination"},
      { 11,  7,  30, "Pulverise"},
      { 13, 12,  50, "Character Armour"},
      { 15, 12,  60, "Psychometry" },
      { 18, 10,  45, "Mind Wave" },
      { 23, 15,  50, "Adrenaline Channeling"},
      { 26, 28,  60, "Telekinesis"},
      { 28, 10,  40, "Psychic Drain"},
      { 35, 35,  75, "Psycho-Spear"},
      { 45,150,  85, "The World"},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
#endif
      
    }
  },
  
  {
    {
      /* Level gained,  cost,  %fail,  name */
#ifdef JP
      { 1,   1,  15, "��ζ"},
      { 3,   3,  30, "����"},
      { 5,   6,  35, "�����"},
      { 8,   5,  40, "����ϥ���"},
      { 10,  7,  45, "����ˡ�ɸ�"},
      { 13,  5,  60, "����"},
      { 17, 17,  50, "ŻƮ��"},
      { 20, 20,  50, "����"},
      { 23, 18,  55, "��ζ"},
      { 25, 30,  70, "���ƤĤ���ư"},
      { 28, 26,  50, "�����"},
      { 32, 35,  65, "�����б�"},
      { 38, 42,  75, "Ķ����ϥ���"},
      { 44, 50,  80, "��®��ư"},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
#else
      { 1,   1,  15, "Small Force Ball"},
      { 3,   3,  30, "Flash Light"},
      { 5,   6,  35, "Flying Technique"},
      { 8,   5,  40, "Kamehameha"},
      { 10,  7,  45, "Magic Resistance"},
      { 13,  5,  60, "Improve Force"},
      { 17, 17,  50, "Aura of Force"},
      { 20, 20,  50, "Shock Power"},
      { 23, 18,  55, "Large Force Ball"},
      { 25, 30,  70, "Dispel Magic"},
      { 28, 26,  50, "Summon Ghost"},
      { 32, 35,  65, "Exploding Frame"},
      { 38, 42,  75, "Super Kamehameha"},
      { 44, 50,  80, "Light Speed"},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
#endif
      
    }
  },
  
  {
    {
      /* Level gained,  cost,  %fail,  name */
#ifdef JP
      {  8,  5,  40, "��������"},
      { 15, 20,   0, "�ͷ�"},
      { 20, 15,   0, "�ȥ�å�ʴ��"},
      { 25, 20,  60, "�Ͽ�"},
      { 30, 80,  75, "������"},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
#else
      {  8,  5,  40, "Detect Atmosphere of Menace"},
      { 15, 20,   0, "Charge"},
      { 20, 15,   0, "Smash a Trap"},
      { 25, 20,  60, "Quake"},
      { 30, 80,  75, "Massacre"},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
      { 99,  0,   0, ""},
#endif
      
    }
  },

  {
    {
      /* Level gained,  cost,  %fail,  name */
#ifdef JP
      { 1,   1,  15, "�����ζ�"},
      { 1,   2,  40, "������"},
      { 2,   2,  20, "���Τ�����"},
      { 3,   2,  20, "�Ĥ����"},
      { 5,   3,  35, "������"},
      { 6,   5,  35, "�Ǥ����"},

      { 10,  5,  30, "���б���"},
      { 12, 12,  30, "�����ζ�"},
      { 15, 15,  30, "���դ�"},
      { 19, 13,  30, "��̲��"},
      { 23, 18,  50, "���������쥤"},

      { 25, 20,  40, "��������"},
      { 27, 30,  60, "����ν�"},
      { 29, 30,  60, "�����ѡ��쥤"},
      { 31, 35,  60, "���Ǥθ�"},
      { 33, 50,  80, "���ι�"},

      { 36, 30,  80, "��ȴ��"},
      { 38, 40,  70, "���Ԥζ�"},
      { 40, 50,  55, "��ʬ��"},
      { 43, 55,  70, "����볦"},
      { 46, 70,  75, "��եΡ���ζ�"},
#else
      { 1,   1,  15, "Mirror of Seeing"},
      { 1,   2,  40, "Making a Mirror"},
      { 2,   2,  20, "Drip of Light"},
      { 3,   2,  20, "Warped Mirror"},
      { 5,   3,  35, "Mirror of Light"},
      { 6,   5,  35, "Mirror of Wandering"},

      { 10,  5,  30, "Robe of Dust"},
      { 12, 12,  30, "Banishing Mirror"},
      { 15, 15,  30, "Mirror Clashing"},
      { 19, 13,  30, "Mirror Sleeping"},
      { 23, 18,  50, "Seeker Ray"},

      { 25, 20,  40, "Seal of Mirror"},
      { 27, 30,  60, "Shield of Water"},
      { 29, 30,  60, "Super Ray"},
      { 31, 35,  60, "Illusion Light"},
      { 33, 50,  80, "Mirror Shift"},

      { 36, 30,  80, "Mirror Tunnel"},
      { 38, 40,  70, "Mirror of Recall"},
      { 40, 50,  55, "Multi-Shadow"},
      { 43, 55,  70, "Binding Field"},
      { 46, 70,  75, "Mirror of Ruffnor"},
#endif
      
    }
  },
  
  {
    {
      /* Level gained,  cost,  %fail,  name */
#ifdef JP
      {  1,  1,  20, "�Ű�����"},
      {  2,  2,  25, "����Ĵ��"},
      {  3,  3,  25, "�ձ���"},
      {  5,  3,  30, "�Ѥ���"},
      {  7,  8,  35, "������"},
      {  8, 10,  35, "���Υæ"},
      { 10, 10,  40, "������"},
      { 12, 12,  70, "�Ťθ���"},
      { 15, 10,  50, "���"},
      { 17, 12,  45, "����"},
      { 18, 20,  40, "����"},
      { 20,  5,  50, "Ȭ����΢��"},
      { 22, 15,  55, "����"},
      { 25, 32,  60, "���"},
      { 28, 32,  60, "ž��"},
      { 30, 30,  70, "��ȯ�����"},
      { 32, 40,  40, "����"},
      { 34, 35,  50, "̸����"},
      { 38, 40,  60, "�����б�"},
      { 41, 50,  55, "ʬ��"},
      { 99,  0,   0, ""},
#else
      {  1,  1,  20, "Create Darkness"},
      {  2,  2,  25, "Detect Near"},
      {  3,  3,  25, "Hide in Leafs"},
      {  5,  3,  30, "Kawarimi"},
      {  7,  8,  35, "Absconding"},
      {  8, 10,  35, "Hit and Away"},
      { 10, 10,  40, "Bind Monster"},
      { 12, 12,  70, "Ancient Knowledge"},
      { 15, 10,  50, "Floating"},
      { 17, 12,  45, "Hide in Flame"},
      { 18, 20,  40, "Nyusin"},
      { 20,  5,  50, "Syuriken Spreading"},
      { 22, 15,  55, "Chain Hook"},
      { 25, 32,  60, "Smoke Ball"},
      { 28, 32,  60, "Swap Position"},
      { 30, 30,  70, "Glyph of Explosion"},
      { 32, 40,  40, "Hide in Mud"},
      { 34, 35,  50, "Hide in Mist"},
      { 38, 40,  60, "Rengoku-Kaen"},
      { 41, 50,  55, "Bunshin"},
      { 99,  0,   0, ""},
#endif
      
    }
  },
};

/*! �ü�ǽ�Ϥβ���ʸ���� */
static cptr const mind_tips[5][MAX_MIND_POWERS] =
{
#ifdef JP
{
	"�᤯�����Ƥθ������󥹥������Τ��롣��٥�5���/�⡢15��Ʃ���ʥ�󥹥�����30�Ǻ����ȥ����ƥ���ΤǤ���褦�ˤʤ롣��٥�20�Ǽ��դ��Ϸ����Τ���45�Ǥ��γ����Τ�ʵפ˾Ȥ餷�����󥸥����Τ��٤ƤΥ����ƥ���Τ��롣��٥�25�ǰ�����֥ƥ�ѥ��������롣",
	"��������Υӡ���ޤ��ϵ�����ġ�",
	"���Υ�Υƥ�ݡ��Ȥ򤹤롣",
	"���Υ�Υƥ�ݡ��Ȥ򤹤롣",
	"��٥�30̤���ǡ���󥹥�����ۯ۰�����𤫶��ݤ����������ġ���٥�30�ʾ�ǻ볦������ƤΥ�󥹥�����̥λ���롣�񹳤�����̵����",
	"�ƥ쥭�ͥ����ε�����ġ�",
	"������֡�AC��徺�����롣��٥뤬�夬��ȡ������ꡢ�䵤���ŷ⡢�Ǥ������������롣",
	"��٥�25̤���ǡ������ƥ��ʷ�ϵ����Τ롣��٥�25�ʾ�ǡ������ƥ����ꤹ�롣",
	"��٥�25̤���ǡ���ʬ���濴�Ȥ�����������ε��ȯ�������롣��٥�25�ʾ�ǡ��볦������ƤΥ�󥹥������Ф������������Ԥ���",
	"���ݤ�ۯ۰������������ҡ�����ʬ���Ĳ�®���֤Ǥʤ����HP�������������롣����ˡ�������֥ҡ�����ʬ�ˤʤꡢ��®���롣",
	"�����ƥ��ʬ��­���ذ�ư�����롣",
	"��������ε�����ġ���󥹥�����̿�椹��ȡ�0��1.5��������񤹤롣�񹳤���ʤ���С�MP���������롣",
	"̵�������ڤ��������ʥ��ͥ륮���Υӡ�������ġ�",
	"����ߤ�롣��MP����񤷡����񤷤�MP�˱�����Ĺ������ߤ�Ƥ����롣",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
},
{
	"�������������ε�����ġ�",
	"�������Ȥ餷�Ƥ����ϰϤ��������Τ�ʵפ����뤯���롣",
	"������֡�������⤱��褦�ˤʤ롣",
	"������û�����Υӡ�������ġ�",
	"������֡���ˡ�ɸ�ǽ�Ϥ�徺�����롣",
	"�������롣��������ȽѤΰ��ϤϾ夬�ꡢ��³���֤�Ĺ���ʤ롣���ä����ϻ��֤ȤȤ�˳Ȼ����롣���ꤹ�����˽������������롣",
	"������֡����⤷�Ƥ������ƤΥ�󥹥�������Ĥ��륪�����Ż����",
	"�٤�Υ�󥹥������Ф��Ƶ���֤Ĥ����᤭�ȤФ���",
	"�礭�ʵ��ε�����ġ�",
	"��󥹥���1�Τˤ����ä���ˡ�������롣",
	"1�Τ�ͩ��򾤴����롣",
	"��ʬ���濴�Ȥ���Ķ����ʱ�ε��ȯ�������롣",
	"������Ĺ�������Ϥʵ��Υӡ�������ġ�",
	"���Ф餯�δ֡�����®��ư�����Ȥ��Ǥ��롣",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
},
{
	"�᤯�λ׹ͤ��뤳�Ȥ��Ǥ����󥹥������Τ��롣",
	"���⤷���塢ȿ��¦��ȴ���롣",
	"�ȥ�åפˤ����뤬�����Υȥ�åפ��˲����롣",
	"���ϤΥ��󥸥����ɤ餷���ɤȾ��������������Ѥ��롣",
	"�������˸����äƹ��⤹�롣",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
},
{
	"�᤯�����ƤΥ�󥹥������Τ��롣��٥�15��Ʃ���ʥ�󥹥������Τ��롣��٥�25�ǰ�����֥ƥ�ѥ��������롣��٥�35�Ǽ��դ��Ϸ����Τ��롣���Ƥθ��̤ϡ����ξ�Ǥʤ��ȥ�٥�4����;�פ�ɬ�פˤʤ롣",
	"��ʬ�Τ��뾲�ξ�˶����������롣",
	"������������ġ���٥�10�ʾ�Ǥ϶��ξ�ǻȤ��ȥӡ���ˤʤ롣",
	"���Υ�Υƥ�ݡ��Ȥ򤹤롣",
	"��ʬ�μ��Ϥ䡢 ��ʬ�Τ����������Τ����뤯���롣",
	"���Υ�Υƥ�ݡ��Ȥ򤹤롣",
	"������֡����Υ����餬�դ�����������������ҤΥ��᡼����ȿ�⤷������˶��ξ�ˤ��������Υ�Υƥ�ݡ��Ȥ򤹤롣",
	"��󥹥�����ƥ�ݡ��Ȥ�����ӡ�������ġ��񹳤�����̵����",
	"���Ҥε�����ġ�",
	"���Ƥζ��μ����̲��ε��ȯ�������롣",
	"�������åȤ˸����ä����ϤΥӡ�������ġ�����̿�椹��ȡ����ζ����˲������̤ζ��˸����ä�ȿ�ͤ��롣",
	"���ξ�Υ�󥹥�����ä���롣",
	"������֡�AC��徺�����롣��٥�32��ȿ�ͤ��դ�����٥�40����ˡ�ɸ椬�夬�롣",
	"�������åȤ˸����äƶ��Ϥ����ϤΥӡ�������ġ�����̿�椹��ȡ����ζ����˲�����8���������ϤΥӡ����ȯ�������롣",
	"�볦��Υ�󥹥�����®������ۯ۰�Ȥ��������𤵤������ݤ��������㤵���롣���ξ�ǻȤ��Ȱ��Ϥ��⤤��",
	"�ե������Ѥ��롣���ξ�Ǥ����Ȥ��ʤ���",
	"û��Υ��λ��ꤷ�����˥ƥ�ݡ��Ȥ��롣",
	"�Ͼ�ˤ���Ȥ��ϥ��󥸥��κǿ����ء����󥸥��ˤ���Ȥ����Ͼ�ؤȰ�ư���롣",
	"���Ƥι��⤬��1/2�γ�Ψ��̵���ˤʤ롣",
	"�볦���2�Ĥζ��ȥץ쥤�䡼��ĺ���Ȥ��뻰�ѷ����ΰ�ˡ����Ϥη볦��ȯ�������롣",
	"������֡����᡼��������ʤ��ʤ�Хꥢ��ĥ�롣�ڤ줿�ִ֤˾������������񤹤�Τ���ա�",
},
{
	"Ⱦ��3���⤫����������Ť����롣",
	"�᤯�����Ƥθ������󥹥������Τ��롣��٥�5���/��/���ʡ���٥�15�ǥ����ƥ���ΤǤ���褦�ˤʤ롣��٥�45�Ǥ��γ����Τ��Ϸ������ƤΥ����ƥ���Τ��롣",
	"���Υ�Υƥ�ݡ��Ȥ򤹤롣",
	"�����������ִ֤˥ƥ�ݡ��Ȥ򤹤�褦�ˤʤ롣���Ԥ���Ȥ��ι���Υ��᡼��������롣�ƥ�ݡ��Ȥ˼��Ԥ��뤳�Ȥ⤢�롣",
	"���Υ�Υƥ�ݡ��Ȥ򤹤롣",
	"���⤷�Ƥ����˥ƥ�ݡ��Ȥ��롣",
	"Ũ1�Τ�ư���������롣��ˡ�����󥹥������ξ�������񹳤��줿���ˤ�̵����",
	"�����ƥ���̤��롣",
	"������֡���ͷǽ�Ϥ����롣",
	"��ʬ���濴�Ȥ����Фε��ȯ���������ƥ�ݡ��Ȥ��롣����ˡ�������ֱ���Ф������������롣�����ˤ�����������Ѥ��롣",
	"���᤯���˶��깶�⤹�롣",
	"�������������8�󤯤��Ӥ��ꤲ�롣",
	"Ũ��1�μ�ʬ�ζ᤯�˰����󤻤롣",
	"���᡼���Τʤ�����ε�����ġ�",
	"1�ΤΥ�󥹥����Ȱ��֤�򴹤��롣",
	"��ʬ�Τ��뾲�ξ�ˡ���󥹥������̤����ȯ���ƥ��᡼����Ϳ����롼���������",
	"������֡�Ⱦʪ�������ɤ��̤�ȴ������褦�ˤʤ롣����ˡ�������ֻ��ؤ����������롣�����ˤ�����������Ѥ��롣",
	"��ʬ���濴�Ȥ���Ķ������ǡ���塢����ε��ȯ���������ƥ�ݡ��Ȥ��롣",
	"������������˲��󤫱꤫�Ϲ����ץ饺�ޤΥӡ�������ġ�",
	"���Ƥι��⤬��1/2�γ�Ψ��̵���ˤʤ롣",
	"",
},
#else
{
	"Detects visible monsters in your vicinity and more and more. Detects traps and doors at level 5, invisible monsters at level 15, items at level 30. And magic mapping at level 20. Lights and know the whole level at level 45. Gives telepathy at level 25.",
	"Fires a beam or ball which inflicts PSI damage.",
	"Teleport short distance.",
	"Teleport long distance.",
	"Stuns, confuses or scares a monster. Or attempts to charm all monsters in sight at level 30.",
	"Fires a ball which hurts monsters with telekinesis.",
	"Gives stone skin and some resistance to elements for a while. The level increased, the more number of resistances given.",
	"Gives feeling of an item. Or identify an item at level 25.",
	"Generate a ball centered on you which inflict monster with PSI damage. Or inflict all monsters with PSI damage at level 25.",
	"Removes fear and stun. Gives heroism and speed. Heals HP a little unless you already have heroism and temporal speed boost.",
	"Pulls a distant item close to you.",
	"Fires a ball which damages, co. Absorbing is takes more turns which from 0 to 1.5.",
	"Fires a beam of pure energy which penetrate the invulnerability barrier.",
	"Stops time. Consumes all of your SP. The more consumes SP, the longer duration of spell.",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
},
{
	"Fires a very small energy ball.",
	"Lights up nearby area and the inside of a room permanently.",
	"Gives levitaion a while.",
	"Fires a short energy beam.",
	"Gives magic resistance for a while.",
	"Improves spirit energy power temporaly. Improved spirit energy will be more and more powerfull or have longer duration. Too many improving results in uncontrollable explosion of spirit energy.",
	"Gives aura which damages all monsters which attacked you for a while.",
	"Damages an adjacent monster, and blow it away.",
	"Fires a large energy ball.",
	"Dispels all magics which is effecting a monster.",
	"Summons ghosts.",
	"Generates a huge ball of frame which centered on you.",
	"Fires a long, powerful energy beam.",
	"Gives extremely fast speed.",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
},
{
	"Detects all monsters except mindless in your vicinity.",
	"Attacks monster with your weapons normaly, then move through counter side of the monster.",
	"Sets off a trap, then destroy that trap.",
	"Shakes dungeon structure, and results in random swaping of floors and walls.",
	"Attacks all adjacent monsters.",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
},
{
	"Detects visible monsters in your vicinity and more and more. Detects invisible monsters at level 15. Gives telepathy at level 25. Magic mapping at level 35. All of effects need 4 more levels unless on a mirror.",
	"Makes a mirror under you.",
	"Fires bolt of flash light. Or fires beam of light on a mirror at level 10.",
	"Teleport short distance.",
	"Lights up nearby area and the inside of a room permanently.",
	"Teleport long distance.",
	"Gives aura of shards of mirror for a while. It cause counter attack to monsters which attacks you.",
	"Teleports all monsters on the line away unless resisted.",
	"Fires a ball of shards.",
	"Generate balls which send monsters to sleep on all mirrors in the whole level.",
	"Fires a beam of mana. If the beam hit a mirror, it breaks that mirror and reflects toward another mirror.",
	"Eliminates a monster on a mirror from current dungeon level.",
	"Gives bonus to AC. Gives reflection at level 32. Gives magic resistance at level 40.",
	"Fires a powerful beam of mana. If the beam hit a mirror, it breaks that mirror and fires 8 beams of mana to 8 different directions from that point.",
	"Attempts to slow, stun, confuse, scare, freeze all monsters in sight. Gets more power on a mirror.",
	"Recreates current dungeon level. Can only be used on a mirror.",
	"Teleport to given location.",
	"Recalls player from dungeon to town, or from town to the deepest level of dungeon.",
	"Completely protects you from any attacks at one in two chance.",
	"Generates a magical triangle which damages all monsters in the area. The vertices of the triangle is you and two mirrors in sight.",
	"Generates barrier which completly protect you from almost all damages. Takes a few your turns when the barrier breaks or duration time is exceeded.",
},

{
	"Darken nearby area and inside of a room.",
	"Detects visible monsters in your vicinity and more and more. Detects traps, doors and stairs at level 5, items at level 15. Lights and know the whole level at level 45.",
	"Teleport short distance.",
	"Teleport as you recieve an attack. Might be able to teleport just before recieveing damages at higher level.",
	"Teleport long distance.",
	"Attack and teleport in a time.",
	"Attempt to freeze a monster.",
	"Identifies an item.",
	"Gives levitation for a while.",
	"Generate a fire ball and teleport in a time. Gives resistance to fire for a while. This resistance can be added to which from equipment for more powerful resistance.",
	"Steps close to a monster and attacks at a time.",
	"Shoots 8 iron Spikes in 8 random directions.",
	"Teleport a monster to a place adjacent to you.",
	"Releases a confusion ball which doesn't inflict any damage.",
	"Swaps positions of you and a monster.",
	"Sets a glyph under you. The glyph will explode when a monster moves on it.",
	"Become ethereal form which gives ability to pass walls, and gives resistance to acid for a while. This resistance can be added to which from equipment for more powerful resistance.",
	"Generates huge balls of poison, drain life and confusion, then teleport in a time.",
	"Fires some number of beams of fire, nether or plasma in random directions.",
	"Creates shadows of yourself which gives you abillity to completely evade any attacks at one in two chance for a while.",
	"",
},
#endif
};

/*!
 * @brief �ü쵻ǽ�θ��̾����ޤȤ᤿�ե����ޥåȤ��֤�
 * @param p ������֤�ʸ���󻲾ȥݥ���
 * @param use_mind ��������ü쵻ǽID
 * @param power ��󥹥�����ˡ��ID
 * @return �ʤ�
 */
void mindcraft_info(char *p, int use_mind, int power)
{
#ifdef JP
	cptr s_dam = "»��:";
	cptr s_dur = "����:";
	cptr s_range = "�ϰ�:";
#else
	cptr s_dam = "dam ";
	cptr s_dur = "dur ";
	cptr s_range = "range ";
#endif
	int plev = p_ptr->lev;

	strcpy(p, "");

	switch (use_mind)
	{
	case MIND_MINDCRAFTER:
		switch (power)
		{
		case 0:  break;
		case 1:  sprintf(p, " %s%dd%d", s_dam, 3 + ((plev - 1) / 4), 3 + plev/15); break;
		case 2:  sprintf(p, " %s10", s_range); break;
		case 3:  sprintf(p, " %s%d", s_range, plev * 5);  break;
		case 4:  break;
		case 5: sprintf(p, " %s%dd8", s_dam, 8 + ((plev - 5) / 4));  break;
		case 6:  sprintf(p, " %s%d", s_dur, plev);  break;
		case 7:  break;
		case 8:  sprintf(p, (plev < 25 ? " %s%d" : " %sd%d"), s_dam, (plev < 25 ? plev * 3 / 2 : plev * ((plev - 5) / 10 + 1))); break;
		case 9:  sprintf(p, " %s10+d%d", s_dur, plev * 3 / 2);  break;
#ifdef JP
		case 10: sprintf(p, " �������:%d.%dkg", lbtokg1(plev * 15),lbtokg2(plev * 15));  break;
#else
		case 10: sprintf(p, " max wgt %d", plev * 15);  break;
#endif
		case 11: sprintf(p, " %s%dd6", s_dam, plev / 2);  break;
		case 12: sprintf(p, " %sd%d+%d", s_dam, plev * 3, plev * 3); break;
		case 13: sprintf(p, _(" ��ư:%ld��", " %ld acts."), (long int)(p_ptr->csp + 100-p_ptr->energy_need - 50)/100); break;
		}
		break;
	case MIND_KI:
	{
		int boost = P_PTR_KI;

		if (heavy_armor()) boost /= 2;

		switch (power)
		{
		case 0:  sprintf(p, " %s%dd4", s_dam, 3 + ((plev - 1) / 5) + boost / 12); break;
		case 1:  break;
		case 2:  sprintf(p, " %s%d+d30", s_dur, 30 + boost / 5); break;
		case 3:  sprintf(p, " %s%dd5", s_dam, 5 + ((plev - 1) / 5) + boost / 10); break;
		case 4:  sprintf(p, " %s%d+d20", s_dur, 20 + boost / 5); break;
		case 5:  break;
		case 6:  sprintf(p, " %s%d+d%d", s_dur, 15 + boost / 7, plev / 2); break;
		case 7:  sprintf(p, " %s%dd8", s_dam, 8 + ((plev - 5) / 4) + boost / 12); break;
		case 8:  sprintf(p, " %s10d6+%d", s_dam, plev * 3 / 2 + boost * 3 / 5); break;
		case 9:  break;
		case 10: sprintf(p, _(" ����%d��", " max %d"), 1+boost/100); break;
		case 11: sprintf(p, " %s%d", s_dam, 100 + plev + boost); break;
		case 12: sprintf(p, " %s%dd15", s_dam, 10 + plev / 2 + boost * 3 / 10); break;
		case 13: sprintf(p, _(" ��ư:%d+d16��", " %d+d16 acts"), 16+boost/20); break;
		}
		break;
	}
	case MIND_MIRROR_MASTER:
	{
		switch (power)
		{
		case 0:  break;
		case 1:  break;
		case 2:  sprintf(p, " %s%dd4", s_dam,  3 + ((plev - 1) / 5) ); break;
		case 3:  sprintf(p, " %s10", s_range); break;
		case 4:  break;
		case 5:  sprintf(p, " %s%d", s_range, plev *5); break;
		case 6:  sprintf(p, " %s20+d20", s_dur);  break;
		case 7:  break;
		case 8:  sprintf(p, " %s%dd8", s_dam, 8+((plev -5)/4) ); break;
		case 9:  break;
		case 10: sprintf(p, " %s%dd8", s_dam, 11+(plev-5)/4 ); break;
		case 11: break;
		case 12: sprintf(p, " %s20+d20", s_dur);  break;
		case 13: sprintf(p, " %s150+d%d", s_dam, plev*2 ); break;
		case 14: break;
		case 15: break;
		case 16: sprintf(p, " %s%d", s_range, plev/2 +10); break;
		case 17: break;
		case 18: sprintf(p, " %s6+d6", s_dur);  break;
		case 19: sprintf(p, " %s%d", s_dam, plev*11+5 ); break;
		case 20: sprintf(p, " %s4+d4", s_dur);  break;
		}
		break;
	}
	case MIND_NINJUTSU:
	{
		switch (power)
		{
		case 0:  break;
		case 1:  break;
		case 2:  sprintf(p, " %s10", s_range); break;
		case 3:  break;
		case 4:  sprintf(p, " %s%d", s_range , plev *5); break;
		case 5:  sprintf(p, " %s30", s_range); break;
		case 6:  break;
		case 7:  break;
		case 8:  sprintf(p, " %s20+d20", s_dur);  break;
		case 9:  sprintf(p, " %s%d", s_dam, (50+plev)/2 ); break;
		case 10: break;
		case 11: break;
		case 12: break;
		case 13: break;
		case 14: break;
		case 15: break;
		case 16: sprintf(p, " %s%d+d%d", s_dur, plev/2, plev/2);  break;
		case 17: sprintf(p, " %s%d*3", s_dam, (75+plev*2/3)/2 ); break;
		case 18: sprintf(p, " %s%dd10", s_dam, 6+plev/8 ); break;
		case 19: sprintf(p, " %s6+d6", s_dur);  break;
		}
		break;
	}
	}
}

/*!
 * @brief ���Ѳ�ǽ���ü쵻ǽ�����򤹤� /
 * Allow user to choose a mindcrafter power.
 * @param sn ���򤷤��ü쵻ǽID������󥻥�ξ��-1������������ξ��-2���֤�
 * @param only_browse �����򸫤�����ξ��TRUE���֤�
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
static bool_hack get_mind_power(SPELL_IDX *sn, bool only_browse)
{
	SPELL_IDX i;
	int             num = 0;
	int             y = 1;
	int             x = 10;
	int             minfail = 0;
	int             plev = p_ptr->lev;
	int             chance = 0;
	int             ask = TRUE;
	char            choice;
	char            out_val[160];
	char            comment[80];
	cptr            p;
	COMMAND_CODE code;
	mind_type       spell;
	const mind_power      *mind_ptr;
	bool            flag, redraw;
	int             use_mind;
	int menu_line = (use_menu ? 1 : 0);

	switch (p_ptr->pclass)
	{
	case CLASS_MINDCRAFTER:
	{
		use_mind = MIND_MINDCRAFTER;
		p = _("Ķǽ��", "mindcraft");
		break;
	}
	case CLASS_FORCETRAINER:
	{
		use_mind = MIND_KI;
		p = _("������", "Force");
		break;
	}
	case CLASS_BERSERKER:
	{
		use_mind = MIND_BERSERKER;
		p = _("��", "brutal power");
		break;
	}
	case CLASS_MIRROR_MASTER:
	{
		use_mind = MIND_MIRROR_MASTER;
		p = _("����ˡ", "magic");
		break;
	}
	case CLASS_NINJA:
	{
		use_mind = MIND_NINJUTSU;
		p = _("Ǧ��", "ninjutsu");
		break;
	}
	default:
	{
		use_mind = 0;
		p = _("Ķǽ��", "mindcraft");
		break;
	}
	}
	mind_ptr = &mind_powers[use_mind];

	/* Assume cancelled */
	*sn = (-1);

#ifdef ALLOW_REPEAT /* TNB */

	/* Get the spell, if available */

	if (repeat_pull(&code))
	{
		*sn = (SPELL_IDX)code;
		/* Hack -- If requested INVEN_FORCE(1111), pull again */
		if (*sn == INVEN_FORCE) repeat_pull(&code);
		*sn = (SPELL_IDX)code;

		/* Verify the spell */
		if (mind_ptr->info[*sn].min_lev <= plev)
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

    for (i = 0; i < MAX_MIND_POWERS; i++)
	{
	  if (mind_ptr->info[i].min_lev <= plev)
	    {
	      num++;
	    }
	}

    /* Build a prompt (accept all spells) */
    if (only_browse)
	{
		(void) strnfmt(out_val, 78, 
					_("(%^s %c-%c, '*'�ǰ���, ESC) �ɤ�%s�ˤĤ����Τ�ޤ�����", "(%^ss %c-%c, *=List, ESC=exit) Use which %s? "),
				       p, I2A(0), I2A(num - 1), p);
	}
	else
	{
		(void) strnfmt(out_val, 78, 
					_("(%^s %c-%c, '*'�ǰ���, ESC) �ɤ�%s��Ȥ��ޤ�����", "(%^ss %c-%c, *=List, ESC=exit) Use which %s? "),
						p, I2A(0), I2A(num - 1), p);
	}

	if (use_menu && !only_browse) screen_save();
	/* Get a spell from the user */

	choice= (always_show_list || use_menu) ? ESCAPE:1 ;
	while (!flag)
	{
		if(choice==ESCAPE) choice = ' '; 
		else if( !get_com(out_val, &choice, TRUE) )break;

		if (use_menu && choice != ' ')
		{
			switch(choice)
			{
				case '0':
				{
					if (!only_browse) screen_load();
					return (FALSE);
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
		}
		/* Request redraw */
		if ((choice == ' ') || (choice == '*') || (choice == '?') || (use_menu && ask))
		{
			/* Show the list */
			if (!redraw || use_menu)
			{
				char psi_desc[80];
				bool has_weapon[2];

				/* Show list */
				redraw = TRUE;

				/* Save the screen */
				if (!only_browse && !use_menu) screen_save();

				/* Display a list of spells */
				prt("", y, x);
				put_str(_("̾��", "Name"), y, x + 5);

#ifdef JP
put_str(format("Lv   %s   ��Ψ ����", ((use_mind == MIND_BERSERKER) || (use_mind == MIND_NINJUTSU)) ? "HP" : "MP"), y, x + 35);
#else
put_str(format("Lv   %s   Fail Info", ((use_mind == MIND_BERSERKER) || (use_mind == MIND_NINJUTSU)) ? "HP" : "MP"), y, x + 35);
#endif
				has_weapon[0] = buki_motteruka(INVEN_RARM);
				has_weapon[1] = buki_motteruka(INVEN_LARM);

				/* Dump the spells */
				for (i = 0; i < MAX_MIND_POWERS; i++)
				{
					int mana_cost;

					/* Access the spell */
					spell = mind_ptr->info[i];

					if (spell.min_lev > plev)   break;

					chance = spell.fail;

					mana_cost = spell.mana_cost;
					if (chance)
					{

						/* Reduce failure rate by "effective" level adjustment */
						chance -= 3 * (plev - spell.min_lev);

						/* Reduce failure rate by INT/WIS adjustment */
						chance -= 3 * (adj_mag_stat[p_ptr->stat_ind[mp_ptr->spell_stat]] - 1);

						if (use_mind == MIND_KI)
						{
							if (heavy_armor()) chance += 20;
							if (p_ptr->icky_wield[0]) chance += 20;
							else if (has_weapon[0]) chance += 10;
							if (p_ptr->icky_wield[1]) chance += 20;
							else if (has_weapon[1]) chance += 10;
							if (i == 5)
							{
								int j;
								for (j = 0; j < P_PTR_KI / 50; j++)
									mana_cost += (j+1) * 3 / 2;
							}
						}

						/* Not enough mana to cast */
						if ((use_mind != MIND_BERSERKER) && (use_mind != MIND_NINJUTSU) && (mana_cost > p_ptr->csp))
						{
							chance += 5 * (mana_cost - p_ptr->csp);
						}

						chance += p_ptr->to_m_chance;

						/* Extract the minimum failure rate */
						minfail = adj_mag_fail[p_ptr->stat_ind[mp_ptr->spell_stat]];

						/* Minimum failure rate */
						if (chance < minfail) chance = minfail;

						/* Stunning makes spells harder */
						if (p_ptr->stun > 50) chance += 25;
						else if (p_ptr->stun) chance += 15;

						if (use_mind == MIND_KI)
						{
							if (heavy_armor()) chance += 5;
							if (p_ptr->icky_wield[0]) chance += 5;
							if (p_ptr->icky_wield[1]) chance += 5;
						}
						/* Always a 5 percent chance of working */
						if (chance > 95) chance = 95;
					}

					/* Get info */
					mindcraft_info(comment, use_mind, i);

					if (use_menu)
					{
						if (i == (menu_line-1)) strcpy(psi_desc, _("  �� ", "  >  "));
						else strcpy(psi_desc, "     ");
					}
					else
						sprintf(psi_desc, "  %c) ", I2A(i));
					/* Dump the spell --(-- */
					strcat(psi_desc,
					       format("%-30s%2d %4d%s %3d%%%s",
						      spell.name, spell.min_lev, mana_cost,
#ifdef JP
						      (((use_mind == MIND_MINDCRAFTER) && (i == 13)) ? "��" : "  "), 
#else
						      (((use_mind == MIND_MINDCRAFTER) && (i == 13)) ? "~ " : "  "), 
#endif
						      chance, comment));
					prt(psi_desc, y + i + 1, x);
				}

				/* Clear the bottom line */
				prt("", y + i + 1, x);
			}

			/* Hide the list */
			else if (!only_browse)
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
			ask = isupper(choice);

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
		spell = mind_ptr->info[i];

		/* Verify it */
		if (ask)
		{
			char tmp_val[160];

			/* Prompt */
			(void) strnfmt(tmp_val, 78, _("%s��Ȥ��ޤ�����", "Use %s? "), spell.name);

			/* Belay that order */
			if (!get_check(tmp_val)) continue;
		}

		/* Stop the loop */
		flag = TRUE;
	}

	/* Restore the screen */
	if (redraw && !only_browse) screen_load();

	/* Show choices */
	p_ptr->window |= (PW_SPELL);

	/* Window stuff */
	window_stuff();

	/* Abort if needed */
	if (!flag) return (FALSE);

	/* Save the choice */
	(*sn) = i;

#ifdef ALLOW_REPEAT /* TNB */

	repeat_push((COMMAND_CODE)i);

#endif /* ALLOW_REPEAT -- TNB */

	/* Success */
	return (TRUE);
}

/*!
 * @brief Ķǽ�Ϥ�ȯư /
 * do_cmd_cast calls this function if the player's class is 'mindcrafter'.
 * @param spell ȯư�����ü쵻ǽ��ID
 * @return ������¹Ԥ�����TRUE������󥻥뤷�����FALSE���֤���
 */
static bool cast_mindcrafter_spell(int spell)
{
	int             b = 0;
	int             dir;
	TIME_EFFECT t;
	PLAYER_LEVEL plev = p_ptr->lev;

	/* spell code */
	switch (spell)
	{
	case 0:   /* Precog */
		if (plev > 44)
		{
			chg_virtue(V_KNOWLEDGE, 1);
			chg_virtue(V_ENLIGHTEN, 1);
			wiz_lite(FALSE);
		}
		else if (plev > 19)
			map_area(DETECT_RAD_MAP);

		if (plev < 30)
		{
			b = detect_monsters_normal(DETECT_RAD_DEFAULT);
			if (plev > 14) b |= detect_monsters_invis(DETECT_RAD_DEFAULT);
			if (plev > 4)  {
				b |= detect_traps(DETECT_RAD_DEFAULT, TRUE);
				b |= detect_doors(DETECT_RAD_DEFAULT);
			}
		}
		else
		{
			b = detect_all(DETECT_RAD_DEFAULT);
		}

		if ((plev > 24) && (plev < 40))
			set_tim_esp((TIME_EFFECT)plev, FALSE);

		if (!b) msg_print(_("�����ʵ������롣", "You feel safe."));

		break;
	case 1:
		/* Mindblast */
		if (!get_aim_dir(&dir)) return FALSE;

		if (randint1(100) < plev * 2)
			fire_beam(GF_PSI, dir, damroll(3 + ((plev - 1) / 4), (3 + plev / 15)));
		else
			fire_ball(GF_PSI, dir, damroll(3 + ((plev - 1) / 4), (3 + plev / 15)), 0);
		break;
	case 2:
		/* Minor displace */
		teleport_player(10, 0L);
		break;
	case 3:
		/* Major displace */
		teleport_player(plev * 5, 0L);
		break;
	case 4:
		/* Domination */
		if (plev < 30)
		{
			if (!get_aim_dir(&dir)) return FALSE;

			fire_ball(GF_DOMINATION, dir, plev, 0);
		}
		else
		{
			charm_monsters(plev * 2);
		}
		break;
	case 5:
		/* Fist of Force  ---  not 'true' TK  */
		if (!get_aim_dir(&dir)) return FALSE;

		fire_ball(GF_TELEKINESIS, dir, damroll(8 + ((plev - 5) / 4), 8),
			(plev > 20 ? (plev - 20) / 8 + 1 : 0));
		break;
	case 6:
		/* Character Armour */
		set_shield((TIME_EFFECT)plev, FALSE);
		if (plev > 14) set_oppose_acid((TIME_EFFECT)plev, FALSE);
		if (plev > 19) set_oppose_fire((TIME_EFFECT)plev, FALSE);
		if (plev > 24) set_oppose_cold((TIME_EFFECT)plev, FALSE);
		if (plev > 29) set_oppose_elec((TIME_EFFECT)plev, FALSE);
		if (plev > 34) set_oppose_pois((TIME_EFFECT)plev, FALSE);
		break;
	case 7:
		/* Psychometry */
		if (plev < 25)
			return psychometry();
		else
			return ident_spell(FALSE);
	case 8:
		/* Mindwave */
		msg_print(_("������Ǳ���ʤ�����ư��ȯ����������", "Mind-warping forces emanate from your brain!"));

		if (plev < 25)
			project(0, 2 + plev / 10, p_ptr->y, p_ptr->x,
			(plev * 3), GF_PSI, PROJECT_KILL, -1);
		else
			(void)mindblast_monsters(randint1(plev * ((plev - 5) / 10 + 1)));
		break;
	case 9:
		/* Adrenaline */
		set_afraid(0);
		set_stun(0);

		/*
		 * Only heal when Adrenalin Channeling is not active. We check
		 * that by checking if the player isn't fast and 'heroed' atm.
		 */
		if (!IS_FAST() || !IS_HERO())
		{
			hp_player(plev);
		}

		t = 10 + randint1((plev * 3) / 2);
		set_hero(t, FALSE);
		/* Haste */
		(void)set_fast(t, FALSE);
		break;
	case 10:
		/* Telekinesis */
		if (!get_aim_dir(&dir)) return FALSE;

		fetch(dir, plev * 15, FALSE);

		break;
	case 11:
		/* Psychic Drain */
		if (!get_aim_dir(&dir)) return FALSE;

		b = damroll(plev / 2, 6);

		/* This is always a radius-0 ball now */
		if (fire_ball(GF_PSI_DRAIN, dir, b, 0))
			p_ptr->energy_need += randint1(150);
		break;
	case 12:
		/* psycho-spear */
		if (!get_aim_dir(&dir)) return FALSE;

		fire_beam(GF_PSY_SPEAR, dir, randint1(plev*3)+plev*3);
		break;
	case 13:
	{
		if (world_player)
		{
			msg_print(_("���˻��ϻߤޤäƤ��롣", "Time is already stopped."));
			return (FALSE);
		}
		world_player = TRUE;
		msg_print(_("�ֻ��衪��", "You yell 'Time!'"));
		msg_print(NULL);

		/* Hack */
		p_ptr->energy_need -= 1000 + (100 + p_ptr->csp - 50)*TURNS_PER_TICK/10;

		/* Redraw map */
		p_ptr->redraw |= (PR_MAP);

		/* Update monsters */
		p_ptr->update |= (PU_MONSTERS);

		/* Window stuff */
		p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);

		handle_stuff();
		break;
	}
	default:
		msg_print(_("�ʤˡ�", "Zap?"));
	}

	return TRUE;
}

/*!
 * @brief �����Ѥ�ȯư /
 * do_cmd_cast calls this function if the player's class is 'ForceTrainer'.
 * @param spell ȯư�����ü쵻ǽ��ID
 * @return ������¹Ԥ�����TRUE������󥻥뤷�����FALSE���֤���
 */
static bool cast_force_spell(int spell)
{
	DIRECTION dir;
	int plev = p_ptr->lev;
	int boost = P_PTR_KI;

	if (heavy_armor()) boost /= 2;

	/* spell code */
	switch (spell)
	{
	case 0:
		if (!get_aim_dir(&dir)) return FALSE;
		fire_ball(GF_MISSILE, dir, damroll(3 + ((plev - 1) / 5) + boost / 12, 4), 0);
		break;
	case 1:
		(void)lite_area(damroll(2, (plev / 2)), (plev / 10) + 1);
		break;
	case 2:
		set_tim_levitation(randint1(30) + 30 + boost / 5, FALSE);
		break;
	case 3:
		project_length = plev / 8 + 3;
		if (!get_aim_dir(&dir)) return FALSE;

		fire_beam(GF_MISSILE, dir, damroll(5 + ((plev - 1) / 5) + boost / 10, 5));
		break;
	case 4:
		set_resist_magic(randint1(20) + 20 + boost / 5, FALSE);
		break;
	case 5:
		msg_print(_("�������ä���", "You improved the Force."));
		P_PTR_KI += (70 + plev);
		p_ptr->update |= (PU_BONUS);
		if (randint1(P_PTR_KI) > (plev * 4 + 120))
		{
			msg_print(_("����˽��������", "The Force exploded!"));
			fire_ball(GF_MANA, 0, P_PTR_KI / 2, 10);
			take_hit(DAMAGE_LOSELIFE, p_ptr->magic_num1[0] / 2, _("����˽��", "Explosion of the Force"), -1);
		}
		else return TRUE;
		break;
	case 6:
		set_tim_sh_touki(randint1(plev / 2) + 15 + boost / 7, FALSE);
		break;
	case 7:
	{
		int y, x, dam;
		project_length = 1;
		if (!get_aim_dir(&dir)) return FALSE;

		y = p_ptr->y + ddy[dir];
		x = p_ptr->x + ddx[dir];
		dam = damroll(8 + ((plev - 5) / 4) + boost / 12, 8);
		fire_beam(GF_MISSILE, dir, dam);
		if (cave[y][x].m_idx)
		{
			int i;
			int ty = y, tx = x;
			int oy = y, ox = x;
			MONSTER_IDX m_idx = cave[y][x].m_idx;
			monster_type *m_ptr = &m_list[m_idx];
			monster_race *r_ptr = &r_info[m_ptr->r_idx];
			char m_name[80];

			monster_desc(m_name, m_ptr, 0);

			if (randint1(r_ptr->level * 3 / 2) > randint0(dam / 2) + dam/2)
			{
				msg_format(_("%s�����Ф���ʤ��ä���", "%^s was not blown away."), m_name);
			}
			else
			{
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
					cave[ty][tx].m_idx = (s16b)m_idx;
					m_ptr->fy = (byte_hack)ty;
					m_ptr->fx = (byte_hack)tx;

					update_mon(m_idx, TRUE);
					lite_spot(oy, ox);
					lite_spot(ty, tx);

					if (r_ptr->flags7 & (RF7_LITE_MASK | RF7_DARK_MASK))
						p_ptr->update |= (PU_MON_LITE);
				}
			}
		}
		break;
	}
	case 8:
		if (!get_aim_dir(&dir)) return FALSE;
		fire_ball(GF_MISSILE, dir, damroll(10, 6) + plev * 3 / 2 + boost * 3 / 5, (plev < 30) ? 2 : 3);
		break;
	case 9:
	{
		MONSTER_IDX m_idx;

		if (!target_set(TARGET_KILL)) return FALSE;
		m_idx = cave[target_row][target_col].m_idx;
		if (!m_idx) break;
		if (!player_has_los_bold(target_row, target_col)) break;
		if (!projectable(p_ptr->y, p_ptr->x, target_row, target_col)) break;
		dispel_monster_status(m_idx);
		break;
	}
	case 10:
	{
		int i;
		bool success = FALSE;

		for (i = 0; i < 1 + boost/100; i++)
			if (summon_specific(-1, p_ptr->y, p_ptr->x, plev, SUMMON_PHANTOM, PM_FORCE_PET))
				success = TRUE;
		if (success)
		{
			msg_print(_("���ѤǤ������ޤ����������͡�", "'Your wish, master?'"));
		}
		else
		{
			msg_print(_("���⸽��ʤ��ä���", "Nothing happen."));
		}
		break;
	}
	case 11:
		fire_ball(GF_FIRE, 0, 200 + (2 * plev) + boost * 2, 10);
		break;
	case 12:
		if (!get_aim_dir(&dir)) return FALSE;

		fire_beam(GF_MANA, dir, damroll(10 + (plev / 2) + boost * 3 / 10, 15));
		break;
	case 13:
		set_lightspeed(randint1(16) + 16 + boost / 20, FALSE);
		break;
	default:
		msg_print(_("�ʤˡ�", "Zap?"));
	}
	P_PTR_KI = 0;
	p_ptr->update |= (PU_BONUS);

	return TRUE;
}


/*!
 * @brief ���ߥե���¸�ߤ��Ƥ�����ο�������� / calculate mirrors
 * @return �������
 */
static int number_of_mirrors( void )
{
  int x,y;
  int val=0;
  for( x=0 ; x < cur_wid ; x++ ){
    for( y=0 ; y < cur_hgt ; y++ ){
      if (is_mirror_grid(&cave[y][x])) val++;
    }
  }
  return val;
}

/*!
 * @brief ����ˡ��ȯư /
 * do_cmd_cast calls this function if the player's class is 'Mirror magic'.
 * @param spell ȯư�����ü쵻ǽ��ID
 * @return ������¹Ԥ�����TRUE������󥻥뤷�����FALSE���֤���
 */
static bool cast_mirror_spell(int spell)
{
	int             dir;
	PLAYER_LEVEL plev = p_ptr->lev;
	int		tmp;
	TIME_EFFECT t;
	int		x, y;

	/* spell code */
	switch (spell)
	{
		/* mirror of seeing */
	case 0:
		tmp = is_mirror_grid(&cave[p_ptr->y][p_ptr->x]) ? 4 : 0;
		if (plev + tmp > 4)detect_monsters_normal(DETECT_RAD_DEFAULT);
		if (plev + tmp > 18)detect_monsters_invis(DETECT_RAD_DEFAULT);
		if (plev + tmp > 28)set_tim_esp((TIME_EFFECT)plev, FALSE);
		if (plev + tmp > 38)map_area(DETECT_RAD_MAP);
		if (tmp == 0 && plev < 5) {
			msg_print(_("�����ʤ��ƽ���Ǥ��ʤ��ä���", "You need a mirror to concentrate!"));
		}
		break;
		/* drip of light */
	case 1:
		if (number_of_mirrors() < 4 + plev / 10) {
			place_mirror();
		}
		else {
			msg_format(_("����ʾ��������Ǥ��ʤ���", "There are too many mirrors to control!"));
		}
		break;
	case 2:
		if (!get_aim_dir(&dir)) return FALSE;
		if (plev > 9 && is_mirror_grid(&cave[p_ptr->y][p_ptr->x])) {
			fire_beam(GF_LITE, dir, damroll(3 + ((plev - 1) / 5), 4));
		}
		else {
			fire_bolt(GF_LITE, dir, damroll(3 + ((plev - 1) / 5), 4));
		}
		break;
		/* warped mirror */
	case 3:
		teleport_player(10, 0L);
		break;
		/* mirror of light */
	case 4:
		(void)lite_area(damroll(2, (plev / 2)), (plev / 10) + 1);
		break;
		/* mirror of wandering */
	case 5:
		teleport_player(plev * 5, 0L);
		break;
		/* robe of dust */
	case 6:
		set_dustrobe(20 + randint1(20), FALSE);
		break;
		/* banishing mirror */
	case 7:
		if (!get_aim_dir(&dir)) return FALSE;
		(void)fire_beam(GF_AWAY_ALL, dir, plev);
		break;
		/* mirror clashing */
	case 8:
		if (!get_aim_dir(&dir)) return FALSE;
		fire_ball(GF_SHARDS, dir, damroll(8 + ((plev - 5) / 4), 8),
			(plev > 20 ? (plev - 20) / 8 + 1 : 0));
		break;
		/* mirror sleeping */
	case 9:
		for (x = 0; x < cur_wid; x++) {
			for (y = 0; y < cur_hgt; y++) {
				if (is_mirror_grid(&cave[y][x])) {
					project(0, 2, y, x, (HIT_POINT)plev, GF_OLD_SLEEP, (PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL | PROJECT_JUMP | PROJECT_NO_HANGEKI), -1);
				}
			}
		}
		break;
		/* seeker ray */
	case 10:
		if (!get_aim_dir(&dir)) return FALSE;
		fire_beam(GF_SEEKER, dir, damroll(11 + (plev - 5) / 4, 8));
		break;
		/* seal of mirror */
	case 11:
		seal_of_mirror(plev * 4 + 100);
		break;
		/* shield of water */
	case 12:
		t = 20 + randint1(20);
		set_shield(t, FALSE);
		if (plev > 31)set_tim_reflect(t, FALSE);
		if (plev > 39)set_resist_magic(t, FALSE);
		break;
		/* super ray */
	case 13:
		if (!get_aim_dir(&dir)) return FALSE;
		fire_beam(GF_SUPER_RAY, dir, 150 + randint1(2 * plev));
		break;
		/* illusion light */
	case 14:
		tmp = is_mirror_grid(&cave[p_ptr->y][p_ptr->x]) ? 4 : 3;
		slow_monsters(plev);
		stun_monsters(plev*tmp);
		confuse_monsters(plev*tmp);
		turn_monsters(plev*tmp);
		stun_monsters(plev*tmp);
		stasis_monsters(plev*tmp);
		break;
		/* mirror shift */
	case 15:
		if (!is_mirror_grid(&cave[p_ptr->y][p_ptr->x])) {
			msg_print(_("���ι�ξ�꤬�狼��ʤ���", "You cannot find out where is the world of mirror!"));
			break;
		}
		alter_reality();
		break;
		/* mirror tunnel */
	case 16:
		msg_print(_("�����������̤�ȴ����  ", "Go through the world of mirror..."));
		return mirror_tunnel();

		/* mirror of recall */
	case 17:
		return word_of_recall();
		/* multi-shadow */
	case 18:
		set_multishadow(6 + randint1(6), FALSE);
		break;
		/* binding field */
	case 19:
		if (!binding_field(plev * 11 + 5))msg_print(_("Ŭ���ʶ������٤ʤ��ä���", "You were not able to choose suitable mirrors!"));
		break;
		/* mirror of Ruffnor */
	case 20:
		(void)set_invuln(randint1(4) + 4, FALSE);
		break;
	default:
		msg_print(_("�ʤˡ�", "Zap?"));

	}
	p_ptr->magic_num1[0] = 0;

	return TRUE;
}

/*!
 * @brief �ܤ��ȯư /
 * do_cmd_cast calls this function if the player's class is 'berserker'.
 * @param spell ȯư�����ü쵻ǽ��ID
 * @return ������¹Ԥ�����TRUE������󥻥뤷�����FALSE���֤���
 */
static bool cast_berserk_spell(int spell)
{
	POSITION y, x;
	DIRECTION dir;

	/* spell code */
	switch (spell)
	{
	case 0:
		detect_monsters_mind(DETECT_RAD_DEFAULT);
		break;
	case 1:
	{
		if (p_ptr->riding)
		{
			msg_print(_("������ˤ�̵������", "You cannot do it when riding."));
			return FALSE;
		}

		if (!get_rep_dir2(&dir)) return FALSE;

		if (dir == 5) return FALSE;
		y = p_ptr->y + ddy[dir];
		x = p_ptr->x + ddx[dir];

		if (!cave[y][x].m_idx)
		{
			msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "There is no monster."));
			return FALSE;
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
		break;
	}
	case 2:
	{
		if (!get_rep_dir2(&dir)) return FALSE;
		y = p_ptr->y + ddy[dir];
		x = p_ptr->x + ddx[dir];
		move_player(dir, easy_disarm, TRUE);
		break;
	}
	case 3:
		earthquake(p_ptr->y, p_ptr->x, 8+randint0(5));
		break;
	case 4:
		massacre();
		break;
	default:
		msg_print(_("�ʤˡ�", "Zap?"));

	}
	return TRUE;
}

/*!
 * @brief Ǧ�Ѥ�ȯư /
 * do_cmd_cast calls this function if the player's class is 'ninja'.
 * @param spell ȯư�����ü쵻ǽ��ID
 * @return ������¹Ԥ�����TRUE������󥻥뤷�����FALSE���֤���
 */
static bool cast_ninja_spell(int spell)
{
	POSITION x, y;
	int dir;
	int plev = p_ptr->lev;

	/* spell code */
	switch (spell)
	{
	case 0:
		(void)unlite_area(0, 3);
		break;
	case 1:
		if (plev > 44)
		{
			wiz_lite(TRUE);
		}
		detect_monsters_normal(DETECT_RAD_DEFAULT);
		if (plev > 4)
		{
			detect_traps(DETECT_RAD_DEFAULT, TRUE);
			detect_doors(DETECT_RAD_DEFAULT);
			detect_stairs(DETECT_RAD_DEFAULT);
		}
		if (plev > 14)
		{
			detect_objects_normal(DETECT_RAD_DEFAULT);
		}
		break;
	case 2:
	{
		teleport_player(10, 0L);
		break;
	}
	case 3:
	{
		if (!(p_ptr->special_defense & NINJA_KAWARIMI))
		{
			msg_print(_("Ũ�ι�����Ф����Ҵ��ˤʤä���", "You are now prepare to evade any attacks."));
			p_ptr->special_defense |= NINJA_KAWARIMI;
			p_ptr->redraw |= (PR_STATUS);
		}
		break;
	}
	case 4:
	{
		teleport_player(p_ptr->lev * 5, 0L);
		break;
	}
	case 5:
	{
		if (!get_rep_dir(&dir, FALSE)) return FALSE;
		y = p_ptr->y + ddy[dir];
		x = p_ptr->x + ddx[dir];
		if (cave[y][x].m_idx)
		{
			py_attack(y, x, 0);
			if (randint0(p_ptr->skill_dis) < 7)
				msg_print(_("���ޤ�ƨ�����ʤ��ä���", "You failed to run away."));
			else
				teleport_player(30, 0L);
		}
		else
		{
			msg_print(_("���������ˤϥ�󥹥����Ϥ��ޤ���", "You don't see any monster in this direction"));
			msg_print(NULL);
		}
		break;
	}
	case 6:
	{
		if (!get_aim_dir(&dir)) return FALSE;
		(void)stasis_monster(dir);
		break;
	}
	case 7:
		return ident_spell(FALSE);
	case 8:
		set_tim_levitation(randint1(20) + 20, FALSE);
		break;
	case 9:
		fire_ball(GF_FIRE, 0, 50+plev, plev/10+2);
		teleport_player(30, 0L);
		set_oppose_fire((TIME_EFFECT)plev, FALSE);
		break;
	case 10:
		return rush_attack(NULL);
	case 11:
	{
		int i;
		for (i = 0; i < 8; i++)
		{
			OBJECT_IDX slot;

			for (slot = 0; slot < INVEN_PACK; slot++)
			{
				if (inventory[slot].tval == TV_SPIKE) break;
			}
			if (slot == INVEN_PACK)
			{
#ifdef JP
				if (!i) msg_print("�����Ӥ���äƤ��ʤ���");
				else msg_print("�����Ӥ��ʤ��ʤä���");
#else
				if (!i) msg_print("You have no Iron Spikes.");
				else msg_print("You have no more Iron Spikes.");
#endif
				return FALSE;
			}

			/* Gives a multiplier of 2 at first, up to 3 at 40th */
			do_cmd_throw_aux(1, FALSE, slot);

			p_ptr->energy_use = 100;
		}
		break;
	}
	case 12:
	{
		monster_type *m_ptr;
		MONSTER_IDX m_idx;
		char m_name[80];
		int i;
		int path_n;
		u16b path_g[512];
		POSITION ty, tx;

		if (!target_set(TARGET_KILL)) return FALSE;
		m_idx = cave[target_row][target_col].m_idx;
		if (!m_idx) break;
		if (m_idx == p_ptr->riding) break;
		if (!player_has_los_bold(target_row, target_col)) break;
		if (!projectable(p_ptr->y, p_ptr->x, target_row, target_col)) break;
		m_ptr = &m_list[m_idx];
		monster_desc(m_name, m_ptr, 0);
		msg_format(_("%s������ᤷ����", "You pull back %s."), m_name);
		path_n = project_path(path_g, MAX_RANGE, target_row, target_col, p_ptr->y, p_ptr->x, 0);
		ty = target_row, tx = target_col;
		for (i = 1; i < path_n; i++)
		{
			int ny = GRID_Y(path_g[i]);
			int nx = GRID_X(path_g[i]);
			cave_type *c_ptr = &cave[ny][nx];

			if (in_bounds(ny, nx) && cave_empty_bold(ny, nx) &&
			    !(c_ptr->info & CAVE_OBJECT) &&
				!pattern_tile(ny, nx))
			{
				ty = ny;
				tx = nx;
			}
		}
		/* Update the old location */
		cave[target_row][target_col].m_idx = 0;

		/* Update the new location */
		cave[ty][tx].m_idx = (s16b)m_idx;

		/* Move the monster */
		m_ptr->fy = (byte_hack)ty;
		m_ptr->fx = (byte_hack)tx;

		/* Wake the monster up */
		(void)set_monster_csleep(m_idx, 0);

		/* Update the monster (new location) */
		update_mon(m_idx, TRUE);

		/* Redraw the old grid */
		lite_spot(target_row, target_col);

		/* Redraw the new grid */
		lite_spot(ty, tx);

		if (r_info[m_ptr->r_idx].flags7 & (RF7_LITE_MASK | RF7_DARK_MASK))
			p_ptr->update |= (PU_MON_LITE);

		if (m_ptr->ml)
		{
			/* Auto-Recall if possible and visible */
			if (!p_ptr->image) monster_race_track(m_ptr->ap_r_idx);

			/* Track a new monster */
			health_track(m_idx);
		}

		break;
	}
	case 13:
		if (!get_aim_dir(&dir)) return FALSE;
		fire_ball(GF_OLD_CONF, dir, plev*3, 3);
		break;
	case 14:
		project_length = -1;
		if (!get_aim_dir(&dir))
		{
			project_length = 0;
			return FALSE;
		}
		project_length = 0;

		(void)teleport_swap(dir);
		break;
	case 15:
		explosive_rune();
		break;
	case 16:
		(void)set_kabenuke(randint1(plev/2) + plev/2, FALSE);
		set_oppose_acid((TIME_EFFECT)plev, FALSE);
		break;
	case 17:
		fire_ball(GF_POIS, 0, 75+plev*2/3, plev/5+2);
		fire_ball(GF_HYPODYNAMIA, 0, 75+plev*2/3, plev/5+2);
		fire_ball(GF_CONFUSION, 0, 75+plev*2/3, plev/5+2);
		teleport_player(30, 0L);
		break;
	case 18:
	{
		int k;
		int num = damroll(3, 9);

		for (k = 0; k < num; k++)
		{
			int typ = one_in_(2) ? GF_FIRE : one_in_(3) ? GF_NETHER : GF_PLASMA;
			int attempts = 1000;

			while (attempts--)
			{
				scatter(&y, &x, p_ptr->y, p_ptr->x, 4, 0);

				if (!player_bold(y, x)) break;
			}
			project(0, 0, y, x, damroll(6 + plev / 8, 10), typ,
				(PROJECT_BEAM | PROJECT_THRU | PROJECT_GRID | PROJECT_KILL), -1);
		}
		break;
	}
	case 19:
		set_multishadow(6+randint1(6), FALSE);
		break;
	default:
		msg_print(_("�ʤˡ�", "Zap?"));

	}
	return TRUE;
}

/*!
 * @brief �ü쵻ǽ���ޥ�ɤΥᥤ��롼���� /
 * @return �ʤ�
 */
void do_cmd_mind(void)
{
	SPELL_IDX n = 0;
	int b = 0;
	int             chance;
	int             minfail = 0;
	int             plev = p_ptr->lev;
	int             old_csp = p_ptr->csp;
	mind_type       spell;
	bool            cast;
	int             use_mind, mana_cost;
	cptr            p;
	bool		on_mirror = FALSE;

	/* not if confused */
	if (p_ptr->confused)
	{
		msg_print(_("���𤷤Ƥ��ƽ���Ǥ��ʤ���", "You are too confused!"));
		return;
	}

	/* get power */
	if (!get_mind_power(&n, FALSE)) return;

#ifdef JP
	switch(p_ptr->pclass)
	{
		case CLASS_MINDCRAFTER: use_mind = MIND_MINDCRAFTER;p = "����";break;
		case CLASS_FORCETRAINER:          use_mind = MIND_KI;p = "��";break;
		case CLASS_BERSERKER:   use_mind = MIND_BERSERKER;p = "�ܤ�";break;
		case CLASS_MIRROR_MASTER:   use_mind = MIND_MIRROR_MASTER;p = "����ˡ";break;
		case CLASS_NINJA:       use_mind = MIND_NINJUTSU;p = "����";break;
		default:                use_mind = 0;p = "Ķǽ��";break;
	}
#else
	switch(p_ptr->pclass)
	{
		case CLASS_MINDCRAFTER: use_mind = MIND_MINDCRAFTER;break;
		case CLASS_FORCETRAINER:          use_mind = MIND_KI;break;
		case CLASS_BERSERKER:   use_mind = MIND_BERSERKER;break;
		case CLASS_MIRROR_MASTER:   use_mind = MIND_MIRROR_MASTER;break;
		case CLASS_NINJA:       use_mind = MIND_NINJUTSU;break;
		default:                use_mind = 0;break;
	}
#endif
	spell = mind_powers[use_mind].info[n];

	/* Spell failure chance */
	chance = spell.fail;

	mana_cost = spell.mana_cost;
	if (use_mind == MIND_KI)
	{
		if (heavy_armor()) chance += 20;
		if (p_ptr->icky_wield[0]) chance += 20;
		else if (buki_motteruka(INVEN_RARM)) chance += 10;
		if (p_ptr->icky_wield[1]) chance += 20;
		else if (buki_motteruka(INVEN_LARM)) chance += 10;
		if (n == 5)
		{
			int j;
			for (j = 0; j < P_PTR_KI / 50; j++)
				mana_cost += (j+1) * 3 / 2;
		}
	}

	/* Verify "dangerous" spells */
	if ((use_mind == MIND_BERSERKER) || (use_mind == MIND_NINJUTSU))
	{
		if (mana_cost > p_ptr->chp)
		{
			msg_print(_("�ȣФ�­��ޤ���", "You do not have enough hp to use this power."));
			return;
		}
	}
	else if (mana_cost > p_ptr->csp)
	{
		/* Warning */
		msg_print(_("�ͣФ�­��ޤ���", "You do not have enough mana to use this power."));

		if (!over_exert) return;

		/* Verify */
		if (!get_check(_("����Ǥ�ĩ�路�ޤ���? ", "Attempt it anyway? "))) return;

	}

	if (chance)
	{
		/* Reduce failure rate by "effective" level adjustment */
		chance -= 3 * (plev - spell.min_lev);

		chance += p_ptr->to_m_chance;

		/* Reduce failure rate by INT/WIS adjustment */
		chance -= 3 * (adj_mag_stat[p_ptr->stat_ind[mp_ptr->spell_stat]] - 1);

		/* Not enough mana to cast */
		if ((mana_cost > p_ptr->csp) && (use_mind != MIND_BERSERKER) && (use_mind != MIND_NINJUTSU))
		{
			chance += 5 * (mana_cost - p_ptr->csp);
		}

		/* Extract the minimum failure rate */
		minfail = adj_mag_fail[p_ptr->stat_ind[mp_ptr->spell_stat]];

		/* Minimum failure rate */
		if (chance < minfail) chance = minfail;

		/* Stunning makes spells harder */
		if (p_ptr->stun > 50) chance += 25;
		else if (p_ptr->stun) chance += 15;

		if (use_mind == MIND_KI)
		{
			if (heavy_armor()) chance += 5;
			if (p_ptr->icky_wield[0]) chance += 5;
			if (p_ptr->icky_wield[1]) chance += 5;
		}
	}

	/* Always a 5 percent chance of working */
	if (chance > 95) chance = 95;

	/* Failed spell */
	if (randint0(100) < chance)
	{
		if (flush_failure) flush();
		msg_format(_("%s�ν���˼��Ԥ�����", "You failed to concentrate hard enough!"), p);

		sound(SOUND_FAIL);

		if ((use_mind != MIND_BERSERKER) && (use_mind != MIND_NINJUTSU))
		{
			if ((use_mind == MIND_KI) && (n != 5) && P_PTR_KI)
			{
				msg_print(_("�������äƤ��ޤä�������", "Your improved Force has gone away..."));
				P_PTR_KI = 0;
			}

			if (randint1(100) < (chance / 2))
			{
				/* Backfire */
			  b = randint1(100);

			  if( use_mind == MIND_MINDCRAFTER ){
				if (b < 5)
				{
					msg_print(_("�ʤ�Ƥ��ä���Ƭ���椬������ˤʤä���", "Oh, no! Your mind has gone blank!"));
					lose_all_info();
				}
				else if (b < 15)
				{
					msg_print(_("��̯�ʸ��ʤ��ܤ������٤äƤ���...", "Weird visions seem to dance before your eyes..."));
					set_image(p_ptr->image + 5 + randint1(10));
				}
				else if (b < 45)
				{
					msg_print(_("���ʤ���Ƭ�Ϻ��𤷤���", "Your brain is addled!"));
					set_confused(p_ptr->confused + randint1(8));
				}
				else if (b < 90)
				{
					set_stun(p_ptr->stun + randint1(8));
				}
				else
				{
					/* Mana storm */
					msg_format(_("%s���Ϥ�����Ǥ��ʤ���ή�ȤʤäƲ������줿��", "Your mind unleashes its power in an uncontrollable storm!"), p);

					project(PROJECT_WHO_UNCTRL_POWER, 2 + plev / 10, p_ptr->y, p_ptr->x, plev * 2,
						GF_MANA, PROJECT_JUMP | PROJECT_KILL | PROJECT_GRID | PROJECT_ITEM, -1);
					p_ptr->csp = MAX(0, p_ptr->csp - plev * MAX(1, plev / 10));
				}
			  }
			  if( use_mind == MIND_MIRROR_MASTER ){
				if (b < 51)
				{
				  /* Nothing has happen */
				}
				else if (b < 81)
				{
					msg_print(_("���������δ��Ĥ��������", "Weird visions seem to dance before your eyes..."));
					teleport_player(10, TELEPORT_PASSIVE);
				}
				else if (b < 96)
				{
					msg_print(_("�ޤ��Τ�Τ����饭�鵱���Ƥ��롪", "Your brain is addled!"));
					set_image(p_ptr->image + 5 + randint1(10));
				}
				else
				{
					/* Mana storm */
					msg_format(_("%s���Ϥ�����Ǥ��ʤ���ή�ȤʤäƲ������줿��", "Your mind unleashes its power in an uncontrollable storm!"), p);

					project(PROJECT_WHO_UNCTRL_POWER, 2 + plev / 10, p_ptr->y, p_ptr->x, plev * 2,
						GF_MANA, PROJECT_JUMP | PROJECT_KILL | PROJECT_GRID | PROJECT_ITEM, -1);
					p_ptr->csp = MAX(0, p_ptr->csp - plev * MAX(1, plev / 10));
				}
			  }
			}
		}
	}
	else
	{
		sound(SOUND_ZAP);

		switch(use_mind)
		{
		case MIND_MINDCRAFTER:
			/* Cast the spell */
			cast = cast_mindcrafter_spell(n);
			break;
		case MIND_KI:
			/* Cast the spell */
			cast = cast_force_spell(n);
			break;
		case MIND_BERSERKER:
			/* Cast the spell */
			cast = cast_berserk_spell(n);
			break;
		case MIND_MIRROR_MASTER:
			/* Cast the spell */
			if( is_mirror_grid(&cave[p_ptr->y][p_ptr->x]) )on_mirror = TRUE;
			cast = cast_mirror_spell(n);
			break;
		case MIND_NINJUTSU:
			/* Cast the spell */
			cast = cast_ninja_spell(n);
			break;
		default:
			msg_format(_("���ǽ��:%d, %d", "Mystery power:%d, %d"),use_mind, n);
			return;
		}

		if (!cast) return;
	}


	/* Take a turn */
	p_ptr->energy_use = 100;
	/* teleport from mirror costs small energy */
	if( on_mirror && p_ptr->pclass == CLASS_MIRROR_MASTER )
	{
	  if( n==3 || n==5 || n==7 || n==16 ) p_ptr->energy_use = 50;
	}

	if ((use_mind == MIND_BERSERKER) || (use_mind == MIND_NINJUTSU))
	{
		take_hit(DAMAGE_USELIFE, mana_cost, _("���٤ν���", "concentrating too hard"), -1);
		/* Redraw hp */
		p_ptr->redraw |= (PR_HP);
	}

	/* Sufficient mana */
	else if (mana_cost <= old_csp)
	{
		/* Use some mana */
		p_ptr->csp -= mana_cost;

		/* Limit */
		if (p_ptr->csp < 0) p_ptr->csp = 0;

		if ((use_mind == MIND_MINDCRAFTER) && (n == 13))
		{
			/* No mana left */
			p_ptr->csp = 0;
			p_ptr->csp_frac = 0;
		}
	}

	/* Over-exert the player */
	else
	{
		int oops = mana_cost - old_csp;

		/* No mana left */
		if ((p_ptr->csp - mana_cost) < 0) p_ptr->csp_frac = 0;
		p_ptr->csp = MAX(0, p_ptr->csp - mana_cost);

		/* Message */
		msg_format(_("%s���椷�����Ƶ��򼺤äƤ��ޤä���", "You faint from the effort!"),p);

		/* Hack -- Bypass free action */
		(void)set_paralyzed(p_ptr->paralyzed + randint1(5 * oops + 1));

		/* Damage WIS (possibly permanently) */
		if (randint0(100) < 50)
		{
			bool perm = (randint0(100) < 25);

			/* Message */
			msg_print(_("��ʬ�������򹶷⤷�Ƥ��ޤä���", "You have damaged your mind!"));

			/* Reduce constitution */
			(void)dec_stat(A_WIS, 15 + randint1(10), perm);
		}
	}

	/* Redraw mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
	p_ptr->window |= (PW_SPELL);
}


/*!
 * @brief ���ߥץ쥤�䡼�����Ѳ�ǽ���ü쵻ǽ�ΰ���ɽ�� /
 * @return �ʤ�
 */
void do_cmd_mind_browse(void)
{
	SPELL_IDX n = 0;
	int j, line;
	char temp[62*5];
	int use_mind = 0;

	if (p_ptr->pclass == CLASS_MINDCRAFTER) use_mind = MIND_MINDCRAFTER;
	else if (p_ptr->pclass == CLASS_FORCETRAINER) use_mind = MIND_KI;
	else if (p_ptr->pclass == CLASS_BERSERKER) use_mind = MIND_BERSERKER;
	else if (p_ptr->pclass == CLASS_NINJA) use_mind = MIND_NINJUTSU;
	else if (p_ptr->pclass == CLASS_MIRROR_MASTER)
	  use_mind = MIND_MIRROR_MASTER;

	screen_save();

	while(1)
	{
		/* get power */
		if (!get_mind_power(&n, TRUE))
		{
			screen_load();
			return;
		}

		/* Clear lines, position cursor  (really should use strlen here) */
		Term_erase(12, 21, 255);
		Term_erase(12, 20, 255);
		Term_erase(12, 19, 255);
		Term_erase(12, 18, 255);
		Term_erase(12, 17, 255);
		Term_erase(12, 16, 255);

		roff_to_buf(mind_tips[use_mind][n], 62, temp, sizeof(temp));
		for(j=0, line = 17;temp[j];j+=(1+strlen(&temp[j])))
		{
			prt(&temp[j], line, 15);
			line++;
		}
		switch (use_mind)
		{
		case MIND_MIRROR_MASTER:
		case MIND_NINJUTSU:
		  prt(_("���������򲡤��Ʋ�������", "Hit any key."),0,0);
		  (void)inkey();
		}
	}
}
