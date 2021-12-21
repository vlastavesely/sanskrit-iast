/* SPDX-License-Identifier: GPL-2.0 */
/* https://cs.wikipedia.org/wiki/Wikipedie:Transkripce_hind%C5%A1tiny */

#include "compat.h"
#include "hindi.h"
#include "transliteration.h"
#include "utf8.h"

#define SCHWA_CHARACTER   'a'
#define ZERO_WIDTH_JOINER 0x200d
#define NUKTA             0x093c
#define CHUNKSIZE         1024

static struct translit_letter table[] = {

	/* Special characters */
	{0x0950, SPECIAL, "aum"},        /* aum */

	/* Vowels */
	{0x0910, VOWEL, "ai"},           /* 01 */
	{0x0914, VOWEL, "au"},           /* 02 */
	{0x0905, VOWEL, "a"},            /* 03 */
	{0x0906, VOWEL, "aa"},           /* 04 */
	{0x0907, VOWEL, "i"},            /* 05 */
	{0x0908, VOWEL, "ee"},           /* 06 */
	{0x0909, VOWEL, "u"},            /* 07 */
	{0x090a, VOWEL, "oo"},           /* 08 */
	{0x090b, VOWEL, "r"},            /* 09 */
	{0x0960, VOWEL, "rr"},           /* 10 */
	{0x090c, VOWEL, "l"},            /* 11 */
	{0x0961, VOWEL, "ll"},           /* 12 */
	{0x090f, VOWEL, "e"},            /* 13 */
	{0x0913, VOWEL, "o"},            /* 14 */
	{0x0911, VOWEL, "o"},            /* candra o */
	{0x0912, VOWEL, "o"},            /* short o */

	/* Consonants */
	{0x0916, CONSONANT, "kh"},       /* 01 */
	{0x0918, CONSONANT, "gh"},       /* 02 */
	{0x091b, CONSONANT, "chh"},      /* 03 */
	{0x091d, CONSONANT, "jh"},       /* 04 */
	{0x0920, CONSONANT, "th"},       /* 05 */
	{0x0922, CONSONANT, "dh"},       /* 06 */
	{0x0925, CONSONANT, "th"},       /* 07 */
	{0x0927, CONSONANT, "dh"},       /* 08 */
	{0x092b, CONSONANT, "ph"},       /* 09 */
	{0x092d, CONSONANT, "bh"},       /* 10 */
	{0x0915, CONSONANT, "k"},        /* 11 */
	{0x0917, CONSONANT, "g"},        /* 12 */
	{0x0919, CONSONANT, "n"},        /* 13 */
	{0x0939, CONSONANT, "h"},        /* 14 */
	{0x091a, CONSONANT, "ch"},       /* 15 */
	{0x091c, CONSONANT, "j"},        /* 16 */
	{0x091e, CONSONANT, "n"},        /* 17 */
	{0x092f, CONSONANT, "y"},        /* 18 */
	{0x0936, CONSONANT, "sh"},       /* 19 */
	{0x091f, CONSONANT, "t"},        /* 20 */
	{0x0921, CONSONANT, "d"},        /* 21 */
	{0x0923, CONSONANT, "n"},        /* 22 */
	{0x0930, CONSONANT, "r"},        /* 23 */
	{0x0937, CONSONANT, "sh"},       /* 24 */
	{0x0924, CONSONANT, "t"},        /* 25 */
	{0x0926, CONSONANT, "d"},        /* 26 */
	{0x0928, CONSONANT, "n"},        /* 27 */
	{0x0932, CONSONANT, "l"},        /* 28 */
	{0x0938, CONSONANT, "s"},        /* 29 */
	{0x092a, CONSONANT, "p"},        /* 30 */
	{0x092c, CONSONANT, "b"},        /* 31 */
	{0x092e, CONSONANT, "m"},        /* 32 */
	{0x0935, CONSONANT, "v"},        /* 33 */
	{0x0933, CONSONANT, "l"},        /* (.l) */

