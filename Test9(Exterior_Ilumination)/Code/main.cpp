#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vboteapot.h"
#include "teapotdata.h"
#include "lodepng.h"
#include "OBJ_Loader.h"

void load_obj(const GLchar *, std::vector<glm::vec4> &, std::vector<glm::vec3> &, std::vector<GLushort> &);
GLint initObj(std::string);
void drawObj(GLboolean = true);

void initCube();
GLint initSphere(GLfloat radius, GLuint rings, GLuint sectors);
GLint initTeapot(GLint grid, glm::mat4 transform);
void drawCube();
void drawSphere();
void drawTeapot();

void loadSource(GLuint &shaderID, std::string name);
void printCompileInfoLog(GLuint shadID);
void printLinkInfoLog(GLuint programID);
void validateProgram(GLuint programID);

GLboolean init();
void display();
void resize(GLint, GLint);
void idle();
void keyboard(GLubyte, GLint, GLint);
void specialKeyboard(GLint, GLint, GLint);
void mouse(GLint, GLint, GLint, GLint);
void mouseMotion(GLint, GLint);

void printFPS();


GLboolean fullscreen = false;
GLboolean mouseDown = false;
GLboolean animation = false;
 
GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat xdiff = 0.0f;
GLfloat ydiff = 0.0f;

GLint g_Width = 500;
GLint g_Height = 500;


GLboolean specularActivated = false;
GLboolean ambientActivated = false;
GLboolean hemisActivated = false;
GLuint armonicActivated = 0;
GLboolean imageActivated = false;

GLuint objVAOHandle, cubeVAOHandle, sphereVAOHandle, teapotVAOHandle;
GLuint programID;

GLuint locUniformEnvironmentMap, locUniformSpecularMap, locUniformDiffuseMap;
GLuint locUniformMVPM, locUniformMM, locUniformNM;
GLuint locUniformDrawEnvironment, locUniformCameraPos;
GLuint locUniformLightDir, locUniformLightIntensity;
GLuint locUniformMaterialAmbient, locUniformMaterialDiffuse, locUniformMaterialSpecular, locUniformMaterialShininess;

GLuint locUniformAmbientLight, locUniformGroundLight, locUniformSkyLight;
GLuint locUniformSpecularActivated, locUniformAmbientActivated, locUniformHemisActivated, locUniformArmonicActivated, locUniformImageActivated;
GLuint locUniformSkyColor, locUniformGroundColor;
GLuint locUniformMR, locUniformMG, locUniformMB;

GLint numVertTeapot, numVertSphere, numVertObj;

GLuint textIds[4];

// Initialize loader for obj objects
objl::Loader Loader;

// cube ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3

// Coordinates of the vertex array ==================================================
// A cube has 6 sides and each side has 2 triangles, therefore, a cube
// has 36 vertices (6 sides * 2 trian * 3 vertices = 36 vertices). And every
// vertex has 4 real components (x,y,z), therefore, the size of the vertex
// array is 144 floats (36 * 4 = 144).
GLfloat vertices1[] = { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,      // v0-v1-v2 (front)
					   -1,-1, 1,   1,-1, 1,   1, 1, 1,      // v2-v3-v0

						1, 1, 1,   1,-1, 1,   1,-1,-1,      // v0-v3-v4 (right)
						1,-1,-1,   1, 1,-1,   1, 1, 1,      // v4-v5-v0

						1, 1, 1,   1, 1,-1,  -1, 1,-1,      // v0-v5-v6 (top)
					   -1, 1,-1,  -1, 1, 1,   1, 1, 1,      // v6-v1-v0

					   -1, 1, 1,  -1, 1,-1,  -1,-1,-1,      // v1-v6-v7 (left)
					   -1,-1,-1,  -1,-1, 1,  -1, 1, 1,      // v7-v2-v1

					   -1,-1,-1,   1,-1,-1,   1,-1, 1,      // v7-v4-v3 (bottom)
						1,-1, 1,  -1,-1, 1,  -1,-1,-1,      // v3-v2-v7

						1,-1,-1,  -1,-1,-1,  -1, 1,-1,      // v4-v7-v6 (back)
					   -1, 1,-1,   1, 1,-1,   1,-1,-1 };    // v6-v5-v4

GLfloat normales1[] = { 0, 0, 1,   0, 0, 1,   0, 0, 1,      // v0-v1-v2 (front)
						0, 0, 1,   0, 0, 1,   0, 0, 1,      // v2-v3-v0

						1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v3-v4 (right)
						1, 0, 0,   1, 0, 0,   1, 0, 0,      // v4-v5-v0

						0, 1, 0,   0, 1, 0,   0, 1, 0,      // v0-v5-v6 (top)
						0, 1, 0,   0, 1, 0,   0, 1, 0,      // v6-v1-v0

					   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v1-v6-v7 (left)
					   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v7-v2-v1

						0,-1, 0,   0,-1, 0,   0,-1, 0,      // v7-v4-v3 (bottom)
						0,-1, 0,   0,-1, 0,   0,-1, 0,      // v3-v2-v7

						0, 0,-1,   0, 0,-1,   0, 0,-1,      // v4-v7-v6 (back)
						0, 0,-1,   0, 0,-1,   0, 0,-1 };    // v6-v5-v4


