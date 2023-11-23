#version 400

in vec4 tePosition;
in vec3 teNormal;
in vec3 eColor;

struct LightInfo {
	vec3 lightDir;
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

uniform int uLighting = 0;

vec3 ads()
{
	vec3 ldir = normalize(uLight.lightDir);
	vec3 vdir = -normalize(tePosition.xyz);
	vec3 n = normalize(teNormal);
	vec3 r = reflect(-ldir, n);
	vec3 color;
	color = uLight.intensity * (   uMaterial.ambient 
								 + uMaterial.diffuse * max(dot(ldir,n), 0.0) 
								 + uMaterial.specular * pow(max(dot(r,vdir),0.0), uMaterial.shininess)
							   );

	return clamp(color, 0.0, 1.0);
}

out vec4 fFragColor;

void main()
{
	vec3 color;

	if ( uLighting == 1 )
		color = ads();
	else 
		color = eColor;

	fFragColor = vec4(color, 1.0);	
}