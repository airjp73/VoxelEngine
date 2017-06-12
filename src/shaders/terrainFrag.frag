#version 330 core

in vec3 normal;

vec3 dirLight = normalize(vec3(0.2f, 1.0f, 0.0f));
vec3 myColor  = vec3(0.5f, 0.4f, 0.5f);
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

out vec4 color;


void main() {
  vec3 norm = normalize(normal);

  float diff = max(dot(norm, dirLight), 0.0);
  vec3 diffuse = diff * lightColor;
  vec3 result = diffuse * myColor;
  color = vec4(result, 1.0f);
}
