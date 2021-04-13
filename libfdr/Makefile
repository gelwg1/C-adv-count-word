CXX = gcc



OBJS = fields.c	jrb.c	jval.c

all	:	test	clean

test:	test.o	${OBJS}
	gcc -o test test.o ${OBJS}
test.o:	test.c
	gcc -c test.c
fields.o: ${CPPFLAGS}fields.c
	${CXX} -c fields.c
jrb.o: ${CPPFLAGS}jrb.c
	${CXX} -c jrb.c
jval.o: ${CPPFLAGS}jval.c
	${CXX} -c jval.c

clean:
	rm -f *.o *~
