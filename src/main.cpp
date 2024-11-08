#include <SFML/Graphics.hpp>
#include <iostream>

sf::Vector2f cactusSize() {
    float height, width;
    //width
    int n=rand() % 3;
    switch (n) {
        case 0:
            width=75;
            break;
        case 1:
            width=100;
            break;
        case 2:
            width=150;
    }
    n=rand() % 2;
    switch (n) {
        case 0:
            height=100;
            break;
        case 1:
            height=150;
    }
    sf::Vector2f v = {width,height};
    return v;
}

int main()
{
    auto window = sf::RenderWindow{ { 1920u, 1080u }, "CMake SFML Project" };
    window.setFramerateLimit(144);
    
    float jt=0;
    float ct=0;
    float dt=1.f/144.f;
    sf::Vector2f catSize(100,100);
    sf::Vector2u screen = window.getSize();
    float sizeFloor=200;
    float heightFloor=screen.y-sizeFloor;
    float catBasePos=heightFloor-catSize.y;
    float jumpTime=0.75;
    float speedScroll=1000*dt;
    bool jumping=false;

    while (window.isOpen())
    {
        bool jump=false;
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

        }

        window.clear();

        //cat jumps
        jump=sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        if (jump){
            jumping=true;
        }
        if (jumping){
            jt+=dt;
            if (jt >= jumpTime){
                jumping=false;
                jt=0;
            }
        }

        //cactus
        sf::Vector2f cactuSize = cactusSize();
        sf::RectangleShape cactus1(cactusSize());
        ct+=dt;
        cactus1.setPosition(screen.x-speedScroll*ct,heightFloor-cactusSize().y);
        cactus1.setFillColor(sf::Color::Red);
        window.draw(cactus1);

        //cat
        sf::RectangleShape cat(catSize);
        cat.setPosition(200,catBasePos+(jt*(jt-jumpTime))*1600);
        cat.setFillColor(sf::Color::Blue);
        window.draw(cat);

        //floor
        sf::RectangleShape floor(sf::Vector2f (screen.x,sizeFloor));
        floor.setPosition(0,heightFloor);
        floor.setFillColor(sf::Color::Green);
        window.draw(floor);


        window.display();
    }
}
