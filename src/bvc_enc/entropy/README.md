# Entropy Coding
BVC implements two entropy coders: Adaptive Arithmetic Coding, and Adaptive Huffman Coding. 

These two coders are all extensions of a base type of entropy coder (Arithmetic Coding for ABAC; Huffman Coding for Adaptive Huffman) that enable the symbol dictionaries to be constructed on the fly (ie. The coders aren't required to know the symbol probabilities/frequencies before encoding).

## Adaptive Binary Arithmetic Coding (ABAC)
Adaptive Binary Arithmetic Coding is an extension of standard [Arithmetic Coding](https://en.wikipedia.org/wiki/Arithmetic_coding) that forces the coder into only using two symbols (0 and 1).


## Adaptive Huffman Coding 
Adaptive Huffman Coding is achieved through the [Vitter Algorithm](https://en.wikipedia.org/wiki/Adaptive_Huffman_coding)