#include "test.h"
#include "encoder.h"
#include "../encoder.h"

static void test_encoding(const char *in, const char *expected)
{
	char *iast = encode_iast_punctation(in);
	ck_assert_str_eq(expected, iast);
	free(iast);
}

START_TEST(test_encode_punctation)
{
	test_encoding("sam.skr.tam", "saṃskṛtam");

	test_encoding("yogah.", "yogaḥ");

	test_encoding("tantras,a-stram", "tantraśāstram");

	test_encoding("tantrašástram", "tantraśāstram");
}
END_TEST

void register_encoder_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_encode_punctation);
}
