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
#include "lodepng.h"

GLint initTeapot(GLint grid, glm::mat4 transform);
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
GLboolean explosion = false;
GLboolean change = false;

GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat xdiff = 0.0f;
GLfloat ydiff = 0.0f;

GLint g_Width = 500;
GLint g_Height = 500;

GLuint teapotVAOHandle;
GLuint programID1, programID2;
GLuint locUniformMVPM, locUniformMVM, locUniformNM, locUniformPM;
GLuint locUniformSpriteTex;
GLuint locUniformExplosionFactor;

GLfloat factor_explosion = 0;

int numVertTeapot;

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
	source = new std::string(std::istreambuf_iterator<GLchar>(f),
		std::istreambuf_iterator<GLchar>());
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
	glGetShaderiv(shadID, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		GLint infoLength = 0;
		glGetShaderiv(shadID, GL_INFO_LOG_LENGTH, &infoLength);

		GLchar *infoLog = new GLchar[infoLength];
		GLint chsWritten = 0;
		glGetShaderInfoLog(shadID, infoLength, &chsWritten, infoLog);

		std::cerr << "Shader compiling failed:" << infoLog << std::endl;
		system("pause");
		delete[] infoLog;

		exit(EXIT_FAILURE);
	}
	return;
}

void printLinkInfoLog(GLuint programID)
{
	GLint linked;
	glGetProgramiv(programID, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint infoLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLength);

		GLchar *infoLog = new GLchar[infoLength];
		GLint chsWritten = 0;
		glGetProgramInfoLog(programID, infoLength, &chsWritten, infoLog);

		std::cerr << "Shader linking failed:" << infoLog << std::endl;
		system("pause");
		delete[] infoLog;

		exit(EXIT_FAILURE);
	}
	return;
}

void validateProgram(GLuint programID)
{
	GLint status;
	glValidateProgram(programID);
	glGetProgramiv(programID, GL_VALIDATE_STATUS, &status);

	if (status == GL_FALSE)
	{
		GLint infoLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLength);

		if (infoLength > 0)
		{
			GLchar *infoLog = new GLchar[infoLength];
			GLint chsWritten = 0;
			glGetProgramInfoLog(programID, infoLength, &chsWritten, infoLog);
			std::cerr << "Program validating failed:" << infoLog << std::endl;
			system("pause");
			delete[] infoLog;

			exit(EXIT_FAILURE);
		}
	}
	return;
}

// END: Load shaders ////////////////////////////////////////////////////////////////////////////////////////////

// BEGIN: Initialize primitives ////////////////////////////////////////////////////////////////////////////////////

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
	GLfloat * v = new GLfloat[verts * 3];
	GLfloat * n = new GLfloat[verts * 3];
	GLfloat * tc = new GLfloat[verts * 2];
	GLuint * el = new GLuint[faces * 6];

	generatePatches(v, n, tc, el, grid);
	moveLid(grid, v, transform);

	glGenVertexArrays(1, &teapotVAOHandle);
	glBindVertexArray(teapotVAOHandle);

	GLuint handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, (3 * verts) * sizeof(GLfloat), v, GL_STATIC_DRAW); // Datos de la posici�n de los v�rtices
	// GLuint loc1 = glGetAttribLocation(programID, "aPosition");
	// It is not necessary to locate it because it has been assigned as ID = 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, (3 * verts) * sizeof(GLfloat), n, GL_STATIC_DRAW); // Datos de las normales de los v�rtices
	// GLuint loc2 = glGetAttribLocation(programID, "aNormal");
	// It is not necessary to locate it because it has been assigned as ID = 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(1);  // Vertex normal

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * faces * sizeof(GLuint), el, GL_STATIC_DRAW); // Array de �ndices

	delete[] v;
	delete[] n;
	delete[] el;
	delete[] tc;

	glBindVertexArray(0);

	return 6 * faces;
}
// END: Initialize primitives ////////////////////////////////////////////////////////////////////////////////////

