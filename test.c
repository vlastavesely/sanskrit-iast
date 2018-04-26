#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "syllable.h"
#include "utf8.h"
#include "transliteration.h"
#include "iast.h"

struct string_pair {
	const char *devanagari;
	const char *latin;
};

static const struct string_pair table_iast[] = {
	{"संस्कृतम्", "saṃskṛtam"},
	{"आर्यावर्त", "āryāvarta"},
	{"तन्त्रशास्त्रम्", "tantraśāstram"},
	{"ऋग्वेद", "ṛgveda"},
	{"महाभारतम्", "mahābhāratam"},
	{"सरस्वती नदी", "sarasvatī nadī"},
	{"देवनागरी", "devanāgarī"},
	{"योगः", "yogaḥ"},
	{"भगवद्गीता", "bhagavadgītā"},
	{NULL, NULL}
};

int main(int argc, const char **argv)
{
	struct transliteration_context *context;
	const struct string_pair *walk = table_iast;
	char *ptr;
	int retval = 0;
	int match;

	context = transliteration_context_iast_alloc();
	while (walk->devanagari != NULL) {
		ptr = transliterate_devanagari_to_latin(walk->devanagari, context);

		match = strcmp(ptr, walk->latin) == 0;
		printf("\033[%dm", match ? 32 : 31);
		printf("%s = %s", ptr, walk->latin);
		printf("\033[0m\n");

		if (!match)
			retval++;

		free(ptr);
		walk++;
	}

	return retval;
}
