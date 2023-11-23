#version 430

in vec3 gColor;

// collect texture coordinates as input data.

// We assign the ID directly so we don't have to locate the uniform variable in the application code
layout(location = 5) uniform sampler2D uSpriteTex;

out vec4 fFragColor;

void main()
{
	vec4 color;
	
	color = vec4(gColor, 1.0);

	fFragColor = color;	
}