// BEGIN: Support for OBJ models /////////////////////////////////////////////////////////////////////////////////

void load_obj(const GLchar* filename, std::vector<GLfloat> &vertices, std::vector<GLfloat> &normals, std::vector<GLfloat> &textcoord, std::vector<GLuint> &elements) 
{
	GLint offset = 0;

	// Load .obj File
	GLboolean loadout = Loader.LoadFile(filename);

	if (!loadout) { std::cerr << "Failed to load file: " << filename << std::endl; std::system("pause"); exit(1); }

	for (GLuint i = 0; i < Loader.LoadedMeshes.size(); i++)
	{
		// Copy one of the loaded meshes to be our current mesh
		objl::Mesh curMesh = Loader.LoadedMeshes[i];

		// Go through each vertex and get its number,
		//  position, normal, and texture coordinate
		for (GLuint j = 0; j < curMesh.Vertices.size(); j++)
		{
			vertices.push_back(curMesh.Vertices[j].Position.X); 
			vertices.push_back(curMesh.Vertices[j].Position.Y); 
			vertices.push_back(curMesh.Vertices[j].Position.Z);

			normals.push_back(curMesh.Vertices[j].Normal.X);
			normals.push_back(curMesh.Vertices[j].Normal.Y);
			normals.push_back(curMesh.Vertices[j].Normal.Z);

			textcoord.push_back(curMesh.Vertices[j].TextureCoordinate.X); 
			textcoord.push_back(curMesh.Vertices[j].TextureCoordinate.Y);
		}

		// Go through every 3rd index and print the
		//	triangle that these indices represent
		for (GLuint j = 0; j < curMesh.Indices.size(); j ++)
		{
			elements.push_back(curMesh.Indices[j] + offset);
		}
		offset += curMesh.Vertices.size();
	}
	return;
}

GLint initObj(std::string file)
{
	std::vector<GLfloat> obj_vertices;
	std::vector<GLfloat> obj_normals;
	std::vector<GLfloat> obj_texcoord;
	std::vector<GLuint> obj_elements;

	load_obj(file.c_str(), obj_vertices, obj_normals, obj_texcoord, obj_elements);

	glGenVertexArrays( 1, &objVAOHandle );
	glBindVertexArray(objVAOHandle);

	GLuint handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, obj_vertices.size() * sizeof(GLfloat), obj_vertices.data(), GL_STATIC_DRAW); // Datos de la posici�n de los v�rtices
	GLuint loc1 = glGetAttribLocation(programID, "aPosition");   
	glEnableVertexAttribArray(loc1); // Vertex position
	glVertexAttribPointer( loc1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL + 0 ); 

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, obj_normals.size() * sizeof(GLfloat), obj_normals.data(), GL_STATIC_DRAW); // Datos de las normales de los v�rtices
	GLuint loc2 = glGetAttribLocation(programID, "aNormal");   
	glEnableVertexAttribArray(loc2); // Vertex normal
	glVertexAttribPointer( loc2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL + 0 );

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, obj_texcoord.size() * sizeof(GLfloat), obj_texcoord.data(), GL_STATIC_DRAW); // Datos de las coordenadas de textura
	GLuint loc3 = glGetAttribLocation(programID, "aTexCoord");
	glEnableVertexAttribArray(loc3); // Texture coords
	glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL + 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj_elements.size() * sizeof(GLuint), obj_elements.data(), GL_STATIC_DRAW); // Array de �ndices

	glBindVertexArray(0);

	return obj_elements.size();
}

void drawObj(GLboolean useMaterial)  {
	GLuint offset = 0;
	GLuint isize = 0;

	glBindVertexArray(objVAOHandle);
	for (GLuint i = 0; i < Loader.LoadedMeshes.size(); i++)
	{
		if (useMaterial && Loader.LoadedMeshes[i].hasMaterial)
		{
			objl::Material material = Loader.LoadedMeshes[i].MeshMaterial;

			glUniform3f(locUniformMaterialAmbient, material.Ka.X, material.Ka.Y, material.Ka.Z);
			glUniform3f(locUniformMaterialDiffuse, material.Kd.X, material.Kd.Y, material.Kd.Z);
			glUniform3f(locUniformMaterialSpecular, material.Ks.X, material.Ks.Y, material.Ks.Z);
			glUniform1f(locUniformMaterialShininess, material.Ns);
		}
		
		isize = Loader.LoadedMeshes[i].Indices.size();
		glDrawElements(GL_TRIANGLES, isize, GL_UNSIGNED_INT, ((GLuint *)NULL + (offset)));
		offset += isize;
	}
	glBindVertexArray(0);
	return;
}

