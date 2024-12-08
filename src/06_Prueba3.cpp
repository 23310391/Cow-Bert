#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>

using namespace sf;

const int N = 10; // Columnas
const int M = 10; // Filas
const int size = 80; // Tamaño de cada celda
const int w = 1000; // Ancho de la ventana
const int h = 900; // Alto de la ventana

struct Player {
    int x, y;
    std::vector<std::vector<bool>> visited;
    Player() {
        x = y = 0;
        visited.resize(N, std::vector<bool>(M, false));
        visited[0][0] = true;
    }
};

void move(Player& player, int dx, int dy) {
    int newX = player.x + dx;
    int newY = player.y + dy;

    // Verificar los límites de la cuadrícula
    if (newX >= 0 && newX < N && newY >= 0 && newY < M) {
        player.x = newX;
        player.y = newY;
        player.visited[newX][newY] = true;
    }
}

bool checkWin(const Player& player) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (!player.visited[i][j])
                return false;
    return true;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    RenderWindow window(VideoMode(w, h), "Q-bert");

    Texture t1;
    t1.loadFromFile("assets/images/Vaca3.png");

    Sprite sprite1(t1);

    Player player;

    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();

            // Manejar las teclas cuando son presionadas
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Left) move(player, -1, 0);
                if (e.key.code == Keyboard::Right) move(player, 1, 0);
                if (e.key.code == Keyboard::Up) move(player, 0, -1);
                if (e.key.code == Keyboard::Down) move(player, 0, 1);
            }
        }

        if (checkWin(player)) {
            // Si ganas, reiniciamos el juego
            player = Player();
        }

        window.clear(Color::Black);

        // Dibujar la cuadrícula
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                sprite1.setPosition(i * size, j * size);
                if (player.visited[i][j]) {
                    sprite1.setColor(Color::Green); // Marca los lugares visitados
                } else {
                    sprite1.setColor(Color::White);
                }
                window.draw(sprite1);
            }
        }

        // Dibujar al jugador
        sprite1.setPosition(player.x * size, player.y * size);
        sprite1.setColor(Color::Red);
        window.draw(sprite1);

        window.display();
    }

    return 0;
}
