#include "test.h"
#include "translit.h"

static Suite *create_test_suite()
{
	Suite *suite;
	TCase *test_case;

	suite = suite_create(NULL);
	test_case = tcase_create(NULL);

	register_translit_tests(test_case);
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

	srunner_run_all(runner, CK_NORMAL);
	retval = srunner_ntests_failed(runner);
	srunner_free(runner);

	return retval;
}
