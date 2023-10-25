// Óscar Marín Egea

#include <iostream>
#include <GL/glut.h>

void drawBox();
void drawScene();
GLboolean init();
void display();
void resize(GLint, GLint);
void idle();
void keyboard(GLubyte, GLint, GLint);
void specialKeyboard(GLint, GLint, GLint);
void mouse(GLint, GLint, GLint, GLint);
void mouseMotion(GLint, GLint);
void printFPS();

// Variables globales (estados globales de la aplicación)
GLboolean fullscreen = false;
GLboolean mouseDown = false;
GLboolean animation = true;
GLboolean nowired = true;
GLboolean perspective = true;

GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat xdiff = 0.0f;
GLfloat ydiff = 0.0f;

GLfloat earthRot = 0.0f;
GLfloat earthSunRot = 0.0f;
GLfloat moonEarthRot = 0.0f;
GLfloat satelEarthRot = 0.0f;

GLfloat Kw = 100.0f;

GLfloat earthW = 1.0f * Kw;
GLfloat earthSunW = (1.0f / 365.0f) * Kw;
GLfloat moonEarthW = (1.0f / 28.0f) * Kw;
GLfloat satelEarthW = 2.0f * Kw;
GLfloat geoStatW = 1.0f * Kw;

GLint g_Width = 500;                          // Ancho inicial de la ventana
GLint g_Height = 500;                         // Altura incial de la ventana


GLint main(GLint argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(g_Width, g_Height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("Ejercicios tema 1 - parte 1");
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

void drawBox()
{
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.0f, 0.0f, 1.0f);
	//TOP
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);



	glColor3f(0.0f, 1.0f, 0.0f);
	// RIGHT
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);

	glColor3f(0.0f, 0.0f, 1.0f);
	// BOTTOM
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(1.0f, 0.0f, 0.0f);
	// BACK
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);

	glColor3f(0.0f, 1.0f, 0.0f);
	// LEFT
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	glColor3f(1.0f, 0.0f, 0.0f);
	// FRONT
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glEnd();

	return;
}

void drawScene() {

	glPushMatrix();

	// escalamos los cubos
	glScalef(0.4f, 0.4f, 0.4f);
	// el sol
	drawBox();

	// tierra
	glRotatef(earthSunRot, 0.0f, 1.0f, 0.0f);
	glTranslatef(2.0f, 0.0f, 0.0f);

	glPushMatrix();
	glRotatef(earthRot, 0.0f, 1.0f, 0.0f);
	glScalef(0.5f, 0.5f, 0.5f);
	drawBox();

	//geostacionario
	glPushMatrix();
	//glRotatef(10.0f, 1.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, 1.2f);
	glScalef(0.2f, 0.2f, 0.2f);
	drawBox();
	glPopMatrix();
	glPopMatrix();

	// luna
	glPushMatrix();
	glRotatef(moonEarthRot, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.9f, 0.0f, 0.0f);
	glScalef(0.3f, 0.3f, 0.3f);
	drawBox();
	glPopMatrix();

	// satelite vertical
	glPushMatrix();
	glRotatef(satelEarthRot, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.8f, 0.0f);
	glScalef(0.1f, 0.1f, 0.1f);
	drawBox();
	glPopMatrix();
	glPopMatrix();
}

GLboolean init()
{
	glClearColor(0.93f, 0.93f, 0.93f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);

	glShadeModel(GL_FLAT);

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

	// rotacion del raton
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);

	// escena a dibujar
	drawScene();

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

	if (perspective)
		gluPerspective(45.0f, 1.0f * g_Width / g_Height, 1.0f, 100.0f);
	else
		glOrtho(-1.0f * g_Width / g_Height, 1.0f * g_Width / g_Height, -1.0f, 1.0f, 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	return;
}

void idle()
{
	if (!mouseDown && animation)
	{
		// xrot += 0.050f; // 0.3f
		// yrot += 0.065f; // 0.4f
	}


	GLfloat t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

	earthRot = t * earthW;
	earthSunRot = t * earthSunW;
	moonEarthRot = t * moonEarthW;
	satelEarthRot = t * satelEarthW;


	glutPostRedisplay();
	printFPS();
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
		animation = !animation;
		break;

	case 'v': case 'V':
		nowired = !nowired;

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE + nowired);
		/*
		if(!wired)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		*/
		break;
	case 'x': case 'X':
		perspective = !perspective;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glViewport(0, 0, g_Width, g_Height);

		if (perspective)
			gluPerspective(45.0f, 1.0f * g_Width / g_Height, 1.0f, 100.0f);
		else
			glOrtho(-1.0f * g_Width / g_Height, 1.0f * g_Width / g_Height, -1.0f, 1.0f, 1.0f, 100.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

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
	static GLint frameCount = 0;			//  Número de frames
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