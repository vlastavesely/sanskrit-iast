#include <stdlib.h>
#include <string.h>

#include "transliteration.h"
#include "syllable.h"
#include "utf8.h"

char *transliterate_devanagari_to_latin(const char *text)
{
	unsigned int length = strlen(text);
	const char *ptr = text;
	const char *end = ptr + length;
	char *tmp;
	unsigned long c;
	struct syllable *head, *tail;

	head = syllable_alloc("");
	tail = head;

	while (ptr < end) {
		c = utf8_unpack_char(ptr);
		ptr += utf8_char_length(c);

		tmp = utf8_code_to_string(c);
		tail = syllable_append(tail, tmp);
		free(tmp);
	}

	return syllable_chain_to_string(head);
}
