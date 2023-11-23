#version 150 

in vec3 vTextCoordCuboEntorno;
in vec3 vNormal;
in vec3 vPosition;
in vec3 vAmbientLight;
in vec3 vDiffuse;

in vec3 vReflectDir; // To be used in the reflections of the reflection map (specular part of IBL and SPH)


uniform samplerCube uEnvironmentMap; 
uniform samplerCube uSpecularMap;
uniform samplerCube uDiffuseMap;

uniform vec3 uCameraPos;
uniform int uDrawEnvironment;

uniform int uSpecularActivated;
uniform int uAmbientActivated;
uniform int uArmonicActivated;
uniform int uImageActivated;

uniform mat4 uRedMatrix;
uniform mat4 uGreenMatrix;
uniform mat4 uBlueMatrix;

struct LightInfo {
	vec3 lightDir; // Direction of light (world)
	vec3 intensity;
};
uniform LightInfo uLight;
struct MaterialInfo {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform MaterialInfo uMaterial;

out vec4 fFragColor;

// Scale factor by which the illumination result must be multiplied. image-based (diffuse)
const float IBL_Scale = 1.2;
// Scale factor by which the illumination result must be multiplied. based on harmonics (diffuse)
const float SPH_Scale = 0.90f;

void main()
{
	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);
	vec3 norm = normalize(vNormal);

	if ( uDrawEnvironment == 1 )
		fFragColor = texture(uEnvironmentMap, vTextCoordCuboEntorno); 
	else
	{
		// Ambient part of local lighting
		ambient = uMaterial.ambient * vAmbientLight;

		if (uAmbientActivated == 0) {
			// Diffuse part of local lighting
			vec3 ldir = vec3(uLight.lightDir);
			diffuse = uMaterial.diffuse * uLight.intensity * max(dot(ldir,norm), 0.0);

			// Specular part of local lighting
			if (uSpecularActivated == 1)
			{
				vec3 view = normalize(uCameraPos - vPosition);
				vec3 refl = normalize(reflect(-ldir, norm));
				specular = uMaterial.specular * uLight.intensity * pow(max(dot(refl,view),0),uMaterial.shininess);
			}
		}

		if (uArmonicActivated == 1) {
			ambient = vec3(0.0f);
			if (uSpecularActivated)
				specular = uMaterial.specular * SPH_Scale * vec3(texture(uSpecularMap, normalize(vReflectDir)));
			else
				specular = vec3(0.0f);

			vec4 n = vec4(norm, 1.0);
			diffuse = uMaterial.diffuse * SPH_Scale * vec3(dot(n, uRedMatrix*n), dot(n, uGreenMatrix*n), dot(n, uBlueMatrix*n));
		}

		if (uArmonicActivated == 2) {
			ambient = vec3(0.0f);
			if (uSpecularActivated)
				specular = uMaterial.specular * SPH_Scale * vec3(texture(uSpecularMap, normalize(vReflectDir)));
			else
				specular = vec3(0.0f);

			diffuse = vDiffuse;
		}

		if (uImageActivated) {
			ambient = vec3(0.0f);
			if (uSpecularActivated)
				specular = uMaterial.specular * IBL_Scale * vec3(texture(uSpecularMap, normalize(vReflectDir)));
			else
				specular = vec3(0.0f);
			diffuse = uMaterial.diffuse * IBL_Scale * vec3(texture(uDiffuseMap, norm));
		}

		fFragColor = vec4(clamp(ambient + diffuse + specular, 0.0, 1.0), 1.0); 
	}
}