
# 🎮 Escape del Laboratorio

**Proyecto Final – Programación Orientada a Objetos (POO)**

**Universidad Peruana Cayetano Heredia | Ciclo 2026-2**

---

## 📖 Descripción

**Escape del Laboratorio** es un videojuego 2D desarrollado en **C++**, utilizando la biblioteca gráfica **SFML 3**.

El juego cuenta la historia de un **gato científico** que despierta atrapado en un laboratorio después de que uno de sus experimentos sale mal. Como consecuencia, los robots de seguridad pierden el control y el laboratorio se convierte en un lugar lleno de peligros.

Para conseguir escapar, el gato científico deberá recorrer diferentes niveles, recolectar baterías de energía, evitar a los robots enemigos y encontrar la puerta de salida antes de quedarse sin vida.

Este proyecto tiene como finalidad aplicar los fundamentos de la **Programación Orientada a Objetos (POO)**, utilizando clases, objetos, herencia, encapsulamiento, abstracción, polimorfismo y patrones de diseño.

---

## 🎯 Objetivo del juego

El objetivo principal es **ayudar al gato científico a escapar de los tres niveles del laboratorio**, superando los desafíos y enemigos presentes en cada escenario.

Para completar cada nivel, el jugador deberá:

- 🔋 Recolectar las cuatro baterías de energía.
- ⭐ Acumular 40 puntos para desbloquear la salida.
- 🤖 Evitar los ataques de los robots enemigos.
- ⚡ Utilizar el arma eléctrica para aturdir temporalmente a los robots.
- 🚪 Encontrar la puerta de salida y avanzar al siguiente nivel.

---

## 🕹️ Mecánicas principales

- **Movimiento del personaje:** el jugador controla al gato científico y puede desplazarse por el laboratorio.
- **Sistema de colisiones:** evita que el personaje atraviese paredes y obstáculos.
- **Recolección de baterías:** cada batería proporciona 10 puntos.
- **Sistema de puntuación:** permite acumular los puntos necesarios para desbloquear las salidas.
- **Sistema de vida:** el gato científico comienza con 100 puntos de vida y puede recibir daño de los enemigos.
- **Arma eléctrica:** permite aturdir temporalmente a los robots utilizando cargas limitadas.
- **Inteligencia artificial:** los enemigos tienen comportamientos de persecución, patrullaje y ataque.
- **Sistema de niveles:** tres escenarios con dificultad progresiva.
- **Interfaz HUD:** muestra la vida, puntuación, nivel actual y cargas eléctricas disponibles.

---

## 🤖 Tipos de enemigos

El laboratorio cuenta con tres tipos principales de robots:

### 🔴 Robot Perseguidor

Persigue al gato científico utilizando el algoritmo de búsqueda de rutas **A*** para encontrar caminos a través del laboratorio.

### 🟠 Robot Guardián

Patrulla zonas determinadas del laboratorio, desplazándose entre diferentes puntos y representando un obstáculo para el jugador.

### ⚡ Robot de Ataque

Detecta al gato científico dentro de su rango de acción y realiza ataques a distancia mediante proyectiles láser.

---

## 🧪 Niveles del juego

El videojuego cuenta con tres niveles de dificultad progresiva.

| Nivel | Nombre | Descripción |
|-------|--------|-------------|
| 1 | El despertar | El gato científico conoce el laboratorio, aprende las mecánicas y evita a su primer enemigo. |
| 2 | Alerta de seguridad | Aparecen nuevos robots y aumenta la dificultad para escapar. |
| 3 | Escape final | El gato científico debe superar distintos tipos de robots para escapar definitivamente del laboratorio. |

---

## 🧩 Programación Orientada a Objetos

El proyecto aplica los principales fundamentos de la **Programación Orientada a Objetos**:

- **Encapsulamiento:** protección y organización de los atributos mediante métodos de acceso.
- **Herencia:** reutilización de atributos y métodos entre clases relacionadas.
- **Polimorfismo:** implementación de comportamientos diferentes mediante métodos virtuales.
- **Abstracción:** utilización de clases base para representar personajes y robots.
- **Composición:** organización de los objetos que conforman los distintos sistemas del videojuego.

### 🏗️ Patrones de diseño

Se implementan los siguientes patrones de diseño:

**Factory Method**
- Organiza la creación de los diferentes tipos de robots.
- Reduce la dependencia directa de las clases concretas.

**Observer**
- Permite notificar cambios en el estado del videojuego.
- Facilita la actualización de información en la interfaz HUD.

---

## 💻 Tecnologías utilizadas

| Tecnología | Función |
|------------|---------|
| C++17 | Desarrollo de la lógica del videojuego |
| SFML 3 | Gráficos, animaciones y manejo de eventos |
| CMake | Configuración y compilación del proyecto |
| Visual Studio Code | Entorno de desarrollo |
| Git | Control de versiones |
| GitHub | Almacenamiento y colaboración del equipo |

---

## 📂 Estructura general del proyecto

    EscapeDelLaboratorio/
    ├── src/                 # Archivos de implementación C++
    ├── include/             # Archivos de cabecera
    ├── assets/
    │   └── imagenes/        # Sprites, fondos y texturas
    ├── CMakeLists.txt        # Configuración de CMake
    ├── CMakePresets.json     # Configuraciones de compilación
    └── README.md            # Documentación del proyecto

---

## 👥 Integrantes

- Mori Mendoza, Daniela
- Uscamayta Lazaro, Marx
- Jimenez Ostos, Giacomo
- Ronceros Huaynapomas, Rolando
- Ccoyllo Sotelo, Alex

---

## 🎓 Propósito académico

El videojuego **Escape del Laboratorio** se desarrolla como proyecto final del curso de **Programación Orientada a Objetos**, correspondiente al ciclo académico **2026-2** de la **Universidad Peruana Cayetano Heredia**.

Su finalidad es demostrar la aplicación de los conceptos de POO, patrones de diseño, arquitectura de software y trabajo colaborativo utilizando Git y GitHub.

---
