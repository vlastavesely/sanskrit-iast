PREFIX=/usr

.PHONY: main test install uninstall clean

OBJECTS      = iast.o transliteration.o utf8.o encoder.o
TEST_OBJECTS = tests/main.o tests/translit.o
CFLAGS       = -Wall
LIBS         =
TEST_CFLAGS  = $(CFLAGS) $(shell pkg-config --cflags check)
TEST_LIBS    = $(LIBS) $(shell pkg-config --libs check)


all: iast doc/iast.1.gz

iast: main.o $(OBJFILES)
	$(CC) $^ -o $@ $(CFLAGS)

test: tests/test
	tests/test

%.o: %.c
	$(CC) -MMD -MP -c $< -o $@ $(CFLAGS)

tests/%.o: tests/%.c
	$(CC) -MMD -MP -c $< -o $@ $(TEST_CFLAGS) $(TEST_LIBS)

tests/test: $(OBJECTS) $(TEST_OBJECTS)
	$(CC) $^ -o $@ $(TEST_CFLAGS) $(TEST_LIBS)

doc/%.gz: doc/%.adoc
	asciidoctor -d manpage -b manpage $< -o $(<:.adoc=) && gzip -f $(<:.adoc=)

install:
	install -m 0755 iast $(PREFIX)/bin
	install -m 644 doc/iast.1.gz $(PREFIX)/share/man/man1

uninstall:
	rm -f $(PREFIX)/bin/iast
	rm -f $(PREFIX)/share/man/man1/$(PROGNAME).1.gz

clean:
	$(RM) iast *.o *.d doc/*.gz
