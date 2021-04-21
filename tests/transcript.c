#include "test.h"
#include "transcript.h"
#include "../compat.h"
#include "../transcription.h"

static void test_transcript(const char *devanagari, const char *latin)
{
	char *czech;
	int ret;

	ret = transcript_devanagari_to_czech(devanagari, &czech);
	ck_assert_int_eq(0, ret);
	ck_assert_str_eq(latin, czech);
	free(czech);
}

START_TEST(test_transcript_devanagari_to_czech)
{
	test_transcript("तन्त्रशास्त्रम्", "tantrašástra");

	test_transcript("सांख्य", "sánkhja");

	test_transcript("महाभारतम्", "mahábhárata");

	test_transcript("योगः", "jóga");

	test_transcript("भगवद्गीता", "bhagavadgíta");

	test_transcript("अग्निमीळे", "agnimílé");

	test_transcript("तान्यजत्राँ", "tánjadžatrám");

	test_transcript("श‍ृ शृ", "šr šr"); /* the zero width joiner */
}
END_TEST

START_TEST(test_transcript_detect_hindi)
{
	char *czech;
	int ret;

	ret = transcript_devanagari_to_czech("लड़की", &czech);
	ck_assert_int_eq(EHINDI, ret);
}
END_TEST

void register_transcript_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_transcript_devanagari_to_czech);
	tcase_add_test(test_case, test_transcript_detect_hindi);
}
