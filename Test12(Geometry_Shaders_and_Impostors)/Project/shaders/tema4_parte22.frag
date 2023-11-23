#version 430

in vec3 gColor;

// Tarea 2.3 (por hacer): recoger como dato de entada las coordenadas de textura.
in vec2 gTexCoord;

// Le asignamos el ID directamente para no tener que localizar la variable uniform en el código de la aplicación
layout(location = 5) uniform sampler2D uSpriteTex;

out vec4 fFragColor;

void main()
{
	vec4 color;
	vec4 tex = texture(uSpriteTex, gTexCoord);

	if (distance(gTexCoord, vec2(0.5,0.5)) > 0.38f)
		discard;

	color = vec4(gColor, 1.0);

	fFragColor = tex * color;	
}