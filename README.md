# CryptoCellularAutomata
Elementary Cellular Automata Cryptography

## Description
Model with 1D cellular automata, use the 8 radius=1 rules 30 / 86 / 90 / 101 / 105 / 150 / 153 / 165.  
Use the hash SHA-256 of a passphrase as generation seed (start state of the CA).  
Generate a 1024 rules sequence applying a 3 bits mask (0-7) through the start state.  
Change the sequence at each new state generation depending on the previous state.  
Use the generated states as XOR mask (disposable mask).  

## Usage
compilation : simple make command  
arguments : -e(encrypt)/-d(decrypt) "passphrase" "file"  
The encryption process create an encrypted .ca file, this is the file to use for decryption, with the same passphrase.  
