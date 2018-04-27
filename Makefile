.PHONY: main test clean

OBJS = syllable.o utf8.o transliteration.o iast.o


main: main.o $(OBJS)
	$(CC) $^ -o $@

test: main
	sh test.sh

%.o: %.c
	$(CC) -MMD -MP -c $< -o $@

clean:
	$(RM) main test *.o *.d
