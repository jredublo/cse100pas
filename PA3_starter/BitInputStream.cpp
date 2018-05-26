//FILE: BitInputStream.cpp BIPS

#include "BitInputStream.h"
#include <iostream>
#include <vector>
#include <bitset>

using namespace std;
typedef unsigned char byte;


// Fill the buffer from the input
void BitInputStream::fill() {
  buf = in.get();
  bitset<8> x(buf);
  //cout << "NEW VALUBUF: " << buf << "    " << x << endl;
  //cout << "~~~~ VALUE IN BASE 2: " << x << endl;
  nbits = 0;          // the amount of the buf that has been read and processed
}



/* Read the next bit from the bit buffer
 * Fill buf from the input stream first if needed
 * return 1 if the bit read is 1
 * return 0 if the bit read is 0
 */
int BitInputStream::readBit() {
  //cout << "value of nbits at start of readBit= " << nbits << endl;
  
  //if all bits in the buffer are read, fill the buffer first
  if (nbits == 8){
    fill();
  }

  //get the bit at the appropriate location in the bit buffer,
  //and return the appropriate int
  byte i = bitVal(buf, nbits+1);

  //cout << "     value of byte i " << i << endl;
  //incremement the index
  nbits++;

  //cout << "      value of (INT)byte i " << (int)i << endl;
  return (int)i;
}



// return the value (1 or 0) of the nth bit from the right of a byte 
// argument, and return the result
byte BitInputStream::bitVal (char b, int n) {
  //cout << "looking for " << n << " bit in " << b << endl;
  
  unsigned char c = b; 

  // shift desired bit to be the rightmost bit of c
  c = c >> n-1;
  // & with 1 to check if the bit is 0 or 1
  c = c & 0b00000001;

  bitset<8> cBIN(c);
  //cout << "   cBIN: " << cBIN << endl;
  //then the nth bit was 1
  if (c == 1) {
      return (byte)1;
  }

  //else it was 0
  return (byte)0;
}







// set the value (1 or 0) of the nth bit from the right of a byte
// argument, leaving other bits unchanged, and return the result
byte BitInputStream::setBit(char b, int bit, int n) {
  unsigned char c = 0b00000001;
  c << n;
  // change the bit to 1
  if (bit == 1){ 
    b | c;
  }
  if (bit == 0) {
    b & c;
  }
  return (byte)b;
}



