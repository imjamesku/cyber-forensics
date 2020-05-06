# Additional Assignment2


## (1) a high-level description of what you did
Right out of the gate, in EDB, I found there was a function call:
![](https://i.imgur.com/avJGVmC.png)
around the end of the program. Looking at it in a debugger, I figured it was probably a `strcmp`. So in order to make the return value 0(meaning the input string matches the correct string), I tried to make it compare the correct string to itself. But for some reason, it does not return 0 even though I tried both `*rdi = *rsi` and `*rsi = *rdi` to make the two arguments the same.

So my first attempt to force the program into the success branch failed.



So I ran the program with a debugger again and found that the function `shift_int_to_char` was called 3 times in the program, each writing 4 characters into a buffer of size 15.
![](https://i.imgur.com/OmM91KP.png)
Upon further inspection, I found that the function `shift_int_to_char` converts a 4 byte number into 4 characters. So I reimplemented the function as shown below:
![](https://i.imgur.com/1sS61Cy.png).

To get the correct input string, what I needed to do next was locate the calls to `shift_int_to_char` and get the first argument value, and feed it into my implementation to get the result string.

In order to locate the function calls, I found the address of `shift_int_to_char` with the instrumentation below:
![](https://i.imgur.com/segXD3y.png)

Having found the address(0x400c57), now I could insert some calls(setReg) before the function runs:
![](https://i.imgur.com/Sf1Ie5r.png)

In `setReg`, I run my own `shift_int_to_char` to convert the number to chars and save it in a buffer. Since the last characters are set to 3Q in both crackme1 and crackme2, I set the last chars to 3Q here. I also print the buffer whenever this function is called:
![](https://i.imgur.com/NpGJfyh.png)


Now the program will literally tell me the answer itself!
![](https://i.imgur.com/Bm3wqPC.png)
