#version 150  

in vec3 aPosition;
in vec3 aNormal;

uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelMatrix;
uniform mat3 uNormalMatrix;

uniform int uDrawEnvironment;
uniform vec3 uCameraPos;
uniform vec3 uAmbientLight;

uniform int uHemisActivated;
uniform int uArmonicActivated;

struct MaterialInfo {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform MaterialInfo uMaterial;

uniform mat4 uRedMatrix;
uniform mat4 uGreenMatrix;
uniform mat4 uBlueMatrix;

out vec3 vNormal;
out vec3 vPosition;
out vec3 vAmbientLight; 
out vec3 vTextCoordCuboEntorno;
out vec3 vDiffuse;

out vec3 vReflectDir; // To be used in the reflections of the reflection map (specular part of IBL and SPH)

uniform vec3 uSkyColor;
uniform vec3 uGroundColor;

// Scale factor by which the illumination result must be multiplied. based on harmonics (diffuse)
const float SPH_Scale = 0.90f;

void main()
{
	vec4 pos = vec4(aPosition, 1.0);

	vNormal = normalize(uNormalMatrix * aNormal);
	vPosition = vec3(uModelMatrix * pos);
	vReflectDir = reflect(vec3(vPosition) - uCameraPos, vNormal);

	if (uDrawEnvironment == 1)
		vTextCoordCuboEntorno = aPosition;
	else {
		if (uHemisActivated) {
			float costheta = vNormal.y;
			float a = costheta * 0.5 + 0.5;
			vAmbientLight = mix(uGroundColor, uSkyColor, a);
		}
		else
			vAmbientLight = uAmbientLight;

		if (uArmonicActivated == 2) {
			vec4 n = vec4(vNormal, 1.0);
			vDiffuse = uMaterial.diffuse * SPH_Scale * vec3(dot(n, uRedMatrix*n), dot(n, uGreenMatrix*n), dot(n, uBlueMatrix*n));
		}
	}
	
	gl_Position = uModelViewProjMatrix * pos;
}