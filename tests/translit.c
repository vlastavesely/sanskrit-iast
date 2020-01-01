#include "translit.h"
#include "../transliteration.h"


START_TEST(test_translit_devanagari_to_latin)
{
	char *latin;

	/* https://en.wikipedia.org/wiki/Sanskrit */
	latin = transliterate_devanagari_to_latin("संस्कृतम्");
	ck_assert_str_eq("saṃskṛtam", latin);
	free(latin);

	/* https://en.wikipedia.org/wiki/Bhagavad_Gita */
	latin = transliterate_devanagari_to_latin("भगवद्गीता");
	ck_assert_str_eq("bhagavadgītā", latin);
	free(latin);

	/* https://en.wikipedia.org/wiki/%C4%80ry%C4%81varta */
	latin = transliterate_devanagari_to_latin("आर्यावर्त");
	ck_assert_str_eq("āryāvarta", latin);
	free(latin);

	/* https://en.wikipedia.org/wiki/Mahabharata */
	latin = transliterate_devanagari_to_latin("महाभारतम्");
	ck_assert_str_eq("mahābhāratam", latin);
	free(latin);
}
END_TEST

void register_translit_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_translit_devanagari_to_latin);
}
