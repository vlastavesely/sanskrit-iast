/* SPDX-License-Identifier: GPL-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "encoder.h"
#include "utf8.h"

struct encoder_tuple {
	const char *from;
	const char *to;
};

static const struct encoder_tuple table[] = {
	{"a-", "ā"},	{"A-", "Ā"},
	{"á", "ā"},	{"Á", "Ā"},
	{"i-", "ī"},	{"I-", "Ī"},
	{"í", "ī"},	{"Í", "Ī"},
	{"u-", "ū"},	{"U-", "Ū"},
	{"ú", "ū"},	{"Ú", "Ū"},
	{"r.", "ṛ"},	{"R.", "Ṛ"},
	{"r.-", "ṝ"},	{"R.-", "Ṝ"},
	{"l.", "ḷ"},	{"L.", "Ḷ"},
	{"l.-", "ḹ"},	{"L.-", "Ḹ"},
	{"n^.", "ṅ"},	{"N^.", "Ṅ"},
	{"n~", "ñ"},	{"N~", "Ñ"},
	{"ň", "ñ"},	{"Ň", "Ñ"},
	{"s,", "ś"},	{"S,", "Ś"},
	{"š", "ś"},	{"Š", "Ś"},
	{"t.", "ṭ"},	{"T.", "Ṭ"},
	{"d.", "ḍ"},	{"D.", "Ḍ"},
	{"n.", "ṇ"},	{"N.", "Ṇ"},
	{"s.", "ṣ"},	{"S.", "Ṣ"},
	{"m.", "ṃ"},	{"M.", "Ṃ"},
	{"h.", "ḥ"},	{"H.", "Ḥ"},

	{NULL, NULL}
};

static const struct encoder_tuple *find_tuple(const char *text)
{
	const struct encoder_tuple *walk = table;

	while (walk->from != NULL) {
		if (strncmp(text, walk->from, strlen(walk->from)) == 0) {
			return walk;
		}
		walk++;
	}

	return NULL;
}

char *encode_iast_punctation(const char *text)
{
	const char *str = text, *end = str + strlen(str);
	const struct encoder_tuple *tuple;
	char *buf, *dest;

	buf = malloc(strlen(text) << 1);
	if (buf == NULL)
		return NULL;

	buf[0] = 0;
	dest = buf;

	while (str < end) {
		tuple = find_tuple(str);
		if (tuple) {
			sprintf(dest, "%s", tuple->to);
			str += strlen(tuple->from);
			dest += strlen(tuple->to);
		} else {
			sprintf(dest, "%c", *str);
			str++;
			dest++;
		}
	}

	return buf;
}
