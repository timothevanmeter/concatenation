#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ht_array_DYNAMIC.h"

// ##############################################################
// This script expects as input a space-separated file with
// in the first column a key used to identify an entry and
// in the second column an integer value.

// The files listed as input are read and the keys are used to
// populate a hash table. The hash table has any key associated
// with a vector of all the integer values corresponding to that
// key.

// The script resolves the problem of presence / absence of keys
// between different files by including 0 values in the vectors.

// -----------------
// USAGE:

// How to call the executable:
// $ ./conc.o <file_1_to_concatenate> <file_2_to_concatenate> <file_N_to_concatenate> <output_name>

// The makefile has 5 different options
 // By default if make is launch without any options it will do make all (option 5)

// 1) COMPILATION and production of an executable:
// $ make compile

// 2) RUNNING the executable:
//     By default this is done on the files T4_retina_100.counts and T6_retina_100.counts located in the data_test folder
// $ make run

// 3) TESTING the executable:
//     By default this is done on the files _m1.counts and _m1.counts located in the data_test folder
// $ make test

// 4) REMOVING the output file and the executable:
// $ make clean

// 5) COMPILING and RUNNING options 1) + 2)
// $ make all
// -----------------

// ##############################################################
#define KEY_SIZE 40

int TOTAL_FILE_NUMBER = 0;

// -----------------------------------------------------------
// -----------------------------------------------------------

void exit_nomem(void) {
  fprintf(stderr, "out of memory\n");
  exit(1);
}
// -------------------------------------------------
// -------------------------------------------------
void print_hash(ht *htable) {
  hti iterator = ht_iterator(htable);
  hti *it = &iterator;
  while(ht_next(it)) {
    fprintf(stdout, "%s,", it->key);
    int i = 0;
    while(i < TOTAL_FILE_NUMBER) {
      fprintf(stdout, "%d,", it->counts[i]);
      i++;
    }
    fprintf(stdout, "\n");
  }
}

// -------------------------------------------------
// -------------------------------------------------
void hash_to_file(ht *htable, char *output) {
  FILE *file;
  hti iterator = ht_iterator(htable);
  hti *it = &iterator;
  file = fopen(output, "w");
  while(ht_next(it)) {
    fprintf(file, "%s,", it->key);
    int i = 0;
    while(i < TOTAL_FILE_NUMBER) {
      fprintf(file, "%d,", it->counts[i]);
      i++;
    }
    fprintf(file, "\n");
  }
  fclose(file);
}
// -------------------------------------------------

// ###########################################################################

int main(int argc, char **argv)
{
  FILE * fp;
  char * output = strcat(argc[argv-1], ".csv");
  char ch;
  char * key = malloc(sizeof(char) * KEY_SIZE);
  char * countC = malloc(sizeof(char) * KEY_SIZE);
  int countI;
  int p = 0;
  int READ = 1;
  int END = 0;
  int n = 0;

  // NEED TO:
  // TESTING IF THE INPUT FILE IS CORRECTLY FORMATTED !

  // NEED TO:
  // TESTING IF AN OUTPUT NAME WAS PROVIDED !
  

  fprintf( stdout, "---------------------------\n");
  TOTAL_FILE_NUMBER = argc - 2;
  printf("FILE NUMBER = %d\n", TOTAL_FILE_NUMBER);
  fprintf( stdout, "---------------------------\n");
  // size_t hash_table_size = 0;
  ht * htable = ht_create();
  if (htable == NULL) {
    exit_nomem();
  }

  // LOOPING OVER ALL THE INPUT FILES CONTAINING THE COUNTS
  for(int f = 1; f < TOTAL_FILE_NUMBER + 1; f++) {
    fp = fopen(argv[f], "r");
    fprintf(stdout, "\n\tProcessing file %s\n", argv[f]);
    fflush(stdout);
    while( 1 ) {
      // Parsing the data file character by character
      ch = fgetc(fp);
      // We reached the end of the file:
      if( feof(fp) ) { END = 1; }
      if(ch == '\n' || ch == '\r' || END == 1) {
	// TERMINATE THE STRING TO AVOID UNDEFINED BEHAVIOUR
	countC[p++] = '\0';
	countI = (int)strtol(countC, (char **)NULL, 10);
	int *temp_count = &countI;
	fflush(stdout);
	// ------------------------------
	if (ht_set(htable, key, temp_count, f) == NULL) {
	  exit_nomem();
	}
	// ------------------------------
	p = 0;
	READ = 1;
	n++;
	// If we reached the end of the file we stop
	if( END != 1) {
	  continue;
	} else {
	  END = 0;
	  break;
	}
      }
      if(ch == ' ') {
	// TERMINATE THE STRING TO AVOID UNDEFINED BEHAVIOUR
	key[p++] = '\0';
	p = 0;
	READ++;
      }
      // READING THE 7MER OR ID COLUMN USED TO MERGE FILES
      if( READ == 1 ) {
	key[p] = ch;
	p++;     
      }
      // READING THE CORRESPONDING HTABLE FOR THAT ID
      if( READ == 2 ) {
	countC[p] = ch;
	p++;
      }
    }
    fclose(fp);
    fprintf(stdout,"\t%d items processed\n\n", n);
    n = 0;
  }

  fprintf( stdout, "---------------------------\n");
  // print_hash(htable);

  hash_to_file(htable, output);
  // CLEANING UP ALL THE MESS,
  //  CLOSING DOORS AND TURNING OFF THE LIGHTS!
  ht_destroy(htable);
  free(countC);
  free(key);
  
  printf("-----------------------------------------------------\n");
  return 0;
}
