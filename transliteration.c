/* SPDX-License-Identifier: GPL-2.0 */

#include <stdlib.h>
#include <string.h>

#include "transliteration.h"
#include "syllable.h"
#include "utf8.h"

static const struct transliteration_letter *find_letter_by_code(unsigned int c,
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

static void syllable_modify(struct syllable *syllable, const char *data)
{
	char buffer[10];

	if (syllable == NULL)
		return;

	strcpy(buffer, syllable->data);
	buffer[strlen(buffer) - 1] = 0;
	strcat(buffer, data);

	free(syllable->data);
	syllable->data = strdup(buffer);
}

static void apply_transliteration_filters(struct syllable *head,
	const transliteration_filter_t *filters)
{
	const transliteration_filter_t *filter = filters;

	while (*filter) {
		(*filter)(head);
		filter++;
	}
}

char *transliterate_devanagari_to_latin(const char *devanagari,
	const struct transliteration_context *context)
{
	struct syllable *head = NULL, *tail = NULL, **indirect = &head;
	const struct transliteration_letter *letter;
	const char *ptr = devanagari;
	const char *end = ptr + strlen(devanagari);
	unsigned int code;
	char *retval;

	while (ptr < end) {
		code = utf8_unpack_char(ptr);
		ptr += utf8_char_length(code);

		letter = find_letter_by_code(code, context->table);
		if (letter && letter->flags & FLAG_MODIFIER) {
			syllable_modify(tail, letter->data);
			continue;
		}

		*indirect = syllable_alloc(letter ? letter->data : NULL, code);
		tail = *indirect;
		indirect = &(*indirect)->next;
	}

	apply_transliteration_filters(head, context->filters);

	retval = syllable_chain_to_string(head);
	syllable_chain_drop(head);

	return retval;
}
