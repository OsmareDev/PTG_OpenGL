#version 430

// indicate the type of input and output primitive.
layout( triangles ) in;
layout( points, max_vertices = 1 ) out;

// Size (side / 2) of the square where we paste the image of the sphere
const float uSize2 = 0.04;

// We assign the ID directly so we don't have to locate the uniform variable in the application code
layout(location = 3) uniform mat4 uProjectionMatrix;
layout(location = 4) uniform float uExplosionFactor = 0;

// define input data (color and normal)
in vec3[] vECNorm; // S.R. Vista
in vec3[] vColor;
// define the shader output data (color)
out vec3 gECNorm;
out vec3 gColor;

// define the output data corresponding to the texture coordinates

void main()
{
	// obtain the center of gravity of the triangle
	vec3 baricentro = mix(mix(gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, 0.5f), gl_in[0].gl_Position.xyz, 0.5f);
	vec3 color = mix(mix(vColor[0], vColor[1], 0.5), vColor[2], 0.5);
	vec3 normal = normalize(mix(mix(vECNorm[0], vECNorm[1], 0.5), vECNorm[2], 0.5));


	// obtain the average of the normals of the three vertices and move the centroid in that direction
	
	// set the output data of the vertex and emit the vertex
	gECNorm = normal; gColor = color;
	gl_Position = uProjectionMatrix * vec4(baricentro + normal * uExplosionFactor, 1);
	EmitVertex();

	EndPrimitive();

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
	// Where v0 = center of gravity + vec4(-uSize2, -uSize2, 0.0, 0.0), etc.
	// For each vertex of the output primitive, set the output data (gl_Position, texture and color coordinates) and output its vertices
}
