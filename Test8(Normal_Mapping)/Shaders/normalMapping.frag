#version 150

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

uniform sampler2D uNormalMap;
uniform sampler2D uColorMap;
uniform sampler2D uGlossMap;
uniform sampler2D uHeightMap;

uniform int uUseNor;
uniform int uUseCol;
uniform int uUseGol;
uniform int uUseHei;

in vec3 vLdir;
in vec3 vVdir;
in vec2 vTexCoord;

out vec4 fFragColor;

vec3 n = vec3(0,0,1);
vec3 pos = vec3(0,0,0);
vec2 tex = vTexCoord;


const float scale = 0.05;
const float bias = -0.025;

// ADS para la luz puntual
// Parámetros:	pos - posición del vértice en el S.R. de la vista
//				n - normal del vértice en el S.R. de la vista
vec3 ads()
{
	vec3 ldir = normalize( vLdir );
	vec3 vdir = normalize( vVdir );
	vec3 r = reflect(-ldir, n);
	vec3 color;
	if (uUseCol) {
		if (uUseGol) {
			vec4 colorTex = texture( uColorMap, tex );
			vec4 glossTex = texture( uGlossMap, tex );
			color = uLight.intensity * (   vec3(colorTex) * 0.2
										 + vec3(colorTex) * max(dot(ldir,n), 0.0) 
										 + vec3(glossTex) * pow(max(dot(r,vdir),0.0), uMaterial.shininess)
									   );
		}
		else {
			vec4 colorTex = texture( uColorMap, tex );
			color = uLight.intensity * (   vec3(colorTex) * 0.2
										 + vec3(colorTex) * max(dot(ldir,n), 0.0) 
										 + vec3(0.5) * pow(max(dot(r,vdir),0.0), uMaterial.shininess)
									   );
		}
	}
	else
		color = uLight.intensity * (   uMaterial.ambient 
									 + uMaterial.diffuse * max(dot(ldir,n), 0.0) 
									 + uMaterial.specular * pow(max(dot(r,vdir),0.0), uMaterial.shininess)
								   );

	return color;
}

void main()
{
	if (uUseHei == 1) {
		float height = texture(uHeightMap, vTexCoord).r;
		float hsb = height * scale + bias;
		vec2 SToffset = vTexCoord + hsb * normalize(vVdir).xy;

		tex = SToffset;
	}

	if (uUseHei == 2) {
		float hsb1 = 1 * scale + bias;
		vec2 ST1 = vTexCoord + hsb1 * normalize(vVdir).xy;

		float hsb2 = 0 * scale + bias;
		vec2 ST2 = vTexCoord + hsb2 * normalize(vVdir).xy;

		vec2 SToffset;
		for (int i = 0; i < 6; i++) {
			SToffset = (ST1 + ST2) / 2;
			float hsb = (hsb1 + hsb2) / 2;
			float h = texture(uHeightMap, SToffset).r * scale + bias;

			if (h < hsb) {
				ST1 = SToffset;
				hsb1 = hsb;
			} else {
				ST2 = SToffset;
				hsb2 = hsb;
			}
		}

		tex = SToffset;
	}

	// normal   0, 1, 0
	if (uUseNor){
		n = normalize( 2.0 * vec3(texture(uNormalMap, tex)) - vec3(1.0) );
	}
	// position 0, 0, 0

	vec3 phong = clamp( ads(), 0.0, 1.0 );

	fFragColor = vec4(phong, 1);
}