#include "compat.h"
#include "transliteration.h"
#include "transcription.h"
#include "velthuis.h"
#include "config.h"

#define FLAG_REVERSE	1 << 0
#define FLAG_VELTHUIS	1 << 1
#define FLAG_CZECH	1 << 2

static const char *usage_str =
	PROGNAME ", a helper for Sanskrit transliteration.\n"
	"\n"
	"Usage:\n"
	"  " PROGNAME " [flags and text arguments in any order]\n"
	"\n"
	"Options:\n"
	"  -f, --file        the input file for transliteration\n"
	"  -r, --reverse     reverse transliteration (from Latin to Devanagari)\n"
	"  -e, --encode      convert an ASCII text to IAST using the Velthuis scheme\n"
	"  -c, --czech       transcript Devanagari to Czech language (experimental)\n"
	"  -h, --help        show this help and exit\n"
	"  -v, --version     show version number and exit\n"
	"\n"
	"Information:\n"
	"  By default, the program takes all non-option text arguments written in\n"
	"  Devanagari and transliterates them into the IAST version. When the FILE\n"
	"  is set to '-', the standard input shall be read for input. The input data\n"
	"  are expected to be a valid Unicode string.\n"
	"\n"
	"  Since the program outputs Unicode characters, you need to ensure that your\n"
	"  terminal emulator is able to display the characters correctly if you are\n"
	"  printing those into the console instead of a file.\n"
	"\n"
	"  For more information see the iast(1) manual page.\n";

static const char *short_opts = "f:rechv";

static const struct option long_opts[] = {
	{"file",       required_argument,  0, 'f'},
	{"reverse",    no_argument,        0, 'r'},
	{"encode",     no_argument,        0, 'e'},
	{"velthuis",   no_argument,        0, 'e'},
	{"czech",      no_argument,        0, 'c'},
	{"help",       no_argument,        0, 'h'},
	{"version",    no_argument,        0, 'v'},
	{0, 0, 0, 0}
};

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

static int process_input(const char *input, char **out, unsigned int flags)
{
	if (flags & FLAG_REVERSE)
		return transliterate_latin_to_devanagari(input, out);

	if (flags & FLAG_VELTHUIS)
		return encode_velthuis_to_iast_punctation(input, out);

	if (flags & FLAG_CZECH)
		return transcript_devanagari_to_czech(input, out);

	return transliterate_devanagari_to_latin(input, out);
}

static int process_string(const char *input, unsigned int flags)
{
	char *output;
	int ret;

	ret = process_input(input, &output, flags);

	switch (ret) {
	case 0:
		fprintf(stdout, "%s", output);
		break;
	case EHINDI:
		error("the input text is Hindi.");
		break;
	default:
		error("unexpected error.");
		break;
	}

	free(output);

	return ret;
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
	int i, retval, c = 0, opt_index = 0;
	const char *files[argc];
	unsigned int nfiles = 0;
	unsigned int flags = 0;
	char *input;

	if (argc == 1) {
		print_usage();
		return -1;
	}

	opterr = 0; /* disable the auto error message */
	while (c != -1) {
		c = getopt_long(argc, (char * const *) argv, short_opts,
				long_opts, &opt_index);

		switch (c) {
		case 'f':
			files[nfiles++] = optarg;
			break;
		case 'r':
			flags |= FLAG_REVERSE;
			break;
		case 'e':
			flags |= FLAG_VELTHUIS;
			break;
		case 'c':
			flags |= FLAG_CZECH;
			break;
		case 'h':
			print_usage();
			return 0;
		case 'v':
			print_version();
			return 0;
		case '?':
			error("unrecognised option '-%s'.", optopt ?
			      (char *) &(optopt) : argv[optind - 1] + 1);
			break;
		default:
			break;
		}
	}

	while (optind < argc) {
		const char *arg = argv[optind++];

		retval = process_string(arg, flags);
		if (retval != 0)
			return retval;
		putchar('\n');
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
		retval = process_string(input, flags);
		free(input);
		if (retval != 0)
			return retval;
	}

	return 0;
}
