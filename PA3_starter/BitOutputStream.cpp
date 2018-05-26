//FILE: BitOutputStream.cpp

#include <iostream>
#include <vector>
#include <bitset>
#include "BitOutputStream.h"

using namespace std;
typedef unsigned char byte;

// Send the buffer to the output, and clear it
void BitOutputStream::flush() {
  bitset<8> huh(buf);
  //cout << "JUST FLUSHED: " << huh << endl;
  //os.flush();
  os.put(buf);            // writing single byte to the stream
  os.flush();             // flush the ostream
  buf = (char)0;
  nbits = 0;    // set everything back to 0
}


/**
 * Write the least significant bit of the argument to the 
 * bit buffer, and incrememnt the bit buffer index.
 * But flush the buffer first, if it is full
 */
void BitOutputStream::writeBit(int i) {
  
  bitset<8> bset = bitset<8>(buf);
  //cout << "Writing " << i << " to buf." << endl;
  //cout << "   current state of buf: " << bset << endl;
  
  //cout << "   nBits valueee: " << nbits << endl << "   ~~~~ Changing ~~~~" << endl;
  //is the bit buffer full? then flush it
  if (nbits > 7) {
    flush();
  } 

  //write the least significant bit of i into the buffer
  //at the current index
  //byte b = setBit(buf, nbits+1, i);
  bset[nbits] = i;
  //cout << "   !!newbset!!  " << bset << endl;
  string what = bset.to_string();
  //cout << "what:: " << what << endl;
  //int number = stoi(what);
  int done = (int)(bset.to_ulong());
  buf = done;

  
  //buf = (byte)number;
  bitset<8> brandNew = bitset<8>(buf);
  //cout << "   now new state of buf: " << brandNew << endl;

  //increment the index
  nbits++; 
  //cout << endl;
}



// set the value (1 or 0) of the nth bit from the right of a byte
// argument, leaving other bits unchanged, and return the result
byte BitOutputStream::setBit(char b, int bit, int n) {
  bitset<8> bset(b);
  //cout << "Setting " << n << "bit of byte " << b << "to " << bit << endl;
  
  bset[n] = bit;

  string what = bset.to_string();

  int number = stoi(what);
  return (byte)number;
  /*

  unsigned char c = 0b00000001;
  c << n;
  // change the bit to 1
  if (bit == 1){
    b | c;
  }
  if (bit == 0) {
    b & c;
  }
  return (byte)b; */
}
















// return the value (1 or 0) of the nth bit from the right of a byte 
// argument, and return the result
byte BitOutputStream::bitVal (char b, int n) {
  unsigned char c = b;   //copy of argument
 
  //shift desired bit to be the rightmost bit of c
  c >> n-1;
  //& with 1 to check if the bit is 0 or 1
  c & 0b00000001;

  //if c is 1 then the nth bit was 1
  if (c == 1) {
    return (byte)1;
  }

  //else it was 0
  return (byte)0;
}
