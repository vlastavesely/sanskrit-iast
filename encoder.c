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
	{"a-", "ā"},
	{"i-", "ī"},
	{"u-", "ū"},
	{"r.", "ṛ"},
	{"r.-", "ṝ"},
	{"l.-", "ḹ"},
	{"n^.", "ṅ"},
	{"n~", "ñ"},
	{"s,", "ś"},
	{"t.", "ṭ"},
	{"d.", "ḍ"},
	{"n.", "ṇ"},
	{"s.", "ṣ"},
	{"m.", "ṃ"},
	{"h.", "ḥ"},

	{"A-", "Ā"},
	{"I-", "Ī"},
	{"U-", "Ū"},
	{"R.", "Ṛ"},
	{"R.-", "Ṝ"},
	{"L.-", "Ḹ"},
	{"N^.", "Ṅ"},
	{"N~", "Ñ"},
	{"S,", "Ś"},
	{"T.", "Ṭ"},
	{"D.", "Ḍ"},
	{"N.", "Ṇ"},
	{"S.", "Ṣ"},
	{"M.", "Ṃ"},
	{"H.", "Ḥ"},

	{NULL, NULL}
};

const struct encoder_tuple *find_tuple(const char *text)
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
