#include "test.h"
#include "translit.h"
#include "transcript.h"
#include "encoder.h"

static Suite *create_test_suite()
{
	Suite *suite;
	TCase *test_case;

	suite = suite_create(NULL);
	test_case = tcase_create(NULL);

	register_translit_tests(test_case);
	register_transcript_tests(test_case);
	register_encoder_tests(test_case);
	suite_add_tcase(suite, test_case);

	return suite;
}

int main(void)
{
	Suite *suite;
	SRunner *runner;
	int retval;

	suite = create_test_suite();
	runner = srunner_create(suite);

	puts("-----------------------------------------");
	srunner_run_all(runner, CK_NORMAL);
	retval = srunner_ntests_failed(runner);
	puts("-----------------------------------------");

	puts(retval == 0 ? "\033[32mpassed\033[0m\n"
			 : "\033[31mfailed\033[0m\n");

	srunner_free(runner);

	return retval;
}