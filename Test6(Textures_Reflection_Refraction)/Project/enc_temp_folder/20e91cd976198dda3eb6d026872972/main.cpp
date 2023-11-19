#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vboteapot.h"
#include "teapotdata.h"
#include "lodepng.h"


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

GLboolean fullscreen = false;
GLboolean mouseDown = false;
GLboolean animation = false;
GLboolean dispersion = false;
GLboolean Fvalue = false;
GLfloat fresnelValue = 0.0;

GLint shaderAUsar = 0;

 
GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat xdiff = 0.0f;
GLfloat ydiff = 0.0f;

GLint g_Width = 500;
GLint g_Height = 500;

GLuint cubeVAOHandle;
GLuint sphereVAOHandle;
GLuint teapotVAOHandle;
GLuint offsetExtra = 0;
GLuint* programIDs = new GLuint[5];
GLuint locUniformStone[5], locUniformMoss[5], locUniformMap[5];
GLuint locUniformMVPM[5], locUniformMM[5], locUniformNM[5];
GLuint locUniformDrawSky[5], locUniformCamera[5], locUniformRatio[3], locUniformRatioR[3], locUniformRatioG[3], locUniformRatioB[3], locUniformFresnel[3], locUniformIsFresnel[3];
GLuint locUniformTam[2], locUniformToModel[2];

int numVertTeapot, numVertSphere;

GLuint textIds[3];


// cubo ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3

// Vertex array coordinates ==================================================
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

GLfloat texCoord1[] = { 1, 1,	0, 1,	0, 0,		// v0-v1-v2 (front)
						0, 0,	1, 0,	1, 1,		// v2-v3-v0

						1, 1,   1, 0,   0, 0,		// v0-v3-v4 (right)
						0, 0,   0, 1,   1, 1,		// v4-v5-v0

						1, 0,   1, 1,	0, 1,		// v0-v5-v6 (top)
						0, 1,	0, 0,	1, 0,		// v6-v1-v0

						0, 1,	1, 1,	1, 0,		// v1-v6-v7 (left)
						1, 0,	0, 0,	0, 1,		// v7-v2-v1

						0, 0,   1, 0,   1, 1,		// v7-v4-v3 (bottom)
						1, 1,	0, 1,	0, 0,		// v3-v2-v7

						0, 0,	1, 0,	1, 1,		// v4-v7-v6 (back)
						1, 1,	0, 1,   0, 0 };		// v6-v5-v4

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

// END:   Load shaders ////////////////////////////////////////////////////////////////////////////////////////////

// BEGIN: Initialize primitives ////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Init for drawCube_VAO
///////////////////////////////////////////////////////////////////////////////
void initCube( GLint i = 0) 
{
GLuint vboHandle;

	glGenVertexArrays (1, &cubeVAOHandle);
	glBindVertexArray (cubeVAOHandle);

	glGenBuffers(1, &vboHandle); 
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle);    
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1) + sizeof(normales1) + sizeof(texCoord1), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices1), vertices1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices1), sizeof(normales1), normales1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices1) + sizeof(normales1), sizeof(texCoord1), texCoord1);

	// Locate the in variables of the vertex shader for the position
	// the normal and texture coordinates, enable these attributes and set the pointers
	GLuint locCub1 = glGetAttribLocation(programIDs[i], "aPosition");
	GLuint locCub2 = glGetAttribLocation(programIDs[i], "aNormal");
	GLuint locCub3 = glGetAttribLocation(programIDs[i], "aTexCoord");

	glEnableVertexAttribArray(locCub1);
	glEnableVertexAttribArray(locCub2);
	glEnableVertexAttribArray(locCub3);

	glVertexAttribPointer(locCub1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(locCub2, 3, GL_FLOAT, GL_FALSE, 0, (char*)NULL + sizeof(vertices1));
	glVertexAttribPointer(locCub3, 2, GL_FLOAT, GL_FALSE, 0, (char*)NULL + sizeof(vertices1) + sizeof(normales1));

	glBindVertexArray (0);
	return;
}

