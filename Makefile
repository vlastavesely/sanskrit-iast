.PHONY: main test install uninstall clean

SRCFILES := $(shell find . -type f -name "*.c")
OBJFILES := $(patsubst %.c, %.o, $(SRCFILES))


iast: main.o $(OBJFILES)
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
