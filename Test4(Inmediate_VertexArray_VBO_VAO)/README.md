![Test4](https://github.com/OsmareDev/PTG_OpenGL/assets/50903643/a8b2e1f2-7313-411f-a8ad-72c139a0e3c7)

## English

The code generates a large number of cubes and checks the performance difference between:

### Immediate mode (**Rendering commands are sent immediately to the graphics card hardware. Being simpler to use but losing performance.**)

### Using VA (**Instead of sending vertices one by one, vertex arrays allow you to send a block of vertex data at once, which can significantly improve performance compared to immediate mode**)

* **Overhead Reduction:** In immediate mode, sending vertices one by one can generate significant overhead due to repeated calls to OpenGL functions. With vertex arrays, you can send a block of vertex data at once, reducing the overhead associated with multiple function calls.
* **Less Bandwidth Usage:** By sending vertex data in a single block, the amount of data sent between the CPU and GPU is minimized, which can be beneficial for performance.

### Using VBO (**Vertex data is stored directly in GPU memory, reducing the need for frequent transfers between the CPU and GPU.**)

* **GPU Storage:** VBOs allow vertex data to be stored directly in GPU memory. This means that data does not have to be transferred
* **Overhead Reduction:** By reducing the amount of data transferred between the CPU and GPU, the overhead associated with data transfer is minimized, which can significantly improve performance.

### Using VAO (**Structure that encapsulates the state of several Vertex Buffer Objects (VBOs) and vertex attribute specifications, allowing to simplify and organize the configuration of the graphics pipeline for efficient rendering of 3D objects**)

---

## Español

En el código se genera un número alto de cubos y se comprueba la diferencia de rendimiento entre:

### Modo inmediato (**Los comandos de renderizado se envían inmediatamente al hardware de la tarjeta gráfica. Siendo más simple de usar pero perdiendo rendimiento.**)

### Usando VA (**En lugar de enviar vértices uno por uno, los vertex arrays permiten enviar un bloque de datos de vértices de una sola vez, lo que puede mejorar significativamente el rendimiento en comparación con el modo inmediato**)

* **Reducción de Overhead:** En el modo inmediato, enviar vértices uno por uno puede generar un overhead significativo debido a la repetición de llamadas a funciones de OpenGL. Con vertex arrays, puedes enviar un bloque de datos de vértices de una sola vez, reduciendo el overhead asociado con múltiples llamadas de función.
* **Menos Uso de Ancho de Banda:** Al enviar datos de vértices en un solo bloque, se minimiza la cantidad de datos enviados entre la CPU y la GPU, lo que puede ser beneficioso para el rendimiento.

### Usando VBO (**Los datos de vértices se almacenan directamente en la memoria de la GPU, reduciendo la necesidad de transferencias frecuentes entre la CPU y la GPU.**)

* **Almacenamiento en la GPU:** Los VBOs permiten almacenar datos de vértices directamente en la memoria de la GPU. Esto significa que los datos no tienen que transferirse
* **Reducción de Overhead:** Al reducir la cantidad de datos transferidos entre la CPU y la GPU, se minimiza el overhead asociado con la transferencia de datos, lo que puede mejorar significativamente el rendimiento.

### Usando VAO (**Estructura que encapsula el estado de varios Vertex Buffer Objects (VBOs) y especificaciones de atributos de vértices, permitiendo simplificar y organizar la configuración del pipeline gráfico para la renderización eficiente de objetos 3D**)