///////////////////////////////////////////////////////////////////////////////
// Init Teapot
// parameters:
// 		grid - number of grids
// 		transform - model transformation matrix
// return:
// 		number of vertices
///////////////////////////////////////////////////////////////////////////////
GLint initTeapot(GLint grid, glm::mat4 transform, GLint i = 0)
{
	GLint verts = 32 * (grid + 1) * (grid + 1);
	GLint faces = grid * grid * 32;
	GLfloat * v = new GLfloat[ verts * 3 ];
	GLfloat * n = new GLfloat[ verts * 3 ];
	GLfloat * tc = new GLfloat[ verts * 2 ];
	GLuint * el = new GLuint[faces * 6];

	generatePatches( v, n, tc, el, grid );
	moveLid(grid, v, transform);

	glGenVertexArrays( 1, &teapotVAOHandle );
	glBindVertexArray(teapotVAOHandle);

	GLuint handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, (3 * verts) * sizeof(float), v, GL_STATIC_DRAW);
	
	GLuint loc1 = glGetAttribLocation(programIDs[i], "aPosition");
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
	glEnableVertexAttribArray(loc1);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, (3 * verts) * sizeof(float), n, GL_STATIC_DRAW);
	
	GLuint loc2 = glGetAttribLocation(programIDs[i], "aNormal");
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
	glEnableVertexAttribArray(loc2);  // Vertex normal

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, (2 * verts) * sizeof(float), tc, GL_STATIC_DRAW);
	
	GLuint loc3 = glGetAttribLocation(programIDs[i], "aTexCoord");
	glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
	glEnableVertexAttribArray(loc3);  // texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * faces * sizeof(unsigned int), el, GL_STATIC_DRAW);

	delete [] v;
	delete [] n;
	delete [] el;
	delete [] tc;

	glBindVertexArray(0);

	return 6 * faces;
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
GLint initSphere(GLfloat radius, GLuint rings, GLuint sectors, GLint shader = 0)
{
	const GLfloat R = 1.0f/(GLfloat)(rings-1);
	const GLfloat S = 1.0f/(GLfloat)(sectors-1);
	const GLdouble PI = 3.14159265358979323846;

	GLfloat *sphere_vertices = new GLfloat[rings * sectors * 3];
	GLfloat *sphere_normals = new GLfloat[rings * sectors * 3];
	GLfloat *sphere_texcoords = new GLfloat[rings * sectors * 2];
	GLfloat *v = sphere_vertices;
	GLfloat *n = sphere_normals;
	GLfloat *t = sphere_texcoords;
	for(GLuint r = 0; r < rings; r++) for(GLuint s = 0; s < sectors; s++) {
			GLfloat const y = GLfloat( sin( -PI/2 + PI * r * R ) );
			GLfloat const x = GLfloat( cos(2*PI * s * S) * sin( PI * r * R ) );
			GLfloat const z = GLfloat( sin(2*PI * s * S) * sin( PI * r * R ) );

			*t++ = s*S;
			*t++ = r*R;

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

	GLuint handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, (rings * sectors * 3) * sizeof(GLfloat), sphere_vertices, GL_STATIC_DRAW);
	// Locate the variable in of the vertex shader for the position,
	// enable vertex arrays for the attribute and set the pointer
	GLuint locSph1 = glGetAttribLocation(programIDs[shader], "aPosition");
	glEnableVertexAttribArray(locSph1);
	glVertexAttribPointer(locSph1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, (rings * sectors * 3) * sizeof(GLfloat), sphere_normals, GL_STATIC_DRAW);
	// ditto for the normal one.
	GLuint locSph2 = glGetAttribLocation(programIDs[shader], "aNormal");
	glEnableVertexAttribArray(locSph2);
	glVertexAttribPointer(locSph2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, (rings * sectors * 2) * sizeof(float), sphere_texcoords, GL_STATIC_DRAW);
	// ditto for the texture coordinates.
	GLuint locSph3 = glGetAttribLocation(programIDs[shader], "aTexCoord");
	glEnableVertexAttribArray(locSph3);
	glVertexAttribPointer(locSph3, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, rings * sectors * 4 * sizeof(GLushort), sphere_indices, GL_STATIC_DRAW);

	delete [] sphere_vertices;
	delete [] sphere_normals;
	delete [] sphere_texcoords;
	delete [] sphere_indices;

	glBindVertexArray(0);

	return rings * sectors * 4;
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

int main(int argc, char *argv[])
{
	glutInit(&argc, argv); 
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(g_Width, g_Height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Test 6");
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

	const GLchar* ShaderVerts[5] = {"../Shaders/mix.vert", "../Shaders/reflejo.vert", "../Shaders/trans.vert", "../Shaders/transCube.vert", "../Shaders/transSphere.vert" };
	const GLchar* ShaderFrags[5] = {"../Shaders/mix.frag", "../Shaders/reflejo.frag", "../Shaders/trans.frag", "../Shaders/transCube.frag", "../Shaders/transSphere.frag" };

	// program 1 :::: texture mixing
	for (GLuint i = 0; i < 5; ++i) {
		programIDs[i] = glCreateProgram();

		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		loadSource(vertexShaderID, ShaderVerts[i]);
		glCompileShader(vertexShaderID);
		printCompileInfoLog(vertexShaderID);
		glAttachShader(programIDs[i], vertexShaderID);

		std::cout << ShaderVerts[i] << std::endl;

		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		loadSource(fragmentShaderID, ShaderFrags[i]);
		glCompileShader(fragmentShaderID);
		printCompileInfoLog(fragmentShaderID);
		glAttachShader(programIDs[i], fragmentShaderID);

		std::cout << ShaderFrags[i] << std::endl;

		glLinkProgram(programIDs[i]);
		printLinkInfoLog(programIDs[i]);
	}

	
	initCube(0);
	
	numVertTeapot = initTeapot(10, glm::mat4(1.0f),0);
	numVertSphere = initSphere(1.0f, 30, 60, 0);

	// Inicializa Texturas
	std::vector<GLubyte> img_data;
	std::vector<GLubyte> img_px, img_nx, img_py, img_ny, img_pz, img_nz;
	GLuint img_width, img_height;
	GLuint error;

	const GLchar* img_filename_tex1 = "../Textures/stone.png";
	const GLchar* img_filename_tex2 = "../Textures/moss.png";

	const GLchar* img_filename_px = "../Textures/t1posx.png";
	const GLchar* img_filename_nx = "../Textures/t1negx.png";
	const GLchar* img_filename_py = "../Textures/t1posy.png";
	const GLchar* img_filename_ny = "../Textures/t1negy.png";
	const GLchar* img_filename_pz = "../Textures/t1posz.png";
	const GLchar* img_filename_nz = "../Textures/t1negz.png";

	glGenTextures (3, textIds);
	
	// Textura piedra
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textIds[0]);

	error = lodepng::decode(img_data, img_width, img_height, img_filename_tex1);
	if (!error)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << img_filename_tex1 << std::endl;
	img_data.clear();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	// Moss texture
	// Set the texture unit, activate the texture object, load the image data into it, and define the texture parameters
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textIds[1]);

	error = lodepng::decode(img_data, img_width, img_height, img_filename_tex2);
	if (!error)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
	else std::cout << "Error al cargar la textura " << img_filename_tex2 << std::endl;
	img_data.clear();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Environment map
	// ditto for the environment map
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textIds[2]);

	error = lodepng::decode(img_px, img_width, img_height, img_filename_px);
	error = lodepng::decode(img_nx, img_width, img_height, img_filename_nx);
	error = lodepng::decode(img_py, img_width, img_height, img_filename_py);
	error = lodepng::decode(img_ny, img_width, img_height, img_filename_ny);
	error = lodepng::decode(img_pz, img_width, img_height, img_filename_pz);
	error = lodepng::decode(img_nz, img_width, img_height, img_filename_nz);
	
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_px[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_nx[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_py[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_ny[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_pz[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_nz[0]);
	
	img_px.clear();
	img_nx.clear();
	img_py.clear();
	img_ny.clear();
	img_pz.clear();
	img_nz.clear();	

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (GLint i = 0; i < 5; ++i) {
		locUniformCamera[i] = glGetUniformLocation(programIDs[i], "uCameraPos");
		// locate the rest of the uniform variables defined in the shaders.
		locUniformStone[i] = glGetUniformLocation(programIDs[i], "uStoneTex");
		locUniformMoss[i] = glGetUniformLocation(programIDs[i], "uMossTex");
		locUniformMap[i] = glGetUniformLocation(programIDs[i], "uTexMap");
		locUniformMVPM[i] = glGetUniformLocation(programIDs[i], "uModelViewProjMatrix");
		locUniformMM[i] = glGetUniformLocation(programIDs[i], "uModelMatrix");
		locUniformNM[i] = glGetUniformLocation(programIDs[i], "uNormalMatrix");
		locUniformDrawSky[i] = glGetUniformLocation(programIDs[i], "uDrawSky");

	}

	for (GLint i = 0; i < 3; ++i) {
		locUniformRatio[i] = glGetUniformLocation(programIDs[2+i], "uRefractRatio");
		locUniformRatioR[i] = glGetUniformLocation(programIDs[2+i], "uRefractRatioR");
		locUniformRatioG[i] = glGetUniformLocation(programIDs[2+i], "uRefractRatioG");
		locUniformRatioB[i] = glGetUniformLocation(programIDs[2+i], "uRefractRatioB");
		locUniformFresnel[i] = glGetUniformLocation(programIDs[2+i], "uFresnel");
		locUniformIsFresnel[i] = glGetUniformLocation(programIDs[2+i], "uIsFresnel");
	}
	
	locUniformTam[0] = glGetUniformLocation(programIDs[3], "uAngCrit");
	locUniformTam[1] = glGetUniformLocation(programIDs[4], "uAngCrit");

	locUniformToModel[0] = glGetUniformLocation(programIDs[3], "uToModelMatrix");
	locUniformToModel[1] = glGetUniformLocation(programIDs[4], "uToModelMatrix");

	
	return true;
}
 
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 Projection = glm::perspective(45.0f, 1.0f * g_Width / g_Height, 1.0f, 100.0f);
	
	glm::vec3 cameraPos = vec3( 7.0f * cos( yrot / 100 ), 2.0f * sin(xrot / 100), 7.0f * sin( yrot / 100 ) * cos(xrot /100) );
	glm::mat4 View = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	glm::mat4 ModelSky = glm::scale(glm::mat4(1.0), glm::vec3(25.0f, 25.0f, 25.0f));
	GLfloat tamCube = 0.4f;
	glm::mat4 ModelCube = glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(tamCube, tamCube, tamCube)),vec3(-3.0f, 0.0f, 3.0f));
	glm::mat4 ModelSphere = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 ModelTeapot = glm::translate(glm::rotate(glm::scale(glm::mat4(1.0f),vec3(0.25, 0.25, 0.25)), -90.0f, vec3(1.0, 0.0, 0.0)), vec3(4.0f, 0.0f, 2.0f));

	glm::mat4 mvp;
	glm::mat4 mv;
	glm::mat3 nm;

	float refractionRatio = (1.0 / (1.521*2));
	float refractionRatioR = (1.0 / (1.506*2));
	float refractionRatioG = (1.0 / (1.520*2));
	float refractionRatioB = (1.0 / (1.535*2));

	float fresnelC = pow((1.0 - (1.0/1.521)), 2) / pow((1.0 + (1.0/1.521)), 2);

	//float refractionRatio =  ((1.521 * 2));
	//float refractionRatioR = ((1.506 * 2));
	//float refractionRatioG = ((1.520 * 2));
	//float refractionRatioB = ((1.535 * 2));

	glUseProgram(programIDs[shaderAUsar+offsetExtra]);

	glUniform3fv(locUniformCamera[shaderAUsar + offsetExtra], 1, &cameraPos[0]);
	// set values for sampler-type uniform variables
	glUniform1i(locUniformStone[shaderAUsar + offsetExtra], 0);
	glUniform1i(locUniformMoss[shaderAUsar + offsetExtra], 1);
	glUniform1i(locUniformMap[shaderAUsar + offsetExtra], 2);


	// Draw the environment
	mvp = Projection * View * ModelSky;
	nm = glm::mat3(glm::transpose(glm::inverse(ModelSky)));
	// pass matrix to the uniform variables
	glUniformMatrix4fv(locUniformMM[shaderAUsar+ offsetExtra], 1, GL_FALSE, &ModelSky[0][0]);
	glUniformMatrix4fv(locUniformMVPM[shaderAUsar+ offsetExtra], 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix3fv(locUniformNM[shaderAUsar+ offsetExtra], 1, GL_FALSE, &nm[0][0]);
	glUniform1i(locUniformDrawSky[shaderAUsar+ offsetExtra], 1);
	
	if (shaderAUsar > 0) {
		drawCube();

		if (shaderAUsar == 2) {

			glUniform1f(locUniformRatio[offsetExtra], refractionRatio);
			glUniform1i(locUniformIsFresnel[offsetExtra], Fvalue);
			glUniform1f(locUniformFresnel[offsetExtra], fresnelC);

			if (offsetExtra) {
				refractionRatio = (1.0 / (1.521));
				refractionRatioR = (1.0 / (1.506));
				refractionRatioG = (1.0 / (1.520));
				refractionRatioB = (1.0 / (1.535));
			}

			if (dispersion) {
				glUniform1f(locUniformRatioR[offsetExtra], refractionRatioR);
				glUniform1f(locUniformRatioG[offsetExtra], refractionRatioG);
				glUniform1f(locUniformRatioB[offsetExtra], refractionRatioB);
			}
			else {
				glUniform1f(locUniformRatioR[offsetExtra], refractionRatio);
				glUniform1f(locUniformRatioG[offsetExtra], refractionRatio);
				glUniform1f(locUniformRatioB[offsetExtra], refractionRatio);
			}
		}

		if (offsetExtra) {
			glm::mat3 ToModelCube = glm::mat3(glm::inverse(ModelCube));
			glUniform1f(locUniformTam[0], tamCube); 
			glUniformMatrix3fv(locUniformToModel[0], 1, GL_FALSE, &ToModelCube[0][0]);
		}
	}

	// Draw Cube
	mvp = Projection * View * ModelCube;
	nm = glm::mat3(glm::transpose(glm::inverse(ModelCube)));
	// pass matrix to the uniform variables
	glUniformMatrix4fv(locUniformMM[shaderAUsar + offsetExtra], 1, GL_FALSE, &ModelCube[0][0]);
	glUniformMatrix4fv(locUniformMVPM[shaderAUsar + offsetExtra], 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix3fv(locUniformNM[shaderAUsar + offsetExtra], 1, GL_FALSE, &nm[0][0]);
	glUniform1i(locUniformDrawSky[shaderAUsar + offsetExtra], 0);

	drawCube();

	/*
	-----------------------------------------
	IF: the extra offset is being used, that is, there is 1 different shader for each one, the program is changed and the variables are passed again
	*/

	if (offsetExtra) {
		glUseProgram(programIDs[shaderAUsar+(offsetExtra*2)]);

		glUniform3fv(locUniformCamera[shaderAUsar + offsetExtra*2], 1, &cameraPos[0]);
		glUniform1i(locUniformStone[shaderAUsar + offsetExtra*2], 0);
		glUniform1i(locUniformMoss[shaderAUsar + offsetExtra*2], 1);
		glUniform1i(locUniformMap[shaderAUsar + offsetExtra*2], 2);


		glUniform1f(locUniformRatio[offsetExtra*2], refractionRatio);
		glUniform1i(locUniformIsFresnel[offsetExtra * 2], Fvalue);
		glUniform1f(locUniformFresnel[offsetExtra * 2], fresnelC);
		if (dispersion) {
			glUniform1f(locUniformRatioR[offsetExtra * 2], refractionRatioR);
			glUniform1f(locUniformRatioG[offsetExtra * 2], refractionRatioG);
			glUniform1f(locUniformRatioB[offsetExtra * 2], refractionRatioB);
		}
		else {
			glUniform1f(locUniformRatioR[offsetExtra * 2], refractionRatio);
			glUniform1f(locUniformRatioG[offsetExtra * 2], refractionRatio);
			glUniform1f(locUniformRatioB[offsetExtra * 2], refractionRatio);
		}

		glUniform1i(locUniformDrawSky[shaderAUsar + offsetExtra * 2], 0);

		glm::mat4 ToModelCube = glm::inverse(ModelSphere);

		// the critical angle is calculated
		GLfloat ang = asin(1/1.521);
		glUniform1f(locUniformTam[1], ang);
		glUniformMatrix4fv(locUniformToModel[1], 1, GL_FALSE, &ToModelCube[0][0]);
	}


	// Draw Sphere
	mvp = Projection * View * ModelSphere;
	nm = glm::mat3(glm::transpose(glm::inverse(ModelSphere)));
	// pass arrays to the uniform variables
	glUniformMatrix4fv(locUniformMM[shaderAUsar + offsetExtra * 2], 1, GL_FALSE, &ModelSphere[0][0]);
	glUniformMatrix4fv(locUniformMVPM[shaderAUsar + offsetExtra * 2], 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix3fv(locUniformNM[shaderAUsar + offsetExtra * 2], 1, GL_FALSE, &nm[0][0]);

	drawSphere();

	/*
	-----------------------------------------
	IF: the extra offset is being used, that is, there is 1 different shader for each one, the program is changed and the variables are passed again
	*/

	if (offsetExtra) {
		glUseProgram(programIDs[shaderAUsar]);

		glUniform3fv(locUniformCamera[shaderAUsar], 1, &cameraPos[0]);
		glUniform1i(locUniformStone[shaderAUsar], 0);
		glUniform1i(locUniformMoss[shaderAUsar], 1);
		glUniform1i(locUniformMap[shaderAUsar], 2);

		glUniform1f(locUniformRatio[0], refractionRatio);
		glUniform1i(locUniformIsFresnel[0], Fvalue);
		glUniform1f(locUniformFresnel[0], fresnelC);
		if (dispersion) {
			glUniform1f(locUniformRatioR[0], refractionRatioR);
			glUniform1f(locUniformRatioG[0], refractionRatioG);
			glUniform1f(locUniformRatioB[0], refractionRatioB);
		}
		else {
			glUniform1f(locUniformRatioR[0], refractionRatio);
			glUniform1f(locUniformRatioG[0], refractionRatio);
			glUniform1f(locUniformRatioB[0], refractionRatio);
		}

		glUniform1i(locUniformDrawSky[shaderAUsar], 0);
	}

	// Draw Teapot
	mvp = Projection * View * ModelTeapot;
	nm = glm::mat3(glm::transpose(glm::inverse(ModelTeapot)));
	// pass matrix to the uniform variables
	glUniformMatrix4fv(locUniformMM[shaderAUsar], 1, GL_FALSE, &ModelTeapot[0][0]);
	glUniformMatrix4fv(locUniformMVPM[shaderAUsar], 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix3fv(locUniformNM[shaderAUsar], 1, GL_FALSE, &nm[0][0]);

	drawTeapot();

	glUseProgram(0);

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
		xrot += 0.03f;
		yrot += 0.04f;
	}
	glutPostRedisplay();
	return;
}
 
void keyboard(GLubyte key, GLint x, GLint y)
{
	switch(key)
	{
	case 27 : case 'q': case 'Q':
		exit(EXIT_SUCCESS); 
		break;
	case 'a': case 'A':
		animation = !animation;
		break;
	case 'r': case 'R':
		shaderAUsar = (shaderAUsar + 1) % 3;

		/*
		OFFSETEXTRA controls whether the specific sphere and cube shaders are used.
		If it is 0, always when the refraction is done, the generic one will be used for the 3 objects
		*/
		offsetExtra = shaderAUsar / 2;

		//offsetExtra = 0;

		// std::cout << offsetExtra << std::endl;

		initCube(shaderAUsar + offsetExtra);
		numVertTeapot = initTeapot(10, glm::mat4(1.0f), shaderAUsar);
		numVertSphere = initSphere(1.0f, 30, 60, shaderAUsar + (2 * offsetExtra));
		break;
	case 'c': case 'C':
		dispersion = !dispersion;
		break;
	case 'f': case 'F':
		Fvalue = !Fvalue;

		if (Fvalue)
			fresnelValue = 1.0;
		else
			fresnelValue = 0.0;
		break;
	case 'x': case 'X':
		offsetExtra = (offsetExtra+1)%2;

		initCube(shaderAUsar + offsetExtra);
		numVertTeapot = initTeapot(10, glm::mat4(1.0f), shaderAUsar);
		numVertSphere = initSphere(1.0f, 30, 60, shaderAUsar + (2 * offsetExtra));
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
