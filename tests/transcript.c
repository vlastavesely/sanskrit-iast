#include "test.h"
#include "transcript.h"
#include "../transcription.h"

START_TEST(test_transcript_devanagari_to_czech)
{
	char *czech;

	czech = transcript_devanagari_to_czech("तन्त्रशास्त्रम्");
	ck_assert_str_eq("tantrašástra", czech);
	free(czech);

	czech = transcript_devanagari_to_czech("सांख्य");
	ck_assert_str_eq("sánkhja", czech);
	free(czech);

	czech = transcript_devanagari_to_czech("महाभारतम्");
	ck_assert_str_eq("mahábhárata", czech);
	free(czech);

	czech = transcript_devanagari_to_czech("योगः");
	ck_assert_str_eq("jóga", czech);
	free(czech);

	czech = transcript_devanagari_to_czech("भगवद्गीता");
	ck_assert_str_eq("bhagavadgíta", czech);
	free(czech);
}
END_TEST

void register_transcript_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_transcript_devanagari_to_czech);
}
