#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normalIn;

out vec3 normal;

uniform mat4 view;
uniform mat4 projection;

void main() {
  normal = normalIn;
  gl_Position = projection * view * vec4(position, 1.0f);
}
