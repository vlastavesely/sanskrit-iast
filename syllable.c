/* SPDX-License-Identifier: GPL-2.0 */

#include <stdlib.h>
#include <string.h>
#include "syllable.h"
#include "utf8.h"


struct syllable *syllable_alloc(const char *data, unsigned int code)
{
	struct syllable *ptr = malloc(sizeof(*ptr));

	if (ptr == NULL)
		goto out;

	ptr->data = data != NULL ? strdup(data) : utf8_code_to_string(code);
	ptr->code = code;
	ptr->prev = NULL;
	ptr->next = NULL;

out:
	return ptr;
}

void syllable_drop(struct syllable *ptr)
{
	if (ptr == NULL)
		return;

	free(ptr->data);
	free(ptr);
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

void syllable_chain_drop(struct syllable *head)
{
	struct syllable *walk = head, *next;

	while (walk) {
		next = walk->next;
		syllable_drop(walk);
		walk = next;
	}
}
