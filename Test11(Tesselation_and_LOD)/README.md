

## English

In this example we convert the cube into a sphere through tessellation shaders.

### tessellation control:
The TCS is used to calculate tessellation factors that determine how many new vertices should be generated between the original vertices of a triangle.

### tessellation evaluation:
The TES is responsible for evaluating the position of the new vertices generated during the tessellation process and calculating their final attributes.

We will take advantage and make use of a technique known as LOD (Level of detail). For which we can make different approximations

1. global variable for the entire sphere
2. variable per patch
3. variable without cracks (**in this way we equalize the vertices on the faces to avoid failures in the connections of the sphere**)

---

## Español

En este ejemplo convertimos el cubo en una esfera a través de los shaders de teselación.

### teselación de control:
El TCS se utiliza para calcular los factores de teselación que determinan cuántos nuevos vértices se deben generar entre los vértices originales de un triángulo.

### teselación de evaluación:
El TES se encarga de evaluar la posición de los nuevos vértices generados durante el proceso de teselación y calcular sus atributos finales.

Aprovecharemos y haremos uso de una técnica conocida como LOD (Level of detail). Para la cual podemos hacer diferentes aproximaciones

1. variable global para toda la esfera
2. variable por parche
3. variable sin grietas (**de este modo igualamos los vértices en las caras para evitar fallos en las conexiones de la esfera**)

