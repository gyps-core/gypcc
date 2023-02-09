CFLAGS =-std=c11 -g -static

minicc: minicc.c

test: minicc
	sh ./test.sh

clean:
	rm -rf minicc *.o *~ tmp*

.PHONY: test clean
