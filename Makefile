.PHONY: all clean

all:
	$(CC) main.c syllable.c -o main
	./main

clean:
	$(RM) -f main
