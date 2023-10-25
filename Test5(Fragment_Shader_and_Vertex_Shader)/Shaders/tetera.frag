#version 150 compatibility

in vec4 vColor;
in vec2 vTex;

out vec4 fFragColor;

uniform float uUmbral;

void main()
{
	vec2 aux = fract(vTex * 8);
	
	if (aux.s < uUmbral && aux.t < uUmbral)
		discard;
	else
		fFragColor = vColor;
}