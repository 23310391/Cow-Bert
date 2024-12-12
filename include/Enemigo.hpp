#pragma once
#include <SFML/Graphics.hpp>
#include <random>

extern int N;
extern int M;

class Enemigo {
private:
    int x, y;
    bool visible;

public:
    Enemigo() {
        respawn();
        visible = true;
    }

    int getX() const { return x; }
    int getY() const { return y; }
    bool isVisible() const { return visible; }

    void move() {
        if (!visible) return;

        // Generar un movimiento aleatorio usando std::random
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(-1, 1);

        int dx = dis(gen);
        int dy = dis(gen);

        x += dx;
        y += dy;

        if (x < -1 || x > N + 1 || y < -1 || y > M + 1) {
            visible = false;
        }
    }

    void respawn() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, N);

        x = dis(gen);
        y = dis(gen);
        visible = true;
    }
};
