#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vbotorus.h"
#include "lodepng.h"

GLint initPlane(GLfloat, GLfloat, GLint, GLint);
GLint initTorus(GLfloat, GLfloat, GLint, GLint);
void drawPlane();
void drawTorus();

void loadSource(GLuint &, std::string);
void printCompileInfoLog(GLuint);
void printLinkInfoLog(GLuint);
void validateProgram(GLuint);

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
GLint normal = 0, useCol = 0, useGol = 0, useHei = 0;
 
GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat xdiff = 0.0f;
GLfloat ydiff = 0.0f;

GLint g_Width = 500;
GLint g_Height = 500;

GLuint planeVAOHandle, torusVAOHandle;
GLuint programID;
GLuint locUniformMVPM, locUniformMVM, locUniformNM;
GLuint locUniformLightPos, locUniformLightIntensity;
GLuint locUniformMaterialAmbient, locUniformMaterialDiffuse, locUniformMaterialSpecular, locUniformMaterialShininess;
GLuint locUniformNormalMap, locUniformColorMap, locUniformGlossMap, locUniformHeightMap;

GLuint locUniformCam, locUniformUseNor, locUniformUseCol, locUniformUseGol, locUniformUseHei;

GLint numVertPlane, numVertTorus;

GLuint textIds[4];


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

