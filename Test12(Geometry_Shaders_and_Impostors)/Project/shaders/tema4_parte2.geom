#version 430

// Tarea por hacer: indicar el tipo de primitiva de entrada y el de salida.
layout( triangles ) in;
layout( points, max_vertices = 1 ) out;

// Tamaño (lado / 2) del cuadrado donde pegamos la imagen de la esfera
const float uSize2 = 0.04;

// Le asignamos el ID directamente para no tener que localizar la variable uniform en el código de la aplicación
layout(location = 3) uniform mat4 uProjectionMatrix;
layout(location = 4) uniform float uExplosionFactor = 0;

// Tarea (por hacer): definir los datos de entrada (color y normal) 
in vec3[] vECNorm; // S.R. Vista
in vec3[] vColor;
// Tarea 2.1 (por hacer): definir el dato de salida del shader (color)
out vec3 gECNorm;
out vec3 gColor;

// Tarea 2.3 (por hacer): definir el dato de salida correspondiente a las coordenadas de textura

void main()
{
	// Tarea 2.1 (por hacer): obtener el baricentro del triángulo
	vec3 baricentro = mix(mix(gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, 0.5f), gl_in[0].gl_Position.xyz, 0.5f);
	vec3 color = mix(mix(vColor[0], vColor[1], 0.5), vColor[2], 0.5);
	vec3 normal = normalize(mix(mix(vECNorm[0], vECNorm[1], 0.5), vECNorm[2], 0.5));


	// Tarea 2.2 (por hacer): obtener la media de las normales de los tres vértices y desplazar el baricentro en esa direccion
	
	// Tarea 2.1 (por hacer): fijar los datos de salida del vértice y emitir el vértice
	gECNorm = normal; gColor = color;
	gl_Position = uProjectionMatrix * vec4(baricentro + normal * uExplosionFactor, 1);
	EmitVertex();

	EndPrimitive();

	// Tarea 2.3 (por hacer): Sustituir lo anterior por
	// Generar como salida el siguiente strip de triángulos (con 2 triángulos)
	//     v2    v3
	//     *-----*
	//     |\    |
	//     | \   |
	//     |  \  |
	//     |   \ |
	//     *-----*
	//    v0     v1
	//
	//  Donde v0 = baricentro + vec4(-uSize2, -uSize2, 0.0, 0.0), etc.
	//  Para cada vértice de la primitiva de salida, fijar los datos de salida (gl_Position, coordenadas de textura y color) y emitir sus vértices
}
