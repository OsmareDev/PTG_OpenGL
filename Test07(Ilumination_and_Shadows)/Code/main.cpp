#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vboteapot.h"
#include "teapotdata.h"
#include "vbotorus.h"

GLint initSphere(GLfloat, GLuint, GLuint);
GLint initTeapot(GLint, glm::mat4);
GLint initPlane(GLfloat, GLfloat, GLint, GLint);
GLint initTorus(GLfloat, GLfloat, GLint, GLint);
void drawSphere();
void drawTeapot();
void drawPlane();
void drawTorus();

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
GLboolean luzEstatica = true;
 
GLfloat xrot = 1.0f;
GLfloat yrot = 0.0f;
GLfloat xdiff = 0.0f;
GLfloat ydiff = 0.0f;

GLint g_Width = 500;
GLint g_Height = 500;

GLuint cubeVAOHandle, sphereVAOHandle, teapotVAOHandle, planeVAOHandle, torusVAOHandle;
GLuint programID;
GLuint locUniformMVPM, locUniformMVM, locUniformNM;
GLuint locUniformLightPos, locUniformLightIntensity, locUniformLightK, locUniformLightDir, locUniformLightcutOffInt, locUniformLightcutOffExt;
GLuint locUniformMaterialAmbient, locUniformMaterialDiffuse, locUniformMaterialSpecular, locUniformMaterialShininess;
GLuint locUniformColorSuave, locUniformColorPlano, locUniformColorPhong, locUniformColorComic, locUniformFoco;

int numVertTeapot, numVertSphere, numVertPlane, numVertTorus;

GLint colorSuave = 0, colorPlano = 1, colorPhong = 0, colorComic = 0, foco = 0;

GLuint textIds[3];

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
	source = new std::string( std::istreambuf_iterator<char>(f),   
						std::istreambuf_iterator<char>() );
	f.close();
   
	// add a null to the string
	*source += "\0";
	const GLchar* data = source->c_str();
	glShaderSource(shaderID, 1, &data, NULL);
	delete source;
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
	GLuint loc1 = glGetAttribLocation(programID, "aPosition");   
	glEnableVertexAttribArray(loc1); // Vertex position
	glVertexAttribPointer( loc1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL + 0 ); 

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, (rings * sectors * 3) * sizeof(GLfloat), sphere_normals, GL_STATIC_DRAW);
	GLuint loc2 = glGetAttribLocation(programID, "aNormal");   
	glEnableVertexAttribArray(loc2); // Vertex normal
	glVertexAttribPointer( loc2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL + 0 ); 

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, (rings * sectors * 2) * sizeof(GLfloat), sphere_texcoords, GL_STATIC_DRAW);
	GLuint loc3 = glGetAttribLocation(programID, "aTexCoord");   
	glEnableVertexAttribArray(loc3); // texture coords
	glVertexAttribPointer( loc3, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL + 0 ); 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, rings * sectors * 4 * sizeof(GLushort), sphere_indices, GL_STATIC_DRAW);

	delete [] sphere_vertices;
	delete [] sphere_normals;
	delete [] sphere_texcoords;
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
	GLfloat * tc = new GLfloat[ verts * 2 ];
	GLuint * el = new GLuint[faces * 6];

	generatePatches( v, n, tc, el, grid );
	moveLid(grid, v, transform);

	glGenVertexArrays( 1, &teapotVAOHandle );
	glBindVertexArray(teapotVAOHandle);

	GLuint handle[4];
	glGenBuffers(4, handle);

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

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, (2 * verts) * sizeof(GLfloat), tc, GL_STATIC_DRAW);
	GLuint loc3 = glGetAttribLocation(programID, "aTexCoord");   
	glVertexAttribPointer( loc3, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
	glEnableVertexAttribArray(loc3);  // texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * faces * sizeof(GLuint), el, GL_STATIC_DRAW);

	delete [] v;
	delete [] n;
	delete [] el;
	delete [] tc;

	glBindVertexArray(0);

	return 6 * faces;
}

