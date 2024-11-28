#include <SFML/Graphics.hpp>

class Personaje {
public:
    Personaje(sf::Vector2f position, sf::Color color) {
        shape.setRadius(25);  // Hacerlo circular
        shape.setPosition(position);
        shape.setFillColor(color);
    }

    void move(float offsetX, float offsetY) {
        shape.move(offsetX, offsetY);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

private:
    sf::CircleShape shape;
};

const float CASILLA_SIZE = 50.0f;  // Tamaño del paso en una "casilla"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Movimiento por Casillas");

    Personaje character(sf::Vector2f(400, 300), sf::Color::Cyan);  // Posición inicial en el centro
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
