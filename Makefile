.PHONY: main test install uninstall clean

OBJS = syllable.o utf8.o transliteration.o iast.o iast-czech.o


iast: main.o $(OBJS)
	$(CC) $^ -o $@

test: iast
	sh tests/test.sh

%.o: %.c
	$(CC) -MMD -MP -c $< -o $@

install:
	install -m 0755 iast /usr/bin

uninstall:
	rm -f /usr/bin/iast

clean:
	$(RM) iast *.o *.d
