# Privacy-Preserving-systems
Privacy-Preserving and Trustworthy Cyber-Systems using encryption

Alice and Bob c files are present.
You must install VMs, ZeroMQ and Libtomcrypt to complete this and the files should be kept in same folder as them
Specifically, consider the toy stream cipher built from a Pseudo Random Number Generator (PRNG) In this exercise, SHA-256 from Libtomcrypt will play the role of deterministic PRNG function. Moreover to avoid repeating keys and information leakage, a PRNG must have a “state”. In this case, a counter i=1,…n will be incorporated in as H(seed||i) to ensure a stateful PRNG is implemented. 
a) Setup: Create two terminals which we will refer to as “Alice” and “Bob” as the sender 
and receiver, respectively. These two terminals will communicate via ZeroMQ as 
discussed in the demo video and in-class demo. Place all necessary files such as 
sharedSecret.txt in their folder. 
b) Encryption at Alice and Transmission to the Bob
1) Alice takes a message as a char array as input from a file called HW1PlaintextTest.txt.(any text file)
Bob identifies its length as n-byte, which must be multiple of 32 byte (compatible with the output bit-length of SHA-256).
2) Alice reads a shared secret (32 chars), from a file named sharedSecret.txt(another text file) to a char buffer “seed”.
3) Alice computes h H(contents of HW1PlaintextTest.txt). Print the hash in the terminal.
4) Alice creates a char array called “key” by using H as PRNG. Specifically, Alice uses H(seed||i), where “||” denotes concatenation. Given n-byte message, for i=1,…,n, Alice  extract 256-bit (32 byte) chucks of random numbers and place them into char array “key”  accordingly with the proper length. 
5) Alice XORs each 256-bit (32-byte) chuck of random output formatted as char elements in 
“key” with “Plaintext” to obtain char array “Ciphertext”, which can be shown in 
pseudocode as Ciphertext[i] Plaintext[i] XOR key[i] for i=1,..,n.
6) Alice transmits the “Ciphertext” char array and hash output h to the Bob via ZeroMQ 
c) Decryption and correctness check at Bob:
7) Bob reads the shared secret as in Step 2 above. 
8) Bob creates his own “key” char array as in step 4 above, and then decrypts the ciphertext 
in a similar way to step 5 but with adjustment, where Plaintext [i] Ciphertext[i] XOR 
key[i] for i=1,..,n. 
9) Bob hashes it as h’ H(contents of recovered XOR output) and compare if h= h’ holds. If 
it does not hold, there is an error, otherwise, Bob writes h’ and “Plaintext” array into a 
file called “Bob_h” and “BobPlaintext”. 

Main findings:
1. Alice reads the plaintext from “HW1PlaintextTest.txt” file.
2. Alice reads the shared key from “sharedSecret.txt” file.
3. Alice hashes the content of “HW1PlaintextTest.txt” file and prints it in the terminal 
(shown in Hex).
4. Alice computes the ciphertext and writes it in ``TheCiphertext.txt" file (write it in Hex
with no spaces between ciphertexts).

1. Create a string consisting of Hash + Ciphertexts, and then send it via ZeroMQ. (The test 
file contains 1 hash and 4 ciphertexts)
2. First run the Bob code (./bob) and then the Alice code (./alice).
Bob part of the code:
1. Bob reads the shared key from “sharedSecret.txt” file.
2. Bob computes the corresponding plaintext of the received ciphertext. Afterwards, writes 
the plaintext in ``BobPlaintext.txt" file.
3. Bob hashes the plaintext and compares it with the received hash. Print ``Hashes 
successfully match" and write the hash in ``Bob_h.txt" file.
