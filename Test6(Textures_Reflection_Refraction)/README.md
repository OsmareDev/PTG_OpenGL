

## English

The following code uses:

### Vertex Shader (**Processes each individual vertex of an object before it is rasterized and passed to the next stages of the graphics pipeline.**)

1. Mix: This vertex shader passes the texture coordinate values to the fragment.

2. Reflejo: This vertex shader, if the environment map is activated, calculates the reflected vector based on the normal and the position of the camera

3. Trans: This vertex shader passes the values of the vertex position, camera position and normal value to the fragment.

4. TransCube: This vertex shader passes the values of the vertex position, camera position and normal value to the fragment.

5. TransSphere: This vertex shader passes the values of the vertex position, camera position and normal value to the fragment.

### Fragment Shader (**Calculates the final color of each pixel based on several factors, such as lighting, textures, and vertex colors.**)

1. Mix: This fragment shader mixes two textures (uStoneTex and uMossTex) based on the alpha component of the second texture to make it look like the rock texture has grown moss.

2. Reflejo: This fragment shader uses environment mapping to assign a color to the final pixel based on the reflected texture coordinates.

3. Trans: This fragment shader calculates the refraction coordinates for each channel, simulating the transparency of the glass, calculating the fresnel (**Optical phenomenon that describes how the reflectivity of a material varies with the angle of incidence of light.**) if activated and using it to mix refraction and reflection as necessary.

4. TransCube: This fragment shader calculates the exact refraction of a glass cube taking into account that light is modified twice when entering and leaving

5. TransSphere: This fragment shader calculates the exact refraction of a glass sphere taking into account that light is modified twice when entering and leaving

---

## Español

En el siguiente código se hace uso de:

### Vertex Shader (**Procesa cada vértice individual de un objeto antes de que se rasterice y se pase a las siguientes etapas del pipeline gráfico.**)

1. Mix: Este vertex shader pasa los valores de coordenadas de textura al fragment.

2. Reflejo: Este vertex shader en caso de estar el mapa de entorno activado calcula el vector reflejado en función de la normal y la posición de la cámara

3. Trans: Este vertex shader pasa los valores de posición del vértice, posición de la cámara y valor de la normal al fragment.

4. TransCube: Este vertex shader pasa los valores de posición del vértice, posición de la cámara y valor de la normal al fragment.

5. TransSphere: Este vertex shader pasa los valores de posición del vértice, posición de la cámara y valor de la normal al fragment.

### Fragment Shader (**Calcula el color final de cada píxel en función de varios factores, como la iluminación, texturas y colores de los vértices.**)

1. Mix: Este fragment shader mezcla dos texturas (uStoneTex y uMossTex) en función del componente alfa de la segunda textura para que parezca que a la textura de la roca le ha salido musgo.

2. Reflejo: Este fragment shader utiliza un mapeo de entorno para asignar un color al píxel final basado en las coordenadas de textura reflejadas.

3. Trans: Este fragment shader calcula las coordenadas de refracción para cada canal simulando la transparencia que tiene el cristal, calculando el fresnel (**Fenómeno óptico que describe cómo la reflectividad de un material varía con el ángulo de incidencia de la luz.**) en caso de estar activado y usandolo para mezclar refraccion y reflexion conforme sea necesario.

4. TransCube: Este fragment shader calcula la refracción exacta de un cubo de cristal teniendo en cuenta que la luz se modifica 2 veces al entrar y salir

5. TransSphere: Este fragment shader calcula la refracción exacta de un esfera de cristal teniendo en cuenta que la luz se modifica 2 veces al entrar y salir