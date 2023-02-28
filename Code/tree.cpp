#include <SFML/Graphics.hpp>
#include "global.h"
#include "tree.h"


Tree::Tree(void)
	: w(60), h(100), tree_num(plat_num - 1), all_trees(false)
{
	this->setup();
}

void Tree::setup(void) {
	this->all_trees = false;
	for (int i = 0; i < tree_num; i++) {
		float x = platforms[i][0];
		float y = platforms[i][1];
		float width = platforms[i][2];
		float height = platforms[i][3];

		pos[i][0] = x + width / 2 - this->w / 2;
		pos[i][1] = y - this->h;
		pos[i][2] = false;
	}
}

void Tree::draw(sf::RenderWindow& window, float cx, float cy, int width, int height) {
	int count = 0;
	for (int i = 0; i < tree_num; i++) {
		float x = pos[i][0] + cx;
		float y = pos[i][1] + cy;
		
		if (pos[i][2]) {
			this->texture.loadFromFile("tree.png");
			count++;
		}
		else {
			this->texture.loadFromFile("fire_tree.png");
		}
		
		if (!(x > width || x < -(this->w)) && !(y > height || y < -(this->h))) {
			this->tree_sprite.setTexture(this->texture);
			this->tree_sprite.setPosition(sf::Vector2f(x, y));

			window.draw(this->tree_sprite);
		}
	}
	if (count == this->tree_num) {
		this->all_trees = true;
	}
}

bool Tree::check_collide_player(float px, float py) {
	bool has_hit = false;
	for (int i = 0; i < tree_num; i++) {
		sf::FloatRect player(sf::Vector2f(px, py), sf::Vector2f(50.0f, 70.0f));
		sf::FloatRect tree(sf::Vector2f(pos[i][0], pos[i][1]), sf::Vector2f(this->w, this->h));

		if (tree.intersects(player)) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				bool old_hit = pos[i][2];
				pos[i][2] = true;
				if (!old_hit) {
					has_hit = true;
				}
			}
		}
	}
	return has_hit;
}