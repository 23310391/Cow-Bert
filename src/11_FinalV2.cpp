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

class Player {
private:
    int x, y;
    std::vector<std::vector<bool>> visited;

public:
    Player() : x(1), y(1), visited(N, std::vector<bool>(M, false)) {
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

class Enemy {
private:
    int x, y;
    bool visible;

public:
    Enemy() {
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

class GameLogic {
public:
    static bool checkWin(const Player& player) {
        return player.hasVisitedAll();
    }

    static bool checkLose(const Player& player) {
        return (player.getX() == 0 || player.getX() == N + 1 || player.getY() == 0 || player.getY() == M + 1);
    }

    static bool checkCollision(const Player& player, const Enemy& enemy) {
        return enemy.isVisible() && player.getX() == enemy.getX() && player.getY() == enemy.getY();
    }
};

class GameState {
private:
    int nivel;
    int vidas;
    bool gameOver;

public:
    GameState() : nivel(1), vidas(3), gameOver(false) {}

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

int main() {
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

    Font font;
    if (!font.loadFromFile("assets/fonts/Minecraft.ttf")) {
        return -1; // Manejo de error al cargar la fuente
    }

    Text text;
    text.setFont(font);
    text.setCharacterSize(30);
    text.setFillColor(Color::White);
    text.setPosition(10, 10);

    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(w / 2 - 150, h / 2 - 50);
    gameOverText.setString("Game Over");

    Text restartText;
    restartText.setFont(font);
    restartText.setCharacterSize(30);
    restartText.setFillColor(Color::Black);
    restartText.setPosition(w / 2 - 200, h / 2 + 30);
    restartText.setString("Presiona R para reiniciar");

    Player player;
    Enemy enemy;
    GameState gameState;

    Clock moveClock;
    Clock respawnClock;

    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();

            if (gameState.isGameOver() && e.type == Event::KeyPressed && e.key.code == Keyboard::R) {
                gameState.reset();
                player.reset();
                enemy.respawn();
            }

            if (!gameState.isGameOver() && e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::A) player.move(-1, 0);
                if (e.key.code == Keyboard::D) player.move(1, 0);
                if (e.key.code == Keyboard::W) player.move(0, -1);
                if (e.key.code == Keyboard::S) player.move(0, 1);
            }
        }

        if (!gameState.isGameOver() && GameLogic::checkLose(player)) {
            gameState.decreaseVidas();
            if (!gameState.isGameOver()) {
                player.reset();
            }
        }

        if (!gameState.isGameOver() && GameLogic::checkWin(player)) {
            player.reset();
            gameState.nextNivel();
        }

        if (!gameState.isGameOver() && moveClock.getElapsedTime().asSeconds() > enemySpeed) {
            enemy.move();
            moveClock.restart();
        }

        if (!enemy.isVisible() && respawnClock.getElapsedTime().asSeconds() > respawnTime) {
            enemy.respawn();
            respawnClock.restart();
        }

        if (!gameState.isGameOver() && GameLogic::checkCollision(player, enemy)) {
            gameState.decreaseVidas();
            if (!gameState.isGameOver()) {
                player.reset();
                enemy.respawn();
            }
        }

        window.clear(Color::Black);

        for (int i = 0; i < N + 2; i++) {
            for (int j = 0; j < M + 2; j++) {
                sprite2.setPosition(i * size, j * size);
                sprite2.setColor((i == 0 || j == 0 || i == N + 1 || j == M + 1) ? Color::Black : (player.isVisited(i - 1, j - 1) ? Color::Green : Color::White));
                window.draw(sprite2);
            }
        }

        if (!gameState.isGameOver()) {
            sprite1.setPosition(player.getX() * size, player.getY() * size);
            window.draw(sprite1);

            if (enemy.isVisible()) {
                sprite3.setPosition(enemy.getX() * size, enemy.getY() * size);
                window.draw(sprite3);
            }

            text.setString("Nivel: " + std::to_string(gameState.getNivel()) + "  Vidas: " + std::to_string(gameState.getVidas()));
            window.draw(text);
        } else {
            window.draw(gameOverText);
            window.draw(restartText);
        }

        window.display();
    }

    return 0;
}