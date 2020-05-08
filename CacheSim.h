/**
 * Liam Creedon
 * lcreedo1@jhu.edu
 * Simulating Cache
 */
#ifndef CACHESIM_H
#define CACHESIM_H

#include <vector>
#include <map>
#include <ostream>

using namespace std;

class Block {
public:
    int timeStamp;
    bool dirty;
    Block();
};

class Set {
public:
    // represents number of items in map (should not exceed numBlocks)
    int count;
    // maps tags to Block containing time stamp and level of filth
    map<string, Block> blocks;

    Set();
};

class CacheSim {
public:

    int totLoad;
    int totStore;
    int loadHit;
    int loadMiss;
    int storeHit;
    int storeMiss;
    int cycles;
    int time;
    map<string, Set> cache;
    int numSets;
    int numBlocks;
    int numBytes;
    bool allocation;    // true if write-allocate, false if no-write-allocate
    bool write;         // true if write-through, false if write-back
    bool eviction;      // true if lru, false if fifo


    CacheSim(int numsets, int numblocks, int numbytes, bool allo, bool write,
        bool evict);

    void addSet(string index);

    void addBlock(string index, string tag);

    void updateTime(string index, string tag);

    bool hit(string index, string tag);

    void load(string index, string tag);

    void store(string index, string tag);

    void printInfo();

};

#endif
