#version 150

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelViewMatrix;
uniform mat3 uNormalMatrix;
uniform mat4 uShadowMatrix;

uniform int uDrawingShadowMap;

out vec3 vECPos;
out vec3 vECNorm;
out vec4 vShadowCoord;

void main()
{
	if (uDrawingShadowMap == 0) {
		vECPos = vec3(uModelViewMatrix * vec4(aPosition, 1.0));
		vECNorm = normalize(uNormalMatrix * aNormal);

		vShadowCoord = uShadowMatrix * vec4(aPosition,1.0);
	}

	gl_Position = uModelViewProjMatrix * vec4(aPosition, 1.0);
}