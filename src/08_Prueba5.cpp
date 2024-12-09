#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <string> // Para convertir números a texto

using namespace sf;

const int N = 10; // Columnas
const int M = 10; // Filas
const int size = 82; // Tamaño de cada celda
const int w = 1000; // Ancho de la ventana
const int h = 900; // Alto de la ventana
int nivel = 1; // Contador de nivel

struct Player {
    int x, y; // Coordenadas en la cuadrícula extendida
    std::vector<std::vector<bool>> visited;
    Player() {
        x = y = 1; // Comenzamos dentro de la cuadrícula, no en el borde negro
        visited.resize(N, std::vector<bool>(M, false));
        visited[0][0] = true; // Casilla inicial marcada como visitada
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

int main() {
    srand(static_cast<unsigned>(time(0)));

    RenderWindow window(VideoMode(w, h), "Q-bert");

    Texture t1;
    t1.loadFromFile("assets/images/Vaca3.png"); // personaje
    Texture t2;
    t2.loadFromFile("assets/images/blanco.png"); // cuadrado para la cuadrícula

    Sprite sprite1(t1); // vaca
    Sprite sprite2(t2); // cuadrado

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

        if (checkLose(player)) {
            // Si pierdes, reinicia el nivel
            player = Player();
        }

        if (checkWin(player)) {
            // Si ganas, pasa al siguiente nivel
            player = Player();
            nivel++;
            printf("Nivel %i\n", nivel);
        }

        window.clear(Color::Black); // color del fondo

        // Dibujar la cuadrícula (con borde negro)
        for (int i = 0; i < N + 2; i++) {
            for (int j = 0; j < M + 2; j++) {
                sprite2.setPosition(i * size, j * size);

                if (i == 0 || j == 0 || i == N + 1 || j == M + 1) {
                    sprite2.setColor(Color::Black); // Casillas invisibles
                } else if (player.visited[i - 1][j - 1]) {
                    sprite2.setColor(Color::Green); // Marca los lugares visitados
                } else {
                    sprite2.setColor(Color::White);
                }
                window.draw(sprite2);
            }
        }

        // Dibujar al jugador
        sprite1.setPosition(player.x * size, player.y * size);
        sprite1.setColor(Color::Blue);
        window.draw(sprite1);

        // Mostrar el nivel en la pantalla
        text.setString("Nivel: " + std::to_string(nivel));
        window.draw(text);

        window.display();
    }

    return 0;
}
