#version 150 compatibility

out vec4 vColor;
in vec4 aColor;
in vec4 aPosition;
uniform mat4 uModelViewProj;

void main()
{
	vColor = aColor;
	gl_Position = uModelViewProj * aPosition;
}