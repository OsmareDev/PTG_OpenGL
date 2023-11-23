#version 430

in vec3 gColor;

// collect texture coordinates as input data.
in vec2 gTexCoord;

// We assign the ID directly so we don't have to locate the uniform variable in the application code
layout(location = 5) uniform sampler2D uSpriteTex;

out vec4 fFragColor;

void main()
{
	vec4 color;
	vec4 tex = texture(uSpriteTex, gTexCoord);

	if (distance(gTexCoord, vec2(0.5,0.5)) > 0.38f)
		discard;

	color = vec4(gColor, 1.0);

	fFragColor = tex * color;	
}