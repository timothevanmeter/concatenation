#include <stdlib.h>
#include <stdio.h>

int ** create_array(int file_number, int count) {
  int *temp = (int*) malloc(file_number * sizeof(int));
  int **ptr_temp = &temp;
  /* array = (int*) malloc(file_number * sizeof(int)); */
  int i = 0;
  while(i != file_number-1) {
    printf("*");
    temp[i] = 0;
    /* array[i] = 0; */
    i++;
  }
  temp[i] = count;
  /* array[i] = count; */
  printf("#");
  /* printf("\n%d\n", array[0]); */
  /* printf("%d\n", array[1]); */
  /* printf("%d\n", array[2]); */
  /* array = &temp; */
  /* printf("\n%d\n", (*array)[0]); */
  /* printf("%d\n", (*array)[1]); */
  /* printf("%d\n", (*array)[2]); */  
  /* free(temp); */
  return ptr_temp;
}

void printa(int **array) {
  int i = 0;
  while(*array != NULL) {
    printf("%d\n", (*array)[i]);
    (*array)++;
    i++;
  }
}


int main (int argc, char* argv[]) {


  int d = 9877;
  int file_number = 3;
  /* int temp[5] = {1,2,3,4,5}; */
  /* int *test = temp;  */
  /* int **t = &test; */
  int **array = NULL;
  /* int **a = &array; */

  array = create_array(file_number, d);

  /* float max = sizeof(temp) / sizeof(int); */
  /* printf("\n%f\n", max); */
  /* max = sizeof(array) / sizeof(array[0]); */
  /* printf("\n%f\n", max); */

  /* printf("\n%d\n", array[0]); */
  
  printf("\n%d\n", (*array)[0]);
  printf("%d\n", (*array)[1]);
  printf("%d\n", (*array)[2]);

  /* printa(a); */
  /* printa(t); */
  free((*array));

  return 0;
}
