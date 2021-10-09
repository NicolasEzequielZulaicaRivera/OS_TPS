CFLAGS := -ggdb3 -O2 -Wall -Wextra -std=gnu11
CFLAGS += -Wmissing-prototypes

EXEC := test
SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o)

all: $(EXEC)

run: $(EXEC)
	./$(EXEC)

valgrind: $(EXEC)
	valgrind --leak-check=full --track-fds=yes --show-leak-kinds=all ./$(EXEC)

$(EXEC): $(OBJS)

format: .clang-files .clang-format
	xargs -r clang-format -i <$<

clean:
	rm -rf $(EXEC) *.o core vgcore.*

.PHONY: all clean format run valgrind