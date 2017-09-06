/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

in  vec3  in_Position;
in  vec3  in_Normal;
in  vec2  in_TexCoord;
in 	vec3  light2Pos;  

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 vNormal;
out vec3 vPos;
out vec2 frag_texcoord;
out vec3 Rotating_Light_Pos;
void main(void)
{
	vPos = in_Position;
	Rotating_Light_Pos = light2Pos;
	vec4 oNormal = projectionMatrix * viewMatrix * vec4(in_Normal, 1.0);

	frag_texcoord = in_TexCoord;

	vNormal = normalize(mat3(viewMatrix)*in_Normal);

	gl_Position=projectionMatrix*viewMatrix*vec4(in_Position, 1.0);
}

