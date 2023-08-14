# Entropy Coding
BVC implements three entropy coders: ABAC, CABAC, and Adaptive Huffman Coding. 

These three coders are all extensions of a base type of entropy coder (Arithmetic Coding for ABAC and CABAC; Huffman Coding for Adaptive Huffman) that enable the symbol dictionaries to be constructed on the fly (ie. The coders aren't required to know the symbol probabilities/frequencies before encoding).

## Adaptive Binary Arithmetic Coding (ABAC)
Adaptive Binary Arithmetic Coding is an extension of standard [Arithmetic Coding](https://en.wikipedia.org/wiki/Arithmetic_coding) that forces the coder into only using two symbols (0 and 1).

## Context-Adaptive Binary Arithmetic Coding (CABAC)
A potential drawback of ABAC is its inability to adapt symbol probabilities to local changes. CABAC aims to address this problem by using multiple contexts (ie dictionaries).


## Adaptive Huffman Coding 
Adaptive Huffman Coding is achieved through the [Vitter Algorithm](https://en.wikipedia.org/wiki/Adaptive_Huffman_coding)