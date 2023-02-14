#include "test.h"
#include "harvard-kyoto.h"
#include "../harvard-kyoto.h"

static void test_harvard_kyoto(const char *hk, const char *iast)
{
	char *str;
	int ret;

	ret = encode_harvard_kyoto_to_iast(hk, &str);
	ck_assert_int_eq(0, ret);
	ck_assert_str_eq(iast, str);

	free(str);
}

START_TEST(test_harvard_kyoto_encoding)
{
	test_harvard_kyoto("saMskRtam", "saṃskṛtam");
	test_harvard_kyoto("yogaH", "yogaḥ");
	test_harvard_kyoto("zaastraM zAstram", "śāstraṃ śāstram");
	test_harvard_kyoto("liGga yajJa varNa", "liṅga yajña varṇa");
	test_harvard_kyoto("Aaa Iii Uuu RR R L lR lRR", "āā īī ūū ṝ ṛ ḷ ḷ ḹ");
}
END_TEST

void register_harvard_kyoto_encoder_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_harvard_kyoto_encoding);
}
