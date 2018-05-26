// FILE: BitInputStream.h


#include <iostream>
#include <vector>

using namespace std;
typedef unsigned char byte;

// used in encode and compress functions to compress
// a file and then writes it to a txt file
class BitInputStream {
private:
  char buf;         // one byte buf of bits
  int nbits;        // how many bits have been read from buf
  istream& in;      // input stream to use

public:

  // CONSTRUCTOR:
  // Initialize a BIPS that will use the given
  // istream for input
  BitInputStream(istream& is) : in(is) {
    buf = 0;      // clear buf
    nbits = 8;    // init bit index;  start at 8 to actually start filling
  }

  // Reads bits into 8-bit buffer
  int readBit();
  
  // Fill buffer from input
  void fill();



  // HELPER FUNCS
  // Returns value of nth bit from the right of a byte 
  // arg and returns the result
  byte bitVal(char b, int n);

  // Set some value of nth bit from the right of a byte 
  // arg, leaving other bits unchanged
  byte setBit(char b, int bit, int n);


};




/*
 *  bitwise operators
 *    a & b
 *    a | b
 *    ~a
 *    a << 4
 *    b >> 1
 *    ( b >> 1 ) & 1
 *
 */
