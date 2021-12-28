#include "test.h"
#include "velthuis.h"
#include "../velthuis.h"

#define ZWJ  "\u200d"
#define ZWNJ "\u200c"

static void test_velthuis(const char *velthuis, const char *iast)
{
	char *a, *b;
	int ret;

	ret = encode_velthuis_to_iast(velthuis, &a);
	ck_assert_int_eq(0, ret);
	ck_assert_str_eq(iast, a);

	ret = encode_iast_to_velthuis(a, &b);
	ck_assert_int_eq(0, ret);
	ck_assert_str_eq(velthuis, b);

	free(a);
	free(b);
}

static void test_velthuis_oneway(const char *velthuis, const char *iast)
{
	char *str;
	int ret;

	ret = encode_velthuis_to_iast(velthuis, &str);
	ck_assert_int_eq(0, ret);
	ck_assert_str_eq(iast, str);

	free(str);
}

START_TEST(test_velthuis_encoding)
{
	test_velthuis("sa.msk.rtam", "saṃskṛtam");
	test_velthuis("yoga.h", "yogaḥ");
	test_velthuis("tantra\"saastram", "tantraśāstram");
	test_velthuis("Aa Ii .Rr", "Ā Ī Ṝ");
	test_velthuis("atha prathamo.adhyaaya.h", "atha prathamo'dhyāyaḥ");
}
END_TEST

START_TEST(test_encode_zwnj_and_zwj)
{
	test_velthuis("ka+i", "ka"ZWJ"i");
	test_velthuis("ka-i", "ka"ZWNJ"i");
}
END_TEST

START_TEST(test_encode_oneway)
{
	test_velthuis_oneway("puuu puu{}u pu{}uu", "pūu pūu puū");
}
END_TEST

void register_velthuis_encoder_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_velthuis_encoding);
	tcase_add_test(test_case, test_encode_zwnj_and_zwj);
	tcase_add_test(test_case, test_encode_oneway);
}
