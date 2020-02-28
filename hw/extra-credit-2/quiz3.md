# Quiz 3:

## Method 1

Even if I did not know the key string, I could still run the binary with the memory dump as input so it would output the original message, because $(input \oplus keystring) \oplus keystring = input$

## Method 2

I could also try to encrypt a long sequence of the same character like `aaaaaaaaaaaaaaaaaaaaaaaaaaaa` and look at the pattern in the output string to guess the password.