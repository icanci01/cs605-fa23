/************************************************
>gcc -O0 main.c
>time ./a.out
sum = 4e+38
1.375u 0.000s 0:01.37 100.0%    0+0k 16+0io 0pf+0w
>gcc -O3 main.c
>time ./a.out
sum = 4e+38
0.375u 0.000s 0:00.37 100.0%    0+0k 16+0io 0pf+0w
==========================================================
if we remove the printf line
gcc -O0 main.c
>time ./a.out
1.343u 0.000s 0:01.34 100.0%    0+0k 16+0io 0pf+0w
>gcc -O3 main.c
>time ./a.out
0.000u 0.000s 0:00.00 0.0%      0+0k 16+0io 0pf+0w

****************************************************************/
#include <stdio.h>
double
powern (double d, unsigned n)
{
  double x = 1.0;
  unsigned j;

  for (j = 0; j < n; j++)
    x *= d;

  return x;
}


int main (void)
{
  double sum = 0.0;
  unsigned i;
  
  for (i = 0; i < 100000000; i++)
    {
      sum += powern (i, i % 5);
    }

  printf ("sum = %.2f\n", sum);
  return 0;
}
