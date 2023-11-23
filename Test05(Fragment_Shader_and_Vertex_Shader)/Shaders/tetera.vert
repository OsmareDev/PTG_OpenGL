#version 150 compatibility

out vec4 vColor;
out vec2 vTex;
in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;
uniform mat4 uModelViewProj;
uniform mat4 uColorRotation;
uniform float uInter;

void main()
{
	vColor = uColorRotation * vec4(abs(aNormal),1);

	//vec3 posMax = (aNormal * 3);
	vec3 posMax = (normalize(aPosition) * 3);

	vec4 auxPos = vec4(mix(aPosition, posMax, uInter), 1);
	gl_Position = uModelViewProj * auxPos;

	vTex = aTexCoord;
}