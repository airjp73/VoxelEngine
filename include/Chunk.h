/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#ifndef CHUNK_H
#define CHUNK_H

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <vector>

#include "Logger.h"

#define CHUNK_SIZE 16

/*
  ChunkPosition
  -- The location of a chunk
  -- Voxel data within the chunk
*/

struct Chunk {

  static Logger chunkLog;
  glm::ivec3 _chunkPos;
  std::vector<int> _voxels;
  std::vector<GLfloat> _verts;

  Chunk();
  int getVoxel(glm::ivec3 loc);
  int getVoxel(int x, int y, int z);
  void setVoxel(glm::ivec3 loc, int val);
  void setVoxel(int x, int y, int z, int val);
};

#endif
