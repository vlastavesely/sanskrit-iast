/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __TRANSLITERATION_H
#define __TRANSLITERATION_H

enum translit_letter_type {
	VOWEL,
	CONSONANT,
	CODA,
	SPECIAL,
	NUMBER,
	VOWEL_SIGN
};

struct translit_letter {
	unsigned int code;
	enum translit_letter_type type;
	const char *data;
};

struct translit_context {
	struct translit_letter *table;
};

char *transliterate_devanagari_to_latin(const char *text);

static inline int is_devanagari(unsigned int code)
{
	return code >= 0x0900 && code <= 0x097f;
}

#endif /* __TRANSLITERATION_H */
