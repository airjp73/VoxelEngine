/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#ifndef CHUNK_MAP_H
#define CHUNK_MAP_H

#include <glm/glm.hpp>

#include <vector>
#include <map>

#include "Chunk.h"
#include "Logger.h"

/*
  ChunkMap is a simple hash-map used to improve chunk lookup
*/

struct BucketNode {
  glm::ivec3 pos;
  int chunkID;
  BucketNode *next;

  BucketNode();
};

class ChunkMap {
private:
  static Logger chunkMapLogger;
  static const int HASH_MOD = 5003;
  static const int PRIME_X = 1163;
  static const int PRIME_Y = 1201;
  static const int PRIME_Z = 491;
  static const int PRIME_ABS = 107;
  std::vector<BucketNode*> buckets;

  int hash(int x, int y, int z);

  //debuging only
  std::map<int,int> collisions;

public:
  ChunkMap();
  ~ChunkMap();
  int getChunk(int x, int y, int z);
  int getChunk(glm::ivec3 pos);
  void removeChunk(int x, int y, int z);
  void removeChunk(glm::ivec3 pos);
  void addChunk(int x, int y, int z, int chunkID);
};

#endif
