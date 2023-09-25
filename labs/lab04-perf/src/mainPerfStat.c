/************************************************

>gcc -O3 main.c
time ./a.out
sum = 400000034998780787062429554585290932224.00
0.376u 0.000s 0:00.37 100.0%    0+0k 16+0io 0pf+0w
>perf stat ./a.out
sum = 400000034998780787062429554585290932224.00

 Performance counter stats for './a.out':

        373.038548 task-clock                #    0.998 CPUs utilized
                39 context-switches          #    0.105 K/sec
                 0 cpu-migrations            #    0.000 K/sec
               129 page-faults               #    0.346 K/sec
     1,108,828,204 cycles                    #    2.972 GHz                     [50.12%]
   <not supported> stalled-cycles-frontend
   <not supported> stalled-cycles-backend
     2,600,740,775 instructions              #    2.35  insns per cycle         [75.10%]
       400,189,661 branches                  # 1072.784 M/sec                   [75.07%]
            11,156 branch-misses             #    0.00% of all branches         [74.87%]

       0.373782171 seconds time elapsed


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
