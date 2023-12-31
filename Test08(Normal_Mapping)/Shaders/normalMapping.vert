#version 150

in vec3 aPosition; // Vertex position in the R.S. local to object
in vec2 aTexCoord;

in vec3 aNormal; // Normal of the vertex in the R.S. local to object
in vec3 aTangent;
in vec3 aBitangent;

uniform vec3 uCamPos;

uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelViewMatrix;
uniform mat3 uNormalMatrix;

struct LightInfo {
	vec4 lightPos; // Light Position (R.S. of Sight)
	vec3 intensity;
	vec3 k;
	vec3 dir; // Focus direction (R.S. of view)
	float cutOffInt; // Interior angle (illuminated zone limit)
	float cutOffExt; // Exterior angle (penumbra zone limit)
};
uniform LightInfo uLight;

struct MaterialInfo {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform MaterialInfo uMaterial;

out vec3 vLdir;
out vec3 vVdir;
out vec2 vTexCoord;

void main()
{
	// Isn't it necessary to use the normal defined in the texture map?
	vec3 n = normalize(uNormalMatrix * aNormal);
	vec3 t = normalize(uNormalMatrix * aTangent);
	vec3 b = cross(n, t);
	mat3 TBN = transpose(mat3(t, b, n));

	vec3 eyeVertex = vec3(uModelViewMatrix * vec4(aPosition,1));

	// vector light
	vLdir = normalize( TBN * (vec3(uLight.lightPos) - eyeVertex) );

	// vector view
	vVdir = normalize( TBN * vec3(-eyeVertex) );

	vTexCoord = aTexCoord;

	gl_Position = uModelViewProjMatrix * vec4(aPosition, 1.0);
}