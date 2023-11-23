#version 150

in vec3 aPosition; // Posición del vértice en el S.R. local al objeto
in vec2 aTexCoord;

in vec3 aNormal; // Normal del vértice en el S.R. local al objeto
in vec3 aTangent;
in vec3 aBitangent;

uniform vec3 uCamPos;

uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelViewMatrix;
uniform mat3 uNormalMatrix;

struct LightInfo {
	vec4 lightPos; // Posición de la luz (S.R. de la vista)
	vec3 intensity;
	vec3 k;
	vec3 dir; // Dirección del foco (S.R. de la vista)
	float cutOffInt; // Ángulo interior (límite zona iluminada)
	float cutOffExt; // Ángulo exterior (límite zona penumbra)
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
	// no es necesario usar la normal definida en el texture map?
	vec3 n = normalize(uNormalMatrix * aNormal);
	vec3 t = normalize(uNormalMatrix * aTangent);
	vec3 b = cross(n, t);
	mat3 TBN = transpose(mat3(t, b, n));

	vec3 eyeVertex = vec3(uModelViewMatrix * vec4(aPosition,1));

	// vector luz
	vLdir = normalize( TBN * (vec3(uLight.lightPos) - eyeVertex) );

	// vector vista
	vVdir = normalize( TBN * vec3(-eyeVertex) );

	vTexCoord = aTexCoord;

	gl_Position = uModelViewProjMatrix * vec4(aPosition, 1.0);
}