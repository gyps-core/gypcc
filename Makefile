CFLAGS =-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)


main: $(OBJS)
	$(CC) -o main $(OBJS) $(LDFLAGS)

$(OBJS): gypcc.h

test: main
	sh ./test.sh

clean:
	rm -rf main *.o *~ tmp*

.PHONY: test clean
