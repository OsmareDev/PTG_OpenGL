#version 150

uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelMatrix;
uniform mat3 uNormalMatrix;
uniform vec3 uCameraPos;
uniform int uDrawSky;

in vec4 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

out vec2 vTexCoord;
out vec3 vTexCoordReflect;

void main()
{
	vTexCoord = aTexCoord;

	if (uDrawSky)
		vTexCoordReflect = vec3(aPosition.x, aPosition.y, aPosition.z);
	else {
		vec3 ecNormal = normalize(uNormalMatrix * aNormal);
		vec3 ecView = vec3((uModelMatrix * aPosition).xyz) - vec3(uCameraPos.x,uCameraPos.y,uCameraPos.z);

		vTexCoordReflect = reflect( ecView, ecNormal );
	}

	gl_Position = uModelViewProjMatrix * aPosition;
}
