/* SPDX-License-Identifier: GPL-2.0 */

#include "utf8.h"

unsigned int utf8_unpack_char(const char *src)
{
	unsigned int c = 0;

	if ((src[0] & 0x80) == 0x00) {
		c  = ((src[0] & 0x7f) <<  0);

	} else if ((src[0] & 0xe0) == 0xc0) {
		c  = ((src[0] & 0x1f) <<  6);
		c |= ((src[1] & 0x3f) <<  0);

	} else if ((src[0] & 0xf0) == 0xe0) {
		c  = ((src[0] & 0x0f) << 12);
		c |= ((src[1] & 0x3f) <<  6);
		c |= ((src[2] & 0x3f) <<  0);

	} else if ((src[0] & 0xf8) == 0xf0) {
		c  = ((src[0] & 0x07) << 18);
		c |= ((src[1] & 0x3f) << 12);
		c |= ((src[2] & 0x3f) <<  6);
		c |= ((src[3] & 0x3f) <<  0);
	}

	return c;
}

void utf8_pack_char(char *dest, unsigned int c)
{
	if (c <= 0x00007f) {
		dest[0] = c;

	} else if (c <= 0x0007ff) {
		dest[0] = (0xc0 | ((c >>  6) & 0xff));
		dest[1] = (0x80 | ((c >>  0) & 0x3f));

	} else if (c <= 0x00ffff) {
		dest[0] = (0xe0 | ((c >> 12) & 0xff));
		dest[1] = (0x80 | ((c >>  6) & 0x3f));
		dest[2] = (0x80 | ((c >>  0) & 0x3f));

	} else if (c <= 0x10ffff) {
		dest[0] = (0xf0 | ((c >> 18) & 0xff));
		dest[1] = (0x80 | ((c >> 12) & 0x3f));
		dest[2] = (0x80 | ((c >>  6) & 0x3f));
		dest[3] = (0x80 | ((c >>  0) & 0x3f));

	} else {
		dest[0] = '?'; /* should not happen */
	}
}

unsigned int utf8_char_length(unsigned int c)
{
	if (c <= 0x00007f) {
		return 1;

	} else if (c <= 0x0007ff) {
		return 2;

	} else if (c <= 0x00ffff) {
		return 3;

	} else if (c <= 0x10ffff) {
		return 4;
	}

	return 0; /* should not happen */
}
