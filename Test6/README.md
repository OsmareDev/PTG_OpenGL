# README

## Exercise Code Explanation

In this README, we will provide a detailed explanation of the code provided. The code appears to be related to graphics programming using OpenGL and GLSL shaders. Let's break down the code, including functions, variables, and their roles.

### Table of Contents
1. [Introduction](#introduction)
2. [Dependencies](#dependencies)
3. [Global Variables](#global-variables)
4. [Shader Loading Functions](#shader-loading-functions)
5. [Primitive Initialization Functions](#primitive-initialization-functions)
6. [Drawing Functions](#drawing-functions)
7. [Main Function](#main-function)
8. [Initialization Function](#initialization-function)
9. [Display Function](#display-function)

### 1. Introduction <a name="introduction"></a>

The program sets up an OpenGL environment using GLUT and GLEW libraries to manage the window and OpenGL functions. It creates and renders 3D objects like cubes, spheres, and a teapot using different shaders and texture mapping. Key features of the program include:

    Multiple Shaders: The code loads different vertex and fragment shaders to render the objects with varying effects, including mixing textures, reflection, and refraction.
    Texture Mapping: It demonstrates texture mapping by loading stone and moss textures for objects.
    Environment Mapping: It showcases environment mapping by using a cube map for skybox reflection.
    User Interaction: The program allows user interaction, including mouse movement and keyboard input for controlling camera angles.
    Dynamic Effects: It simulates dynamic effects such as refraction and Fresnel effects on transparent objects.
    3D Primitives: The program initializes and renders 3D objects, including cubes, spheres, and a teapot.

### 2. Dependencies <a name="dependencies"></a>

The code relies on the following libraries and dependencies:

    OpenGL: The core graphics library for rendering 3D graphics.
    GLEW: The OpenGL Extension Wrangler Library is used to access modern OpenGL functions.
    GLUT: The OpenGL Utility Toolkit provides window management and user input functionality.

Make sure you have these libraries installed and properly set up on your system before running the code.

### 3. Global Variables <a name="global-variables"></a>

The code begins with a list of global variables, including libraries, flags, and OpenGL-related variables:

- `#include` statements: Include necessary libraries like OpenGL, GLUT, glm, and others.
- `fullscreen`, `mouseDown`, `animation`, `dispersion`, `Fvalue`, `fresnelValue`: Boolean flags controlling program behavior.
- `shaderAUsar`: An integer controlling which shader to use.
- Various `GLfloat` variables: Used for storing rotation angles, differences in rotation, window dimensions, and other values.
- `GLuint` variables: Used for handling OpenGL objects like VAOs, VBOs, and textures.

### 3. Shader Loading Functions <a name="shader-loading-functions"></a>

The code provides functions for loading and managing shaders:

- `loadSource(GLuint &shaderID, std::string name)`: Loads shader source code from a file and attaches it to a shader program.
- `printCompileInfoLog(GLuint shadID)`: Checks and prints shader compilation information.
- `printLinkInfoLog(GLuint programID)`: Checks and prints program linking information.
- `validateProgram(GLuint programID)`: Validates the program and checks for any errors.

### 4. Primitive Initialization Functions <a name="primitive-initialization-functions"></a>

The code contains functions to initialize different 3D primitives like cubes, teapots, and spheres:

- `initCube(GLint i = 0)`: Initializes a cube's VAO and VBO, setting up position, normal, and texture coordinate attributes.
- `initTeapot(GLint grid, glm::mat4 transform, GLint i = 0)`: Initializes a teapot with the specified grid and transformation matrix, setting up position, normal, and texture coordinate attributes.
- `initSphere(GLfloat radius, GLuint rings, GLuint sectors, GLint shader = 0)`: Initializes a sphere with the given radius, rings, and sectors. It sets up position, normal, and texture coordinate attributes.

### 5. Drawing Functions <a name="drawing-functions"></a>

The code provides functions for drawing different 3D objects:

- `drawCube()`: Draws a cube using its VAO.
- `drawTeapot()`: Draws a teapot using its VAO.
- `drawSphere()`: Draws a sphere using its VAO.

### 6. Main Function <a name="main-function"></a>

The `main` function sets up the GLUT window and enters the main loop. It also initializes GLEW and calls the `init` function.

### 7. Initialization Function <a name="initialization-function"></a>

The `init` function handles OpenGL initialization, including setting up the shaders, initializing primitives, and loading textures.

This README provides an overview of the code structure and functions. Detailed implementations and additional functions may exist within the code, but this should serve as a starting point for understanding the program.