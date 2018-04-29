/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __UTF8_H
#define __UTF8_H

unsigned int utf8_unpack_char(const char *src);
void utf8_pack_char(char *dest, unsigned int c);

unsigned int utf8_char_length(unsigned int c);
char *utf8_code_to_string(unsigned int c);

#endif /* __UTF8_H */