// END: Support for OBJ models /////////////////////////////////////////////////////////////////////////////////

// BEGIN: Load shaders ////////////////////////////////////////////////////////////////////////////////////////////

void loadSource(GLuint &shaderID, std::string name) 
{
	std::ifstream f(name.c_str());
	if (!f.is_open()) 
	{
		std::cerr << "File not found " << name.c_str() << std::endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	// now read in the data
	std::string *source;
	source = new std::string( std::istreambuf_iterator<GLchar>(f),   
						std::istreambuf_iterator<GLchar>() );
	f.close();
   
	// add a null to the string
	*source += "\0";
	const GLchar * data = source->c_str();
	glShaderSource(shaderID, 1, &data, NULL);
	delete source;
	return;
}

void printCompileInfoLog(GLuint shadID) 
{
GLint compiled;
	glGetShaderiv( shadID, GL_COMPILE_STATUS, &compiled );
	if (compiled == GL_FALSE)
	{
		GLint infoLength = 0;
		glGetShaderiv( shadID, GL_INFO_LOG_LENGTH, &infoLength );

		GLchar *infoLog = new GLchar[infoLength];
		GLint chsWritten = 0;
		glGetShaderInfoLog( shadID, infoLength, &chsWritten, infoLog );

		std::cerr << "Shader compiling failed:" << infoLog << std::endl;
		system("pause");
		delete [] infoLog;

		exit(EXIT_FAILURE);
	}
	return;
}

void printLinkInfoLog(GLuint programID)
{
GLint linked;
	glGetProgramiv( programID, GL_LINK_STATUS, &linked );
	if(! linked)
	{
		GLint infoLength = 0;
		glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &infoLength );

		GLchar *infoLog = new GLchar[infoLength];
		GLint chsWritten = 0;
		glGetProgramInfoLog( programID, infoLength, &chsWritten, infoLog );

		std::cerr << "Shader linking failed:" << infoLog << std::endl;
		system("pause");
		delete [] infoLog;

		exit(EXIT_FAILURE);
	}
	return;
}

void validateProgram(GLuint programID)
{
GLint status;
	glValidateProgram( programID );
	glGetProgramiv( programID, GL_VALIDATE_STATUS, &status );

	if( status == GL_FALSE ) 
	{
		GLint infoLength = 0;
		glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &infoLength );

		if( infoLength > 0 ) 
		{
			GLchar *infoLog = new GLchar[infoLength];
			GLint chsWritten = 0;
			glGetProgramInfoLog( programID, infoLength, &chsWritten, infoLog );
			std::cerr << "Program validating failed:" << infoLog << std::endl;
			system("pause");
			delete [] infoLog;

			exit(EXIT_FAILURE);
		}
	}
	return;
}

// END: Load shaders ////////////////////////////////////////////////////////////////////////////////////////////

// BEGIN: Initialize primitives ////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Init for drawCube_VAO
///////////////////////////////////////////////////////////////////////////////
void initCube() 
{
GLuint vboHandle;

	glGenVertexArrays (1, &cubeVAOHandle);
	glBindVertexArray (cubeVAOHandle);

	glGenBuffers(1, &vboHandle); 
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle);    
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1) + sizeof(normales1), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices1), vertices1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices1), sizeof(normales1), normales1);

	GLuint loc = glGetAttribLocation(programID, "aPosition");   
	glEnableVertexAttribArray(loc); 
	glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, (GLchar *)NULL + 0 ); 
	GLuint loc2 = glGetAttribLocation(programID, "aNormal"); 
	glEnableVertexAttribArray(loc2); 
	glVertexAttribPointer( loc2, 3, GL_FLOAT, GL_FALSE, 0, (GLchar *)NULL + sizeof(vertices1) );

	glBindVertexArray (0);
	return;
}

