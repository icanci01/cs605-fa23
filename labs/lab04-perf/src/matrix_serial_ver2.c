/********************************************************
 * matrix_serial.c
 *
 * A simple matrix multiplication program
 * (Matrix_A  X  Matrix_B) => Matrix_C
 * 
 * Compile Using:
 * gcc -Werror -Wall matrix_serial_ver?.c -o matrix_serial_ver?.out
 * or for Bebugging use
 * gcc -DDEBUG -Werror -Wall matrix_serial.c -o matrix_serial_dbg_ver?.out
 * 
 * Performance Evaluation: 
 * perf stat -e cycles -e instructions -e cache-references -e cache-misses ./matrix_serial_ver?.out
 */

#include <stdio.h>
#include "support.h"
#ifdef DEBUG
	#define ARRAY_SIZE 10
#else
	#define ARRAY_SIZE 1000
#endif
typedef int matrix_t[ARRAY_SIZE][ARRAY_SIZE];

matrix_t MA,MB,MC;
/* Print and Matrix in an appropriate form. */
void printMatrix(int size, matrix_t M){
 int row, column;
 for(row = 0; row < size; row ++) {
    for (column = 0; column < size; column++) {
      printf("%5d ",M[row][column]);
    }
    printf("\n");
  }
 }
  
/* Fill in matrices A and B with Values */
void initMatrices(int size,
	  matrix_t MA, 
	  matrix_t MB)
{
  int row, column;
  for (row = 0; row < size; row++) {
    for (column = 0; column < size; column++) {
      MA[row][column] = 1;
    }
  }
  for (row = 0; row < size; row++) {
    for (column = 0; column < size; column++) {
      MB[row][column] = row + column + 1;
    }
  }
  printf("MATRIX A is ready:\n");
  #ifdef DEBUG
	printMatrix(size, MA);
  #endif
  printf("MATRIX B is ready:\n");
  #ifdef DEBUG
	printMatrix(size, MB);
  #endif
}
  
/*
* Routine to multiply a row by a column and place element in 
* resulting matrix.
*/
void mult(int size,
	  int row,
	  int column,
	  matrix_t MA, 
	  matrix_t MB,
	  matrix_t MC)
{
  int position;

  MC[row][column] = 0;
  for(position = 0; position < size; position++) {
    MC[row][column] +=  MA[row][position]  *  MB[position][column] ;
  }
}

/*
 * Main: allocates matrix, assigns values, computes the results
 */
extern int 
main(void)
{
  int   size, row, column, position;
  /* Currently size hardwired to ARRAY_SIZE size */
  size = ARRAY_SIZE;

  /* Initialize the matrices A and B. */
  initMatrices(size, MA, MB);
  startTime();
  /* Process Matrix, by row, column. */
  for(row = 0; row < size; row++) {
    for(position = 0; position < size; position++)
		for (column = 0; column < size; column++) {	  
		   MC[row][column]+= MA[row][position]  *  MB[position][column] ;
	}
  }
  stopTime();
  elapsedTime(); 
  /* Print results */
  #ifdef DEBUG
   printf("MATRIX: The resulting matrix C is;\n");
   printMatrix(size, MC);
  #endif
  return 0;
}