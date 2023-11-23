// Oscar Marin Egea
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "lodepng.h"


void printFPS();

void draw1Cube();
void drawMultipleCubes();

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
 
GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat xdiff = 0.0f;
GLfloat ydiff = 0.0f;

GLint g_Width = 500;
GLint g_Height = 500;

GLboolean textureOn = false;
GLuint opcion = 0;

GLuint textures;
GLuint VBOBuffers1[3];
GLuint VBOBuffers2[3];
GLuint VBOBuffers3;
GLuint VBOIndex;
GLuint vaoHandle[3];

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
// vertex has 3 real components (x,y,z), therefore, the size of the vertex
// array is 108 floats (36 * 3 = 108).
const GLfloat vertices1[] = { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,      // v0-v1-v2 (front)
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
// color array
const GLfloat colores1[]   = { 1, 1, 1,   1, 1, 0,   1, 0, 0,      // v0-v1-v2 (front)
						1, 0, 0,   1, 0, 1,   1, 1, 1,      // v2-v3-v0

						1, 1, 1,   1, 0, 1,   0, 0, 1,      // v0-v3-v4 (right)
						0, 0, 1,   0, 1, 1,   1, 1, 1,      // v4-v5-v0

						1, 1, 1,   0, 1, 1,   0, 1, 0,      // v0-v5-v6 (top)
						0, 1, 0,   1, 1, 0,   1, 1, 1,      // v6-v1-v0

						1, 1, 0,   0, 1, 0,   0, 0, 0,      // v1-v6-v7 (left)
						0, 0, 0,   1, 0, 0,   1, 1, 0,      // v7-v2-v1

						0, 0, 0,   0, 0, 1,   1, 0, 1,      // v7-v4-v3 (bottom)
						1, 0, 1,   1, 0, 0,   0, 0, 0,      // v3-v2-v7

						0, 0, 1,   0, 0, 0,   0, 1, 0,      // v4-v7-v6 (back)
						0, 1, 0,   0, 1, 1,   0, 0, 1 };    // v6-v5-v4
// texture coord. array
const GLfloat texcoord1[] = { 1, 1,   0, 1,  0, 0,      // v0-v1-v2 (front)
						0, 0,   1, 0,  1, 1,      // v2-v3-v0

						1, 1,   0, 1,   0, 0,      // v0-v3-v4 (right)
						0, 0,   1, 0,   1, 1,      // v4-v5-v0

						1, 1,   1, 0,   0, 0,      // v0-v5-v6 (top)
						0, 0,   0, 1,   1, 1,      // v6-v1-v0

						1, 1,   1, 0,   0, 0,      // v1-v6-v7 (left)
						0, 0,   0, 1,   1, 1,      // v7-v2-v1

						0, 0,   1, 0,   1, 1,      // v7-v4-v3 (bottom)
						1, 1,   0, 1,   0, 0,      // v3-v2-v7

						1, 0,   0, 0,   0, 1,      // v4-v7-v6 (back)
						0, 1,   1, 1,   1, 0 };    // v6-v5-v4


// Coordinates of the vertex array eliminating repeated vertices (more compact)
const GLfloat vertices2[]= { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1,   // v0,v1,v2,v3 (front)
						1,-1,-1,   1, 1,-1,  -1, 1,-1,  -1,-1,-1};	// v4,v5,v6,v7 (back)

// color array
const GLfloat colores2[]  = { 1, 1, 1,   1, 1, 0,   1, 0, 0,   1, 0, 1,   // v0,v1,v2,v3 (front)
						0, 0, 1,   0, 1, 1,   0, 1, 0,   0, 0, 0};	// v4,v5,v6,v7 (back)

// texture coord. array
const GLfloat texcoord2[] = { 1, 1,   0, 1,   0, 0,   1, 0,		// v0,v1,v2,v3 (front)
						1, 1,   1, 0,   0, 0,   0, 1 };		// v4,v5,v6,v7 (back)

// index array del vertex array
const GLubyte indices[] = { 0, 1, 2,   2, 3, 0,      // front
					   0, 3, 4,   4, 5, 0,      // right
					   0, 5, 6,   6, 1, 0,      // top
					   1, 6, 7,   7, 2, 1,      // left
					   7, 4, 3,   3, 2, 7,      // bottom
					   4, 7, 6,   6, 5, 4 };    // back

// Vertex interleaved array
// The two attributes (position and color) are packaged together. The result is
// something similar to a set of structures: ((V,C,T), (V,C,T), (V,C,T),...).
const GLfloat vertices3[]= {	 1, 1, 1,  1, 1, 1,	 1, 1,	// v0
						-1, 1, 1,  1, 1, 0,	 0, 1,	// v1
						-1,-1, 1,  1, 0, 0,	 0, 0,	// v2
						 1,-1, 1,  1, 0, 1,	 1, 0,	// v3
						 1,-1,-1,  0, 0, 1,	 1, 1,	// v4
						 1, 1,-1,  0, 1, 1,	 1, 0,	// v5
						-1, 1,-1,  0, 1, 0,	 0, 0,	// v6
						-1,-1,-1,  0, 0, 0,  0, 1};	// v7


int main(int argc, char *argv[])
{
	glutInit(&argc, argv); 
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(g_Width, g_Height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Tema 1, parte 3");
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

///////////////////////////////////////////////////////////////////////////////
// draw 1 cube: immediate mode
// 108 calls = 36 glVertex*() + 36 glColor*() + 36 glTexCoord*()
///////////////////////////////////////////////////////////////////////////////
void draw1Cube()
{
	glBegin(GL_TRIANGLES);
		
		// front face

		// triangle v0-v1-v2
		glColor3f(1, 1, 1);
		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 1);

		glColor3f(1, 1, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-1, 1, 1);

		glColor3f(1, 0, 0);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, 1);

		// triangle v2-v3-v0
		glColor3f(1, 0, 0);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, 1);

		glColor3f(1, 0, 1);
		glTexCoord2f(1, 0);
		glVertex3f(1, -1, 1);

		glColor3f(1, 1, 1);
		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 1);

		// right face

		// triangle v0-v3-v4
		glColor3f(1, 1, 1);
		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 1);

		glColor3f(1, 0, 1);
		glTexCoord2f(0, 1);
		glVertex3f(1, -1, 1);

		glColor3f(0, 0, 1);
		glTexCoord2f(0, 0);
		glVertex3f(1, -1, -1);

		// triangle v4-v5-v0
		glColor3f(0, 0, 1);
		glTexCoord2f(0, 0);
		glVertex3f(1, -1, -1);

		glColor3f(0, 1, 1);
		glTexCoord2f(1, 0);
		glVertex3f(1, 1, -1);

		glColor3f(1, 1, 1);
		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 1);

		// top face

		// triangle v0-v5-v6
		glColor3f(1, 1, 1);
		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 1);

		glColor3f(0, 1, 1);
		glTexCoord2f(1, 0);
		glVertex3f(1, 1, -1);

		glColor3f(0, 1, 0);
		glTexCoord2f(0, 0);
		glVertex3f(-1, 1, -1);

		// triangle v6-v1-v0
		glColor3f(0, 1, 0);
		glTexCoord2f(0, 0);
		glVertex3f(-1, 1, -1);

		glColor3f(1, 1, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-1, 1, 1);

		glColor3f(1, 1, 1);
		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 1);

		// left face

		// triangle  v1-v6-v7
		glColor3f(1, 1, 0);
		glTexCoord2f(1, 1);
		glVertex3f(-1, 1, 1);

		glColor3f(0, 1, 0);
		glTexCoord2f(1, 0);
		glVertex3f(-1, 1, -1);

		glColor3f(0, 0, 0);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, -1);

		// triangle v7-v2-v1
		glColor3f(0, 0, 0);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, -1);

		glColor3f(1, 0, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-1, -1, 1);

		glColor3f(1, 1, 0);
		glTexCoord2f(1, 1);
		glVertex3f(-1, 1, 1);

		// bottom face
		// triangle v7-v4-v3
		glColor3f(0, 0, 0);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, -1);

		glColor3f(0, 0, 1);
		glTexCoord2f(1, 0);
		glVertex3f(1, -1, -1);

		glColor3f(1, 0, 1);
		glTexCoord2f(1, 1);
		glVertex3f(1, -1, 1);

		// triangle v3-v2-v7
		glColor3f(1, 0, 1);
		glTexCoord2f(1, 1);
		glVertex3f(1, -1, 1);

		glColor3f(1, 0, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-1, -1, 1);

		glColor3f(0, 0, 0);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, -1);

		// back face
		// triangle v4-v7-v6
		glColor3f(0, 0, 1);
		glTexCoord2f(1, 0);
		glVertex3f(1, -1, -1);

		glColor3f(0, 0, 0);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, -1);

		glColor3f(0, 1, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-1, 1, -1);

		// triangle v6-v5-v4
		glColor3f(0, 1, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-1, 1, -1);

		glColor3f(0, 1, 1);
		glTexCoord2f(1, 1);
		glVertex3f(1, 1, -1);

		glColor3f(0, 0, 1);
		glTexCoord2f(1, 0);
		glVertex3f(1, -1, -1);
		
	glEnd();
	return;
}

