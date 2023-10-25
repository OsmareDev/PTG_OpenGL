This is a C++ program for a basic OpenGL application to draw cubes using various rendering methods (immediate mode, vertex arrays, vertex buffer objects, and vertex array objects). The program sets up an OpenGL window, initializes various rendering options, and draw a grid of cubes with different rendering techniques.

![Imgur Gif](https://i.imgur.com/QL5tyAq.gifv)

### Cube Drawing Program

This C++ program uses OpenGL and GLUT to create a 3D graphics application for drawing cubes. It provides different rendering options for drawing cubes with various rendering techniques. Here's a breakdown of the code:

#### Main Function (`main`)

- Initializes the GLUT library and sets up the window.
- Initializes GLEW for OpenGL extension handling.
- Calls the `init` function for program initialization.
- Sets up callback functions for rendering, keyboard input, mouse events, window resizing, and idle processing.
- Starts the main loop (`glutMainLoop`) for running the program.

#### Cube Drawing Functions (`draw1Cube`, `drawMultipleCubes`)

- `draw1Cube`: This function uses immediate mode rendering to draw a single cube. It specifies the vertices, colors, and texture coordinates for each face of the cube using OpenGL commands.
- `drawMultipleCubes`: This function draws multiple cubes in a grid pattern using the specified rendering technique (either immediate mode or vertex array, vertex buffer object, or vertex array object).

#### Initialization Functions (`init`, `initVBO`, `initVAO`)

- `init`: Initializes various OpenGL settings, sets up the window, and loads a texture.
- `initVBO`: Initializes Vertex Buffer Objects (VBOs) for rendering with vertex arrays.
- `initVAO`: Initializes Vertex Array Objects (VAOs) for rendering with vertex arrays.

#### Rendering State Functions (`stateVA1`, `stateVA2`, `stateVA3`, `stateVBO1`, `stateVBO2`, `stateVBO3`, `stateVAO1`, `stateVAO2`, `stateVAO3`)

These functions set up OpenGL rendering states for different rendering techniques, including immediate mode, vertex arrays, VBOs, and VAOs.

#### Constants and Global Variables

- Various constants and global variables are declared to control the program's behavior, such as window size, texture loading, and rendering options.
