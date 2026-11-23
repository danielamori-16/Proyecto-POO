
#pragma once

#include <memory>

#include "Robot.hpp"
#include "RobotPerseguidor.hpp"
#include "RobotGuardian.hpp"
#include "RobotAtaque.hpp"

// ========================================
// FACTORY METHOD - CREADOR ABSTRACTO
// ========================================

// POO - ABSTRACCION:
// Define como se crea un robot sin
// especificar su clase concreta.

class FabricaRobots {
public:
    virtual ~FabricaRobots() = default;

    // FACTORY METHOD:
    // Las fabricas hijas deciden que robot crear.
    virtual std::unique_ptr<Robot> crear(
        const Personaje& objetivo
    ) const = 0;
};

// ========================================
// FABRICA DEL ROBOT PERSEGUIDOR
// ========================================

class FabricaPerseguidor : public FabricaRobots {
public:
    std::unique_ptr<Robot> crear(
        const Personaje& objetivo
    ) const override {

        return std::make_unique<RobotPerseguidor>(
            objetivo
        );
    }
};

// ========================================
// FABRICA DEL ROBOT GUARDIAN
// ========================================

class FabricaGuardian : public FabricaRobots {
private:
    sf::Vector2f inicio;
    sf::Vector2f final;

public:
    FabricaGuardian(
        sf::Vector2f puntoInicial,
        sf::Vector2f puntoFinal
    )
        : inicio(puntoInicial),
          final(puntoFinal) {
    }

    std::unique_ptr<Robot> crear(
        const Personaje& objetivo
    ) const override {

        // El guardian no necesita perseguir
        // a un objetivo para patrullar.
        (void)objetivo;

        return std::make_unique<RobotGuardian>(
            inicio,
            final
        );
    }
};

// ========================================
// FABRICA DEL ROBOT DE ATAQUE
// ========================================

class FabricaAtaque : public FabricaRobots {
private:
    sf::Vector2f posicion;

public:
    explicit FabricaAtaque(
        sf::Vector2f posicionInicial
    )
        : posicion(posicionInicial) {
    }

    std::unique_ptr<Robot> crear(
        const Personaje& objetivo
    ) const override {

        return std::make_unique<RobotAtaque>(
            objetivo,
            posicion
        );
    }
};
