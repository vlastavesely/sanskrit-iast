.PHONY: all clean

all:
	$(CC) main.c syllable.c utf8.c transliteration.c iast.c -o main
	./main

clean:
	$(RM) -f main
