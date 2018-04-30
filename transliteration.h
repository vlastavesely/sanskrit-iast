/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __TRANSLITERATION_H
#define __TRANSLITERATION_H

#include "syllable.h"


#define FLAG_REGULAR	1 << 0
#define FLAG_MODIFIER	1 << 1

typedef void (*transliteration_filter_t)(struct syllable *syllable_chain);

struct transliteration_letter {
	unsigned int code;
	unsigned int flags;
	const char *data;
};

struct transliteration_context {
	const struct transliteration_letter *table;
	const transliteration_filter_t *filters;
};

char *transliterate_devanagari_to_latin(const char *text,
	struct transliteration_context *context);

void transliteration_context_drop(struct transliteration_context *context);

#endif /* __TRANSLITERATION_H */
