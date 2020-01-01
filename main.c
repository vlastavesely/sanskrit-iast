#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>

#include "transliteration.h"
#include "encoder.h"

#define PROGNAME "iast"
#define VERSION "0.1"

#define FLAG_STDIN	1 << 0
#define FLAG_CZECH	1 << 1
#define FLAG_ENCODE	1 << 2


static const char *usage_str =
	PROGNAME ", a helper for Sanskrit transliteration.\n"
	"\n"
	"usage:\n"
	"  " PROGNAME " [flags and text arguments in any order]\n"
	"\n"
	"options:\n"
	"  -h     shows this help and exits\n"
	"  -v     shows version number and exits\n"
	"  -c     transliterate to Czech language\n"
	"  -e     convert symbolic ASCII text to IAST representation\n"
	"  --     read data from the standard input\n"
	"\n"
	"  By default, `" PROGNAME "` takes all input arguments written in Devanagari\n"
	"  and transliterates them to IAST version.\n"
	"\n"
	"  When flag `-e` is set up, the program converts purely ASCII-encoded\n"
	"  strings into special characters of IAST alphabet. For example, it\n"
	"  converts `sam.skr.tam` to `saṃskṛtam` or `s,a-stram` to `śāstram`.\n";

static void print_usage()
{
	fprintf(stdout, "%s\n", usage_str);
}

static void print_version()
{
	fprintf(stdout, PROGNAME " v" VERSION "\n");
}

static void error(const char *msg, ...)
{
	va_list params;
	char buf[256];

	va_start(params, msg);
	vsnprintf(buf, sizeof(buf), msg, params);
	fprintf(stderr, "[" PROGNAME "] error: %s\n", buf);
	va_end(params);
}

static char *stdin_read()
{
	char buffer[1024];
	unsigned int n, length = 0;;
	char *text = NULL;

	while ((n = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {

		text = realloc(text, length + n + 1);
		if (text == NULL)
			return NULL;

		strncpy(text + length, buffer, n);
		length += n;
	}

	if (n == -1)
		return NULL;

	return text;
}

int main(int argc, const char **argv)
{
	int i;
	unsigned int flags = 0, n = 0;
	const char *arg;
	const char *queue[argc];
	char *input, *output;

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
			case 'h':
				print_usage();
				return 0;
			case 'v':
				print_version();
				return 0;
			}

			error("unknown option '%s'.", arg);
			return -1;
		} else {
			queue[n++] = arg;
		}
	}

	if (flags & FLAG_STDIN) {
		input = stdin_read();
		if (input == NULL) {
			error("failed to read from standard input.");
			return -1;
		}

		if (flags & FLAG_ENCODE) {
			output = encode_iast_punctation(input);
		} else {
			output = transliterate_devanagari_to_latin(input);
		}

		fprintf(stdout, "%s\n", output);
		free(output);
		free(input);
	}

	for (i = 0; i < n; i++) {
		if (flags & FLAG_ENCODE) {
			output = encode_iast_punctation(queue[i]);
		} else {
			output = transliterate_devanagari_to_latin(queue[i]);
		}

		fprintf(stdout, "%s\n", output);
		free(output);
	}

	return 0;
}