	/* Additional consonants - idependent versions */
	{0x0958, CONSONANT, "k"},
	{0x0959, CONSONANT, "kh"},
	{0x095a, CONSONANT, "g"},
	{0x095b, CONSONANT, "z"},
	{0x095c, CONSONANT, "d"},
	{0x095d, CONSONANT, "dh"},
	{0x095e, CONSONANT, "f"},

	/* Codas */
	{0x0902, CODA, "n"},             /* anusvara */
	{0x0903, CODA, "h"},             /* visarga */
	{0x093d, CODA, "'"},             /* avagrada (') */
	{0x0901, CODA, "n"},             /* candrabindu */

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
	{0x093e, VOWEL_SIGN, "aa"},
	{0x093f, VOWEL_SIGN, "i"},
	{0x0940, VOWEL_SIGN, "ee"},
	{0x0941, VOWEL_SIGN, "u"},
	{0x0942, VOWEL_SIGN, "oo"},
	{0x0943, VOWEL_SIGN, "r"},
	{0x0944, VOWEL_SIGN, "rr"},
	{0x0962, VOWEL_SIGN, "l"},
	{0x0963, VOWEL_SIGN, "ll"},
	{0x0947, VOWEL_SIGN, "e"},
	{0x094b, VOWEL_SIGN, "o"},
	{0x0949, VOWEL_SIGN, "o"},       /* candra o */
	{0x094a, VOWEL_SIGN, "o"},       /* short o */
	{0x094d, VOWEL_SIGN, ""},        /* virama */

	{0x0965, CODA, "||"},            /* double danda */
	{0x0964, CODA, "|"},             /* danda */
};

static struct translit_letter *letter_by_code(unsigned int c)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(table); i++) {
		if (table[i].code == c) {
			return table + i;
		}
	}

	return NULL;
}

static void nukta_filter(char *latin, unsigned int *pos, unsigned int prev)
{
	switch (prev) {
	case 0x091c: /* z */
		latin[*pos - 2] = 'z';
		break;
	case 0x091d: /* zh */
		latin[*pos - 3] = 'z';
		break;
	case 0x092b:
		strcpy(latin + *pos - 3, "fa");
		*pos = *pos - 1;
		break;
	}
}

int transcript_devanagari_to_hindi(const char *devanagari, char **ret)
{
	struct translit_letter *letter;
	unsigned int c, prev = 0, alloc = 0, done = 0, len;
	const char *src = devanagari;
	char *latin = NULL;

	while (1) {
		if (alloc < done + UNICODE_MAX_LENGTH) {
			latin = realloc(latin, alloc + CHUNKSIZE);
			alloc += CHUNKSIZE;
		}

		c = utf8_unpack_char(src);
		len = utf8_char_length(c);
		src += len;

		if (c == ZERO_WIDTH_JOINER)
			continue;

		letter = letter_by_code(c);
		if (letter) {
			switch (letter->type) {
			case CONSONANT:
				strcpy(latin + done, letter->data);
				done += strlen(letter->data);
				*(latin + done++) = SCHWA_CHARACTER;
				break;
			case VOWEL_SIGN:
				if (done) {
					/* delete the inherent schwa */
					done--;
				}
			default:
				strcpy(latin + done, letter->data);
				done += strlen(letter->data);
				break;
			}
		} else {
			if (done && c == NUKTA) {
				nukta_filter(latin, &done, prev);
				goto next;
			}

			/* remove the final schwa */
			if (is_devanagari(prev) && !is_devanagari(c)) {
				if (latin[done - 1] == SCHWA_CHARACTER) {
					latin[--done] = '\0';
				}
			}

			utf8_pack_char(latin + done, c);
			done += len;
		}
next:
		if (c == 0)
			break;
		prev = c;
	}

	*(latin + done - 1) = '\0';

	*ret = latin;

	return 0;
}
