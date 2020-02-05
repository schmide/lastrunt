# lastrunt
schmide's analysis runt
# problem
 f(a,b) = 333.75 * b ^ 6 + a ^ 2 * (11 * a ^ 2 * b ^ 2 - b ^ 6 - 121 * b ^ 4 - 2) + 5.5 * b ^ 8 + (a / (2 * b))

 f( 77617.0, 33096.0) = -8.273960599468213050755593940266408026e-01

 In the above equation everything cancels
 out except for 2 small values. If one were
 to expand it, 6 distinct values are produced.
 Orders 2 x 36, 3 x 29, 10, and 0. 5 of them
 reduce down to - 2. The remaining (a / (2 * b))
 provides the fractional element of the answer.

 Why a long double will not work.

 Numbers of the order 10^36 require in this case
 ~2^122 bits. A long double provides 2^113 which
 only provides orders into the 10^34 range. So 
 ~9 bits underflow.

# Solution

 Make a chain of floats that shifts data
 to preserve precision. Do operations in
 long form.

 Example shown in "split numbers"

 Without an operator function this would
 be very tedious. So c is the wrong tool
 for the job.

# Previous testing

 After trying some shifts and adjustments
 I found that simply ordering the operations 
 helps avoid some pitfalls and provides better 
 precision.

 Optimal order would probably be magnitude 
 greatest to least with alternating signs

 After testing it looks like almost acceptable 
 cases are

 sorted either direction
 sorted inside out

 worst case 
 
 sorted outside in

 None of this removes the computational pitfalls
