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

World::World(glm::ivec3 playerStartPos) {
  _terrainNoise.SetNoiseType(FastNoise::NoiseType::Perlin);

  //generate starting chunks
  genChunk(glm::ivec3(playerStartPos));

  //Mesh Data
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
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
    }
  }

  //gen terrain
  _chunks.push_back(ChunkPosition());
  for (int y = 0; y < CHUNK_SIZE; ++y) {
    for (int z = 0; z < CHUNK_SIZE; ++z) {
      for (int x = 0; x < CHUNK_SIZE; ++x) {
        if (y <= heightMap[z][x]) {
          _chunks.back().setVoxel(glm::ivec3(x,y,z), 1);
        }
      }
    }
  }

  //mesh chunk
  meshChunk(_chunks.back());
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////
//meshChunk
//
//A big, long, scary chunk of code that implements a greedy meshing type algorithm
//the way it's implemented here, it loops through every block in the chunk 3 times
//and checks the top and the bottom face. The reasoning behind this is to reduce
//the number of cache misses. The int array that contains the chunk data has
//a size of 16 * 16 * 16 which means that each loop through it is going to have
//several cache misses. Processing the bottom faces in a separate loop through
//would double the number of cache misses.
//In the future, I would like to come up with a solution that involves fewer loops
//through the chunk. However, it's currently not critical to do so and this solution
//will do.
//
///////////////////////////////////////////////////
///////////////////////////////////////////////////
//Dimension class used for keeping track of coordinates in meshChunk function
class Dimension {
private:
  int a[3];
  int d;
public:
  int *x, *y, *z;

  Dimension(): a{0,0,0}, d(0), x(&a[0]), y(&a[1]), z(&a[2]) {}

  void rotate() {
    ++d;
    x = &a[d % 3];
    y = &a[(1 + d) % 3];
    z = &a[(2 + d) % 3];
  }

  int getD() {
    return d;
  }

