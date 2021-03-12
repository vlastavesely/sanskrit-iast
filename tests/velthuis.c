#include "test.h"
#include "velthuis.h"
#include "../velthuis.h"

static void test_encoding(const char *in, const char *expected)
{
	char *iast;
	encode_velthuis_to_iast_punctation(in, &iast);
	ck_assert_str_eq(expected, iast);
	free(iast);
}

START_TEST(test_encode_punctation)
{
	test_encoding("sa.msk.rtam", "saṃskṛtam");

	test_encoding("yoga.h", "yogaḥ");

	test_encoding("tantra\"saastram", "tantraśāstram");

	test_encoding("Aa AA - II Ii - .RR .Rr", "Ā Ā - Ī Ī - Ṝ Ṝ");
}
END_TEST

void register_velthuis_encoder_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_encode_punctation);
}
