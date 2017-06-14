/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#ifndef WORLD_H
#define WORLD_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Logger.h"
#include "Chunk.h"

//FastNoise open-source lib for terrain gen
#include "../libs/FastNoise/FastNoise.h"

class World {
private:
  static Logger worldLog;

  FastNoise _terrainNoise;
  std::vector<Chunk> _chunks;

  GLuint _VAO, _VBO;
  GLuint _terrainShader;

  void genChunk(glm::ivec3 pos);
  void meshChunk(Chunk &chunk);
  void fillMeshVerts(Chunk &chunk, glm::vec3 botLeft, glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botRight, bool negFace, int dim);

public:
  World(glm::ivec3 playerStartPos);
  void render(glm::mat4 view, glm::mat4 projection);

  //global voxel access
  int getVoxel(glm::ivec3 loc);
  int getVoxel(int x, int y, int z);
  void setVoxel(glm::ivec3 loc, int val);
  void setVoxel(int x, int y, int z, int val);
};

#endif
