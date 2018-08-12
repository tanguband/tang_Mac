/*!
 *  @file japanese.c
 *  @brief ���ܸ�����ؿ�
 *  @date 2014/07/07
 */

#include "angband.h"

#ifdef JP

typedef struct convert_key convert_key;

struct convert_key
{
	cptr key1;
	cptr key2;
};

static const convert_key s2j_table[] = {
	{"mb","nb"}, {"mp","np"}, {"mv","nv"}, {"mm","nm"},
	{"x","ks"},
	/* sindar:���������  parantir:�ѥ��ƥ���  feanor:�ե����Ρ��� */
	{"ar$","a-ru$"}, {"ir$","ia$"}, {"or$","o-ru$"},
	{"ra","��"}, {"ri","��"}, {"ru","��"}, {"re","��"}, {"ro","��"},
	{"ir","ia"}, {"ur","ua"}, {"er","ea"}, {"ar","a��"},
	{"sha","����"}, {"shi","��"}, {"shu","����"}, {"she","����"}, {"sho","����"},
	{"tha","��"}, {"thi","��"}, {"thu","��"}, {"the","��"}, {"tho","��"},
	{"cha","��"}, {"chi","��"}, {"chu","��"}, {"che","��"}, {"cho","��"},
	{"dha","��"}, {"dhi","��"}, {"dhu","��"}, {"dhe","��"}, {"dho","��"},
	{"ba","��"}, {"bi","��"}, {"bu","��"}, {"be","��"}, {"bo","��"},
	{"ca","��"}, {"ci","��"}, {"cu","��"}, {"ce","��"}, {"co","��"},
	{"da","��"}, {"di","�ǥ�"}, {"du","�ɥ�"}, {"de","��"}, {"do","��"},
	{"fa","�ե�"}, {"fi","�ե�"}, {"fu","��"}, {"fe","�ե�"}, {"fo","�ե�"},
	{"ga","��"}, {"gi","��"}, {"gu","��"}, {"ge","��"}, {"go","��"},
	{"ha","��"}, {"hi","��"}, {"hu","��"}, {"he","��"}, {"ho","��"},
	{"ja","����"}, {"ji","��"}, {"ju","����"}, {"je","����"}, {"jo","����"},
	{"ka","��"}, {"ki","��"}, {"ku","��"}, {"ke","��"}, {"ko","��"},
	{"la","��"}, {"li","��"}, {"lu","��"}, {"le","��"}, {"lo","��"},
	{"ma","��"}, {"mi","��"}, {"mu","��"}, {"me","��"}, {"mo","��"},
	{"na","��"}, {"ni","��"}, {"nu","��"}, {"ne","��"}, {"no","��"},
	{"pa","��"}, {"pi","��"}, {"pu","��"}, {"pe","��"}, {"po","��"},
	{"qu","��"},
	{"sa","��"}, {"si","��"}, {"su","��"}, {"se","��"}, {"so","��"},
	{"ta","��"}, {"ti","�ƥ�"}, {"tu","�ȥ�"}, {"te","��"}, {"to","��"},
	{"va","����"}, {"vi","����"}, {"vu","��"}, {"ve","����"}, {"vo","����"},
	{"wa","��"}, {"wi","����"}, {"wu","��"}, {"we","����"}, {"wo","����"},
	{"ya","��"}, {"yu","��"}, {"yo","��"},
	{"za","��"}, {"zi","��"}, {"zu","��"}, {"ze","��"}, {"zo","��"},
	{"dh","��"}, {"ch","��"}, {"th","��"},
	{"b","��"}, {"c","��"}, {"d","��"}, {"f","��"}, {"g","��"},
	{"h","��"}, {"j","����"}, {"k","��"}, {"l","��"}, {"m","��"},
	{"n","��"}, {"p","��"}, {"q","��"}, {"r","��"}, {"s","��"},
	{"t","��"}, {"v","��"}, {"w","��"}, {"y","��"},
	{"a","��"}, {"i","��"}, {"u","��"}, {"e","��"}, {"o","��"},
	{"-","��"},
	{NULL,NULL}
};

