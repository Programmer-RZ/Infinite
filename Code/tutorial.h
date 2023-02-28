#pragma once

class Tutorial {
private:
	std::string texts[17];

	int state;

	int index;

	sf::Font font;
	sf::Text text;

	sf::Text click;

	void calculatePosition(std::string string);

public:
	bool do_tutorial;

	Tutorial();

	void draw(sf::RenderWindow& window);

	bool update_index(int delay);
};
