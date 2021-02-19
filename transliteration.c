/* SPDX-License-Identifier: GPL-2.0 */

#include "compat.h"
#include "transliteration.h"
#include "iast.h"
#include "utf8.h"

#define SCHWA_CHARACTER 'a'
#define VIRAMA 0x094d
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
	struct translit_letter *table, *letter;
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

static struct translit_letter *letter_by_data(struct translit_letter *table,
					      const char *data)
{
	while (table->code != 0) {
		unsigned int len = strlen(table->data);
		if (len && strncmp(table->data, data, len) == 0)
			return table;
		table++;
	}

	return NULL;
}

static struct translit_letter *vowel_sign_by_data(struct translit_letter *table,
						  const char *data)
{
	while (table->code != 0) {
		unsigned int len = strlen(table->data);
		if (len && strncmp(table->data, data, len) == 0 &&
		    table->type == VOWEL_SIGN)
			return table;
		table++;
	}

	return NULL;
}

char *transliterate_latin_to_devanagari(const char *latin)
{
	struct translit_letter *table, *letter, *next;
	unsigned int alloc = 0, done = 0, len;
	const char *src = latin;
	char *devanagari = NULL;

	table = get_iast_transliteration_table();

	while (*src) {
		if (alloc < done + UNICODE_MAX_LENGTH) {
			devanagari = realloc(devanagari, alloc + CHUNKSIZE);
			alloc += CHUNKSIZE;
		}

		/* consonant (.l) */
		if (strncmp(src, "\u1e37", 3) == 0) {
			letter = letter_by_data(table, src + 3);
			if (letter && letter->type == VOWEL) {
				utf8_pack_char(devanagari + done, 0x0933);
				done += 3;
				src += 3;
				goto encode_vowel_modifier;
			}
		}

		/* candrabindu */
		if (strncmp(src, "m\u0310", 3) == 0) {
			utf8_pack_char(devanagari + done, 0x0901);
			done += 3;
			src += 3;
			continue;
		}

		letter = letter_by_data(table, src);
		if (letter) {
			utf8_pack_char(devanagari + done, letter->code);
			len = utf8_char_length(letter->code);
			done += len;
			src += strlen(letter->data);

			if (letter->type == VOWEL || letter->type == CODA)
				continue;
encode_vowel_modifier:
			next = vowel_sign_by_data(table, src);
			if (next) {
				utf8_pack_char(devanagari + done, next->code);
				done += utf8_char_length(next->code);
				src += strlen(next->data);
			} else {
				if (*src == SCHWA_CHARACTER) {
					src++;
				} else {
					if (letter->type == CONSONANT) {
						utf8_pack_char(devanagari + done, VIRAMA);
						done += utf8_char_length(VIRAMA);
					}
				}
			}
		} else {
			devanagari[done++] = *src++;
		}
	}

	if (devanagari)
		devanagari[done] = '\0';

	return devanagari;
}
