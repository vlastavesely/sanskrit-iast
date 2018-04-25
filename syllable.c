#include <stdlib.h>
#include <string.h>
#include "syllable.h"


struct syllable *syllable_alloc(const char *data)
{
	struct syllable *ptr = malloc(sizeof(*ptr));

	if (ptr == NULL)
		return NULL;

	ptr->data = strdup(data);
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
	tail->next = ptr;

	return ptr;
}
