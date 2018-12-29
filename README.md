# Cryptography

We will design and analyze cryptographic systems. The systems include symmetric cryptography, asymmetric cryptography, and authentication. 

Symmetric cryptography: Block ciphers and secure hash functions. 
Asymmetric cryptography: Key exchange and public-key systems. 
Authentication and encryption in an adversarial model.

## Setup / Prerequisites

You may find all the programs listed in sub directories fo the 'Programs Directory.' Some directories contain more than one file for the purpose of dependencies. Please be aware of the command snippets in each section. 

If one would like to run a program, I would recommend using CodeRunner for MacOS users. It is a fast and easy tool to test one's code fast. 

If one's copiler yells at them, then note the flages below:  

```
-std=c99 -Wall -Wextra -pedantic -c
```

## 48-Byte Permutation

The purpose of having an encryption permutation sits entirely on sudo-randomizing characters with bit manipulation. A good permutation is one of the key components to have in a cryptographic algorithm. 

```
$ gcc P52.c
$ ./a.out
```

## 48-Byte Permutation - Intel Intrinsics

To optimize the permuation, I rewrote my P52 function to use SSE intrinsics registers. This allowed me to process all four columns of the old P52 permutation at once.   

```
$ gcc intel_P52.c
$ ./a.out
```

Source: https://software.intel.com/sites/landingpage/IntrinsicsGuide/

## Counter Mode Encryption

Counter mode turns a block cipher into a stream cipher. It generates the next keystream block by encrypting successive values of a counter

The blocks fed to P52 during CTR mode processing should be 12 bytes of nonce followed by 36
bytes of a counter. The counter is big-endian incremented between blocks. P52 handles
the little-endian reading of data from memory.

```
$ gcc ctr.c
$ ./a.out
```

## Permutation Hash 

Sponge-based cryptographic hash using P52. The sponge has a capacity 32 bytes, rate 16 bytes, and a specifiable number of output bytes. Hash inputs use 10∗1 padding.

```
$ gcc P52_hash.c P52.c
$ ./a.out
```

II have also included another file in this folder labeled 'collision.c.' The collision takes Rho's method to find a collision in the hash. I was only able to get a size of five. If one would like to try to create a larger pool, feel free to increment the 'SZ' variable.

```
$ gcc collision.c P52_hash.c P52.c
$ ./a.out
```


## Polynomial Hashing

Implement polynomial hashing using Horner’s method and divisionless mod.


```
$ gcc poly61.c
$ ./a.out
```
