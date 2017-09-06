/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

in float shade;

in vec3 vNormal;
in vec3 vPos;
in vec2 frag_texcoord;
//in vec3 Rotating_Light_Pos;

uniform vec3 lightPos;
uniform vec3 light2Pos;
uniform sampler2D exampletexture;
uniform float time;

out vec4 out_Color;

void main(void)
{
	vec3 color = vec3(1.0,1.0,1.0);
	//vec3 light2Pos = Rotating_Light_Pos;
	vec3 cameraPosition = vec3(0.0, 0.0, 1.0);

	vec3 ambcolor = vec3(1.0);
	vec3 lightColor = vec3(0.0,0.0,0.8);
	vec3 light = normalize(lightPos); //position of light from origo
	vec3 V = normalize(cameraPosition-vPos); //Direction to eye
	vec3 L = normalize(lightPos-vPos); //Direction to light from surface
	vec3 R = -reflect(L,vNormal); //Light reflection

	//Rotating lights
	vec3 light2Color = vec3(0.8,0.0,0.0);
	vec3 light2 = normalize(light2Pos); //position of light from origo
	vec3 L2 = normalize(light2Pos-vPos);
	vec3 R2 = -reflect(L2,vNormal);

	float ka = 0.1;
	float kd = 1.0;
	float ks = 1.0;
	float shinyness = 20.0;

	//ambient
	vec3 amb = ka*ambcolor;
	//diffuse
	vec3 diff = kd*lightColor*max(0.0, dot(vNormal,L)) + kd*light2Color*max(0.0, dot(vNormal,L2));

	//specular ligts
	float RdotV = clamp(dot(R,V),0.0,1.0);
	float RdotV2 = clamp(dot(R2,V),0.0,1.0);
	vec3 specular = ks*pow(RdotV,shinyness)*lightColor + ks*pow(RdotV2,shinyness)*light2Color;

	vec3 finalColor = (amb + diff + specular); 
	out_Color=texture(exampletexture, frag_texcoord)*vec4(finalColor,1.0);
}

