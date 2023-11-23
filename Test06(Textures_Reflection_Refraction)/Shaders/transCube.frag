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

uniform float uAngCrit;

uniform sampler2D uStoneTex; 
uniform sampler2D uMossTex;
uniform samplerCube uTexMap; 
uniform int uDrawSky;

in vec2 vTexCoord;
in flat int vdebug;
out vec4 fFragColor;

in vec3 vPos;
in vec3 vNor;
in vec3 vCam;

float vFresnelRatio;
vec3 vTexCoordRef;
vec3 vTexCoordReflect;
vec3 vTexCoordRefR;
vec3 vTexCoordRefG;
vec3 vTexCoordRefB;

vec3 npos;

vec3 normalisimo;

vec3 calcSalida(vec3 point, vec3 dir);
int debugger;
vec3 colordebug;

void main()
{	
	debugger = 0;
	if (uDrawSky){
		vTexCoordRefR = vec3(vPos.x, vPos.y, vPos.z);
		vTexCoordRefG = vec3(vPos.x, vPos.y, vPos.z);
		vTexCoordRefB = vec3(vPos.x, vPos.y, vPos.z);
	}
	else {

		vec3 normal = normalize(uNormalMatrix * vNor);
		vec3 eyeDir = (((uModelMatrix * vec4(vPos,1)).xyz) - vec3(vCam));
		
		vTexCoordRefR = refract(eyeDir, normal, uRefractRatioR);
		vTexCoordRefG = refract(eyeDir, normal, uRefractRatioG);
		vTexCoordRefB = refract(eyeDir, normal, uRefractRatioB);


		//
		vec3 sal1 = calcSalida(vPos, vTexCoordRefR);
		vec3 normalExtra1 = normalisimo;
		vec3 sal2 = calcSalida(vPos, vTexCoordRefG);
		vec3 normalExtra2 = normalisimo;
		vec3 sal3 = calcSalida(vPos, vTexCoordRefB);
		vec3 normalExtra3 = normalisimo;



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

	if (debugger)
		fFragColor = vec4(colordebug,1);

}

vec3 calcSalida(vec3 point, vec3 dir) {

	//vec3 recta = (point + alpha * dir);
	vec3 alpha;
	alpha.x = ((sign(dir.x) * 1.0f) - point.x) / dir.x;
	alpha.y = ((sign(dir.y) * 1.0f) - point.y) / dir.y;
	alpha.z = ((sign(dir.z) * 1.0f) - point.z) / dir.z;

	// hay que encontrar el alpha mas pequeño
	float a = min(min(alpha.x, alpha.y), alpha.z);

	vec3 res = point + a * dir;

	debugger = 0;

	if (alpha.x == a){
		normalisimo = vec3(-sign(dir.x), 0, 0);
		colordebug = vec3(1,0,0);
	}

	if (alpha.y == a){
		normalisimo = vec3(0, -sign(dir.y), 0);
		colordebug = vec3(0,1,0);
	}

	if (alpha.z == a){
		normalisimo = vec3(0,0,-sign(dir.z));
		colordebug = vec3(0,0,1);
	}

	// tener en cuenta la reflexion total

	vec3 n = normalize(normalisimo);
	vec3 d = -normalize(dir);

	float x = dot(d, n);

	if (acos((length(n)*x)/length(d)) >= uAngCrit) {
		vec3 ndir = -(n / ((n*x)/d));
		/*
		vec3 normal1 = n;
		vec3 eyeDir1 = dir;
		ndir = refract(eyeDir1, normal1, 1.0/uRefractRatioR);
		*/
		alpha;
		alpha.x = ((sign(ndir.x) * 1.0f) - point.x) / ndir.x;
		alpha.y = ((sign(ndir.y) * 1.0f) - point.y) / ndir.y;
		alpha.z = ((sign(ndir.z) * 1.0f) - point.z) / ndir.z;

		// hay que encontrar el alpha mas pequeño
		float a = min(min(alpha.x, alpha.y), alpha.z);

		vec3 res = point + a * ndir;

		if (alpha.x == a){
			normalisimo = vec3(-sign(ndir.x), 0, 0);
			colordebug = vec3(1,0,0);
		}

		if (alpha.y == a){
			normalisimo = vec3(0, -sign(ndir.y), 0);
			colordebug = vec3(0,1,0);
		}

		if (alpha.z == a){
			normalisimo = vec3(0,0,-sign(ndir.z));
			colordebug = vec3(0,0,1);
		}

		vec3 n = normalize(normalisimo);
		vec3 d = -normalize(ndir);

		x = dot(d, n);
	}

	return res;
}