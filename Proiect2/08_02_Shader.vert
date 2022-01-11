// Shader-ul de varfuri  
#version 400

layout(location=0) in vec4 in_Position;
layout(location=1) in vec2 texCoord;

out vec4 gl_Position; // comentati daca este cazul!
out vec2 tex_Coord;

uniform mat4 viewShader;
uniform mat4 projectionShader;
uniform mat4 translate_matrix;

void main(void)
{
    gl_Position = projectionShader*viewShader*translate_matrix*in_Position;
    tex_Coord = vec2(texCoord.x, 1-texCoord.y);
} 
 