  glm::vec3 getTrueCoord(int x, int y, int z) {
    glm::vec3 coord;
    coord[d % 3] = x;
    coord[(1 + d) % 3] = y;
    coord[(2 + d) % 3] = z;

    return coord;
  }
};
void World::meshChunk(ChunkPosition &chunk) {
  //create a ChunkMesh object and create a reference to it to avoid _meshes.back() calls
  _meshes.push_back(ChunkMesh());
  ChunkMesh &thisMesh = _meshes.back();


  ///////////////////////////////////////////////
  //Loop through all 3 dimensions
  //all the variable names are written as though they're calculating the top and bottom faces (positive y and negative y)
  //after each iteration of the loop, imagine rotating the entire chunk so that
  //the faces being calculated are still the top and the bottom
  ///////////////////////////////////////////////
  Dimension dim;
  for (; dim.getD() < 3; dim.rotate()) {

    for (*dim.y = 0; *dim.y < CHUNK_SIZE; ++*dim.y) {

      bool meshed_top[CHUNK_SIZE][CHUNK_SIZE];
      bool meshed_bot[CHUNK_SIZE][CHUNK_SIZE];
      for (int zi = 0; zi < CHUNK_SIZE; ++zi) {
        for (int xi = 0; xi < CHUNK_SIZE; ++xi) {
          meshed_top[zi][xi] = false;
          meshed_bot[zi][xi] = false;
        }
      }

      for (*dim.z = 0; *dim.z <= CHUNK_SIZE; ++*dim.z) {
        //top face vars
        int  start_x_top = 0, start_z_top = 0; //starting verts
        int  end_x_top   = 0, end_z_top   = 0; //ending verts
        bool start_new_top = true;             //starting a new check or continuing a previous check?
        int  cur_type_top  = -1;               //current block type being checked

        //bot face vars
        int  start_x_bot = 0, start_z_bot = 0; //starting verts
        int  end_x_bot   = 0, end_z_bot   = 0; //ending verts
        bool start_new_bot = true;             //starting a new check or continuing a previous check?
        int  cur_type_bot  = -1;               //current block type being checked



        for (*dim.x = 0; *dim.x <= CHUNK_SIZE; ++*dim.x) {
          int thisVoxel = chunk.getVoxel(dim.getTrueCoord(*dim.x, *dim.y, *dim.z));
          int aboveVoxel = 0;
          int belowVoxel = 0;
          //if (y + 1 < CHUNK_SIZE)
            aboveVoxel = chunk.getVoxel(dim.getTrueCoord(*dim.x, *dim.y+1, *dim.z));
          //if (y - 1 >= 0)
            belowVoxel = chunk.getVoxel(dim.getTrueCoord(*dim.x, *dim.y-1, *dim.z));

          ////////////
          //Process top face
          ////////////
          if (start_new_top) {
            if(thisVoxel != 0 && aboveVoxel == 0 && !meshed_top[*dim.z][*dim.x]) {
              start_x_top = *dim.x;
              start_z_top = *dim.z;
              cur_type_top = thisVoxel;
              start_new_top = false;
              meshed_top[*dim.z][*dim.x] = true;
            }
          }
          else {
            if (thisVoxel != cur_type_top || meshed_top[*dim.z][*dim.x] || aboveVoxel) {
              end_x_top = *dim.x;

              //find value for end_z_top
              bool doneCheck = false;
              for (int zCheck = start_z_top + 1; zCheck < CHUNK_SIZE && !doneCheck; ++zCheck) {
                for (int xCheck = start_x_top; xCheck < end_x_top && !doneCheck; ++xCheck) {

                  if (chunk.getVoxel(dim.getTrueCoord(xCheck, *dim.y, zCheck)) != cur_type_top ||
                      meshed_top[zCheck][xCheck] ||
                      chunk.getVoxel(dim.getTrueCoord(xCheck, *dim.y+1, zCheck)) != 0) {
                    doneCheck = true;
                    end_z_top = zCheck;

                    //reset meshed bools for failed row
                    for (xCheck = xCheck - 1; xCheck >= start_x_top; --xCheck)
                      meshed_top[zCheck][xCheck] = false;
                  }
                  else {
                    //update meshed bools while checking
                    //it would be faster to undo a few changes than
                    //to loop back through an entire check just to change all of them
                    meshed_top[zCheck][xCheck] = true;
                  }
                }
              }
              if (!doneCheck)
                end_z_top = CHUNK_SIZE;


              //fill the mesh object with the necessary vert data
              glm::vec3 botLeft = dim.getTrueCoord(start_x_top, *dim.y+1, start_z_top),
                        topLeft = dim.getTrueCoord(start_x_top, *dim.y+1, end_z_top),
                        botRight = dim.getTrueCoord(end_x_top, *dim.y+1, start_z_top),
                        topRight = dim.getTrueCoord(end_x_top, *dim.y+1, end_z_top);
              fillMeshVerts(thisMesh, botLeft, topLeft, topRight, botRight, false, dim.getD());


              if (meshed_top[*dim.z][*dim.x] || aboveVoxel || thisVoxel == 0)
                start_new_top = true;
              else {
                //if the check ended because the block just didn't match
                //then start new check on this block
                start_x_top = *dim.x;
                start_z_top = *dim.z;
                cur_type_top = thisVoxel;
              }
            }
            else {
              meshed_top[*dim.z][*dim.x] = true;
            }
          }

          ////////////
          //Process bottom face
          ////////////
          if (start_new_bot) {
            if(thisVoxel != 0 && belowVoxel == 0 && !meshed_bot[*dim.z][*dim.x]) {
              start_x_bot = *dim.x;
              start_z_bot = *dim.z;
              cur_type_bot = thisVoxel;
              start_new_bot = false;
            }
          }
          else {
            if (thisVoxel != cur_type_bot || meshed_bot[*dim.z][*dim.x] || belowVoxel) {
              end_x_bot = *dim.x;

              //find value for end_z_bot
              bool doneCheck = false;
              for (int zCheck = start_z_bot + 1; zCheck < CHUNK_SIZE && !doneCheck; ++zCheck) {
                for (int xCheck = start_x_bot; xCheck < end_x_bot && !doneCheck; ++xCheck) {

                  if (chunk.getVoxel(dim.getTrueCoord(xCheck, *dim.y, zCheck)) != cur_type_bot ||
                      meshed_bot[zCheck][xCheck] ||
                      chunk.getVoxel(dim.getTrueCoord(xCheck, *dim.y-1, zCheck)) != 0) {
                    doneCheck = true;
                    end_z_bot = zCheck;

                    //reset meshed bools for failed row
                    for (xCheck = xCheck - 1; xCheck >= start_x_bot; --xCheck)
                      meshed_bot[zCheck][xCheck] = false;
                  }
                  else {
                    //update meshed bools while checking
                    //it would be faster to undo a few changes than
                    //to loop back through an entire check just to change all of them
                    meshed_bot[zCheck][xCheck] = true;
                  }
                }
              }
              if (!doneCheck)
                end_z_bot = CHUNK_SIZE;


              //fill the mesh object with the necessary vert data
              glm::vec3 botLeft = dim.getTrueCoord(start_x_bot, *dim.y, start_z_bot),
                        topLeft = dim.getTrueCoord(start_x_bot, *dim.y, end_z_bot),
                        botRight = dim.getTrueCoord(end_x_bot, *dim.y, start_z_bot),
                        topRight = dim.getTrueCoord(end_x_bot, *dim.y, end_z_bot);
              fillMeshVerts(thisMesh, botLeft, topLeft, topRight, botRight, true, dim.getD());

              if (meshed_bot[*dim.z][*dim.x] || belowVoxel || thisVoxel == 0)
                start_new_bot = true;
              else {
                //if the check ended because the block just didn't match
                //then start new check on this block
                start_x_bot = *dim.x;
                start_z_bot = *dim.z;
                cur_type_bot = thisVoxel;
              }
            }
            else {
              meshed_bot[*dim.z][*dim.x] = true;
            }
          }
        }
      }
    }
  }
}

