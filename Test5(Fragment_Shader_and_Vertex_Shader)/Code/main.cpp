#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vboteapot.h"

void initCube(); 
GLint initTeapot(GLint grid, glm::mat4 transform);
void drawCube();
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
GLboolean drawing = true;
GLboolean crecer = false;
GLboolean rotar = false;
GLboolean discard = false;
 
GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat xdiff = 0.0f;
GLfloat ydiff = 0.0f;

GLint g_Width = 500;
GLint g_Height = 500;

GLuint vaoCube;
GLuint vaoTea;
GLuint programID;
GLuint programID2;

GLuint locMat1;
GLuint locMat2;
GLuint locMat3;
GLuint locFlo1;
GLuint locFlo2;

GLfloat inter = 0.0f;
GLfloat cantRotCol = 0.0f;
GLfloat umbral = 0.0f;


GLint numVertTeapot;


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
GLfloat vertices1[] = { 1, 1, 1, 1,  -1, 1, 1, 1,  -1,-1, 1, 1,     // v0-v1-v2 (front)
					   -1,-1, 1, 1,   1,-1, 1, 1,   1, 1, 1, 1,     // v2-v3-v0

						1, 1, 1, 1,   1,-1, 1, 1,   1,-1,-1, 1,      // v0-v3-v4 (right)
						1,-1,-1, 1,   1, 1,-1, 1,   1, 1, 1, 1,      // v4-v5-v0

						1, 1, 1, 1,   1, 1,-1, 1,  -1, 1,-1, 1,      // v0-v5-v6 (top)
					   -1, 1,-1, 1,  -1, 1, 1, 1,   1, 1, 1, 1,      // v6-v1-v0

					   -1, 1, 1, 1,  -1, 1,-1, 1,  -1,-1,-1, 1,      // v1-v6-v7 (left)
					   -1,-1,-1, 1,  -1,-1, 1, 1,  -1, 1, 1, 1,      // v7-v2-v1

					   -1,-1,-1, 1,   1,-1,-1, 1,   1,-1, 1, 1,      // v7-v4-v3 (bottom)
						1,-1, 1, 1,  -1,-1, 1, 1,  -1,-1,-1, 1,      // v3-v2-v7

						1,-1,-1, 1,  -1,-1,-1, 1,  -1, 1,-1, 1,      // v4-v7-v6 (back)
					   -1, 1,-1, 1,   1, 1,-1, 1,   1,-1,-1, 1 };    // v6-v5-v4
// color array
GLfloat colores1[]   = {1, 1, 1, 1,   1, 1, 0, 1,   1, 0, 0, 1,      // v0-v1-v2 (front)
						1, 0, 0, 1,   1, 0, 1, 1,   1, 1, 1, 1,      // v2-v3-v0

						1, 1, 1, 1,   1, 0, 1, 1,   0, 0, 1, 1,      // v0-v3-v4 (right)
						0, 0, 1, 1,   0, 1, 1, 1,   1, 1, 1, 1,      // v4-v5-v0

						1, 1, 1, 1,   0, 1, 1, 1,   0, 1, 0, 1,      // v0-v5-v6 (top)
						0, 1, 0, 1,   1, 1, 0, 1,   1, 1, 1, 1,      // v6-v1-v0

						1, 1, 0, 1,   0, 1, 0, 1,   0, 0, 0, 1,      // v1-v6-v7 (left)
						0, 0, 0, 1,   1, 0, 0, 1,   1, 1, 0, 1,      // v7-v2-v1

						0, 0, 0, 1,   0, 0, 1, 1,   1, 0, 1, 1,      // v7-v4-v3 (bottom)
						1, 0, 1, 1,   1, 0, 0, 1,   0, 0, 0, 1,      // v3-v2-v7

						0, 0, 1, 1,   0, 0, 0, 1,   0, 1, 0, 1,      // v4-v7-v6 (back)
						0, 1, 0, 1,   0, 1, 1, 1,   0, 0, 1, 1 };    // v6-v5-v4



// BEGIN: Shader support //////////////////////////////////////////////////////////////////////////////////////////

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
	if(!linked)
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

// END:   Shader support //////////////////////////////////////////////////////////////////////////////////////////


