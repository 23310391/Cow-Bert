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
#include <Interfaz.hpp>
#include <Logica.hpp>

const int N = 10;
const int M = 10;
const int size = 82;
const int w = 1000;
const int h = 1000;
const float enemySpeed = 0.5f;
const float respawnTime = 3.0f;

class EstadoJuego 
{
private:
    int nivel;
    int vidas;
    bool gameOver;

public:
    EstadoJuego(){}
    ~EstadoJuego(){}
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
