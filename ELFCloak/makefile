CC      = gcc
CFLAGS  = -Wall -O3 -w -g
BINDIR  = /usr/local/bin

all: elfcloak target dec_and_run

elfcloak: elfcloak.o
	$(CC) $(CFLAGS) -o elfcloak elfcloak.o -L ../

target: target.o
	$(CC) $(CFLAGS) -no-pie -o target target.o -L ../

dec_and_run: dec_and_run.o
	$(CC) $(CFLAGS) -o dec_and_run dec_and_run.o -L ../

elfcloak_tests: elfcloak_tests
	gcc $(CFLAGS) elfcloak_tests.c elfcloak.c -L ../

clean:
	rm -f elfcloak elfcloakobf elfcloakobfdec elfcloakobfdecobf elfcloakobfdecobfdec target targetobf targetobfdec targetobfdecobf targetobfdecobfdec dec_and_run *.o

