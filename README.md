# aes256
An aes256 implementation in C that I wrote following along with the [NIST FIPS 197 document](https://nvlpubs.nist.gov/nistpubs/fips/nist.fips.197.pdf) for some C practice and to understand AES256.

# build

gcc *.c -w -g -lm -o aes256

# usage

`./aes256 -ecb [Message to encrypt] [256 bit key in 02x hex]` Encrypt data using AES256-ECB

`./aes256 -cbc [Message to encrypt] [256 bit key in 02x hex] [128 bit iv in 02x hex]` Encrypt data using AES256-CBC

`./aes256 -ctr [Message to encrypt] [256 bit key in 02x hex] [128 bit iv in 02x hex]` Encrypt data using AES256-CTR

# note
If the data and key from the command line didn't tip you off, don't use this for anything that you actually want to encrypt. This implementation was an educational exercise and is slow, probably naive, and vulnerable to all of the cryptographic no-nos. Use [OpenSSL](https://github.com/openssl/openssl) for a fast, free, open-source, and infinitely more secure encryption option.
