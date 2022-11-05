/*
 * abAB
 * 01100001  01100010  01000001  01000010
 * 0110 0001 0110 0010 0100 0001 0100 0010
 * pIC  kle  pIC  piC  pIc  kle  pIc  piC
 * pICklepICpiCpIcklepIcpiC
 * 
 * Hello
 * 01001000  01100101  01101100  01101100  01101111
 * 0100 1000 0110 0101 0110 1100 0110 1100 0110 1111
 * pIc  Pic  pIC  kLe  pIC  PIc  pIC  PIc  pIC  KLE
 * pIcPicpICkLepICPIcpICPIcpICKLE
 * 
 * While the original pickle encoding used the first bit
 * of each 4-bit group to determine the type of the word
 * (PIC - KLE) and the remaining three bits to determine
 * the capitalization, my implementation instead does the
 * opposite, by using the last bit of the group to determine
 * the type and the remaining bits for the capitalization
 */

// descriptions of the functions can be found in pickle.h
#include "pickle.h"

#include <ctype.h>
#include <string.h>

// encoding
void byteenc(const char byte, char *dest) {
    // encoding the first 4 bits
    if(byte & 0x10) {                           // if the last bit of the group is 1
        dest[0] = (byte & 0x80) ? 'K' : 'k';    // the pickle word is "KLE"
        dest[1] = (byte & 0x40) ? 'L' : 'l';    // the remaining bits determine capitalization
        dest[2] = (byte & 0x20) ? 'E' : 'e';    // 0 is lowercase and 1 is uppercase
    } else {
        dest[0] = (byte & 0x80) ? 'P' : 'p';    // if the last bit is 0
        dest[1] = (byte & 0x40) ? 'I' : 'i';    // the pickle word is "PIC"
        dest[2] = (byte & 0x20) ? 'C' : 'c';
    }

    // encoding the last 4 bits
    if(byte & 0x01) {                           // same as above, but with the 2nd group
        dest[3] = (byte & 0x08) ? 'K' : 'k';
        dest[4] = (byte & 0x04) ? 'L' : 'l';
        dest[5] = (byte & 0x02) ? 'E' : 'e';
    } else {
        dest[3] = (byte & 0x08) ? 'P' : 'p';
        dest[4] = (byte & 0x04) ? 'I' : 'i';
        dest[5] = (byte & 0x02) ? 'C' : 'c';
    }
    
    dest[6] = 0;
}

void strenc(const char *str, char *dest) {
    for(int i = 0; i < strlen(str); ++i) {  // for every byte in str
        byteenc(str[i], dest);              // encode the byte to dest
        dest += 6;                          // then move dest forward
    }
}

// decoding
void bytedec(const char *str, char *dest) {
    *dest = 0;                                          // starting with a 0x00 byte

    for(int i = 0; i < 3; ++i) {                        // if the characters of the first group are uppercase
        *dest |= (isupper(str[i]) ? (0x80 >> i) : 0);   // sets the corresponding bit to 1
    }
    
    if(tolower(str[0]) == 'k')                          // if the first pickle word is "KLE"
        *dest |= 0x10;                                  // set the corresponding bit to 1

    for(int i = 3; i < 6; ++i) {                        // same as above, but with the 2nd group
        *dest |= isupper(str[i]) ? (0x80 >> (i+1)) : 0;
    }

    if(tolower(str[3]) == 'k')
        *dest |= 0x01;
}

char chrdec(const char *str) {
    char byte = 0;                                      // starting with a 0x00 byte

    for(int i = 0; i < 3; ++i) {                        // if the characters of the first group are uppercase
        byte |= (isupper(str[i]) ? (0x80 >> i) : 0);    // sets the corresponding bit to 1
    }
    
    if(tolower(str[0]) == 'k')                          // if the first pickle word is "KLE"
        byte |= 0x10;                                   // sets the corresponding bit to 1

    for(int i = 3; i < 6; ++i) {                        // same as above, but with the 2nd group
        byte |= isupper(str[i]) ? (0x80 >> (i+1)) : 0;
    }

    if(tolower(str[3]) == 'k')
        byte |= 0x01;

    return byte;                                        // return the resulting byte
}

void strdec(const char *str, char *dest) {
    size_t max = strlen(str) / 6;   // how many bytes will be decoded

    for(int i = 0; i < max; ++i) {  // for every byte in dest
        bytedec(str+6*i, dest+i);   // decode 2 pickle words from str to dest
    }

    dest[max] = 0;                  // terminate dest
}


// n encoding/decoding
void strnenc(const char *str, char *dest, size_t n) {
    for(int i = 0; (i < strlen(str)) && i <  n; ++i) {  // for every byte in str (below n)
        byteenc(str[i], dest);                          // encode the byte to dest
        dest += 6;                                      // then move dest forward
    }
}

void strndec(const char *str, char *dest, size_t n) {
    size_t max = strlen(str) / 6;           // how many bytes will be decoded (not considering n)

    for(int i = 0; i < max && i < n; ++i) { // for every byte in dest (unless above n)
        bytedec(str+6*i, dest+i);           // decode 2 pickle words from str to dest
    }

    dest[max] = 0;                          // terminate dest
}
