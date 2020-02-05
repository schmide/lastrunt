// lastrunt.cpp  (c compliant)
// 
// after trying some shifts and adjustments
// I found that simply ordering the operations 
// to avoid some pitfalls and provides better 
// precision.
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

#define FLOAT_TYPE double
#define POW_FUN pow


int compare_long_double(const void * a, const void * b)
{
   FLOAT_TYPE diff = *(FLOAT_TYPE*)a - *(FLOAT_TYPE*)b;
   return diff == 0.0 ? 0 : diff < 0.0 ? -1 : 1;
}

FLOAT_TYPE ordered_fp(FLOAT_TYPE a, FLOAT_TYPE b)
{
#define NUM_NUMBERS 7   
   FLOAT_TYPE numbers[NUM_NUMBERS] = {
      333.75 * pow(b, 6.0),
      11.0 * pow(a, 4.0) * pow(b, 2.0),
      -pow(a, 2.0) * pow(b, 6.0),
      -121.0 * pow(b, 4.0) * pow(a, 2.0),
      -2.0 * pow(a, 2.0),
      5.5 * pow(b, 8.0),
      a / (2.0 * b)
   };

   printf("  (answer)\n  -2.0 + numbers[6] = %-+42.36Le\n\n", -2.0 + numbers[6]);

   printf("  base numbers\n\n");
   int i = 0;
   do {
      printf("  numbers[%i] = %-+42.36Le\n", i, numbers[i]);
   } while (++i < NUM_NUMBERS);

   printf("\n\n  original order\n\n");
   FLOAT_TYPE output = 0.0;
   i = 0;
   do {
      output += numbers[i];
      printf("  output = %-+42.36Le\n", output);
   } while (++i < NUM_NUMBERS);

   printf("\n\n  reverse order\n\n");
   output = 0.0;
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
   output = 0.0;
   i = 0;
   do {
      output += numbers[i];
      printf("  output = %-+42.36Le\n", output);
   } while (++i < NUM_NUMBERS);

   printf("\n\n  sorted reverse order\n\n");
   output = 0.0;
   i = 0;
   do {
      output += numbers[NUM_NUMBERS - 1 - i];
      printf("  output = %-+42.36Le\n", output);
   } while (++i < NUM_NUMBERS);

   printf("\n\n  inside out\n\n");
   output = 0.0;
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
   output = 0.0;
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

FLOAT_TYPE shift_fp(FLOAT_TYPE a, FLOAT_TYPE b)
{
#define NUM_NUMBERS 7   
   FLOAT_TYPE numbers[NUM_NUMBERS] = {
      333.75 * pow(b, 6.0),
      11.0 * pow(a, 4.0) * pow(b, 2.0),
      -pow(a, 2.0) * pow(b, 6.0),
      -121.0 * pow(b, 4.0) * pow(a, 2.0),
      -2.0 * pow(a, 2.0),
      5.5 * pow(b, 8.0),
      a / (2.0 * b)
   };

   printf("  (answer)\n  -2.0 + numbers[6] = %-+42.36Le\n\n", -2.0 + numbers[6]);

   printf("  base numbers\n\n");
   int i = 0;
   do {
      printf("  numbers[%i] = %-+42.36Le\n", i, numbers[i]);
   } while (++i < NUM_NUMBERS);

   printf("\n\n  split numbers\n\n");
   FLOAT_TYPE high[NUM_NUMBERS];
   FLOAT_TYPE low[NUM_NUMBERS];
   FLOAT_TYPE lowlow[NUM_NUMBERS];
   i = 0;
   do {
      FLOAT_TYPE temp;
      high[i] = numbers[i];
      ((unsigned long *)&high[i])[0] = 0;
      temp = low[i] = numbers[i] - high[i];
      ((unsigned long *)&low[i])[0] = 0;
      lowlow[i] = temp - low[i];
      printf("  high[%i] = %-+42.36Le\n", i, high[i]);
      printf("  low[%i] = %-+42.36Le\n", i, low[i]);
      printf("  lowlow[%i] = %-+42.36Le\n", i, lowlow[i]);
      printf("  check[%i] = %-+42.36Le\n", i, high[i] + low[i] + lowlow[i]);
   } while (++i < NUM_NUMBERS);

   FLOAT_TYPE shifts[NUM_NUMBERS];
   FLOAT_TYPE max_shift = 0;
   printf("\n\n  power 2 shifts\n\n");
   i = 0;
   do {
      FLOAT_TYPE power_2 = logl(fabsl(numbers[i])) / logl(2.0);
      shifts[i] = pow(2.0, (FLOAT_TYPE)(unsigned long)power_2);
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
   FLOAT_TYPE output = 0.0;
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
