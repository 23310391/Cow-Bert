#pragma once
#include<iostream>
using namespace std;

#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

#include <Jugador.hpp>
#include <EstadoJuego.hpp>
#include <Interfaz.hpp>
#include <Logica.hpp>

const int N = 10;
const int M = 10;
const int size = 82;
const int w = 1000;
const int h = 1000;
const float enemySpeed = 0.5f;
const float respawnTime = 3.0f;

class Enemigo 
{
private:
    int x, y;
    bool visible;

public:
    Enemigo(){}
    ~Enemigo(){}
    Enemigo() {
        respawn();
        visible = true;
    }

    int getX() const { return x; }
    int getY() const { return y; }
    bool isVisible() const { return visible; }

    void move() {
        if (!visible) return;

        int dx = (rand() % 3) - 1;
        int dy = (rand() % 3) - 1;
        x += dx;
        y += dy;

        if (x < -1 || x > N + 1 || y < -1 || y > M + 1) {
            visible = false;
        }
    }

    void respawn() {
        x = rand() % N + 1;
        y = rand() % M + 1;
        visible = true;
    }
};
