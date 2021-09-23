CFLAGS := -ggdb3 -O0 -Wall -Wextra -std=c11
CFLAGS += -Wmissing-prototypes -Wvla
CPPFLAGS := -D_DEFAULT_SOURCE

PROGS := pingpong primes find xargs

all: $(PROGS)

find: util.o find.o
xargs: util.o xargs.o
primes: util.o primes.o
pingpong: util.o pingpong.o

format: .clang-files .clang-format
	xargs -r clang-format -i <$<

clean:
	rm -f $(PROGS) *.o core vgcore.*

.PHONY: all clean format
