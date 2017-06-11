#include <GL/glew.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

GLuint createShader(const GLchar *vertPath, const GLchar *fragPath) {
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  ////Read Files
  try {
    vShaderFile.open(vertPath);           fShaderFile.open(fragPath);
    std::stringstream vShaderStream,      fShaderStream;

    vShaderStream << vShaderFile.rdbuf(); fShaderStream << fShaderFile.rdbuf();
    vShaderFile.close();                  fShaderFile.close();

    vertexCode = vShaderStream.str();     fragmentCode = fShaderStream.str();
  }
  catch(std::ifstream::failure e) {
    std::cout <<"Shader Error: File not succesfully read\n";
  }
  const GLchar *vShaderCode = vertexCode.c_str();
  const GLchar *fShaderCode = fragmentCode.c_str();


  ////Compile shaders
  GLuint vertex, fragment;
  GLint success;

  //Vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
    //error check
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
      GLchar infoLog[512];
      glGetShaderInfoLog(vertex, 512, NULL, infoLog);
      std::cout << "Vertex Shader Compilation Failed\n" << infoLog << "\n";
    }

  //Fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
    //error check
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
      GLchar infoLog[512];
      glGetShaderInfoLog(fragment, 512, NULL, infoLog);
      std::cout << "Fragment Shader Compilation Failed\n" << infoLog << "\n";
    }


  //Shader Porgram
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
    //error check
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
      GLchar infoLog[512];
      glGetProgramInfoLog(program, 512, NULL, infoLog);
      std::cout << "Shader Linking Failed\n\n" << infoLog << "\n";
    }

  //delete shaders
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  return program;
}