void World::fillMeshVerts(ChunkMesh &mesh, glm::vec3 botLeft, glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botRight, bool negFace, int dim) {
  if (!negFace) {
    //vert
      //normal
    mesh._verts.push_back(botLeft.x);
    mesh._verts.push_back(botLeft.y);
    mesh._verts.push_back(botLeft.z);
      mesh._verts.push_back(botLeft.x + (dim == 2));
      mesh._verts.push_back(botLeft.y + (dim == 0));
      mesh._verts.push_back(botLeft.z + (dim == 1));
    mesh._verts.push_back(topLeft.x);
    mesh._verts.push_back(topLeft.y);
    mesh._verts.push_back(topLeft.z);
      mesh._verts.push_back(topLeft.x + (dim == 2));
      mesh._verts.push_back(topLeft.y + (dim == 0));
      mesh._verts.push_back(topLeft.z + (dim == 1));
    mesh._verts.push_back(botRight.x);
    mesh._verts.push_back(botRight.y);
    mesh._verts.push_back(botRight.z);
      mesh._verts.push_back(botRight.x + (dim == 2));
      mesh._verts.push_back(botRight.y + (dim == 0));
      mesh._verts.push_back(botRight.z + (dim == 1));

    mesh._verts.push_back(topLeft.x);
    mesh._verts.push_back(topLeft.y);
    mesh._verts.push_back(topLeft.z);
      mesh._verts.push_back(topLeft.x + (dim == 2));
      mesh._verts.push_back(topLeft.y + (dim == 0));
      mesh._verts.push_back(topLeft.z + (dim == 1));
    mesh._verts.push_back(topRight.x);
    mesh._verts.push_back(topRight.y);
    mesh._verts.push_back(topRight.z);
      mesh._verts.push_back(topRight.x + (dim == 2));
      mesh._verts.push_back(topRight.y + (dim == 0));
      mesh._verts.push_back(topRight.z + (dim == 1));
    mesh._verts.push_back(botRight.x);
    mesh._verts.push_back(botRight.y);
    mesh._verts.push_back(botRight.z);
      mesh._verts.push_back(botRight.x + (dim == 2));
      mesh._verts.push_back(botRight.y + (dim == 0));
      mesh._verts.push_back(botRight.z + (dim == 1));
  }
  else {
    mesh._verts.push_back(botLeft.x);
    mesh._verts.push_back(botLeft.y);
    mesh._verts.push_back(botLeft.z);
      mesh._verts.push_back(botLeft.x - (dim == 2));
      mesh._verts.push_back(botLeft.y - (dim == 0));
      mesh._verts.push_back(botLeft.z - (dim == 1));
    mesh._verts.push_back(botRight.x);
    mesh._verts.push_back(botRight.y);
    mesh._verts.push_back(botRight.z);
      mesh._verts.push_back(botRight.x - (dim == 2));
      mesh._verts.push_back(botRight.y - (dim == 0));
      mesh._verts.push_back(botRight.z - (dim == 1));
    mesh._verts.push_back(topLeft.x);
    mesh._verts.push_back(topLeft.y);
    mesh._verts.push_back(topLeft.z);
      mesh._verts.push_back(topLeft.x - (dim == 2));
      mesh._verts.push_back(topLeft.y - (dim == 0));
      mesh._verts.push_back(topLeft.z - (dim == 1));

    mesh._verts.push_back(topLeft.x);
    mesh._verts.push_back(topLeft.y);
    mesh._verts.push_back(topLeft.z);
      mesh._verts.push_back(topLeft.x - (dim == 2));
      mesh._verts.push_back(topLeft.y - (dim == 0));
      mesh._verts.push_back(topLeft.z - (dim == 1));
    mesh._verts.push_back(botRight.x);
    mesh._verts.push_back(botRight.y);
    mesh._verts.push_back(botRight.z);
      mesh._verts.push_back(botRight.x - (dim == 2));
      mesh._verts.push_back(botRight.y - (dim == 0));
      mesh._verts.push_back(botRight.z - (dim == 1));
    mesh._verts.push_back(topRight.x);
    mesh._verts.push_back(topRight.y);
    mesh._verts.push_back(topRight.z);
      mesh._verts.push_back(topRight.x - (dim == 2));
      mesh._verts.push_back(topRight.y - (dim == 0));
      mesh._verts.push_back(topRight.z - (dim == 1));
  }
}
