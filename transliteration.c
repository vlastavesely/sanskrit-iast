/* SPDX-License-Identifier: GPL-2.0 */
/* https://en.wikipedia.org/wiki/IAST */

#include "compat.h"
#include "transliteration.h"
#include "utf8.h"

#define SCHWA_CHARACTER 'a'
#define VIRAMA    0x094d
#define NUKTA     0x093c
#define CHUNKSIZE 1024

static struct translit_letter table[] = {

	/* Special characters */
	{0x0950, SPECIAL, "aum"},        /* aum */

	/* Vowels */
	{0x0910, VOWEL, "ai"},           /* 01 */
	{0x0914, VOWEL, "au"},           /* 02 */
	{0x0905, VOWEL, "a"},            /* 03 */
	{0x0906, VOWEL, "\u0101"},       /* 04 (aa) */
	{0x0907, VOWEL, "i"},            /* 05 */
	{0x0908, VOWEL, "\u012b"},       /* 06 (ii) */
	{0x0909, VOWEL, "u"},            /* 07 */
	{0x090a, VOWEL, "\u016b"},       /* 08 (uu) */
	{0x090b, VOWEL, "\u1e5b"},       /* 09 (.r) */
	{0x0960, VOWEL, "\u1e5d"},       /* 10 (.rr) */
	{0x090c, VOWEL, "\u1e37"},       /* 11 (.l) */
	{0x0961, VOWEL, "\u1e39"},       /* 12 (.ll) */
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
	{0x0919, CONSONANT, "\u1e45"},   /* 13 ("n) */
	{0x0939, CONSONANT, "h"},        /* 14 */
	{0x091a, CONSONANT, "c"},        /* 15 */
	{0x091c, CONSONANT, "j"},        /* 16 */
	{0x091e, CONSONANT, "\u00f1"},   /* 17 (~n) */
	{0x092f, CONSONANT, "y"},        /* 18 */
	{0x0936, CONSONANT, "\u015b"},   /* 19 ("s) */
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
	{0x093e, VOWEL_SIGN, "\u0101"},  /* (aa) */
	{0x093f, VOWEL_SIGN, "i"},
	{0x0940, VOWEL_SIGN, "\u012b"},  /* (ii) */
	{0x0941, VOWEL_SIGN, "u"},
	{0x0942, VOWEL_SIGN, "\u016b"},  /* (uu) */
	{0x0943, VOWEL_SIGN, "\u1e5b"},  /* (.r) */
	{0x0944, VOWEL_SIGN, "\u1e5d"},  /* (.rr) */
	{0x0962, VOWEL_SIGN, "\u1e37"},  /* (.l) */
	{0x0963, VOWEL_SIGN, "\u1e39"},  /* (.ll) */
	{0x0947, VOWEL_SIGN, "e"},
	{0x094b, VOWEL_SIGN, "o"},
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

int transliterate_devanagari_to_latin(const char *devanagari, char **ret)
{
	struct translit_letter *letter;
	unsigned int c, alloc = 0, done = 0, len;
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

		if (c == NUKTA) {
			*ret = NULL;
			return EHINDI;
		}

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
			utf8_pack_char(latin + done, c);
			done += len;
		}

		if (c == 0)
			break;
	}

	*ret = latin;

	return 0;
}

static struct translit_letter *letter_by_data(const char *data)
{
	unsigned int i, len;

	for (i = 0; i < ARRAY_SIZE(table); i++) {
		len = strlen(table[i].data);
		if (len && strncmp(table[i].data, data, len) == 0) {
			return table + i;
		}
	}

	return NULL;
}

static struct translit_letter *vowel_sign_by_data(const char *data)
{
	unsigned int i, len;

	for (i = 0; i < ARRAY_SIZE(table); i++) {
		len = strlen(table[i].data);
		if (len && strncmp(table[i].data, data, len) == 0 &&
		    table[i].type == VOWEL_SIGN) {
			return table + i;
		}
	}

	return NULL;
}

int transliterate_latin_to_devanagari(const char *latin, char **ret)
{
	struct translit_letter *letter, *next;
	unsigned int alloc = 0, done = 0, len;
	const char *src = latin;
	char *devanagari = NULL;

	while (*src) {
		if (alloc < done + UNICODE_MAX_LENGTH) {
			devanagari = realloc(devanagari, alloc + CHUNKSIZE);
			alloc += CHUNKSIZE;
		}

		/* consonant (.l) */
		if (strncmp(src, "\u1e37", 3) == 0) {
			letter = letter_by_data(src + 3);
			if (letter && letter->type == VOWEL) {
				utf8_pack_char(devanagari + done, 0x0933);
				done += 3;
				src += 3;
				goto encode_vowel_modifier;
			}
		}

		/* candrabindu */
		if (strncmp(src, "m\u0310", 3) == 0) {
			utf8_pack_char(devanagari + done, 0x0901);
			done += 3;
			src += 3;
			continue;
		}

		letter = letter_by_data(src);
		if (letter) {
			utf8_pack_char(devanagari + done, letter->code);
			len = utf8_char_length(letter->code);
			done += len;
			src += strlen(letter->data);

			if (letter->type == VOWEL || letter->type == CODA)
				continue;
encode_vowel_modifier:
			next = vowel_sign_by_data(src);
			if (next) {
				utf8_pack_char(devanagari + done, next->code);
				done += utf8_char_length(next->code);
				src += strlen(next->data);
			} else {
				if (*src == SCHWA_CHARACTER) {
					src++;
				} else {
					if (letter->type == CONSONANT) {
						utf8_pack_char(devanagari + done, VIRAMA);
						done += utf8_char_length(VIRAMA);
					}
				}
			}
		} else {
			devanagari[done++] = *src++;
		}
	}

	if (devanagari)
		devanagari[done] = '\0';

	*ret = devanagari;

	return 0;
}
