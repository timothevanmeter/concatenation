CC 	= gcc
CFLAGS 	= -Wall -pedantic
RM 	= rm

default: all

all: conc test

conc: concatenate_tables.c
	$(CC) $(CFLAGS) -o conc.o concatenate_tables.c ht_array_DYNAMIC.c -lm

run:
	./conc.o data_test/T*_retina_100.counts

test:
	./conc.o data_test/*_m1.counts

clean:
	$(RM) *.dat
	$(RM) *.o
