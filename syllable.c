/* SPDX-License-Identifier: GPL-2.0 */

#include <stdlib.h>
#include <string.h>
#include "syllable.h"


struct syllable *syllable_alloc(const char *data)
{
	struct syllable *ptr = malloc(sizeof(*ptr));

	if (ptr == NULL)
		return NULL;

	ptr->data = strdup(data);
	ptr->code = 0;
	ptr->prev = NULL;
	ptr->next = NULL;

	return ptr;
}

void syllable_drop(struct syllable *ptr)
{
	free(ptr);
}

struct syllable *syllable_append(struct syllable *tail, const char *data)
{
	struct syllable *ptr;

	ptr = syllable_alloc(data);
	ptr->prev = tail;
	tail->next = ptr;

	return ptr;
}

unsigned int syllable_chain_length(struct syllable *head)
{
	struct syllable *walk = head;
	unsigned int length = 0;

	while (walk) {
		length += strlen(walk->data);
		walk = walk->next;
	}

	return length;
}

char *syllable_chain_to_string(struct syllable *head)
{
	struct syllable *walk = head;
	unsigned int length = syllable_chain_length(head);
	char *buffer = malloc(length + 1);
	char *ptr = buffer;

	while (walk) {
		strcpy(ptr, walk->data);
		ptr += strlen(walk->data);
		walk = walk->next;
	}

	return buffer;
}
