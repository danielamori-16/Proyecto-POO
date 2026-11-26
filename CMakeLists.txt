cmake_minimum_required(VERSION 3.20)
project(EscapeDelLaboratorio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics)
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(EscapeDelLaboratorio
    src/main.cpp
    src/Juego.cpp
    src/EstadoJuego.cpp
    src/HUD.cpp
    src/Personaje.cpp
    src/Cientifico.cpp
    src/Robot.cpp
    src/RobotPerseguidor.cpp
    src/Objeto.cpp
    src/Puntuacion.cpp
    src/Nivel.cpp
    src/BuscadorRutas.cpp
    src/RobotGuardian.cpp
    src/RobotAtaque.cpp
    src/APIClient.cpp
)

target_include_directories(
    EscapeDelLaboratorio
    PRIVATE include
)

target_link_libraries(
    EscapeDelLaboratorio
    PRIVATE
        SFML::Graphics
        CURL::libcurl
        nlohmann_json::nlohmann_json
)