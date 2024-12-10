#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string> // Para convertir números a texto

using namespace sf;

const int N = 10; // Columnas
const int M = 10; // Filas
const int size = 82; // Tamaño de cada celda
const int w = 1000; // Ancho de la ventana
const int h = 900; // Alto de la ventana
int nivel = 1; // Contador de nivel
int vidas = 3; // Número de vidas del jugador
const float enemySpeed = 0.5f; // Velocidad del enemigo (en segundos)
const float respawnTime = 3.0f; // Tiempo de reaparecer el enemigo (en segundos)

struct Player {
    int x, y; // Coordenadas en la cuadrícula extendida
    std::vector<std::vector<bool>> visited;
    Player() {
        x = y = 1; // Comienza dentro de la cuadrícula jugable
        visited.resize(N, std::vector<bool>(M, false));
        visited[0][0] = true; // Casilla inicial marcada como visitada
    }
};

struct Enemy {
    int x, y; // Coordenadas del enemigo
    bool visible; // Determina si el enemigo está visible

    Enemy() {
        respawn(); // Inicializa en una posición aleatoria
        visible = true; // Empieza visible
    }

    void move() {
        if (!visible) return; // Si no está visible, no se mueve

        int dx = (rand() % 3) - 1; // Movimiento aleatorio: -1, 0 o 1
        int dy = (rand() % 3) - 1;
        x += dx;
        y += dy;

        // Permitir que el enemigo salga de la cuadrícula
        if (x < -1 || x > N + 1 || y < -1 || y > M + 1) {
            visible = false; // Hacerlo invisible si sale
        }
    }

    void respawn() {
        x = rand() % N + 1; // Posición inicial aleatoria dentro de la cuadrícula
        y = rand() % M + 1;
        visible = true;
    }
};

void move(Player& player, int dx, int dy) {
    int newX = player.x + dx;
    int newY = player.y + dy;

    // Verificar los límites de la cuadrícula incluyendo las casillas invisibles
    if (newX >= 0 && newX < N + 2 && newY >= 0 && newY < M + 2) {
        player.x = newX;
        player.y = newY;

        // Si estamos dentro de la cuadrícula jugable, marcar como visitada
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

    RenderWindow window(VideoMode(w, h), "Q-bert");

    Texture t1, t2, t3;
    t1.loadFromFile("assets/images/Vaca3.png"); // personaje
    t2.loadFromFile("assets/images/blanco.png"); // cuadrado para la cuadrícula
    t3.loadFromFile("assets/images/Fantasma.png");

    Sprite sprite1(t1); // vaca
    Sprite sprite2(t2); // cuadrado
    Sprite sprite3(t3); // fantasma

    // Fuente para el texto
    Font font;
    if (!font.loadFromFile("assets/fonts/Minecraft.ttf")) {
        return -1; // Asegúrate de tener esta fuente o usa otra disponible
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

    Player player;
    Enemy enemy;
    bool gameOver = false;

    Clock moveClock; // Reloj para controlar el movimiento del enemigo
    Clock respawnClock; // Reloj para controlar la reaparición del enemigo

    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();

            if (!gameOver && e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Left) move(player, -1, 0);
                if (e.key.code == Keyboard::Right) move(player, 1, 0);
                if (e.key.code == Keyboard::Up) move(player, 0, -1);
                if (e.key.code == Keyboard::Down) move(player, 0, 1);
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

        // Movimiento y reaparición del enemigo
        if (!gameOver && moveClock.getElapsedTime().asSeconds() > enemySpeed) {
            enemy.move();
            moveClock.restart();
        }

        if (!enemy.visible && respawnClock.getElapsedTime().asSeconds() > respawnTime) {
            enemy.respawn();
            respawnClock.restart();
        }

        // Verificar colisión
        if (!gameOver && checkCollision(player, enemy)) {
            vidas--;
            if (vidas > 0) {
                player = Player();
                enemy.respawn();
            } else {
                gameOver = true;
            }
        }

        // Renderizado
        window.clear(Color::Black);

        // Dibujar la cuadrícula
        for (int i = 0; i < N + 2; i++) {
            for (int j = 0; j < M + 2; j++) {
                sprite2.setPosition(i * size, j * size);
                sprite2.setColor((i == 0 || j == 0 || i == N + 1 || j == M + 1) ? Color::Black : (player.visited[i - 1][j - 1] ? Color::Green : Color::White));
                window.draw(sprite2);
            }
        }

        if (!gameOver) {
            // Dibujar al jugador
            sprite1.setPosition(player.x * size, player.y * size);
            window.draw(sprite1);

            // Dibujar al enemigo si es visible
            if (enemy.visible) {
                sprite3.setPosition(enemy.x * size, enemy.y * size);
                window.draw(sprite3);
            }

            // Mostrar nivel y vidas
            text.setString("Nivel: " + std::to_string(nivel) + "  Vidas: " + std::to_string(vidas));
            window.draw(text);
        } else {
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}
