#include "test.h"
#include "translit.h"
#include "../compat.h"
#include "../transliteration.h"

#define ZWJ  "\u200d"
#define ZWNJ "\u200c"

static void test_translit(const char *devanagari, const char *latin)
{
	char *a, *b;
	int ret;

	ret = transliterate_devanagari_to_latin(devanagari, &a);
	ck_assert_int_eq(0, ret);
	ck_assert_str_eq(latin, a);

	ret = transliterate_latin_to_devanagari(a, &b);
	ck_assert_int_eq(0, ret);
	ck_assert_str_eq(devanagari, b);

	free(a);
	free(b);
}

static void test_translit_latin(const char *latin, const char *devanagari)
{
	char *str;
	int ret;

	ret = transliterate_latin_to_devanagari(latin, &str);
	ck_assert_int_eq(0, ret);
	ck_assert_str_eq(devanagari, str);

	free(str);
}

START_TEST(test_translit_words)
{
	/* https://en.wikipedia.org/wiki/Sanskrit */
	test_translit("संस्कृतम्", "saṃskṛtam");

	/* https://en.wikipedia.org/wiki/Bhagavad_Gita */
	test_translit("भगवद्गीता", "bhagavadgītā");

	/* https://en.wikipedia.org/wiki/%C4%80ry%C4%81varta */
	test_translit("आर्यावर्त", "āryāvarta");

	/* https://en.wikipedia.org/wiki/Mahabharata */
	test_translit("महाभारतम्", "mahābhāratam");

	/* https://en.wikipedia.org/wiki/Devanagari */
	test_translit("देवनागरी", "devanāgarī");
}
END_TEST

START_TEST(test_translit_vedic)
{
	/* rigveda 1.25.4 */
	test_translit("वस्यइष्टये", "vasya"ZWNJ"iṣṭaye");

	/* rigveda 3.5.2 */
	test_translit("पूर्वीर्ऋतस्य", "pūrvīr"ZWNJ"ṛtasya");

	/* vedic stresses */
	test_translit("अ॒ग्निमी॑ळे पु॒रोहि॑तं", "a\\_gnimī\\'ḷe pu\\_rohi\\'taṃ");
}
END_TEST

START_TEST(test_translit_lla_sylable)
{
	test_translit("अग्निमीळे", "agnimīḷe"); /* rigveda 1.1.1 */
	test_translit("चाकॢप्र", "cākḷpra"); /* rigveda 10.130.5 */
	test_translit("समूळ्हमस्य", "samūḷhamasya"); /* rigveda 1.22.17 */

	test_translit("ऌ ऌप ऌळ", "ḷ ḷpa ḷḷa"); /* at the beginning = vowel */
	test_translit("ळ ळे ळृ", "ḷa ḷe ḷṛ"); /* followed by a vowel = consonant */
	test_translit("प्ळ पळ ओप्ळ", "pḷa paḷa opḷa");
}
END_TEST

START_TEST(test_translit_aum)
{
	test_translit("ॐ औम औमे तौमे ॐ", "aum auma aume taume aum");
}

START_TEST(test_translit_avagraha)
{
	test_translit_latin("śivo'ham", "शिवोऽहम्");
	test_translit_latin("śivo’ham", "शिवोऽहम्");
}

START_TEST(test_translit_candrabindu)
{
	test_translit("तान्यजत्राँ", "tānyajatrām̐");
}
END_TEST

START_TEST(test_translit_zero_width_joiner)
{
	test_translit("श‍ृ शृ", "ś"ZWJ"ṛ śṛ");
}
END_TEST

void register_translit_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_translit_words);
	tcase_add_test(test_case, test_translit_vedic);
	tcase_add_test(test_case, test_translit_lla_sylable);
	tcase_add_test(test_case, test_translit_aum);
	tcase_add_test(test_case, test_translit_avagraha);
	tcase_add_test(test_case, test_translit_candrabindu);
	tcase_add_test(test_case, test_translit_zero_width_joiner);
}
