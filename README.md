# Ransomware

> Disclaimer:  Use this tool at your own risk. Author is not responsible or liable if you damage your own system or others. Follow all local, state, federal, and international laws as it pertains to your geographic location. Do NOT use this tool maliciously as it is being released for educational purposes. This tools intended use is in cyber exercises or demonstrations of adversarial tools.

## Synopsis

Simple C Ransomware I built to learn more about C and Ransomwares.

Source code for the ransomware is in main.c, Testing contains codes for parts of the ransomware, DummyFiles contains files used for testing the encryption.

Since it is using xor encryption currently, run it once to encrypt run it again to decrypt your files.

## Usage

Compile the program and run it with the path you want it to start encrypting as the argument.

`$./main /home/`

## To Do

* Use stronger encryption algorithm
* Add a challenge for victims complete to retrieve their files
* Add a C&C Server