// FILE: compress.cpp
#include  <iostream>
#include "HCTree.h"
#include <fstream>

using namespace std;



// take in a file filled with ASCII chars, build tree based off
// freqs, then encode each char
void compress( string infile, string outfile ) {
  cout << "START OF COMPRESS FNC" << endl;
  
  // IF STREAM, first open
  ifstream ifs;
  ifs.open(infile);

  //make sure the file was opened
  if (!ifs.is_open()) {
    cerr << "File was not opened" << endl;
    return;
  }

  
  // getting frequencies of letters in input
  vector<int> freqs = vector<int>(256, 0); // empty vector n = 256
  char c;
  while(!ifs.eof()) {
    ifs.get(c);
    int place = (int)c;
    freqs[place] = freqs[place] + 1;
  }
  
  freqs.at(10) = 0;
  freqs.at(255) = 0;

  const vector<int> cFreqs = freqs;

  ifs.close(); // close input
  
  // OFSTREAM, for output 
  ofstream ofs;
  ofs.open(outfile);

  if (!ofs.is_open()) {
    cerr << "File was not opened" << endl;
    return;
  }

  // making tree
  HCTree tree;
  tree.build(cFreqs);

  // header: printing out freqs line by line
  for (int n = 0; n < cFreqs.size(); n++) {
    int curr = cFreqs.at(n);
    ofs << curr << endl;
  }

  ifs.open(infile); // OPEN INPUT FILE AGAIN
  if (!ifs.is_open()) {
    cerr << "File was not opened" << endl;
    return;
  } 
 // start encoding
  char ch;
  while(!ifs.eof()) {
    ifs.get(ch);
    byte b = (byte)ch;
    if (int(ch) != 10)
      tree.encode(b,ofs);  
  }
  ifs.close();
  ofs.close();

}

// MAIN
/*int main(string ogfile, string encoded) {
  compress(ogfile, encoded);
  return 1;
}*/

int main (int argc, char** argv) {
  cout << "ARG C IS " << argc << endl;
  for (unsigned int i = 0; i < argc; i++ ) {
    cout << "ARGUMENT: " << argv[i] << endl;  
  }
  string ogfile = argv[1];
  string encoded = argv[2];

  compress(ogfile, encoded);
  return 1;

}