GLint initPlane(GLfloat xsize, GLfloat zsize, GLint xdivs, GLint zdivs)
{
	const GLfloat TexScale = 2.0f;
	GLfloat * v = new GLfloat[3 * (xdivs + 1) * (zdivs + 1)];
	GLfloat * n = new GLfloat[3 * (xdivs + 1) * (zdivs + 1)];
	GLfloat * t = new GLfloat[3 * (xdivs + 1) * (zdivs + 1)];
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
			t[vidx] = -1.0f;
			t[vidx+1] = 0.0f;
			t[vidx+2] = 0.0f;
			vidx += 3;
			tex[tidx] = -j * texi * TexScale;
			tex[tidx+1] = i * texj * TexScale;
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

	GLuint handle[6];
	glGenBuffers(6, handle);

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
	glBufferData(GL_ARRAY_BUFFER, 3 * (xdivs+1) * (zdivs+1) * sizeof(GLfloat), t, GL_STATIC_DRAW);
	GLuint loc3 = glGetAttribLocation(programID, "aTangent");   
	glVertexAttribPointer( loc3, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
	glEnableVertexAttribArray(loc3);  // Vertex tangent

	glBindBuffer(GL_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ARRAY_BUFFER, 2 * (xdivs+1) * (zdivs+1) * sizeof(GLfloat), tex, GL_STATIC_DRAW);
	GLuint loc4 = glGetAttribLocation(programID, "aTexCoord");   
	glVertexAttribPointer( loc4, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
	glEnableVertexAttribArray(loc4);  // texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * xdivs * zdivs * sizeof(GLuint), el, GL_STATIC_DRAW);

	glBindVertexArray(0);
	
	delete [] v;
	delete [] n;
	delete [] t;
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
	// Tangent
	GLfloat * t = new GLfloat[3 * nVerts];
	// Tex coords
	GLfloat * tex = new GLfloat[2 * nVerts];
	// Elements
	GLuint * el = new GLuint[6 * faces];

	// Generate the vertex data
	generateVerts(v, n, t, tex, el, outerRadius, innerRadius, nrings, nsides);

	// Create and populate the buffer objects
	GLuint handle[5];
	glGenBuffers(5, handle);

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
	glBufferData(GL_ARRAY_BUFFER, (3 * nVerts) * sizeof(GLfloat), t, GL_STATIC_DRAW);
	GLuint loc3 = glGetAttribLocation(programID, "aTangent");   
	glVertexAttribPointer( loc3, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
	glEnableVertexAttribArray(loc3);  // Vertex tangent

	glBindBuffer(GL_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ARRAY_BUFFER, (2 * nVerts) * sizeof(GLfloat), tex, GL_STATIC_DRAW);
	GLuint loc4 = glGetAttribLocation(programID, "aTexCoord");
	glVertexAttribPointer( loc4, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
	glEnableVertexAttribArray(loc4);  // texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * faces * sizeof(GLuint), el, GL_STATIC_DRAW);

	glBindVertexArray(0);

	delete [] v;
	delete [] n;
	delete [] t;
	delete [] el;
	delete [] tex;

	return 6 * faces;
}

// END: Initialize primitives ////////////////////////////////////////////////////////////////////////////////////

// BEGIN: Drawing Functions ////////////////////////////////////////////////////////////////////////////////////

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
	glutCreateWindow("Test 8");
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
	loadSource(vertexShaderID, "../Shaders/normalMapping.vert");
	glCompileShader(vertexShaderID);
	printCompileInfoLog(vertexShaderID);
	glAttachShader(programID, vertexShaderID);

	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	loadSource(fragmentShaderID, "../Shaders/normalMapping.frag");
	glCompileShader(fragmentShaderID);
	printCompileInfoLog(fragmentShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);
	printLinkInfoLog(programID);
	validateProgram(programID);

	numVertPlane = initPlane(10.0f, 10.0f, 4, 4);
	numVertTorus = initTorus(0.5f, 0.25f, 20, 40);
	
	locUniformMVPM = glGetUniformLocation(programID, "uModelViewProjMatrix");
	locUniformMVM = glGetUniformLocation(programID, "uModelViewMatrix");
	locUniformNM = glGetUniformLocation(programID, "uNormalMatrix");

	locUniformLightPos = glGetUniformLocation(programID, "uLight.lightPos");
	locUniformLightIntensity = glGetUniformLocation(programID, "uLight.intensity");
	locUniformMaterialAmbient = glGetUniformLocation(programID, "uMaterial.ambient");
	locUniformMaterialDiffuse = glGetUniformLocation(programID, "uMaterial.diffuse");
	locUniformMaterialSpecular = glGetUniformLocation(programID, "uMaterial.specular");
	locUniformMaterialShininess = glGetUniformLocation(programID, "uMaterial.shininess");

	locUniformCam = glGetUniformLocation(programID, "uCamPos");
	locUniformUseNor = glGetUniformLocation(programID, "uUseNor");
	locUniformUseCol = glGetUniformLocation(programID, "uUseCol");
	locUniformUseGol = glGetUniformLocation(programID, "uUseGol");
	locUniformUseHei = glGetUniformLocation(programID, "uUseHei");


	// Location of the samplers with the maps
	locUniformNormalMap = glGetUniformLocation(programID, "uNormalMap");
	locUniformColorMap = glGetUniformLocation(programID, "uColorMap");
	locUniformGlossMap = glGetUniformLocation(programID, "uGlossMap");
	locUniformHeightMap = glGetUniformLocation(programID, "uHeightMap");
	
	// Texture Initialization
	std::vector<GLubyte> img_data;
	GLuint img_width, img_height;
	GLuint error;

	//const GLchar img_filename_normalmap2[] = "texturas/normal_map.png";

	const GLchar img_filename_normalmap[] = "texturas2/Tiles_09_normal.png";
	const GLchar img_filename_colormap[] = "texturas2/Tiles_09_color.png";
	const GLchar img_filename_heightmap[] = "texturas2/Tiles_09_height.png";
	const GLchar img_filename_glossmap[] = "texturas2/Tiles_09_gloss.png";



/*	
	const GLchar img_filename_normalmap[] = "texturas/GroundPeables02_normal.png";
	const GLchar img_filename_colormap[] = "texturas/GroundPeables02_color.png";
	const GLchar img_filename_heightmap[] = "texturas/GroundPeables02_height.png";
	const GLchar img_filename_glossmap[] = "texturas/GroundPeables02_gloss.png";

	const GLchar img_filename_normalmap[] = "texturas/Holidays_Cookies_normal.png";
	const GLchar img_filename_colormap[] = "texturas/Holidays_Cookies_color.png";
	const GLchar img_filename_heightmap[] = "texturas/Holidays_Cookies_height.png";
	const GLchar img_filename_glossmap[] = "texturas/Holidays_Cookies_gloss.png";

	const GLchar img_filename_normalmap[] = "texturas/Tiles_09_normal.png";
	const GLchar img_filename_colormap[] = "texturas/Tiles_09_color.png";
	const GLchar img_filename_heightmap[] = "texturas/Tiles_09_height.png";
	const GLchar img_filename_glossmap[] = "texturas/Tiles_09_gloss.png";

	const GLchar img_filename_normalmap[] = "texturas/Tiles_Bricks_01_normal.png";
	const GLchar img_filename_colormap[] = "texturas/Tiles_Bricks_01_color.png";
	const GLchar img_filename_heightmap[] = "texturas/Tiles_Bricks_01_height.png";
	const GLchar img_filename_glossmap[] = "texturas/Tiles_Bricks_01_gloss.png";

	const GLchar img_filename_normalmap[] = "texturas/Tiles_Floor_02_normal.png";
	const GLchar img_filename_colormap[] = "texturas/Tiles_Floor_02_color.png";
	const GLchar img_filename_heightmap[] = "texturas/Tiles_Floor_02_height.png";
	const GLchar img_filename_glossmap[] = "texturas/Tiles_Floor_02_gloss.png";
*/

	glGenTextures (4, textIds);
	
	// create the texture objects and load the images
	
	// Normal map
	error = lodepng::decode(img_data, img_width, img_height, img_filename_normalmap);
	if (!error)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textIds[0]);
		gluBuild2DMipmaps(GL_TEXTURE_2D,			// texture to specify
			GL_RGBA,								// internal texture storage format
			img_width,   							// texture width
			img_height,								// texture height
			GL_RGBA,								// pixel format
			GL_UNSIGNED_BYTE,						// color component format
			&img_data[0]);							// pointer to texture image
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else 
		std::cout << "Error al cargar la textura " << img_filename_normalmap << std::endl;
	img_data.clear();
	
	/*
	// Same for colormap, heightmap and glossmap
	*/
	// Color map
	error = lodepng::decode(img_data, img_width, img_height, img_filename_colormap);
	if (!error)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textIds[1]);
		gluBuild2DMipmaps(GL_TEXTURE_2D,			// texture to specify
			GL_RGBA,								// internal texture storage format
			img_width,   							// texture width
			img_height,								// texture height
			GL_RGBA,								// pixel format
			GL_UNSIGNED_BYTE,						// color component format
			&img_data[0]);							// pointer to texture image
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
		std::cout << "Error al cargar la textura " << img_filename_colormap << std::endl;
	img_data.clear();

	// Gloss map
	error = lodepng::decode(img_data, img_width, img_height, img_filename_glossmap);
	if (!error)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textIds[2]);
		gluBuild2DMipmaps(GL_TEXTURE_2D,			// texture to specify
			GL_RGBA,								// internal texture storage format
			img_width,   							// texture width
			img_height,								// texture height
			GL_RGBA,								// pixel format
			GL_UNSIGNED_BYTE,						// color component format
			&img_data[0]);							// pointer to texture image
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
		std::cout << "Error al cargar la textura " << img_filename_glossmap << std::endl;
	img_data.clear();

	// Height map
	error = lodepng::decode(img_data, img_width, img_height, img_filename_heightmap);
	if (!error)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textIds[3]);
		gluBuild2DMipmaps(GL_TEXTURE_2D,			// texture to specify
			GL_RGBA,								// internal texture storage format
			img_width,   							// texture width
			img_height,								// texture height
			GL_RGBA,								// pixel format
			GL_UNSIGNED_BYTE,						// color component format
			&img_data[0]);							// pointer to texture image
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
		std::cout << "Error al cargar la textura " << img_filename_heightmap << std::endl;
	img_data.clear();

	return true;
}
 
