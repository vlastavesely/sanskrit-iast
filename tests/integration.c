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

START_TEST(test_transcript_czech)
{
	test_output("./iast -c \"भगवद्गीता\"", "bhagavadgíta\n");
	test_output("./iast --czech \"तन्त्रशास्त्रम्\"", "tantrašástra\n");
}
END_TEST

START_TEST(test_transcript_hindi)
{
	test_output("./iast -H \"हिन्दी\"", "hindee\n");
	test_output("./iast --hindi \"लड़की\"", "ladakee\n");
}
END_TEST

START_TEST(test_velthuis)
{
	test_output("./iast \".rta.m ca satyam\" -e", "ṛtaṃ ca satyam\n");
	test_output("./iast \".rta.m ca satyam\" --encode", "ṛtaṃ ca satyam\n");
	test_output("./iast \".rta.m ca satyam\" --velthuis", "ṛtaṃ ca satyam\n");

	/* reverse */
	test_output("./iast \"ṛtaṃ ca satyam\" -er", ".rta.m ca satyam\n");

	/* force Devanagari output */
	test_output("./iast \"agnii.h\" -ed", "अग्नीः\n");
}
END_TEST

START_TEST(test_harvard)
{
	test_output("./iast \"RtaM\" -k", "ṛtaṃ\n");
	test_output("./iast \"RtaM\" --harvard", "ṛtaṃ\n");
	test_output("./iast \"RtaM\" --kyoto", "ṛtaṃ\n");

	/* force Devanagari output */
	test_output("./iast \"agnIH zatruH\" -kd", "अग्नीः शत्रुः\n");
}
END_TEST

START_TEST(test_ascii)
{
	test_output("./iast \"अग्निमीळे पुरोहितं\" -a", "agnimii.le purohita.m\n");
}
END_TEST

#define TEST_FILE "./tests/output.test"
START_TEST(test_file_output)
{
	char *str;

	unlink(TEST_FILE);
	str = exec_command("./iast -eo "TEST_FILE" .rta.m");
	free(str);

	str = read_file(TEST_FILE);
	ck_assert_str_eq("ṛtaṃ\n", str);
	free(str);

	unlink(TEST_FILE);
}
END_TEST

START_TEST(test_version)
{
	test_output("./iast -v", "iast v3.1.0\n");
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
	test_output("./iast -Hr 2>&1", "[iast] error: invalid combination of '-H' and '-r'.\n");
	test_output("./iast -cr 2>&1", "[iast] error: invalid combination of '-c' and '-r'.\n");
}
END_TEST

void register_integration_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_transliterate_files);
	tcase_add_test(test_case, test_transliterate_arguments);
	tcase_add_test(test_case, test_transcript_czech);
	tcase_add_test(test_case, test_transcript_hindi);
	tcase_add_test(test_case, test_velthuis);
	tcase_add_test(test_case, test_harvard);
	tcase_add_test(test_case, test_ascii);
	tcase_add_test(test_case, test_file_output);
	tcase_add_test(test_case, test_version);
	tcase_add_test(test_case, test_usage);
	tcase_add_test(test_case, test_errors);
}
