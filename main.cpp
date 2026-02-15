
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <sstream>


private:
    sf::ConvexShape shipShape;   
    sf::RectangleShape wing1;    
    sf::RectangleShape wing2;    
    sf::RectangleShape engine;   

    float speed;                 
    sf::Vector2f position;       
public:
   
    Player(float startX, float startY) {
        speed    = 300.f;
        position = sf::Vector2f(startX, startY);

        shipShape.setPointCount(3);
        shipShape.setPoint(0, sf::Vector2f( 0.f, -30.f));  
        shipShape.setPoint(1, sf::Vector2f(-20.f,  20.f)); 
        shipShape.setPoint(2, sf::Vector2f( 20.f,  20.f)); 
        shipShape.setFillColor(sf::Color::Cyan);
        shipShape.setOutlineThickness(1.5f);
        shipShape.setOutlineColor(sf::Color(0, 200, 255));

        wing1.setSize(sf::Vector2f(15.f, 10.f));
        wing1.setOrigin(15.f, 5.f);
        wing1.setFillColor(sf::Color(0, 180, 220));

        
        wing2.setSize(sf::Vector2f(15.f, 10.f));
        wing2.setOrigin(0.f, 5.f);
        wing2.setFillColor(sf::Color(0, 180, 220));

       
        engine.setSize(sf::Vector2f(10.f, 8.f));
        engine.setOrigin(5.f, 0.f);
        engine.setFillColor(sf::Color(255, 140, 0));


        updateShapePositions();
    }

    void update(float deltaTime, const sf::RenderWindow& window) {
       
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            position.x -= speed * deltaTime;
        }
     
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            position.x += speed * deltaTime;
        }

        
        float halfW = 22.f;
        float winW  = static_cast<float>(window.getSize().x);
        if (position.x < halfW)          position.x = halfW;
        if (position.x > winW - halfW)   position.x = winW - halfW;

        updateShapePositions();
    }


    void render(sf::RenderWindow& window) const {
        window.draw(wing1);
        window.draw(wing2);
        window.draw(engine);
        window.draw(shipShape);
    }


    sf::Vector2f getPosition() const { return position; }

    sf::FloatRect getBounds() const {
        return shipShape.getGlobalBounds();
    }

private:
 
    void updateShapePositions() {
        shipShape.setPosition(position);
        wing1.setPosition(position.x - 20.f, position.y + 10.f);
        wing2.setPosition(position.x + 20.f, position.y + 10.f);
        engine.setPosition(position.x,        position.y + 20.f);
    }
};


class Game {
private:
    sf::RenderWindow window;
    Player           player;
    sf::Clock        clock;      // Used to compute delta time each frame


    sf::Font font;
    sf::Text infoText;         
    sf::Text fpsText;            


    struct Star {
        sf::CircleShape shape;
        float speed;
    };
    std::vector<Star> stars;

public:
    Game()
        : window(sf::VideoMode(800, 600), "Space Shooter - Week 1",
                 sf::Style::Titlebar | sf::Style::Close),
          player(400.f, 530.f)
    {
        window.setFramerateLimit(60);

        
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "[Warning] arial.ttf not found. Text wont display.\n"
                      << "  -> Place a .ttf font file in the same folder as the exe.\n";
        }

       
        infoText.setFont(font);
        infoText.setCharacterSize(16);
        infoText.setFillColor(sf::Color(180, 180, 180));
        infoText.setString("Arrow Keys: Move   |   ESC: Quit");
        infoText.setPosition(10.f, 570.f);

     
        fpsText.setFont(font);
        fpsText.setCharacterSize(14);
        fpsText.setFillColor(sf::Color(100, 255, 100));
        fpsText.setPosition(10.f, 10.f);

        initStars();
    }

    // run(): Main game loop.
    //   while (window is open):
    //      Calculate deltaTime
    //      Process OS events (close, keypresses)
    //      Update game state
    //      Render everything
 
    void run() {
        while (window.isOpen()) {

            // sf::Clock::restart() returns elapsed time AND resets the clock.
            float dt = clock.restart().asSeconds();
            // Cap to 0.1s so huge spikes (e.g. breakpoints) don't teleport objects
            if (dt > 0.1f) dt = 0.1f;

            processEvents();
            update(dt);
            render();
        }
    }

private:
 
    // initStars(): Randomly scatter 60 stars across the screen.
    void initStars() {
        srand(42); // Fixed seed for reproducible layout
        for (int i = 0; i < 60; ++i) {
            Star s;
            float radius = 1.f + static_cast<float>(rand() % 2);
            s.shape.setRadius(radius);
            s.shape.setFillColor(sf::Color(
                180 + rand() % 76,   // r: 180-255
                180 + rand() % 76,   // g
                200 + rand() % 56)); // b: slightly blue tint
            s.shape.setPosition(
                static_cast<float>(rand() % 800),
                static_cast<float>(rand() % 600));
            s.speed = 30.f + static_cast<float>(rand() % 80); // 30–110 px/s
            stars.push_back(s);
        }
    }


    // processEvents(): Handle OS-level events.

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Window close button
            if (event.type == sf::Event::Closed)
                window.close();

            // ESC key to quit
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
                window.close();
        }
    }


    // update(): Move stars and the player each frame.

    void update(float dt) {
        // Scroll stars downward at their individual speeds
        for (auto& s : stars) {
            s.shape.move(0.f, s.speed * dt);
            // Wrap star back to top when it leaves the bottom
            if (s.shape.getPosition().y > 605.f)
                s.shape.setPosition(s.shape.getPosition().x, -4.f);
        }

        // Update player
        player.update(dt, window);

        // Update FPS display
        std::ostringstream oss;
        oss << "FPS: " << static_cast<int>(1.f / (dt > 0.f ? dt : 0.001f));
        fpsText.setString(oss.str());
    }
    void render() {
        window.clear(sf::Color(5, 5, 20)); 
        for (const auto& s : stars)
            window.draw(s.shape);
        sf::RectangleShape floorLine(sf::Vector2f(800.f, 1.f));
        floorLine.setFillColor(sf::Color(30, 60, 90));
        floorLine.setPosition(0.f, 560.f);
        window.draw(floorLine);
        player.render(window);
        window.draw(fpsText);
        window.draw(infoText);
        window.display();
    }
};
int main() {
    Game game;
    game.run();
    return 0;
}
