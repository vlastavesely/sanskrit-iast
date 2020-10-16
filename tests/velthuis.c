#include "test.h"
#include "velthuis.h"
#include "../velthuis.h"

static void test_encoding(const char *in, const char *expected)
{
	char *iast = encode_velthuis_to_iast_punctation(in);
	ck_assert_str_eq(expected, iast);
	free(iast);
}

START_TEST(test_encode_punctation)
{
	test_encoding("sa.msk.rtam", "saṃskṛtam");

	test_encoding("yoga.h", "yogaḥ");

	test_encoding("tantra\"saastram", "tantraśāstram");
}
END_TEST

void register_velthuis_encoder_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_encode_punctation);
}
