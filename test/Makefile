CFLAGS=-pedantic -Wextra -Wall
LDFLAGS=
BINS=getchildpids_test helloworld_test

all: $(BINS)

getchildpids_test: getchildpids_test.c getchildpids_testcases.c
	gcc -o $@ $(CFLAGS) $(LDFLAGS) $^

helloworld_test: helloworld.c
	gcc -o $@ $(CFLAGS) $(LDFLAGS) $^

clean:
	rm -f *.o *.d


mrproper: clean
	rm -f $(BINS)