/*!
 * @brief ������������ܸ���ɤߤ��Ѵ�����
 * @param kana �Ѵ�������ܸ�ʸ����ݥ���
 * @param sindarin �Ѵ����Υ�������ʸ����ݥ���
 * @return �ʤ�
 * @details
 */
void sindarin_to_kana(char *kana, const char *sindarin)
{
	char buf[256];
	int idx;

	sprintf(kana, "%s$", sindarin);
	for (idx = 0; kana[idx]; idx++)
		if (isupper(kana[idx])) kana[idx] = (char)tolower(kana[idx]);

	for (idx = 0; s2j_table[idx].key1 != NULL; idx++)
	{
		cptr pat1 = s2j_table[idx].key1;
		cptr pat2 = s2j_table[idx].key2;
		int len = strlen(pat1);
		char *src = kana;
		char *dest = buf;

		while (*src)
		{
			if (strncmp(src, pat1, len) == 0)
			{
				strcpy(dest, pat2);
				src += len;
				dest += strlen(pat2);
			}
			else
			{
				if (iskanji(*src))
				{
					*dest = *src;
					src++;
					dest++;
				}
				*dest = *src;
				src++;
				dest++;
			}
		}

		*dest = 0;
		strcpy(kana, buf);
	}

	idx = 0;

	while (kana[idx] != '$') idx++;

	kana[idx] = '\0';
}


/*! ���ܸ�ư����� (�Ǥġ��Ǥä�,�Ǥ� etc)
 * JVERB_AND: ����,���� > ����,����
 * JVERB_TO:  ����,���� > ���äƽ���
 * JVERB_OR:  ����,���� > ���ä��꽳�ä��� */
static const struct jverb_table_t {
	const char* from;
	const char* to[3];
} jverb_table[] = {
	{ "����", {"��", "����", "����"}},
	{ "����", {"����", "����", "����"}},

	{ "����", {"��", "����", "����"}},
	{ "����", {"��", "����", "����"}},
	{ "����", {"��", "����", "����"}},
	{ "����", {"��", "����", "����"}},
	{ "����", {"��", "����", "����"}},
	{ "�Ƥ�", {"��", "�Ƥä�", "�Ƥä�"}},
	{ "�Ǥ�", {"��", "�Ǥ�", "�Ǥ�"}},
	{ "�ͤ�", {"��", "�ͤ�", "�ͤ�"}},
	{ "�ؤ�", {"��", "�ؤ�", "�ؤ�"}},
	{ "�٤�", {"��", "�٤�", "�٤�"}},
	{ "���", {"��", "���", "�᤿"}},
	{ "���", {"��", "���", "�줿"}},

	{ "��", {"��", "�ä�", "�ä�"}},
	{ "��", {"��", "����", "����"}},
	{ "��", {"��", "����", "����"}},
	{ "��", {"��", "����", "����"}},
	{ "��", {"��", "����", "����"}},
	{ "��", {"��", "�ä�", "�ä�"}},
	{ "��", {"��", "�ä�", "�ä�"}},
	{ "��", {"��", "�ͤ�", "�ͤ�"}},
	{ "��", {"��", "�ؤ�", "�ؤ�"}},
	{ "��", {"��", "���", "���"}},
	{ "��", {"��", "���", "���"}},
	{ "��", {"��", "�ä�", "�ä�"}},
	{ NULL, {"������", "���Ȥˤ��", "���Ȥ�"}},
};

/*!
 * @brief jverb_table_t�˽��ä�ư�����Ѥ���
 * @param in �Ѵ���ʸ����ݥ���
 * @param out �Ѵ���ʸ����ݥ���
 * @param flag �Ѵ���������(JVERB_AND/JVERB_TO/JVERB_OR)
 * @return �ʤ�
 * @details
 */
