#include "test.h"
#include "velthuis.h"
#include "../velthuis.h"

static void test_encoding_to_iast(const char *in, const char *expected)
{
	char *out;

	encode_velthuis_to_iast(in, &out);
	ck_assert_str_eq(expected, out);
	free(out);
}

static void test_encoding_to_velthuis(const char *in, const char *expected)
{
	char *out;

	encode_iast_to_velthuis(in, &out);
	ck_assert_str_eq(expected, out);
	free(out);
}

START_TEST(test_encode_velthuis_to_iast)
{
	test_encoding_to_iast("sa.msk.rtam", "saṃskṛtam");
	test_encoding_to_iast("yoga.h", "yogaḥ");
	test_encoding_to_iast("tantra\"saastram", "tantraśāstram");
	test_encoding_to_iast("Aa AA - II Ii - .RR .Rr", "Ā Ā - Ī Ī - Ṝ Ṝ");
	test_encoding_to_iast("atha prathamo.adhyaaya.h", "atha prathamo'dhyāyaḥ");
}
END_TEST

START_TEST(test_encode_iast_to_velthuis)
{
	test_encoding_to_velthuis("saṃskṛtam", "sa.msk.rtam");
	test_encoding_to_velthuis("tantraśāstram", "tantra\"saastram");
	test_encoding_to_velthuis("Ā - Ī - Ṝ", "Aa - Ii - .Rr");
	test_encoding_to_velthuis("atha prathamo'dhyāyaḥ", "atha prathamo.adhyaaya.h");
}
END_TEST

void register_velthuis_encoder_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_encode_velthuis_to_iast);
	tcase_add_test(test_case, test_encode_iast_to_velthuis);
}