///////////////////////////////////////////////////////////////////////////////
// Init Sphere
// parameters:
// 		radius - radius of the sphere
// 		rings - number of parallel rings
// 		sectors - number of ring divisions
// return:
// 		number of vertices
///////////////////////////////////////////////////////////////////////////////
GLint initSphere(GLfloat radius, GLuint rings, GLuint sectors)
{
	const GLfloat R = 1.0f/(GLfloat)(rings-1);
	const GLfloat S = 1.0f/(GLfloat)(sectors-1);
	const double PI = 3.14159265358979323846;

	GLfloat *sphere_vertices = new GLfloat[rings * sectors * 3];
	GLfloat *sphere_normals = new GLfloat[rings * sectors * 3];
	GLfloat *v = sphere_vertices;
	GLfloat *n = sphere_normals;
	for(GLuint r = 0; r < rings; r++) for(GLuint s = 0; s < sectors; s++) {
			GLfloat const y = GLfloat( sin( -PI/2 + PI * r * R ) );
			GLfloat const x = GLfloat( cos(2*PI * s * S) * sin( PI * r * R ) );
			GLfloat const z = GLfloat( sin(2*PI * s * S) * sin( PI * r * R ) );

			*v++ = x * radius;
			*v++ = y * radius;
			*v++ = z * radius;

			*n++ = x;
			*n++ = y;
			*n++ = z;
	}

	GLushort *sphere_indices = new GLushort[rings * sectors * 4];
	GLushort *i = sphere_indices;
	for(GLuint r = 0; r < rings; r++) for(GLuint s = 0; s < sectors; s++) {
			*i++ = r * sectors + s;
			*i++ = r * sectors + (s+1);
			*i++ = (r+1) * sectors + (s+1);
			*i++ = (r+1) * sectors + s;
	}

	glGenVertexArrays( 1, &sphereVAOHandle );
	glBindVertexArray(sphereVAOHandle);

	GLuint handle[3];
	glGenBuffers(3, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, (rings * sectors * 3) * sizeof(GLfloat), sphere_vertices, GL_STATIC_DRAW);
	GLuint loc1 = glGetAttribLocation(programID, "aPosition");   
	glEnableVertexAttribArray(loc1); // Vertex position
	glVertexAttribPointer( loc1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL + 0 ); 

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, (rings * sectors * 3) * sizeof(GLfloat), sphere_normals, GL_STATIC_DRAW);
	GLuint loc2 = glGetAttribLocation(programID, "aNormal");   
	glEnableVertexAttribArray(loc2); // Vertex normal
	glVertexAttribPointer( loc2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL + 0 ); 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, rings * sectors * 4 * sizeof(GLushort), sphere_indices, GL_STATIC_DRAW); 

	delete [] sphere_vertices;
	delete [] sphere_normals;
	delete [] sphere_indices;

	glBindVertexArray(0);

	return rings * sectors * 4;
}


///////////////////////////////////////////////////////////////////////////////
// Init Teapot
// parameters:
// 		grid - number of grids
// 		transform - model transformation matrix
// return:
// 		number of vertices
///////////////////////////////////////////////////////////////////////////////
GLint initTeapot(GLint grid, glm::mat4 transform)
{
	GLint verts = 32 * (grid + 1) * (grid + 1);
	GLint faces = grid * grid * 32;
	GLfloat * v = new GLfloat[ verts * 3 ];
	GLfloat * n = new GLfloat[ verts * 3 ];
	GLfloat * tc = new GLfloat[verts * 2];
	GLuint * el = new GLuint[faces * 6];

	generatePatches( v, n, tc, el, grid );
	moveLid(grid, v, transform);

	glGenVertexArrays( 1, &teapotVAOHandle );
	glBindVertexArray(teapotVAOHandle);

	GLuint handle[3];
	glGenBuffers(3, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, (3 * verts) * sizeof(GLfloat), v, GL_STATIC_DRAW);
	GLuint loc1 = glGetAttribLocation(programID, "aPosition");   
	glVertexAttribPointer( loc1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
	glEnableVertexAttribArray(loc1);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, (3 * verts) * sizeof(GLfloat), n, GL_STATIC_DRAW);
	GLuint loc2 = glGetAttribLocation(programID, "aNormal");   
	glVertexAttribPointer( loc2, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
	glEnableVertexAttribArray(loc2);  // Vertex normal

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * faces * sizeof(GLuint), el, GL_STATIC_DRAW);

	delete [] v;
	delete [] n;
	delete [] tc;
	delete [] el;

	glBindVertexArray(0);

	return 6 * faces;
}

// END: Initialize primitives ////////////////////////////////////////////////////////////////////////////////////

// BEGIN: Drawing Features ////////////////////////////////////////////////////////////////////////////////////

void drawCube()
{
	glBindVertexArray(cubeVAOHandle);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	return;
}

void drawTeapot()  {
	glBindVertexArray(teapotVAOHandle);
	glDrawElements(GL_TRIANGLES, numVertTeapot, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
	glBindVertexArray(0);
	return;
}

void drawSphere()  {
	glBindVertexArray(sphereVAOHandle);
	glDrawElements(GL_QUADS, numVertSphere, GL_UNSIGNED_SHORT, ((GLubyte *)NULL + (0)));
	glBindVertexArray(0);
	return;
}

// END: Drawing Features ////////////////////////////////////////////////////////////////////////////////////

GLint main(GLint argc, GLchar *argv[])
{
	glutInit(&argc, argv); 
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(g_Width, g_Height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Test 9");
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
	  system("pause");
	  exit(-1);
	}
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);
 
	glutMainLoop();
 
	return EXIT_SUCCESS;
}