void stateVA1() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices1);
	glColorPointer(3, GL_FLOAT, 0, colores1);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoord1);
}

void cubeVA1() {
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void disableIt() {
	glBindVertexArray(0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void stateVA2() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices2);
	glColorPointer(3, GL_FLOAT, 0, colores2);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoord2);
}

void cubeVA2() {
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
}

void stateVA3() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 8 * sizeof(GL_FLOAT), vertices3);
	glColorPointer(3, GL_FLOAT, 8 * sizeof(GL_FLOAT), &vertices3[3]);
	glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(GL_FLOAT), &vertices3[6]);
}

void cubeVA3() {
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
}

void stateVBO1() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers1[0]);
	glVertexPointer(3, GL_FLOAT, 0, (char*)NULL + 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers1[1]);
	glColorPointer(3, GL_FLOAT, 0, (char*)NULL + 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers1[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, (char*)NULL + 0);
}

void cubeVBO1() {
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void stateVBO2() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOIndex);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers2[0]);
	glVertexPointer(3, GL_FLOAT, 0, (char*)NULL + 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers2[1]);
	glColorPointer(3, GL_FLOAT, 0, (char*)NULL + 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers2[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, (char*)NULL + 0);
}

void cubeVBO2() {
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
}

void stateVBO3() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOIndex);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers3);
	glVertexPointer(3, GL_FLOAT, 8 * sizeof(GL_FLOAT), (char*)NULL + 0);
	glColorPointer(3, GL_FLOAT, 8 * sizeof(GL_FLOAT), (char*)NULL + 12);
	glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(GL_FLOAT), (char*)NULL + 24);
}

