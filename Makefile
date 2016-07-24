CC=gcc
CFLAGS=-O3 -Wall -D ELEV_TEST 
target=elev
objects=auxilary.o elevinit.o elevstep.o elevcall.c elevsched.o test.o

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@
$(target): $(objects)
	$(CC) $(CFLAGS) $(objects) -o $@
clean:
	rm -f *.o $(target)