void jverb(const char *in, char *out, int flag)
{
	const struct jverb_table_t * p;
	int in_len = strlen(in);

	strcpy(out, in);

	for (p = jverb_table; p->from; p++) {
		int from_len = strlen(p->from);
		if (strncmp(&in[in_len-from_len], p->from, from_len) == 0) {
			strcpy(&out[in_len - from_len], p->to[flag - 1]);
			break;
		}
	}

	if (p->from == NULL)
		strcpy(&out[in_len], p->to[flag - 1]);
}

/*!
 * @brief ʸ�������ɤ�SJIS����EUC���Ѵ����� / Convert SJIS string to EUC string
 * @param str �Ѵ�����ʸ����Υݥ���
 * @return �ʤ�
 * @details
 */
void sjis2euc(char *str)
{
	int i;
	unsigned char c1, c2;
	unsigned char *tmp;

	int len = strlen(str);

	C_MAKE(tmp, len+1, byte);

	for (i = 0; i < len; i++)
	{
		c1 = str[i];
		if (c1 & 0x80)
		{
			i++;
			c2 = str[i];
			if (c2 >= 0x9f)
			{
				c1 = c1 * 2 - (c1 >= 0xe0 ? 0xe0 : 0x60);
				c2 += 2;
			}
			else
			{
				c1 = c1 * 2 - (c1 >= 0xe0 ? 0xe1 : 0x61);
				c2 += 0x60 + (c2 < 0x7f);
			}
			tmp[i - 1] = c1;
			tmp[i] = c2;
		}
		else
			tmp[i] = c1;
	}
	tmp[len] = 0;
	strcpy(str, (char *)tmp);

	C_KILL(tmp, len+1, byte);
}  


/*!
 * @brief ʸ�������ɤ�EUC����SJIS���Ѵ����� / Convert EUC string to SJIS string
 * @param str �Ѵ�����ʸ����Υݥ���
 * @return �ʤ�
 * @details
 */
void euc2sjis(char *str)
{
	int i;
	unsigned char c1, c2;
	unsigned char *tmp;
	
	int len = strlen(str);

	C_MAKE(tmp, len+1, byte);

	for (i = 0; i < len; i++)
	{
		c1 = str[i];
		if (c1 & 0x80)
		{
			i++;
			c2 = str[i];
			if (c1 % 2)
			{
				c1 = (c1 >> 1) + (c1 < 0xdf ? 0x31 : 0x71);
				c2 -= 0x60 + (c2 < 0xe0);
			}
			else
			{
				c1 = (c1 >> 1) + (c1 < 0xdf ? 0x30 : 0x70);
				c2 -= 2;
			}

			tmp[i - 1] = c1;
			tmp[i] = c2;
		}
		else
			tmp[i] = c1;
	}
	tmp[len] = 0;
	strcpy(str, (char *)tmp);

	C_KILL(tmp, len+1, byte);
}  


/*!
 * @brief str��Ķ��˹�ä�ʸ�������ɤ��Ѵ������Ѵ�����ʸ�������ɤ��֤���str��Ĺ�������¤Ϥʤ���
 * @param str �Ѵ�����ʸ����Υݥ���
 * @return 
 * 0: Unknown<br>
 * 1: ASCII (Never known to be ASCII in this function.)<br>
 * 2: EUC<br>
 * 3: SJIS<br>
 */
byte codeconv(char *str)
{
	byte code = 0;
	int i;

	for (i = 0; str[i]; i++)
	{
		unsigned char c1;
		unsigned char c2;

		/* First byte */
		c1 = str[i];

		/* ASCII? */
		if (!(c1 & 0x80)) continue;

		/* Second byte */
		i++;
		c2 = str[i];

		if (((0xa1 <= c1 && c1 <= 0xdf) || (0xfd <= c1 && c1 <= 0xfe)) &&
		    (0xa1 <= c2 && c2 <= 0xfe))
		{
			/* Only EUC is allowed */
			if (!code)
			{
				/* EUC */
				code = 2;
			}

			/* Broken string? */
			else if (code != 2)
			{
				/* No conversion */
				return 0;
			}
		}

		else if (((0x81 <= c1 && c1 <= 0x9f) &&
			  ((0x40 <= c2 && c2 <= 0x7e) || (0x80 <= c2 && c2 <= 0xfc))) ||
			 ((0xe0 <= c1 && c1 <= 0xfc) &&
			  (0x40 <= c2 && c2 <= 0x7e)))
		{
			/* Only SJIS is allowed */
			if (!code)
			{
				/* SJIS */
				code = 3;
			}

			/* Broken string? */
			else if (code != 3)
			{
				/* No conversion */
				return 0;
			}
		}
	}


	switch (code)
	{
#ifdef EUC
	case 3:
		/* SJIS -> EUC */
		sjis2euc(str);
		break;
#endif

#ifdef SJIS
	case 2:
		/* EUC -> SJIS */
		euc2sjis(str);

		break;
#endif
	}

	/* Return kanji code */
	return code;
}

