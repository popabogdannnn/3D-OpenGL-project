// Shader-ul de fragment / Fragment shader  
#version 400

in vec3 ex_Color; 
in vec2 tex_Coord;
out vec4 out_Color;

uniform sampler2D myTexture;
uniform int codCol;

void main(void)
{
    switch (codCol)
    {
        case 1: out_Color=vec4(0.0, 0.0, 0.0, 1.0); break;
        case 0: out_Color=vec4(0.0, 0.4, 0.0, 1.0); break;
        default: out_Color=texture(myTexture, tex_Coord);
    }
}