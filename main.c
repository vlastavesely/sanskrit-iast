#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "transliteration.h"
#include "iast.h"
#include "iast-czech.h"

#define FLAG_CZECH	1 << 0

int main(int argc, const char **argv)
{
	struct transliteration_context *context;
	unsigned int n, length = 0;
	int i, retval = 0;
	char buffer[6];
	char *text = malloc(0);
	char *out;
	const char *arg;
	unsigned int flags = 0;


	for (i = 0; i < argc; i++) {
		arg = argv[i];

		if (*arg == '-') {
			switch (arg[1]) {
			case 'c':
				flags |= FLAG_CZECH;
				continue;
			}

			fprintf(stderr, "error: unknown option '%s'\n", arg);
			exit(1);
		}
	}


	while ((n = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
		text = realloc(text, length + n + 1);
		strncpy(text + length, buffer, n);
		length += n;
	}
	if (n == -1) {
		retval = -errno;
		goto out;
	}

	text[length] = '\0';

	if (flags & FLAG_CZECH) {
		context = transliteration_context_iast_czech_alloc();
	} else {
		context = transliteration_context_iast_alloc();
	}

	out = transliterate_devanagari_to_latin(text, context);
	printf("%s\n", out);

	transliteration_context_drop(context);
	free(text);
	free(out);

out:
	return retval;
}
