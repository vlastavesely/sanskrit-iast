PREFIX=/usr/local

.PHONY: main test install uninstall clean

OBJECTS      = iast.o iast-czech.o transliteration.o transcription.o utf8.o encoder.o
TEST_OBJECTS = tests/main.o tests/translit.o tests/transcript.o tests/encoder.o
CFLAGS       = -Wall
LIBS         =
TEST_CFLAGS  = $(CFLAGS) $(shell pkg-config --cflags check)
TEST_LIBS    = $(LIBS) $(shell pkg-config --libs check)


all: iast doc/iast.1.gz

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

doc/%.gz: doc/%.adoc
	asciidoctor -d manpage -b manpage $< -o $(<:.adoc=) && gzip -f $(<:.adoc=)

install:
	install -m 0755 -d $(PREFIX)/bin $(PREFIX)/share/man/man1
	install -m 0755 iast $(PREFIX)/bin
	install -m 644 doc/iast.1.gz $(PREFIX)/share/man/man1

uninstall:
	rm -f $(PREFIX)/bin/iast
	rm -f $(PREFIX)/share/man/man1/$(PROGNAME).1.gz

clean:
	$(RM) iast tests/test *.o */*.o *.d */*.d doc/*.gz
