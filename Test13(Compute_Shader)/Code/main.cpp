#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lodepng.h"


void printFPS();

void initPoints(GLint);
void drawPoints(GLint);

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
GLint lighting = 0; 

/// <summary>
GLfloat amortiguacion = 0;
GLuint locUniformAmortig;
/// </summary>
 
GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat xdiff = 0.0f;
GLfloat ydiff = 0.0f;

GLint g_Width = 500;
GLint g_Height = 500;

GLuint cubeVAOHandle, pointsVAOHandle;
GLuint graphicProgramID, computeProgramID;
GLuint locUniformMVPM, locUniformMVM, locUniformPM;
GLuint locUniformSpriteTex;

//const GLint NUM_PARTICLES = 128;
//const GLint WORK_GROUP_SIZE = 16;

//const GLint NUM_PARTICLES = 32 * 256;
//const GLint WORK_GROUP_SIZE = 256;

const GLint NUM_PARTICLES = 16*1024*256;
const GLint WORK_GROUP_SIZE = 1024;
// 43 fps con grupo de 128
// 43 fps con grupo de 256
// 43 fps con grupo de 512
// 43 fps con grupo de 1024

inline GLfloat ranf( GLfloat min = 0.0f, GLfloat max = 1.0f )
{
	return ((max - min) * rand() / RAND_MAX + min);
}


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

void initPoints(GLint numPoints) 
{
	GLfloat *points = new GLfloat[NUM_PARTICLES * 4];
	GLfloat *colors = new GLfloat[NUM_PARTICLES * 4];
	GLfloat *velocs = new GLfloat[NUM_PARTICLES * 4];

	for ( GLint i = 0; i < NUM_PARTICLES; i ++ )
	{
		points[4 * i + 0] = ranf(-1.0f, 1.0f); // x
		points[4 * i + 1] = ranf(-1.0f, 1.0f); // y
		points[4 * i + 2] = ranf(-1.0f, 1.0f); // z
		points[4 * i + 3] = 1.0f;
		colors[4 * i + 0] = ranf(); // r
		colors[4 * i + 1] = ranf(); // g
		colors[4 * i + 2] = ranf(); // b
		colors[4 * i + 3] = 1.0f;
		velocs[4 * i + 0] = ranf(-4.0f, 4.0f); // x
		velocs[4 * i + 1] = ranf(-4.0f, 4.0f); // y
		velocs[4 * i + 2] = ranf(-4.0f, 4.0f); // z
		velocs[4 * i + 3] = 0.0f;
	}

	GLuint posSSbo;
	GLuint velSSbo;
	GLuint colSSbo;


	// Create SSBO instead of VBO
	glGenBuffers( 1, &posSSbo);
	glGenBuffers( 1, &velSSbo); 
	glGenBuffers( 1, &colSSbo);
	// Activate them to be indexed within the compute shader
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSSbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 4 * sizeof(GLfloat) * NUM_PARTICLES, points, GL_STATIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velSSbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 4 * sizeof(GLfloat) * NUM_PARTICLES, colors, GL_STATIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, colSSbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 4 * sizeof(GLfloat) * NUM_PARTICLES, velocs, GL_STATIC_DRAW);
	

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posSSbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velSSbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, colSSbo);

	// Within the VAO, use the SSBOs as VBOs (it will not be necessary to pass the data back to it)
	glGenVertexArrays (1, &pointsVAOHandle);
	glBindVertexArray (pointsVAOHandle);


	glBindBuffer(GL_ARRAY_BUFFER, posSSbo);    
	//glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLfloat) * NUM_PARTICLES, points, GL_STATIC_DRAW);
	GLuint loc = glGetAttribLocation(graphicProgramID, "aPosition");   
	glEnableVertexAttribArray(loc); 
	glVertexAttribPointer( loc, 4, GL_FLOAT, GL_FALSE, 0, (GLchar *)NULL + 0 ); 

	glBindBuffer(GL_ARRAY_BUFFER, colSSbo);    
	//glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLfloat) * NUM_PARTICLES, colors, GL_STATIC_DRAW);
	GLuint loc2 = glGetAttribLocation(graphicProgramID, "aColor"); 
	glEnableVertexAttribArray(loc2); 
	glVertexAttribPointer( loc2, 4, GL_FLOAT, GL_FALSE, 0, (GLchar *)NULL + 0 );

	glBindVertexArray (0);

	delete []points;
	delete []colors;
	delete []velocs;
	return;
}

// END: Initialize primitives ////////////////////////////////////////////////////////////////////////////////////

// BEGIN: Drawing Functions ////////////////////////////////////////////////////////////////////////////////////

