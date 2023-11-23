#version 430  

// We assign the ID directly to avoid having to locate the attribute in the application code
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

// Same with the uniform variables
layout(location = 0) uniform mat4 uModelViewProjMatrix;
layout(location = 1) uniform mat4 uModelViewMatrix;
layout(location = 2) uniform mat3 uNormalMatrix;

out vec3 vECNorm;
out vec3 vColor;

void main()
{
	vECNorm = normalize(uNormalMatrix * aNormal);
	vColor = abs(normalize(aNormal));

	// Save in gl_Position the position of the vertex in the S.R. of sight
	// (instead of window space, as is done now)
	//gl_Position = uModelViewProjMatrix * vec4(aPosition, 1.0);
	gl_Position = uModelViewMatrix * vec4(aPosition, 1.0);
}