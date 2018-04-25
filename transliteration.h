#ifndef __TRANSLITERATION_H
#define __TRANSLITERATION_H

struct transliteration_letter {
	unsigned long code;
	const char *data;
};

struct transliteration_context {
	const struct transliteration_letter *table_letters;
};

char *transliterate_devanagari_to_latin(const char *text,
	struct transliteration_context *context);

#endif /* __TRANSLITERATION_H */
