CFLAGS := -ggdb3 -O2 -Wall -Wextra -std=c11
CFLAGS += -Wmissing-prototypes -Wvla
CPPFLAGS := -D_DEFAULT_SOURCE

PROGS := pingpong primes find xargs
DESAF := ps timeout

all: $(PROGS)
des: $(DESAF)

find: util.o find.o
xargs: util.o xargs.o
primes: util.o primes.o
pingpong: util.o pingpong.o

ps: util.o ps.o
timeout: timeout.o

format: .clang-files .clang-format
	xargs -r clang-format -i <$<

clean:
	rm -f $(PROGS) *.o core vgcore.*

.PHONY: all clean format
