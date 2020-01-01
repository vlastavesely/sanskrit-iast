/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __TRANSLITERATION_H
#define __TRANSLITERATION_H

#include "syllable.h"

enum translit_letter_type {
	VOWEL,
	CONSONANT,
	CODA,
	SPECIAL,
	NUMBER,
	VOWEL_SIGN
};

typedef void (*transliteration_filter_t)(struct syllable *syllable_chain);

struct translit_letter {
	unsigned int code;
	enum translit_letter_type type;
	const char *data;
};

struct translit_context {
	const struct translit_letter *table;
	const transliteration_filter_t *filters;
};

char *transliterate_devanagari_to_latin(const char *text,
	const struct transliteration_context *context);

static inline int is_devanagari(unsigned int code)
{
	return code >= 0x0900 && code <= 0x097f;
}

#endif /* __TRANSLITERATION_H */
