/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#include <glm/glm.hpp>
#include <vector>

#include "../include/ChunkPosition.h"

Logger ChunkPosition::chunkPosLog("ChunkPosition");

ChunkPosition::ChunkPosition() {
  //init _voxels to proper dimensions
  for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; ++i) {
    _voxels.push_back(0);
  }
}

int ChunkPosition::getVoxel(glm::ivec3 loc) {
  return getVoxel(loc.x, loc.y, loc.z);
}

int ChunkPosition::getVoxel(int x, int y, int z) {
  if (
    x < 0 || x >= CHUNK_SIZE ||
    y < 0 || y >= CHUNK_SIZE ||
    z < 0 || z >= CHUNK_SIZE
  ) {
    std::string str = std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
    //chunkPosLog.log("Attempt to read voxel out of bounds -- " + str, Logger::ERROR);
    return 0;
  }

  int index = y * CHUNK_SIZE * CHUNK_SIZE +
              z * CHUNK_SIZE +
              x;
  return _voxels[index];
}

void ChunkPosition::setVoxel(glm::ivec3 loc, int val) {
  if (
    loc.x < 0 || loc.x >= CHUNK_SIZE ||
    loc.y < 0 || loc.y >= CHUNK_SIZE ||
    loc.z < 0 || loc.z >= CHUNK_SIZE
  ) {
    chunkPosLog.log("Attempt to write voxel out of bounds", Logger::ERROR);
    return;
  }

  int index = loc.y * CHUNK_SIZE * CHUNK_SIZE +
              loc.z * CHUNK_SIZE +
              loc.x;
  _voxels[index] = val;
}
