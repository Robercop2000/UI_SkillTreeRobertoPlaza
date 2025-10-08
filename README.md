# Árbol de Habilidades (C++) — Unreal Engine 5

## 📘 Descripción general

Este proyecto implementa un **árbol de habilidades** completamente funcional en **Unreal Engine 5** usando **C++**.
Cumple con todos los requisitos de la actividad de Interfaces de Usuario:

* 3 ramas (Velocidad, Salud, Daño).
* Mínimo 2 nodos por rama (6 en total).
* Desbloqueo progresivo dependiente de la mejora anterior.
* Coste por nodo y sistema de puntos.
* Notificaciones visuales y textuales.
* Menú accesible desde el juego mediante la tecla **K**.

No se aplican efectos reales al personaje (opcional según enunciado), pero la arquitectura está preparada para hacerlo fácilmente mediante eventos.

---

## 🧩 Estructura del sistema

**Carpeta:** `Source\SkillTreeUI\SkillSystem`

| Archivo                       | Función                                                                                                |
| ----------------------------- | ------------------------------------------------------------------------------------------------------ |
| `SkillNode.h`                 | Define la estructura `FSkillNode` con los datos de cada mejora (ID, coste, descripción, rama, estado). |
| `SkillTreeComponent.h / .cpp` | Componente con la lógica del árbol: puntos, dependencias, compra y desbloqueo automático.              |
| `SkillTreeWidget.h / .cpp`    | Widget principal del árbol. Genera dinámicamente los nodos y gestiona la interacción.                  |
| `SkillNodeWidget.h / .cpp`    | Widget individual de cada nodo (botón con texto y color según estado).                                 |

**Integración:**

* `USkillTreeComponent` se añade al **Character**.
* El **PlayerController** abre/cierra el menú con la tecla **K** y crea el widget `USkillTreeWidget`.

---

## 🧠 Funcionamiento

1. El jugador inicia con los puntos definidos en `StartingSkillPoints`.
2. El componente genera automáticamente un árbol con 3 ramas y 6 nodos.
3. Al pulsar **K**, se abre el menú del árbol de habilidades (`SkillTreeWidget`).
4. El árbol se muestra en una **rejilla (GridPanel)** con:

   * Filas = ramas (Movement, Health, Damage).
   * Columnas = niveles de habilidad.
5. Los nodos raíz están disponibles desde el inicio.
6. Al comprar un nodo:

   * Se descuentan puntos.
   * Se cambia su estado a *Purchased*.
   * Se desbloquea automáticamente el siguiente nodo.
   * La interfaz se actualiza en tiempo real.
7. Colores de estado:

   * 🟩 Verde → Comprado
   * 🟨 Amarillo → Disponible
   * ⬜ Gris → Bloqueado

---

## 🧱 Interfaz (UMG)

### `WBP_SkillTreeWidget`

* **Parent Class:** `USkillTreeWidget`
* Contiene:

  * `GridPanel` (Variable: `NodeContainer`)
  * `TextBlock` (Variable: `PointsText`)
* Muestra las ramas como filas y los niveles como columnas.

### `WBP_SkillNodeWidget`

* **Parent Class:** `USkillNodeWidget`
* Contiene:

  * `Button` (Variable: `NodeButton`)
  * `TextBlock` (Variable: `NodeLabel`)
* Muestra el nombre y coste de cada nodo.
* Cambia de color según el estado y llama a `TrySelectNode()` al hacer clic.

Los mensajes al jugador se muestran con `GEngine->AddOnScreenDebugMessage()`.

---

## ⚙️ Justificación técnica

* **C++**:
  Toda la lógica del sistema (componentes, dependencias, puntos, estados) se ha implementado en C++ para cumplir la restricción de la práctica y mantener el control total de la lógica.

* **Blueprints (UMG)**:
  Usados únicamente para la interfaz visual, lo que permite diseñar fácilmente la disposición de botones y texto sin escribir código adicional de layout.

---

## 🧪 Pruebas realizadas

* Verificación de que los **nodos raíz** están disponibles al inicio.
* Compra de un nodo y desbloqueo automático del siguiente.
* Mensajes de error:

  * “Nodo bloqueado — desbloquea el anterior”
  * “No tienes suficientes puntos”
  * “Nodo ya adquirido”
* Prueba del sistema de puntos y refresco de interfaz.
* Validación de apertura/cierre del menú con **K**.
* Confirmación de que `StartingSkillPoints` es editable en el editor.

---

## 🔧 Parámetros configurables

| Propiedad                    | Descripción                                                            |
| ---------------------------- | ---------------------------------------------------------------------- |
| `StartingSkillPoints`        | Puntos iniciales del jugador (en el componente `USkillTreeComponent`). |
| `SkillTreeWidgetClass`       | Clase UMG del árbol (asignada en el PlayerController).                 |
| `SkillNodeWidgetClass`       | Clase UMG del widget de nodo (asignada en el editor).                  |
| Input Action `OpenSkillTree` | Tecla **K** para abrir/cerrar el menú.                                 |

---

## 🎨 Estilo visual

El diseño utiliza un formato funcional (“arte de programador”):

* Rejilla limpia tipo árbol.
* Colores según estado.
* Textos centrados y claros.

Preparado para incorporar fácilmente iconos o efectos gráficos personalizados.

---

## 🚀 Posibles mejoras futuras

* Aplicar efectos reales al personaje (velocidad, vida, daño).
* Guardado/carga del progreso del árbol.
* Añadir animaciones o sonido al desbloquear nodos.
* Implementar confirmación por mantener pulsado (“Hold to confirm”).
* Dibujar líneas o conectores entre nodos.

---

## 🧾 Conclusión

El sistema cumple todos los requisitos del enunciado:

* 3 ramas, mínimo 6 nodos.
* Dependencias correctas entre mejoras.
* Sistema de puntos funcional.
* Menú accesible desde el juego.
* Lógica en C++, interfaz en UMG.

El resultado es un sistema modular, ampliable y completamente integrable en cualquier proyecto Unreal Engine 5.
