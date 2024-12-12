#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

#include <Jugador.hpp>
#include <Enemigo.hpp>

extern int N;
extern int M;
extern int size;
extern int w;
extern int h;
extern float enemySpeed;
extern float respawnTime;

class Logica {
public:
    static bool checkWin(const Jugador& jugador) {
        return jugador.hasVisitedAll();
    }

    static bool checkLose(const Jugador& jugador) {
        return (jugador.getX() == 0 || jugador.getX() == N + 1 || jugador.getY() == 0 || jugador.getY() == M + 1);
    }

    static bool checkCollision(const Jugador& jugador, const Enemigo& enemigo) {
        return enemigo.isVisible() && jugador.getX() == enemigo.getX() && jugador.getY() == enemigo.getY();
    }
};