GLboolean init()
{
	glClearColor(0.93f, 0.93f, 0.93f, 0.0f);
 
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearDepth(1.0f);

	glShadeModel(GL_SMOOTH);

	programID = glCreateProgram();

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	loadSource(vertexShaderID, "../Shaders/main.vert");
	std::cout << "Compiling vertex shader ..." << std::endl;
	glCompileShader(vertexShaderID);
	printCompileInfoLog(vertexShaderID);
	glAttachShader(programID, vertexShaderID);

	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	loadSource(fragmentShaderID, "../Shaders/main.frag");
	std::cout << "Compiling fragment shader ..." << std::endl;
	glCompileShader(fragmentShaderID);
	printCompileInfoLog(fragmentShaderID);
	glAttachShader(programID, fragmentShaderID);

	std::cout << "Linking program ..." << std::endl;
	glLinkProgram(programID);
	printLinkInfoLog(programID);
	validateProgram(programID);

	initCube();

	// uncomment the following line to draw the dragon
	//numVertObj = initObj("modelos_obj/chinese_parade_dragon.obj");

	numVertTeapot = initTeapot(10, glm::mat4(1.0f));
	numVertSphere = initSphere(1.0f, 30, 60);

	locUniformMVPM = glGetUniformLocation(programID, "uModelViewProjMatrix");
	locUniformMM = glGetUniformLocation(programID, "uModelMatrix"); 
	locUniformNM = glGetUniformLocation(programID, "uNormalMatrix");
	locUniformDrawEnvironment = glGetUniformLocation(programID, "uDrawEnvironment");
	locUniformCameraPos = glGetUniformLocation(programID, "uCameraPos");

	locUniformMR = glGetUniformLocation(programID, "uRedMatrix");
	locUniformMG = glGetUniformLocation(programID, "uGreenMatrix");
	locUniformMB = glGetUniformLocation(programID, "uBlueMatrix");

	locUniformLightDir = glGetUniformLocation(programID, "uLight.lightDir");
	locUniformLightIntensity = glGetUniformLocation(programID, "uLight.intensity");
	locUniformMaterialAmbient = glGetUniformLocation(programID, "uMaterial.ambient");
	locUniformMaterialDiffuse = glGetUniformLocation(programID, "uMaterial.diffuse");
	locUniformMaterialSpecular = glGetUniformLocation(programID, "uMaterial.specular");
	locUniformMaterialShininess = glGetUniformLocation(programID, "uMaterial.shininess");

	locUniformAmbientLight = glGetUniformLocation(programID, "uAmbientLight");

	locUniformSpecularActivated = glGetUniformLocation(programID, "uSpecularActivated"); 
	locUniformAmbientActivated = glGetUniformLocation(programID, "uAmbientActivated");
	locUniformHemisActivated = glGetUniformLocation(programID, "uHemisActivated");
	locUniformArmonicActivated = glGetUniformLocation(programID, "uArmonicActivated");
	locUniformImageActivated = glGetUniformLocation(programID, "uImageActivated");

	// locate the rest of the uniform variables.
	locUniformSkyColor = glGetUniformLocation(programID, "uSkyColor");
	locUniformGroundColor = glGetUniformLocation(programID, "uGroundColor");

	locUniformEnvironmentMap = glGetUniformLocation(programID, "uEnvironmentMap");
	locUniformSpecularMap = glGetUniformLocation(programID, "uSpecularMap");
	locUniformDiffuseMap = glGetUniformLocation(programID, "uDiffuseMap");


	// Initialize Textures
	std::vector<GLubyte> img_data;
	GLuint img_width, img_height;
	GLuint error;

	const GLchar* mapa_entorno_px = "texturas/grace_cubemap_env_posx.png";
	const GLchar* mapa_entorno_nx = "texturas/grace_cubemap_env_negx.png";
	const GLchar* mapa_entorno_py = "texturas/grace_cubemap_env_posy.png";
	const GLchar* mapa_entorno_ny = "texturas/grace_cubemap_env_negy.png";
	const GLchar* mapa_entorno_pz = "texturas/grace_cubemap_env_posz.png";
	const GLchar* mapa_entorno_nz = "texturas/grace_cubemap_env_negz.png";

	const GLchar* mapa_especular_px = "texturas/grace_cubemap_specular_posx.png";
	const GLchar* mapa_especular_nx = "texturas/grace_cubemap_specular_negx.png";
	const GLchar* mapa_especular_py = "texturas/grace_cubemap_specular_posy.png";
	const GLchar* mapa_especular_ny = "texturas/grace_cubemap_specular_negy.png";
	const GLchar* mapa_especular_pz = "texturas/grace_cubemap_specular_posz.png";
	const GLchar* mapa_especular_nz = "texturas/grace_cubemap_specular_negz.png";

	const GLchar* mapa_irradiacion_px = "texturas/grace_cubemap_diffuse_posx.png";
	const GLchar* mapa_irradiacion_nx = "texturas/grace_cubemap_diffuse_negx.png";
	const GLchar* mapa_irradiacion_py = "texturas/grace_cubemap_diffuse_posy.png";
	const GLchar* mapa_irradiacion_ny = "texturas/grace_cubemap_diffuse_negy.png";
	const GLchar* mapa_irradiacion_pz = "texturas/grace_cubemap_diffuse_posz.png";
	const GLchar* mapa_irradiacion_nz = "texturas/grace_cubemap_diffuse_negz.png";

	glGenTextures (3, textIds);


	// Environment map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textIds[0]);

	error = lodepng::decode(img_data, img_width, img_height, mapa_entorno_px);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_entorno_px << std::endl;
	img_data.clear();	
	error = lodepng::decode(img_data, img_width, img_height, mapa_entorno_py);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_entorno_py << std::endl;
	img_data.clear();
	error = lodepng::decode(img_data, img_width, img_height, mapa_entorno_pz);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_entorno_pz << std::endl;
	img_data.clear();
	error = lodepng::decode(img_data, img_width, img_height, mapa_entorno_nx);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_entorno_nx << std::endl;
	img_data.clear();
	error = lodepng::decode(img_data, img_width, img_height, mapa_entorno_ny);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_entorno_ny << std::endl;
	img_data.clear();
	error = lodepng::decode(img_data, img_width, img_height, mapa_entorno_nz);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_entorno_nz << std::endl;
	img_data.clear();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Do the same for the specular map and the irradiation map.
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textIds[1]);

	error = lodepng::decode(img_data, img_width, img_height, mapa_especular_px);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_especular_px << std::endl;
	img_data.clear();
	error = lodepng::decode(img_data, img_width, img_height, mapa_especular_py);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_especular_py << std::endl;
	img_data.clear();
	error = lodepng::decode(img_data, img_width, img_height, mapa_especular_pz);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_especular_pz << std::endl;
	img_data.clear();
	error = lodepng::decode(img_data, img_width, img_height, mapa_especular_nx);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_especular_nx << std::endl;
	img_data.clear();
	error = lodepng::decode(img_data, img_width, img_height, mapa_especular_ny);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_especular_ny << std::endl;
	img_data.clear();
	error = lodepng::decode(img_data, img_width, img_height, mapa_especular_nz);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_especular_nz << std::endl;
	img_data.clear();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textIds[2]);

	error = lodepng::decode(img_data, img_width, img_height, mapa_irradiacion_px);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_irradiacion_px << std::endl;
	img_data.clear();
	error = lodepng::decode(img_data, img_width, img_height, mapa_irradiacion_py);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_irradiacion_py << std::endl;
	img_data.clear();
	error = lodepng::decode(img_data, img_width, img_height, mapa_irradiacion_pz);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_irradiacion_pz << std::endl;
	img_data.clear();
	error = lodepng::decode(img_data, img_width, img_height, mapa_irradiacion_nx);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_irradiacion_nx << std::endl;
	img_data.clear();
	error = lodepng::decode(img_data, img_width, img_height, mapa_irradiacion_ny);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_irradiacion_ny << std::endl;
	img_data.clear();
	error = lodepng::decode(img_data, img_width, img_height, mapa_irradiacion_nz);
	if (!error)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << mapa_irradiacion_nz << std::endl;
	img_data.clear();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return true;
}
 
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	struct LightInfo {
	 glm::vec3 lightDir;
	 glm::vec3 intensity;
	};
	// Grace Cathedral LightInfo
	LightInfo light = { glm::vec3(-1.0, 1.0f, -1.0f),
						glm::vec3(1.0,0.45,0.5), 
	};

	struct MaterialInfo {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		GLfloat shininess;
	};
	MaterialInfo perl = { glm::vec3(0.25f, 0.20725f, 0.20725f) * 1.5f, glm::vec3(1.0f, 0.829f, 0.829f) * 1.1f, glm::vec3(0.3f, 0.3f, 0.3f), 20.0f };
	MaterialInfo white = { glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.3f, 0.3f, 0.3f), 20.0f };


	// Grace Cathedral - Ambient Light
	glm::vec3 ambientLight = glm::vec3(0.64, 0.39, 0.52);

	// Grace Cathedral - Hemisphere Light (Sky and Ground Color)
	glm::vec3 groundLight = glm::vec3(0.578125, 0.1953126, 0.0625);
	glm::vec3 skyLight = glm::vec3(1.0, 0.6745098, 1.0);

	// Elements of M (spherical harmonics) for Grace Cathedral
	const glm::mat4 Mred = glm::mat4(0.090099, -0.0471947, 0.240264, -0.148383, -0.0471947, -0.090099, -0.111551, 0.199549, 0.240264, -0.111551, -0.1189, -0.173966, -0.148383, 0.199549, -0.173966, 0.739753);
	const glm::mat4 Mgreen = glm::mat4(-0.0214521, -0.0214521, 0.090099, -0.0306998, -0.0214521, 0.0214521, -0.0943895, 0.179082, 0.090099, -0.0943895, -0.0668813, -0.0920995, -0.0306998, 0.179082, -0.0920995, 0.412234);
	const glm::mat4 Mblue = glm::mat4(-0.128713, -0.0514852, 0.060066, 0.00511664, -0.0514852, 0.128713, -0.20165, 0.306998, 0.060066, -0.20165, -0.111469, -0.138149, 0.00511664, 0.306998, -0.138149, 0.515719);


	glm::mat4 Projection = glm::perspective(45.0f, 1.0f * g_Width / g_Height, 1.0f, 100.0f);
	
	glm::vec3 cameraPos = vec3( 7.0f * cos( yrot / 100 ), 2.0f * sin(xrot / 100) + 1.0f, 7.0f * sin( yrot / 100 ) * cos(xrot /100) );
	glm::mat4 View = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	glm::mat4 ModelSky = glm::scale(glm::mat4(1.0), glm::vec3(25.0f, 25.0f, 25.0f));
	
	// Model Matrix for Chinese_parade_dragon
	glm::mat4 ModelObj = glm::translate(glm::rotate(glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)), -90.0f, vec3(1.0, 0.0, 0.0)), 90.0f, vec3(0.0, 0.0, 1.0)), vec3(20.0f, 0.0f, -10.0f));
	
	glm::mat4 ModelSphere = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)), vec3(4.0f, 0.0f, 2.0f));
	glm::mat4 ModelTeapot = glm::translate(glm::rotate(glm::scale(glm::mat4(1.0f),vec3(0.25, 0.25, 0.25)), -90.0f, vec3(1.0, 0.0, 0.0)), vec3(4.0f, 0.0f, 2.0f));

	glm::mat4 mvp;
	glm::mat3 nm;

	glUseProgram(programID);

	glUniform3fv(locUniformLightDir, 1, &(light.lightDir.x));
	glUniform3fv(locUniformLightIntensity, 1, &(light.intensity.r));

	glUniform3fv(locUniformAmbientLight, 1, &(ambientLight.r));
	glUniform3f (locUniformCameraPos, cameraPos.x, cameraPos.y, cameraPos.z);

	glUniform1i(locUniformEnvironmentMap, 0);
	glUniform1i(locUniformSpecularMap, 1);
	glUniform1i(locUniformDiffuseMap, 2);

	glUniform1i(locUniformSpecularActivated, GLuint(specularActivated)); 
	glUniform1i(locUniformAmbientActivated, GLuint(ambientActivated)); 
	glUniform1i(locUniformArmonicActivated, GLuint(armonicActivated));
	glUniform1i(locUniformHemisActivated, GLuint(hemisActivated));
	glUniform1i(locUniformImageActivated, GLuint(imageActivated));


	// pass the values to the rest of the uniform variables.
	glUniform3fv(locUniformSkyColor, 1, &skyLight.x);
	glUniform3fv(locUniformGroundColor, 1, &groundLight.x);

	glUniformMatrix4fv(locUniformMR, 1, GL_FALSE, &Mred[0][0]);
	glUniformMatrix4fv(locUniformMG, 1, GL_FALSE, &Mgreen[0][0]);
	glUniformMatrix4fv(locUniformMB, 1, GL_FALSE, &Mblue[0][0]);



	// Draw the environment
	mvp = Projection * View * ModelSky;
	nm = glm::mat3(glm::transpose(glm::inverse(ModelSky)));
	glUniformMatrix4fv( locUniformMVPM, 1, GL_FALSE, &mvp[0][0] );
	glUniformMatrix3fv( locUniformNM, 1, GL_FALSE, &nm[0][0] );
	glUniformMatrix4fv( locUniformMM, 1, GL_FALSE, &ModelSky[0][0] );
	glUniform1i (locUniformDrawEnvironment, 1);
	drawCube();

	glUniform1i(locUniformDrawEnvironment, 0);
	
	// Draw Sphere
	mvp = Projection * View * ModelSphere;
	nm = glm::mat3(glm::transpose(glm::inverse(ModelSphere)));
	glUniformMatrix4fv( locUniformMVPM, 1, GL_FALSE, &mvp[0][0] );
	glUniformMatrix3fv( locUniformNM, 1, GL_FALSE, &nm[0][0] );
	glUniformMatrix4fv( locUniformMM, 1, GL_FALSE, &ModelSphere[0][0] );
	glUniform3fv(locUniformMaterialAmbient, 1, &(perl.ambient.r));
	glUniform3fv(locUniformMaterialDiffuse, 1, &(perl.diffuse.r));
	glUniform3fv(locUniformMaterialSpecular, 1, &(perl.specular.r));
	glUniform1f(locUniformMaterialShininess, perl.shininess);
	drawSphere();

	// Draw Teapot
	mvp = Projection * View * ModelTeapot;
	nm = glm::mat3(glm::transpose(glm::inverse(ModelTeapot)));
	glUniformMatrix4fv( locUniformMVPM, 1, GL_FALSE, &mvp[0][0] );
	glUniformMatrix3fv( locUniformNM, 1, GL_FALSE, &nm[0][0] );
	glUniformMatrix4fv( locUniformMM, 1, GL_FALSE, &ModelTeapot[0][0] );
	glUniform3fv(locUniformMaterialAmbient, 1, &(white.ambient.r));
	glUniform3fv(locUniformMaterialDiffuse, 1, &(white.diffuse.r));
	glUniform3fv(locUniformMaterialSpecular, 1, &(white.specular.r));
	glUniform1f(locUniformMaterialShininess, white.shininess);
	drawTeapot();

	// uncomment the following set of lines to draw the dragon
	// Dibuja Obj
	/*
	mvp = Projection * View * ModelObj;
	nm = glm::mat3(glm::transpose(glm::inverse(ModelObj)));
	glUniformMatrix4fv( locUniformMVPM, 1, GL_FALSE, &mvp[0][0] );
	glUniformMatrix3fv( locUniformNM, 1, GL_FALSE, &nm[0][0] );
	glUniformMatrix4fv( locUniformMM, 1, GL_FALSE, &ModelObj[0][0] );
	glUniform3fv(locUniformMaterialAmbient, 1, &(white.ambient.r));
	glUniform3fv(locUniformMaterialDiffuse, 1, &(white.diffuse.r));
	glUniform3fv(locUniformMaterialSpecular, 1, &(white.specular.r));
	glUniform1f(locUniformMaterialShininess, white.shininess);
	drawObj(false);
	*/

	glUseProgram(0);

	printFPS();
	glutSwapBuffers();
	return;
}
 
