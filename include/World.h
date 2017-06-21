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
#include <mutex>
#include <queue>

#include "Logger.h"
#include "Chunk.h"
#include "ChunkMap.h"
#include "ITask.h"

//FastNoise open-source lib for terrain gen
#include "../libs/FastNoise/FastNoise.h"

class World {
private:
  static Logger worldLog;
  static int VIEW_DISTANCE;

  //chunk data
  std::vector<Chunk> _chunks;
  ChunkMap _chunkMap;

  //rendering data
  GLuint _VAO;
  std::vector<GLuint> _terrainVBOs;
  GLuint _terrainShader;
  std::mutex updateVBOMutex;
  std::queue<GLuint> updateVBOs;

  //chunk generation
  FastNoise _terrainNoise;
  //std::mutex noiseMutex;
  //void genChunk(glm::ivec3 pos);
  //void genChunk(int x, int y, int z);
  void genVoxels(int chunk_id);
  void meshChunk(int chunk_id);
  void fillMeshVerts(Chunk &chunk, glm::vec3 botLeft, glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botRight, bool negFace, int dim);
  void sendVertexData(int chunk_id);

public:
  World(glm::ivec3 playerStartPos);

  //render all chunks player can see
  void render(glm::mat4 view, glm::mat4 projection); // change to include position and view direction

  //global voxel access
  int getVoxel(glm::ivec3 loc);
  int getVoxel(int x, int y, int z);
  //void setVoxel(glm::ivec3 loc, int val);
  //void setVoxel(int x, int y, int z, int val);

  //tasks
  class GenVoxelsTask : public ITask {
  private:
    World *world;
    int chunk_id;
  public:
    GenVoxelsTask(World *aworld, int achunk_id)
      : world(aworld), chunk_id(achunk_id)
    {}
    void execute();
  };

  class MeshChunkTask : public ITask {
  private:
    World *world;
    int chunk_id;
  public:
    MeshChunkTask(World *aworld, int achunk_id)
      : world(aworld), chunk_id(achunk_id)
    {}
    void execute();
  };
};

#endif
