#version 150 

in vec3 vECPos;
in vec3 vECNorm;
in vec4 vShadowCoord;

out vec4 fFragColor;

uniform int uDrawingShadowMap;

struct LightInfo {
	vec4 lightPos;
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

uniform sampler2DShadow uShadowMap;
uniform int uFilter;

const int DESP = 2;
const int INC = 1;

vec3 phongModelDiffAndSpec () 
{
	vec3 ldir = normalize(vec3(uLight.lightPos) - vECPos);
	vec3 view = normalize(vec3(-vECPos));
	vec3 r = reflect(-ldir,vECNorm);

	vec3 color = uLight.intensity * ( uMaterial.diffuse * max(dot(ldir,vECNorm), 0.0) +
									  uMaterial.specular * pow(max(dot(r,view),0),uMaterial.shininess) );

	return color;
}

void main()
{
	if (uDrawingShadowMap == 0) {
		vec3 ambient = uLight.intensity * uMaterial.ambient;
		vec3 diffAndSpec = phongModelDiffAndSpec();

		float shadow;

		if (uFilter == 2) {
			float sum = 0.0f;

			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(-1,-1));
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(-1,1));
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(1,1));
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(1,-1));

			shadow = sum * 0.25;
		} else if (uFilter == 3) {
			float sum = 0.0f;
			int cont = 0;

			/* Does not work with AMD
			for (int i = -DESP; i <= DESP; i += INC) {
				for (int j = -DESP; j <= DESP; j += INC) {
					// it does not allow because the values i and j must be constant
					sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(i,j) );
					cont++;
				}
			}
			*/

			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(-2,-2) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(-2,-1) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(-2,0) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(-2,1) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(-2,2) );

			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(-1,-2) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(-1,-1) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(-1,0) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(-1,1) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(-1,2) );

			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(0,-2) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(0,-1) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(0,0) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(0,1) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(0,2) );

			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(1,-2) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(1,-1) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(1,0) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(1,1) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(1,2) );

			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(2,-2) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(2,-1) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(2,0) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(2,1) );
			sum += textureProjOffset(uShadowMap, vShadowCoord, ivec2(2,2) );

			cont = 25;


			shadow = sum / cont;

		} else {
			shadow = textureProj(uShadowMap, vShadowCoord);
		}

		fFragColor = vec4( clamp(ambient + shadow * diffAndSpec, 0.0, 1.0), 1.0 );
	}
}