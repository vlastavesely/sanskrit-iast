/* SPDX-License-Identifier: GPL-2.0 */
/* https://en.wikipedia.org/wiki/Velthuis */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "velthuis.h"
#include "utf8.h"

#define ARRAY_SIZE(a) sizeof(a) / sizeof(*a)

struct encoder_tuple {
	const char *from;
	const char *to;
};

static const struct encoder_tuple table[] = {
	{"aa",  "\u0101"},  {"AA",  "\u0100"},
	{"ii",  "\u012b"},  {"II",  "\u012a"},
	{"uu",  "\u016b"},  {"UU",  "\u016a"},
	{".rr", "\u1e5d"},  {".RR", "\u1e5c"},
	{".r",  "\u1e5b"},  {".R",  "\u1e5a"},
	{".ll", "\u1e39"},  {".LL", "\u1e38"},
	{".l",  "\u1e37"},  {".L",  "\u1e36"},

	{".m",  "\u1e43"},  {".M",  "\u1e42"},
	{".h",  "\u1e25"},  {".H",  "\u1e24"},

	{"\"n", "\u1e45"},  {"\"N", "\u1e44"},
	{"~n",  "\u00f1"},  {"~N",  "\u00d1"},
	{"\"s", "\u015b"},  {"\"S", "\u015a"},
	{".t",  "\u1e6d"},  {".T",  "\u1e6c"},
	{".d",  "\u1e0d"},  {".D",  "\u1e0c"},
	{".n",  "\u1e47"},  {".N",  "\u1e46"},
	{".s",  "\u1e63"},  {".S",  "\u1e62"},

	{"/",   "m\u0310"}
};

static const struct encoder_tuple *find_tuple(const char *text)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(table); i++) {
		if (strncmp(text, table[i].from, strlen(table[i].from)) == 0) {
			return &table[i];
		}
	}

	return NULL;
}

char *encode_velthuis_to_iast_punctation(const char *text)
{
	const char *str = text, *end = str + strlen(str);
	const struct encoder_tuple *tuple;
	char *buf, *dest;

	buf = calloc(1, strlen(text) << 1);
	if (buf == NULL)
		return NULL;

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
