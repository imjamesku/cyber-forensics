# LLVM

## 1. How my LLVM pass works
Since the goal is to print all the decoded strings, I figured the simplest way would be to insert a function call that takes a string as input and prints the string.
![](https://i.imgur.com/MOYDJuH.png)
Using the function name to identify each `decode` call, I insert a function that prints the return value of decode. The print function is as shown below
![](https://i.imgur.com/dCTKs3x.png)
I implemented and linked my print function by using the following commands:

- `clang++ -emit-llvm -o print.bc -c print.c`
- `llvm-link print.bc sample2.bc -S -o=sample2p.bc`

Having hooked the `print` function to the sample, I was able to run `MyLLVMPass` on the resulting bitcode.
The result from sample1 is:
![](https://i.imgur.com/ueV8Bzt.png)
The result from sample2 is:
![](https://i.imgur.com/8ztUa7D.png)

## 2. How did I identify data dependency between the argument and return values of the decode function

## 3. how did you identify range of the index (around 0.25 page)
In `runOnModule`, I loop through each function and identify`decode` by and call `AnalyzeDecodeFunction`, which runs through each instruction in the `decode` function, allowing me to perform operations on the instruction.

![](https://i.imgur.com/lRc3D0m.png)

How I find the range of the for loop shown in the picture above.

To find out what the range of the for loop is, I use `isa<CmpInst>` to check if the instruction is a `cmp`. If it is, I then check if it's an `SLT` . If it is, I extract the second operand, the range of the for loop, using `getOperand`.

## 4. how did you identify the decoding computation

## 5. how did you handle variable aliasing

## 6. how did you stitch all the above logic together?
I did not rewrite the logic. I simply take advantage of the decode function and insert a print to print the result string of decode.


