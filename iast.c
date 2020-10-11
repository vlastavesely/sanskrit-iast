/* SPDX-License-Identifier: GPL-2.0 */
/* https://en.wikipedia.org/wiki/IAST */

#include "compat.h"
#include "iast.h"

static struct translit_letter table[] = {

	/* Special characters */
	{0x0950, SPECIAL, "aum"},        /* aum */

	/* Vowels */
	{0x0910, VOWEL, "ai"},           /* 01 */
	{0x0914, VOWEL, "au"},           /* 02 */
	{0x0905, VOWEL, "a"},            /* 03 */
	{0x0906, VOWEL, "\u0101"},       /* 04 (a-) */
	{0x0907, VOWEL, "i"},            /* 05 */
	{0x0908, VOWEL, "\u012b"},       /* 06 (i-) */
	{0x0909, VOWEL, "u"},            /* 07 */
	{0x090a, VOWEL, "\u016b"},       /* 08 (u-) */
	{0x090b, VOWEL, "\u1e5b"},       /* 09 (.r) */
	{0x0960, VOWEL, "\u1e5d"},       /* 10 (.r-) */
	{0x090c, VOWEL, "\u1e37"},       /* 11 (.l) */
	{0x0961, VOWEL, "\u1e39"},       /* 12 (.l-) */
	{0x090f, VOWEL, "e"},            /* 13 */
	{0x0913, VOWEL, "o"},            /* 14 */

	/* Consonants */
	{0x0916, CONSONANT, "kh"},       /* 01 */
	{0x0918, CONSONANT, "gh"},       /* 02 */
	{0x091b, CONSONANT, "ch"},       /* 03 */
	{0x091d, CONSONANT, "jh"},       /* 04 */
	{0x0920, CONSONANT, "\u1e6dh"},  /* 05 (.th) */
	{0x0922, CONSONANT, "\u1e0dh"},  /* 06 (.dh) */
	{0x0925, CONSONANT, "th"},       /* 07 */
	{0x0927, CONSONANT, "dh"},       /* 08 */
	{0x092b, CONSONANT, "ph"},       /* 09 */
	{0x092d, CONSONANT, "bh"},       /* 10 */
	{0x0915, CONSONANT, "k"},        /* 11 */
	{0x0917, CONSONANT, "g"},        /* 12 */
	{0x0919, CONSONANT, "\u1e45"},   /* 13 (n.) */
	{0x0939, CONSONANT, "h"},        /* 14 */
	{0x091a, CONSONANT, "c"},        /* 15 */
	{0x091c, CONSONANT, "j"},        /* 16 */
	{0x091e, CONSONANT, "\u00f1"},   /* 17 (n~) */
	{0x092f, CONSONANT, "y"},        /* 18 */
	{0x0936, CONSONANT, "\u015b"},   /* 19 (s,) */
	{0x091f, CONSONANT, "\u1e6d"},   /* 20 (.t) */
	{0x0921, CONSONANT, "\u1e0d"},   /* 21 (.d) */
	{0x0923, CONSONANT, "\u1e47"},   /* 22 (.n) */
	{0x0930, CONSONANT, "r"},        /* 23 */
	{0x0937, CONSONANT, "\u1e63"},   /* 24 (.s) */
	{0x0924, CONSONANT, "t"},        /* 25 */
	{0x0926, CONSONANT, "d"},        /* 26 */
	{0x0928, CONSONANT, "n"},        /* 27 */
	{0x0932, CONSONANT, "l"},        /* 28 */
	{0x0938, CONSONANT, "s"},        /* 29 */
	{0x092a, CONSONANT, "p"},        /* 30 */
	{0x092c, CONSONANT, "b"},        /* 31 */
	{0x092e, CONSONANT, "m"},        /* 32 */
	{0x0935, CONSONANT, "v"},        /* 33 */
	{0x0933, CONSONANT, "\u1e37"},   /* (.l) */

	/* Codas */
	{0x0902, CODA, "\u1e43"},        /* anusvara (.m) */
	{0x0903, CODA, "\u1e25"},        /* visarga (.h) */
	{0x093d, CODA, "'"},             /* avagrada (') */
	{0x0901, CODA, "m\u0310"},       /* candrabindu */

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
	{0x093e, VOWEL_SIGN, "\u0101"},  /* (a-) */
	{0x093f, VOWEL_SIGN, "i"},
	{0x0940, VOWEL_SIGN, "\u012b"},  /* (i-) */
	{0x0941, VOWEL_SIGN, "u"},
	{0x0942, VOWEL_SIGN, "\u016b"},  /* (u-) */
	{0x0943, VOWEL_SIGN, "\u1e5b"},  /* (.r) */
	{0x0944, VOWEL_SIGN, "\u1e5d"},  /* (.r-) */
	{0x0962, VOWEL_SIGN, "\u1e37"},  /* (.l) */
	{0x0963, VOWEL_SIGN, "\u1e39"},  /* (.l-) */
	{0x0947, VOWEL_SIGN, "e"},
	{0x094b, VOWEL_SIGN, "o"},
	{0x094d, VOWEL_SIGN, ""},        /* virama */

	{0, 0, NULL}
};

struct translit_letter *get_iast_transliteration_table()
{
	return table;
}
