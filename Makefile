.PHONY: all clean

all:
	$(CC) main.c -o main
	./main

clean:
	$(RM) -f main
