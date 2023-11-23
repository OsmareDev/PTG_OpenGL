#version 400  

in vec3 aPosition;
in vec3 aNormal;

uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelViewMatrix;
uniform mat3 uNormalMatrix;


out vec3 vColor;

void main()
{
	vColor = abs(normalize(aNormal));

	gl_Position = vec4(aPosition, 1.0);
}