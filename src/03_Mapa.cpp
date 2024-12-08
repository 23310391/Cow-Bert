#include <SFML/Graphics.hpp>
#include <vector>

void drawRectangle(sf::RenderWindow& window, float x, float y, float size, const sf::Color& color) {
    // Crear un rectángulo
    sf::RectangleShape rectangle(sf::Vector2f(size, size));
    rectangle.setPosition(x, y);
    rectangle.setFillColor(color);
    window.draw(rectangle);
}

int main() {
    // Crear una ventana
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pirámide de Colores Alternos");

    // Definir el tamaño de los cubos
    float cubeSize = 50.0f;  // Tamaño de un cubo

    // Definir el número de niveles de la pirámide
    int baseLevels = 7;

    // Definir los colores para alternar
    sf::Color color1 = sf::Color::Yellow;
    sf::Color color2 = sf::Color::Blue;

    // Bucle principal
    while (window.isOpen())
    {
        // Procesar eventos
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // Cerrar la ventana si se recibe el evento de cerrar
                window.close();
            }
        }

        // Limpiar la ventana
        window.clear(sf::Color::Black);

        // Dibujar la pirámide de cubos
        for (int level = 0; level < baseLevels; ++level)
        {
            int numCubes = baseLevels - level;
            for (int i = 0; i < numCubes; ++i)
            {
                // Calcular la posición x y y para cada cubo
                float x = (window.getSize().x / 2) - (numCubes * cubeSize / 2) + (i * cubeSize);
                float y = (window.getSize().y / 2) + (level * cubeSize / 2) - (level * cubeSize);

                // Alternar colores entre dos colores fijos
                sf::Color color = (i % 2 == 0) ? color1 : color2;

                // Dibujar el rectángulo
                drawRectangle(window, x, y, cubeSize, color);
            }
        }

        // Mostrar la ventana
        window.display();
    }

    return 0;
}
