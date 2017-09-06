#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;


void main() {

  float offset = 1.0 / 512;
  vec2 tc = outTexCoord;

  vec4 c = texture(texUnit, tc);
  tc.x = tc.x + offset;
  vec4 l = texture(texUnit, tc);
  tc.x = tc.x - 2.0*offset;
  vec4 r = texture(texUnit, tc);
  tc.x = tc.x - offset;
  out_Color = (c + c + l + r) * 0.25;
}

/*uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);


void main(void)
{
    vec2 tex_offset = 1.0 / textureSize(texUnit, 0); // gets size of single texel
    vec3 result = texture(texUnit, outTexCoord).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(texUnit, outTexCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(texUnit, outTexCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(texUnit, outTexCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(texUnit, outTexCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    out_Color = vec4(result, 1.0);
}*/