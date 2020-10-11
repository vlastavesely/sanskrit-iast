#include "test.h"
#include "transcript.h"
#include "../transcription.h"

static void test_transcript(const char *devanagari, const char *latin)
{
	char *czech = transcript_devanagari_to_czech(devanagari);
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
}
END_TEST

void register_transcript_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_transcript_devanagari_to_czech);
}
