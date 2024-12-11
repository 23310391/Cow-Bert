#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

#include <Jugador.hpp>
#include <Enemigo.hpp>
#include <EstadoJuego.hpp>
#include <Interfaz.hpp>
#include <Logica.hpp>

using namespace sf;

const int N = 10;
const int M = 10;
const int size = 82;
const int w = 1000;
const int h = 1000;
const float enemySpeed = 0.5f;
const float respawnTime = 3.0f;

int main(int argc, char const *argv[]) 
{
    Jugador jugador;
    Enemigo enemigo;
    EstadoJuego estadojuego;
    Interfaz interfaz;

    srand(static_cast<unsigned>(time(0)));

    RenderWindow window(VideoMode(w, h), "Cow-Bert");

    Texture t1, t2, t3;
    if (!t1.loadFromFile("assets/images/Vaca3.png") ||
        !t2.loadFromFile("assets/images/blanco.png") ||
        !t3.loadFromFile("assets/images/Fantasma.png")) {
        return -1; // Manejo de error al cargar texturas
    }

    Sprite sprite1(t1);
    Sprite sprite2(t2);
    Sprite sprite3(t3);

    Clock moveClock;
    Clock respawnClock;

    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();

            if (estadojuego.isGameOver() && e.type == Event::KeyPressed && e.key.code == Keyboard::R) {
                estadojuego.reset();
                jugador.reset();
                enemigo.respawn();
            }

            if (!estadojuego.isGameOver() && e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::A) jugador.move(-1, 0);
                if (e.key.code == Keyboard::D) jugador.move(1, 0);
                if (e.key.code == Keyboard::W) jugador.move(0, -1);
                if (e.key.code == Keyboard::S) jugador.move(0, 1);
            }
        }

        if (!estadojuego.isGameOver() && Logica::checkLose(jugador)) {
            estadojuego.decreaseVidas();
            if (!estadojuego.isGameOver()) {
                jugador.reset();
            }
        }

        if (!estadojuego.isGameOver() && Logica::checkWin(jugador)) {
            jugador.reset();
            estadojuego.nextNivel();
        }

        if (!estadojuego.isGameOver() && moveClock.getElapsedTime().asSeconds() > enemySpeed) {
            enemigo.move();
            moveClock.restart();
        }

        if (!enemigo.isVisible() && respawnClock.getElapsedTime().asSeconds() > respawnTime) {
            enemigo.respawn();
            respawnClock.restart();
        }

        if (!estadojuego.isGameOver() && Logica::checkCollision(jugador, enemigo)) {
            estadojuego.decreaseVidas();
            if (!estadojuego.isGameOver()) {
                jugador.reset();
                enemigo.respawn();
            }
        }

        window.clear(Color::Black);

        // Dibuja el fondo del juego
        for (int i = 0; i < N + 2; i++) {
            for (int j = 0; j < M + 2; j++) {
                sprite2.setPosition(i * size, j * size);
                sprite2.setColor((i == 0 || j == 0 || i == N + 1 || j == M + 1) ? Color::Black : (jugador.isVisited(i - 1, j - 1) ? Color::Magenta : Color::Cyan));
                window.draw(sprite2);
            }
        }

        // Dibuja al jugador
        if (!estadojuego.isGameOver()) {
            sprite1.setPosition(jugador.getX() * size, jugador.getY() * size);
            window.draw(sprite1);

            // Dibuja al enemigo si es visible
            if (enemigo.isVisible()) {
                sprite3.setPosition(enemigo.getX() * size, enemigo.getY() * size);
                window.draw(sprite3);
            }

            // Actualiza y dibuja el texto de nivel y vidas
            interfaz.updateLevelAndLives(estadojuego.getNivel(), estadojuego.getVidas());
            interfaz.draw(window, false);
        } else {
            // Dibuja el texto de Game Over
            interfaz.draw(window, true);
        }

        window.display();
    }

    return 0;
}
