#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <random>
#include <deque>

// rng gen
std::mt19937 rng(std::random_device{}());
std::uniform_int_distribution<int> DistY(2, 17);
std::uniform_int_distribution<int> DistX(0, 15);

void centerOrigin(sf::Text& text) {
    sf::FloatRect b = text.getLocalBounds();
    text.setOrigin({
        b.position.x + b.size.x / 2.f,
        b.position.y + b.size.y / 2.f
    });
}

// apple spawner
void spawnApple(sf::RectangleShape &apple, const std::deque<sf::Vector2f> &snakeParts) {
    while (true) {
        int col = DistX(rng);
        int row = DistY(rng);
        sf::Vector2f applePos{col * 50.f, row * 50.f};

        bool occupied = false;
        for (const sf::Vector2f& part : snakeParts) {
            if (part == applePos) {
                occupied = true;
                break;           // leaves the for loop only
            }
        }

        if (!occupied) {
            apple.setPosition(applePos);
            break;               // this one leaves the while loop
        }
    }
}

bool tailCollision(const std::deque<sf::Vector2f> &snakeParts) {
    for (size_t i = 1; i < snakeParts.size(); i++) {
        if (snakeParts.front() == snakeParts[i]){
            return true;
        }
    }
    return false;
}

int main() {
    // SFML setup stuff
    const int WIDTH = 800;
    const int HEIGHT = 900;
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Snake", sf::Style::Close); // create window, first arg is size, 2nd is title
    window.setFramerateLimit(60); // set framerate cap
    window.setKeyRepeatEnabled(false); // disable held key repeating inputs
    sf::Font font("Pixel Digivolve.otf"); // set font
    sf::Text scoreCount(font); // set scoreCount text to use font
    sf::Text title(font); // set title -||-
    sf::Text gameOverText(font); // set gameOverText -||-
    sf::Text finalScoreText(font); // set finalScoreText -||-
    sf::Text restartMessage(font); // set restartMessage -||-
    
    //sfx
    sf::Listener::setGlobalVolume(50.f); // volume, 0 muted - 100 max
    sf::SoundBuffer hit("hit.wav");
    sf::SoundBuffer pickup("pickup.wav");
    sf::Sound hitSound(hit);
    sf::Sound pickupSound(pickup);

    // game variables
    int score = 0;
    bool gameOver = false;
    const sf::Color BG_COLOR {50, 54, 55};

    // text inits
    // scoreCount
    scoreCount.setCharacterSize(24);
    scoreCount.setFillColor(sf::Color::White);
    scoreCount.setStyle(sf::Text::Bold);
    scoreCount.setString("Score: " + std::to_string(score));
    centerOrigin(scoreCount);
    scoreCount.setPosition({100.f, 50.f});

    // title text
    title.setCharacterSize(50);
    title.setFillColor(sf::Color(165, 230, 176));
    title.setStyle(sf::Text::Bold);
    title.setString("SNAKE");
    centerOrigin(title);
    title.setPosition({400.f, 50.f});

    // game over text
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color(255, 0, 0));
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setString("GAME OVER!");
    centerOrigin(gameOverText);
    gameOverText.setPosition({WIDTH/2.f, HEIGHT/2.f - 100});

    // final score
    finalScoreText.setCharacterSize(50);
    finalScoreText.setString("Final score: " + std::to_string(score));
    centerOrigin(finalScoreText);
    finalScoreText.setPosition({WIDTH/2.f, HEIGHT/2.f});

    // restart message
    restartMessage.setCharacterSize(30);
    restartMessage.setFillColor(sf::Color::White);
    restartMessage.setStyle(sf::Text::Bold);
    restartMessage.setString("\"R\" to restart!");
    centerOrigin(restartMessage);
    restartMessage.setPosition({WIDTH/2.f, HEIGHT/2.f + 100});

    // define shapes
    // title bar
    sf::RectangleShape titleBar({800.f, 100.f}); // title bar
    titleBar.setFillColor({10, 94, 24});

    // snake
    sf::RectangleShape snake({50.f, 50.f}); // snake head
    snake.setFillColor({0, 255, 0});
    snake.setOutlineThickness(-2.f);
    snake.setOutlineColor(BG_COLOR);

    // apple
    sf::RectangleShape apple({50.f, 50.f});
    apple.setFillColor({255, 0, 0});
    apple.setOutlineThickness(-2.f);
    apple.setOutlineColor(BG_COLOR);
    
    // snake logic
    const float HEADER = 100.f;
    float snakeXpos = WIDTH/2.f;
    float snakeYpos = WIDTH/2.f;
    const float CELL = 50.f;
    const sf::Vector2f UP{0.f, -CELL};
    const sf::Vector2f DOWN{0.f, CELL};
    const sf::Vector2f LEFT{-CELL, 0.f};
    const sf::Vector2f RIGHT{CELL, 0.f};
    const sf::Vector2f NONE{0.f, 0.f};
    sf::Vector2f currentDirection = NONE;
    std::deque<sf::Vector2f> turnQueue;
    snake.setPosition({snakeXpos, snakeYpos + HEADER});

    // snake parts positioning
    sf::Vector2f snakeHead = {snake.getPosition().x, snake.getPosition().y};
    sf::Vector2f lastPos;
    std::deque<sf::Vector2f> snakeParts = {snakeHead};

    const sf::Time STEP = sf::milliseconds(150); // time per step
    sf::Time accumulator = sf::Time::Zero; // unspent time
    sf::Clock clock;

    //spawn one apple to start the game
    spawnApple(apple, snakeParts);

    // game loop
    while (window.isOpen()) { // run forever as long as the window is open
        accumulator += std::min(clock.restart(), sf::milliseconds(100));
        while (const std::optional event = window.pollEvent()) { // keep running if there is one or more events in the queue
            if (event->is<sf::Event::Closed>()) { // check if the current event is closing the window
                window.close(); // if the above checks out, close the window (quitting the program)
            
            // input stuff
            } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) { //check if any keys are pressed
                sf::Vector2f facing = turnQueue.empty() ? currentDirection : turnQueue.back();
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) { // if key pressed is escape
                    window.close(); // close the window
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::W && facing != DOWN && turnQueue.size() != 2) {
                    turnQueue.push_back(UP);
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::A && facing != RIGHT && turnQueue.size() != 2) {
                    turnQueue.push_back(LEFT);
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::S && facing != UP && turnQueue.size() != 2) {
                    turnQueue.push_back(DOWN);
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::D && facing != LEFT && turnQueue.size() != 2) {
                    turnQueue.push_back(RIGHT);
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::R && gameOver) {
                    // reset score
                    score = 0;

                    // reset accumulator
                    accumulator = sf::Time::Zero;

                    // stop snake movement and spawn new apple
                    currentDirection = NONE;
                    turnQueue.clear();
                    spawnApple(apple, snakeParts);
                    
                    // reset gameOver flag back to false
                    gameOver = false;
                }
            }
        }
        // clear previous frame
        window.clear(sf::Color(BG_COLOR)); // fill entire screen with black
        
        if (!gameOver) {
            // updates
            lastPos = snakeParts.back();
            
            scoreCount.setString("Score: " + std::to_string(score));
            if (accumulator >= STEP) {
                accumulator -= STEP;
                if (turnQueue.size() != 0){
                    currentDirection = turnQueue.front();
                    turnQueue.pop_front();
                }
                sf::Vector2f newHead = snakeParts.front() + currentDirection;
                snakeParts.push_front(newHead);
                snakeParts.pop_back();

                // check for collision/game over
                if (snakeParts.front().x >= WIDTH || 
                    snakeParts.front().x < 0 ||
                    snakeParts.front().y >= HEIGHT || 
                    snakeParts.front().y < 0 + HEADER ||
                    tailCollision(snakeParts)) {
                        finalScoreText.setString("Final score: " + std::to_string(score));
                        snakeParts.clear();
                        snakeParts.push_front({snakeXpos, snakeYpos + HEADER});
                        hitSound.play();
                        gameOver = true;
                } else if (snakeParts.front() == apple.getPosition()) {
                    snakeParts.push_back(lastPos);
                    spawnApple(apple, snakeParts);
                    score += 1;
                    pickupSound.play();
                }
            }

            // rendering stuff
            window.draw(titleBar); //draw the title box on top
            window.draw(title); // draw title text
            window.draw(scoreCount); // show score counter
            for (const sf::Vector2f& part : snakeParts) {
                snake.setPosition(part);
                window.draw(snake);
            }
            window.draw(apple);
        }
        else {
            // render game over screen
            window.draw(gameOverText);
            window.draw(finalScoreText);
            window.draw(restartMessage);
        }
        window.display(); // show newly created frame
    }
}