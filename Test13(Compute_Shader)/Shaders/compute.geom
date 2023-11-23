#version 430

// Indicate the type of input and output primitive.
layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;

// Size (side / 2) of the square where we paste the image of the sphere
const float uSize2 = 0.04;

// We assign the ID directly so we don't have to locate the uniform variable in the application code
layout(location = 3) uniform mat4 uProjectionMatrix;
layout(location = 4) uniform float uExplosionFactor = 0;

// define input data (color and normal)
//in vec3[] vECNorm;
in vec3[] vColor;
// define the shader output data (color)
out vec2 gTexCoord;
out vec3 gColor;

// define the output data corresponding to the texture coordinates

void main()
{
	// obtain the center of gravity of the triangle
	vec3 baricentro = gl_in[0].gl_Position.xyz;
	vec3 color = vColor[0];
	//vec3 normal = vECNorm[0];
	

	// Replace the above with
	// Generate as output the following strip of triangles (with 2 triangles)
	//     v2    v3
	//     *-----*
	//     |\    |
	//     | \   |
	//     |  \  |
	//     |   \ |
	//     *-----*
	//    v0     v1
	//
	//  Where v0 = center of gravity + vec4(-uSize2, -uSize2, 0.0, 0.0), etc.
	// For each vertex of the output primitive, set the output data (gl_Position, texture and color coordinates) and output its vertices
	gTexCoord = vec2(0, 1); gColor = color;
	vec3 v = baricentro + vec3(-uSize2, -uSize2, 0.0);
	gl_Position = uProjectionMatrix * vec4(v, 1);
	EmitVertex();

	gTexCoord = vec2(1, 1); gColor = color;
	v = baricentro + vec3(uSize2, -uSize2, 0.0);
	gl_Position = uProjectionMatrix * vec4(v, 1);
	EmitVertex();

	gTexCoord = vec2(0, 0); gColor = color;
	v = baricentro + vec3(-uSize2, uSize2, 0.0);
	gl_Position = uProjectionMatrix * vec4(v, 1);
	EmitVertex();

	gTexCoord = vec2(1, 0); gColor = color;
	v = baricentro + vec3(uSize2, uSize2, 0.0);
	gl_Position = uProjectionMatrix * vec4(v, 1);
	EmitVertex();

	EndPrimitive();
}
