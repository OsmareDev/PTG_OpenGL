#version 150 compatibility

in vec4 vColor;
out vec4 fFragColor;

void main()
{
	fFragColor = vColor;
	//return FragColor;
}