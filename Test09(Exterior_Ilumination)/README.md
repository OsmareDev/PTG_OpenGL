![Test9](https://github.com/OsmareDev/PTG_OpenGL/assets/50903643/b4d5b5be-883a-4d26-861c-c41ec5508b04)

## English

### Hemispherical lighting
It is a technique for simulating ambient light coming from all directions around a point in three-dimensional space. Unlike other lighting techniques, such as directional or spot lighting, which focus on specific light sources.

### Image-based lighting
Image-Based Lighting (IBL), typically environment maps or light maps, to provide lighting information in a three-dimensional scene.

### Lighting based on spherical harmonics
Illumination is efficiently represented using a relatively small set of spherical harmonic coefficients rather than storing lighting information for each individual pixel or vertex.

Spherical harmonics represent the distribution of light in all directions in a scene. This includes both direct and indirect lighting.

(**the ambient part does not make sense, since the model intrinsically simulates the light coming from the entire environment**)

**The illumination reached by each point on the surface of the object now comes from the environment, and is determined by the product of the MM coefficient matrix times the bidirectional normal, multiplied in turn by a scale factor (SPH_Scale constant, defined in the code of the shader). The color of the fragment is obtained by multiplying this lighting by the diffuse component of the material (uMaterial.diffuse).**

**There are actually three MM matrices, one for each color component. The elements of these arrays are defined in the function \texttt{display}. These arrays must be passed to the fragment shader as uniform variables.**

---

## Español

### Iluminación hemisférica
Es una técnica para simular la luz ambiente que proviene de todas las direcciones alrededor de un punto en el espacio tridimensional. A diferencia de otras técnicas de iluminación, como la iluminación direccional o puntual, que se centran en fuentes de luz específicas.

### Iluminación basada en imágenes
La iluminación basada en imágenes (Image-Based Lighting, IBL), generalmente mapas de entorno o mapas de luz, para proporcionar información de iluminación en una escena tridimensional.

### Iluminación basada en armónicos esféricos
La iluminación se representa de manera eficiente utilizando un conjunto relativamente pequeño de coeficientes de armónicos esféricos en lugar de almacenar información de iluminación para cada píxel o vértice individual.

Los armónicos esféricos representan la distribución de luz en todas las direcciones en una escena. Esto incluye tanto la iluminación directa como la indirecta.

(**la parte ambiente no tiene  sentido, ya que el modelo simula de forma intrínseca la luz proveniente de todo el entorno**)

**La iluminación que alcanza cada punto en la superficie del objeto ahora proviene del entorno, y se determina mediante el producto de la matriz de coeficientes MM por la normal bidireccional, multiplicado a su vez por un factor de escala (constante SPH_Scale, definida en el código del shader). El color del fragmento se obtiene multiplicando esta iluminación por la componente difusa del material (uMaterial.diffuse).**

**En realidad, existen tres matrices MM, una para cada componente de color. Los elementos de estas matrices se definen en la función \texttt{display}. Estas matrices deben ser pasadas al shader de fragmento como variables uniformes.**
