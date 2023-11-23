#version 150

in vec3 aPosition; // Vertex position in the R.S. local to object
in vec3 aNormal; // Vertex normal in the R.S. local to object
in vec2 aTexCoord;

uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelViewMatrix;
uniform mat3 uNormalMatrix;

struct LightInfo {
	vec4 lightPos; // Light Position (R.S. of Sight)
	vec3 intensity;
	vec3 k;
	vec3 dir; // Focal direction (R.S. of view)
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

uniform int uFoco;

out vec3 vColor;
out flat vec3 vColorFlat;

out vec3 vEcPos;
out vec3 vEcNorm;

// ADS for spot light
// 		Parameters: pos - position of the vertex in the R.S. of view
// 					n - normal of the vertex in the R.S. of view
vec3 ads(in vec3 pos, in vec3 n)
{
	vec3 ldir = normalize( vec3(uLight.lightPos) - pos );
	vec3 vdir = normalize( vec3(-pos) );
	vec3 r = reflect(-ldir, n);
	vec3 color;
	color = uLight.intensity * (   uMaterial.ambient 
								 + uMaterial.diffuse * max(dot(ldir,n), 0.0) 
								 + uMaterial.specular * pow(max(dot(r,vdir),0.0), uMaterial.shininess)
							   );

	return color;
}

vec3 adsSpotLight (in vec3 pos, in vec3 n) {
	vec3 ldir = vec3(uLight.lightPos) - pos;
	float d = length(ldir);
	ldir = normalize(ldir);

	float angle = acos(dot(-ldir, normalize(uLight.dir)));
	vec3 I = uLight.intensity / (uLight.k[0] + uLight.k[1]*d + uLight.k[2]*d*d );
	vec3 ambient = I * uMaterial.ambient;
	I *= 1 - smoothstep(uLight.cutOffInt, uLight.cutOffExt, angle);
	vec3 view = normalize(vec3(-pos));
	vec3 r = reflect(-ldir, n);

	return ambient + I * (   uMaterial.ambient 
							+ uMaterial.diffuse * max(dot(ldir,n), 0.0) 
							+ uMaterial.specular * pow(max(dot(r,view),0.0), uMaterial.shininess)
						);
}

void main()
{
	vec3 ecPos = vec3(uModelViewMatrix * vec4(aPosition, 1.0)); // Position in the R.S. view
	vEcPos = ecPos;
	vec3 ecNorm = normalize(uNormalMatrix * aNormal); // Normal in the R.S. view
	vEcNorm = ecNorm;

	vColor = clamp( adsSpotLight(ecPos, ecNorm), 0.0, 1.0 ) * (1 - uFoco) + clamp( ads(ecPos, ecNorm), 0.0, 1.0 ) * (uFoco);
	vColorFlat = clamp( adsSpotLight(ecPos, ecNorm), 0.0, 1.0 ) * (1 - uFoco) + clamp( ads(ecPos, ecNorm), 0.0, 1.0 ) * (uFoco);

	gl_Position = uModelViewProjMatrix * vec4(aPosition, 1.0);
}