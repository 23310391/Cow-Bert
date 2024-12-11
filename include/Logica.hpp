#pragma once
#include<iostream>
using namespace std;

#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

#include <Jugador.hpp>
#include <Enemigo.hpp>
#include <EstadoJuego.hpp>
#include <Interfaz.hpp>

const int N = 10;
const int M = 10;
const int size = 82;
const int w = 1000;
const int h = 1000;
const float enemySpeed = 0.5f;
const float respawnTime = 3.0f;

class Logica {
public:
    Logica(){}
    ~Logica(){}
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