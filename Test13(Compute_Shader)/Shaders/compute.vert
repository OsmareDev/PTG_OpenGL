#version 430  

// We assign the ID directly to avoid having to locate the attribute in the application code
in vec3 aPosition;
in vec3 aColor;
in vec3 aVeloc;
//layout(location = 1) in vec3 aNormal;

// Idem con las variables uniform
uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelViewMatrix;
uniform mat3 uNormalMatrix;

//out vec3 vECNorm;
out vec3 vColor;

void main()
{
	//vECNorm = normalize(uNormalMatrix * aNormal);
	vColor = aColor;

	// Tarea 2.1: save in gl_Position the position of the vertex in the S.R. of the view 
	// (instead of the window space, as is done now)
	//gl_Position = uModelViewProjMatrix * vec4(aPosition, 1.0);
	gl_Position = uModelViewMatrix * vec4(aPosition, 1.0);
}