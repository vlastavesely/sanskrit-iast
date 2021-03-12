/* SPDX-License-Identifier: GPL-2.0 */
/* https://cs.wikipedia.org/wiki/Wikipedie:Transkripce_hind%C5%A1tiny */

#include "compat.h"
#include "transcription.h"
#include "iast-czech.h"
#include "utf8.h"

#define SCHWA_CHARACTER 'a'
#define VIRAMA    0x094d
#define NUKTA     0x093c
#define CHUNKSIZE 1024

static inline int is_consonant(unsigned int c)
{
	return (c >= 0x0915 && c <= 0x0939);
}

static inline int is_nasal(unsigned int c)
{
	return c == 0x0919 || c == 0x091e || c == 0x0923 ||
	       c == 0x0928 || c == 0x092e || c == 0x0902;
}

static void nasal_consonants_filter(char *latin, unsigned int *pos,
				    unsigned int prev, unsigned int c)
{
	char *tail = latin + *pos - 1;

	if (is_nasal(prev)) {
		/* rewrite nasals before labials to 'm' */
		switch (c) {
		case 0x092b: /* ph */
		case 0x092d: /* bh */
		case 0x092a: /* p */
		case 0x092c: /* b */
		case 0x092e: /* m */
			*tail = 'm';
			break;
		default:
			if (is_consonant(c) && *tail != SCHWA_CHARACTER)
				*tail = 'n';
			break;
		}
	}
}

static void end_of_word_filter(char *latin, unsigned int *pos,
			       unsigned int prev, unsigned int c)
{
	unsigned int len;

	if (is_devanagari(prev) && !is_devanagari(c)) {

		/* shorten ending 'á' to 'a' */
		if (prev == 0x093e) { /* modifier 'á' */
			*pos = *pos - 1;
			latin[*pos - 2] = 'a';
		}

		/* remove singular nominative suffix */
		len = utf8_char_length(c);
		if (prev == VIRAMA && *(latin + *pos - 1 - len) == 'm') {
			memmove(latin + *pos - 1 - len, latin + *pos - len, c);
			*pos = *pos - 1;
		}
	}
}

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

int transcript_devanagari_to_czech(const char *devanagari, char **ret)
{
	struct translit_letter *table, *letter;
	unsigned int c, prev = 0, alloc = 0, done = 0, len;
	const char *src = devanagari;
	char *latin = NULL;

	table = get_iast_czech_transliteration_table();

	while (1) {
		if (alloc < done + UNICODE_MAX_LENGTH) {
			latin = realloc(latin, alloc + CHUNKSIZE);
			alloc += CHUNKSIZE;
		}

		c = utf8_unpack_char(src);
		len = utf8_char_length(c);
		src += len;

		nasal_consonants_filter(latin, &done, prev, c);

		if (c == NUKTA) {
			*ret = NULL;
			return EHINDI;
		}

		letter = letter_by_code(table, c);
		if (letter) {
			switch (letter->type) {
			case CONSONANT:
				strcpy(latin + done, letter->data);
				done += strlen(letter->data);
				*(latin + done++) = SCHWA_CHARACTER;
				break;
			case VOWEL_SIGN:
				if (done) {
					/* delete the inherent schwa */
					done--;
				}
			default:
				strcpy(latin + done, letter->data);
				done += strlen(letter->data);
				break;
			}
		} else {
			utf8_pack_char(latin + done, c);
			done += len;
		}

		end_of_word_filter(latin, &done, prev, c);


		if (c == 0)
			break;
		prev = c;
	}

	*(latin + done - 1) = '\0';

	*ret = latin;

	return 0;
}
