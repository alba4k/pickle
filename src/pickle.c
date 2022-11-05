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

