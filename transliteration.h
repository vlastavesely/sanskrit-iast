#ifndef __TRANSLITERATION_H
#define __TRANSLITERATION_H

#include "syllable.h"

struct transliteration_letter {
	unsigned long code;
	const char *data;
};

typedef void (*syllable_modification_t)(struct syllable *syllable);

struct transliteration_modifier {
	unsigned long code;
	syllable_modification_t modifier;
};

struct transliteration_context {
	const struct transliteration_letter *table_letters;
	const struct transliteration_modifier *table_modifiers;
};

char *transliterate_devanagari_to_latin(const char *text,
	struct transliteration_context *context);

#endif /* __TRANSLITERATION_H */
