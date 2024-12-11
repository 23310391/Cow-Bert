#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace sf;

const int N = 10;
const int M = 10;
const int size = 82;
const int w = 1000;
const int h = 1000;
const float enemySpeed = 0.5f;
const float respawnTime = 3.0f;

class Jugador {
private:
    int x, y;
    std::vector<std::vector<bool>> visited;

public:
    Jugador() : x(1), y(1), visited(N, std::vector<bool>(M, false)) {
        visited[0][0] = true;
    }

    int getX() const { return x; }
    int getY() const { return y; }

    void move(int dx, int dy) {
        int newX = x + dx;
        int newY = y + dy;

        if (newX >= 0 && newX < N + 2 && newY >= 0 && newY < M + 2) {
            x = newX;
            y = newY;

            if (newX > 0 && newX <= N && newY > 0 && newY <= M) {
                visited[newX - 1][newY - 1] = true;
            }
        }
    }

    bool isVisited(int gridX, int gridY) const {
        return visited[gridX][gridY];
    }

    void reset() {
        x = y = 1;
        visited = std::vector<std::vector<bool>>(N, std::vector<bool>(M, false));
        visited[0][0] = true;
    }

    bool hasVisitedAll() const {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                if (!visited[i][j])
                    return false;
        return true;
    }
};

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

int main(int argc, char const *argv[]) 
{
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

    Jugador jugador;
    Enemigo enemigo;
    EstadoJuego estadojuego;
    Interfaz interfaz;

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
