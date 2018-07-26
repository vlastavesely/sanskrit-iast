PREFIX=/usr

.PHONY: main test install uninstall clean

CFLAGS := -Wall
SRCFILES := $(shell find . -type f -name "*.c")
OBJFILES := $(patsubst %.c, %.o, $(SRCFILES))


all: iast doc/iast.1.gz

iast: main.o $(OBJFILES)
	$(CC) $^ -o $@ $(CFLAGS)

test: iast
	sh tests/test.sh

%.o: %.c
	$(CC) -MMD -MP -c $< -o $@ $(CFLAGS)

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
