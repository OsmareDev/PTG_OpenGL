This code sets up a basic OpenGL window and displays a rotating 3D box. It handles user input for quitting the application and toggling animation. The box's rotation can also be controlled by mouse input. The FPS is displayed in the console.

![Imgur Gif](https://i.imgur.com/sJYtRbp.gifv)

1. **Include Statements:**
   - `#include <iostream>`: Standard C++ library for input and output.
   - `#include <GL/glut.h>`: GLUT library for setting up an OpenGL window.

2. **Function Prototypes:**
   - `void drawBox()`: Function prototype for drawing the 3D box.
   - `GLboolean initOpenGL()`: Function prototype for initializing OpenGL settings.
   - `void display()`: Function prototype for the display callback.
   - `void resize(GLint, GLint)`: Function prototype for window resize callback.
   - `void inactive()`: Function prototype for idle (inactivity) callback.
   - `void keyboard(GLubyte, GLint, GLint)`: Function prototype for keyboard callback.
   - `void mouseClick(GLint, GLint, GLint, GLint)`: Function prototype for mouse click callback.
   - `void mouseMovement(GLint, GLint)`: Function prototype for mouse movement callback.
   - `void printFPS()`: Function prototype for printing frames per second.

3. **Global Variables:**
   - `fullscreen`, `mouseDown`, `animation`: Boolean flags to control application state.
   - `xrot`, `yrot`, `xdiff`, `ydiff`: Variables to control rotation and mouse interaction.
   - `g_Width`, `g_Height`: Initial window dimensions.

4. **Main Function (`main`):**
   - Initializes GLUT and creates a window.
   - Sets up various callback functions.
   - Calls the `initOpenGL()` function to initialize OpenGL settings.
   - Starts the main loop.

5. **`drawBox` Function:**
   - Draws a colored 3D box using OpenGL's immediate mode (legacy).
   - Defines vertices for each face of the box and sets colors.

6. **`initOpenGL` Function:**
   - Initializes OpenGL settings, including the clear color and depth test.

7. **`display` Function:**
   - Clears the screen and sets up the view and projection matrices.
   - Rotates the box based on the `xrot` and `yrot` variables.
   - Calls `drawBox` to render the 3D box.
   - Prints frames per second using `printFPS`.

8. **`resize` Function:**
   - Handles window resizing by updating the viewport and projection matrix.

9. **`inactive` Function:**
   - Controls the animation of the box when the mouse is not interacting.
   - Rotates the box slightly if the `animation` flag is set.

10. **`keyboard` Function:**
    - Handles keyboard input, including quitting the application with the 'q' key and toggling animation with 'a'.

11. **Mouse Interaction Functions (`mouseClick` and `mouseMovement`):**
    - `mouseClick` is called when a mouse button is clicked and tracks mouse down state.
    - `mouseMovement` is called when the mouse is moved and updates rotation based on mouse movement.

12. **`printFPS` Function:**
    - Calculates and prints frames per second (FPS) in the console.
