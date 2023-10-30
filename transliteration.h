/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __TRANSLITERATION_H
#define __TRANSLITERATION_H

#if defined (__cplusplus)
extern "C" {
#endif

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

int transliterate_devanagari_to_latin(const char *text, char **ret);
int transliterate_latin_to_devanagari(const char *text, char **ret);

static inline int is_devanagari(unsigned int code)
{
	return code >= 0x0900 && code <= 0x097f;
}

#if defined (__cplusplus)
}
#endif

#endif /* __TRANSLITERATION_H */
