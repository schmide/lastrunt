# lastrunt
schmide's shifted version of runt
# problem
 f = 333.75 * b ^ 6
 
 + a ^ 2 * (11 * a ^ 2 * b ^ 2 - b ^ 6 - 121 * b ^ 4 - 2) 
 
 + 5.5 * b ^ 8 
 
 + (a / (2 * b))

# solution
 make a chain of floats that shifts data
 to preserve precision. do operations in
 long form.

 split example shown in "split numbers"

 without an operator function this would
 be very tedious. c isn't going to happen
 as it would be a dead end.

# previous testing

 after trying some shifts and adjustments
 I found that simply ordering the operations 
 helps avoid some pitfalls and provides better 
 precision.

# guess
 optimal order would probably be magnitude 
 greatest to least with alternating signs

# after testing 
 it looks like almost acceptable cases are

 sorted either direction
 
 sorted inside out

 worst case 
 
 sorted outside in

# conclusion
 none of this removes the computational pitfalls

 will not be solved by using bigger floats
