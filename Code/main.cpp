// included libraries
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>

// included files
#include "player.h"
#include "tree.h"
#include "global.h"
#include "tutorial.h"


// define global variables
float platforms[10][4];
int plat_num = 10;

float cx, cy;

// function for drawing platforms

void draw_platforms(sf::RenderWindow& window, float px, float py) {
    sf::RectangleShape platform;
    for (int p = 0; p < plat_num; p++) {
        float x = platforms[p][0] + cx;
        float y = platforms[p][1] + cy;
        float width = platforms[p][2];
        float height = platforms[p][3];

        if (!(x > 960 || x < -width) && !(y > 640 || y < -height)) {
            platform.setFillColor(sf::Color(90, 90, 90));
            platform.setSize(sf::Vector2f(width, height));
            platform.setPosition(sf::Vector2f(x, y));

            window.draw(platform);
        }
    }
}

// function for setting up platforms

void setup_platforms(void) {

    // create previous position variables to store the position of previous platform
    float prev_x = 0, prev_y = 400;

    // create random x, y, size generator
    std::random_device rand;
    std::uniform_int_distribution<int> x(450, 550);
    std::uniform_int_distribution<int> y_decrease(-200, 0);
    std::uniform_int_distribution<int> y_increase(0, 150);
    std::uniform_int_distribution<int> picker(1, 2);
    std::uniform_int_distribution<int> width(300, 400);
    std::uniform_int_distribution<int> height(100, 200);

    // loop through all the platforms and set them a random position and size
    for (int p = 1; p < plat_num; p++) {
        platforms[p][0] = prev_x + x(rand);

        if (picker(rand) == 1) {
            platforms[p][1] = prev_y + -(y_decrease(rand));
        }
        else {
            platforms[p][1] = prev_y + -(y_increase(rand));
        }

        platforms[p][2] = float(width(rand));
        platforms[p][3] = float(height(rand));

        prev_x = platforms[p][0];
        prev_y = platforms[p][1];
    }

    // setup starting platform
    platforms[0][0] = 0;
    platforms[0][1] = 400;
    platforms[0][2] = float(width(rand));
    platforms[0][3] = float(height(rand));

}

// function for setting up the door
void setup_door(sf::RectangleShape& door, float& door_x, float& door_y) {
    door_x = platforms[plat_num - 1][0] + platforms[plat_num - 1][2] / 2 - 25;
    door_y = platforms[plat_num - 1][1] - 70;
    
    door.setPosition(sf::Vector2f(door_x + cx, door_y + cy));
    door.setSize(sf::Vector2f(50.0f, 70.0f));
    door.setFillColor(sf::Color(0, 0, 0));
}

// function for the fade effect
bool fadeOut(sf::RenderWindow& window, sf::Clock timer) {
    sf::RectangleShape fadeRect;
    fadeRect.setSize(sf::Vector2f(960, 640));
    fadeRect.setPosition(sf::Vector2f(0, 0));

    int startAlpha = 0;
    int endAlpha = 255;
    int targetTime = 3000;

    int currentTime = timer.getElapsedTime().asMilliseconds();
    int currentAlpha = endAlpha;
    if (currentTime >= targetTime)
    {
        return false;
    }
    else
    {
        currentAlpha = startAlpha + (endAlpha - startAlpha) * currentTime / targetTime;
    }
    fadeRect.setFillColor(sf::Color(0, 0, 0, currentAlpha));
    window.draw(fadeRect);
    return true;
}

// main function
int main()
{
    // setup all the local variables
    sf::RenderWindow window(sf::VideoMode(960, 640), "Geo_CodeHero");
    window.setFramerateLimit(60);

    // player
    Player* player = new Player(20.0f, 0.0f, -14.0f, 0.5f, sf::Vector2f(0.0, 0.0));
    player->setup();

    // camera
    cx = (450 - player->x);
    cy = (270 - player->y);

    // platforms
    setup_platforms();

    // door
    sf::RectangleShape door;
    float door_x, door_y;
    setup_door(door, door_x, door_y);

    // trees
    Tree* tree = new Tree();

    // tutorial
    Tutorial* tutorial = new Tutorial();

    // fade
    bool fade = false;
    sf::Clock timer;

    // audio
    sf::SoundBuffer point;
    point.loadFromFile("point.wav");

    sf::SoundBuffer lose;
    lose.loadFromFile("negative_beeps-6008.wav");

    sf::SoundBuffer respawn;
    respawn.loadFromFile("respawn.wav");

    sf::SoundBuffer finish;
    finish.loadFromFile("finish.wav");

    sf::Sound sound;

    // background
    sf::Texture background;
    background.loadFromFile("background_game.png");

    sf::Sprite background_s;
    background_s.setTexture(background);

    // theme music
    sf::Music theme;
    theme.openFromFile("theme.wav");
    theme.setVolume(2.0f);
    theme.setLoop(true);
    theme.play();


    // main loop
    while (window.isOpen())
    {
        // check for any input events
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
            case(sf::Event::Closed):
                window.close();
            }
        }

        window.clear();

        background_s.setPosition(sf::Vector2f(-130 + cx / 10, -60 + cy / 10));
        window.draw(background_s);

        // door
        if (!fade) {
            player->move();
            player->gravity_change();
            player->collision();
            player->change_pos();

            if (tree->check_collide_player(player->x, player->y)) {
                sound.setBuffer(point);
                sound.setVolume(10.0f);
                sound.play();

                player->score += 1;
            }

            if (player->collide_door(door_x, door_y) && tree->all_trees) {
                sound.setBuffer(finish);
                sound.setVolume(2.5f);
                sound.play();

                fade = true;
                tutorial->do_tutorial = false;
                timer.restart();
            }

            if (!player->dead) {
                // lerp equation for smooth camera effect
                cx = float(cx + (450 - player->x - cx) * 0.1);
                cy = float(cy + (270 - player->y - cy) * 0.1);
            }
            else {
                if (timer.getElapsedTime().asMilliseconds() >= 1500) {
                    player->dead = false;
                    sound.setBuffer(respawn);
                    sound.setVolume(1.0f);
                    sound.play();
                }
            }

            if (player->check_dead()) {
                player->setup();

                sound.setBuffer(lose);
                sound.setVolume(5.0f);
                sound.play();

                player->dead = true;
                player->score -= 1;

                timer.restart();
            }
        }

        door.setPosition(sf::Vector2f(door_x + cx, door_y + cy));
        tree->draw(window, cx, cy, 960, 640);
        window.draw(door);
        draw_platforms(window, player->x, player->y);
        player->draw(window);
        player->draw_score(window);

        // tutorial
        if (tutorial->do_tutorial) {
            if (tutorial->update_index(timer.getElapsedTime().asMilliseconds())) {
                timer.restart();
            }
        }
        if (tutorial->do_tutorial) {
            tutorial->draw(window);
        }


        if (fade) {

            if (!fadeOut(window, timer)) {
                timer.restart();
                fade = false;

                setup_platforms();

                setup_door(door, door_x, door_y);

                tree->setup();

                player->setup();

                cx = float(cx + (450 - player->x - cx) * 1);
                cy = float(cy + (270 - player->y - cy) * 1);

                sound.setBuffer(respawn);
                sound.setVolume(1.0f);
                sound.play();
            }
        }

        window.display();
    }
    delete player;
    delete tree;
    delete tutorial;

    return 0;
}