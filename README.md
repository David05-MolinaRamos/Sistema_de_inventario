# Gestor de Inventario para Comedor

Este es un programa en C++ que hice para llevar el control de los insumos (como comida y materiales) en un comedor. Empezó como un código básico en la terminal y lo fui mejorando hasta conectarlo con una ventana gráfica usando Qt para que sea más fácil de usar.

## Lo que hace el programa
* **Registrar productos:** Permite dar de alta insumos con su ID, nombre, cantidad y unidad (kilos, litros, piezas).
* **Descontar inventario:** Tiene una sección para restar los productos cuando se sacan para usarlos en la cocina, actualizando la lista al instante.
* **Guardar los datos:** Todo lo que se registra se guarda automáticamente en un archivo de texto (`inventario.txt`). Así, si cierras el programa, la información no se pierde y vuelve a cargar todo al abrirlo al día siguiente.

## Herramientas que usé
* **C++:** Uso de arreglos, ciclos, y programación orientada a objetos básica.
* **Qt5:** Para diseñar la interfaz visual (botones, cajas de texto y listas).
* **Manejo de archivos:** Uso de la librería `fstream` para guardar y leer el bloc de notas.

