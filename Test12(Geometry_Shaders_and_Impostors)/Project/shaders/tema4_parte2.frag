#version 430

in vec3 gColor;

// Tarea 2.3 (por hacer): recoger como dato de entada las coordenadas de textura.

// Le asignamos el ID directamente para no tener que localizar la variable uniform en el c�digo de la aplicaci�n
layout(location = 5) uniform sampler2D uSpriteTex;

out vec4 fFragColor;

void main()
{
	vec4 color;
	
	color = vec4(gColor, 1.0);

	fFragColor = color;	
}