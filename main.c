#include "compat.h"
#include "transliteration.h"
#include "transcription.h"
#include "encoder.h"

#define PROGNAME "iast"
#define VERSION "1.0"

#define FLAG_STDIN	1 << 0
#define FLAG_REVERSE	1 << 1
#define FLAG_ENCODE	1 << 2
#define FLAG_CZECH	1 << 3


static const char *usage_str =
	PROGNAME ", a helper for Sanskrit transliteration.\n"
	"\n"
	"usage:\n"
	"  " PROGNAME " [flags and text arguments in any order]\n"
	"\n"
	"options:\n"
	"  -h     shows this help and exits\n"
	"  -v     shows version number and exits\n"
	"  -r     reverse transliteration (from Latin to Devanagari)\n"
	"  -e     convert symbolic ASCII text to IAST representation\n"
	"  -c     transcript Devanagari to Czech language\n"
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

static char *process_input(const char *input, unsigned int flags)
{
	if (flags & FLAG_ENCODE) {
		return encode_iast_punctation(input);
	} else if (flags & FLAG_REVERSE) {
		return transliterate_latin_to_devanagari(input);
	} else if (flags & FLAG_CZECH) {
		return transcript_devanagari_to_czech(input);
	} else {
		return transliterate_devanagari_to_latin(input);
	}
}

int main(int argc, const char **argv)
{
	int i;
	unsigned int flags = 0, n = 0;
	const char *arg;
	const char *queue[argc];
	char *input, *output;

	if (argc == 1) {
		print_usage();
		return -1;
	}

	for (i = 1; i < argc; i++) {
		arg = argv[i];

		if (*arg == '-') {
			switch (arg[1]) {
			case '-':
				flags |= FLAG_STDIN;
				continue;
			case 'r':
				flags |= FLAG_REVERSE;
				continue;
			case 'e':
				flags |= FLAG_ENCODE;
				continue;
			case 'c':
				flags |= FLAG_CZECH;
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

	if (flags != FLAG_REVERSE && flags != FLAG_CZECH && flags != FLAG_ENCODE) {
		error("options '-r', '-e' and '-c' are mutually exclusive.");
		return -1;
	}

	if (flags & FLAG_STDIN) {
		input = stdin_read();
		if (input == NULL) {
			error("failed to read from standard input.");
			return -1;
		}

		output = process_input(input, flags);

		fprintf(stdout, "%s\n", output);
		free(output);
		free(input);
	}

	for (i = 0; i < n; i++) {
		output = process_input(queue[i], flags);

		fprintf(stdout, "%s\n", output);
		free(output);
	}

	return 0;
}
