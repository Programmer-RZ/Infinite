#pragma once

class Tree {
private:
	float pos[20][3];
	int tree_num;
	float w, h;

	sf::Texture texture;
	sf::Sprite tree_sprite;

	
public:
	bool all_trees;

	Tree(void);

	void setup(void);

	void draw(sf::RenderWindow& window, float cx, float cy, int width, int height);

	bool check_collide_player(float px, float py);
};
