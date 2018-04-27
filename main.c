#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "transliteration.h"
#include "iast.h"

int main(int argc, const char **argv)
{
	struct transliteration_context *context;
	unsigned int n, length = 0;
	int retval = 0;
	char buffer[6];
	char *text = malloc(0);
	char *out;

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

	context = transliteration_context_iast_alloc();

	out = transliterate_devanagari_to_latin(text, context);
	printf("%s\n", out);

	transliteration_context_drop(context);
	free(text);
	free(out);

out:
	return retval;
}
