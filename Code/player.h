#pragma once

class Player {
private:

	sf::Texture texture;
	sf::Sprite player_sprite;

	const float jumpForce, gravity;

	bool isJump;

	sf::Font font;
	sf::Text text;

public:
	float x, y;
	sf::Vector2f vel;
	bool dead; // eh...

	int score;

	Player(float x, float y, const float jumpForce, const float gravity, sf::Vector2f vel);

	void setup(void);

	void draw(sf::RenderWindow& window);

	void move(void);

	void gravity_change(void);

	void collision(void);

	void change_pos(void);

	bool check_dead(void);

	bool collide_door(float door_x, float door_y);

	void draw_score(sf::RenderWindow& window);
};
