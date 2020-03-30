# Cyber Forensics HW2
###### tags: `class`

## How my pintool works
![(https://i.imgur.com/Ei5n6gm.png)

![image-20200309122818911](/home/james/.config/Typora/typora-user-images/image-20200309122818911.png)

My pintool first locates the instruction that calls `stega_encrypt` and inserts a callback that overwrites register values. (the pointer and the size). To get the correct memory content, I also had to fix the pointers in the buffer containing the memory dump file. Then it looks for the `size_encrypt` function call and changes the argument value to the correct size.

## How I injected the contents from memory buffer and find the corret offset that points to the buffer that holds instances of the BUFENC structure
The first thing I thought of was, because the structs are allocated with `malloc`, they should all be located in the heap. To find out which memory dump file was the heap, I printed out the values in the memory dump files went through each of them, and found that `2824-7fffbe15c000-7fffbe17d000` contains multiple addresses storing 0xdeadbeef(or EF BE AD DE in little endian), indicating this section was where the heap was.

Having found the heap, I needed to find the correct offset of the first BUFENC struct in the heap section.
![](https://i.imgur.com/Il9zSr4.jpg)


Notice the pattern in the values at there addresses. Since each `BUFENC` contains 9 pointers, I figured the addresses in the picture were probably where a `BUFENC` struct was located. Having found 1 node, all that was left to do was trace back and find the first node in the linked list, which turned out to be at 7fffbe15eaf0.



## Why the program crashes after you inject the memory and how I fixed it
The reason the program would crash was that there are pointers in the memory dump(in the linked list), and those pointers weren't pointing to the correct addresses because the base address will change every time we allocate a buffer to store the memory content. So I needed to fix the pointers for the injection to work.

![](https://i.imgur.com/GunqOna.png)
The fixing of the pointers is done in the while loop. For each `BUFENC` struct, which has 9 pointers, I fix all the nine pointers and then proceed to fix the next struct on the linked list by going to the address in the last pointer in the current struct.


## what is the secret message and how did you recover?
The secret message is
>Everything's amazing right now, and nobody's happy.

I recovered the message by calling `ptr2str`. I passed the pointer to the head of the linked list as an argument to the function and got back the original secret message. (code in loadMemory.cpp)