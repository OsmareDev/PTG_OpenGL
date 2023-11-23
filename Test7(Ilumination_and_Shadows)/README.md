

## English

In the following code, shaders are used to perform different calculations and modifications on the lighting of the scene. Illumination calculations are made in the visual reference system because this simplifies the necessary calculations.

### vertex lighting: This lighting is the cheapest to perform since the lighting calculation is done in the vertex shader and therefore we save calculations in exchange for having “pixelated” lighting.

### vertex lighting (soft): To achieve this effect we just have to not use the reserved word flat in the variable passed to the fragment shader. In this way the colors between the vertices are automatically interpolated.

### phong shading: to achieve this effect, the lighting calculation must be done directly in the fragment shader in this way the light is softened.

### comic shading: to achieve this effect what we must do is calculate the lighting in the fragment shader and reduce the number of colors used to 3 following the following rules:

**In the rendering process, conditions are set to determine the color of a fragment based on the angles between normalized vectors. If the cosine of the angle between the view and reflection vectors is greater than 0.94, the sum of the specular and diffuse property of the material is applied to the fragment color. When this cosine is smaller and the cosine of the angle between the normal vector and the light vector is positive, the diffuse property is used. Otherwise, the pixel will be colored with the ambient property multiplied by 1.5. In addition, the outline of the object is highlighted by coloring them black. This contour is identified by scalarly multiplying the view and normal vectors, both normalized, and considering fragments whose result is less than a set threshold, such as 0.14.**

---

## Español

En el siguiente código se hace uso de shaders para realizar distintos cálculos y modificaciones sobre la iluminación de la escena. Los calculos de la iluminación se realizan en el sistema de referencia de la vista debido a que esto simplifica los cálculos necesarios.

### iluminación por vértice: Esta iluminación es la más barata de realizar dado que el cálculo de la iluminación se realiza en el vertex shader y por lo tanto nos ahorramos cálculos a cambio de tener una iluminación “pixelada”.

### iluminación por vértice (suave): Para conseguir este efecto solo debemos no usar la palabra reservada flat en la variable que se le pasa al shader de fragmento. De esta manera los colores entre los vértices son automáticamente interpolados.

### sombreado phong: para lograr este efecto se debe hacer el calculo de la iluminación directamente en el shader de fragmento de esta manera se consigue suavizar la luz.

### sombreado comic: para lograr dicho efecto lo que debemos hacer es calcular la iluminación en el shader de fragmento y reducir la cantidad de colores usados a 3 siguiendo las siguientes reglas:

**En el proceso de renderización, se establecen condiciones para determinar el color de un fragmento en función de los ángulos entre vectores normalizados. Si el coseno del ángulo entre los vectores de vista y reflejo es mayor que 0.94, se aplica la suma de la propiedad especular y difusa del material al color del fragmento. Cuando este coseno es menor y el coseno del ángulo entre el vector normal y el vector de luz es positivo, se utiliza la propiedad difusa. En caso contrario, el pixel se coloreará con la propiedad ambiente multiplicada por 1.5. Además, se destaca el contorno del objeto al colorearlos de negro. Este contorno se identifica al multiplicar escalarmente los vectores de vista y normal, ambos normalizados, y considerar fragmentos cuyo resultado sea menor que un umbral establecido, como 0.14.**