void display()
{
	struct LightInfo {
	 glm::vec4 lightPos;
	 glm::vec3 intensity;
	};
	LightInfo light = { glm::vec4(0.0f, 4.0f, 0.0f, 1.0f),
						glm::vec3(1.0f, 1.0f, 1.0f)
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
	
	glm::vec3 cameraPos = glm::vec3( 5.0f * sin( xrot / 200 ) * cos( yrot / 100 ) + 1.8f, 2.2f * cos(xrot / 200) + 2.5f, 5.0f * sin( xrot / 200 ) * sin( yrot / 100 ) + 1.8f );
	glm::mat4 View = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 ModelPlane = glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(1.0f, 1.0f, 1.0f)), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 ModelTorus = glm::translate(glm::scale(glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(1.0, 0.0, 0.0)), glm::vec3(2.0, 2.0, 2.0)), glm::vec3(0.0f, 0.0f, 0.25f));

	glm::mat4 mvp; // Model-view-projection matrix
	glm::mat4 mv;  // Model-view matrix
	glm::mat3 nm;  // Normal matrix

	glUseProgram(programID);
	
	//glm::cross(normal, tangente);

	// Go to the light properties shader
	glm::vec4 lpos = View * light.lightPos;	// Focal position in the S.E.R. view
	glm::vec3 cpos = glm::mat3(View) * cameraPos;

	glUniform3fv(locUniformLightIntensity, 1, &(light.intensity.r));
	glUniform3fv(locUniformCam, 1, &(cpos.x));
	glUniform4fv(locUniformLightPos, 1, &(lpos.x));
	glUniform1i(locUniformUseNor, normal);
	glUniform1i(locUniformUseCol, useCol);
	glUniform1i(locUniformUseGol, useGol);
	glUniform1i(locUniformUseHei, useHei);
	
	
	// Pass other uniform variables common to the two objects
	glUniform1i (locUniformNormalMap, 0);	// GL_TEXURE0
	glUniform1i (locUniformColorMap, 1);	// GL_TEXURE1
	glUniform1i(locUniformGlossMap, 2);	// GL_TEXURE1
	glUniform1i (locUniformHeightMap, 3);	// GL_TEXURE1
	


	// Draw Donut
	mvp = Projection * View * ModelTorus;
	mv = View * ModelTorus;
	nm = glm::mat3(glm::transpose(glm::inverse(mv)));

	glUniformMatrix4fv( locUniformMVPM, 1, GL_FALSE, &mvp[0][0] );
	glUniformMatrix4fv( locUniformMVM, 1, GL_FALSE, &mv[0][0] );
	glUniformMatrix3fv( locUniformNM, 1, GL_FALSE, &nm[0][0] );
	glUniform3fv(locUniformMaterialAmbient, 1, &(emerald.ambient.r));
	glUniform3fv(locUniformMaterialDiffuse, 1, &(emerald.diffuse.r));
	glUniform3fv(locUniformMaterialSpecular, 1, &(emerald.specular.r));
	glUniform1f(locUniformMaterialShininess, emerald.shininess);
	// We pass the position of the camera and light into object coordinates
	drawTorus();

	// Draw Plano
	mvp = Projection * View * ModelPlane;
	mv = View * ModelPlane;
	nm = glm::mat3(glm::transpose(glm::inverse(mv)));

	glUniformMatrix4fv( locUniformMVPM, 1, GL_FALSE, &mvp[0][0] );
	glUniformMatrix4fv( locUniformMVM, 1, GL_FALSE, &mv[0][0] );
	glUniformMatrix3fv( locUniformNM, 1, GL_FALSE, &nm[0][0] );
	glUniform3fv(locUniformMaterialAmbient, 1, &(brass.ambient.r));
	glUniform3fv(locUniformMaterialDiffuse, 1, &(brass.diffuse.r));
	glUniform3fv(locUniformMaterialSpecular, 1, &(brass.specular.r));
	glUniform1f(locUniformMaterialShininess, brass.shininess);
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
	case 'n': case 'N':
		normal = (normal + 1) % 2;
		break;
	case 'c': case 'C':
		useCol = (useCol + 1) % 2;
		break;
	case 'b': case 'B':
		useGol = (useGol + 1) % 2;
		break;
	case 'p': case 'P':
		useHei = (useHei + 1) % 3;
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
