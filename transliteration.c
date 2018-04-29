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

	strcpy(buffer, syllable->data);
	buffer[strlen(buffer) - 1] = 0;
	strcat(buffer, data);

	free(syllable->data);
	syllable->data = strdup(buffer);
}

char *transliterate_devanagari_to_latin(const char *text,
	struct transliteration_context *context)
{
	unsigned int length = strlen(text);
	const char *ptr = text;
	const char *end = ptr + length;
	char *tmp;
	unsigned int code;
	struct syllable *head, *tail;
	const struct transliteration_letter *letter;

	head = syllable_alloc("");
	tail = head;

	while (ptr < end) {
		code = utf8_unpack_char(ptr);
		ptr += utf8_char_length(code);

		letter = find_letter_by_code(code, context->table);
		if (letter != NULL) {

			if (letter->flags & FLAG_REGULAR) {
				tail = syllable_append(tail, letter->data);
				tail->code = code;
			} else if (letter->flags & FLAG_MODIFIER) {
				syllable_modify(tail, letter->data);
			}

			continue;
		}

		tmp = utf8_code_to_string(code);
		tail = syllable_append(tail, tmp);
		free(tmp);
	}

	return syllable_chain_to_string(head);
}

void transliteration_context_drop(struct transliteration_context *context)
{
	free(context);
}
