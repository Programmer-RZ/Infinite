#include <SFML/Graphics.hpp>
#include <string>
#include "player.h"
#include "global.h"


Player::Player(float x, float y, const float jumpForce, const float gravity, sf::Vector2f vel)
	: x(x), y(y), jumpForce(jumpForce), gravity(gravity), vel(vel), dead(false), score(0)
{
	this->isJump = false;

	this->texture.loadFromFile("player.png");
	this->player_sprite.setTexture(this->texture);

	this->font.loadFromFile("ThaleahFat.ttf");
	this->text.setFont(font);
	this->text.setCharacterSize(50);
	this->text.setPosition(sf::Vector2f(20, 10));
	this->text.setOutlineThickness(4);
	this->text.setOutlineColor(sf::Color(0, 0, 0));

}

void Player::setup(void) {
	this->x = 20.0f;
	this->y = 0.0f;

	this->player_sprite.setPosition(sf::Vector2f(this->x, this->y));
	this->vel.x = 0;
	this->vel.y = 0;
}

void Player::change_pos(void) {
	this->x += this->vel.x;
	this->y += this->vel.y;
}

void Player::draw(sf::RenderWindow& window) {
	this->player_sprite.setPosition(sf::Vector2f(cx + this->x, cy + this->y));
	window.draw(this->player_sprite);
}

void Player::move(void) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && this->isJump == false) {
		this->vel.y = this->jumpForce;
		this->isJump = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		this->vel.x = -6.0;
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		this->vel.x = 6.0;
	}
	else {
		this->vel.x = 0.0;
	}
}

void Player::gravity_change(void) { 
	this->vel.y += this->gravity;

}

void Player::collision(void) {

	// assume that we are not colliding
	this->isJump = true;
	for (int p = 0; p < plat_num; p++) {

		sf::FloatRect platform_rect(sf::Vector2f(platforms[p][0], platforms[p][1]), sf::Vector2f(platforms[p][2], platforms[p][3]));

		// horizontal collision check
		sf::FloatRect player_rect_x(sf::Vector2f(this->vel.x + x, y), sf::Vector2f(30.0, 50.0));

		float x_inc_or_dec = 0;
		if (this->vel.x > 0) {
			x_inc_or_dec = -1;
		}
		else if (this->vel.x < 0) {
			x_inc_or_dec = 1;
		}
		while (player_rect_x.intersects(platform_rect)) {
			this->vel.x += x_inc_or_dec;
			player_rect_x.left = this->vel.x + x;
			if (!player_rect_x.intersects(platform_rect)) {
				break;
			}
		}


		// vertical collision check

		sf::FloatRect player_rect_y(sf::Vector2f(x, this->vel.y + y), sf::Vector2f(30.0, 50.0));

		if (player_rect_y.intersects(platform_rect)) {
			this->isJump = false;
			this->vel.y = 0.0f;
		}


	}
}

bool Player::check_dead(void) {
	if (this->y > 4000) {
		return true;
	}
	else {
		return false;
	}
}

bool Player::collide_door(float door_x, float door_y) {
	sf::FloatRect door(sf::Vector2f(door_x, door_y), sf::Vector2f(50.0f, 70.0f));
	sf::FloatRect player_rect(sf::Vector2f(x, y), sf::Vector2f(30.0, 50.0));

	if (player_rect.intersects(door)) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void Player::draw_score(sf::RenderWindow& window) {
	this->text.setString(std::to_string(this->score));

	window.draw(this->text);
}