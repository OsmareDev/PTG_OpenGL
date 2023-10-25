// Nombre: Oscar Marin Egea

#include <GL/glut.h>
#include <vector>
#include "lodepng.h"
#include <iostream>

// Prototipos de las funciones

GLboolean init();
void display();
void resize(GLint, GLint);
void idle();
void keyboard(GLubyte, GLint, GLint);
void specialKeyboard(GLint, GLint, GLint);
void mouse(GLint, GLint, GLint, GLint);
void mouseMotion(GLint, GLint);

// Variables globales con los estados globales de la aplicación

GLboolean fullscreen = false;
GLboolean mouseDown = false;
GLboolean animation = false;

GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat xdiff = 0.0f;
GLfloat ydiff = 0.0f;

GLint g_Width = 500;                          // Ancho inicial de la ventana
GLint g_Height = 500;                         // Altura incial de la ventana


GLuint textures;

GLint main(GLint argc, char **argv)
{
	glutInit(&argc, argv); 
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(g_Width, g_Height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Programa Ejemplo");
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

void drawPlane()
{
	const GLuint numDivisiones = 100;
	const GLfloat TAM = 3.0f;
	for (GLuint i = 0; i < numDivisiones; i ++ )
	{
		GLfloat y0 = TAM * GLfloat(i) / numDivisiones - TAM / 2.0f;
		GLfloat yf = TAM * GLfloat(i+1) / numDivisiones - TAM / 2.0f;
		glBegin(GL_TRIANGLE_STRIP);
		for ( GLint j = 0; j <= numDivisiones; j ++ )
		{
			GLfloat x = TAM * GLfloat(j) / numDivisiones - TAM / 2.0f;
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(x,y0);
			glVertex3f(x, y0, 0.0f);
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(x,yf);
			glVertex3f(x, yf, 0.0f);
		}
		glEnd();
	}
	return;
}

GLboolean init()
{
	const GLfloat luzLocal[] = { 0.5f, 0.5f, 1.0f, 1.0f };
	const GLfloat luzDireccional[] = { 0.5f, 0.5f, 1.0f, 0.0f };
	const GLfloat direccionFoco[] = { -0.5f, -0.5f, -3.0f };
	const GLfloat amplitudFoco = 10.0f;							// En grados
	const GLfloat intensidadLuz[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Color de la parte ambiente, difusa y especular de la luz.

	glClearColor(0.93f, 0.93f, 0.93f, 0.0f);
 
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearDepth(1.0f);
	
	// Tarea por hacer: Activar la iluminación.
	glEnable(GL_LIGHTING);
	
	// Tarea por hacer: definir 3 luces.
	// 1º luz: puntual
	glLightfv(GL_LIGHT0, GL_POSITION, luzLocal);
	glLightfv(GL_LIGHT0, GL_AMBIENT, intensidadLuz);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, intensidadLuz);
	glLightfv(GL_LIGHT0, GL_SPECULAR, intensidadLuz);
	glEnable(GL_LIGHT0);

	// 2º luz: direccional
	glLightfv(GL_LIGHT1, GL_POSITION, luzDireccional);
	glLightfv(GL_LIGHT1, GL_AMBIENT, intensidadLuz);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, intensidadLuz);
	glLightfv(GL_LIGHT1, GL_SPECULAR, intensidadLuz);
	//glEnable(GL_LIGHT1);

	// 3º luz: foco
	glLightfv(GL_LIGHT2, GL_POSITION, luzLocal);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direccionFoco);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, amplitudFoco);
	glLightfv(GL_LIGHT2, GL_AMBIENT, intensidadLuz);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, intensidadLuz);
	glLightfv(GL_LIGHT2, GL_SPECULAR, intensidadLuz);
	//glEnable(GL_LIGHT2);

	// Tarea por hacer: Crear el obj. textura, cargar la imagen png, activar la textura, 
	// cargar los datos en el objeto textura y definir filtros, modo repetición y modo pegado

	
	std::vector<GLubyte> img_data;
	GLuint img_width, img_height;

	const char* img_filename = "../Textures/Copper Rough_diffuse.png";

	std::cout << "Cargando la textura " << img_filename << std::endl;
	GLuint error = lodepng::decode(img_data, img_width, img_height, img_filename);

	if ( !error )
	{
		//...
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &textures);
		glBindTexture(GL_TEXTURE_2D, textures);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img_data[0]);
		gluBuild2DMipmaps(GL_TEXTURE_2D,			// texture to specify
				GL_RGBA,				// internal texture storage format
				img_width,			// texture width
				img_height,			// texture height
				GL_RGBA,				// pixel format
				GL_UNSIGNED_BYTE,	// color component format
				&img_data[0]);		// pointer to texture image
	   
		//modo de pegado
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		std::cout << "tamanyo de la textura (w,h) = (" << img_width << "," << img_height << ")" << std::endl;
		
		//...
	}
	else
	   std::cout << "Error al cargar la textura" << img_filename << std::endl;
	
	return true;
}
 
