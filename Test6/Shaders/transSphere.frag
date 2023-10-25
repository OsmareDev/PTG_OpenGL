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
uniform float uTamCubo;
uniform mat3 uToModelMatrix;

uniform sampler2D uStoneTex; 
uniform sampler2D uMossTex;
uniform samplerCube uTexMap; 
uniform int uDrawSky;

out vec4 fFragColor;

vec3 vTexCoordReflect;
float vFresnelRatio;
vec3 vTexCoordRef;
vec3 vTexCoordRefR;
vec3 vTexCoordRefG;
vec3 vTexCoordRefB;

in vec3 vPos;
in vec3 vNor;
in vec3 vCam;

vec3 calcSalida(vec3 point, vec3 dir);

void main()
{	
	vec3 normal = normalize(uNormalMatrix * vNor);
	vec3 eyeDir = (((uModelMatrix * vec4(vPos,1)).xyz) - vec3(vCam));
		
	vTexCoordRefR = refract(eyeDir, normal, uRefractRatioR);
	vTexCoordRefG = refract(eyeDir, normal, uRefractRatioG);
	vTexCoordRefB = refract(eyeDir, normal, uRefractRatioB);

	//

	/*
	vTexCoordRefR = uToModelMatrix * vTexCoordRefR;
	vTexCoordRefG = uToModelMatrix * vTexCoordRefG;
	vTexCoordRefB = uToModelMatrix * vTexCoordRefB;
	*/
	//

	vec3 sal1 = calcSalida(vPos, vTexCoordRefR);
	vec3 sal2 = calcSalida(vPos, vTexCoordRefG);
	vec3 sal3 = calcSalida(vPos, vTexCoordRefB);

	//

	vec3 normalExtra1 = normalize(-sal1);
	vec3 normalExtra2 = normalize(-sal2);
	vec3 normalExtra3 = normalize(-sal3);

	vec3 normal1 = normalize(uNormalMatrix * normalExtra1);
	vec3 normal2 = normalize(uNormalMatrix * normalExtra2);
	vec3 normal3 = normalize(uNormalMatrix * normalExtra3);

	vec3 eyeDir1 = (((uModelMatrix * vec4(sal1,1)).xyz) - (uModelMatrix * vec4(vPos,1)).xyz);
	vec3 eyeDir2 = (((uModelMatrix * vec4(sal2,1)).xyz) - (uModelMatrix * vec4(vPos,1)).xyz);
	vec3 eyeDir3 = (((uModelMatrix * vec4(sal3,1)).xyz) - (uModelMatrix * vec4(vPos,1)).xyz);

	eyeDir1 = vTexCoordRefR;
	eyeDir2 = vTexCoordRefG;
	eyeDir3 = vTexCoordRefB;

	vTexCoordRefR = refract(eyeDir1, normal1, 1.0/uRefractRatioR);
	vTexCoordRefG = refract(eyeDir2, normal2, 1.0/uRefractRatioG);
	vTexCoordRefB = refract(eyeDir3, normal3, 1.0/uRefractRatioB);

	vFresnelRatio = ((uFresnel) + (1 - uFresnel)*pow((1 - dot(-normalize(eyeDir), normalize(normal))), 5)) * uIsFresnel;

	vec3 ecNormal = normalize(uNormalMatrix * vNor);
	vec3 ecView = vec3((uModelMatrix * vec4(vPos,1)).xyz) - vec3(uCameraPos.x,uCameraPos.y,uCameraPos.z);
	vTexCoordReflect = reflect( ecView, ecNormal );

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
}

vec3 calcSalida(vec3 point, vec3 dir) {

	/*
	float a = dir.x*dir.x+dir.y*dir.y+dir.z*dir.z;
	float b = 2*point.x*dir.x+2*point.y*dir.z+2*point.z*dir.z;
	float c = point.x*point.x+point.y*point.y+point.z*point.z;

	//a = 4*4*4*dir.x*dir.x+dir.y*dir.y+dir.z*dir.z;
	//b = 8*point.x*dir.x+8*point.y*dir.z+8*point.z*dir.z;
	//c = 4*4*4*point.x*point.x+point.y*point.y+point.z*point.z;

	float alpha1 = (-b + sqrt(b*b - 4 * a * c))/ (2 * a);
	float alpha2 = (-b - sqrt(b*b - 4 * a * c))/ (2 * a);

	float al = max(alpha1, alpha2);


	
	return point + al * dir;
	*/


	vec3 p = normalize(-point);
	vec3 d = normalize(dir);
	float x = dot(p, d);
	vec3 salida = point + ( d * x )*2;


	return salida;
}