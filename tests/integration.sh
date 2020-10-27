#!/bin/sh
set -e

usage=$(./iast -h)
test -n $(echo "$usage" | grep "iast [flags and text arguments in any order]")

version=$(./iast -v)
test "$version" = "iast v1.0"

./iast -f tests/texts/bhagavadgita-1.txt |
	./iast -r -f - >/tmp/iast-bhagavadgita-1.txt.out
./iast -f tests/texts/mandukya-upanishad.txt |
	./iast -r -f - >/tmp/iast-mandukya-upanishad.txt.out

expected=$(sha1sum tests/texts/bhagavadgita-1.txt | head -c40)
computed=$(sha1sum /tmp/iast-bhagavadgita-1.txt.out | head -c40)
test "$expected" = "$computed"

expected=$(sha1sum tests/texts/mandukya-upanishad.txt | head -c40)
computed=$(sha1sum /tmp/iast-mandukya-upanishad.txt.out | head -c40)
test "$expected" = "$computed"

printf "\033[32mpassed\033[0m\n"
