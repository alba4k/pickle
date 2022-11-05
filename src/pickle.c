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

// encode byte into a 6 byte string,
// dest needs to have a buffer of at least 7B
void encode(char byte, char *dest) {
    // encoding the first 4 bits
    if(byte & 0x10) {                           // checks if the last bit of the group is 1
        dest[0] = (byte & 0x80) ? 'K' : 'k';    // if so, the pickle word is "KLE"
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