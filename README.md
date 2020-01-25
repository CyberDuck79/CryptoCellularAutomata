# CryptoCellularAutomata
Elementary Cellular Automata Cryptography

## algo version 2

### analyse documentation
rule radius r -> selon les voisins distant de <= r  
appliquer une regle differente par bit ?  
90 / 105 / 150 / 165 -> Tomassini and Perrenoud rules.  
86 / 90 / 101 / 105 / 150 / 153 / 165 -> Franciszek Seredynskia, Pascal Bouvryc and Albert Y. Zomaya.  

### description
Model with 8 rules 30 / 86 / 90 / 101 / 105 / 150 / 153 / 165  
Use the hash SHA-256 of a passphrase as generation seed (start state of the CA).  
Generate a 256 rules sequence applying a 3 bits mask (0-7) through the start state.  
Change the sequence at each new state generation depending on the previous state.  