void display()
{
	GLfloat bronzeColorAmb[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat bronzeColorDiff[4] = { 0.8f, 0.6f, 0.0f, 1.0f };
	GLfloat bronzeColorSpec[4] = { 1.0f, 1.0f, 0.8f, 1.0f };
	GLfloat bronzeShininess = 50;
	GLfloat colorNone[4]       = { 0.0f, 0.0f, 0.0f, 0.0f };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluLookAt(
	0.0f, 0.0f, 3.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f);

	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);

	glColor4fv(bronzeColorDiff); // Color predefinidos de los objetos
	
	// Tarea por hacer: Activar el material antes de dibujar los objetos de la escena.

	glMaterialfv(GL_FRONT, GL_AMBIENT, bronzeColorAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, bronzeColorDiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, bronzeColorSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, bronzeShininess);

	glBindTexture(GL_TEXTURE_2D, textures);
	
	drawPlane();
	glutSolidTeapot(0.5f);
	//glutSolidCube(0.5f);
	//glutSolidSphere(0.5f, 20, 20);
	//glutSolidCone(0.25f, 0.5f, 20, 1);
	//glutSolidTorus(0.25, 0.5, 10, 20);

	glPopMatrix();
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
	static GLint tipoLuz = 0; // 0 - local, 1 - Direccional, 2- Foco
	const GLint NUM_TIPOS_LUZ = 3;
	static GLint modoPegadoTextura = 0; // 0 - GL_REPLACE, 1 - GL_MODULATE
	const GLint NUM_TIPOS_PEGADO = 2;
	static GLboolean  modoRepeticion = true;
	static GLint filtro = 0;
	const GLint NUM_TIPOS_FILTRO = 3;

	static GLboolean luces = true;
	static GLboolean smooth = true;
	static GLboolean texture = true;

	switch(key)
	{
	case 27 : case 'q': case 'Q':
		exit(EXIT_SUCCESS); 
		break;
	case 'a': case 'A':
		animation = !animation;
		break;
	case 'l': case 'L':
		luces = !luces;
		if (luces) {
			glEnable(GL_LIGHTING);
		}
		else {
			glDisable(GL_LIGHTING);
		}
		break;
	case 's': case 'S':
		smooth = !smooth;
		glShadeModel(GL_FLAT+smooth);
		break;
	case 't': case 'T':
		tipoLuz++;
		glEnable(GL_LIGHT0 + (tipoLuz % NUM_TIPOS_LUZ));
		glDisable(GL_LIGHT0 + ((tipoLuz - 1) % NUM_TIPOS_LUZ));
		break;
	case 'x': case 'X':
		texture = !texture;
		if (texture)
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
		break;
	case 'p': case 'P':
		tipoLuz++;
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE + 767*(tipoLuz % NUM_TIPOS_PEGADO));
		break;
	case 'e': case 'E':
		modoRepeticion = !modoRepeticion;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP + modoRepeticion);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP + modoRepeticion);
		break;
	case 'f': case 'F':
		filtro = (filtro+1) % NUM_TIPOS_FILTRO;
		if (filtro == 0) {
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else if (filtro == 1) {
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		else {
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
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
