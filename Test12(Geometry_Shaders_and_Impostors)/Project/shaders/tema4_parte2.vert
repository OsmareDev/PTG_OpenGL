#version 430  

// Le asignamos el ID directamente para no tener que localizar el atributo en el código de la aplicación
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

// Idem con las variables uniform
layout(location = 0) uniform mat4 uModelViewProjMatrix;
layout(location = 1) uniform mat4 uModelViewMatrix;
layout(location = 2) uniform mat3 uNormalMatrix;

out vec3 vECNorm; // S.R. Vista
out vec3 vColor;

void main()
{
	vECNorm = normalize(uNormalMatrix * aNormal);
	vColor = abs(normalize(aNormal));

	// Tarea 2.1: guardar en gl_Position la posición del vértice en el S.R. de la vista 
	// (en lugar del espacio de la ventana, tal como se hace ahora)
	//gl_Position = uModelViewProjMatrix * vec4(aPosition, 1.0);
	gl_Position = uModelViewMatrix * vec4(aPosition, 1.0);
}