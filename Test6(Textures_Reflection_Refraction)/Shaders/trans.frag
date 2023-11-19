#version 150

uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelMatrix;
uniform mat3 uNormalMatrix;
uniform vec3 uCameraPos;
uniform float uRefractRatio;
uniform float uRefractRatioR;
uniform float uRefractRatioG;
uniform float uRefractRatioB;
uniform float uCantidadFresnel;
uniform float uFresnel;
uniform int uIsFresnel;

uniform sampler2D uStoneTex; 
uniform sampler2D uMossTex;
uniform samplerCube uTexMap; 
uniform int uDrawSky;

in vec2 vTexCoord;
float vFresnelRatio;
vec3 vTexCoordRef;
vec3 vTexCoordReflect;
vec3 vTexCoordRefR;
vec3 vTexCoordRefG;
vec3 vTexCoordRefB;
out vec4 fFragColor;

in vec3 vPos;
in vec3 vNor;
in vec3 vCam;

void main()
{	
	if (uDrawSky){
		vTexCoordRef = vec3(vPos.x, vPos.y, vPos.z);
		vTexCoordRefR = vec3(vPos.x, vPos.y, vPos.z);
		vTexCoordRefG = vec3(vPos.x, vPos.y, vPos.z);
		vTexCoordRefB = vec3(vPos.x, vPos.y, vPos.z);
	}
	else {
		vec3 normal = normalize(uNormalMatrix * vNor);
		vec3 eyeDir = ((uModelMatrix * vec4(vPos,1)).xyz) - vec3(uCameraPos.x,uCameraPos.y,uCameraPos.z);
		vTexCoordRef = refract(eyeDir, normal, uRefractRatio);
		vTexCoordRefR = refract(eyeDir, normal, uRefractRatioR);
		vTexCoordRefG = refract(eyeDir, normal, uRefractRatioG);
		vTexCoordRefB = refract(eyeDir, normal, uRefractRatioB);


		if (uIsFresnel)
			//vFresnelRatio = (uFresnel);
			vFresnelRatio = ((uFresnel) + (1 - uFresnel)*pow((1 - dot(-normalize(eyeDir), normalize(normal))), 5));
		else
			vFresnelRatio = 0.0;

		vec3 ecNormal = normalize(uNormalMatrix * vNor);
		vec3 ecView = vec3((uModelMatrix * vec4(vPos,1)).xyz) - vec3(uCameraPos.x,uCameraPos.y,uCameraPos.z);
		vTexCoordReflect = reflect( ecView, ecNormal );
	}

	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
	color.r = texture(uTexMap, vTexCoordRefR).r;
	color.g = texture(uTexMap, vTexCoordRefG).g;
	color.b = texture(uTexMap, vTexCoordRefB).b; 

	if (uDrawSky)
		fFragColor = color; 
	else {
		vec4 reflection = texture( uTexMap, vTexCoordReflect );
		fFragColor = mix(color, reflection, vFresnelRatio);
	}
	//fFragColor = texture( uTexMap, vTexCoordRef );
}
