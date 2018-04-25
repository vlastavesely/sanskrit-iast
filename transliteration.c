#include <stdlib.h>
#include <string.h>

#include "transliteration.h"
#include "syllable.h"
#include "utf8.h"

const struct transliteration_letter *find_letter_by_code(unsigned long c,
	const struct transliteration_letter *table)
{
	const struct transliteration_letter *walk = table;

	if (c == 0)
		return NULL;

	while (walk->code != 0) {
		if (c == walk->code)
			return walk;
		walk++;
	}

	return NULL;
}

const struct transliteration_modifier *find_modifier_by_code(unsigned long c,
	const struct transliteration_modifier *table)
{
	const struct transliteration_modifier *walk = table;

	if (c == 0)
		return NULL;

	while (walk->code != 0) {
		if (c == walk->code)
			return walk;
		walk++;
	}

	return NULL;
}

char *transliterate_devanagari_to_latin(const char *text,
	struct transliteration_context *context)
{
	unsigned int length = strlen(text);
	const char *ptr = text;
	const char *end = ptr + length;
	char *tmp;
	unsigned long c;
	struct syllable *head, *tail;
	const struct transliteration_letter *letter;
	const struct transliteration_modifier *modifier;

	head = syllable_alloc("");
	tail = head;

	while (ptr < end) {
		c = utf8_unpack_char(ptr);
		ptr += utf8_char_length(c);

		letter = find_letter_by_code(c, context->table_letters);
		if (letter != NULL) {
			tail = syllable_append(tail, letter->data);
			continue;
		}

		modifier = find_modifier_by_code(c, context->table_modifiers);
		if (modifier != NULL) {
			modifier->modifier(tail);
			continue;
		}

		tmp = utf8_code_to_string(c);
		tail = syllable_append(tail, tmp);
		free(tmp);
	}

	return syllable_chain_to_string(head);
}
