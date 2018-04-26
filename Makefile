.PHONY: all test clean

all:
	$(CC) main.c syllable.c utf8.c transliteration.c iast.c -o main
	./main

test:
	$(CC) test.c syllable.c utf8.c transliteration.c iast.c -o test
	./test

clean:
	$(RM) -f main
