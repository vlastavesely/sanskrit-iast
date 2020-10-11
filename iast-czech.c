/* SPDX-License-Identifier: GPL-2.0 */

#include "compat.h"
#include "iast-czech.h"

static struct translit_letter table[] = {

	/* Special characters */
	{0x0950, SPECIAL, "óm"},      /* aum */

	/* Vowels */
	{0x0910, VOWEL, "ai"},        /* 01 */
	{0x0914, VOWEL, "au"},        /* 02 */
	{0x0905, VOWEL, "a"},         /* 03 */
	{0x0906, VOWEL, "á"},         /* 04 */
	{0x0907, VOWEL, "i"},         /* 05 */
	{0x0908, VOWEL, "í"},         /* 06 */
	{0x0909, VOWEL, "u"},         /* 07 */
	{0x090a, VOWEL, "ú"},         /* 08 */
	{0x090b, VOWEL, "r"},         /* 09 */
	{0x0960, VOWEL, "r"},         /* 10 */
	{0x090c, VOWEL, "l"},         /* 11 */
	{0x0961, VOWEL, "l"},         /* 12 */
	{0x090f, VOWEL, "é"},         /* 13 */
	{0x0913, VOWEL, "ó"},         /* 14 */

	/* Consonants */
	{0x0916, CONSONANT, "kh"},    /* 01 */
	{0x0918, CONSONANT, "gh"},    /* 02 */
	{0x091b, CONSONANT, "čh"},    /* 03 */
	{0x091d, CONSONANT, "džh"},   /* 04 */
	{0x091c, CONSONANT, "dž"},    /* 05 */
	{0x0920, CONSONANT, "th"},    /* 06 */
	{0x0922, CONSONANT, "dh"},    /* 07 */
	{0x0925, CONSONANT, "th"},    /* 08 */
	{0x0927, CONSONANT, "dh"},    /* 09 */
	{0x092b, CONSONANT, "ph"},    /* 10 */
	{0x092d, CONSONANT, "bh"},    /* 11 */
	{0x0915, CONSONANT, "k"},     /* 12 */
	{0x0917, CONSONANT, "g"},     /* 13 */
	{0x0919, CONSONANT, "n"},     /* 14 */
	{0x0939, CONSONANT, "h"},     /* 15 */
	{0x091a, CONSONANT, "č"},     /* 16 */
	{0x091e, CONSONANT, "ň"},     /* 17 */
	{0x092f, CONSONANT, "j"},     /* 18 */
	{0x0936, CONSONANT, "š"},     /* 19 */
	{0x091F, CONSONANT, "t"},     /* 20 */
	{0x0921, CONSONANT, "d"},     /* 21 */
	{0x0923, CONSONANT, "n"},     /* 22 */
	{0x0930, CONSONANT, "r"},     /* 23 */
	{0x0937, CONSONANT, "š"},     /* 24 */
	{0x0924, CONSONANT, "t"},     /* 25 */
	{0x0926, CONSONANT, "d"},     /* 26 */
	{0x0928, CONSONANT, "n"},     /* 27 */
	{0x0932, CONSONANT, "l"},     /* 28 */
	{0x0938, CONSONANT, "s"},     /* 29 */
	{0x092a, CONSONANT, "p"},     /* 30 */
	{0x092c, CONSONANT, "b"},     /* 31 */
	{0x092e, CONSONANT, "m"},     /* 32 */
	{0x0935, CONSONANT, "v"},     /* 33 */
	{0x0933, CONSONANT, "l"},

	/* Codas */
	{0x0902, CODA, "m"},          /* anusvara */
	{0x0903, CODA, ""},           /* visarga  */
	{0x093d, CODA, "'"},          /* avagrada */
	{0x0901, CODA, "m"},          /* candrabindu */

	/* Numbers */
	{0x0966, NUMBER, "0"},
	{0x0967, NUMBER, "1"},
	{0x0968, NUMBER, "2"},
	{0x0969, NUMBER, "3"},
	{0x096a, NUMBER, "4"},
	{0x096b, NUMBER, "5"},
	{0x096c, NUMBER, "6"},
	{0x096d, NUMBER, "7"},
	{0x096e, NUMBER, "8"},
	{0x096f, NUMBER, "9"},

	/* Diacritic modifiers */
	{0x0948, VOWEL_SIGN, "ai"},
	{0x094c, VOWEL_SIGN, "au"},
	{0x093e, VOWEL_SIGN, "á"},
	{0x093f, VOWEL_SIGN, "i"},
	{0x0940, VOWEL_SIGN, "í"},
	{0x0941, VOWEL_SIGN, "u"},
	{0x0942, VOWEL_SIGN, "ú"},
	{0x0943, VOWEL_SIGN, "r"},
	{0x0944, VOWEL_SIGN, "r"},
	{0x0962, VOWEL_SIGN, "l"},
	{0x0963, VOWEL_SIGN, "l"},
	{0x0947, VOWEL_SIGN, "é"},
	{0x094b, VOWEL_SIGN, "ó"},
	{0x094d, VOWEL_SIGN, ""},     /* virama */

	{0, 0, NULL}
};

struct translit_letter *get_iast_czech_transliteration_table()
{
	return table;
}
