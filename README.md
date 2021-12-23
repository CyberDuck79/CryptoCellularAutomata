# CryptoCellularAutomata
Cellular Automata Cryptography

## Description (From the Original Author, CyberDuck79):
Cryptographic algorithm using a 1D cellular automata with radius 4.\
The algorithm is inspired by "Random Sequence Generation by Cellular Automata" by Stephen Wolfram.\
It use a cellular automata to generate a pseudo random number generator and apply a Vernam cipher in cipher feedback mode.\
The algorithm is therefore symmetrical.\
To esure a good pseudo random generation the algorithm use hybrid CA, which means that 5 rules are alternately used to generate the CA states.\
These rules are taken for "Four Neighbourhood Cellular Automata asBetter Cryptographic Primitives" by Jimmy Jose and Dipanwita Roy Chowdhury.

## README
The program assumes GCC to compile programs by default. You should be able to run GNUMake by typing in 'make' on the terminal when the project's Makefile is in the present working directory. In this case, download and unzip the project, and type "make" in the terminal while in the same directory as the Makefile. When this works, you may notice some new files in the project subdirectories. To clean unnecessary object files from these folders, type "make clean".

A text data file for testing the program in included under the filename "ode". To see its contents, type in 'cat ode' on the terminal. If everything's going well, there will be some John Keats poetry on your screen.

Now, to encrypt the poem we will 
arguments : -e(encrypt)/-d(decrypt) "passphrase" file nonce\
The encryption process create an encrypted .ca file, this is the file to use for decryption, with the same passphrase and nonce.\
To ensure the security of the key (passphrase) use a different nonce on each encryption, the nonce can be send on insecure channel.

## TODO
Adapt to network communication.