// BEGIN: Initializes drawing primitives //////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Init for Cube
///////////////////////////////////////////////////////////////////////////////
void initCube()
{
	GLuint vboHandle;

	glGenVertexArrays(1, &vaoCube);
	glBindVertexArray(vaoCube);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	glGenBuffers(1, &vboHandle);
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1) + sizeof(colores1), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices1), vertices1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices1), sizeof(colores1), colores1);

	//glVertexPointer(4, GL_FLOAT, 0, (GLubyte *)NULL + 0);
	//glColorPointer(4, GL_FLOAT, 0, (GLubyte *)NULL + sizeof(GL_FLOAT) * 4 * 36);
	GLuint loc = glGetAttribLocation(programID, "aPosition");
	GLuint loc2 = glGetAttribLocation(programID, "aColor");

	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	
	glEnableVertexAttribArray(loc2);
	glVertexAttribPointer(loc2, 4, GL_FLOAT, GL_FALSE, 0, (char*)NULL + (sizeof(vertices1)));

	glBindVertexArray(0);
	return;
}

///////////////////////////////////////////////////////////////////////////////
// Init Teapot
// parameters:
// 		grid - number of grids
// 		transform - model transformation matrix
// return:
// 		number of vertices in the model
///////////////////////////////////////////////////////////////////////////////
GLint initTeapot(GLint grid, glm::mat4 transform)
{
	GLint verts = 32 * (grid + 1) * (grid + 1);
	GLint faces = grid * grid * 32;
	GLfloat * v = new GLfloat[verts * 3];
	GLfloat * n = new GLfloat[verts * 3];
	GLfloat * tc = new GLfloat[verts * 2];
	GLuint * el = new GLuint[faces * 6];

	std::cout << faces << " : " << verts << std::endl;

	generatePatches(v, n, tc, el, grid);
	moveLid(grid, v, transform);

	// Creating and activating VAO
	glGenVertexArrays(1, &vaoTea);
	glBindVertexArray(vaoTea);

	GLuint handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, (3 * verts) * sizeof(GLfloat), v, GL_STATIC_DRAW); // Vertex position data
	// Setting the pointer to the vertex shader attribute and activate the array
	GLuint locTea1 = glGetAttribLocation(programID2, "aPosition");
	glEnableVertexAttribArray(locTea1);
	glVertexAttribPointer(locTea1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, (3 * verts) * sizeof(GLfloat), n, GL_STATIC_DRAW); // Vertex normal data
	// Setting the pointer to the vertex shader attribute and activating the array
	GLuint locTea2 = glGetAttribLocation(programID2, "aNormal");
	glEnableVertexAttribArray(locTea2);
	glVertexAttribPointer(locTea2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, (char*)NULL + ((3 * verts) * sizeof(GLfloat)));


	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, (2 * verts) * sizeof(GLfloat), tc, GL_STATIC_DRAW); // Texture coordinate data
	// Set the pointer to the vertex shader attribute and activate the array
	GLuint locTea3 = glGetAttribLocation(programID2, "aTexCoord");
	glEnableVertexAttribArray(locTea3);
	glVertexAttribPointer(locTea3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	//glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, (char*)NULL + ((3 * verts) * sizeof(GLfloat)) + ((3 * verts) * sizeof(GLfloat)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces * 6 * sizeof(GLuint), el, GL_STATIC_DRAW); // index array
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces * 6 * sizeof(GLuint), el, GL_STATIC_DRAW); // index array


	delete[] v;
	delete[] n;
	delete[] el;
	delete[] tc;


	// Dissable VAO
	glBindVertexArray(0);

	return 6 * faces;
}

// END: Initialize primitives ////////////////////////////////////////////////////////////////////////////////////


// BEGIN: Drawing Functions ////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Draw cube using VAO (non-indexed)
///////////////////////////////////////////////////////////////////////////////
void drawCube()
{
	glBindVertexArray(vaoCube);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0); 
	return;
}