// BEGIN: Drawing Features ////////////////////////////////////////////////////////////////////////////////////
void drawTeapot() {
	glBindVertexArray(teapotVAOHandle);
	glDrawElements(GL_TRIANGLES, numVertTeapot, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
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
	glutCreateWindow("Test 12");
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glShadeModel(GL_SMOOTH);

	programID1 = glCreateProgram();

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	loadSource(vertexShaderID, "../Shaders/geometry.vert");
	std::cout << "Compiling Vertex Shader" << std::endl;
	glCompileShader(vertexShaderID);
	printCompileInfoLog(vertexShaderID);
	glAttachShader(programID1, vertexShaderID);

	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	loadSource(fragmentShaderID, "../Shaders/geometry.frag");
	std::cout << "Compiling Fragment Shader" << std::endl;
	glCompileShader(fragmentShaderID);
	printCompileInfoLog(fragmentShaderID);
	glAttachShader(programID1, fragmentShaderID);

	// Do the same for the geometry shader (as for the vertex and fragment shader)
	GLuint geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	loadSource(geometryShaderID, "../Shaders/geometry.geom");
	std::cout << "Compiling Geometry Shader" << std::endl;
	glCompileShader(geometryShaderID);
	printCompileInfoLog(geometryShaderID);
	glAttachShader(programID1, geometryShaderID);

	glLinkProgram(programID1);
	printLinkInfoLog(programID1);
	validateProgram(programID1);

	// Create a second program object with impostors implementation
	programID2 = glCreateProgram();

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	loadSource(vertexShaderID, "../Shaders/geometry.vert");
	std::cout << "Compiling Vertex Shader" << std::endl;
	glCompileShader(vertexShaderID);
	printCompileInfoLog(vertexShaderID);
	glAttachShader(programID2, vertexShaderID);

	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	loadSource(fragmentShaderID, "../Shaders/geometry2.frag");
	std::cout << "Compiling Fragment Shader" << std::endl;
	glCompileShader(fragmentShaderID);
	printCompileInfoLog(fragmentShaderID);
	glAttachShader(programID2, fragmentShaderID);

	// Do the same for the geometry shader (as for the vertex and fragment shader)
	geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	loadSource(geometryShaderID, "../Shaders/geometry2.geom");
	std::cout << "Compiling Geometry Shader" << std::endl;
	glCompileShader(geometryShaderID);
	printCompileInfoLog(geometryShaderID);
	glAttachShader(programID2, geometryShaderID);

	glLinkProgram(programID2);
	printLinkInfoLog(programID2);
	validateProgram(programID2);

	// uncomment uploading the image with the imposter.
	std::vector<GLubyte> img_data;
	GLuint img_width, img_height;
	GLuint error;
	const GLchar img_filename[] = "texturas/white_sphere.png";

	GLuint textId;
	glGenTextures(1, &textId);

	error = lodepng::decode(img_data, img_width, img_height, img_filename);
	if (!error)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
		std::cout << "Error al cargar la textura " << img_filename << std::endl;
	img_data.clear();

	numVertTeapot = initTeapot(5, glm::mat4(1.0f));

	// It is not necessary to locate the ID because a fixed ID has been assigned in the shaders (see shader code)
	/*
	locUniformMVPM = glGetUniformLocation(programID, "uModelViewProjMatrix");
	locUniformMVM = glGetUniformLocation(programID, "uModelViewMatrix");
	locUniformNM = glGetUniformLocation(programID, "uNormalMatrix");
	locUniformPM = glGetUniformLocation(programID, "uProjectionMatrix");
	locUniformExplosionFactor = glGetUniformLocation(programID, "uExplosionFactor");
	locUniformSpriteTex = glGetUniformLocation(programID, "uSpriteTex");
	*/
	// Instead, we directly put the ID that we have assigned in the shader code.
	locUniformMVPM = 0;
	locUniformMVM = 1;
	locUniformNM = 2;
	locUniformPM = 3;
	locUniformExplosionFactor = 4;
	locUniformSpriteTex = 5;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT_FACE);

	return true;
}
 
void display()
{
	// Constant with the amount that explosion_factor has to increase in each frame
	const GLfloat inc_explosion = 0.04;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 Projection = glm::perspective(45.0f, 1.0f * g_Width / g_Height, 1.0f, 100.0f);
	
	glm::vec3 cameraPos = glm::vec3( 9.0f * cos( yrot / 100 ), 2.0f * sin(xrot / 100) + 2.0f, 9.0f * sin( yrot / 100 ) * cos(xrot /100) );
	glm::mat4 View = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 ModelTeapot = glm::translate(glm::rotate(glm::scale(glm::mat4(1.0f), vec3(0.25, 0.25, 0.25)), -90.0f, vec3(1.0, 0.0, 0.0)), vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 mvp; // Model-view-projection matrix
	glm::mat4 mv;  // Model-view matrix
	glm::mat3 nm;  // Normal matrix

	// Choose the program object that is activated (depending on whether or not the imposters option is activated)
	if (change)
		glUseProgram(programID2);
	else
		glUseProgram(programID1);

	// Pass to the shader of the values for the uniform variables
	mv = View;
	nm = glm::mat3(glm::transpose(glm::inverse(mv)));
	
	// pass to the sampler the texture unit where the sphere image is located
	glUniform1i(locUniformSpriteTex, 0);

	// update the value of explosion_factor and pass it to the variable uniform
	if (explosion) {
		//if (factor_explosion < 1.0f)
			factor_explosion += 0.001;
	}
	glUniform1f(locUniformExplosionFactor, factor_explosion);

	// Draw Teapot
	mvp = Projection * View * ModelTeapot;
	mv = View * ModelTeapot;
	nm = glm::mat3(glm::transpose(glm::inverse(mv)));
	glUniformMatrix4fv(locUniformMVPM, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(locUniformMVM, 1, GL_FALSE, &mv[0][0]);
	glUniformMatrix3fv(locUniformNM, 1, GL_FALSE, &nm[0][0]);
	glUniformMatrix4fv(locUniformPM, 1, GL_FALSE, &Projection[0][0]);
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
		xrot += 0.3f;
		yrot += 0.4f;
	}
	glutPostRedisplay();
	return;
}

void keyboard(GLubyte key, GLint x, GLint y)
{
	static bool wireframe = false;
	switch(key)
	{
	case 27 : case 'q': case 'Q':
		exit(EXIT_SUCCESS); 
		break;
	case 'a': case 'A':
		animation = !animation;
		break;
	case 'e': case 'E':
		explosion = !explosion;
		factor_explosion = 0;
		break;
	case 'i': case 'I':
		change = !change;
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
