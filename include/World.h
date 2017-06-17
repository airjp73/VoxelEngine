/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#ifndef WORLD_H
#define WORLD_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>

#include "Logger.h"
#include "Chunk.h"
#include "ChunkMap.h"

//FastNoise open-source lib for terrain gen
#include "../libs/FastNoise/FastNoise.h"

class World {
private:
  static Logger worldLog;
  static int    VIEW_DISTANCE;

  //chunk data
  std::vector<Chunk> _chunks;
  ChunkMap _chunkMap;

  //rendering data
  GLuint _VAO;
  std::vector<GLuint> _terrainVBOs;
  GLuint _terrainShader;

  //chunk generation
  FastNoise _terrainNoise;
  void genChunk(glm::ivec3 pos);
  void genChunk(int x, int y, int z);
  void meshChunk(Chunk &chunk);
  void fillMeshVerts(Chunk &chunk, glm::vec3 botLeft, glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botRight, bool negFace, int dim);

public:
  World(glm::ivec3 playerStartPos);

  //render all chunks player can see
  void render(glm::mat4 view, glm::mat4 projection); // change to include position and view direction

  //global voxel access
  int getVoxel(glm::ivec3 loc);
  int getVoxel(int x, int y, int z);
  //void setVoxel(glm::ivec3 loc, int val);
  //void setVoxel(int x, int y, int z, int val);
};

#endif
