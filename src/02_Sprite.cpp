#include <SFML/Graphics.hpp>

class Personaje {
public:
    Personaje(sf::Vector2f position, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setPosition(position);
        sprite.setScale(0.9f, 0.9f);  // Ajusta la escala si es necesario para que encaje en el espacio
    }

    void move(float offsetX, float offsetY) {
        sprite.move(offsetX, offsetY);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

private:
    sf::Sprite sprite;
};

const float CASILLA_SIZE = 50.0f;  // Tamaño del paso en una "casilla"

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Movimiento por Casillas");

    // Cargar la textura del archivo de imagen
    sf::Texture texture;
    if (!texture.loadFromFile("assets/images/Vaca3.png")) {  // Asegúrate de tener un archivo "sprite.png" en el mismo directorio
        return -1;  // Salir si no se pudo cargar la textura
    }

    // Crear el personaje con la textura cargada
    Personaje character(sf::Vector2f(400, 300), texture);
    bool keyHeld = false;  // Indica si la tecla ya fue procesada mientras está presionada

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Detectar movimiento solo cuando una tecla es presionada inicialmente
        if (!keyHeld) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                character.move(CASILLA_SIZE, -CASILLA_SIZE);  // Arriba derecha
                keyHeld = true;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                character.move(-CASILLA_SIZE, -CASILLA_SIZE);  // Arriba izquierda
                keyHeld = true;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                character.move(-CASILLA_SIZE, CASILLA_SIZE);  // Abajo izquierda
                keyHeld = true;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                character.move(CASILLA_SIZE, CASILLA_SIZE);  // Abajo derecha
                keyHeld = true;
            }
        }

        // Resetear el estado cuando se suelta la tecla
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            keyHeld = false;
        }

        window.clear();
        character.draw(window);
        window.display();
    }

    return 0;
}
