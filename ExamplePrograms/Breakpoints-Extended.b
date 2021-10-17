/*

+++ +++
[>++++ ++++<-]
>+.[-]<[-]

*/
/* If comments do not work, will print out a '1' */
/* Does scratch memory work? */
// If scratch memory works, cell 2 will become 2. Then, adding 48 to it will print 2 instead of 0.
+++ +++ >
[-] 
{
  ++ > +<	
}
<[>++++ ++++<-]>.
[-] < [-]

/* expected output:
	2
   expected output if scratch memory does not work:
   	0
*/

/* Does return-to-0 operator work? */
/* Expected output:
	1
   Expected output if operator does not work:
   	0
*/

+ > > ++++ ++++ [<++++++<++++++>>-] <^.

/* Call Breakpoints-Extended.include.b */
[-]
@Breakpoints-Extended.include.b@
