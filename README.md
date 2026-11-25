# 🎮 ESCAPE DEL LABORATORIO
### 🧪 Un experimento fallido. Un laboratorio fuera de control. Una misión: escapar.

<p align="center">
  **Proyecto Final – Programación Orientada a Objetos (POO)**
  
  **Universidad Peruana Cayetano Heredia | Ciclo 2026-2**
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Lenguaje-C%2B%2B17-blue?style=for-the-badge&logo=cplusplus" />
  <img src="https://img.shields.io/badge/Gráficos-SFML%203-green?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Build-CMake-red?style=for-the-badge&logo=cmake" />
  <img src="https://img.shields.io/badge/Estado-En%20desarrollo-orange?style=for-the-badge" />
</p>

---

## 📌 Índice

- [📖 Descripción del proyecto](#-descripción-del-proyecto)
- [📚 Historia](#-historia)
- [🎯 Objetivo del juego](#-objetivo-del-juego)
- [🕹️ Características principales](#️-características-principales)
- [🎮 Controles](#-controles)
- [🤖 Enemigos](#-enemigos)
- [🧪 Niveles](#-niveles)
- [❤️ Sistema de juego](#️-sistema-de-juego)
- [🧩 Programación Orientada a Objetos](#-programación-orientada-a-objetos)
- [🏗️ Patrones de diseño](#️-patrones-de-diseño)
- [💻 Tecnologías utilizadas](#-tecnologías-utilizadas)
- [⚙️ Instalación y ejecución](#️-instalación-y-ejecución)
- [📂 Estructura del proyecto](#-estructura-del-proyecto)
- [👥 Equipo de desarrollo](#-equipo-de-desarrollo)
- [🎓 Propósito académico](#-propósito-académico)

---

## 📖 Descripción del proyecto

**Escape del Laboratorio** es un videojuego de aventura y supervivencia en 2D desarrollado en **C++17**, utilizando la biblioteca gráfica **SFML 3**.

El jugador toma el control de un gato científico que despierta atrapado en un laboratorio de investigación después de que uno de sus experimentos sale inesperadamente mal.

El incidente provoca un fallo general en el sistema de seguridad, haciendo que los robots pierdan el control y comiencen a representar una amenaza.

Para escapar, el protagonista deberá explorar diferentes escenarios, recolectar baterías de energía, superar obstáculos, enfrentarse a robots y encontrar la salida de cada nivel.

El proyecto busca integrar conceptos de programación, diseño de software, inteligencia artificial básica y desarrollo de videojuegos mediante los principios de la **Programación Orientada a Objetos**.

## 📚 Historia

Todo parecía un día normal en el laboratorio...

El gato científico se encontraba realizando un experimento cuando ocurrió una falla inesperada. El sistema principal quedó comprometido y los robots de seguridad comenzaron a actuar de manera independiente.

Al despertar, el protagonista descubre que todas las salidas están bloqueadas y que la única manera de escapar es recuperar las baterías necesarias para reactivar los mecanismos de apertura.

Ahora deberá recorrer tres sectores del laboratorio, enfrentarse a diferentes tipos de robots y descubrir cómo salir de aquel lugar.

**¿Podrá escapar antes de que sea demasiado tarde?**

---

## 🎯 Objetivo del juego

La misión principal consiste en ayudar al gato científico a escapar de los tres niveles del laboratorio.

Para completar cada nivel, el jugador deberá:

- 🔋 Recolectar las cuatro baterías de energía.
- ⭐ Alcanzar los 40 puntos necesarios para desbloquear la salida.
- 🤖 Evitar los ataques de los robots enemigos.
- ⚡ Utilizar el arma eléctrica para aturdir temporalmente a los enemigos.
- 🚪 Encontrar la puerta de salida y avanzar al siguiente escenario.

**Condición de victoria:** completar satisfactoriamente los tres niveles.

**Condición de derrota:** perder todos los puntos de vida del personaje.

---

## 🕹️ Características principales

| Característica | Descripción |
|---|---|
| 🐱 Personaje principal | Gato científico controlado por el jugador. |
| 🏃 Movimiento | Desplazamiento por diferentes áreas del laboratorio. |
| 🧱 Colisiones | Detección de paredes, obstáculos y elementos del escenario. |
| 🔋 Coleccionables | Baterías que permiten acumular puntos. |
| ❤️ Sistema de vida | Administración de los puntos de salud del personaje. |
| ⚡ Arma eléctrica | Permite aturdir temporalmente a los robots. |
| 🤖 Inteligencia artificial | Enemigos con distintos comportamientos. |
| 🚪 Sistema de salida | Desbloqueo de puertas mediante objetivos cumplidos. |
| 📈 Progresión | Tres niveles con dificultad creciente. |
| 🖥️ HUD | Visualización de información importante durante la partida. |

## 🎮 Controles

Los controles permiten manejar al protagonista y utilizar sus habilidades durante la partida.

| Tecla | Acción |
|---|---|
| W / ↑ | Mover hacia arriba |
| S / ↓ | Mover hacia abajo |
| A / ← | Mover hacia la izquierda |
| D / → | Mover hacia la derecha |
| ESPACIO | Utilizar el arma eléctrica |
| ESC | Pausar o salir del juego |

*Nota: los controles deben coincidir con la configuración definitiva implementada en el videojuego.*

---

## 🤖 Enemigos

El laboratorio cuenta con tres tipos principales de robots de seguridad.

### 🔴 Robot Perseguidor

Su función es localizar y perseguir al gato científico utilizando el algoritmo de búsqueda de rutas **A***. Intenta encontrar un camino hacia el jugador evitando los obstáculos del escenario.

### 🟠 Robot Guardián

Se encarga de patrullar determinadas zonas del laboratorio. Se desplaza entre puntos establecidos y representa un peligro para el protagonista.

### ⚡ Robot de Ataque

Detecta al protagonista cuando se encuentra dentro de su rango de acción y realiza ataques a distancia mediante proyectiles láser.

---

## 🧪 Niveles

El videojuego está compuesto por tres escenarios con dificultad progresiva.

| Nivel | Nombre | Descripción |
|---|---|---|
| 01 | El despertar | El protagonista descubre el laboratorio y comienza su aventura. |
| 02 | Alerta de seguridad | Los sistemas de seguridad se intensifican y aparecen nuevos desafíos. |
| 03 | Escape final | El último desafío reúne diferentes tipos de enemigos y obstáculos. |

Cada escenario representa una nueva etapa de dificultad y exige que el jugador utilice correctamente sus recursos para continuar avanzando.

---

## ❤️ Sistema de juego

El videojuego incorpora diferentes sistemas que permiten administrar el progreso de la partida.

### 🔋 Sistema de energía y puntuación
- Cada batería recolectada proporciona 10 puntos.
- Se necesitan 40 puntos para desbloquear la salida.
- Las baterías funcionan como elementos coleccionables.

### ❤️ Sistema de vida
- El personaje comienza con 100 puntos de vida.
- Los enemigos pueden provocar pérdida de vida.
- El jugador debe evitar los ataques para mantenerse con vida.

### ⚡ Sistema de combate
- El protagonista dispone de un arma eléctrica.
- El arma permite aturdir temporalmente a los robots.
- Su utilización está limitada por una cantidad determinada de cargas.

### 🤖 Inteligencia artificial
Los enemigos cuentan con comportamientos diferenciados que permiten generar desafíos durante la exploración del laboratorio.

---

## 🧩 Programación Orientada a Objetos

El desarrollo de **Escape del Laboratorio** aplica los principales fundamentos de la Programación Orientada a Objetos.

| Concepto | Aplicación en el proyecto |
|---|---|
| Encapsulamiento | Protección y administración de atributos mediante métodos. |
| Herencia | Reutilización de características entre clases relacionadas. |
| Polimorfismo | Diferentes comportamientos mediante métodos virtuales. |
| Abstracción | Representación de entidades mediante clases base. |
| Composición | Integración de diferentes objetos para construir sistemas del juego. |

### 🏗️ Patrones de diseño

**1. Factory Method**
- Centraliza la creación de los diferentes tipos de robots.
- Reduce el acoplamiento entre las clases.
- Facilita incorporar nuevos tipos de enemigos.

**2. Observer**
- Permite comunicar cambios entre diferentes componentes.
- Facilita la actualización de información del HUD.
- Ayuda a mantener separados los sistemas del videojuego.

---

## 💻 Tecnologías utilizadas

| Tecnología | Aplicación |
|---|---|
| C++17 | Desarrollo de la lógica del videojuego. |
| SFML 3 | Gráficos, ventanas, eventos y recursos multimedia. |
| CMake | Configuración y compilación del proyecto. |
| Visual Studio Code | Entorno de desarrollo. |
| Git | Control de versiones. |
| GitHub | Repositorio y colaboración del equipo. |

---

## ⚙️ Instalación y ejecución

### Requisitos previos

Para compilar y ejecutar el proyecto se necesita:

- Compilador compatible con C++17.
- CMake.
- Biblioteca SFML 3.
- Git.

### 1. Clonar el repositorio

```bash
git clone URL_DEL_REPOSITORIO
```

### 2. Ingresar a la carpeta

```bash
cd EscapeDelLaboratorio
```

### 3. Configurar el proyecto

```bash
cmake -S . -B build
```

### 4. Compilar

```bash
cmake --build build
```

### 5. Ejecutar

El ejecutable se encontrará dentro de la carpeta de compilación, dependiendo de la configuración utilizada.

*Importante: SFML 3 y sus dependencias deben estar correctamente instaladas y configuradas antes de compilar.*

---

## 📂 Estructura del proyecto

```text
EscapeDelLaboratorio/
│
├── src/
│   └── Archivos de implementación (.cpp)
│
├── include/
│   └── Archivos de cabecera (.hpp)
│
├── assets/
│   └── imagenes/
│       └── Sprites, fondos y texturas
│
├── CMakeLists.txt
├── CMakePresets.json
└── README.md
```

---

## 👥 Equipo de desarrollo

Proyecto desarrollado colaborativamente por estudiantes de la Universidad Peruana Cayetano Heredia.

| Integrante | Participación |
|---|---|
| Mori Mendoza, Daniela | Desarrollo del proyecto |
| Uscamayta Lazaro, Marx | Desarrollo del proyecto |
| Jimenez Ostos, Giacomo | Desarrollo del proyecto |
| Ronceros Huaynapomas, Rolando | Desarrollo del proyecto |
| Ccoyllo Sotelo, Alex | Desarrollo del proyecto |

---

## 🎓 Propósito académico

**Escape del Laboratorio** es el proyecto final del curso de **Programación Orientada a Objetos (POO)**, correspondiente al ciclo académico **2026-2** de la Universidad Peruana Cayetano Heredia.

Su desarrollo tiene como finalidad demostrar la aplicación práctica de:

- Fundamentos de Programación Orientada a Objetos.
- Diseño y arquitectura de software.
- Implementación de patrones de diseño.
- Desarrollo de videojuegos en C++.
- Gestión de proyectos mediante Git y GitHub.
- Trabajo colaborativo y organización del código.

---

## 🚀 Proyección del proyecto

Como posibles mejoras y ampliaciones futuras se consideran:

- Incorporación de nuevos escenarios y desafíos.
- Nuevos tipos de robots y comportamientos.
- Mejoras en las animaciones del protagonista.
- Efectos de sonido y música ambiental.
- Sistema de récords y estadísticas.
- Optimización del rendimiento y de la inteligencia artificial.

---

<p align="center">
  **🐱 ¡Ayuda al gato científico a escapar del laboratorio!**
  
  🎮 Escape del Laboratorio | C++17 + SFML 3
  
  **Universidad Peruana Cayetano Heredia – 2026**
</p>
