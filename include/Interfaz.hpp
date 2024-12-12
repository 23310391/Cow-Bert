#pragma once
#include <SFML/Graphics.hpp>
#include <stdexcept>
using namespace sf;

extern int N;
extern int M;
extern int size;
extern int w;
extern int h;
extern float enemySpeed;
extern float respawnTime;

class Interfaz {
private:
    Font font;
    Text levelAndLivesText;
    Text gameOverText;
    Text restartText;

public:
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