/*!
 * @brief ʸ����s��x�Х����ܤ�������1�Х����ܤ��ɤ���Ƚ�ꤹ��
 * @param s Ƚ�ꤹ��ʸ����Υݥ���
 * @param x Ƚ�ꤹ�����(�Х���)
 * @return ������1�Х����ܤʤ��TRUE
 */
bool iskanji2(cptr s, int x)
{
	int i;

	for (i = 0; i < x; i++)
	{
		if (iskanji(s[i])) i++;
	}
	if ((x == i) && iskanji(s[x])) return TRUE;

	return FALSE;
}

/*!
 * @brief ʸ�����ʸ�������ɤ�ASCII���ɤ�����Ƚ�ꤹ��
 * @param str Ƚ�ꤹ��ʸ����ؤΥݥ���
 * @return ʸ�����ʸ�������ɤ�ASCII�ʤ�TRUE�������Ǥʤ����FALSE
 */
static bool is_ascii_str(cptr str)
{
	for (;*str; str++) {
		if (!(0x00 < *str && *str <= 0x7f))
			return FALSE;
	}
	return TRUE;
}

/*!
 * @brief ʸ�����ʸ�������ɤ�UTF-8���ɤ�����Ƚ�ꤹ��
 * @param str Ƚ�ꤹ��ʸ����ؤΥݥ���
 * @return ʸ�����ʸ�������ɤ�UTF-8�ʤ�TRUE�������Ǥʤ����FALSE
 */
static bool is_utf8_str(cptr str)
{
	const unsigned char* p;
	for (p = (const unsigned char*)str; *p; p++) {
		int subseq_num = 0;
		if (0x00 < *p && *p <= 0x7f) continue;
		
		if ((*p & 0xe0) == 0xc0) subseq_num = 1;
		if ((*p & 0xf0) == 0xe0) subseq_num = 2;
		if ((*p & 0xf8) == 0xf0) subseq_num = 3;

		if (subseq_num == 0) return FALSE;
		while (subseq_num--) {
			p++;
			if (!*p || (*p & 0xc0) != 0x80) return FALSE;
		}
	}
	return TRUE;
}

#if defined(EUC)
#include <iconv.h>

static const struct ms_to_jis_unicode_conv_t {
	char from[3];
	char to[3];
} ms_to_jis_unicode_conv[] = {
	{{0xef, 0xbd, 0x9e}, {0xe3, 0x80, 0x9c}}, /* FULLWIDTH TILDE -> WAVE DASH */
	{{0xef, 0xbc, 0x8d}, {0xe2, 0x88, 0x92}}, /* FULLWIDTH HYPHEN-MINUS -> MINUS SIGN */
};

/*!
 * @brief EUC�������ƥॳ���ɤǤ���Ķ���������UTF-8�����Ѵ�������Ԥ����֥롼����
 * @param str �Ѵ�����ʸ����Υݥ���
 * @return �ʤ�
 */
