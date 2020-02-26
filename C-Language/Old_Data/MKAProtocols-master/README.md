### MSc Thesis Project 

This repository contains the source code of my MSc thesis project: 
implementation of various (4 up to now) Multiparty Key Agreement protocols, 
i.e. generalizations  of the famous Diffie-Hellman key exchange. 
The target platform is an embedded bare metal environment, 
specifically I've developed all the code on a stm32f407 discovery board, 
mounting an arm cortex-m4 processor. The protocol used to communicate 
between the boards is [KNX](http://en.wikipedia.org/wiki/KNX_%28standard%29)

Up to now the physical transmission works through the uart 
peripheral provided by the board.

All the code to set up and manage the board hardware and peripherals 
(all the code related to the board is in the stm32f4d_helper module) 
has been taken or adapted from this nice repository of [dwelch67](https://github.com/dwelch67/stm32f4d)

Up to now I'm not using the standard ST library, therefore the code dealing 
with the board is really low level, but nonetheless very instructive 
since I have to manage a bunch of stuff (leds, uart1 and uart6, interrupts etc).
The repository contains also the makefiles, linker scripts and startup files
that I use; to compile and link everything I use this pre-built [toolchain](https://launchpad.net/gcc-arm-embedded/+download)
that facilitates a lot the creation of a cross-compiler.

All the crytpo primitives needed to implement the aforementioned protocols 
are provided by the [PolarSSL library](https://polarssl.org/api/)

The great thing about this library is that it is designed to run on embedded 
devices and moreover it's written in a very elegant, easy to understand 
and modular fashion; indeed I've incorporated in my project only the modules
that are strictly needed in order to limit code size.

Algorithms implemented so far:

- BD1(M. Burmester, Y. Desmedt, A Secure and Efficient Conference Key Distribution System)
- BD2 (M. Burmester, Y. Desmedt, Efficient and Secure Conference Key Distribution)
- MKA-abp (A. Antonini, A. Barenghi, G. Pelosi, Security Analysis of Building Automation Networks
Threat Model and Viable Mitigation Techniques)
- GDH2 (M. Steiner et al., Diffie-Hellman Key Distribution Extended to Group Communication)
