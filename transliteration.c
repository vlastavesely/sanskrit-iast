/* SPDX-License-Identifier: GPL-2.0 */

#include "compat.h"
#include "transliteration.h"
#include "iast.h"
#include "utf8.h"

char *transliterate_devanagari_to_latin(const char *devanagari)
{
	struct translit_letter *table;
	char *retval;

	table = get_iast_transliteration_table();

	retval = strdup(""); /* FIXME */

	return retval;
}
