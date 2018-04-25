#include <stdlib.h>
#include "iast.h"

static const struct transliteration_letter table_letters[] = {

	/* Vowels */
	{0x0905, "a"},		/* अ */
	{0x0906, "ā"},		/* आ */

	{0, NULL}
};

struct transliteration_context *transliteration_context_iast_alloc()
{
	struct transliteration_context *context;

	context = malloc(sizeof(*context));
	context->table_letters = table_letters;

	return context;
}
