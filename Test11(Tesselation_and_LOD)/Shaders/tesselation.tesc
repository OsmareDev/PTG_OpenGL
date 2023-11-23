#version 400

layout (vertices = 4) out;

const int uTessLevel = 4;

const float uMinDepth = 3.0;
const float uMaxDepth = 15.0;
const float uMinTessLevel = 2.0;
const float uMaxTessLevel = 10.0; // 64.0

uniform mat4 uModelViewMatrix;
uniform int uLOD;

in vec3 vColor[];
out vec3 cColor[];

float level (in vec4 pos)
{
	vec4 p = uModelViewMatrix * pos;
	float depth = clamp( (abs(p.z) - uMinDepth) / (uMaxDepth - uMinDepth), 0.0, 1.0);
	return mix( uMaxTessLevel, uMinTessLevel, depth );
}

void main()
{
	float tessLevel;
	if (uLOD == 1)
		tessLevel = level(vec4(0.f,0.f,0.f,1.f));
	else if (uLOD == 2 || uLOD == 3)
		tessLevel = level(mix(gl_in[0].gl_Position, gl_in[2].gl_Position, 0.5));
	else 
		tessLevel = float(uTessLevel);

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	gl_TessLevelInner[0] = tessLevel;
	gl_TessLevelInner[1] = tessLevel;

	if (uLOD == 3)
		tessLevel = level(mix(gl_in[3].gl_Position, gl_in[0].gl_Position, 0.5));
	gl_TessLevelOuter[0] = tessLevel;

	if (uLOD == 3)
		tessLevel = level(mix(gl_in[1].gl_Position, gl_in[0].gl_Position, 0.5));
	gl_TessLevelOuter[1] = tessLevel;

	if (uLOD == 3)
		tessLevel = level(mix(gl_in[1].gl_Position, gl_in[2].gl_Position, 0.5));
	gl_TessLevelOuter[2] = tessLevel;

	if (uLOD == 3)
		tessLevel = level(mix(gl_in[3].gl_Position, gl_in[2].gl_Position, 0.5));
	gl_TessLevelOuter[3] = tessLevel;

	
	cColor[gl_InvocationID] = vColor[gl_InvocationID];
}