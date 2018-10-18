# aes256
An aes256 implementation in C that I wrote following along with the [NIST FIPS 197 document](https://nvlpubs.nist.gov/nistpubs/fips/nist.fips.197.pdf) for some C practice and to understand AES256.

# build

gcc *.c -w -g -lm -o aes256

# usage

`./aes256`  Run with the data and key from the NIST FIPS 197 document Appendix C.3 example.

`./aes256 [Message to encrypt]` Run with data from command line.

`./aes256 [Message to encrypt] [Key in 02x hex]` Run with data and key from command line.

# note
If the data and key from the command line didn't tip you off, don't use this for anything that you actually want to encrypt. This implementation was an educational exercise and is slow, probably naive, and vulnerable to all of the cryptographic no-nos. Use [OpenSSL](https://github.com/openssl/openssl) for a free, open-source, and infinitely more secure encryption option.
