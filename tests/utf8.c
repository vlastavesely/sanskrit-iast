#include "test.h"
#include "utf8.h"
#include "../utf8.h"

START_TEST(test_utf8)
{
	char buf[10] = {};
	unsigned int c;

	c = utf8_unpack_char("\u0024");
	utf8_pack_char(buf, c);
	ck_assert_int_eq(1, utf8_char_length(c));
	ck_assert_str_eq("\u0024", buf);

	c = utf8_unpack_char("\u00a2");
	utf8_pack_char(buf, c);
	ck_assert_int_eq(2, utf8_char_length(c));
	ck_assert_str_eq("\u00a2", buf);

	c = utf8_unpack_char("\u0939");
	utf8_pack_char(buf, c);
	ck_assert_int_eq(3, utf8_char_length(c));
	ck_assert_str_eq("\u0939", buf);

	c = utf8_unpack_char("\U00010348");
	utf8_pack_char(buf, c);
	ck_assert_int_eq(4, utf8_char_length(c));
	ck_assert_str_eq("\U00010348", buf);
}
END_TEST

void register_utf8_tests(TCase *test_case)
{
	tcase_add_test(test_case, test_utf8);
}
