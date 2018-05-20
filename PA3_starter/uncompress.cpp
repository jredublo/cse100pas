//FILE: uncompress.cpp
#include <iostream>
#include <fstream>
#include "HCTree.h"

using namespace std;


/**
 * Uncompress function:  takes endcoded file of header and 0s and 1s
 *                  and recreates original file with decode function
 */
void uncompress(string infile, string outfile) {
  cout << "START OF UNCOMPRESS FNC" << endl;

  /*   Opening IF STREAM and connect to infile   */
  ifstream ifs;
  ifs.open(infile);
  if (!ifs.is_open()) {       // check for successful open
    cerr << "File was not opened" << endl;
    return;
  }
  cout << "File is open" << endl;

  /*   Recreating frequency vector with header   */
  vector<int> freqs = vector<int>(256, 0);    // empty vector n = 256
  string code;
  for (unsigned int i = 0; i < 256; i++) {
    getline(ifs, code);
    freqs[i] = stoi(code);
    // cout << "FREQ IS " << freqs[i] << endl;
  }
 
  /*   Building Tree   */
  HCTree tree;
  tree.build(freqs);
  cout << "TREE WAS JUST BUILT" << endl;

  /*   Opening OF STREAM and connect to outfile   */
  ofstream ofs;
  ofs.open(outfile);
  if (!ofs.is_open()) {         // check for successful open
    cerr << "File was not opened" << endl;
    return;
  }

  /*   Passing series of 0s and 1s to decode function   */
  char c;
  while(!ifs.eof()) {
    cout << "DECODE HERE" << endl;
    int result = tree.decode(ifs);
    char cRes = (char)result;
    ofs << cRes; 
  }
  
  ifs.close();      // CLOSE of and if stream
  ofs.close();

}



/**
 * Main function to take in arguments and call uncompress.
 */
int main (int argc, char** argv) {
  cout << "ARGC: " << argc << endl;
  for (int i = 0; i < argc; i++) {
    cout << " ARG: " << argv[i] << endl;
  }

  string encodedfile = argv[1];
  string finalfile = argv[2];

  uncompress(encodedfile, finalfile);
  return 1;
}


