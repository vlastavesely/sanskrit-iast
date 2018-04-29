/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __SYLLABE_H
#define __SYLLABE_H

struct syllable {
	char *data;
	unsigned int code;
	struct syllable *prev;
	struct syllable *next;
};

struct syllable *syllable_alloc();
void syllable_drop(struct syllable *syllable);

struct syllable *syllable_append(struct syllable *tail, const char *data);

unsigned int syllable_chain_length(struct syllable *head);
char *syllable_chain_to_string(struct syllable *head);

#endif /* __SYLLABE_H */
