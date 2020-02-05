/* lastrunt.cpp  
 
 problem
 f(a,b) = 333.75 * b ^ 6
    + a ^ 2 * (11 * a ^ 2 * b ^ 2 - b ^ 6 - 121 * b ^ 4 - 2) 
    + 5.5 * b ^ 8 
    + (a / (2 * b))

 f( 77617.0, 33096.0 ) =
   -8.273960599468213050755593940266408026e-01

 solution
 make a chain of floats that shifts data
 to preserve precision. do operations in
 long form.

 split example shown in "split numbers"

 without an operator function this would
 be very tedious. c isn't going to happen
 as it would be a dead end.

 previous testing

 after trying some shifts and adjustments
 I found that simply ordering the operations 
 helps avoid some pitfalls and provides better 
 precision.

 guess
 optimal order would probably be magnitude 
 greatest to least with alternating signs

 after testing it looks like almost acceptable 
 cases are

 sorted either direction
 sorted inside out

 worst case 
 
 sorted outside in

 none of this removes the computational pitfalls

 will not be solved by using bigger floats
*/

#include <iostream>

#define FLOAT_TYPE double
#define POW_FUN pow

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
      bool negative = numbers[i] < 0.0;
      temp = high[i] = negative ? -numbers[i] : numbers[i];
      ((unsigned long *)&high[i])[0] = 0;
      temp = low[i] = temp - high[i];
      ((unsigned long *)&low[i])[0] = 0;
      lowlow[i] = temp - low[i];
      temp = negative ? -1.0 : 1.0;
      printf("  high[%i] = %-+42.36Le\n", i, temp * high[i]);
      printf("  low[%i] = %-+42.36Le\n", i, temp * low[i]);
      printf("  lowlow[%i] = %-+42.36Le\n", i, temp * lowlow[i]);
      printf("  check[%i] = %-+42.36Le\n", i, temp * (high[i] + low[i] + lowlow[i]));
   } while (++i < NUM_NUMBERS);

   FLOAT_TYPE shifts[NUM_NUMBERS];
   FLOAT_TYPE max_shift = 0;
   printf("\n\n  power 2 shifts\n\n");
   i = 0;
   do {
      FLOAT_TYPE power_2 = log(fabsl(numbers[i])) / log(2.0);
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
      output += numbers[i];
      printf("  output = %-+42.36Le\n", output);
   } while (++i < NUM_NUMBERS);
   output = -2.0 + numbers[6];
   printf("\n  (answer)\n  -2.0 + numbers[6] = %-+42.36Le\n\n", output);

   return output;
}

int compare_float_type(const void * a, const void * b)
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

   qsort(numbers, NUM_NUMBERS, sizeof(numbers[0]), compare_float_type);

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
   output = -2.0 + numbers[3];
   printf("\n  (answer)\n  -2.0 + numbers[3] = %-+42.36Le\n\n", output);

   return output;
}
int main()
{
   // -0.827396059946821368141165095479816291999033115785
   // +1.172603940053178694924440605973359197e+00
   printf("\n\n  --shift_fp--\n\n");
   FLOAT_TYPE test = shift_fp(77617.0, 33096.0);
   printf("\n\n  --ordered_fp--\n\n");
   test = ordered_fp(77617.0, 33096.0);
}
