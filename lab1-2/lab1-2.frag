#version 150
// bump mapping should be calculated
// 1) in view coordinates
// 2) in texture coordinates

in vec2 outTexCoord;
in vec3 out_Normal;
in vec3 Ps;
in vec3 Pt;
in vec3 pixPos;  // Needed for specular reflections
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
	bool swap = true;
    vec3 light = vec3(0.0, 0.7, 0.7); // Light source in view coordinates
	
	// Calculate gradients here
	float offset = 1.0 / 256.0; // texture size, same in both directions
  	
	if(swap)
	{
		//Texturkoordinater
		//===========================//
		vec2 tc = outTexCoord;
		// Calculate ds and dt
		vec4 ds = texture(texUnit, vec2(tc.x+offset, tc.y)) - texture(texUnit, vec2(tc.x-offset, tc.y));
		vec4 dt = texture(texUnit, vec2(tc.x, tc.y+offset)) - texture(texUnit, vec2(tc.x, tc.y-offset));
		// Mvt matrix
		mat3 mvt = transpose(mat3(Ps, Pt, out_Normal));
		//transform the light vector using the mvt
		light = mvt * light; //Lt = Mvt*L
		vec3 unitNormal = normalize(vec3(ds.x, dt.x, 1.0)); //n' = n+ds*
		out_Color = vec4( dot(unitNormal, light)) * texture(texUnit, outTexCoord);
		//===========================//
	}
	else
	{
		//Vykoordinater
		//===========================//
		vec3 dx = vec3(texture(texUnit, vec2(outTexCoord.x + offset, outTexCoord.y)) - texture(texUnit, outTexCoord));
		vec3 dy = vec3(texture(texUnit, vec2(outTexCoord.x, outTexCoord.y+offset)) - texture(texUnit, outTexCoord));	
		
		vec3 normal = normalize(out_Normal);

		out_Color = vec4( dot(normalize(normal+dx*Ps+dy*Pt), light));
		//===========================//
	}



}