#ifndef __UTF8_H
#define __UTF8_H

unsigned long utf8_unpack_char(char *src);
void utf8_pack_char(char *dest, unsigned long c);

unsigned int utf8_char_length(unsigned long c);
char *utf8_code_to_string(unsigned long c);

#endif /* __UTF8_H */
