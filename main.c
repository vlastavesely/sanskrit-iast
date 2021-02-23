#include "compat.h"
#include "transliteration.h"
#include "transcription.h"
#include "velthuis.h"
#include "config.h"

#define PROGNAME "iast"

#define FLAG_REVERSE	1 << 0
#define FLAG_ENCODE	1 << 1
#define FLAG_CZECH	1 << 2

static const char *usage_str =
	PROGNAME ", a helper for Sanskrit transliteration.\n"
	"\n"
	"usage:\n"
	"  " PROGNAME " [flags and text arguments in any order]\n"
	"\n"
	"options:\n"
	"  -h     shows this help and exits\n"
	"  -v     shows version number and exits\n"
	"  -f     specifies file for conversion (‘-’ means standard input)\n"
	"  -r     reverse transliteration (from Latin to Devanagari)\n"
	"  -e     convert Velthuis scheme text to IAST representation\n"
	"  -c     transcript Devanagari to Czech language\n"
	"\n"
	"  By default, ‘" PROGNAME "’ takes all input arguments written in Devanagari\n"
	"  and transliterates them to the IAST version.\n"
	"\n"
	"  When the flag ‘-e’ is set up, the program converts purely ASCII-encoded\n"
	"  strings into the special characters of the IAST alphabet. For example, it\n"
	"  converts ‘sam.skr.tam’ to ‘saṃskṛtam’ or ‘s,a-stram’ to ‘śāstram’.\n";

static void print_usage()
{
	fprintf(stdout, "%s\n", usage_str);
}

static void print_version()
{
	fprintf(stdout, PROGNAME " v" PACKAGE_VERSION "\n");
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

static char *process_input(const char *input, unsigned int flags)
{
	if (flags & FLAG_ENCODE)
		return encode_velthuis_to_iast_punctation(input);

	if (flags & FLAG_REVERSE)
		return transliterate_latin_to_devanagari(input);

	if (flags & FLAG_CZECH)
		return transcript_devanagari_to_czech(input);

	return transliterate_devanagari_to_latin(input);
}

#define CHUNKSIZE 1024
static int read_fd(char **out, int fd)
{
	int n, alloc = 0, space, len = 0;
	char *buf = NULL;

	while (1) {
		space = alloc - len;
		if (space == 0) {
			space = CHUNKSIZE;
			alloc += space;
			buf = realloc(buf, alloc + 1); /* + 1 is '\0' */
		}
		n = read(fd, buf + len, space);
		if (n > 0) {
			len += n;
			continue;
		} else if (n == 0) {
			break;
		}

		free(buf);
		return -errno;
	}

	if (buf)
		buf[len] = '\0';

	*out = buf;

	return 0;
}

static int read_file(char **out, const char *path)
{
	int fd, retval;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return -errno;

	retval = read_fd(out, fd);
	close(fd);

	return retval;
}

int main(int argc, const char **argv)
{
	int i, retval;
	const char *arg;
	const char *files[argc], *texts[argc];
	unsigned int nfiles = 0, ntexts = 0;
	unsigned int flags = 0;
	char *input, *output;

	if (argc == 1) {
		print_usage();
		return -1;
	}

	for (i = 1; i < argc; i++) {
		arg = argv[i];

		if (*arg == '-') {
			switch (arg[1]) {
			case 'r':
				flags |= FLAG_REVERSE;
				continue;
			case 'e':
				flags |= FLAG_ENCODE;
				continue;
			case 'c':
				flags |= FLAG_CZECH;
				continue;
			case 'f':
				files[nfiles++] = argv[++i];
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
			texts[ntexts++] = arg;
		}
	}

	for (i = 0; i < ntexts; i++) {
		output = process_input(texts[i], flags);
		fprintf(stdout, "%s\n", output);
		free(output);
	}

	for (i = 0; i < nfiles; i++) {
		if (strcmp(files[i], "-") == 0) {
			retval = read_fd(&input, STDIN_FILENO);
		} else {
			retval = read_file(&input, files[i]);
		}

		if (retval != 0) {
			error("failed to read file '%s'.", files[i]);
			return retval;
		}

		output = process_input(input, flags);
		fprintf(stdout, "%s", output);
		free(output);
		free(input);
	}

	return 0;
}
