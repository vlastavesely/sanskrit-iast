#include "test.h"
#include "translit.h"
#include "../compat.h"
#include "../transliteration.h"

static void test_transliterate_devanagari_to_latin(const char *devanagari,
						   const char *latin)
{
	char *str;
	int ret;

	ret = transliterate_devanagari_to_latin(devanagari, &str);
	ck_assert_int_eq(0, ret);
	ck_assert_str_eq(latin, str);
	free(str);
}

static void test_transliterate_latin_to_devanagari(const char *latin,
						   const char *devanagari)
{
	char *str;
	int ret;

	ret = transliterate_latin_to_devanagari(latin, &str);
	ck_assert_str_eq(devanagari, str);
	free(str);
}

START_TEST(test_translit_devanagari_to_latin)
{
	/* https://en.wikipedia.org/wiki/Sanskrit */
	test_transliterate_devanagari_to_latin("संस्कृतम्", "saṃskṛtam");

	/* https://en.wikipedia.org/wiki/Bhagavad_Gita */
	test_transliterate_devanagari_to_latin("भगवद्गीता", "bhagavadgītā");

	/* https://en.wikipedia.org/wiki/%C4%80ry%C4%81varta */
	test_transliterate_devanagari_to_latin("आर्यावर्त", "āryāvarta");

	/* https://en.wikipedia.org/wiki/Mahabharata */
	test_transliterate_devanagari_to_latin("महाभारतम्", "mahābhāratam");

	/* https://en.wikipedia.org/wiki/Devanagari */
	test_transliterate_devanagari_to_latin("देवनागरी", "devanāgarī");

}
END_TEST

START_TEST(test_translit_latin_to_devanagari)
{
	test_transliterate_latin_to_devanagari("saṃskṛtam", "संस्कृतम्");
	test_transliterate_latin_to_devanagari("bhagavadgītā", "भगवद्गीता");
	test_transliterate_latin_to_devanagari("āryāvarta", "आर्यावर्त");
	test_transliterate_latin_to_devanagari("mahābhāratam", "महाभारतम्");
	test_transliterate_latin_to_devanagari("devanāgarī", "देवनागरी");
}
END_TEST

START_TEST(test_translit_lla_sylable)
{
	test_transliterate_devanagari_to_latin("अग्निमीळे", "agnimīḷe");
	test_transliterate_latin_to_devanagari("agnimīḷe", "अग्निमीळे");

	test_transliterate_devanagari_to_latin("चाकॢप्र", "cākḷpra");
	test_transliterate_latin_to_devanagari("cākḷpra", "चाकॢप्र");
}
END_TEST

START_TEST(test_translit_candrabindu)
{
	test_transliterate_devanagari_to_latin("तान्यजत्राँ", "tānyajatrām̐");
	test_transliterate_latin_to_devanagari("tānyajatrām̐", "तान्यजत्राँ");
}
END_TEST

START_TEST(test_translit_detect_hindi)
{
	char *hindi = NULL;
	int ret = transliterate_devanagari_to_latin("लड़की", &hindi);
	ck_assert_int_eq(EHINDI, ret);
}
END_TEST

void register_translit_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_translit_devanagari_to_latin);
	tcase_add_test(test_case, test_translit_latin_to_devanagari);
	tcase_add_test(test_case, test_translit_lla_sylable);
	tcase_add_test(test_case, test_translit_candrabindu);
	tcase_add_test(test_case, test_translit_detect_hindi);
}
