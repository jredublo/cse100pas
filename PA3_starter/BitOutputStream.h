//FILE: BitOutputStream.h BOPS

#include <iostream>
#include <vector>

using namespace std;
typedef unsigned char byte;

// Delegates to an object of an existing iostream class
class BitOutputStream {
private:
  char buf;       // one byte buffer of bits
  int nbits;      // how many bits have been written to buf
  ostream& os;   // reference to the output stream to use

public:

  // CONSTRUCTOR, initialise a bops that will use given ostream for output
  BitOutputStream(ostream& os): os(os) {
    buf = nbits = 0;
  }

  // Send the buffer to the output and clear it
  void flush();

  // Write least sig bit of the arg to the buf and 
  // increment the buf index but flush buffer first if full
  void writeBit(int i);

  byte bitVal (char b, int n);

  byte setBit (char b, int bit, int n);

};
