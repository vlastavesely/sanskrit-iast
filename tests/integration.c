#include "test.h"
#include "integration.h"
#include "../transliteration.h"

static char *exec_command(const char *command)
{
	char buf[512], *ret = NULL;
	FILE *proc;
	int n, len = 0;

	proc = popen(command, "r");

	while (1) {
		n = fread(buf, 1, sizeof(buf), proc);
		if (n == 0 || n == -1)
			break;
		ret = realloc(ret, len + n + 1);
		memcpy(ret + len, buf, n);
		len += n;
	}

	if (ret)
		ret[len] = '\0';

	pclose(proc);

	return ret;
}

static char *read_file(const char *filename)
{
	unsigned char buf[512];
	char *ret = NULL;
	int fd, len = 0, n;

	fd = open(filename, O_RDONLY);
	while (1) {
		n = read(fd, buf, sizeof(buf));
		if (n == 0 || n == -1)
			break;
		ret = realloc(ret, len + n + 1);
		memcpy(ret + len, buf, n);
		len += n;
	}

	if (ret)
		ret[len] = '\0';

	close(fd);

	return ret;
}

static void test_file_transliteration(const char *filename)
{
	char cmd[512], *a, *b;

	sprintf(cmd, "./iast -f %s | ./iast -r -f -", filename);
	a = exec_command(cmd);
	b = read_file(filename);

	ck_assert_str_eq(a, b);

	free(a);
	free(b);
}

START_TEST(test_transliterate_files)
{
	test_file_transliteration("tests/texts/bhagavadgita-1.txt");
	test_file_transliteration("tests/texts/mandukya-upanishad.txt");
	test_file_transliteration("tests/texts/rg-01001.txt");
}
END_TEST

static void test_output(const char *command, const char *expected)
{
	char *out;

	out = exec_command(command);
	ck_assert_str_eq(expected, out);
	free(out);
}

START_TEST(test_transliterate_arguments)
{
	test_output("./iast \"भगवद्गीता\"", "bhagavadgītā\n");
}
END_TEST

START_TEST(test_transcript)
{
	test_output("./iast -c \"भगवद्गीता\"", "bhagavadgíta\n");
	test_output("./iast --czech \"तन्त्रशास्त्रम्\"", "tantrašástra\n");
}
END_TEST

START_TEST(test_velthuis)
{
	test_output("./iast \"r.ta.m ca satyam\" -e", "rṭaṃ ca satyam\n");
	test_output("./iast \"r.ta.m ca satyam\" --encode", "rṭaṃ ca satyam\n");
	test_output("./iast \"r.ta.m ca satyam\" --velthuis", "rṭaṃ ca satyam\n");
}
END_TEST

START_TEST(test_ascii)
{
	test_output("./iast \"अग्निमीळे पुरोहितं\" -a", "agnimii.le purohita.m\n");
}
END_TEST

START_TEST(test_version)
{
	test_output("./iast -v", "iast v2.0.0\n");
}
END_TEST

START_TEST(test_usage)
{
	char *out;

	out = exec_command("./iast");
	out[44] = '\0'; /* the first line is enough here */
	ck_assert_str_eq("iast, a helper for Sanskrit transliteration.", out);
	free(out);

	out = exec_command("./iast -h");
	out[44] = '\0';
	ck_assert_str_eq("iast, a helper for Sanskrit transliteration.", out);
	free(out);
}
END_TEST

START_TEST(test_errors)
{
	test_output("./iast -x 2>&1", "[iast] error: unrecognised option '-x'.\n");
	test_output("./iast -f xxx 2>&1", "[iast] error: failed to read file 'xxx'.\n");
	test_output("./iast \u0921\u093c 2>&1", "[iast] error: the input text is Hindi.\n");
}
END_TEST

void register_integration_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_transliterate_files);
	tcase_add_test(test_case, test_transliterate_arguments);
	tcase_add_test(test_case, test_transcript);
	tcase_add_test(test_case, test_velthuis);
	tcase_add_test(test_case, test_ascii);
	tcase_add_test(test_case, test_version);
	tcase_add_test(test_case, test_usage);
	tcase_add_test(test_case, test_errors);
}
