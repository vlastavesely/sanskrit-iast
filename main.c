#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "transliteration.h"
#include "iast.h"
#include "iast-czech.h"
#include "encoder.h"

#define FLAG_STDIN	1 << 0
#define FLAG_CZECH	1 << 1
#define FLAG_ENCODE	1 << 2


static char *stdin_read()
{
	char buffer[1024];
	unsigned int n, length = 0;;
	char *text = NULL;

	while ((n = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
		if (text == NULL)
			text = malloc(n + 1);
		else
			text = realloc(text, length + n + 1);
		strncpy(text + length, buffer, n);
		length += n;
	}

	if (n == -1)
		return NULL;

	return text;
}

int main(int argc, const char **argv)
{
	int i, retval = 0;
	unsigned int flags = 0, n = 0;
	const char *arg;
	const char *queue[argc];
	char *input, *output;
	struct transliteration_context *context;

	for (i = 1; i < argc; i++) {
		arg = argv[i];

		if (*arg == '-') {
			switch (arg[1]) {
			case '-':
				flags |= FLAG_STDIN;
				continue;
			case 'c':
				flags |= FLAG_CZECH;
				continue;
			case 'e':
				flags |= FLAG_ENCODE;
				continue;
			}

			fprintf(stderr, "error: unknown option '%s'\n", arg);
			exit(1);
		} else {
			queue[n++] = arg;
		}
	}

	if (flags & FLAG_ENCODE) {
		for (i = 0; i < n; i++) {
			output = encode_iast_punctation(queue[i]);
			fprintf(stdout, "%s\n", output);
			free(output);
		}
		goto out;
	}

	context = (flags & FLAG_CZECH)
		? transliteration_context_iast_czech_alloc()
		: transliteration_context_iast_alloc();

	if (flags & FLAG_STDIN) {
		input = stdin_read();
		if (input == NULL) {
			fprintf(stderr, "[iast] failed to read from STDIN.\n");
			retval = -1;
			goto drop_context;
		}

		output = transliterate_devanagari_to_latin(input, context);
		fprintf(stdout, "%s\n", output);
		free(output);
		free(input);
	}

	for (i = 0; i < n; i++) {
		output = transliterate_devanagari_to_latin(queue[i], context);
		fprintf(stdout, "%s\n", output);
		free(output);
	}

drop_context:
	transliteration_context_drop(context);
out:
	return retval;
}
