/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __SYLLABE_H
#define __SYLLABE_H

struct syllable {
	char *data;
	unsigned int code;
	struct syllable *prev;
	struct syllable *next;
};

struct syllable *syllable_alloc(const char *data, unsigned int code);
void syllable_drop(struct syllable *syllable);

unsigned int syllable_chain_length(struct syllable *head);
char *syllable_chain_to_string(struct syllable *head);
void syllable_chain_drop(struct syllable *head);

#endif /* __SYLLABE_H */
