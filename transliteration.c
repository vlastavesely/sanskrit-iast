/* SPDX-License-Identifier: GPL-2.0 */

#include "compat.h"
#include "transliteration.h"
#include "iast.h"
#include "utf8.h"

#define SCHWA_CHARACTER 'a'
#define CHUNKSIZE 1024

static struct translit_letter *letter_by_code(struct translit_letter *table,
					      unsigned int c)
{
	while (table->code != 0) {
		if (table->code == c)
			return table;
		table++;
	}

	return NULL;
}

char *transliterate_devanagari_to_latin(const char *devanagari)
{
	struct translit_letter *table;
	struct translit_letter *letter;
	unsigned int c, alloc = 0, done = 0, len;
	const char *src = devanagari;
	char *latin = NULL;

	table = get_iast_transliteration_table();

	while (1) {
		if (alloc < done + UNICODE_MAX_LENGTH) {
			latin = realloc(latin, alloc + CHUNKSIZE);
			alloc += CHUNKSIZE;
		}

		c = utf8_unpack_char(src);
		len = utf8_char_length(c);
		src += len;

		letter = letter_by_code(table, c);
		if (letter) {
			switch (letter->type) {
			case CONSONANT:
				strcpy(latin + done, letter->data);
				done += strlen(letter->data);
				*(latin + done++) = SCHWA_CHARACTER;
				break;
			case VOWEL_SIGN:
				if (done)
					done--;
				strcpy(latin + done, letter->data);
				done += strlen(letter->data);
				break;
			default:
				strcpy(latin + done, letter->data);
				done += strlen(letter->data);
				break;
			}
		} else {
			utf8_pack_char(latin + done, c);
			done += len;
		}

		if (c == 0)
			break;
	}

	return latin;
}
