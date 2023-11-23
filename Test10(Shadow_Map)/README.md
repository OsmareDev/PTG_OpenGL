![Test10](https://github.com/OsmareDev/PTG_OpenGL/assets/50903643/245b8c03-cf52-470e-8e33-473523f1935c)

## English

In this example, a per-pixel lighting model (with a local light) is implemented.

To obtain the shadows, an FBO (framebuffer) is used. A render buffer or a depth texture is configured to store the depth information of the scene from the perspective of light. So we render the scene from the light on a texture.

To avoid problems, avoid rendering the front faces on the depth map. The resolution of the depth map will determine how pixelated the shadows are.

To avoid this we can use PCF filters for which adjacent samples are taken to blur said shadow. In this test, a filter of 4 samples, 16 samples, has been used.

In theory we could also make it configurable, but when performing the test amd has a bug that prevents me from performing the necessary loop on the shader.

---

## Español

En este ejemplo, se implementa un modelo de iluminación por  pixel (con una luz local).

Para obtener las sombras se hace uso de un FBO (framebuffer) Se configura un render buffer o una textura de profundidad para almacenar la información de profundidad de la escena desde la perspectiva de la luz. Por lo que renderizamos sobre una textura la escena desde la luz.

Para evitar problemas se evita renderizar las caras delanteras sobre el mapa de profundidad. La resolución del mapa de profundidad será lo que determine lo pixeladas que sean las sombras.

Para evitarlo podemos utilizar filtros PCF para los cuales se toman muestras adyacentes para difuminar dicha sombra. En este test se han usado un filtro de 4 muestras, 16 muestras.

En teoría podríamos también hacerlo configurable, pero a la hora de realizar el test amd tiene un bug que me impide realizar el bucle necesario sobre el shader.
