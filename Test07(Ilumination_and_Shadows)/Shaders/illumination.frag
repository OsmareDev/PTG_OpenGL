#version 150

uniform int uColorSuave;
uniform int uColorPlano;
uniform int uColorPhong;
uniform int uColorComic;
uniform int uFoco;

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

in vec3 vColor;
in flat vec3 vColorFlat;

in vec3 vEcPos;
in vec3 vEcNorm;

out vec4 fFragColor;

// ADS para la luz puntual
// Parámetros:	pos - posición del vértice en el S.R. de la vista
//				n - normal del vértice en el S.R. de la vista
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

vec3 adsComic (in vec3 pos, in vec3 n) {
	vec3 ldir = normalize( vec3(uLight.lightPos) - pos );
	vec3 vdir = normalize( vec3(-pos) );
	vec3 r = reflect(-ldir, n);

	float x = dot(vdir, normalize(r));

	vec3 aux = normalize(pos) * normalize(n);
	float t = length(aux);
	t = dot(normalize(-pos), n);
	if (t < 0.14)
		return vec3(0,0,0);


	float angle = acos(dot(-ldir, normalize(uLight.dir)));

	if (x > 0.94) {
		return uMaterial.diffuse + uMaterial.specular;

	} else {
		x = dot(ldir, n);
		if (x > 0.0) {
			return uMaterial.diffuse;
	
		} else {
			return uMaterial.ambient * 1.5;

		}
	}
}

vec3 adsComicFoco (in vec3 pos, in vec3 n) {
	vec3 ldir = normalize( vec3(uLight.lightPos) - pos );
	vec3 vdir = normalize( vec3(-pos) );
	vec3 r = reflect(-ldir, n);

	float x = dot(vdir, normalize(r));

	vec3 aux = normalize(pos) * normalize(n);
	float t = length(aux);
	t = dot(normalize(-pos), n);
	if (t < 0.14)
		return vec3(0,0,0);


	float angle = acos(dot(-ldir, normalize(uLight.dir)));

	if (angle > uLight.cutOffExt)
		return uMaterial.ambient * 1.5;

	if (x > 0.94) {
		return uMaterial.diffuse + uMaterial.specular;

	} else {
		x = dot(ldir, n);
		if (x > 0.0) {
			return uMaterial.diffuse;
	
		} else {
			return uMaterial.ambient * 1.5;

		}
	}
}

void main()
{
	vec3 ecNorm = normalize(vEcNorm);
	vec3 comic = clamp( adsComic(vEcPos, ecNorm), 0.0, 1.0 ) * (uFoco) + clamp( adsComicFoco(vEcPos, ecNorm), 0.0, 1.0 ) * (1 - uFoco);

	vec3 phong = clamp( adsSpotLight(vEcPos, ecNorm), 0.0, 1.0 ) * (1 - uFoco) + clamp( ads(vEcPos, ecNorm), 0.0, 1.0 ) * (uFoco);
	vec3 color = vColorFlat * uColorPlano + vColor * uColorSuave + phong * uColorPhong + comic * uColorComic;

	fFragColor = vec4(color, 1);
}