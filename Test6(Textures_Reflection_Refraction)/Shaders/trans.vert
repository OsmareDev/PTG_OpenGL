#version 150

uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelMatrix;
uniform mat3 uNormalMatrix;
uniform vec3 uCameraPos;
uniform int uDrawSky;
uniform float uRefractRatio;
uniform float uRefractRatioR;
uniform float uRefractRatioG;
uniform float uRefractRatioB;
uniform float uCantidadFresnel;
uniform float uFresnel;
uniform int uIsFresnel;

in vec4 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

out vec2 vTexCoord;
out vec3 vTexCoordRef;
out vec3 vTexCoordReflect;
out vec3 vTexCoordRefR;
out vec3 vTexCoordRefG;
out vec3 vTexCoordRefB;
out float vFresnelRatio;

out vec3 vPos;
out vec3 vNor;
out vec3 vCam;

void main()
{
	vTexCoord = aTexCoord;

	vPos = aPosition.xyz;
	vNor = aNormal;
	vCam = uCameraPos;

	gl_Position = uModelViewProjMatrix * aPosition;
}