void resize(GLint w, GLint h)
{
	g_Width = w;
	g_Height = h;
	glViewport(0, 0, g_Width, g_Height);
	return;
}
 
void idle()
{
	if (!mouseDown && animation)
	{
		xrot += 0.3f;
		yrot += 0.4f;
	}
	glutPostRedisplay();
	return;
}
 
void keyboard(GLubyte key, GLint x, GLint y)
{
	switch (key)
	{
	case 27: case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 'a': case 'A':
		ambientActivated = !ambientActivated;
		armonicActivated = 0;
		imageActivated = false;
		break;
	case 'e': case 'E':
		specularActivated = !specularActivated;
		break;
	case 'h': case 'H':
		hemisActivated = !hemisActivated;
		armonicActivated = 0;
		imageActivated = false;
		break;
	case 's': case 'S':
		armonicActivated = (armonicActivated + 1) % 3;
		imageActivated = false;
		//hemisActivated = false;
		ambientActivated = false;
		break;
	case 'i': case 'I':
		imageActivated = !imageActivated;
		armonicActivated = 0;
		//hemisActivated = false;
		ambientActivated = false;
		break;
	}
	return;
}
 
void specialKeyboard(GLint key, GLint x, GLint y)
{
	if (key == GLUT_KEY_F1)
	{
		fullscreen = !fullscreen;
 
		if (fullscreen)
			glutFullScreen();
		else
		{
			glutReshapeWindow(g_Width, g_Height);
			glutPositionWindow(50, 50);
		}
	}
	return;
}
 
void mouse(GLint button, GLint state, GLint x, GLint y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = true;
 
		xdiff = x - yrot;
		ydiff = -y + xrot;
	}
	else
		mouseDown = false;
	return;
}
 
void mouseMotion(GLint x, GLint y)
{
	if (mouseDown)
	{
		yrot = x - xdiff;
		xrot = y + ydiff;
 
		glutPostRedisplay();
	}
	return;
}

void printFPS()
{
	static GLint frameCount = 0;
	static GLfloat fps = 0;
	static GLint currentTime = 0, previousTime = 0;

	frameCount++;

	currentTime = glutGet(GLUT_ELAPSED_TIME);

	GLint timeInterval = currentTime - previousTime;

	if (timeInterval > 1000)
	{
		fps = frameCount / (timeInterval / 1000.0f);

		previousTime = currentTime;

		frameCount = 0;

		std::cout.precision(4);
		std::cout << "FPS: " << fps << std::endl;
	}
	return;
}