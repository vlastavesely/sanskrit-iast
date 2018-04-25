.PHONY: all clean

all:
	$(CC) main.c syllable.c utf8.c transliteration.c -o main
	./main

clean:
	$(RM) -f main
