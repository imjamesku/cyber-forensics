# Cyber Forensics HW2
###### tags: `class`

## How my pintool works
![](https://i.imgur.com/Ei5n6gm.png)

My pintool first locates the instruction that calls `stega_encrypt` and inserts a callback that overwrrites register values. (the pointer and the size). To get the correct memory content, I also had to fix the pointers in the buffer containing the memory dump file.

## How I injected the contents from memory buffer and find the corret offset that points to the buffer that holds instances of the BUFENC structure
The first thing I thought of was, because the structs are allocated with `malloc`, they should all be located in the heap. To find out which memory dump file was the heap, I went through each file, and found that `2824-7fffbe15c000-7fffbe17d000` contains multiple addresses storing 0xdeadbeef(or EF BE AD DE in little endian), indicating this section was where the heap was.

Having found the heap, I needed to find the correct offset of the first BUFENC struct.
![](https://i.imgur.com/Il9zSr4.jpg)


Notice the pattern in the values at there addresses. Since each `BUFENC` contains 9 pointers, I figured the addresses in the picture were probably where a `BUFENC` struct was located. Having found 1 node, all that was left to do was trace back and find the first node in the linked list, which turned out to be at 7fffbe15eaf0.



## Why the program crashes after you inject the memory and how I fixed it
The reason the program would crash was that there are pointers in the memory dump, and those pointers weren't pointing to the correct addresses because the base address will change every time we allocate a buffer to store the memory content. So I needed to fix the pointers for the injection to work.

![](https://i.imgur.com/GunqOna.png)
The fixing of the pointers is done in the while loop. For each `BUFENC` struct, which has 9 pointers, I fix all the nine pointers and then proceed to fix the next struct on the linked list by going to the address in the last pointer in the current struct.


## what is the secret message and how did you recover?
The secret message is
>Everything's amazing right now, and nobody's happy.

I recovered the message by calling `ptr2str`. I passed the pointer to the head of the linked list as an argument to the function and got back the original secret message. (code in loadMemory.cpp)