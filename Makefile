all : print-bits

print-bits : print-bits.o
	${CC} ${CFLAGS} -o $@ $?

clean :
	rm -f *.o *~ print-bits