///////////////////////////////////////////////////////////////////////////////
// Draw teapot using VAO (indexed)
///////////////////////////////////////////////////////////////////////////////
void drawTeapot() {
	glBindVertexArray(vaoTea);
	//glDrawArrays(GL_TRIANGLES, 0, numVertTeapot);
	glDrawElements(GL_TRIANGLES, numVertTeapot, GL_UNSIGNED_INT, 0);
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
	glutCreateWindow("Programa Ejemplo");
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

	// We create the compiled shader object for the vertex shader.
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	loadSource(vertexShaderID, "../Shaders/main.vert");
	std::cout << "Compilando shader de vertice ..." << std::endl;
	glCompileShader(vertexShaderID);
	printCompileInfoLog(vertexShaderID);
	
	// We create the compiled shader object for the fragment shader.
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	loadSource(fragmentShaderID, "../Shaders/main.frag");
	std::cout << "Compilando shader de fragmento ..." << std::endl;
	glCompileShader(fragmentShaderID);
	printCompileInfoLog(fragmentShaderID);	

	// We create the program object.
	programID = glCreateProgram();

	// We attach the compiled shaders to the program object
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	// We link the program object
	glLinkProgram(programID);

	// Verify that there are no errors in the link
	std::cout << "Linkando objeto programa ..." << std::endl;
	printLinkInfoLog(programID);
	validateProgram(programID);

	// Locate the uniform variables defined in the shaders
	locMat1 = glGetUniformLocation(programID, "uModelViewProj");
	initCube();

	// teapot
	GLuint vertexShaderID2 = glCreateShader(GL_VERTEX_SHADER);
	loadSource(vertexShaderID2, "../Shaders/tetera.vert");
	std::cout << "Compilando shader de vertice de tetera ..." << std::endl;
	glCompileShader(vertexShaderID2);
	printCompileInfoLog(vertexShaderID2);

	// teapot
	GLuint fragmentShaderID2 = glCreateShader(GL_FRAGMENT_SHADER);
	loadSource(fragmentShaderID2, "../Shaders/tetera.frag");
	std::cout << "Compilando shader de fragmento tetera ..." << std::endl;
	glCompileShader(fragmentShaderID2);
	printCompileInfoLog(fragmentShaderID2);

	programID2 = glCreateProgram();
	glAttachShader(programID2, vertexShaderID2);
	glAttachShader(programID2, fragmentShaderID2);
	glLinkProgram(programID2);
	std::cout << "Linkando objeto programa 2 ..." << std::endl;
	printLinkInfoLog(programID2);
	validateProgram(programID2);

	locMat2 = glGetUniformLocation(programID2, "uModelViewProj");
	locMat3 = glGetUniformLocation(programID2, "uColorRotation");
	locFlo1 = glGetUniformLocation(programID2, "uInter");
	locFlo2 = glGetUniformLocation(programID2, "uUmbral");

	numVertTeapot = initTeapot(10, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.5f)));

	return true;
}
 
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.0f * g_Width / g_Height, 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
	0.0f, 0.0f, 5.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f);

	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	*/

	glm::mat4 Projection = glm::perspective(45.0f, 1.0f * g_Width / g_Height, 1.0f, 100.0f);
	glm::mat4 View = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	Model = glm::rotate(Model, xrot, glm::vec3(1.0f, 0.0f, 0.0f));
	Model = glm::rotate(Model, yrot, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mvp = Projection * View * Model;

	glm::mat4 rotColor = glm::rotate(glm::mat4(1.0f), cantRotCol, glm::vec3(1.0f, 0.0f, 0.0f));

	// Activate the program object
	// Pass the uniform variables to the shaders.	

	if (!drawing) {
		glUseProgram(programID);
		glUniformMatrix4fv(locMat1, 1, GL_FALSE, &mvp[0][0]);
		drawCube();
	}

	// Activate the program object
	// Pass the uniform variables to the shaders.
	if (drawing) {
		glUseProgram(programID2);
		glUniformMatrix4fv(locMat2, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(locMat3, 1, GL_FALSE, &rotColor[0][0]);

		glUniform1f(locFlo1, inter);
		glUniform1f(locFlo2, umbral);
		drawTeapot();
	}

	// Dissable the program object
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
		xrot += 0.010f;
		yrot += 0.012f;
	}

	if (drawing) {
		if (rotar)
			cantRotCol += 0.010f;

		if (crecer)
			inter = (sin(glutGet(GLUT_ELAPSED_TIME) / 1000.0f) / 2) + 0.5;

		if (discard)
			umbral = (sin(glutGet(GLUT_ELAPSED_TIME) / 1000.0f) / 2) + 0.5;
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
	case 't': case 'T':
		drawing = !drawing;
		break;
	case 'p': case 'P':
		crecer = !crecer;
		break;
	case 'c': case 'C':
		rotar = !rotar;
		break;
	case 'f': case 'F':
		discard = !discard;

		if (!discard)
			umbral = 0.0f;
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

