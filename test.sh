#!/bin/sh
set -e

test_word()
{
	transliterated=$(echo -n "$1" | ./main)
	expected="$2"

	if test "x$transliterated" = "x$expected"
	then
		echo "\033[32m$transliterated\033[0m = \033[32m$expected\033[0m"
	else
		echo "\033[31m$transliterated\033[0m != \033[31m$expected\033[0m"
	fi
}

test_word "संस्कृतम्" "saṃskṛtam"
test_word "आर्यावर्त" "āryāvarta"
test_word "तन्त्रशास्त्रम्" "tantraśāstram"
test_word "सांख्य" "sāṃkhya"
test_word "ऋग्वेद" "ṛgveda"
test_word "महाभारतम्" "mahābhāratam"
test_word "सरस्वती नदी" "sarasvatī nadī"
test_word "देवनागरी" "devanāgarī"
test_word "योगः" "yogaḥ"
test_word "भगवद्गीता" "bhagavadgītā"
test_word "सम्भोग" "sambhoga"
