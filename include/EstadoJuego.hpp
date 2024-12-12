#pragma once
#include <SFML/Graphics.hpp>

extern int N;
extern int M;
extern int size;
extern int w;
extern int h;
extern float enemySpeed;
extern float respawnTime;

class EstadoJuego {
private:
    int nivel;
    int vidas;
    bool gameOver;

public:
    EstadoJuego() : nivel(1), vidas(3), gameOver(false) {}

    int getNivel() const { return nivel; }
    int getVidas() const { return vidas; }
    bool isGameOver() const { return gameOver; }

    void decreaseVidas() {
        vidas--;
        if (vidas <= 0) {
            gameOver = true;
        }
    }

    void nextNivel() { nivel++; }

    void reset() {
        nivel = 1;
        vidas = 3;
        gameOver = false;
    }
};
