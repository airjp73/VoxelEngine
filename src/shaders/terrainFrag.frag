#version 330 core

in vec3 normal;

vec3 dirLight = vec3(0.2f, 1.0f, 0.0f);
vec3 myColor  = vec3(0.5f, 0.5f, 0.5f);
vec3 lightColor = vec3(0.8f, 0.8f, 0.8f);

out vec4 color;


void main() {
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(dirLight);

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 ambient = (lightColor * 0.5f) * myColor;
  vec3 diffuse = lightColor * diff * myColor;
  color = vec4(diffuse + ambient, 1.0f);
}
