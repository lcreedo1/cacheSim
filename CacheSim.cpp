/**
 * Liam Creedon
 * lcreedo1@jhu.edu
 * Simulating Cache
 */

#include <vector>
#include <map>
#include <iostream>
#include <ostream>
#include "CacheSim.h"

using namespace std;

Block::Block() {
    this->timeStamp = 0;
    this->dirty = false;
}

Set::Set() {
    this->count = 0;
}

CacheSim::CacheSim(int numsets, int numblocks, int numbytes, bool allo,
     bool wrt, bool evict) {
    this->totLoad = 0;
    this->totStore = 0;
    this->loadHit = 0;
    this->loadMiss = 0;
    this->storeHit = 0;
    this->storeMiss = 0;
    this->cycles = 0;
    this->time = 0;
    this->numSets = numsets;
    this->numBlocks = numblocks;
    this->numBytes = numbytes;
    this->allocation = allo;
    this->write = wrt;
    this->eviction = evict;
}

void CacheSim::addSet(string index) {
    this->cache[index] = Set();
}

void CacheSim::addBlock(string index, string tag) {
    // temp map to represent blocks
    map<string, Block> temp = this->cache[index].blocks;
    string minTag = "n/a";
    // set is full, need to replace a block
    if((int)temp.size() == this->numBlocks) {
        // determines lowest time stamp value
        for(auto it = temp.begin(); it != temp.end(); it++) {
            if (minTag.compare("n/a") == 0 || it->second.timeStamp < temp[minTag].timeStamp) {
                minTag = it->first;
            }
        }
        // represents if the block we want to remove is dirty, update cycles
        if(temp[minTag].timeStamp == 1) {
            this->cycles += 100 * this->numBytes / 4;
        }
        // removes key from map associated with lowest time stamp
        auto minIt = temp.find(minTag);
        temp.erase(minIt);
    }
    this->cache[index].blocks[tag] = Block();
    this->cache[index].count++;
    this->cycles += 100 * this->numBytes / 4;
}

void CacheSim::updateTime(string index, string tag) {
    if(this->eviction) {
        this->cache[index].blocks[tag].timeStamp += 1;
    } else {
        this->cache[index].blocks[tag].timeStamp = this->cycles;
    }
}

bool CacheSim::hit(string index, string tag) {
    auto it = this->cache.find(index);
    // index is found
    if (it != this->cache.end()) {
        // iterate through the map associated with the set we found to check for tag
        auto itB = this->cache[index].blocks.find(tag);
        if (itB != this->cache[index].blocks.end()) { // tag is in blocks
            return true;
        }
    }
    return false;
}

void CacheSim::load(string index, string tag) {
    this->cycles += this->numBytes / 4;
    this->totLoad++;
    // index not found, so create a new set
    auto it = this->cache.find(index);
    if(it == this->cache.end()) {
        this->addSet(index);
    }
    if(this->hit(index, tag)) {
        this->loadHit++;
        this->updateTime(index, tag);
    } else {
        this->loadMiss++;
        this->addBlock(index, tag);
    }
}

void CacheSim::store(string index, string tag) {
    this->totStore++;
    auto it = this->cache.find(index);
    // index not found, create new set
    if(it == this->cache.end()) {
        this->addSet(index);
    }
    if(this->allocation) {
        if(this->hit(index, tag)) {
            this->storeHit++;
            this->updateTime(index, tag);
        } else {
            this->storeMiss++;
            this->addBlock(index, tag);
            if(this->write) {
                this->cycles += 100* this->numBytes / 4;
            } else {
                this->cache[index].blocks[tag].dirty = true;
            }
        }
    } else {
        this->cycles += 100;
        if(this->hit(index, tag)) {
            this->storeHit++;
            this->updateTime(index, tag);
        } else {
            this->storeMiss++;
            if(this->write) {
                this->cycles += 100 * this->numBytes / 4;
            }
        }
    }
}

void CacheSim::printInfo() {
    cout << "Total loads: " << this->totLoad << "\n";
    cout << "Total stores: " << this->totStore << "\n";
    cout << "Load hits: " << this->loadHit << "\n";
    cout << "Load misses: " << this->loadMiss << "\n";
    cout << "Store hits: " << this->storeHit << "\n";
    cout << "Store misses: " << this->storeMiss << "\n";
    cout << "Total cycles: " << this->cycles << "\n";
}