void drawPoints(GLint numPoints)
{
	glBindVertexArray(pointsVAOHandle);
	glDrawArrays(GL_POINTS, 0, numPoints);
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
	glutCreateWindow("Test 13");
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
	srand (time(NULL));

	glClearColor(0.093f, 0.093f, 0.093f, 0.0f);
 
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearDepth(1.0f);

	glShadeModel(GL_SMOOTH);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create the program object for the compute shader
	/*
	GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShader, 1, nullptr, NULL);
	GLchar* codigo = "tema4_parte3_comp.glsl";
	glGetShaderSource(computeShader, 10, nullptr, );
	*/

	computeProgramID = glCreateProgram();

	GLuint computeShaderID = glCreateShader(GL_COMPUTE_SHADER);
	loadSource(computeShaderID, "../Shaders/compute.glsl");
	std::cout << "Compiling Geometry Shader" << std::endl;
	glCompileShader(computeShaderID);
	printCompileInfoLog(computeShaderID);
	glAttachShader(graphicProgramID, computeShaderID);

	glAttachShader(computeProgramID, computeShaderID);
	glLinkProgram(computeProgramID);

	
	// Graphic shaders program
	graphicProgramID = glCreateProgram();

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	loadSource(vertexShaderID, "../Shaders/compute.vert");
	std::cout << "Compiling Vertex Shader" << std::endl;
	glCompileShader(vertexShaderID);
	printCompileInfoLog(vertexShaderID);
	glAttachShader(graphicProgramID, vertexShaderID);

	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	loadSource(fragmentShaderID, "../Shaders/compute.frag");
	std::cout << "Compiling Fragment Shader" << std::endl;
	glCompileShader(fragmentShaderID);
	printCompileInfoLog(fragmentShaderID);
	glAttachShader(graphicProgramID, fragmentShaderID);

	// Perform the same task for the geometry shader (as for the vertex and fragment shader)
	GLuint geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	loadSource(geometryShaderID, "../Shaders/compute.geom");
	std::cout << "Compiling Geometry Shader" << std::endl;
	glCompileShader(geometryShaderID);
	printCompileInfoLog(geometryShaderID);
	glAttachShader(graphicProgramID, geometryShaderID);

	glLinkProgram(graphicProgramID);
	printLinkInfoLog(graphicProgramID);
	validateProgram(graphicProgramID);


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

	initPoints(NUM_PARTICLES);
	locUniformMVPM = glGetUniformLocation(graphicProgramID, "uModelViewProjMatrix");
	locUniformMVM = glGetUniformLocation(graphicProgramID, "uModelViewMatrix");
	locUniformPM = glGetUniformLocation(graphicProgramID, "uProjectionMatrix");
	locUniformSpriteTex = glGetUniformLocation(graphicProgramID, "uSpriteTex");

	locUniformAmortig = 1;
	
	return true;
}
 
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 Projection = glm::perspective(45.0f, 1.0f * g_Width / g_Height, 1.0f, 100.0f);
	
	glm::vec3 cameraPos = glm::vec3( 9.0f * cos( yrot / 100 ), 2.0f * sin(xrot / 100) + 2.0f, 9.0f * sin( yrot / 100 ) * cos(xrot /100) );
	glm::mat4 View = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 ModelCube = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)), glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 mvp; // Model-view-projection matrix
	glm::mat4 mv;  // Model-view matrix

	// Activate the program object with the compute shader
	glUseProgram(computeProgramID);

	glUniform1f(locUniformAmortig, amortiguacion);

	// Launch its execution, indicating the size of the dispatch
	glDispatchCompute(NUM_PARTICLES / WORK_GROUP_SIZE, 1, 1);

	// Place a memory barrier before activating the program object with the graphic shaders
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glUseProgram(graphicProgramID);

	// Draw Points
	mvp = Projection * View * ModelCube;
	mv = View * ModelCube;
	glUniformMatrix4fv( locUniformMVPM, 1, GL_FALSE, &mvp[0][0] );
	glUniformMatrix4fv( locUniformMVM, 1, GL_FALSE, &mv[0][0] );
	glUniformMatrix4fv( locUniformPM, 1, GL_FALSE, &Projection[0][0] );

	glUniform1i (locUniformSpriteTex, 0);

	drawPoints(NUM_PARTICLES);

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
	static GLboolean wireframe = false;
	switch(key)
	{
	case 27 : case 'q': case 'Q':
		exit(1); 
		break;
	case 'a': case 'A':
		animation = !animation;
		break;
	case '+':
		amortiguacion += 0.01;
		if (amortiguacion > 1.0) amortiguacion = 1.0;
		break;
	case '-':
		amortiguacion -= 0.01;
		if (amortiguacion < 0.0) amortiguacion = 0.0;
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

//-------------------------------------------------------------------------
//  Print FPS
//-------------------------------------------------------------------------
void printFPS()
{
	static GLint frameCount = 0;			//  N�mero de frames
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
