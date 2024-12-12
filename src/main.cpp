#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <iostream>

#include <Jugador.hpp>
#include <Enemigo.hpp>
#include <EstadoJuego.hpp>
#include <Interfaz.hpp>
#include <Logica.hpp>

using namespace sf;

int N = 10;
int M = 10;
int size = 82;
int w = 1000;
int h = 1000;
float enemySpeed = 0.5f;
float respawnTime = 3.0f;

int main(int argc, char const *argv[]) {
    Jugador jugador;
    Enemigo enemigo;
    EstadoJuego estadoJuego;
    Interfaz interfaz;

    srand(static_cast<unsigned>(time(0)));

    RenderWindow window(VideoMode(w, h), "Cow-Bert");

    Texture t1, t2, t3;
    //cargamos las texturas
    if (!t1.loadFromFile("assets/images/Vaca3.png") || !t2.loadFromFile("assets/images/blanco.png") || !t3.loadFromFile("assets/images/Fantasma.png")) {
        std::cerr << "Error al cargar texturas" << std::endl;
        return -1;
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

            if (estadoJuego.isGameOver() && e.type == Event::KeyPressed && e.key.code == Keyboard::R) { //presionamos R para reiniciar juego
                estadoJuego.reset();
                jugador.reset();
                enemigo.respawn();
            }

            if (!estadoJuego.isGameOver() && e.type == Event::KeyPressed) { //nos movemos
                switch (e.key.code) {
                    case Keyboard::A: jugador.move(-1, 0); break;
                    case Keyboard::D: jugador.move(1, 0); break;
                    case Keyboard::W: jugador.move(0, -1); break;
                    case Keyboard::S: jugador.move(0, 1); break;
                    default: break;
                }
            }
        }

        if (!estadoJuego.isGameOver() && Logica::checkLose(jugador)) { //checar esta seccion
            estadoJuego.decreaseVidas();
            if (!estadoJuego.isGameOver()) {
                jugador.reset();
            }
        }

        if (!estadoJuego.isGameOver() && Logica::checkWin(jugador)) { //Avanzamos de nivel
            jugador.reset();
            estadoJuego.nextNivel();
        }

        if (!estadoJuego.isGameOver() && moveClock.getElapsedTime().asSeconds() > enemySpeed) { //se mueve el fantasma
            enemigo.move();
            moveClock.restart();
        }

        if (!enemigo.isVisible() && respawnClock.getElapsedTime().asSeconds() > respawnTime) { //si se sale, lo respawneamos
            enemigo.respawn();
            respawnClock.restart();
        }

        if (!estadoJuego.isGameOver() && Logica::checkCollision(jugador, enemigo)) { //chocamos con el fantasma, bajamos una vida
            estadoJuego.decreaseVidas();
            if (!estadoJuego.isGameOver()) {
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
        if (!estadoJuego.isGameOver()) {
            sprite1.setPosition(jugador.getX() * size, jugador.getY() * size);
            window.draw(sprite1);

            // Dibuja al enemigo si es visible
            if (enemigo.isVisible()) {
                sprite3.setPosition(enemigo.getX() * size, enemigo.getY() * size);
                window.draw(sprite3);
            }

            // Actualiza y dibuja el texto de nivel y vidas
            interfaz.updateLevelAndLives(estadoJuego.getNivel(), estadoJuego.getVidas());
            interfaz.draw(window, false);
        } 
        else{
            // Dibuja el texto de Game Over
            interfaz.draw(window, true);
        }

        window.display();

    }

    return 0;

}
