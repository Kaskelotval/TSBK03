#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
    vec4 color = texture(texUnit, outTexCoord);
    color = vec4(max(color.r - 1.0, 0),
                 max(color.g - 1.0, 0),
                 max(color.b - 1.0, 0),
                 1.0);

    out_Color = color;
}