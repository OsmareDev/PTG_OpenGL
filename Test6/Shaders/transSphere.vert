#version 150

uniform mat4 uModelViewProjMatrix;
uniform vec3 uCameraPos;

in vec4 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

out vec2 vTexCoord;

out vec3 vPos;
out vec3 vNor;
out vec3 vCam;


void main()
{
	vTexCoord = aTexCoord;

	vPos = aPosition.xyz;
	vNor = aNormal;
	vCam = uCameraPos;

	gl_Position = uModelViewProjMatrix * aPosition;
}
