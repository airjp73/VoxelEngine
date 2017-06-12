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
#include "ChunkPosition.h"
#include "ChunkMesh.h"

//FastNoise open-source lib for terrain gen
#include "../libs/FastNoise/FastNoise.h"

class World {
private:
  static Logger worldLog;

  FastNoise _terrainNoise;
  std::vector<ChunkPosition> _chunks;
  std::vector<ChunkMesh> _meshes;

  GLuint _VAO, _VBO;
  GLuint _terrainShader;

  void genChunk(glm::ivec3 pos);
  void meshChunk(ChunkPosition &chunk);
  void fillMeshVerts(ChunkMesh &mesh, glm::vec3 botLeft, glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botRight, bool negFace, int dim);

public:
  World(glm::ivec3 playerStartPos);
  void render(glm::mat4 view, glm::mat4 projection);
};

#endif
