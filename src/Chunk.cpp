/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#include <glm/glm.hpp>
#include <vector>

#include "../include/Chunk.h"

Logger Chunk::chunkLog("Chunk");

Chunk::Chunk() {
  Chunk(0,0,0);
}

Chunk::Chunk(int x, int y, int z) {
  _chunkPos = glm::ivec3(x,y,z);
  initVoxels();
}

void Chunk::initVoxels() {
  //init _voxels to proper dimensions
  for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; ++i) {
    _voxels.push_back(0);
  }
}

int Chunk::getVoxel(glm::ivec3 loc) {
  return getVoxel(loc.x, loc.y, loc.z);
}

int Chunk::getVoxel(int x, int y, int z) {

  //check for out of range
  if (
    x < 0 || x >= CHUNK_SIZE ||
    y < 0 || y >= CHUNK_SIZE ||
    z < 0 || z >= CHUNK_SIZE
  ) {

  std::string pos = std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
  chunkLog.log("Attempt to read out of range voxel -- " + pos, Logger::ERROR);

  return 0;
}
  //return voxel value
  int index = y * CHUNK_SIZE * CHUNK_SIZE +
              z * CHUNK_SIZE +
              x;
  //std::string pos = std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(index) + ", " + std::to_string(_voxels.size());
  //chunkLog.log(pos, Logger::DEBUG);
  return _voxels[index];
}

void Chunk::setVoxel(glm::ivec3 loc, int val) {
  setVoxel(loc.x, loc.y, loc.z, val);
}

void Chunk::setVoxel(int x, int y, int z, int val) {
  if (
    x < 0 || x >= CHUNK_SIZE ||
    y < 0 || y >= CHUNK_SIZE ||
    z < 0 || z >= CHUNK_SIZE
  ) {
    std::string pos = std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
    chunkLog.log("Attempt to write voxel out of bounds -- " + pos, Logger::ERROR);
    return;
  }

  int index = y * CHUNK_SIZE * CHUNK_SIZE +
              z * CHUNK_SIZE +
              x;
  _voxels[index] = val;
}

glm::ivec3 Chunk::getPos() {
  return _chunkPos;
}
