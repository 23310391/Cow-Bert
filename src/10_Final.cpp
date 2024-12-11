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
int nivel = 1;
int vidas = 3;
const float enemySpeed = 0.5f;
const float respawnTime = 3.0f;

struct Player {
    int x, y;
    std::vector<std::vector<bool>> visited;
    Player() {
        x = y = 1;
        visited.resize(N, std::vector<bool>(M, false));
        visited[0][0] = true;
    }
};

struct Enemy {
    int x, y;
    bool visible;

    Enemy() {
        respawn();
        visible = true;
    }

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

void move(Player& player, int dx, int dy) {
    int newX = player.x + dx;
    int newY = player.y + dy;

    if (newX >= 0 && newX < N + 2 && newY >= 0 && newY < M + 2) {
        player.x = newX;
        player.y = newY;

        if (newX > 0 && newX <= N && newY > 0 && newY <= M) {
            player.visited[newX - 1][newY - 1] = true;
        }
    }
}

bool checkWin(const Player& player) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (!player.visited[i][j])
                return false;
    return true;
}

bool checkLose(const Player& player) {
    return (player.x == 0 || player.x == N + 1 || player.y == 0 || player.y == M + 1);
}

bool checkCollision(const Player& player, const Enemy& enemy) {
    return enemy.visible && player.x == enemy.x && player.y == enemy.y;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    RenderWindow window(VideoMode(w, h), "Cow-Bert");

    Texture t1, t2, t3;
    t1.loadFromFile("assets/images/Vaca3.png");
    t2.loadFromFile("assets/images/blanco.png");
    t3.loadFromFile("assets/images/Fantasma.png");

    Sprite sprite1(t1);
    Sprite sprite2(t2);
    Sprite sprite3(t3);

    Font font;
    if (!font.loadFromFile("assets/fonts/Minecraft.ttf")) {
        return -1;
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
    bool gameOver = false;

    Clock moveClock;
    Clock respawnClock;

    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();

            if (gameOver && e.type == Event::KeyPressed && e.key.code == Keyboard::R) {
                gameOver = false;
                vidas = 3;
                nivel = 1;
                player = Player();
                enemy = Enemy();
            }

            if (!gameOver && e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::A) move(player, -1, 0);
                if (e.key.code == Keyboard::D) move(player, 1, 0);
                if (e.key.code == Keyboard::W) move(player, 0, -1);
                if (e.key.code == Keyboard::S) move(player, 0, 1);
            }
        }

        if (!gameOver && checkLose(player)) {
            vidas--;
            if (vidas > 0) {
                player = Player();
            } else {
                gameOver = true;
            }
        }

        if (!gameOver && checkWin(player)) {
            player = Player();
            nivel++;
        }

        if (!gameOver && moveClock.getElapsedTime().asSeconds() > enemySpeed) {
            enemy.move();

            if (enemy.visible && enemy.x > 0 && enemy.x <= N && enemy.y > 0 && enemy.y <= M) {
                if (player.visited[enemy.x - 1][enemy.y - 1]) {
                    player.visited[enemy.x - 1][enemy.y - 1] = false;
                }
            }

            moveClock.restart();
        }

        if (!enemy.visible && respawnClock.getElapsedTime().asSeconds() > respawnTime) {
            enemy.respawn();
            respawnClock.restart();
        }

        if (!gameOver && checkCollision(player, enemy)) {
            vidas--;
            if (vidas > 0) {
                player = Player();
                enemy.respawn();
            } else {
                gameOver = true;
            }
        }

        window.clear(Color::Black);

        for (int i = 0; i < N + 2; i++) {
            for (int j = 0; j < M + 2; j++) {
                sprite2.setPosition(i * size, j * size);
                sprite2.setColor((i == 0 || j == 0 || i == N + 1 || j == M + 1) ? Color::Black : (player.visited[i - 1][j - 1] ? Color::Green : Color::White));
                window.draw(sprite2);
            }
        }

        if (!gameOver) {
            sprite1.setPosition(player.x * size, player.y * size);
            window.draw(sprite1);

            if (enemy.visible) {
                sprite3.setPosition(enemy.x * size, enemy.y * size);
                window.draw(sprite3);
            }

            text.setString("Nivel: " + std::to_string(nivel) + "  Vidas: " + std::to_string(vidas));
            window.draw(text);
        } else {
            window.draw(gameOverText);
            window.draw(restartText);
        }

        window.display();
    }

    return 0;
}
