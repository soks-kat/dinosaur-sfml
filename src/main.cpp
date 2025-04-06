#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

enum class Difficulty : int {
    easy = 0,
    mid,
    hard,
    length
};

enum class State{
    playing,
    touching,
    dead,
    menu
};

struct Game
{
    float touchingTime=4;
    float maxSpeed=4500;
    float minSpeed=1000;
    float tt;
    State state=State::playing;
    //map
    float rt=0;             //run time
    //cat
    bool touchingCactus=false;        //cat touching cactus
    //jump
    float jt=0;
    bool jumping=false;
    //cactus
    float ct=0;
    std::vector <sf::RectangleShape> cactus={};
    //death sequence
    float timer=100;

    Game (Difficulty difficulty){
        if (difficulty==Difficulty::easy){
            touchingTime=6;
            maxSpeed=3000;
            minSpeed=700;
        //hard
        }else if (difficulty==Difficulty::hard){
            touchingTime=2;
            maxSpeed=6000;
            minSpeed=3000;
        }
        tt=touchingTime;
    }
};

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

    Game g(Difficulty::mid);//change difficulty eventuallly

    //menu
    sf::Font font;
    if (!font.loadFromFile("./epicFont.TTF")) {
        exit(1); //the font file has to be in bin\debug
    }
    //map
    float dt=1.f/144.f;     //time increment
    sf::Vector2u screen = window.getSize();
    float sizeFloor=200;
    float heightFloor=screen.y-sizeFloor;
    //cat
    sf::Vector2f catSize(100,100);
    sf::Color catColor = sf::Color::Blue;
    sf::RectangleShape cat(catSize);
    float catBasePos=heightFloor-catSize.y;
    //jump
    float jumpTime=1.1;
    float jumpHeight=1000;
    //cactus
    float speedScroll;
    //game
    Difficulty difficulty = Difficulty::mid;

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }else if (event.type == sf::Event::Resized){
                sf::FloatRect viewPort(0, 0, event.size.width, event.size.height);
                window. setView(sf::View(viewPort));
                screen=window.getSize();
            }

        }
        window.clear();

        //initializing
        g.rt+=dt;
        bool jump=false;

        switch(g.state){
        case State::playing: {

            //cat jumps
            #pragma region 
            jump=sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
            if (jump){
                g.jumping=true;
            }
            if (g.jumping){
                g.jt+=dt;
                if (g.jt >= jumpTime){
                    g.jumping=false;
                    g.jt=0;
                }
            }

            jump=sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
            if (jump){
                g.jumping=true;
            }
            if (g.jumping){
                g.jt+=dt;
                if (g.jt >= jumpTime){
                    g.jumping=false;
                    g.jt=0;
                }
            }
            #pragma endregion

            //cactus
            #pragma region    
            g.ct-=dt;
            speedScroll=(-1/(1+std::exp(g.rt/3-4))*(g.maxSpeed-g.minSpeed)+g.maxSpeed)*dt;
            //-1/(1+exp(rt/2:stretches so 16s to reach max -4:start at 0))
            if (g.ct < 0){
                sf::Vector2f cactuSize = cactusSize();
                sf::RectangleShape cactus1(cactusSize());
                cactus1.setPosition(screen.x,heightFloor-cactus1.getSize().y);
                g.cactus.push_back(cactus1);
                g.ct=1.2+rand()%11/10;
            }
            for (sf::RectangleShape& e : g.cactus){
                e.setPosition(e.getPosition().x-speedScroll,e.getPosition().y);
                e.setFillColor(sf::Color::Red);
                window.draw(e);
                //collisions
                if (cat.getPosition().x+catSize.x > e.getPosition().x && 
                    cat.getPosition().x < e.getPosition().x+e.getSize().x &&
                    cat.getPosition().y+catSize.y > e.getPosition().y)
                    {
                    catColor = sf::Color::Red;
                    g.touchingCactus=true;
                }
                if (g.touchingCactus && cat.getPosition().x > e.getPosition().x+e.getSize().x){
                    catColor = sf::Color::Blue;
                    g.touchingCactus=false;
                }
            }
            #pragma endregion

            //cat
            cat.setPosition(200,catBasePos+(g.jt*(g.jt-jumpTime))*jumpHeight);
            cat.setFillColor(catColor);
            window.draw(cat);

            //floor
            sf::RectangleShape floor(sf::Vector2f (screen.x,sizeFloor));
            floor.setPosition(0,heightFloor);
            floor.setFillColor(sf::Color::Green);
            window.draw(floor);

            //die
            if (g.touchingCactus){
                g.tt-=1;
            }else{
                g.tt=g.touchingTime;
            }

            if (g.tt <= 0){
                g.state=State::dead;
            }

            break;
        }
        case State::dead: {
            if (g.timer > 0){
                window.clear(sf::Color::Red);
                g.timer-=1;
            }else{
                g.state = State::menu;
            }

            break;
        }
        case State::menu: {
            const float margin = screen.y/16;
            const float buttonHeight = screen.y/4;

            sf::RectangleShape button(sf::Vector2f(screen.x-margin*2, buttonHeight));
            button.setFillColor(sf::Color::Black);

            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(screen.y/8);
            text.setColor(sf::Color::Blue);

            sf::RectangleShape selection(sf::Vector2f(button.getSize().x + 4, button.getSize().y + 4));

            window.clear(sf::Color::Red);
            
            //difficulty selection
            bool upArrow=sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
            bool downArrow=sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

            int temp = (int)difficulty;
            int length = (int)Difficulty::length;
            if (upArrow){
                temp = (temp - 1) % length;
            }else if (downArrow){
                temp = (temp + 1) % length;
            }
            
            selection.setPosition(margin, margin*(temp+1) + buttonHeight*temp);

            button.setPosition(margin, margin); 
            window.draw(button);
            text.setString("Easy");
            text.setOrigin(sf::Vector2f(text.getGlobalBounds().getSize().x, text.getCharacterSize())/2.f + text.getLocalBounds().getPosition());
            text.setPosition(button.getPosition()+button.getSize()/2.f);
            window.draw(text);

            button.setPosition(margin, (margin*2+buttonHeight));
            window.draw(button);
            text.setString("Medium");
            text.setOrigin(sf::Vector2f(text.getGlobalBounds().getSize().x, text.getCharacterSize())/2.f + text.getLocalBounds().getPosition());
            text.setPosition(button.getPosition()+button.getSize()/2.f);
            window.draw(text);
            
            button.setPosition(margin, (margin*3 + 2*buttonHeight));
            window.draw(button);
            text.setString("Hard");
            text.setOrigin(sf::Vector2f(text.getGlobalBounds().getSize().x, text.getCharacterSize())/2.f + text.getLocalBounds().getPosition());
            text.setPosition(button.getPosition()+button.getSize()/2.f);
            window.draw(text);
            


            //restarting
            catColor=sf::Color::Blue;
            // g=Game(Difficulty::mid);
        }
        }
        window.display();
    }
}
