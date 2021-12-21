#include "test.h"
#include "hindi.h"
#include "../compat.h"
#include "../hindi.h"

static void test_transcript(const char *devanagari, const char *latin)
{
	char *hindi;
	int ret;

	ret = transcript_devanagari_to_hindi(devanagari, &hindi);
	ck_assert_int_eq(0, ret);
	ck_assert_str_eq(latin, hindi);
	free(hindi);
}

START_TEST(test_transcript_devanagari_to_hindi)
{
	test_transcript("क़ ख़ ग़ ज़ झ़ ड़ ढ़ फ़", "k kh g z zh d dh f"); /* composite */
	test_transcript("क़ ख़ ग़ ज़ ड़ ढ़ फ़", "k kh g z d dh f"); /* independent */
	test_transcript("कभी ख़ुशी कभी ग़म", "kabhee khushee kabhee gam");
	test_transcript("मैं एक लड़का हूँ और तुम एक लड़की हो", "main ek ladaka hoon aur tum ek ladakee ho");
	test_transcript("स्कॉट्लैण्ड ऑरेंज", "skotlaind orenj"); /* o */
}
END_TEST

void register_transcript_hindi_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_transcript_devanagari_to_hindi);
}