static void ms_to_jis_unicode(char* str)
{
	unsigned char* p;
	for (p = (unsigned char*)str; *p; p++) {
		int subseq_num = 0;
		if (0x00 < *p && *p <= 0x7f) continue;

		if ((*p & 0xe0) == 0xc0) subseq_num = 1;
		if ((*p & 0xf0) == 0xe0) {
			int i;
			for (i = 0; i < sizeof(ms_to_jis_unicode_conv) / sizeof(ms_to_jis_unicode_conv[0]); ++ i) {
				const struct ms_to_jis_unicode_conv_t *c = &ms_to_jis_unicode_conv[i];
				if (memcmp(p, c->from, 3) == 0) {
					memcpy(p, c->to, 3);
				}
			}
			subseq_num = 2;
                }
		if ((*p & 0xf8) == 0xf0) subseq_num = 3;

		p += subseq_num;
	}
}

#elif defined(SJIS) && defined(WINDOWS)
#include <Windows.h>
#endif
/*!
 * @brief ʸ�������ɤ�UTF-8��ʸ����򥷥��ƥ��ʸ�������ɤ��Ѵ�����
 * @param utf8_str �Ѵ�����UTF-8��ʸ����ؤΥݥ���
 * @param sys_str_buffer �Ѵ����������ƥ��ʸ�������ɤ�ʸ������Ǽ����Хåե��ؤΥݥ���
 * @param sys_str_buflen �Ѵ����������ƥ��ʸ�������ɤ�ʸ������Ǽ����Хåե���Ĺ��
 * @return �Ѵ��������������TRUE�����Ԥ������FALSE���֤�
 */
static bool utf8_to_sys(char* utf8_str, char* sys_str_buffer, size_t sys_str_buflen)
{
#if defined(EUC)

	iconv_t cd = iconv_open("EUC-JP", "UTF-8");
	size_t utf8_len = strlen(utf8_str) + 1; /* include termination character */
	char *from = utf8_str;
	int ret;

	ms_to_jis_unicode(utf8_str);
	ret = iconv(cd, &from, &utf8_len, &sys_str_buffer, &sys_str_buflen);
	iconv_close(cd);
	return (ret >= 0);

#elif defined(SJIS) && defined(WINDOWS)

	LPWSTR utf16buf;
	int input_len = strlen(utf8_str) + 1; /* include termination character */

	C_MAKE(utf16buf, input_len, WCHAR);

	/* UTF-8 -> UTF-16 */
	if (MultiByteToWideChar( CP_UTF8, 0, utf8_str, input_len, utf16buf, input_len) == 0) {
		C_KILL(utf16buf, input_len, WCHAR);
		return FALSE;
	}

	/* UTF-8 -> SJIS(CP932) */
	if (WideCharToMultiByte( CP_ACP, 0, utf16buf, -1, sys_str_buffer, sys_str_buflen, NULL, NULL ) == 0) {
		C_KILL(utf16buf, input_len, WCHAR);
		return FALSE;
	}

	C_KILL(utf16buf, input_len, WCHAR);
	return TRUE;

#endif
}

/*!
 * @brief ������ä�ʸ�����ʸ�������ɤ���ꤷ�������ƥ��ʸ�������ɤ��Ѵ�����
 * @param strbuf �Ѵ�����ʸ������Ǽ�����Хåե��ؤΥݥ��󥿡�
 *               �Хåե����Ѵ�����ʸ����Ǿ�񤭤���롣
 *               UTF-8����SJIS�⤷����EUC�ؤ��Ѵ������ꤷ�Ƥ���ΤǥХåե���Ĺ����­��ʤ��ʤ뤳�ȤϤʤ���
 * @param buflen �Хåե���Ĺ����
 * @return �ʤ�
 */
void guess_convert_to_system_encoding(char* strbuf, int buflen)
{
	if (is_ascii_str(strbuf)) return;

	if (is_utf8_str(strbuf)) {
		char* work;
		C_MAKE(work, buflen, char);
		my_strcpy(work, strbuf, buflen);
		if (!utf8_to_sys(work, strbuf, buflen)) {
			msg_print("�ٹ�:ʸ�������ɤ��Ѵ��˼��Ԥ��ޤ���");
			msg_print(NULL);
		}
		C_KILL(work, buflen, char);
	}
}

#endif /* JP */
