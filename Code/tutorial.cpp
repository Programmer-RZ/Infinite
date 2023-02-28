#include <SFML/Graphics.hpp>
#include "tutorial.h"
#include <string>

Tutorial::Tutorial() 
	: index(1), do_tutorial(true)
{
	font.loadFromFile("ThaleahFat.ttf");
	text.setFont(font);

	// introduction texts
	texts[0] = "Hello";
	texts[1] = "I have seen your journey through life";
	texts[2] = "You seem to be a worthy person to take on this task";
	texts[3] = "Because of the BAD company, all trees are on fire";
	texts[4] = "It is your job to water down all the trees";
	texts[5] = "But, I must warn you...";
	texts[6] = "There is a reason why nobody decided to do this...";
	texts[7] = "The BAD company is still setting trees on fire";
	texts[8] = "Your journey will be...";
	texts[9] = "INFINITE";
	texts[10] = "Don't worry, I will guide you on your journey";

	// tutorial texts
	
	texts[11] = "Use arrow keys to move left and right";
	texts[12] = "Press space to water down the tree";
	texts[13] = "Don't fall into the void! Press the up arrow to jump!";
	texts[14] = "Very good. Now reach the door";
	texts[15] = "You have reached the door. You are ready";
	texts[16] = "Press the down arrow to continue";

	// click to continue text
	this->click.setFont(this->font);
	this->click.setOutlineThickness(4);
	this->click.setOutlineColor(sf::Color(0, 0, 0));
	this->click.setCharacterSize(50);
	this->click.setPosition(sf::Vector2f(270, 500));

	this->click.setString("--Click to continue--");
}

void Tutorial::calculatePosition(std::string string) {
	int len;
	int charSize;
	int width;

	std::string text_string;

	text_string = string;

	this->text.setString(text_string);

	len = std::strlen(text_string.c_str());
	charSize = int(960 / len) * 2;

	this->text.setCharacterSize(charSize);

	width = len * charSize;

	this->text.setOutlineThickness(float(charSize / 5));
	this->text.setOutlineColor(sf::Color(0, 0, 0));
	this->text.setPosition(sf::Vector2f(float(960 - width / 2) + charSize / 2, 100));
}

void Tutorial::draw(sf::RenderWindow& window) {
	if (this->index < 11) {
		sf::RectangleShape black_screen;
		black_screen.setFillColor(sf::Color(0, 0, 0));
		black_screen.setSize(sf::Vector2f(960, 640));
		black_screen.setPosition(sf::Vector2f(0, 0));
		window.draw(black_screen);
	}

	this->calculatePosition(texts[index]);
	window.draw(this->text);

	window.draw(this->click);
}

bool Tutorial::update_index(int delay) {
	if (delay > 250) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			this->index += 1;

			if (this->index > 16) {
				this->index = 0;
				this->do_tutorial = false;
			}

			return true;
		}
	}
	return false;
}