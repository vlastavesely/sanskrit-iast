SHELL        = /bin/sh
CC           = gcc

PREFIX       = /usr/local
BINDIR       = $(PREFIX)/bin
MANDIR       = $(PREFIX)/share/man

OBJECTS      = iast.o iast-czech.o transliteration.o transcription.o utf8.o encoder.o
TEST_OBJECTS = tests/main.o tests/translit.o tests/transcript.o tests/encoder.o
CFLAGS       = -Wall
LIBS         =

TEST_CFLAGS  = $(CFLAGS) $(shell pkg-config --cflags check)
TEST_LIBS    = $(shell pkg-config --libs check)


.PHONY: all main test install uninstall clean

all: iast iast.1.gz

include $(wildcard *.d)

iast: main.o $(OBJECTS)
	$(CC) $^ -o $@ $(CFLAGS)

test: iast tests/test
	tests/test
	sh tests/integration.sh

%.o: %.c
	$(CC) -MMD -MP -c $< -o $@ $(CFLAGS)

tests/%.o: tests/%.c
	$(CC) -MMD -MP -c $< -o $@ $(TEST_CFLAGS) $(TEST_LIBS)

tests/test: $(OBJECTS) $(TEST_OBJECTS)
	$(CC) $^ -o $@ $(TEST_CFLAGS) $(TEST_LIBS)

%.1.gz: %.1
	cat $< | gzip -f >$@

install:
	install -m 0755 iast $(BINDIR)
	install -m 644 iast.1.gz $(MANDIR)/man1

uninstall:
	rm -f $(BINDIR)/iast
	rm -f $(MANDIR)/man1/iast.1.gz

clean:
	$(RM) iast tests/test *.o */*.o *.d */*.d *.1.gz
