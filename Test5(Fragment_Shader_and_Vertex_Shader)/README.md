# OpenGL and GLUT C++ Program

This C++ program utilizes OpenGL and GLUT for rendering a cube and a teapot. The code consists of several functions and global variables that are essential for the rendering process. Below is an explanation of the code's components. The program uses shaders to modify the vertices and pixels of the objects:

![Imgur](https://i.imgur.com/oT3iDWi.gifv)

## Global Variables

- `fullscreen`, `mouseDown`, `animation`, `drawing`, `crecer`, `rotar`, `discard`: These global boolean variables are used to control various application states, such as fullscreen mode, mouse interactions, animations, and more.

- `xrot`, `yrot`, `xdiff`, `ydiff`: These variables store rotation values and differences for mouse interaction.

- `g_Width`, `g_Height`: Variables to store the initial width and height of the window.

- Various OpenGL-related variables (e.g., `vaoCube`, `vaoTea`, `programID`, `programID2`, `locMat1`, `locMat2`, `locMat3`, `locFlo1`, `locFlo2`) are used to manage shaders and OpenGL objects.

- Other variables (`inter`, `cantRotCol`, `umbral`, `numVertTeapot`) are used for controlling specific aspects of rendering.

## Cube and Teapot Vertex Data

The code initializes vertex and color data for a cube. Each cube face has its own set of vertices and colors. Similarly, it sets up vertex, normal, and texture coordinate data for the teapot using functions like `initCube()` and `initTeapot()`.

## Shader Functions

Several functions are provided for shader handling:
- `loadSource()`: Reads shader source code from a file.
- `printCompileInfoLog()`: Prints shader compilation information and errors.
- `printLinkInfoLog()`: Prints program linking information and errors.
- `validateProgram()`: Validates an OpenGL program for errors.

## OpenGL Initialization

The `init()` function initializes various OpenGL settings, loads and compiles shaders, and sets up shader programs for both the cube and teapot.

## Display Function

The `display()` function is responsible for rendering the scene. It uses OpenGL functions to set up projection, model-view matrices, and shader programs. It also draws the cube and teapot based on the application state (`drawing`). This function is called repeatedly to create animation.

## Main Function

The `main()` function is the program's entry point. It initializes GLUT, sets up the window, initializes OpenGL extensions (GLEW), and enters the GLUT main loop to start rendering.

# Shaders

## Vertex Shaders

- `main`: simple vertex shader
- `teapot`: performs an interpolation of the points of the object so that they transform into a sphere, according to an external input

## Fragment Shaders

- `main`: simple fragment shader
- `teapot`: performs an interpolation between a threshold to determine which pixels of the object should be discarded following a pattern