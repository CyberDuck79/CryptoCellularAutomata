# CryptoCellularAutomata
Cellular Automata Cryptography

## Description
Cryptographic algorithm using a 1D cellular automata with radius 4.\
The algorithm is inspired by "Random Sequence Generation by Cellular Automata" by Stephen Wolfram.\
It use a cellular automata to generate a pseudo random number generator and apply a Vernam cipher in cipher feedback mode.\
The algorithm is therefore symmetrical.\
To esure a good pseudo random generation the algorithm use hybrid CA, which means that 5 rules are alternately used to generate the CA states.\
These rules are taken for "Four Neighbourhood Cellular Automata asBetter Cryptographic Primitives" by Jimmy Jose and Dipanwita Roy Chowdhury.

## Usage
Compilation uses GCC by default. If necessary, you can modify the Makefile to work with another compiler. You must be able to run "make". In this case, download and unzip the project, and type "make" in the terminal while in the same directory as the Makefile.

arguments : -e(encrypt)/-d(decrypt) "passphrase" file nonce\
The encryption process create an encrypted .ca file, this is the file to use for decryption, with the same passphrase and nonce.\
To ensure the security of the key (passphrase) use a different nonce on each encryption, the nonce can be send on insecure channel.

## TODO
Adapt to network communication.
