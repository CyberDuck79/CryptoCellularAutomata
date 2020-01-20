# CryptoCellularAutomata
Elementary Cellular Automata Cryptography

## algo version 2

### analyse documentation
rule radius r -> selon les voisins distant de <= r  
appliquer une regle differente par bit ?  
90 / 105 / 150 / 165 -> Tomassini and Perrenoud rules.  
86 / 90 / 101 / 105 / 150 / 153 / 165 -> Franciszek Seredynskia, Pascal Bouvryc and Albert Y. Zomaya.  

### description
Try models with 4 rules : 90 / 105 / 150 / 165  
How to generate a starting CA and a sequence of rules ?  
ask a string key and use the hash as seed CA (and the hash of the hash as sequence of rules seed).  
How to parse these seed ?  
find an algorithm to create an 16 bytes (unsigned long long -> size of encryption block) starting CA  
-> sha256 of a passphrase -> excatly 16 bytes
find an algorithm to create a sequence but what size ? static or dynamic ?
-> read the hash 2 bits by 2 bits -> 128 values 0-3 -> indexes of rules to use
How to use the rules sequence ?  
one by bit (strong and slower) or one by block (faster and weaker).  
How about multiple cellular automatas ?
1 used as mask, X other used as hidden, find an algorithm to switch the mask CA.  

