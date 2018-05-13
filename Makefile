.PHONY: main test clean

OBJS = syllable.o utf8.o transliteration.o iast.o iast-czech.o


iast: main.o $(OBJS)
	$(CC) $^ -o $@

test: iast
	sh tests/test.sh

%.o: %.c
	$(CC) -MMD -MP -c $< -o $@

clean:
	$(RM) iast *.o *.d