void cubeVBO3() {
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
}

void stateVAO1() {
	glBindVertexArray(vaoHandle[0]);
}

void cubeVAO1() {
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void stateVAO2() {
	glBindVertexArray(vaoHandle[1]);
}

void cubeVAO2() {
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
}

void stateVAO3() {
	glBindVertexArray(vaoHandle[2]);
}

void cubeVAO3() {
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
}

////////////////////////////////////////////////////////////////////////////////////
// Draw multiple cubes
////////////////////////////////////////////////////////////////////////////////////
void drawMultipleCubes()
{
	const GLint N = 40; // N�mero de cubos en cada eje
	const GLfloat RangoIni = -1.0f;
	const GLfloat RangoFin =  1.0f;
	GLfloat x, y, z;
	GLfloat scale = (RangoFin - RangoIni) / (N * 3);

	switch (opcion)
	{
	case 1:
		stateVA1();
		break;
	case 2:
		stateVA2();
		break;
	case 3:
		stateVA3();
		break;
	case 4:
		stateVBO1();
		break;
	case 5:
		stateVBO2();
		break;
	case 6:
		stateVBO3();
		break;
	case 7:
		stateVAO1();
		break;
	case 8:
		stateVAO2();
		break;
	case 9:
		stateVAO3();
		break;
	}

	for ( GLint i = 0; i < N; i ++ )
	{
		x = RangoIni + i * (RangoFin - RangoIni) / (N - 1);
		for ( GLint j = 0; j < N; j ++ )
		{
			y = RangoIni + j * (RangoFin - RangoIni) / (N - 1);
			for ( GLint k = 0; k < N; k ++ )
			{
				z = RangoIni + k * (RangoFin - RangoIni) / (N - 1);
				glPushMatrix();
					glTranslatef(x,y,z);
					glScalef(scale,scale,scale);
					switch (opcion)
					{
					case 0:
						draw1Cube();
						break;
					case 1:
						cubeVA1();
						break;
					case 2:
						cubeVA2();
						break;
					case 3:
						cubeVA3();
						break;
					case 4:
						cubeVBO1();
						break;
					case 5:
						cubeVBO2();
						break;
					case 6:
						cubeVBO3();
						break;
					case 7:
						cubeVAO1();
						break;
					case 8:
						cubeVAO2();
						break;
					case 9:
						cubeVAO3();
						break;
					}
				glPopMatrix();
			}
		}
	}

	disableIt();
	return;
}

void initVBO() {

	// 1
	glGenBuffers(3, VBOBuffers1);
	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers1[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 108, vertices1, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers1[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 108, colores1, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers1[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 72, texcoord1, GL_STATIC_DRAW);

	// 2
	glGenBuffers(3, VBOBuffers2);
	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers2[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 24, vertices2, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers2[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 24, colores2, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers2[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 16, texcoord2, GL_STATIC_DRAW);

	// 3
	glGenBuffers(1, &VBOBuffers3);
	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 64, vertices3, GL_STATIC_DRAW);

	glGenBuffers(1, &VBOIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 36, indices, GL_STATIC_DRAW);
}

void initVAO() {
	
	glGenVertexArrays(3, vaoHandle);

	// 1
	glBindVertexArray(vaoHandle[0]);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glGenBuffers(3, VBOBuffers1);
	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers1[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 108, vertices1, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, (char*)NULL + 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers1[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 108, colores1, GL_STATIC_DRAW);
	glColorPointer(3, GL_FLOAT, 0, (char*)NULL + 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers1[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 72, texcoord1, GL_STATIC_DRAW);
	glTexCoordPointer(2, GL_FLOAT, 0, (char*)NULL + 0);

	// 2
	glBindVertexArray(vaoHandle[1]);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glGenBuffers(3, VBOBuffers2);
	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers2[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 24, vertices2, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, (char*)NULL + 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers2[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 24, colores2, GL_STATIC_DRAW);
	glColorPointer(3, GL_FLOAT, 0, (char*)NULL + 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers2[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 16, texcoord2, GL_STATIC_DRAW);
	glTexCoordPointer(2, GL_FLOAT, 0, (char*)NULL + 0);

	glGenBuffers(1, &VBOIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 36, indices, GL_STATIC_DRAW);

	// 3
	glBindVertexArray(vaoHandle[2]);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glGenBuffers(1, &VBOBuffers3);
	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 64, vertices3, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOIndex);
	glVertexPointer(3, GL_FLOAT, 8 * sizeof(GL_FLOAT), (char*)NULL + 0);
	glColorPointer(3, GL_FLOAT, 8 * sizeof(GL_FLOAT), (char*)NULL + 12);
	glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(GL_FLOAT), (char*)NULL + 24);

	glBindVertexArray(0);
}

GLboolean init()
{
	glClearColor(0.93f, 0.93f, 0.93f, 0.0f);
 
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearDepth(1.0f);

	
	// Load and initialize the texture
	std::vector<GLubyte> img_data;
	GLuint img_width, img_height;
	const char* img_filename = "../Textures/wood_box.png";

	std::cout << "Cargando la textura " << img_filename << std::endl;
	unsigned int error = lodepng::decode(img_data, img_width, img_height, img_filename);

	if (!error)
	{
		//...
		glGenTextures(1, &textures);
		glBindTexture(GL_TEXTURE_2D, textures);

		gluBuild2DMipmaps(GL_TEXTURE_2D,			// texture to specify
			GL_RGBA,								// internal texture storage format
			img_width,								// texture width
			img_height,								// texture height
			GL_RGBA,								// pixel format
			GL_UNSIGNED_BYTE,						// color component format
			&img_data[0]);							// pointer to texture image

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		//...
	}
	else
		std::cout << "Error al cargar la textura" << img_filename << std::endl;
	
	//initVBO();
	initVAO();

	return true;
}
 
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(
	0.0f, 0.0f, 5.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f);

	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);

	//draw1Cube();
	drawMultipleCubes();

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
 
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27 : case 'q': case 'Q':
			//exit(EXIT_SUCCESS); 
			break;
		case 'a': case 'A':
			animation = !animation;
			break;
		case 'x': case 'X': // Activa/desactiva textura
			textureOn = !textureOn;
			if (textureOn)
				glEnable(GL_TEXTURE_2D);
			else
				glDisable(GL_TEXTURE_2D);
			break;
		case '0': case '1': case '2': case '3': case '4': 
		case '5': case '6': case '7': case '8': case '9':
			opcion = key - '0';
			std::cout << key << std::endl;
			break;
	}

	switch (opcion)
	{
	case 0:
		std::cout << "inmediate mode" << std::endl;
		break;
	case 1:
		std::cout << "3 vertex array" << std::endl;
		break;
	case 2:
		std::cout << "3 vertex array with index array" << std::endl;
		break;
	case 3:
		std::cout << "1 vertex array with index array" << std::endl;
		break;
	case 4:
		std::cout << "3 VBO" << std::endl;
		break;
	case 5:
		std::cout << "3 VBO with index array" << std::endl;
		break;
	case 6:
		std::cout << "1 VBO with index array" << std::endl;
		break;
	case 7:
		std::cout << "3 VAO" << std::endl;
		break;
	case 8:
		std::cout << "3 VAO with index array" << std::endl;
		break;
	case 9:
		std::cout << "1 VAO with index array" << std::endl;
		break;
	}

	

	return;
}
 
void specialKeyboard(int key, int x, int y)
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
 
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = true;
 
		xdiff = x - yrot;
		ydiff = -y + xrot;
	}
	else
		mouseDown = false;
}
 
void mouseMotion(int x, int y)
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
	static int frameCount = 0;
	static float fps = 0;
	static int currentTime = 0, previousTime = 0;

	frameCount++;

	currentTime = glutGet(GLUT_ELAPSED_TIME);

	int timeInterval = currentTime - previousTime;

	if (timeInterval > 1000)
	{
		fps = frameCount / (timeInterval / 1000.0f);

		previousTime = currentTime;

		frameCount = 0;

		std::cout.precision(4);
		std::cout << "FPS: " << fps << std::endl;
	}
}