GLint initPlane(GLfloat xsize, GLfloat zsize, GLint xdivs, GLint zdivs)
{
	
	GLfloat * v = new GLfloat[3 * (xdivs + 1) * (zdivs + 1)];
	GLfloat * n = new GLfloat[3 * (xdivs + 1) * (zdivs + 1)];
	GLfloat * tex = new GLfloat[2 * (xdivs + 1) * (zdivs + 1)];
	GLuint * el = new GLuint[6 * xdivs * zdivs];

	GLfloat x2 = xsize / 2.0f;
	GLfloat z2 = zsize / 2.0f;
	GLfloat iFactor = (GLfloat)zsize / zdivs;
	GLfloat jFactor = (GLfloat)xsize / xdivs;
	GLfloat texi = 1.0f / zdivs;
	GLfloat texj = 1.0f / xdivs;
	GLfloat x, z;
	GLint vidx = 0, tidx = 0;
	for( GLint i = 0; i <= zdivs; i++ ) {
		z = iFactor * i - z2;
		for( GLint j = 0; j <= xdivs; j++ ) {
			x = jFactor * j - x2;
			v[vidx] = x;
			v[vidx+1] = 0.0f;
			v[vidx+2] = z;
			n[vidx] = 0.0f;
			n[vidx+1] = 1.0f;
			n[vidx+2] = 0.0f;
			vidx += 3;
			tex[tidx] = j * texi;
			tex[tidx+1] = i * texj;
			tidx += 2;
		}
	}

	GLuint rowStart, nextRowStart;
	GLint idx = 0;
	for( GLint i = 0; i < zdivs; i++ ) {
		rowStart = i * (xdivs+1);
		nextRowStart = (i+1) * (xdivs+1);
		for( GLint j = 0; j < xdivs; j++ ) {
			el[idx] = rowStart + j;
			el[idx+1] = nextRowStart + j;
			el[idx+2] = nextRowStart + j + 1;
			el[idx+3] = rowStart + j;
			el[idx+4] = nextRowStart + j + 1;
			el[idx+5] = rowStart + j + 1;
			idx += 6;
		}
	}

	GLuint handle[4];
	glGenBuffers(4, handle);

	glGenVertexArrays( 1, &planeVAOHandle );
	glBindVertexArray(planeVAOHandle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * (xdivs+1) * (zdivs+1) * sizeof(GLfloat), v, GL_STATIC_DRAW);
	GLuint loc1 = glGetAttribLocation(programID, "aPosition");   
	glVertexAttribPointer( loc1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
	glEnableVertexAttribArray(loc1);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * (xdivs+1) * (zdivs+1) * sizeof(GLfloat), n, GL_STATIC_DRAW);
	GLuint loc2 = glGetAttribLocation(programID, "aNormal");   
	glVertexAttribPointer( loc2, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
	glEnableVertexAttribArray(loc2);  // Vertex normal

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, 2 * (xdivs+1) * (zdivs+1) * sizeof(GLfloat), tex, GL_STATIC_DRAW);
	GLuint loc3 = glGetAttribLocation(programID, "aTexCoord");   
	glVertexAttribPointer( loc3, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
	glEnableVertexAttribArray(loc3);  // texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * xdivs * zdivs * sizeof(GLuint), el, GL_STATIC_DRAW);

	glBindVertexArray(0);
	
	delete [] v;
	delete [] n;
	delete [] tex;
	delete [] el;

	return 6 * xdivs * zdivs;
}

GLint initTorus(GLfloat outerRadius, GLfloat innerRadius, GLint nsides, GLint nrings) 
{
	GLint faces = nsides * nrings;
	GLint nVerts  = nsides * (nrings+1);   // One extra ring to duplicate first ring

	// Verts
	GLfloat * v = new GLfloat[3 * nVerts];
	// Normals
	GLfloat * n = new GLfloat[3 * nVerts];
	// Tex coords
	GLfloat * tex = new GLfloat[2 * nVerts];
	// Elements
	GLuint * el = new GLuint[6 * faces];

	// Generate the vertex data
	generateVerts(v, n, tex, el, outerRadius, innerRadius, nrings, nsides);

	// Create and populate the buffer objects
	GLuint handle[4];
	glGenBuffers(4, handle);

	// Create the VAO
	glGenVertexArrays( 1, &torusVAOHandle );
	glBindVertexArray(torusVAOHandle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, (3 * nVerts) * sizeof(GLfloat), v, GL_STATIC_DRAW);
	GLuint loc1 = glGetAttribLocation(programID, "aPosition");   
	glVertexAttribPointer( loc1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
	glEnableVertexAttribArray(loc1);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, (3 * nVerts) * sizeof(GLfloat), n, GL_STATIC_DRAW);
	GLuint loc2 = glGetAttribLocation(programID, "aNormal");   
	glVertexAttribPointer( loc2, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
	glEnableVertexAttribArray(loc2);  // Vertex normal

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, (2 * nVerts) * sizeof(GLfloat), tex, GL_STATIC_DRAW);
	GLuint loc3 = glGetAttribLocation(programID, "aTexCoord");   
	glVertexAttribPointer( loc3, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
	glEnableVertexAttribArray(loc3);  // texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * faces * sizeof(GLuint), el, GL_STATIC_DRAW);

	glBindVertexArray(0);

	delete [] v;
	delete [] n;
	delete [] el;
	delete [] tex;

	return 6 * faces;
}
// END: Initialize primitives ////////////////////////////////////////////////////////////////////////////////////

// BEGIN: Drawing Functions ////////////////////////////////////////////////////////////////////////////////////

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

void drawPlane() {
	glBindVertexArray(planeVAOHandle);
	glDrawElements(GL_TRIANGLES, numVertPlane, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
	glBindVertexArray(0);
	return;
}

void drawTorus() {
	glBindVertexArray(torusVAOHandle);
	glDrawElements(GL_TRIANGLES, numVertTorus, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
	glBindVertexArray(0);
	return;
}

// END: Drawing Functions ////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	glutInit(&argc, argv); 
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(g_Width, g_Height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Test 7");
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
	loadSource(vertexShaderID, "../shaders/illumination.vert");
	std::cout << "Compiling vertex shader ..." << std::endl;
	glCompileShader(vertexShaderID);
	printCompileInfoLog(vertexShaderID);
	glAttachShader(programID, vertexShaderID);

	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	loadSource(fragmentShaderID, "../shaders/illumination.frag");
	std::cout << "Compiling fragment shader ..." << std::endl;
	glCompileShader(fragmentShaderID);
	printCompileInfoLog(fragmentShaderID);
	glAttachShader(programID, fragmentShaderID);

	std::cout << "Linking program object ..." << std::endl;
	glLinkProgram(programID);
	printLinkInfoLog(programID);
	validateProgram(programID);

	numVertTeapot = initTeapot(5, glm::mat4(1.0f));
	numVertSphere = initSphere(1.0f, 20, 30);
	numVertPlane = initPlane(10.0f, 10.0f, 20, 20);
	numVertTorus = initTorus(0.5f, 0.25f, 20, 40);

	// Location of the ID of the uniform variables
	locUniformMVPM = glGetUniformLocation(programID, "uModelViewProjMatrix");
	locUniformMVM = glGetUniformLocation(programID, "uModelViewMatrix");
	locUniformNM = glGetUniformLocation(programID, "uNormalMatrix");

	// When the uniform variables are a structure, each field must be located separately
	locUniformLightPos = glGetUniformLocation(programID, "uLight.lightPos");
	locUniformLightIntensity = glGetUniformLocation(programID, "uLight.intensity");
	locUniformLightK = glGetUniformLocation(programID, "uLight.k");
	locUniformLightDir = glGetUniformLocation(programID, "uLight.dir");
	locUniformLightcutOffInt = glGetUniformLocation(programID, "uLight.cutOffInt");
	locUniformLightcutOffExt = glGetUniformLocation(programID, "uLight.cutOffExt");
	
	// locate the id for the material properties.
	locUniformMaterialAmbient = glGetUniformLocation(programID, "uMaterial.ambient");
	locUniformMaterialDiffuse = glGetUniformLocation(programID, "uMaterial.diffuse");
	locUniformMaterialSpecular = glGetUniformLocation(programID, "uMaterial.specular");
	locUniformMaterialShininess = glGetUniformLocation(programID, "uMaterial.shininess");

	locUniformColorSuave = glGetUniformLocation(programID, "uColorSuave");
	locUniformColorPlano = glGetUniformLocation(programID, "uColorPlano");
	locUniformColorPhong = glGetUniformLocation(programID, "uColorPhong");
	locUniformColorComic = glGetUniformLocation(programID, "uColorComic");
	locUniformFoco = glGetUniformLocation(programID, "uFoco");

	return true;
}
 
void display()
{
	static float DEGREE_TO_RADIANS = 3.141592 / 180;
	static int angle_degree = 0;
	float angle_radians;
	
	if ( !luzEstatica )
		angle_degree = (angle_degree + 1) % 360;
	angle_radians = angle_degree * DEGREE_TO_RADIANS;

	struct LightInfo {
	 glm::vec4 lightPos;
	 glm::vec3 intensity;
	 glm::vec3 k;
	 glm::vec3 dir; // focal direction
	 GLfloat cutOffInt; // Interior angle (limit angle of the illuminated area)
	 GLfloat cutOffExt; // Exterior angle (limit angle of the twilight zone)
	};
	LightInfo light = { glm::vec4(3*cos(angle_radians), 2.5f, 3*sin(angle_radians), 1.0f),  // Pos. S.R. mundo
						glm::vec3(1.0f, 1.0f, 1.0f), 
						glm::vec3(1.0f, 0.0f, 0.0f), 
						glm::vec3(-3*cos(angle_radians), -2.0f, -3*sin(angle_radians)), // Dir. S.R. mundo
						35.0f * DEGREE_TO_RADIANS,
						40.0f * DEGREE_TO_RADIANS
	};

	struct MaterialInfo {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		GLfloat shininess;
	};
	MaterialInfo gold = {glm::vec3(0.24725f, 0.1995f, 0.0745f), glm::vec3(0.75164f, 0.60648f, 0.22648f), glm::vec3(0.628281f, 0.555802f, 0.366065f), 52.0f};
	MaterialInfo perl = {glm::vec3(0.25f, 0.20725f, 0.20725f), glm::vec3(1.0f, 0.829f, 0.829f), glm::vec3(0.296648f, 0.296648f, 0.296648f), 12.0f};
	MaterialInfo bronze = {glm::vec3(0.2125f, 0.1275f, 0.054f), glm::vec3(0.714f, 0.4284f, 0.18144f), glm::vec3(0.393548f, 0.271906f, 0.166721f), 25.0f};
	MaterialInfo brass = {glm::vec3(0.329412f, 0.223529f, 0.027451f), glm::vec3(0.780392f, 0.568627f, 0.113725f), glm::vec3(0.992157f, 0.941176f, 0.807843f), 28.0f};
	MaterialInfo emerald = {glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.07568f, 0.61424f, 0.07568f), glm::vec3(0.633f, 0.727811f, 0.633f), 28.0f};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 Projection = glm::perspective(45.0f, 1.0f * g_Width / g_Height, 1.0f, 100.0f);
	
	glm::vec3 cameraPos = vec3( 5.0f * sin( xrot / 200 ) * cos( yrot / 100 ), 2.0f * cos(xrot / 200) + 3.0f, 5.0f * sin( xrot / 200 ) * sin( yrot / 100 ) );
	glm::mat4 View = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 ModelPlane = glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(1.0f, 1.0f, 1.0f)),vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 ModelSphere = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)), vec3(-3.0f,1.0f, 3.0f));
	glm::mat4 ModelTeapot = glm::translate(glm::rotate(glm::scale(glm::mat4(1.0f),vec3(0.25, 0.25, 0.25)), -90.0f, vec3(1.0, 0.0, 0.0)), vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 ModelTorus = glm::translate(glm::rotate(glm::mat4(1.0f), -90.0f, vec3(1.0, 0.0, 0.0)), vec3(2.0f, 0.0f, 0.25f));

	glm::mat4 mvp; // Model-view-projection matrix
	glm::mat4 mv;  // Model-view matrix
	glm::mat3 nm;  // Normal matrix

	glUseProgram(programID);
	
	// Pass light properties to shader
	mv = View;
	nm = glm::mat3(glm::transpose(glm::inverse(mv)));
	glm::vec4 lpos = mv * light.lightPos;	// Position of the spot light/spotlight in the reference system of the view
	glm::vec3 ldir = nm * light.dir;		// Focus direction in the reference system of the view
	glUniform4fv(locUniformLightPos, 1, &(lpos.x));
	glUniform3fv(locUniformLightIntensity, 1, &(light.intensity.r));
	glUniform3fv(locUniformLightK, 1, &(light.k[0]));
	glUniform3fv(locUniformLightDir, 1, &(ldir.x));
	glUniform1f(locUniformLightcutOffInt, light.cutOffInt);
	glUniform1f(locUniformLightcutOffExt, light.cutOffExt);

	glUniform1i(locUniformColorSuave, colorSuave);
	glUniform1i(locUniformColorPlano, colorPlano);
	glUniform1i(locUniformColorPhong, colorPhong);
	glUniform1i(locUniformColorComic, colorComic);
	glUniform1i(locUniformFoco, foco);	


	// pass to the shader the matrices and the material properties (gold)
	mvp = Projection * View * ModelSphere;
	mv = View * ModelSphere;
	nm = glm::mat3(glm::transpose(glm::inverse(mv)));
	glUniformMatrix4fv(locUniformMVPM, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix3fv(locUniformNM, 1, GL_FALSE, &nm[0][0]);
	glUniformMatrix4fv(locUniformMVM, 1, GL_FALSE, &mv[0][0]);

	glUniform3fv(locUniformMaterialAmbient, 1, &(gold.ambient.x));
	glUniform3fv(locUniformMaterialDiffuse, 1, &(gold.diffuse.x));
	glUniform3fv(locUniformMaterialSpecular, 1, &(gold.specular.x));
	glUniform1f(locUniformMaterialShininess, gold.shininess);

	drawSphere();

	// pass to the shader the matrices and the material properties (brass)
	mvp = Projection * View * ModelTeapot;
	mv = View * ModelTeapot;
	nm = glm::mat3(glm::transpose(glm::inverse(mv)));
	glUniformMatrix4fv(locUniformMVPM, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix3fv(locUniformNM, 1, GL_FALSE, &nm[0][0]);
	glUniformMatrix4fv(locUniformMVM, 1, GL_FALSE, &mv[0][0]);

	glUniform3fv(locUniformMaterialAmbient, 1, &(brass.ambient.x));
	glUniform3fv(locUniformMaterialDiffuse, 1, &(brass.diffuse.x));
	glUniform3fv(locUniformMaterialSpecular, 1, &(brass.specular.x));
	glUniform1f(locUniformMaterialShininess, brass.shininess);
	
	drawTeapot();

	// pass to the shader the matrices and the material properties (emerald)
	mvp = Projection * View * ModelTorus;
	mv = View * ModelTorus;
	nm = glm::mat3(glm::transpose(glm::inverse(mv)));
	glUniformMatrix4fv(locUniformMVPM, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix3fv(locUniformNM, 1, GL_FALSE, &nm[0][0]);
	glUniformMatrix4fv(locUniformMVM, 1, GL_FALSE, &mv[0][0]);

	glUniform3fv(locUniformMaterialAmbient, 1, &(emerald.ambient.x));
	glUniform3fv(locUniformMaterialDiffuse, 1, &(emerald.diffuse.x));
	glUniform3fv(locUniformMaterialSpecular, 1, &(emerald.specular.x));
	glUniform1f(locUniformMaterialShininess, emerald.shininess);

	drawTorus();

	// pass to the shader the matrices and the material properties (perl)
	mvp = Projection * View * ModelPlane;
	mv = View * ModelPlane;
	nm = glm::mat3(glm::transpose(glm::inverse(mv)));
	glUniformMatrix4fv(locUniformMVPM, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix3fv(locUniformNM, 1, GL_FALSE, &nm[0][0]);
	glUniformMatrix4fv(locUniformMVM, 1, GL_FALSE, &mv[0][0]);

	glUniform3fv(locUniformMaterialAmbient, 1, &(perl.ambient.x));
	glUniform3fv(locUniformMaterialDiffuse, 1, &(perl.diffuse.x));
	glUniform3fv(locUniformMaterialSpecular, 1, &(perl.specular.x));
	glUniform1f(locUniformMaterialShininess, perl.shininess);

	drawPlane();

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
		xrot += 0.3f;
		yrot += 0.4f;
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
	case 'e': case 'E':
		luzEstatica = !luzEstatica;
		break;
	case 's': case 'S':
		colorPlano = 1;

		colorSuave = colorPlano - (colorSuave + colorPhong + colorComic);
		colorPhong = colorPlano - (colorSuave + colorPhong + colorComic);
		colorComic = colorPlano - (colorSuave + colorPhong + colorComic);
		colorPlano = colorPlano - (colorSuave + colorPhong + colorComic);


		break;
	case 'l': case 'L':
		foco = 1 - foco;
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
