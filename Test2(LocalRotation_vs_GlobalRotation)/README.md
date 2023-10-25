This C++ code sets up a basic OpenGL window and displays several boxes rotating around each other. It handles user input for quitting the application and toggling animation. The box's rotation can also be controlled by mouse input, and the frames per second (FPS) are displayed in the console.

![Imgur Gif](https://i.imgur.com/0f9AbWk.gifv)

1. **Include Statements:**
   - `#include <iostream>`: This header includes the C++ standard library for input and output.
   - `#include <GL/glut.h>`: It includes the GLUT library for setting up an OpenGL window.

2. **Function Prototypes:**
   - A series of function prototypes define functions used throughout the program for rendering, window management, user interaction, and displaying FPS.

3. **Global Variables:**
   - Several global boolean and floating-point variables control the application state, including fullscreen mode, mouse interaction, animation, and perspective. Various rotation and scaling factors are also declared.

4. **`main` Function:**
   - The `main` function initializes GLUT, creates a window, sets up callback functions for various events (display, keyboard, mouse, etc.), and starts the main loop.

5. **`drawBox` Function:**
   - This function uses OpenGL's immediate mode to draw a 3D box with colored faces. It specifies the vertices and colors of each face.

6. **`drawScene` Function:**
   - This function defines a scene composed of multiple geometric elements, including a sun, Earth, geostationary satellite, moon, and a vertical satellite. These elements are drawn and positioned using rotations and translations.

7. **`init` Function:**
   - The `init` function initializes OpenGL settings, such as the clear color, depth testing, and shading model.

8. **`display` Function:**
   - The `display` function clears the screen, sets up the view and projection matrices, applies mouse-based rotations, and renders the scene. It also calls `printFPS` to display the FPS.

9. **`resize` Function:**
   - The `resize` function handles window resizing by updating the viewport and projection matrix based on the window dimensions.

10. **`idle` Function:**
    - When the mouse is not interacting and animation is enabled, the `idle` function rotates the scene slightly based on time.

11. **`keyboard` Function:**
    - The `keyboard` function responds to keyboard input. For example, it allows quitting the application with the 'q' key and toggling animation with 'a'. It also controls wireframe rendering and perspective changes.

12. **Mouse Interaction Functions (`mouse` and `mouseMotion`):**
    - The `mouse` function tracks mouse button clicks and updates mouse down state. The `mouseMotion` function handles mouse movement and updates the rotation based on mouse input.

13. **`printFPS` Function:**
    - The `printFPS` function calculates and prints frames per second (FPS) in the console. It tracks the frame count and timing intervals to compute the FPS.