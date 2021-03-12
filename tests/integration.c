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

START_TEST(test_integration)
{
	test_file_transliteration("tests/texts/bhagavadgita-1.txt");
	test_file_transliteration("tests/texts/mandukya-upanishad.txt");
	test_file_transliteration("tests/texts/rg-01001.txt");
}
END_TEST

START_TEST(test_version)
{
	char *out = exec_command("./iast -v");
	ck_assert_str_eq("iast v2.0.0\n", out);
	free(out);
}
END_TEST

START_TEST(test_usage)
{
	char *out;
	out = exec_command("./iast");
	out[44] = '\0'; /* the first line is enough here */
	ck_assert_str_eq("iast, a helper for Sanskrit transliteration.", out);
	free(out);
}
END_TEST

void register_integration_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_integration);
	tcase_add_test(test_case, test_version);
	tcase_add_test(test_case, test_usage);
}
