# CryptoCellularAutomata
Cellular Automata Cryptography

## Description (From the Original Author, CyberDuck79):
Cryptographic algorithm using a 1D cellular automata with radius 4.\
The algorithm is inspired by "Random Sequence Generation by Cellular Automata" by Stephen Wolfram.\
It use a cellular automata to generate a pseudo random number generator and apply a Vernam cipher in cipher feedback mode.\
The algorithm is therefore symmetrical.\
To esure a good pseudo random generation the algorithm use hybrid CA, which means that 5 rules are alternately used to generate the CA states.\
These rules are taken for "Four Neighbourhood Cellular Automata asBetter Cryptographic Primitives" by Jimmy Jose and Dipanwita Roy Chowdhury.

## README (with example tutorial by Andrew King)
The program assumes GCC to compile programs by default. You should be able to run GNUMake by typing in 'make' on the terminal when the project's Makefile is in the present working directory. In this case, download and unzip the project, and type "make" in the terminal while in the same directory as the Makefile. When this works, you may notice some new files in the project subdirectories. To clean unnecessary object files from these folders, type "make clean".

A text data file for testing the program in included under the filename "ode". To see its contents, type in 'cat ode' on the terminal. If everything's going well, there will be some John Keats poetry on your screen.

Now, to encrypt the poem we will type in:

./ca_crypt -e "password" "ode" nonce

on the terminal. You may use any password you like.
Any integer or ASCII letter should suffice for a nonce.
However, you will need to remember both, to decrypt.

If the poem was encrypted successfully, typing 'cat ode.ca' on the terminal should display a hopelessly unreadable text of random ASCII letters. Fear not! Just decrypt the poem.

./ca_crypt -d "password" "ode.ca" nonce

Typing the above into the terminal with the same password and nonce you used before will run the decryption operation on the ciphertext. If you followed this test without any errors, the program should have deciphered 'ode.ca' and overwritten the ciphertext file with a plaintext file 'ode'.

/*  ***** ***** ***** ***** ***** ***** */
Hope Delaney and Andrew King were not the original authors of the C code; we have contributed notes in some of the source code and added an optional set of tutorial instructions to the README.
/*  ***** ***** ***** ***** ***** ***** */
