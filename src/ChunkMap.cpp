/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/


#include <glm/glm.hpp>

#include "../include/Chunk.h"
#include "../include/ChunkMap.h"

#include <string>

Logger ChunkMap::chunkMapLogger("ChunkMap");

///////////BucketNode Constructor
BucketNode::BucketNode() {
  chunkID = -1;;
  next = nullptr;
}

///////////ChunkMap Constructor
ChunkMap::ChunkMap() {
  buckets = std::vector<BucketNode*>(HASH_MOD);
}

///////////ChunkMap hash function
int ChunkMap::hash(int x, int y, int z) {
  int res = abs((x * PRIME_X + y * PRIME_Y + z * PRIME_Z) + PRIME_ABS) % HASH_MOD;
  return res;
}

///////////ChunkMap getChunk
int ChunkMap::getChunk(glm::ivec3 pos) {
  int index = hash(pos.x, pos.y, pos.z);
  BucketNode *target = buckets[index];

  //find matching BucketNode in bucket
  while (target != nullptr && target->pos != pos)
    target = target->next;

  //no matching bucket
  if (target == nullptr)
    return -1;
  //invalid match
  if (target->chunkID == -1) {
    std::string loc = std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z);
    chunkMapLogger.log("BucketNode contains invalid chunk ID: " + loc, Logger::ERROR);
  }

  return target->chunkID;
}

///////////ChunkMap alternate getChunk args
int ChunkMap::getChunk(int x, int y, int z) {
  glm::ivec3 pos = glm::ivec3(x,y,z);
  return getChunk(pos);
}

///////////ChunkMap addChunk
void ChunkMap::addChunk(int x, int y, int z, int chunkID) {
  int index = hash(x,y,z);
  glm::ivec3 pos = glm::ivec3(x,y,z);

  ////debuging only to make sure hash function is good
  if (buckets[index] != nullptr) {
    ++collisions[index];
    std::string msg = "Hash Collision --- this hash value collided " + std::to_string(collisions[index]) + " times.";
    chunkMapLogger.log(msg, Logger::WARN);
  }

  //insert new BucketNode
  BucketNode *old = buckets[index];
  buckets[index]  = new BucketNode;
  buckets[index]->chunkID = chunkID;
  buckets[index]->pos   = pos;
  buckets[index]->next  = old;
}

///////////ChunkMap Destructor
ChunkMap::~ChunkMap() {
  BucketNode *cur;
  BucketNode *next;

  for (int i = 0; i < buckets.size(); ++i) {
    cur = buckets[i];

    while(cur != nullptr) {
      next = cur->next;
      delete cur;
      cur = next;
    }
  }
}
