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
#include <Logica.hpp>

const int N = 10;
const int M = 10;
const int size = 82;
const int w = 1000;
const int h = 1000;
const float enemySpeed = 0.5f;
const float respawnTime = 3.0f;

class Interfaz 
{
private:
    Font font;
    Text levelAndLivesText;
    Text gameOverText;
    Text restartText;

public:
    Interfaz(){}
    ~Interfaz(){}
    Interfaz() {
        if (!font.loadFromFile("assets/fonts/Minecraft.ttf")) {
            throw std::runtime_error("Error loading font");
        }

        levelAndLivesText.setFont(font);
        levelAndLivesText.setCharacterSize(30);
        levelAndLivesText.setFillColor(Color::White);
        levelAndLivesText.setPosition(10, 10);

        gameOverText.setFont(font);
        gameOverText.setCharacterSize(50);
        gameOverText.setFillColor(Color::Red);
        gameOverText.setPosition(w / 2 - 150, h / 2 - 50);
        gameOverText.setString("Game Over");

        restartText.setFont(font);
        restartText.setCharacterSize(30);
        restartText.setFillColor(Color::Black);
        restartText.setPosition(w / 2 - 200, h / 2 + 30);
        restartText.setString("Presiona R para reiniciar");
    }

    void updateLevelAndLives(int nivel, int vidas) {
        levelAndLivesText.setString("Nivel: " + std::to_string(nivel) + "  Vidas: " + std::to_string(vidas));
    }

    void draw(RenderWindow& window, bool gameOver) {
        if (gameOver) {
            window.draw(gameOverText);
            window.draw(restartText);
        } else {
            window.draw(levelAndLivesText);
        }
    }
};