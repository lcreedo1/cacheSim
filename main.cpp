/**
 * Liam Creedon
 * lcreedo1@jhu.edu
 * Simulating Cache
 */

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <ostream>
#include <iomanip>

#include "CacheSim.h"

using namespace std;

//trims input so only second field is left
string trim(string input) {
    input.erase(0,4);
    input.pop_back();
    input.pop_back();
    return input;
}

bool isPowerOfTwo(int n) {
    if(n<=0) { return false; }
    return ((n & (n-1)) == 0);
}

int logBase2(int n) {
    int log = 0;
    while (n > 1) {
        n /= 2;
        log++;
    }
    return log;
}

string hexToBin(string hexdec)
{
    string bin = "";
    int i = 0;
    while (hexdec[i]) {
        switch (hexdec[i]) {
        case '0':
            bin.append("0000");
            break;
        case '1':
            bin.append("0001");
            break;
        case '2':
            bin.append("0010");
            break;
        case '3':
            bin.append("0011");
            break;
        case '4':
            bin.append("0100");
            break;
        case '5':
            bin.append("0101");
            break;
        case '6':
            bin.append("0110");
            break;
        case '7':
            bin.append("0111");
            break;
        case '8':
            bin.append("1000");
            break;
        case '9':
            bin.append("1001");
            break;
        case 'A':
        case 'a':
            bin.append("1010");
            break;
        case 'B':
        case 'b':
            bin.append("1011");
            break;
        case 'C':
        case 'c':
            bin.append("1100");
            break;
        case 'D':
        case 'd':
            bin.append("1101");
            break;
        case 'E':
        case 'e':
            bin.append("1110");
            break;
        case 'F':
        case 'f':
            bin.append("1111");
            break;
        default:
            //cout << "\nInvalid hexadecimal digit "
            //<< hexdec[i];
            break;
        }
        i++;
    }
    return bin;
}


int main(int argc, char* argv[]) {

    // checks that number of arguments is correct
    if(argc != 7) {
        cerr << "7 arguments needed for this program.\n";
        return 1;
    }

     string in = "";
     char cmd;
     // taking sets, blocks, and bytes from command line command
     // converting to ints
     char* nS = argv[1];
     char* nB = argv[2];
     char* nY = argv[3];
     int numSets = atoi(nS);
     int numBlocks = atoi(nB);
     int numBytes = atoi(nY);

     //error checking for numSets, blocks, and bytes to be power of 2
     if(!(isPowerOfTwo(numSets))) {
         cerr << "Number of sets must be positive power of two.\n";
         return 1;
     }
     if(!(isPowerOfTwo(numBlocks))) {
         cerr << "Number of blocks must be positive power of two.\n";
         return 1;
     }
     if(!(isPowerOfTwo(numBytes)) || numBytes < 4) {
         cerr << "Number of bytes must be positive power of two and at least 4.\n";
         return 1;
     }

     //check arg4 for allocation command
     string allo = argv[4];
     if(allo.compare("write-allocate") != 0 &&
        allo.compare("no-write-allocate") != 0) {
         cerr << "Allocation command is misformatted.\n";
         return 1;
     }
     bool allocateMethod = false;
     if(allo.compare("write-allocate") == 0) {
         allocateMethod = true;
     }
     //check arg5 for write command
     string write = argv[5];
     if(write.compare("write-through") != 0 &&
        write.compare("write-back") != 0) {
         cerr << "Write command is misformatted.\n";
         return 1;
     }
     bool writeMethod = false;
     if(write.compare("write-through") == 0) {
         writeMethod = true;
     }
     //check arg6 for eviction method
     string evict = argv[6];
     if(evict.compare("fifo") != 0 &&
        evict.compare("lru") != 0) {
         cerr << "Eviction method is misformatted.\n";
         return 1;
     }
     bool evictMethod = false;
     if(evict.compare("lru") == 0) {
         evictMethod = true;
     }

     if(!allocateMethod && !writeMethod) {
         cerr << "Cannot have 'no-write-allocate' and 'write-back' together.\n";
         return 1;
     }

     // get offset size
     int off = logBase2(numBytes);
     //cout << off << "\n";
     CacheSim cash = CacheSim(numSets, numBlocks, numBytes,
          allocateMethod, writeMethod, evictMethod);

     while(getline(cin, in)) {

         cmd = in.at(0);    // instruction to store or load
         string data = trim(in);    // sends input string to trim function
         data = hexToBin(data);
         //cout << data << "\n";
         int len = data.length();
         string add = data.substr(0, len-off);

         len = add.length();
         int indexOff = logBase2(numSets);
         //cout << indexOff << "\n";
         string index = add.substr(len-indexOff, indexOff - 1);
         string tag = add.substr(0, len-indexOff);
         //cout << add << "," << tag << "," << index << "\n";

         if(cmd == 'l') {
             cash.load(index, tag);
         } else if(cmd == 's') {
             cash.store(index, tag);
         } else {
             cerr << "Invalid file contents.\n";
             return 1;
         }

     }
     cash.printInfo();

     return 0;
}
