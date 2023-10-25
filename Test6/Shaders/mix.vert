#version 150

uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelMatrix;
uniform mat3 uNormalMatrix;
uniform vec3 uCameraPos;
uniform int uDrawSky;

in vec4 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

out vec2 vTexCoord;

void main()
{
	vTexCoord = aTexCoord;

	gl_Position = uModelViewProjMatrix * aPosition;
}
