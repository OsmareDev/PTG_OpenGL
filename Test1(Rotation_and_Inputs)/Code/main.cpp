#include <iostream>
#include <GL/glut.h>

// Functions
void drawBox();
GLboolean initOpenGL();
void display();
void resize(GLint, GLint);
void inactive();
void keyboard(GLubyte, GLint, GLint);
void mouseClick(GLint, GLint, GLint, GLint);
void mouseMovement(GLint, GLint);
void printFPS();

// Variables
GLboolean fullscreen = false;
GLboolean mouseDown = false;
GLboolean animation = true;
 
GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat xdiff = 0.0f;
GLfloat ydiff = 0.0f;

GLint g_Width = 500;
GLint g_Height = 500;


GLint main(GLint argc, char **argv)
{
	glutInit(&argc, argv); 
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(g_Width, g_Height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("Test 1");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutIdleFunc(inactive);
	initOpenGL();
 
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMovement);

	glutMainLoop();
 
	return EXIT_SUCCESS;
}
 
void drawBox()
{
	glBegin(GL_QUADS);
 
		glColor3f(1.0f, 0.0f, 0.0f);
		// FRONT
		glVertex3f(-0.5f, -0.5f,  0.5f);
		glVertex3f( 0.5f, -0.5f,  0.5f);
		glVertex3f( 0.5f,  0.5f,  0.5f);
		glVertex3f(-0.5f,  0.5f,  0.5f);
		// BACK
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(-0.5f,  0.5f, -0.5f);
		glVertex3f( 0.5f,  0.5f, -0.5f);
		glVertex3f( 0.5f, -0.5f, -0.5f);
 
		glColor3f(0.0f, 1.0f, 0.0f);
		// LEFT
		glVertex3f(-0.5f, -0.5f,  0.5f);
		glVertex3f(-0.5f,  0.5f,  0.5f);
		glVertex3f(-0.5f,  0.5f, -0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		// RIGHT
		glVertex3f( 0.5f, -0.5f, -0.5f);
		glVertex3f( 0.5f,  0.5f, -0.5f);
		glVertex3f( 0.5f,  0.5f,  0.5f);
		glVertex3f( 0.5f, -0.5f,  0.5f);
 
		glColor3f(0.0f, 0.0f, 1.0f);
		// TOP
		glVertex3f(-0.5f,  0.5f,  0.5f);
		glVertex3f( 0.5f,  0.5f,  0.5f);
		glVertex3f( 0.5f,  0.5f, -0.5f);
		glVertex3f(-0.5f,  0.5f, -0.5f);
		// BOTTOM
		glVertex3f(-0.5f, -0.5f,  0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f( 0.5f, -0.5f, -0.5f);
		glVertex3f( 0.5f, -0.5f,  0.5f);
	glEnd();
	return;
}
 
GLboolean initOpenGL()
{
	glClearColor(0.93f, 0.93f, 0.93f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);
 
	return true;
}
 
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
 
	gluLookAt(
	0.0f, 0.0f, 3.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f);
 
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
 
	drawBox();
 
	printFPS();

	glutSwapBuffers();
	return;
}
 
void resize(GLint w, GLint h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	g_Width = w;
	g_Height = h;
	glViewport(0, 0, g_Width, g_Height);
 
	gluPerspective(45.0f, 1.0f * g_Width / g_Height, 1.0f, 100.0f);
 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	return;
}
 
void inactive()
{
	if (!mouseDown && animation)
	{
		xrot += 0.050f;
		yrot += 0.065f;
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
	}
	return;
}
 
void mouseClick(GLint button, GLint state, GLint x, GLint y)
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
 
void mouseMovement(GLint x, GLint y)
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
