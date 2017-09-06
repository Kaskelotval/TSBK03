#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
uniform sampler2D glow;
out vec4 out_Color;

void main(void)
{
    out_Color = 1*texture(texUnit, outTexCoord) + 4*texture(glow, outTexCoord);
}