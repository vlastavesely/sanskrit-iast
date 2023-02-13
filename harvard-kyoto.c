/* SPDX-License-Identifier: GPL-2.0 */
/* https://en.wikipedia.org/wiki/Velthuis */

#include "compat.h"
#include "harvard-kyoto.h"
#include "utf8.h"

struct encoder_tuple {
	const char *from;
	const char *to;
};

static const struct encoder_tuple table[] = {
	{"A",  "\u0101"},
	{"I",  "\u012b"},
	{"U",  "\u016b"},
	{"RR", "\u1e5d"},
	{"R",  "\u1e5b"},
	{"LL", "\u1e39"},
	{"L",  "\u1e37"},

	{"aa",  "\u0101"},
	{"ii",  "\u012b"},
	{"uu",  "\u016b"},
	{".rr", "\u1e5d"},
	{".r",  "\u1e5b"},
	{".ll", "\u1e39"},
	{".l",  "\u1e37"},

	{"M",  "\u1e43"},
	{"H",  "\u1e25"},

	{"G",  "\u1e45"},
	{"J",  "\u00f1"},
	{"T",  "\u1e6d"},
	{"D",  "\u1e0d"},
	{"N",  "\u1e47"},
	{"S",  "\u1e63"},
	{"sh", "\u015b"},
	{"z",  "\u015b"}
};

static const struct encoder_tuple *find_tuple(const char *text)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(table); i++) {
		if (strncmp(text, table[i].from, strlen(table[i].from)) == 0) {
			return &table[i];
		}
	}

	return NULL;
}

int encode_harvard_kyoto_to_iast(const char *text, char **out)
{
	const char *str = text, *end = str + strlen(str);
	const struct encoder_tuple *tuple;
	char *buf, *dest;

	buf = calloc(1, strlen(text) * 3); /* should be enough */
	if (buf == NULL)
		return ENOMEM;

	dest = buf;
	while (str < end) {
		tuple = find_tuple(str);
		if (tuple) {
			sprintf(dest, "%s", tuple->to);
			str += strlen(tuple->from);
			dest += strlen(tuple->to);
		} else if (strncmp(str, "{}", 2) == 0) {
			str += 2;
		} else {
			sprintf(dest, "%c", *str);
			str++;
			dest++;
		}
	}

	*out = buf;

	return 0;
}
