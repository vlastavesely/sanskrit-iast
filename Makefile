PREFIX=/usr

.PHONY: main test install uninstall clean

CFLAGS := -Wall
SRCFILES := $(shell find . -type f -name "*.c")
OBJFILES := $(patsubst %.c, %.o, $(SRCFILES))


iast: main.o $(OBJFILES)
	$(CC) $^ -o $@ $(CFLAGS)

test: iast
	sh tests/test.sh

%.o: %.c
	$(CC) -MMD -MP -c $< -o $@ $(CFLAGS)

install:
	install -m 0755 iast $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/iast

clean:
	$(RM) iast *.o *.d
