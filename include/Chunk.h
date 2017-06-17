/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#ifndef CHUNK_H
#define CHUNK_H

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <vector>
#include <memory>

#include "Logger.h"

#define CHUNK_SIZE 16

/*
  ChunkPosition
  -- The location of a chunk
  -- Voxel data within the chunk
*/


class Chunk {
private:
  static Logger chunkLog;
  glm::ivec3 _chunkPos;
  //std::vector<int> _voxels;
  std::vector<GLfloat> _verts;
  friend class World;

  //6 sides of cube == 6 neighbors
  //top, bot, +x, -x, +z, -z
  static const int POS_Y = 0;
  static const int NEG_Y = 1;
  static const int POS_X = 2;
  static const int NEG_X = 3;
  static const int POS_Z = 4;
  static const int NEG_Z = 5;
  static const int NUM_NEIGHBORS = 6;
  std::vector<Chunk*> neighbors;
public:
  std::vector<int> _voxels;
  Chunk();
  Chunk(int x, int y, int z);
  void initVoxels();

  int getVoxel(glm::ivec3 loc);
  int getVoxel(int x, int y, int z);
  void setVoxel(glm::ivec3 loc, int val);
  void setVoxel(int x, int y, int z, int val);
  glm::ivec3 getPos();
};

#endif
