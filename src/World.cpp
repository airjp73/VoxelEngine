/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../libs/FastNoise/FastNoise.h"
#include "../include/World.h"
#include "../include/ChunkPosition.h"
#include "../include/ChunkMesh.h"
#include "../include/shaderFuncs.h"

#include <iostream>

Logger World::worldLog("World");

World::World(glm::ivec3 playerStartPos) {
  _terrainNoise.SetNoiseType(FastNoise::NoiseType::Perlin);
  genChunk(glm::ivec3(playerStartPos));

  //Mesh Dat
  glGenVertexArrays(1, &_VAO);
  glGenBuffers(1, &_VBO);

  glBindVertexArray(_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(
    GL_ARRAY_BUFFER,
    _meshes.back()._verts.size() * sizeof(GLfloat),
    &(_meshes.back()._verts.front()),
    GL_STATIC_DRAW
  );
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  //Shaders
  _terrainShader = createShader("../src/shaders/terrainVert.vert", "../src/shaders/terrainFrag.frag");
}

void World::genChunk(glm::ivec3 pos) {
  //generate heightMap
  int heightMap[CHUNK_SIZE][CHUNK_SIZE];
  for (int z = 0; z < CHUNK_SIZE; ++z) {
    for (int x = 0; x < CHUNK_SIZE; ++x) {
      //std::cout << _terrainNoise.GetNoise(x,z) <<  ", ";
      heightMap[z][x] = ((int)(_terrainNoise.GetNoise(x + 1, z + 1) * 10000)) % 100;
      heightMap[z][x] -= 83;
      if (heightMap[z][x] < 0)
        heightMap[z][x] = 0;
      //std::cout <<heightMap[z][x] << " ";
    }
    //std::cout << "\n";
  }

  //gen terrain
  _chunks.push_back(ChunkPosition());
  for (int y = 0; y < CHUNK_SIZE; ++y) {
    for (int z = 0; z < CHUNK_SIZE; ++z) {
      for (int x = 0; x < CHUNK_SIZE; ++x) {
        if (y <= heightMap[z][x]) {
          _chunks.back().setVoxel(glm::ivec3(x,y,z), 1);
          std::cout << _chunks.back().getVoxel(x,y,z) << " ";
        }
      }
      std::cout << "\n";
    }
    std::cout << "\n\n\n";
  }

  //mesh chunk
  meshChunk(_chunks.back());
}

void World::meshChunk(ChunkPosition &chunk) {
  _meshes.push_back(ChunkMesh());
  ChunkMesh &thisMesh = _meshes.back();

  for (int y = 0; y < CHUNK_SIZE; ++y) {
    //std::cout << "\nY ------- " << y << "\n";
    //keep track of faces already meshed
    bool meshed_top[CHUNK_SIZE][CHUNK_SIZE];
    bool meshed_bot[CHUNK_SIZE][CHUNK_SIZE];

    for (int zi = 0; zi < CHUNK_SIZE; ++zi)
      for (int xi = 0; xi < CHUNK_SIZE; ++xi)
        meshed_top[zi][xi] = false;

    for (int z = 0; z <= CHUNK_SIZE; ++z) {
      int start_x_top = 0, start_z_top = 0; //starting verts
      int   end_x_top = 0,   end_z_top = 0; //ending verts
      bool start_new_top = true;
      int cur_type = -1;

      for (int x = 0; x <= CHUNK_SIZE; ++x) {
        int thisVoxel = chunk.getVoxel(x,y,z);
        int aboveVoxel = 0;
        int belowVoxel = 0;
        //if (y + 1 < CHUNK_SIZE)
          aboveVoxel = chunk.getVoxel(x,y+1,z);
        if (y - 1 >= 0)
          belowVoxel = chunk.getVoxel(x,y-1,z);

        //process top face
        if (start_new_top) {
          if(thisVoxel != 0 && aboveVoxel == 0 && !meshed_top[z][x]) {
            start_x_top = x;
            start_z_top = z;
            cur_type = thisVoxel;
            start_new_top = false;

            if (y == 0)
            std::cout << "x: " << x << ", z: " << z << ", this: " << thisVoxel << " , above: " << aboveVoxel << "\n";
          }
        }
        else {
          if (thisVoxel != cur_type || meshed_top[z][x] || aboveVoxel) {
            //std::cout << "x: " << x << ", z: " << z << ", this: " << thisVoxel << " , above: " << aboveVoxel << "\n";
            //match row with neiboring rows
            start_new_top = true;
            bool doneCheck = false;
            end_x_top = x;

            //loop up z axis
            for (int zCheck = start_z_top + 1; zCheck < CHUNK_SIZE && !doneCheck; ++zCheck) {

              //loop through row to check if it matches
              for (int xCheck = start_x_top; xCheck < end_x_top && !doneCheck; ++xCheck) {
                if (chunk.getVoxel(xCheck, y, zCheck) != cur_type || meshed_top[zCheck][xCheck] || chunk.getVoxel(xCheck, y+1, zCheck) != 0) {
                  //std::cout << xCheck << ", " << zCheck << ", " << chunk.getVoxel(xCheck,y,zCheck) << ", " << meshed_top[zCheck][xCheck] << ", " << chunk.getVoxel(xCheck, y+1, zCheck) << "\n";
                  doneCheck = true;
                  end_z_top = zCheck;

                  //reset meshed bools for failed row
                  for (xCheck = xCheck - 1; xCheck >= 0; --xCheck)
                    meshed_top[zCheck][xCheck] = false;
                }
                else {
                  //update meshed bools while checking
                  meshed_top[zCheck][xCheck] = true;
                }
              }
            }

            thisMesh._verts.push_back(start_x_top);
            thisMesh._verts.push_back(y);
            thisMesh._verts.push_back(start_z_top);
            thisMesh._verts.push_back(start_x_top);
            thisMesh._verts.push_back(y);
            thisMesh._verts.push_back(end_z_top);
            thisMesh._verts.push_back(end_x_top);
            thisMesh._verts.push_back(y);
            thisMesh._verts.push_back(start_z_top);

            thisMesh._verts.push_back(start_x_top);
            thisMesh._verts.push_back(y);
            thisMesh._verts.push_back(end_z_top);
            thisMesh._verts.push_back(end_x_top);
            thisMesh._verts.push_back(y);
            thisMesh._verts.push_back(end_z_top);
            thisMesh._verts.push_back(end_x_top);
            thisMesh._verts.push_back(y);
            thisMesh._verts.push_back(start_z_top);
          }
          else {
            meshed_top[z][x] = true;
          }
        }
        //

        //process bottom face
      }
      /*if (!start_new_top) {
        thisMesh._verts.push_back(start_x_top);
        thisMesh._verts.push_back(y);
        thisMesh._verts.push_back(start_z_top);
        thisMesh._verts.push_back(start_x_top);
        thisMesh._verts.push_back(y);
        thisMesh._verts.push_back(end_z_top);
        thisMesh._verts.push_back(end_x_top);
        thisMesh._verts.push_back(y);
        thisMesh._verts.push_back(start_z_top);

        thisMesh._verts.push_back(start_x_top);
        thisMesh._verts.push_back(y);
        thisMesh._verts.push_back(end_z_top);
        thisMesh._verts.push_back(end_x_top);
        thisMesh._verts.push_back(y);
        thisMesh._verts.push_back(end_z_top);
        thisMesh._verts.push_back(end_x_top);
        thisMesh._verts.push_back(y);
        thisMesh._verts.push_back(start_z_top);
      }*/
    }
  }
}

void World::render(glm::mat4 view, glm::mat4 projection) {
  glUseProgram(_terrainShader);
  glBindVertexArray(_VAO);
  GLuint viewLoc = glGetUniformLocation(_terrainShader, "view");
  GLuint projLoc = glGetUniformLocation(_terrainShader, "projection");
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
  glDrawArrays(GL_TRIANGLES, 0, _meshes.back()._verts.size());
  glBindVertexArray(0);
}
