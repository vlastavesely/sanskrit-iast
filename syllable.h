#ifndef __SYLLABE_H
#define __SYLLABE_H

struct syllable {
	char *data;
	struct syllable *next;
};

struct syllable *syllable_alloc();
void syllable_drop(struct syllable *syllable);

struct syllable *syllable_append(struct syllable *tail, const char *data);

#endif /* __SYLLABE_H */
