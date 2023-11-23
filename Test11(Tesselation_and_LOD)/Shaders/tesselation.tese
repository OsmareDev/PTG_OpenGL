#version 400


layout (quads) in;

out vec3 teNormal;
out vec4 tePosition;

in vec3 cColor[];
out vec3 eColor;

uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelViewMatrix;
uniform mat3 uNormalMatrix;

void main()
{
	// vertex position
	
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	vec4 p00 = gl_in[0].gl_Position;
	vec4 p01 = gl_in[1].gl_Position;
	vec4 p10 = gl_in[2].gl_Position;
	vec4 p11 = gl_in[3].gl_Position;

	vec3 c00 = cColor[0];
	vec3 c01 = cColor[1];
	vec3 c10 = cColor[2];
	vec3 c11 = cColor[3];


	vec3 color = c00 + ((c11 - c00) * v + (c01 - c00) * u);
	vec4 p = p00 + ((p11 - p00) * v + (p01 - p00) * u);
	vec4 pos = vec4(normalize(p.xyz), 1);

	teNormal = uNormalMatrix * pos.xyz;

	tePosition = uModelViewMatrix * pos;

	eColor = color;

	gl_Position = uModelViewProjMatrix * pos;

}
