# tiny-morse-forth
A tiny FORTH designed for ATtiny85 using Morse Code for I/O

I'm starting out with ASCII using stdin/stdout on ordinary <stdio.h>.

Eventually I'll remove the stdio dependancy and port it to ATtiny85
and use a Morse encoding instead of ASCII.

### Morse Encoding

(Not used here yet).

Each morse code can be represented by a single byte, containing 0 bits for
dits and 1 bits for dahs, right-adjusted.  The left is padded with zeros
and a single 1 bit.  International Morse Code follows that initial 1.
Morse codes up to 7 dit/dah elements long can be uniquely represented
thus, as well as the backspace code (8 dits) can use the all-zeroes
representation (where the initial 1 gets shifted off the left end).

Example: The letter "C" is `-.-.` (dah dit dah dit), so encode that as
`1010`.  Attach a 1 to the front of that, and pad the left with 0s,
so you get `00011010`.
