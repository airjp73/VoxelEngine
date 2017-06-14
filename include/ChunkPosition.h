/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#ifndef CHUNK_POSITION_H
#define CHUNK_POSITION_H

#include <glm/glm.hpp>

#include <vector>

#include "Logger.h"

#define CHUNK_SIZE 16

/*
  ChunkPosition
  -- The location of a chunk
  -- Voxel data within the chunk
*/

struct ChunkPosition {

  static Logger chunkPosLog;
  glm::ivec3 _chunkPos;
  std::vector<int> _voxels;

  ChunkPosition();
  int getVoxel(glm::ivec3 loc);
  int getVoxel(int x, int y, int z);
  void setVoxel(glm::ivec3 loc, int val);
  void setVoxel(int x, int y, int z, int val);
};

#endif
