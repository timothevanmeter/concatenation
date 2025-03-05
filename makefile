CC 	= gcc
CFLAGS 	= -Wall -pedantic
RM 	= rm

default: all

all: compile run

compile: concatenate_tables.c
	$(CC) $(CFLAGS) -o conc.o concatenate_tables.c ht_array_DYNAMIC.c -lm

run:
	./conc.o data_test/T*_retina_100.counts retina-test-run

test:
	./conc.o data_test/file* file-test-run

clean:
	$(RM) *.csv
	$(RM) *.o
