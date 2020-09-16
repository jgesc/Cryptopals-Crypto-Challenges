# Cryptopals-Crypto-Challenges
These are my solutions to the [Cryptopals Cryptochallenges](https://cryptopals.com/) written in C.

# Building
Use `make` to build all the challenges.

Use `make bin/setX/challengeY` to build the Set X Challenge Y.

The compiled binaries will be found in the `bin` folder.

# Project structure
The root folder contains the Makefile required to build the challenges. The compiled binaries will be found in the `bin` folder.

The `src` folder contains the source code of the solutions. The subfolder `utils` contains utilities shared among all the challenges. 
The subfolder `challenges` contains challenge specific code.

The `res` folder contains the resource files given by the challenge statements.
