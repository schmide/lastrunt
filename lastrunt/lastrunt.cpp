// lastrunt.cpp  (c compliant)
//
// schmide's shifted version
//
// order of operations 
// separate out each calculation
// find the number with the highest base
// shift numbers to that base
// add and shift back
//
// a more useful version could work with 
// 2 numbers at a time
// 
// overflow on doubles will happen around 
// 2^2^14 = 1.1897314e+4932‬
//
// further optimizations on precision would
// involve sorting, alternating sign, 
// pairing, variable shifts, etc

#include <iostream>

// f = 333.75 * b ^ 6 + a ^ 2 * (11 * a ^ 2 * b ^ 2 - b ^ 6 - 121 * b ^ 4 - 2) + 5.5 * b ^ 8 + (a / (2 * b))

long double shift_fp(long double a, long double b)
{
#define NUM_NUMBERS 7   
   long double numbers[NUM_NUMBERS] = {
      333.75L * powl(b, 6.0L),
      11.0L * powl(a, 2.0L) * powl(a, 2.0L) * powl(b, 2.0L),
      -powl(a, 2.0L) * powl(b, 6.0L),
      -121.0L * powl(b, 4.0L) * powl(a, 2.0L),
      -2.0L * powl(a, 2.0L),
      5.5L * powl(b, 8.0L),
      a / (2.0 * b) // everything cancels out but this.
   };

   printf("base numbers\n\n");
   int i = 0;
   do {
      printf("numbers[%i] = %-+42.36Le\n", i, numbers[i]);
   } while (++i < NUM_NUMBERS);

   long double shifts[NUM_NUMBERS];
   long double max_shift = 0;
   printf("\n\npower 2 shifts\n\n");
   i = 0;
   do {
      shifts[i] = powl(2.0L, (long double)(unsigned long)(logl(fabsl(numbers[i])) / logl(2.0L)));
      printf("shifts[%i] = %-+42.36Le\n", i, shifts[i]);
      max_shift = fmaxl(max_shift, shifts[i]);
   } while (++i < NUM_NUMBERS);
   printf("\nmax shift = %-+42.36Le\n", max_shift);

   printf("\n\nadjusted shifts\n\n");
   i = 0;
   do {
      shifts[i] /= max_shift;
      printf("shifts[%i] = %-+42.36Le\n", i, shifts[i]);
   } while (++i < NUM_NUMBERS);

   printf("\n\nOutput\n\n");
   long double output = 0.0L;
   i = 0;
   do {
      output = (output * shifts[i] + numbers[i] * shifts[i]) / shifts[i];
      printf("output = %-+42.36Le\n", output);
   } while (++i < NUM_NUMBERS);
   return output;
}

int main()
{
   // -0.827396059946821368141165095479816291999033115785
   // +1.172603940053178694924440605973359197e+00
   double test = shift_fp(77617.0, 33096.0);
}
