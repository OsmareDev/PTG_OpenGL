![Test5](https://github.com/OsmareDev/PTG_OpenGL/assets/50903643/284e3d63-000a-48fc-997f-01ef47c12889)

## English

The following code uses:

### Vertex Shader (**Processes each individual vertex of an object before it is rasterized and passed to the next stages of the graphics pipeline.**)

1. Main: Passes the designated vertex color to the fragment.

2. Tetera: Calculates the vertex color based on the normal and a rotation according to time, activated in the application code. Also calculate a displacement of the vertices so that the object varies with time between a sphere and its normal shape.

### Fragment Shader (**Calculates the final color of each pixel based on several factors, such as lighting, textures, and vertex colors.**)

1. Main: Passes the interpolated color as the pixel color.

2. Tetera: Discards the pixels of the object over time, filling it with holes between the vertex.

---

## Español

En el siguiente código se hace uso de:

### Vertex Shader (**Procesa cada vértice individual de un objeto antes de que se rasterice y se pase a las siguientes etapas del pipeline gráfico.**)

1. Main: Pasa el color designado del vértice al fragment.

2. Tetera: Calcula el color del vértice en base a la normal y a una rotación conforme al tiempo, activado en el código de la aplicación. Calcula también un desplazamiento de los vértices para que el objeto varie conforme el tiempo entre una esfera y su forma normal.

### Fragment Shader (**Calcula el color final de cada píxel en función de varios factores, como la iluminación, texturas y colores de los vértices.**)

1. Main: Pasa el color interpolado como color del píxel.

2. Tetera: Descarta los pixeles del objeto conforme el tiempo haciendo que este se agujeree.
