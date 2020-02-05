// lastrunt.cpp  (c compliant)
// 
// after trying some shifts and adjustments
// I found that simply ordering the operations 
// to avoid pitfalls provides the best precision.
//
// guess
// optimal order would probably be magnitude greatest 
// to least with alternating signs
//
// after testing it looks like good cases are
//
// sorted either direction
// sorted inside out
//
// worst case 
// 
// sorted outside in
//
// none of this removes the computational pitfalls



#include <iostream>

// f = 333.75 * b ^ 6 + a ^ 2 * (11 * a ^ 2 * b ^ 2 - b ^ 6 - 121 * b ^ 4 - 2) + 5.5 * b ^ 8 + (a / (2 * b))

int compare_long_double(const void * a, const void * b)
{
   long double diff = *(long double*)a - *(long double*)b;
   return diff == 0.0L ? 0 : diff < 0.0L ? -1 : 1;
}

long double ordered_fp(long double a, long double b)
{
#define NUM_NUMBERS 7   
   long double numbers[NUM_NUMBERS] = {
      333.75L * powl(b, 6.0L),
      11.0L * powl(a, 4.0L) * powl(b, 2.0L),
      -powl(a, 2.0L) * powl(b, 6.0L),
      -121.0L * powl(b, 4.0L) * powl(a, 2.0L),
      -2.0L * powl(a, 2.0L),
      5.5L * powl(b, 8.0L),
      a / (2.0 * b)
   };

   printf("  (answer)\n  -2.0L + numbers[6] = %-+42.36Le\n\n", -2.0L + numbers[6]);

   printf("  base numbers\n\n");
   int i = 0;
   do {
      printf("  numbers[%i] = %-+42.36Le\n", i, numbers[i]);
   } while (++i < NUM_NUMBERS);

   printf("\n\n  original order\n\n");
   long double output = 0.0L;
   i = 0;
   do {
      output += numbers[i];
      printf("  output = %-+42.36Le\n", output);
   } while (++i < NUM_NUMBERS);

   printf("\n\n  reverse order\n\n");
   output = 0.0L;
   i = 0;
   do {
      output += numbers[NUM_NUMBERS - 1 - i];
      printf("  output = %-+42.36Le\n", output);
   } while (++i < NUM_NUMBERS);

   qsort(numbers, NUM_NUMBERS, sizeof(numbers[0]), compare_long_double);

   printf("\n\n  sorted numbers\n\n");
   i = 0;
   do {
      printf("  numbers[%i] = %-+42.36Le\n", i, numbers[i]);
   } while (++i < NUM_NUMBERS);

   printf("\n\n  sorted in order\n\n");
   output = 0.0L;
   i = 0;
   do {
      output += numbers[i];
      printf("  output = %-+42.36Le\n", output);
   } while (++i < NUM_NUMBERS);

   printf("\n\n  sorted reverse order\n\n");
   output = 0.0L;
   i = 0;
   do {
      output += numbers[NUM_NUMBERS-1-i];
      printf("  output = %-+42.36Le\n", output);
   } while (++i < NUM_NUMBERS);

   printf("\n\n  inside out\n\n");
   output = 0.0L;
   i = 0;
   int upper = NUM_NUMBERS >> 1;
   int lower = upper + 1;
   do {
      if (i & 1)
         output += numbers[lower++];
      else
         output += numbers[upper--];
      printf("  output = %-+42.36Le\n", output);
   } while (++i < NUM_NUMBERS);

   printf("\n\n  outside in\n\n");
   output = 0.0L;
   i = 0;
   upper = NUM_NUMBERS - 1;
   lower = 0;
   do {
      if (i & 1)
         output += numbers[lower++];
      else
         output += numbers[upper--];
      printf("  output = %-+42.36Le\n", output);
   } while (++i < NUM_NUMBERS);
   return output;
}

long double shift_fp(long double a, long double b)
{
#define NUM_NUMBERS 7   
   long double numbers[NUM_NUMBERS] = {
      333.75L * powl(b, 6.0L),
      11.0L * powl(a, 4.0L) * powl(b, 2.0L),
      -powl(a, 2.0L) * powl(b, 6.0L),
      -121.0L * powl(b, 4.0L) * powl(a, 2.0L),
      -2.0L * powl(a, 2.0L),
      5.5L * powl(b, 8.0L),
      a / (2.0 * b)
   };

   printf("  (answer)\n  -2.0L + numbers[6] = %-+42.36Le\n\n", -2.0L + numbers[6]);

   printf("  base numbers\n\n");
   int i = 0;
   do {
      printf("  numbers[%i] = %-+42.36Le\n", i, numbers[i]);
   } while (++i < NUM_NUMBERS);

   long double shifts[NUM_NUMBERS];
   long double max_shift = 0;
   printf("\n\n  power 2 shifts\n\n");
   i = 0;
   do {
      long double power_2 = logl(fabsl(numbers[i])) / logl(2.0L);
      shifts[i] = powl(2.0L, (long double)(unsigned long)power_2);
      printf("  shifts[%i] = %-+42.36Le power 2 = %.2f\n", i, shifts[i], power_2);
      max_shift = fmaxl(max_shift, shifts[i]);
   } while (++i < NUM_NUMBERS);
   printf("\n  max shift = %-+42.36Le\n", max_shift);

   printf("\n\n  adjusted shifts\n\n");
   i = 0;
   do {
      shifts[i] /= max_shift;
      printf("  shifts[%i] = %-+42.36Le\n", i, shifts[i]);
   } while (++i < NUM_NUMBERS);

   printf("\n\n  output\n\n");
   long double output = 0.0L;
   i = 0;
   do {
      output = (output * shifts[i] + numbers[i] * shifts[i]) / shifts[i];
      printf("  output = %-+42.36Le\n", output);
   } while (++i < NUM_NUMBERS);
   return output;
}

int main()
{
   // -0.827396059946821368141165095479816291999033115785
   // +1.172603940053178694924440605973359197e+00
   printf("\n\n  --ordered_fp--\n\n");
   double test = ordered_fp(77617.0, 33096.0);
   printf("\n\n  --shift_fp--\n\n");
   test = shift_fp(77617.0, 33096.0);
}
